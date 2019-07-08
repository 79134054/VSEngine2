#include "VSResource.h"
#include "VSStream.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
VSString VSResource::ms_FileSuffix[] = 
{
	_T("TEXTURE"),
	_T("SKMODEL"),
	_T("STMODEL"),
	_T("ACTION"),
	_T("MATERIAL"),
	_T("POSTEFFECT"),
	_T("SHADER"),
	_T("ANIMTREE"),
	_T("MORPHTREE"),
	_T("TERRAIN"),
	_T("FSM"),
	_T("ACTOR"),
	_T("MAP"),
	_T("FONT"),
	_T("")


}; 
const VSString &VSResource::GetFileSuffix(unsigned int uiFileSuffix)
{
	if(uiFileSuffix > RT_MAX)
		return ms_FileSuffix[RT_MAX];	
	return ms_FileSuffix[uiFileSuffix];
}
VSResource::VSResource()
{

}
VSResource::~VSResource()
{

}

VSResourceProxyBase::VSResourceProxyBase()
{

}
VSResourceProxyBase::~VSResourceProxyBase()
{

}
