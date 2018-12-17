#ifndef VSDX9RESOURCE_H
#define VSDX9RESOURCE_H
#ifdef VSDX9RENDERER_EXPORTS
#define  VSDX9RENDERER_API __declspec(dllexport) 
#else
#define  VSDX9RENDERER_API __declspec(dllimport)
#endif
#include <d3d9.h>
#include <d3dx9tex.h>
#include "VSBind.h"
namespace VSEngine2
{
	
	class  VSDX9RENDERER_API VSVProgramID : public VSResourceIdentifier
	{
	public:
		VSVProgramID()
		{
			m_pVertexShader = NULL;
		}
		virtual ~VSVProgramID()
		{
			VSMAC_RELEASE(m_pVertexShader)
			
		}
		IDirect3DVertexShader9*      m_pVertexShader;  //∂•µ„‰÷»æ∆˜
		
	};
	class  VSDX9RENDERER_API VSPProgramID : public VSResourceIdentifier
	{
	public:
		VSPProgramID()
		{
			m_pPixelShader = NULL;

		}
		virtual ~VSPProgramID()
		{
			VSMAC_RELEASE(m_pPixelShader)

		}
		IDirect3DPixelShader9*      m_pPixelShader;  //œÒÀÿ‰÷»æ∆˜

	};

	class  VSDX9RENDERER_API VSRenderTargetID : public VSResourceIdentifier
	{
	public:
		VSRenderTargetID()
		{
			m_pSaveRenderTarget = NULL;
			m_pRenderTarget = NULL;
			m_pTextureSurface = NULL;
		}
		~VSRenderTargetID()
		{
			VSMAC_RELEASE(m_pRenderTarget);
			VSMAC_RELEASE(m_pSaveRenderTarget);
			VSMAC_RELEASE(m_pTextureSurface);
		}
		LPDIRECT3DSURFACE9			m_pTextureSurface;
		LPDIRECT3DSURFACE9			m_pRenderTarget;
		LPDIRECT3DSURFACE9			m_pSaveRenderTarget;
	};
	class  VSDX9RENDERER_API VSTextureID : public VSResourceIdentifier
	{
	public:
		VSTextureID()
		{
			
			m_pTexture = NULL;
			
		}
		virtual ~VSTextureID()
		{
			
			VSMAC_RELEASE(m_pTexture);
			
			
		}
		IDirect3DBaseTexture9*      m_pTexture;	
		
	};
	class VSDX9RENDERER_API VSDepthStencilID : public VSResourceIdentifier
	{
	public:
		VSDepthStencilID()
		{
			m_pDepthStencilBuffer = NULL;
			m_pSaveDepthStencilBuffer = NULL;
		}
		virtual ~VSDepthStencilID()
		{
			VSMAC_RELEASE(m_pDepthStencilBuffer);
			VSMAC_RELEASE(m_pSaveDepthStencilBuffer);
		}
		LPDIRECT3DSURFACE9	m_pDepthStencilBuffer;
		LPDIRECT3DSURFACE9	m_pSaveDepthStencilBuffer;
	};
	class VSDX9RENDERER_API VSVBufferID : public VSResourceIdentifier
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
		IDirect3DVertexBuffer9* m_pVertexBuffer;
	
	};
	class VSDX9RENDERER_API VSVBufferFormatID : public VSResourceIdentifier
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
		IDirect3DVertexDeclaration9* m_pDeclaration;
	
	};
	class VSDX9RENDERER_API VSIBufferID : public VSResourceIdentifier
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
		LPDIRECT3DINDEXBUFFER9 m_IndexBuffer;
	};
}
#endif