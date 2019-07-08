#include "VSPShader.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSPShader,VSShader)
BEGIN_ADD_PROPERTY(VSPShader,VSShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPShader)
IMPLEMENT_INITIAL_END
VSPointer<VSPShader> VSPShader::Default;
bool VSPShader::ms_bIsEnableASYNLoader = true;
bool VSPShader::ms_bIsEnableGC = true;
VSPShader::VSPShader()
{

}
VSPShader::VSPShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(pBuffer,MainFunName,IsFromFile)
{

}
VSPShader::VSPShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(Buffer,MainFunName,IsFromFile)
{

}
VSPShader::~VSPShader()
{

}


bool VSPShader::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnLoadPShaderProgram (this,pID))
		return 0;

	return 1;
}
bool VSPShader::OnReleaseResource(VSResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleasePShaderProgram(pID))
		return 0;


	return 1;
}