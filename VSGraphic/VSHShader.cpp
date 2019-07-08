#include "VSHShader.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSHShader,VSShader)
BEGIN_ADD_PROPERTY(VSHShader,VSShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSHShader)
IMPLEMENT_INITIAL_END
VSPointer<VSHShader> VSHShader::Default;
bool VSHShader::ms_bIsEnableASYNLoader = true;
bool VSHShader::ms_bIsEnableGC = true;
VSHShader::VSHShader()
{

}
VSHShader::~VSHShader()
{

}
VSHShader::VSHShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(pBuffer,MainFunName,IsFromFile)
{

}
VSHShader::VSHShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(Buffer,MainFunName,IsFromFile)
{

}
bool VSHShader::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnLoadHShaderProgram(this,pID))
		return 0;

	return 1;
}
bool VSHShader::OnReleaseResource(VSResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleaseHShaderProgram(pID))
		return 0;


	return 1;
}