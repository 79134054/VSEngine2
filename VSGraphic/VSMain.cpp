#include "VSMain.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
VSArray<VSMain::Element> *VSMain::ms_pInitialArray = NULL;
VSArray<VSMain::Element> *VSMain::ms_pTerminalArray = NULL;
VSArray<FunctionProperty> *VSMain::ms_pInitialPropertyArray = NULL;
VSArray<Function> *VSMain::ms_pTerminalPropertyArray = NULL;
unsigned int VSMain::ms_uiInitialObject = 0;
unsigned int VSMain::ms_uiTerminalObject = 0;
#include "VSResourceManager.h"
void VSMain::AddInitialPropertyFuntion(FunctionProperty Func)
{
	if(!Func)
		return;
	if(!ms_pInitialPropertyArray)
	{
		ms_pInitialPropertyArray = VS_NEW VSArray<FunctionProperty>;
	}
	ms_pInitialPropertyArray->AddElement(Func);
}
void VSMain::AddTerminalPropertyFuntion(Function Func)
{
	if(!Func)
		return;
	if(!ms_pTerminalPropertyArray)
	{
		ms_pTerminalPropertyArray = VS_NEW VSArray<Function>;
	}
	ms_pTerminalPropertyArray->AddElement(Func);
}
void VSMain::AddInitialFuntion(Function Func)
{
	AddInitialFuntion(Func,NULL);
}
void VSMain::AddInitialFuntion(Function Func,VSPriority *pPriority)
{
	if(!Func)
		return;
	if(!ms_pInitialArray)
	{
		ms_pInitialArray = VS_NEW VSArray<Element>;
	}
	Element e;
	e.Func = Func;
	e.pPriority = pPriority;
	ms_pInitialArray->AddElement(e);

}
void VSMain::AddTerminalFuntion(Function Func)
{
	AddTerminalFuntion(Func,NULL);
}
void VSMain::AddTerminalFuntion(Function Func,VSPriority *pPriority)
{
	if(!Func)
		return;
	if(!ms_pTerminalArray)
	{
		ms_pTerminalArray = VS_NEW VSArray<Element>;
	}
	Element e;
	e.Func = Func;
	e.pPriority = pPriority;
	ms_pTerminalArray->AddElement(e);

}
bool VSMain::Initialize()
{
	
	for(unsigned int i = 0 ; i < ms_pInitialPropertyArray->GetNum(); i++)
	{
		/*(*( (*ms_pInitialArray)[i].Func ))();*/

		if( !(*( (*ms_pInitialPropertyArray)[i] ))(NULL) )
		{
			VSMAC_ASSERT(0);
			return 0;
		}
	}
	ms_pInitialArray->Sort(0,ms_pInitialArray->GetNum() - 1,PriorityCompare());
	for(unsigned int i = 0 ; i < ms_pInitialArray->GetNum(); i++)
	{
		/*(*( (*ms_pInitialArray)[i].Func ))();*/

		if( !(*( (*ms_pInitialArray)[i].Func ))() )
		{
			VSMAC_ASSERT(0);
			return 0;
		}
	}
	

	ms_pInitialArray->Clear();
	VSMAC_DELETE(ms_pInitialArray);
	ms_uiInitialObject = VSObject::GetObjectManager().GetObjectNum();
	ms_pInitialPropertyArray->Clear();
	VSMAC_DELETE(ms_pInitialPropertyArray);
	return 1;
}
bool VSMain::Terminate()
{
	VSResourceManager::CacheResource();
	ms_pTerminalArray->Sort(0,ms_pTerminalArray->GetNum(),PriorityCompare());
	ms_uiTerminalObject = VSObject::GetObjectManager().GetObjectNum();

	for (int i = ms_pTerminalArray->GetNum() - 1; i >= 0; i--)
	{
		/*Function fun = NULL;
		fun = (*ms_pTerminalArray)[i].Func;
		(*fun)();
		//(*( (*ms_pTerminalArray)[i].Func ))();*/
		if( !(*( (*ms_pTerminalArray)[i].Func ))() )
		{
			VSMAC_ASSERT(0);
			return 0;
		}

	}
	ms_pTerminalArray->Clear();
	VSMAC_DELETE(ms_pTerminalArray);

	for(unsigned int i = 0 ; i < ms_pTerminalPropertyArray->GetNum(); i++)
	{

		if( !(*( (*ms_pTerminalPropertyArray)[i]))() )
		{
			VSMAC_ASSERT(0);
			return 0;
		}
	}
	ms_pTerminalPropertyArray->Clear();
	VSMAC_DELETE(ms_pTerminalPropertyArray);
	VSResourceManager::GCObject();
	VSResourceManager::RunAllGCTask();
	VSMAC_ASSERT(VSResourceManager::IsReleaseAll());
	ms_uiTerminalObject = VSObject::GetObjectManager().GetObjectNum();
	VSMAC_ASSERT(ms_uiTerminalObject == 0);
	return 1;

}