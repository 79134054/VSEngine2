#include "VSDx9ShaderInclude.h"
#include "VSFile.h"
#include "VSResourceManager.h"
#include "VSShader.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
VSDx9ShaderInclude::VSDx9ShaderInclude()
{

}
VSDx9ShaderInclude::~VSDx9ShaderInclude()
{

}
HRESULT VSDx9ShaderInclude::Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	assert(D3DXINC_LOCAL == IncludeType);

	VSString FileName(pFileName);
	unsigned int uiIndex = m_FileMap.Find(FileName);
	if (uiIndex != m_FileMap.GetNum())
	{
		*ppData = (void *)m_FileMap[uiIndex].Value.GetBuffer();
		*pBytes = m_FileMap[uiIndex].Value.GetLength();
	}
	else
	{
		VSFile DynamicShaderFile;
		VSString DynamicShaderPath = /*VSResourceManager::ms_ShaderPath +*/ FileName;
		if(!DynamicShaderFile.Open(DynamicShaderPath.GetBuffer(),VSFile::OM_RB))
		{
			VSString PreShaderPath = VSResourceManager::GetRenderTypeShaderPath(VSRenderer::RAT_DIRECTX9);
			DynamicShaderPath = VSResourceManager::ms_ShaderPath + PreShaderPath + FileName;
			if (!DynamicShaderFile.Open(DynamicShaderPath.GetBuffer(),VSFile::OM_RB))
			{
				return S_FALSE;
			}
		}
		unsigned int uiSize = DynamicShaderFile.GetFileSize();
		if (!uiSize)
		{
			return S_FALSE;
		}
		VSString VDynamicShaderString;
		VDynamicShaderString.SetTCHARBufferNum(uiSize);
		if(!DynamicShaderFile.Read(VDynamicShaderString.GetBuffer(),uiSize,1))
		{
			return S_FALSE;
		}
		m_FileMap.AddElement(FileName,VDynamicShaderString);
		*ppData = (void *)m_FileMap[m_FileMap.GetNum() - 1].Value.GetBuffer();
		*pBytes = m_FileMap[m_FileMap.GetNum() - 1].Value.GetLength();

	}
	

	return S_OK;
}

HRESULT VSDx9ShaderInclude::Close(LPCVOID pData)
{
	// do nothing
	return S_OK;
}