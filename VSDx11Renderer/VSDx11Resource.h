#ifndef VSDX11RESOURCE_H
#define VSDX11RESOURCE_H
#ifdef VSDX11RENDERER_EXPORTS
#define  VSDX11RENDERER_API __declspec(dllexport) 
#else
#define  VSDX11RENDERER_API __declspec(dllimport)
#endif
#include <d3d11.h>
#include "VSBind.h"
namespace VSEngine2
{
	class  VSDX11RENDERER_API VSSamplerStateID : public VSResourceIdentifier
	{
	public:
		VSSamplerStateID()
		{
			m_pSamplerState = NULL;
		}

		virtual ~VSSamplerStateID()
		{
			VSMAC_RELEASE(m_pSamplerState)
		}
		ID3D11SamplerState* m_pSamplerState;

	};
	class  VSDX11RENDERER_API VSBlendStateID : public VSResourceIdentifier
	{
	public:
		VSBlendStateID()
		{
			m_pBlendState = NULL;
		}

		virtual ~VSBlendStateID()
		{
				VSMAC_RELEASE(m_pBlendState)
		}
		ID3D11BlendState* m_pBlendState;

	};
	class  VSDX11RENDERER_API VSRasterStateID : public VSResourceIdentifier
	{
	public:
		VSRasterStateID()
		{
			m_pRasterState = NULL;
		}
		virtual ~VSRasterStateID()
		{
			VSMAC_RELEASE(m_pRasterState)

		}
		ID3D11RasterizerState* m_pRasterState;  //∂•µ„‰÷»æ∆˜

	};
	class  VSDX11RENDERER_API VSDepthStencilStateID : public VSResourceIdentifier
	{
	public:
		VSDepthStencilStateID()
		{
			m_pDepthStencilState = NULL;
		}
		virtual ~VSDepthStencilStateID()
		{
			VSMAC_RELEASE(m_pDepthStencilState)

		}
		ID3D11DepthStencilState* m_pDepthStencilState;  //∂•µ„‰÷»æ∆˜

	};
	class  VSDX11RENDERER_API VSVProgramID : public VSResourceIdentifier
	{
	public:
		VSVProgramID()
		{
			m_pVertexShader = NULL;
			m_pShaderBuffer = NULL;
		}
		virtual ~VSVProgramID()
		{
			VSMAC_RELEASE(m_pVertexShader)
			VSMAC_RELEASE(m_pShaderBuffer)
		}
		ID3D11VertexShader*      m_pVertexShader;  //∂•µ„‰÷»æ∆˜
		ID3D11Buffer*			m_pShaderBuffer;
	};
	class  VSDX11RENDERER_API VSPProgramID : public VSResourceIdentifier
	{
	public:
		VSPProgramID()
		{
			m_pPixelShader = NULL;
			m_pShaderBuffer = NULL;
		}
		virtual ~VSPProgramID()
		{
			VSMAC_RELEASE(m_pPixelShader)
			VSMAC_RELEASE(m_pShaderBuffer)
		}
		ID3D11PixelShader*      m_pPixelShader;  //œÒÀÿ‰÷»æ∆˜
		ID3D11Buffer*			m_pShaderBuffer;
	};
	class  VSDX11RENDERER_API VSGProgramID : public VSResourceIdentifier
	{
	public:
		VSGProgramID()
		{
			m_pGeometryShader = NULL;
			m_pShaderBuffer = NULL;
		}
		virtual ~VSGProgramID()
		{
			VSMAC_RELEASE(m_pGeometryShader)
				VSMAC_RELEASE(m_pShaderBuffer)
		}
		ID3D11GeometryShader*      m_pGeometryShader;  //œÒÀÿ‰÷»æ∆˜
		ID3D11Buffer*			m_pShaderBuffer;
	};
	class  VSDX11RENDERER_API VSDProgramID : public VSResourceIdentifier
	{
	public:
		VSDProgramID()
		{
			m_pDomainShader = NULL;
			m_pShaderBuffer = NULL;
		}
		virtual ~VSDProgramID()
		{
			VSMAC_RELEASE(m_pDomainShader)
				VSMAC_RELEASE(m_pShaderBuffer)
		}
		ID3D11DomainShader*      m_pDomainShader;  //œÒÀÿ‰÷»æ∆˜
		ID3D11Buffer*			m_pShaderBuffer;
	};
	class  VSDX11RENDERER_API VSHProgramID : public VSResourceIdentifier
	{
	public:
		VSHProgramID()
		{
			m_pHullShader = NULL;
			m_pShaderBuffer = NULL;
		}
		virtual ~VSHProgramID()
		{
			VSMAC_RELEASE(m_pHullShader)
				VSMAC_RELEASE(m_pShaderBuffer)
		}
		ID3D11HullShader*      m_pHullShader;  //œÒÀÿ‰÷»æ∆˜
		ID3D11Buffer*			m_pShaderBuffer;
	};
// 
	class  VSDX11RENDERER_API VSRenderTargetID : public VSResourceIdentifier
	{
	public:
		VSRenderTargetID()
		{
			m_pSaveRenderTarget = NULL;
			m_pSaveDepthStencilBuffer = NULL;
			m_pRenderTarget = NULL;
			m_pTextureSurface = NULL;
			m_pOldTexture = NULL;
			m_pStagTexture = NULL;
		}
		~VSRenderTargetID()
		{
			VSMAC_RELEASE(m_pRenderTarget);
			VSMAC_RELEASE(m_pTextureSurface);
			VSMAC_RELEASE(m_pStagTexture);
			m_pOldTexture = NULL;
			m_pSaveRenderTarget = NULL;
			m_pSaveDepthStencilBuffer = NULL;
		}
		ID3D11Resource*				m_pOldTexture;
		ID3D11Resource*				m_pTextureSurface;
		ID3D11RenderTargetView *	m_pRenderTarget;
		ID3D11RenderTargetView *	m_pSaveRenderTarget;
		ID3D11DepthStencilView *	m_pSaveDepthStencilBuffer;
		ID3D11Resource*				m_pStagTexture;
	};
	class  VSDX11RENDERER_API VSTextureID : public VSResourceIdentifier
	{
	public:
		VSTextureID()
		{
			
			m_pTexture = NULL;
			m_pShaderResourceView = NULL;
		}
		virtual ~VSTextureID()
		{
			VSMAC_RELEASE(m_pShaderResourceView);
			VSMAC_RELEASE(m_pTexture);		
		}

		ID3D11Resource*      m_pTexture;
		ID3D11ShaderResourceView* m_pShaderResourceView;
	};
	class VSDX11RENDERER_API VSDepthStencilID : public VSResourceIdentifier
	{
	public:
		VSDepthStencilID()
		{
			m_pTexture = NULL;
			m_pDepthStencilBuffer = NULL;
			m_pSaveDepthStencilBuffer = NULL;
		}
		virtual ~VSDepthStencilID()
		{
			VSMAC_RELEASE(m_pDepthStencilBuffer);
			VSMAC_RELEASE(m_pTexture);
			m_pSaveDepthStencilBuffer = NULL;
		}
		ID3D11Resource*      m_pTexture;
		ID3D11DepthStencilView *	m_pDepthStencilBuffer;
		ID3D11DepthStencilView *	m_pSaveDepthStencilBuffer;
	};
	class VSDX11RENDERER_API VSVBufferID : public VSResourceIdentifier
	{
	public:
		VSVBufferID()
		{
			m_pVertexBuffer = NULL;
		}
		virtual ~VSVBufferID()
		{
			VSMAC_RELEASE(m_pVertexBuffer);
		}
		ID3D11Buffer* m_pVertexBuffer;
	
	};
	class VSDX11RENDERER_API VSVBufferFormatID : public VSResourceIdentifier
	{
	public:
		VSVBufferFormatID()
		{

			m_pDeclaration = NULL;
		}
		virtual ~VSVBufferFormatID()
		{
			VSMAC_RELEASE(m_pDeclaration);
		}
		ID3D11InputLayout* m_pDeclaration;
	
	};
	class VSDX11RENDERER_API VSIBufferID : public VSResourceIdentifier
	{
	public:
		VSIBufferID()
		{
			m_IndexBuffer = NULL;
		}
		virtual ~VSIBufferID()
		{
			VSMAC_RELEASE(m_IndexBuffer);

		}
		ID3D11Buffer* m_IndexBuffer;
	};
}
#endif