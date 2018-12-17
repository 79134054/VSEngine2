#include"VSMemManager.h"
#include "VSSystem.h"
#include <DbgHelp.h>
// #define _CRTDBG_MAP_ALLOC
// #include <crtdbg.h>
namespace VSEngine2
{
VSCriticalSection VSMemWin32::ms_MemLock;
VSCriticalSection VSDebugMem::ms_MemLock;
// char *  VSMemManagerSF::ms_pMemHeapPtr = NULL;
// VSMemManagerSF::BlockHeader* VSMemManagerSF::ms_pFreeList = NULL;
// unsigned int VSMemManagerSF::ms_uiSize = 0;
// void VSMemManagerSF::CreateMemMangerSF(unsigned int uiSize)
// {
// 	ms_uiSize = ms_uiSize;
// 	ms_pMemHeapPtr =(char *) malloc(ms_uiSize);
// 	VSMAC_ASSERT(ms_pMemHeapPtr);
// 	ms_pFreeList = (BlockHeader *)ms_pMemHeapPtr;
// 	ms_pFreeList->m_bUsed = false;
// 	ms_pFreeList->m_pcNext = ms_pFreeList;
// 	ms_pFreeList->m_pcPrev = ms_pFreeList;
// 	ms_pFreeList->ms_uiSize = ms_uiSize - HEADERSIZE - ENDERSIZE;
// 
// 	BlockEnder* pTemp =(BlockEnder*) ((unsigned int)ms_pFreeList + ms_uiSize - ENDERSIZE);
// 	pTemp->m_bUsed = false;
// 	pTemp->ms_uiSize = ms_pFreeList->ms_uiSize;
// }
// void VSMemManagerSF::DeleteMemMangerSF()
// {
// 	if(ms_pMemHeapPtr)
// 		free(ms_pMemHeapPtr);	
// 
// }
// void *VSMemManagerSF::SearchByPolicy(unsigned int uiRequestedSize)
// {
// 	if(!ms_pFreeList)
// 		return NULL;
// 	BlockHeader * pTemp = ms_pFreeList;
// 	BlockHeader * pReturn = NULL;
// 	unsigned int uiSize = 0xffffffff;
// 
// 	do 
// 	{
// 		if(pTemp->ms_uiSize >= uiRequestedSize)
// 		{
// 			if(uiSize > pTemp->ms_uiSize)
// 			{
// 				uiSize = pTemp->ms_uiSize;
// 				pReturn = ms_pFreeList;
// 			}
// 		}
// 
// 		pTemp = pTemp->m_pcNext;
// 
// 	} while (pTemp != ms_pFreeList);
// 
// 	return pReturn;
// 
// }
// void *VSMemManagerSF::Allocate(unsigned int uiRequestedSize)
// {
// 	if(uiRequestedSize > ms_uiSize - HEADERSIZE - ENDERSIZE)
// 		return NULL;
// 	BlockHeader * pHeader	= (BlockHeader *)SearchByPolicy(uiRequestedSize);
// 	if(!pHeader)
// 		return NULL;
// 	BlockEnder *pEnder =(BlockEnder *)( (unsigned int)pHeader + HEADERSIZE + pHeader->ms_uiSize);
// 
// 	if(pHeader->ms_uiSize <= uiRequestedSize + HEADERSIZE + ENDERSIZE)
// 	{
// 
// 		pHeader->m_bUsed = true;
// 		pEnder->m_bUsed = true;
// 		if(pHeader->m_pcNext == pHeader)
// 		{
// 			ms_pFreeList = NULL;
// 
// 		}
// 		else
// 		{
// 			pHeader->m_pcNext->m_pcPrev = pHeader->m_pcPrev;
// 			pHeader->m_pcPrev->m_pcNext = pHeader->m_pcNext;
// 			if(pHeader == ms_pFreeList)
// 			{
// 				ms_pFreeList = pHeader->m_pcNext;
// 
// 			}
// 		}
// 
// 	}
// 	else
// 	{
// 		BlockEnder *PNewEnder = pEnder;
// 		pEnder = (BlockEnder *)((unsigned int )pHeader + uiRequestedSize + HEADERSIZE);
// 		BlockHeader * pNewHeader =(BlockHeader *)((unsigned int )pEnder + ENDERSIZE);
// 
// 		pNewHeader->m_bUsed = false;
// 		pNewHeader->ms_uiSize = pHeader->ms_uiSize - uiRequestedSize - HEADERSIZE - ENDERSIZE;
// 		PNewEnder->m_bUsed = false;
// 		PNewEnder->ms_uiSize =  pNewHeader->ms_uiSize;
// 
// 		pHeader->m_bUsed = true;
// 		pHeader->ms_uiSize = uiRequestedSize;		
// 		pEnder->m_bUsed =true;
// 		pEnder->ms_uiSize = uiRequestedSize;
// 
// 		pHeader->m_pcPrev->m_pcNext = pNewHeader;
// 		pNewHeader->m_pcPrev = pHeader->m_pcPrev;
// 		pHeader->m_pcNext->m_pcPrev = pNewHeader;
// 		pNewHeader->m_pcNext = pHeader->m_pcNext;
// 
// 		/*如果只剩下一个块，分成2块，pNewHeader块两个指针都要指向自己,
// 		pHeader->m_pcPrev 等于 pHeader
// 		pHeader->m_pcPrev->m_pcNext = pNewHeader 就是pHeader->m_pcNext = pNewHeader;
// 
// 		pHeader->m_pcNext->m_pcPrev = pNewHeader; 相当于pNewHeader->m_pcPrev = pNewHeader;
// 
// 		pNewHeader->m_pcNext = pHeader->m_pcNext;相当于 pNewHeader->m_pcNext = pNewHeader;
// 		*/
// 
// 		if(pHeader == ms_pFreeList)
// 		{
// 			ms_pFreeList = pNewHeader;   
// 
// 		}
// 
// 
// 	}
// 
// 	return (void *)((unsigned int)pHeader + HEADERSIZE);	
// }
// void VSMemManagerSF::Deallocate(void * pcDelete)
// {
// 	BlockHeader * pHeader =(BlockHeader *)((unsigned int )pcDelete - HEADERSIZE);
// 
// 	BlockEnder *pEnder = (BlockEnder *)((unsigned int)pHeader + HEADERSIZE + pHeader->ms_uiSize);
// 
// 	VSMAC_ASSERT(pHeader->m_bUsed == pEnder->m_bUsed);
// 	VSMAC_ASSERT(pEnder->ms_uiSize == pHeader->ms_uiSize);
// 	BlockHeader * pLeftHeader = NULL;
// 	BlockEnder * pLeftEnder = NULL;
// 
// 	BlockHeader * pRightHeader = NULL;
// 	BlockEnder * pRightEnder = NULL;
// 
// 	if((char *)pHeader != ms_pMemHeapPtr)
// 	{
// 		pLeftEnder = (BlockEnder *)((unsigned int )pHeader - ENDERSIZE);
// 		pLeftHeader = (BlockHeader *)((unsigned int )pLeftEnder - pLeftEnder->ms_uiSize - HEADERSIZE);
// 	}
// 
// 	if(((unsigned int )pEnder + ENDERSIZE) != ((unsigned int )ms_pMemHeapPtr + ms_uiSize))
// 	{
// 		pRightHeader = (BlockHeader *)((unsigned int )pHeader + pHeader->ms_uiSize + HEADERSIZE + ENDERSIZE);
// 		pRightEnder = (BlockEnder *)((unsigned int)pRightHeader + HEADERSIZE + pHeader->ms_uiSize);
// 
// 	}
// 	if(pLeftHeader != NULL)
// 	{
// 		if(pLeftHeader->m_bUsed == false)
// 		{
// 			//只有一个块
// 			if(pLeftHeader->m_pcNext == pLeftHeader)
// 			{
// 				ms_pFreeList = NULL;
// 
// 			}
// 			else
// 			{
// 				pLeftHeader->m_pcPrev->m_pcNext = pLeftHeader->m_pcNext;
// 				pLeftHeader->m_pcNext->m_pcPrev = pLeftHeader->m_pcPrev;
// 			}
// 
// 
// 			pLeftHeader->ms_uiSize = pLeftHeader->ms_uiSize + pHeader->ms_uiSize + HEADERSIZE + ENDERSIZE;
// 			pHeader = pLeftHeader;
// 
// 			pEnder->ms_uiSize = pHeader->ms_uiSize;
// 
// 		}
// 	}
// 
// 	if(pRightHeader !=NULL)
// 	{
// 		if(pRightHeader->m_bUsed == false)
// 		{
// 			if(pRightHeader->m_pcNext == pRightHeader)
// 			{
// 				ms_pFreeList = NULL;
// 
// 			}
// 			else
// 			{
// 				pRightHeader->m_pcNext->m_pcPrev = pRightHeader->m_pcPrev;
// 				pRightHeader->m_pcPrev->m_pcNext = pRightHeader->m_pcNext;
// 			}
// 
// 			//pHeader->m_bUsed = false;
// 			pHeader->ms_uiSize = pHeader->ms_uiSize + pRightHeader->ms_uiSize + HEADERSIZE + ENDERSIZE;
// 			pEnder = pRightEnder;
// 			//pRightEnder->m_bUsed = false;
// 			pRightEnder->ms_uiSize = pHeader->ms_uiSize;
// 		}
// 
// 	}
// 	pHeader->m_bUsed = false;
// 	pEnder->m_bUsed = false;
// 	if(ms_pFreeList)
// 	{
// 		pHeader->m_pcNext = ms_pFreeList;
// 		pHeader->m_pcPrev = ms_pFreeList->m_pcPrev;
// 		ms_pFreeList->m_pcPrev->m_pcNext = pHeader;
// 		ms_pFreeList->m_pcPrev = pHeader;
// 		ms_pFreeList = pHeader;
// 
// 
// 	}
// 	else
// 	{
// 		ms_pFreeList = pHeader;
// 		pHeader->m_pcPrev = ms_pFreeList;
// 		pHeader->m_pcNext = ms_pFreeList;
// 	}
// }/

typedef BOOL
(WINAPI
 *tFMiniDumpWriteDump)
 (
 IN HANDLE hProcess,
 IN DWORD ProcessId,
 IN HANDLE hFile,
 IN MINIDUMP_TYPE DumpType,
 IN CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, OPTIONAL
 IN CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, OPTIONAL
 IN CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL
 );

typedef BOOL
(WINAPI
 *tFSymInitialize)
 (
 IN HANDLE   hProcess,
 IN PSTR     UserSearchPath,
 IN BOOL     fInvadeProcess
 );

typedef BOOL
(WINAPI
 *tFStackWalk64)
 (
 DWORD                             MachineType,
 HANDLE                            hProcess,
 HANDLE                            hThread,
 LPSTACKFRAME64                    StackFrame,
 PVOID                             ContextRecord,
 PREAD_PROCESS_MEMORY_ROUTINE64    ReadMemoryRoutine,
 PFUNCTION_TABLE_ACCESS_ROUTINE64  FunctionTableAccessRoutine,
 PGET_MODULE_BASE_ROUTINE64        GetModuleBaseRoutine,
 PTRANSLATE_ADDRESS_ROUTINE64      TranslateAddress
 );

typedef BOOL
(WINAPI
 *tFSymFromAddr)
 (
 IN  HANDLE              hProcess,
 IN  DWORD64             Address,
 OUT PDWORD64            Displacement,
 IN OUT PSYMBOL_INFO     Symbol
 );

typedef BOOL
(WINAPI
 *tFSymGetLineFromAddr64)
 (
 IN  HANDLE                  hProcess,
 IN  DWORD64                 qwAddr,
 OUT PDWORD                  pdwDisplacement,
 OUT PIMAGEHLP_LINE64        Line64
 );

typedef DWORD
(WINAPI
 *tFSymGetOptions)
 (
 VOID
 );

typedef DWORD
(WINAPI
 *tFSymSetOptions)
 (
 IN DWORD   SymOptions
 );

typedef PVOID
(WINAPI
 *tFSymFunctionTableAccess64)
 (
 HANDLE  hProcess,
 DWORD64 AddrBase
 );

typedef DWORD64
(WINAPI
 *tFSymGetModuleBase64)
 (
 IN  HANDLE              hProcess,
 IN  DWORD64             qwAddr
 );

static tFMiniDumpWriteDump fnMiniDumpWriteDump = NULL;
static tFSymInitialize fnSymInitialize = NULL;
static tFStackWalk64 fnStackWalk64 = NULL;
static tFSymFromAddr fnSymFromAddr = NULL;
static tFSymGetLineFromAddr64 fnSymGetLineFromAddr64 = NULL;
static tFSymGetOptions fnSymGetOptions = NULL;
static tFSymSetOptions fnSymSetOptions = NULL;
static tFSymFunctionTableAccess64 fnSymFunctionTableAccess64 = NULL;
static tFSymGetModuleBase64 fnSymGetModuleBase64 = NULL;

static HMODULE s_DbgHelpLib = NULL;
static HANDLE s_Process = NULL;

bool VSDebugMem::InitDbgHelpLib()
{
	TCHAR szDbgName[MAX_PATH];
	GetModuleFileName(NULL, szDbgName, MAX_PATH);
	TCHAR *p = (TCHAR *)VSCsrchr(szDbgName, _T('\\'));
	if(p)
		*p = 0;
	VSStrcat(szDbgName,MAX_PATH,_T("\\dbghelp.dll"));

	// 查找当前目录的DLL
	s_DbgHelpLib = LoadLibrary(szDbgName);
	if(s_DbgHelpLib == NULL)
	{
		// 使用系统的DLL
		s_DbgHelpLib = LoadLibrary(_T("dbghelp.dll"));
		if(s_DbgHelpLib == NULL)
			return false;
	}

	fnMiniDumpWriteDump = (tFMiniDumpWriteDump)GetProcAddress(s_DbgHelpLib, "MiniDumpWriteDump");
	fnSymInitialize = (tFSymInitialize)GetProcAddress(s_DbgHelpLib, "SymInitialize");
	fnStackWalk64 = (tFStackWalk64)GetProcAddress(s_DbgHelpLib, "StackWalk64");
	fnSymFromAddr = (tFSymFromAddr)GetProcAddress(s_DbgHelpLib, "SymFromAddr");
	fnSymGetLineFromAddr64 = (tFSymGetLineFromAddr64)GetProcAddress(s_DbgHelpLib, "SymGetLineFromAddr64");
	fnSymGetOptions = (tFSymGetOptions)GetProcAddress(s_DbgHelpLib, "SymGetOptions");
	fnSymSetOptions = (tFSymSetOptions)GetProcAddress(s_DbgHelpLib, "SymSetOptions");
	fnSymFunctionTableAccess64 = (tFSymFunctionTableAccess64)GetProcAddress(s_DbgHelpLib, "SymFunctionTableAccess64");
	fnSymGetModuleBase64 = (tFSymGetModuleBase64)GetProcAddress(s_DbgHelpLib, "SymGetModuleBase64");

	if(fnMiniDumpWriteDump &&
		fnSymInitialize &&
		fnStackWalk64 &&
		fnSymFromAddr &&
		fnSymGetLineFromAddr64 &&
		fnSymGetOptions &&
		fnSymSetOptions &&
		fnSymFunctionTableAccess64 &&
		fnSymGetModuleBase64)
	{
		DWORD ProcessID = GetCurrentProcessId();
		s_Process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
		fnSymInitialize(s_Process, ".", TRUE);
		return true;
	}
	return false;
}

void VSDebugMem::FreeDbgHelpLib()
{
	if(s_DbgHelpLib != NULL)
	{
		FreeLibrary(s_DbgHelpLib);
		CloseHandle(s_Process);
		s_Process = NULL;
		s_DbgHelpLib = NULL;
	}

	fnMiniDumpWriteDump = NULL;
	fnSymInitialize = NULL;
	fnStackWalk64 = NULL;
	fnSymFromAddr = NULL;
	fnSymGetLineFromAddr64 = NULL;
	fnSymGetOptions = NULL;
	fnSymSetOptions = NULL;
	fnSymFunctionTableAccess64 = NULL;
	fnSymGetModuleBase64 = NULL;
}

bool VSDebugMem::GetFileAndLine(const void *pAddress, TCHAR szFile[MAX_PATH], int &line)
{

	IMAGEHLP_LINE64 Line;
	Line.SizeOfStruct = sizeof(Line);
	VSMemset(&Line, 0, sizeof(Line));
	DWORD Offset = 0;

	if(fnSymGetLineFromAddr64(s_Process, (DWORD64)pAddress, &Offset, &Line))
	{
#ifdef  _UNICODE
	VSMbsToWcs(szFile,MAX_PATH,Line.FileName,MAX_PATH);
#else
	VSStrCopy(szFile, MAX_PATH,Line.FileName);
#endif
	line = Line.LineNumber;

		return true;
	}
	else
	{
		DWORD error = GetLastError();
// 		if (error == 487)
// 		{
// 			VSOutPutDebugString(_T("No debug info in current module\n"));
// 		}
// 		else if (error == 126)
// 		{
// 			VSOutPutDebugString(_T("Debug info in current module has not loaded\n"));
// 		}
// 		else 
// 		{
// 			VSOutPutDebugString(_T("SymGetLineFromAddr64 failed\n"));
// 		}
		return false;
	}
}
VSMemManager::VSMemManager()
{

}
VSMemManager::~VSMemManager()
{

}
VSDebugMem::VSDebugMem()
{
	m_uiNumNewCalls = 0;
	m_uiNumDeleteCalls = 0;

	m_uiNumBlocks = 0;
	m_uiNumBytes = 0;
	m_uiMaxNumBytes = 0;
	m_uiMaxNumBlocks = 0;

	m_pHead = 0;
	m_pTail = 0;

	for (unsigned int i = 0 ; i < RECORD_NUM ;i++)
	{
		m_uiSizeRecord[i] = 0;
	}
}
VSDebugMem::~VSDebugMem()
{
// 	int temp = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
// 	temp |= _CRTDBG_CHECK_ALWAYS_DF;
// 	temp |= _CRTDBG_DELAY_FREE_MEM_DF;
// 	_CrtSetDbgFlag(temp);
	InitDbgHelpLib();
	PrintInfo();
	FreeDbgHelpLib();
	FreeLeakMem();
}
void* VSDebugMem::Allocate (unsigned int uiSize,unsigned int uiAlignment,bool bIsArray)
{

	ms_MemLock.Lock();
	if (!uiSize)
	{
		ms_MemLock.Unlock();
		return NULL;
	}
	m_uiNumNewCalls++;
// 	_CrtMemState MetState;
// 	_CrtMemCheckpoint(&MetState);
	unsigned int uiExtendedSize = sizeof(Block)+ sizeof(unsigned int) + uiSize + sizeof(unsigned int);
	char* pcAddr = (char*)malloc(uiExtendedSize);
	if(!pcAddr)
		return NULL;

	Block* pBlock = (Block*)pcAddr;
	pBlock->m_uiSize = uiSize;
	pBlock->m_bIsArray = bIsArray;
	
	bool bAlignment = (uiAlignment > 0) ? true : false;
	pBlock->m_bAlignment = bAlignment;
	pBlock->m_uiStackInfoNum = 0;
	DWORD _ebp, _esp;
	__asm mov _ebp, ebp;
	__asm mov _esp, esp;

	for(unsigned int index = 0; index < CALLSTACK_NUM; index++)
	{
		void * pAddr = (void*)ULongToPtr(*(((DWORD*)ULongToPtr(_ebp))+1));
		if (!pAddr)
		{
			break;
		}
		pBlock->pAddr[index] = pAddr;
		pBlock->m_uiStackInfoNum++;
		_ebp = *(DWORD*)ULongToPtr(_ebp);
		if(_ebp == 0 || 0 != (_ebp & 0xFC000000) || _ebp < _esp)
			break;
	}






	InsertBlock(pBlock);



	pcAddr += sizeof(Block);

	unsigned int * pBeginMask = (unsigned int *)(pcAddr);
	*pBeginMask = BEGIN_MASK;
	pcAddr += sizeof(unsigned int);
	
	unsigned int * pEndMask = (unsigned int *)(pcAddr + uiSize);
	*pEndMask = END_MASK;

	m_uiNumBlocks++;
	m_uiNumBytes += uiSize;

	if (m_uiNumBytes > m_uiMaxNumBytes)
	{
		m_uiMaxNumBytes = m_uiNumBytes;
	}
	if (m_uiNumBlocks > m_uiMaxNumBlocks)
	{
		m_uiMaxNumBlocks = m_uiNumBlocks;
	}


	unsigned int uiTwoPowerI = 1;
	int i;
	for (i = 0; i <= RECORD_NUM - 2 ; i++, uiTwoPowerI <<= 1)
	{
		if (uiSize <= uiTwoPowerI)
		{
			m_uiSizeRecord[i]++;
			break;
		}
	}
	if (i == RECORD_NUM - 1)
	{
		m_uiSizeRecord[i]++;
	}

	ms_MemLock.Unlock();
	return (void*)pcAddr;
}

void VSDebugMem::Deallocate (char* pcAddr,unsigned int uiAlignment, bool bIsArray)
{
	ms_MemLock.Lock();
	m_uiNumDeleteCalls++;

	if (!pcAddr)
	{
		ms_MemLock.Unlock();
		return;
	}

	pcAddr -= sizeof(unsigned int);

	unsigned int *pBeginMask = (unsigned int *)(pcAddr);
	VSMAC_ASSERT(*pBeginMask == BEGIN_MASK);

	pcAddr -= sizeof(Block);

	Block* pBlock = (Block*)pcAddr;
	RemoveBlock(pBlock);

	VSMAC_ASSERT(pBlock->m_bIsArray == bIsArray);
	VSMAC_ASSERT(m_uiNumBlocks > 0 && m_uiNumBytes >= pBlock->m_uiSize);
	bool bAlignment = (uiAlignment > 0) ? true : false;
	VSMAC_ASSERT(pBlock->m_bAlignment == bAlignment);
	unsigned int * pEndMask = (unsigned int *)(pcAddr + sizeof(Block) + sizeof(unsigned int) + pBlock->m_uiSize);
	VSMAC_ASSERT( *pEndMask == END_MASK);

	m_uiNumBlocks--;
	m_uiNumBytes -= pBlock->m_uiSize;

	free(pcAddr);
	ms_MemLock.Unlock();
}
void VSDebugMem::InsertBlock (Block* pBlock)
{

	if (m_pTail)
	{
		pBlock->m_pPrev = m_pTail;
		pBlock->m_pNext = 0;
		m_pTail->m_pNext = pBlock;
		m_pTail = pBlock;
	}
	else
	{
		pBlock->m_pPrev = 0;
		pBlock->m_pNext = 0;
		m_pHead = pBlock;
		m_pTail = pBlock;
	}
}
void VSDebugMem::RemoveBlock (Block* pBlock)
{
	if (pBlock->m_pPrev)
	{
		pBlock->m_pPrev->m_pNext = pBlock->m_pNext;
	}
	else
	{
		m_pHead = pBlock->m_pNext;
	}

	if (pBlock->m_pNext)
	{
		pBlock->m_pNext->m_pPrev = pBlock->m_pPrev;
	}
	else
	{
		m_pTail = pBlock->m_pPrev;
	}
}
void VSDebugMem::FreeLeakMem()
{
	Block * pBlock = m_pHead;
	while(pBlock)
	{
		Block * Temp = pBlock;
		pBlock = pBlock->m_pNext;
		free((void*)Temp);
	}
}
void VSDebugMem::PrintInfo()
{


	VSOutPutDebugString(_T("#########################  begin print leak mem  ######################\n"));

	VSOutPutDebugString(_T("Max Byte Num: %d\n"),m_uiMaxNumBytes);

	VSOutPutDebugString(_T("Max Block Num: %d\n"),m_uiMaxNumBlocks);

	VSOutPutDebugString(_T("Totle Size: %d\n"),m_uiNumBytes);

	VSOutPutDebugString(_T("Block Num: %d\n"),m_uiNumBlocks);


	VSOutPutDebugString(_T("New Call: %d\n"),m_uiNumNewCalls);

	VSOutPutDebugString(_T("Delete Call: %d\n"),m_uiNumDeleteCalls);

	if (m_pHead)
	{
		VSOutPutDebugString(_T("Memory Leak:\n"));
	}
	else
	{
		VSOutPutDebugString(_T("No Memory Leak\n"));
	}
	Block * pBlock = m_pHead;
	static unsigned int uiLeakNum = 0;
	while (pBlock)
	{

		uiLeakNum++;
		VSOutPutDebugString(_T("$$$$$$$$$$$$$$$$  Leak %d  $$$$$$$$$$$$$$$$$\n"),uiLeakNum);
		VSOutPutDebugString(_T("Size: %d\n"),pBlock->m_uiSize);
		VSOutPutDebugString(_T("Is Array:%d\n"),pBlock->m_bIsArray);

		TCHAR szFile[MAX_PATH];
		int	  line;
		for (unsigned int i = 0 ; i < pBlock->m_uiStackInfoNum ; i++)
		{

			if (!GetFileAndLine(pBlock->pAddr[i], szFile, line))
			{
				break;
			}
			VSOutPutDebugString(_T("%s(%d)\n"),szFile,line);
		
		}
		VSOutPutDebugString(_T("$$$$$$$$$$$$$$$$$ Leak %d  $$$$$$$$$$$$$$$$$$$\n"),uiLeakNum);
		pBlock = pBlock->m_pNext;
	}
	VSOutPutDebugString(_T("leak block total num : %d\n"),uiLeakNum);

	VSOutPutDebugString(_T("#########################  end print leak mem  ######################\n"));
}
VSMemWin32::VSMemWin32()
{

	PageSize = 0 ;

	// Get OS page size.
	SYSTEM_INFO SI;
	GetSystemInfo( &SI );
	PageSize = SI.dwPageSize;
	VSMAC_ASSERT(!(PageSize&(PageSize-1)));

	// Init tables.
	OsTable.FirstPool    = NULL;
	OsTable.ExhaustedPool = NULL;
	OsTable.BlockSize    = 0;

	PoolTable[0].FirstPool    = NULL;
	PoolTable[0].ExhaustedPool = NULL;
	PoolTable[0].BlockSize    = 8;
	for( DWORD i=1; i<5; i++ )
	{
		PoolTable[i].FirstPool    = NULL;
		PoolTable[i].ExhaustedPool = NULL;
		PoolTable[i].BlockSize    = (8<<((i+1)>>2)) + (2<<i);
	}
	for( DWORD i=5; i<POOL_COUNT; i++ )
	{
		PoolTable[i].FirstPool    = NULL;
		PoolTable[i].ExhaustedPool = NULL;
		PoolTable[i].BlockSize    = (4+((i+7)&3)) << (1+((i+7)>>2));
	}
	for( DWORD i=0; i < POOL_MAX; i++ )
	{
		DWORD Index;
		for( Index=0; PoolTable[Index].BlockSize<i; Index++ );
		VSMAC_ASSERT(Index < POOL_COUNT);
		MemSizeToPoolTable[i] = &PoolTable[Index];
	}
	for( DWORD i=0; i < 32 ; i++ )
	{
		PoolIndirect[i] = NULL;
	}
	VSMAC_ASSERT(POOL_MAX-1==PoolTable[POOL_COUNT-1].BlockSize);
}
VSMemWin32::~VSMemWin32()
{
	for (unsigned int i = 0 ; i < 32 ; i++)
	{
		for (unsigned int j = 0 ; j < 2048 ; j++)
		{
			if (PoolIndirect[i])
			{		
				if (PoolIndirect[i][j].Mem)
				{
					VirtualFree( PoolIndirect[i][j].Mem, 0, MEM_RELEASE );
					PoolIndirect[i][j].Mem = NULL;
				}

				VirtualFree( PoolIndirect[i], 0, MEM_RELEASE );
				PoolIndirect[i] = NULL;
			}
		}
	}
}
VSMemWin32::FPoolInfo* VSMemWin32::FFreeMem::GetPool()
{
	return (FPoolInfo*)((INT)this & 0xffff0000);
}
void* VSMemWin32::Allocate (unsigned int uiSize,unsigned int uiAlignment,bool bIsArray)
{
	ms_MemLock.Lock();
	FFreeMem* Free;
	if( uiSize<POOL_MAX )
	{
		// Allocate from pool.
		FPoolTable* Table = MemSizeToPoolTable[uiSize];
		VSMAC_ASSERT(uiSize<=Table->BlockSize);
		FPoolInfo* Pool = Table->FirstPool;
		if( !Pool )
		{
			// Must create a new pool.
			DWORD Blocks  = 65536 / Table->BlockSize;
			DWORD Bytes   = Blocks * Table->BlockSize;
			VSMAC_ASSERT(Blocks>=1);
			VSMAC_ASSERT(Blocks*Table->BlockSize<=Bytes);

			// Allocate memory.
			Free = (FFreeMem*)VirtualAlloc( NULL, Bytes, MEM_COMMIT, PAGE_READWRITE );
			if( !Free )
			{
				return NULL;
			}

			// Create pool in the indirect table.
			FPoolInfo*& Indirect = PoolIndirect[((DWORD)Free>>27)];
			if( !Indirect )
			{
				Indirect = CreateIndirect();
			}
			Pool = &Indirect[((DWORD)Free>>16)&2047];

			// Init pool.
			Pool->Link( Table->FirstPool );
			Pool->Mem            = (BYTE*)Free;
			Pool->Bytes		     = Bytes;
			Pool->OsBytes		 = Align(Bytes,PageSize);
			Pool->Table		     = Table;
			Pool->Taken			 = 0;
			Pool->FirstMem       = Free;

			// Create first free item.
			Free->Blocks         = Blocks;
			Free->Next           = NULL;
		}

		// Pick first available block and unlink it.
		Pool->Taken++;
		VSMAC_ASSERT(Pool->FirstMem);
		VSMAC_ASSERT(Pool->FirstMem->Blocks>0);
		Free = (FFreeMem*)((BYTE*)Pool->FirstMem + --Pool->FirstMem->Blocks * Table->BlockSize);
		if( Pool->FirstMem->Blocks==0 )
		{
			Pool->FirstMem = Pool->FirstMem->Next;
			if( !Pool->FirstMem )
			{
				// Move to exhausted list.
				Pool->Unlink();
				Pool->Link( Table->ExhaustedPool );
			}
		}
	}
	else
	{
		// Use OS for large allocations.
		INT AlignedSize = Align(uiSize,PageSize);
		Free = (FFreeMem*)VirtualAlloc( NULL, AlignedSize, MEM_COMMIT, PAGE_READWRITE );
		if( !Free )
		{
			return NULL;
		}
		VSMAC_ASSERT(!((SIZE_T)Free&65535));

		// Create indirect.
		FPoolInfo*& Indirect = PoolIndirect[((DWORD)Free>>27)];
		if( !Indirect )
		{
			Indirect = CreateIndirect();
		}

		// Init pool.
		FPoolInfo* Pool = &Indirect[((DWORD)Free>>16)&2047];
		Pool->Mem		= (BYTE*)Free;
		Pool->Bytes		= uiSize;
		Pool->OsBytes	= AlignedSize;
		Pool->Table		= &OsTable;

	}
	ms_MemLock.Unlock();
	return Free;
}
void VSMemWin32::Deallocate (char* pcAddr, unsigned int uiAlignment,bool bIsArray)
{
	ms_MemLock.Lock();
	if( !pcAddr )
	{
		return;
	}

	// Windows version.
	FPoolInfo* Pool = &PoolIndirect[(DWORD)pcAddr>>27][((DWORD)pcAddr>>16)&2047];
	VSMAC_ASSERT(Pool->Bytes!=0);
	if( Pool->Table!=&OsTable )
	{
		// If this pool was exhausted, move to available list.
		if( !Pool->FirstMem )
		{
			Pool->Unlink();
			Pool->Link( Pool->Table->FirstPool );
		}

		// Free a pooled allocation.
		FFreeMem* Free		= (FFreeMem *)pcAddr;
		Free->Blocks		= 1;
		Free->Next			= Pool->FirstMem;
		Pool->FirstMem		= Free;


		// Free this pool.
		VSMAC_ASSERT(Pool->Taken>=1);
		if( --Pool->Taken == 0 )
		{
			// Free the OS memory.
			Pool->Unlink();
			VirtualFree( Pool->Mem, 0, MEM_RELEASE );
			Pool->Mem = NULL;
		}
	}
	else
	{
		// Free an OS allocation.

		VirtualFree( pcAddr, 0, MEM_RELEASE );
		Pool->Mem = NULL;
	}
	ms_MemLock.Unlock();
}
VSCMem::VSCMem()
{

}
VSCMem::~VSCMem()
{

}
void* VSCMem::Allocate (unsigned int uiSize,unsigned int uiAlignment,bool bIsArray)
{
	if (!uiSize)
	{
		return NULL;
	}
	if (uiAlignment == 0)
	{
		return malloc(uiSize);

	}
	else
	{
		return _aligned_malloc(uiSize,uiAlignment);
	}
	return NULL;
}
void VSCMem::Deallocate (char* pcAddr, unsigned int uiAlignment,bool bIsArray)
{
	if (!pcAddr)
	{
		return;
	}

	if (uiAlignment == 0)
	{
		free(pcAddr);

	}
	else
	{
		_aligned_free(pcAddr);
	}

}
VSStackMem::VSStackMem(unsigned int uiDefaultChunkSize)
{
	Top = NULL;				
	End = NULL;				
	DefaultChunkSize = uiDefaultChunkSize;	
	TopChunk = NULL;			
	UnusedChunks = NULL;
	NumMarks = 0;
}
VSStackMem::~VSStackMem()
{
	FreeChunks(NULL);
	while( UnusedChunks )
	{
		void* Old = UnusedChunks;
		UnusedChunks = UnusedChunks->Next;
		VSMemObject::GetMemManager().Deallocate((char *)Old,0,true);
	}
}

BYTE* VSStackMem::AllocateNewChunk( INT MinSize )
{
	FTaggedMemory* Chunk=NULL;
	for( FTaggedMemory** Link=&UnusedChunks; *Link; Link=&(*Link)->Next )
	{
		// Find existing chunk.
		if( (*Link)->DataSize >= MinSize )
		{
			Chunk = *Link;
			*Link = (*Link)->Next;
			break;
		}
	}
	if( !Chunk )
	{
		// Create new chunk.
		INT DataSize    = Max( MinSize, (INT)DefaultChunkSize-(INT)sizeof(FTaggedMemory) );
		Chunk           = (FTaggedMemory*)VSMemObject::GetMemManager().Allocate( DataSize + sizeof(FTaggedMemory),0,true);
		Chunk->DataSize = DataSize;
	}

	Chunk->Next = TopChunk;
	TopChunk    = Chunk;
	Top         = Chunk->Data;
	End         = Top + Chunk->DataSize;
	return Top;
}

void VSStackMem::FreeChunks( FTaggedMemory* NewTopChunk )
{
	while( TopChunk!=NewTopChunk )
	{
		FTaggedMemory* RemoveChunk = TopChunk;
		TopChunk                   = TopChunk->Next;
		RemoveChunk->Next          = UnusedChunks;
		UnusedChunks               = RemoveChunk;
	}
	Top = NULL;
	End = NULL;
	if( TopChunk )
	{
		Top = TopChunk->Data;
		End = Top + TopChunk->DataSize;
	}
}

void* VSStackMem::Allocate (unsigned int uiSize,unsigned int uiAlignment,bool bIsArray)
{
	// Debug checks.
	VSMAC_ASSERT(uiSize>=0);
	if (uiAlignment > 0)
	{
		VSMAC_ASSERT((uiAlignment&(uiAlignment-1))==0);
	}
	VSMAC_ASSERT(Top<=End);
	VSMAC_ASSERT(NumMarks > 0);

	// Try to get memory from the current chunk.
	BYTE* Result = Top;
	if (uiAlignment > 0)
	{
		Result = (BYTE *)(((unsigned int)Top + (uiAlignment - 1)) & ~(uiAlignment - 1));
	}
	Top = Result + uiSize;

	// Make sure we didn't overflow.
	if( Top > End )
	{
		// We'd pass the end of the current chunk, so allocate a new one.
		AllocateNewChunk( uiSize + uiAlignment );
		Result = Top;
		if (uiAlignment > 0)
		{
			Result = (BYTE *)(((unsigned int)Top + (uiAlignment - 1)) & ~(uiAlignment - 1));
		}
		Top    = Result + uiSize;
	}
	return Result;
}
void VSStackMem::Clear()
{
	FreeChunks(NULL);
}
VSMemManager& VSMemObject::GetMemManager()
{	
#ifdef _DEBUG
	static VSDebugMem g_MemManager;
#else
	static VSMemWin32 g_MemManager;
#endif 

	return g_MemManager;
}
VSStackMem& VSMemObject::GetStackMemManager ()
{
	static VSStackMem g_StackMemManager;
	return g_StackMemManager;
}
VSMemObject::VSMemObject()
{
#ifdef USE_CUSTOM_NEW
	GetMemManager();
#endif
}
VSMemObject::~VSMemObject()
{

}
}