#include "VSThread.h"
using namespace VSEngine2;


//------------------------------------------------------------------------------
DWORD THREAD_CALLBACK VSThread::ThreadProc(void* t)
{
	VSThread* pThread = (VSThread*)(t);
	SetThreadName(pThread->GetThreadName());
	pThread->Run();
	return 0;
}

//------------------------------------------------------------------------------
VSThread::VSThread()
	: m_hThread(NULL)
	, m_priority(Normal)
	, m_stackSize(0)
{
	VSMAC_ASSERT(!IsRunning());
	m_hThread = ::CreateThread(0, m_stackSize, ThreadProc, this, CREATE_SUSPENDED, NULL);
	VSMAC_ASSERT(m_hThread);
	m_ThreadState = TS_SUSPEND;
	SetPriority(m_priority);
	m_StopEvent.Create(true);
	m_StopEvent.Reset();
}

//------------------------------------------------------------------------------
VSThread::~VSThread()
{
	if (IsRunning())
	{
		// force to exit
		TerminateThread(m_hThread, 0);
	}
}

//------------------------------------------------------------------------------
void VSThread::SetPriority(Priority p)
{
	int nPriority = THREAD_PRIORITY_NORMAL;

	if (p == Low)
		nPriority = THREAD_PRIORITY_BELOW_NORMAL;
	else if (p == Normal)
		nPriority = THREAD_PRIORITY_NORMAL;
	else if (p == High)
		nPriority = THREAD_PRIORITY_ABOVE_NORMAL;

	::SetThreadPriority(m_hThread, nPriority);
}

//------------------------------------------------------------------------------
void VSThread::Start()
{	
	if (m_ThreadState == TS_SUSPEND)
	{
		ResumeThread((HANDLE)m_hThread);
		m_ThreadState = TS_START;
	}
}
//------------------------------------------------------------------------------
void VSThread::Suspend()
{
	if (m_ThreadState == TS_START)
	{
		SuspendThread((HANDLE)m_hThread);
		m_ThreadState = TS_SUSPEND;
	}
	
}
void VSThread::Sleep(DWORD dwMillseconds)
{
	if (m_ThreadState == TS_START)
	{
		::Sleep(dwMillseconds);
	}
}
//------------------------------------------------------------------------------
bool VSThread::IsRunning() const
{
	if (NULL != m_hThread)
	{
		DWORD exitCode = 0;
		if (GetExitCodeThread(m_hThread, &exitCode))
		{
			if (STILL_ACTIVE == exitCode)
			{
				return true;
			}
		}
	}

	return false;
}

//------------------------------------------------------------------------------
void VSThread::SetThreadName(const TCHAR* name)
{
	// update the Windows thread name so that it shows up correctly
	// in the Debugger
	struct THREADNAME_INFO
	{
		DWORD dwType;     // must be 0x1000
		LPCSTR szName;    // pointer to name (in user address space)
		DWORD dwThreadID; // thread ID (-1 = caller thread)
		DWORD dwFlags;    // reserved for future use, must be zero
	};

	THREADNAME_INFO info;
	info.dwType     = 0x1000;
	info.szName     = name;
	info.dwThreadID = ::GetCurrentThreadId();
	info.dwFlags    = 0;
	__try
	{
		RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (DWORD*)&info);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}

//------------------------------------------------------------------------------
const TCHAR* VSThread::GetThreadName()
{
	return _T("VSThread");
}
bool VSThread::IsStopTrigger()
{
	return m_StopEvent.IsTrigger();
}
//------------------------------------------------------------------------------
void VSThread::Stop()
{
	if (m_ThreadState == TS_START)
	{
		VSMAC_ASSERT(this->IsRunning());
		VSMAC_ASSERT(NULL != m_hThread);

		m_StopEvent.Trigger();
		m_ThreadState = TS_STOP;
		// wait for the thread to terminate
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}