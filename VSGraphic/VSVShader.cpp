#include "VSVShader.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSVShader,VSShader)
BEGIN_ADD_PROPERTY(VSVShader,VSShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSVShader)
IMPLEMENT_INITIAL_END
VSPointer<VSVShader> VSVShader::Default;
bool VSVShader::ms_bIsEnableASYNLoader = true;
bool VSVShader::ms_bIsEnableGC = true;
VSVShader::VSVShader()
{

}
VSVShader::~VSVShader()
{

}
VSVShader::VSVShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(pBuffer,MainFunName,IsFromFile)
{

}
VSVShader::VSVShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(Buffer,MainFunName,IsFromFile)
{

}
bool VSVShader::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnLoadVShaderProgram(this,pID))
		return 0;

	return 1;
}
bool VSVShader::OnReleaseResource(VSResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleaseVShaderProgram(pID))
		return 0;


	return 1;
}