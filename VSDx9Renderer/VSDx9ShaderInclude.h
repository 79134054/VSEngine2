#ifndef VSDX9SHADERINCLUDE_H
#define VSDX9SHADERINCLUDE_H
#include <d3dx9shader.h>
#include "VSString.h"
#include "VSMap.h"

namespace VSEngine2
{
	class VSDx9ShaderInclude : public ID3DXInclude
	{
	public:
		VSDx9ShaderInclude();
		virtual ~VSDx9ShaderInclude();
		STDMETHOD(Open)(THIS_ D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		STDMETHOD(Close)(THIS_ LPCVOID pData);	
	private:
		VSMap<VSString,VSString> m_FileMap;
	};
};
#endif