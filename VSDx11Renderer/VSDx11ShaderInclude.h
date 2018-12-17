#ifndef VSDX11SHADERINCLUDE_H
#define VSDX11SHADERINCLUDE_H
#include <d3d11.h>
#include <D3Dcompiler.h>
#include "VSString.h"
#include "VSMap.h"

namespace VSEngine2
{
	class VSDx11ShaderInclude : public ID3D10Include
	{
	public:
		VSDx11ShaderInclude();
		virtual ~VSDx11ShaderInclude();
		STDMETHOD(Open)(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		STDMETHOD(Close)(THIS_ LPCVOID pData);
	private:
		VSMap<VSString,VSString> m_FileMap;
	};
};
#endif