#include "VSRenderThread.h"
#include "VSUserConstant.h"
#include "VSBind.h"
#include "VSGraphicInclude.h"
#include "VSResourceManager.h"
#include "VSProfiler.h"
using namespace VSEngine2;
//#define DEBUG_RENDER_THREAD
VSRenderThreadBuffer::VSRenderThreadBuffer()
{

}
VSRenderThreadBuffer::~VSRenderThreadBuffer()
{
	
}
unsigned char * VSRenderThreadBuffer::Assign(unsigned int uiSize)
{
	return m_ConstantBuffer.Assign(uiSize);
}
unsigned char * VSRenderThreadBuffer::Assign(unsigned int VTType,unsigned int uiRegisterNum)
{
	return m_ConstantBuffer.Assign(VTType,uiRegisterNum);
}
void VSRenderThreadBuffer::Clear()
{
#ifdef DEBUG_RENDER_THREAD
	VSSynchronize::VSSafeOutPutDebugString("**Clear**\n");
#endif
	for (unsigned int i = 0 ; i < m_CommandList.GetNum() ; i++)
	{
		m_CommandList[i]->~VSRenderCommand();
	}
	m_ConstantBuffer.Clear();
	m_CommandList.Clear();
}

void VSRenderThreadBuffer::Execute()
{
#ifdef DEBUG_RENDER_THREAD
	VSSynchronize::VSSafeOutPutDebugString("==========\n");
	VSSynchronize::VSSafeOutPutDebugString("Command Num : %d\n",m_CommandList.GetNum());
#endif
	for (unsigned int i = 0 ; i < m_CommandList.GetNum() ; i++)
	{
		m_CommandList[i]->Execute();
#ifdef DEBUG_RENDER_THREAD	
		VSSynchronize::VSSafeOutPutDebugString("%d_%s\n",i,m_CommandList[i]->DescribeCommand());
#endif
	}
#ifdef DEBUG_RENDER_THREAD
	VSSynchronize::VSSafeOutPutDebugString("==========\n");
#endif
}
VSRenderThreadSys * VSRenderThreadSys::ms_pRenderThreadSys = NULL;
VSRenderThreadSys::VSRenderThreadSys()
{
	VSMAC_ASSERT(!ms_pRenderThreadSys);
	ms_pRenderThreadSys = this;
	m_bIsRunning = false;
	m_RenderBuffer = VS_NEW VSRenderThreadBuffer();
	m_UpdateBuffer = VS_NEW VSRenderThreadBuffer();
	#ifdef 	RENDERBUFFER_UPDATE
		m_RenderThread.Start();
	#endif
}
VSRenderThreadSys::~VSRenderThreadSys()
{
	Clear();
	VSMAC_DELETE(m_RenderBuffer);
	VSMAC_DELETE(m_UpdateBuffer);
	m_RenderThread.Start();
	m_RenderThread.Stop();
}
unsigned char * VSRenderThreadSys::Assign(unsigned int uiSize)
{
	return m_UpdateBuffer->Assign(uiSize);
}
unsigned char * VSRenderThreadSys::Assign(unsigned int VTType,unsigned int uiRegisterNum)
{
	return m_UpdateBuffer->Assign(VTType,uiRegisterNum);
}
DECLEAR_TIME_PROFILENODE(RenderThreadsysExChange, ApplicationUpdate)
void VSRenderThreadSys::ExChange()
{
	ADD_TIME_PROFILE(RenderThreadsysExChange)
#ifdef EVENT_UPDATE	
	m_RenderThread.m_Event.Wait();
#endif
#ifdef 	RENDERBUFFER_UPDATE
	while(1)
	{
#ifdef SLEEP_FLAG
		Sleep(0);
#endif
		if (m_RenderThread.IsReady())
		{
			break;
		}
	}
#endif
#ifdef DEBUG_RENDER_THREAD
	VSSynchronize::VSSafeOutPutDebugString("**Exchange**\n");
#endif
/*#ifdef EVENT_UPDATE*/
	m_RenderThread.Suspend();
/*#endif*/
	m_RenderBuffer->Clear();
	Swap(m_UpdateBuffer,m_RenderBuffer);
	for (unsigned int i = 0 ; i < VSBind::ms_DynamicTwoBindArray.GetNum() ;i++)
	{
		VSBind::ms_DynamicTwoBindArray[i]->ExChange();
	}
	m_bIsRunning = false;
}
void VSRenderThreadSys::Begin()
{
#ifdef DEBUG_RENDER_THREAD
	VSSynchronize::VSSafeOutPutDebugString("*************************Start****************\n");
#endif
	m_bIsRunning = true;
	m_RenderThread.SetRender(m_RenderBuffer);
/*#ifdef EVENT_UPDATE*/
	m_RenderThread.Start();
/*#endif*/
}
void VSRenderThreadSys::Clear()
{
#ifdef DEBUG_RENDER_THREAD
	VSSynchronize::VSSafeOutPutDebugString("system clear\n");
#endif
	m_RenderBuffer->Clear();
	m_UpdateBuffer->Clear();
}
VSRenderThread::VSRenderThread()
{
#ifdef EVENT_UPDATE
	m_Event.Create(true,_T("RenderEvent"));
	m_Event.Reset();
#endif
	m_pRenderBuffer = NULL;
}
VSRenderThread::~VSRenderThread()
{

}

void VSRenderThread::Run()
{
	while(!IsStopTrigger())
	{
		if (m_pRenderBuffer)
		{
			m_pRenderBuffer->Execute();
			m_pRenderBuffer = NULL;
		#ifdef EVENT_UPDATE
			m_Event.Trigger();
		#endif
		}	
	}
}
void VSRenderThread::SetRender(VSRenderThreadBuffer * pRenderBuffer)
{
	m_pRenderBuffer = pRenderBuffer;
	#ifdef EVENT_UPDATE
		m_Event.Reset();
	#endif
	
}
#ifdef 	RENDERBUFFER_UPDATE
bool VSRenderThread::IsReady()
{
#ifndef SLEEP_FLAG
	m_Lock.Lock();
#endif
	return !m_pRenderBuffer;
#ifndef SLEEP_FLAG
	m_Lock.Unlock();
#endif
}
#endif
