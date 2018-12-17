#include "VSGShader.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSGShader,VSShader)
BEGIN_ADD_PROPERTY(VSGShader,VSShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSGShader)
IMPLEMENT_INITIAL_END
VSPointer<VSGShader> VSGShader::Default;
bool VSGShader::ms_bIsEnableASYNLoader = true;
bool VSGShader::ms_bIsEnableGC = true;
VSGShader::VSGShader()
{

}
VSGShader::~VSGShader()
{

}
VSGShader::VSGShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(pBuffer,MainFunName,IsFromFile)
{

}
VSGShader::VSGShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(Buffer,MainFunName,IsFromFile)
{

}
bool VSGShader::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnLoadGShaderProgram(this,pID))
		return 0;

	return 1;
}
bool VSGShader::OnReleaseResource(VSResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleaseGShaderProgram(pID))
		return 0;


	return 1;
}