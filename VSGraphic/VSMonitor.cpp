#include "VSMonitor.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
#ifdef WINDOWS
//------------------------------------------------------------------------------
VSFileMonitor::VSFileMonitor(VSMonitor * pOwner)
: m_isUseRelativePath(false)
, m_pReadBuffer(NULL)
{
	m_pOwner = pOwner;
}

//------------------------------------------------------------------------------
VSFileMonitor::~VSFileMonitor()
{
}

//------------------------------------------------------------------------------
void
VSFileMonitor::Run()
{
	HRESULT hr = S_OK;
	OVERLAPPED overlapped = { 0 };
	DWORD dwErr = 0;

	const DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
	const DWORD dwFlags = FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED;

	const DWORD dwNotifyFilter = FILE_NOTIFY_CHANGE_FILE_NAME |
		FILE_NOTIFY_CHANGE_LAST_WRITE;

	// must be DWORD-aligned, don't use MAY_NEW!
	m_pReadBuffer = VS_NEW unsigned char[BufferSize];//(uchar*)HeapAlloc(GetProcessHeap(), 0, BufferSize);

	// Open directory handle
	HANDLE hDir = ::CreateFile(m_directory.GetBuffer(), GENERIC_READ,
		dwShareMode, NULL, OPEN_EXISTING, dwFlags, NULL);

	if (INVALID_HANDLE_VALUE == hDir)
	{
		dwErr = ::GetLastError();
		hr = HRESULT_FROM_WIN32(dwErr);
	}

	// Set up the OVERLAPPED structure for receiving async notifications
	VSEvent ioEvent;
	ioEvent.Create(true);
	overlapped.hEvent = (HANDLE)ioEvent.GetHandle();

	VSSynchronize * events[2];
 	events[0] = &ioEvent;
 	events[1] = &m_StopEvent;

	// Loop for change notifications
	while (S_OK == hr)
	{
		DWORD dwBytesReturned = 0;

		FILE_NOTIFY_INFORMATION* pInfo = (FILE_NOTIFY_INFORMATION*)m_pReadBuffer;
		VSMemset(m_pReadBuffer, 0,BufferSize);
		// Get information that describes the most recent file change
		// This call will return immediately and will set overlapped.hEvent when it has completed
		if (0 == ::ReadDirectoryChangesW(hDir, pInfo, BufferSize, TRUE,
			dwNotifyFilter, &dwBytesReturned, &overlapped, NULL))
		{
			dwErr = ::GetLastError();
			hr = HRESULT_FROM_WIN32(dwErr);
		}

		// Set up a wait on the overlapped handle and on the handle used to shut down this thread
		if (S_OK == hr)
		{
			VSSynchronize::WaitAll(events,2,false);

			if (IsStopTrigger())
			{
				// Stop was called, and this thread should be shut down
				hr = S_FALSE;
			}
			// Otherwise, a change notification occured, and we should continue through the loop
		}

		// Retrieve result from overlapped structure
		if (S_OK == hr)
		{
			DWORD dwBytesTransferred = 0;

			if (0 == ::GetOverlappedResult(hDir, &overlapped,
				&dwBytesTransferred, TRUE))
			{
				dwErr = ::GetLastError();
				hr = HRESULT_FROM_WIN32(dwErr);
			}
		}

		if (S_OK == hr)
		{
			this->NotifyHandler();
		}
	}// while

	if (NULL != m_pReadBuffer)
	{
		VSMAC_DELETEA(m_pReadBuffer);
	}

	CloseHandle(hDir);
}

//------------------------------------------------------------------------------
void
VSFileMonitor::NotifyHandler()
{
	FILE_NOTIFY_INFORMATION* pInfo = (FILE_NOTIFY_INFORMATION*)m_pReadBuffer;
	// Notify FileChange events
	bool hasNext = false;
	do
	{
		char szPath[MAX_PATH] = {0};
		// Copy the file name into a message structure
		VSWcsToMbs(szPath,MAX_PATH,pInfo->FileName, pInfo->FileNameLength / sizeof(WCHAR));
		VSString path = szPath;

		if (!m_isUseRelativePath)
		{
			path = m_directory + path;
		}
		FileChangedArgs args;
		args.FilePath = path;
		switch (pInfo->Action)
		{
		case FILE_ACTION_ADDED:
		case FILE_ACTION_RENAMED_NEW_NAME:
			args.Operation = FileOperation::Added;
			break;
		case FILE_ACTION_MODIFIED:
			args.Operation = FileOperation::Modified;
			break;
		case FILE_ACTION_REMOVED:
		case FILE_ACTION_RENAMED_OLD_NAME:
			args.Operation = FileOperation::Removed;
			break;
		}
		m_pOwner->OnFileChanged(args);

		hasNext = (pInfo->NextEntryOffset > 0);
		pInfo = (PFILE_NOTIFY_INFORMATION)((unsigned char*)pInfo + pInfo->NextEntryOffset);
	}
	while (hasNext);
}

//------------------------------------------------------------------------------
const TCHAR*
VSFileMonitor::GetThreadName()
{
	return _T("VSFileMonitorThread");
}
//------------------------------------------------------------------------------
VSResourceMonitor * VSResourceMonitor::ms_pResourceMonitor = NULL;
VSResourceMonitor::VSResourceMonitor()
:m_fileMonitor(NULL)
{
	VSMAC_ASSERT(!ms_pResourceMonitor);
	ms_pResourceMonitor = this;
}

//------------------------------------------------------------------------------
VSResourceMonitor::~VSResourceMonitor()
{

	for (unsigned int  i = 0; i < m_handlers.GetNum(); ++i)
	{
		VSMAC_DELETE(m_handlers[i]);
	}
	m_handlers.Clear();
}

//------------------------------------------------------------------------------
bool
VSResourceMonitor::Open()
{
	m_fileMonitor = VS_NEW VSFileMonitor(this);
	m_fileMonitor->SetDirectory(VSResourceManager::ms_ResourcePath);
	m_fileMonitor->Start();

	return true;
}

//------------------------------------------------------------------------------
void
VSResourceMonitor::Update(double tick)
{
	static double LastTime = tick;
	if (tick - LastTime < 1000)
	{
		return;
	}
	LastTime = tick;
	if (m_changedFiles.IsEmpty())
	{
		return;
	}
	// remove redundant files
	VSArray<FileChangedArgs> files;
	while (m_changedFiles.IsEmpty())
	{
		FileChangedArgs file; 
		m_changedFiles.Dequeue(file);
		files.AddElement(file);
	}
	// handler event
	for (unsigned int i = 0 ; i < files.GetNum(); i++)
	{
		const FileChangedArgs& file = files[i];
		for (unsigned int j = 0; j < m_handlers.GetNum(); ++j)
		{
			if (m_handlers[j]->CheckFileType(file.FilePath))
				m_handlers[j]->OnFileChanged(file);
		}
	}
}

//------------------------------------------------------------------------------
void
VSResourceMonitor::Close()
{
	m_fileMonitor->Stop();
	VSMAC_DELETE(m_fileMonitor);
}

//------------------------------------------------------------------------------
void
VSResourceMonitor::RegisterHandler(VSResourceChangedHandler* handler)
{
	m_handlers.AddElement(handler);
}

//------------------------------------------------------------------------------
void
VSResourceMonitor::UnregisterHandler(VSResourceChangedHandler* handler)
{
	unsigned int index = m_handlers.FindElement(handler);
	m_handlers.Erase(index);
}

//------------------------------------------------------------------------------
void
VSResourceMonitor::OnFileChanged(const FileChangedArgs& args)
{
	m_changedFiles.Enqueue(args);
}
#endif