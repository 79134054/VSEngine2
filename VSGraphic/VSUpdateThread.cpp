#include "VSUpdateThread.h"
using namespace VSEngine2;
VSUpdateContext::VSUpdateContext()
{

}
VSUpdateContext::~VSUpdateContext()
{
	
}
void VSUpdateContext::AddCommand(VSRenderCommand * pCommand)
{
	if (pCommand)
	{
		m_CommandArray.AddElement(pCommand);
	}
}
void VSUpdateContext::Clear()
{
	for (unsigned int i = 0 ; i < m_CommandArray.GetNum() ; i++)
	{
		m_CommandArray[i]->~VSRenderCommand();
	}
	m_CommandArray.Clear();
}
unsigned int VSUpdateContext::Execute()
{
	for (unsigned int i = 0 ; i < m_CommandArray.GetNum() ; i++)
	{
		m_CommandArray[i]->Execute();
	}
	return 1;
}
VSUpdateCenter::VSUpdateCenter(bool bIsMainRun,unsigned int uiThreadNum)
{
	m_uiContextNum = uiThreadNum;
	m_bIsMainRun = bIsMainRun;
	if (m_bIsMainRun)
	{
		m_uiContextNum++;
	}
	m_pContextArray = VS_NEW VSUpdateContext[m_uiContextNum];
	m_uiIndex = -1;
}
VSUpdateCenter::~VSUpdateCenter()
{
	VSMAC_DELETEA(m_pContextArray);
}
unsigned char * VSUpdateCenter::Assign(unsigned int uiSize)
{
	return m_ConstantBuffer.Assign(uiSize);
}
unsigned int VSUpdateCenter::GetIndex()
{
	m_uiIndex++;
	m_uiIndex = m_uiIndex % m_uiContextNum;
	return m_uiIndex;
}
VSUpdateContext * VSUpdateCenter::GetUpdateContext(unsigned int i)
{
	return &m_pContextArray[i];
}
unsigned int VSUpdateCenter::Execute()
{
	if (m_bIsMainRun)
	{
		m_pContextArray[m_uiContextNum - 1].Execute();
	}
	return 1;
}
void VSUpdateCenter::Clear()
{
	
	for (unsigned int i = 0 ; i < m_uiContextNum ; i++)
	{
		m_pContextArray[i].Clear();
	}
	m_ConstantBuffer.Clear();
	m_uiIndex = -1;
}
VSUpdateThread::VSUpdateThread()
{
	static unsigned int s_uiThreadID = 0;
	m_uiThreadId = s_uiThreadID;
	s_uiThreadID++;
	static TCHAR TempString[128];
	VSSprintf(TempString,128,_T("VSUpdateThread%d"),m_uiThreadId);
	m_ThreadName = TempString;
#ifdef EVENT_UPDATE
	VSSprintf(TempString,128,"VSUpdateEvent%d",m_uiThreadId);
	m_Event.Create(true,TempString);
	m_Event.Reset();
#endif
	m_pUpdateContex = NULL;
}

VSUpdateThread::~VSUpdateThread()
{
	m_pUpdateContex = NULL;
}
void VSUpdateThread::Run()
{
	while(!IsStopTrigger())
	{
		if (m_pUpdateContex)
		{
			#ifdef DEBUG_UPDATE_THREAD
				VSSynchronize::VSSafeOutPutDebugString("Execute Begin : %d\n",m_uiThreadId);
			#endif
			m_pUpdateContex->Execute();		
			m_pUpdateContex = NULL;	
			#ifdef DEBUG_UPDATE_THREAD
				VSSynchronize::VSSafeOutPutDebugString("Execute End : %d\n",m_uiThreadId);
			#endif
			#ifdef EVENT_UPDATE
				m_Event.Trigger();
			#endif
			
		}	
	}
}
void VSUpdateThread::SetUpdateContext(VSUpdateContext * pUpdateContex)
{
	m_pUpdateContex = pUpdateContex;
	#ifdef EVENT_UPDATE
		m_Event.Reset();
	#endif
}
#ifdef CONTEXT_UPDATE
bool VSUpdateThread::IsReady()const
{
	return !m_pUpdateContex;
}
#endif
VSUpdateThreadSys * VSUpdateThreadSys::ms_pUpdateThreadSys = NULL;
VSUpdateThreadSys::VSUpdateThreadSys(unsigned int uiThreadNum)
{
	VSMAC_ASSERT(uiThreadNum);
	VSMAC_ASSERT(!ms_pUpdateThreadSys);
	ms_pUpdateThreadSys = this;

	m_uiThreadNum = uiThreadNum;
	m_UpdateThread.SetBufferNum(m_uiThreadNum);
#ifdef DEBUG_UPDATE_THREAD
	for (unsigned int i = 0 ; i < m_UpdateThread.GetNum() ; i++)
	{
		m_UpdateThread[i].m_uiThreadId = i;
	}
#endif
#ifdef EVENT_UPDATE
	m_WaitEvent.SetBufferNum(m_uiThreadNum);
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_WaitEvent[i] = &m_UpdateThread[i].m_Event;
	}
#endif
	m_uiCurUpdateType = UPDATE_NODE;
	m_UpdateCenter[UPDATE_NODE] = VS_NEW VSUpdateCenter(true,m_uiThreadNum);
#ifdef CONTEXT_UPDATE
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Start();
	}
#endif
}
VSUpdateThreadSys::~VSUpdateThreadSys()
{
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Start();
	}
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Stop();
	}
	VSMAC_DELETE(m_UpdateCenter[UPDATE_NODE]);
}
void VSUpdateThreadSys::Begin()
{

#ifdef DEBUG_UPDATE_THREAD
	VSSynchronize::VSSafeOutPutDebugString("*************************Start****************\n");
#endif
	for (unsigned int i = 0 ; i < m_uiThreadNum; i++)
	{
		m_UpdateThread[i].SetUpdateContext(m_UpdateCenter[m_uiCurUpdateType]->GetUpdateContext(i));
	}
/*#ifdef EVENT_UPDATE*/
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Start();
	}
/*#endif*/
	m_UpdateCenter[m_uiCurUpdateType]->Execute();

	
}
void VSUpdateThreadSys::ExChange()
{
#ifdef EVENT_UPDATE
	// 	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	// 	{
	// 		m_UpdateThread[i].m_Event.Wait();
	// 
	// 	}
	VSSynchronize::WaitAll(m_WaitEvent.GetBuffer(),m_uiThreadNum,true);
#endif
#ifdef DEBUG_UPDATE_THREAD
	VSSynchronize::VSSafeOutPutDebugString("**Exchange**\n");
#endif
#ifdef CONTEXT_UPDATE
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		while(1)
		{
			if (m_UpdateThread[i].IsReady())
			{
				break;
			}
		}
	}
#endif
/*#ifdef EVENT_UPDATE*/
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Suspend();
	}
/*#endif*/
#ifdef DEBUG_UPDATE_THREAD
	VSSynchronize::VSSafeOutPutDebugString("system clear\n");
#endif
	m_UpdateCenter[m_uiCurUpdateType]->Clear();
}
