#include "VSDx11Renderer.h"
#include "VSDx11Resource.h"
#include "VSGraphicInclude.h"
#include "VSProfiler.h"
#include "VSRenderThread.h"
using namespace VSEngine2;
void VSDX11Renderer::ClearBackBuffer()
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11ClearBackBufferCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSColorRGBA, m_ClearColor, m_ClearColor,
	{
		ID3D11RenderTargetView * RTVTemp[MAX_RENDER_TARGET] = { NULL };
		m_pDeviceContext->OMGetRenderTargets(MAX_RENDER_TARGET, RTVTemp, NULL);
		for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
		{
			if (RTVTemp[i])
			{
				m_pDeviceContext->ClearRenderTargetView(RTVTemp[i], m_ClearColor.m);
			}
			else
			{
				break;
			}
		}
		for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
		{
			VSMAC_RELEASE(RTVTemp[i]);
		}
	})
	ID3D11RenderTargetView * RTVTemp[MAX_RENDER_TARGET] = { NULL };
	m_pDeviceContext->OMGetRenderTargets(MAX_RENDER_TARGET, RTVTemp, NULL);
	for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
	{
		if (RTVTemp[i])
		{
			m_pDeviceContext->ClearRenderTargetView(RTVTemp[i], m_ClearColor.m);
		}
		else
		{
			break;
		}
	}
	for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
	{
		VSMAC_RELEASE(RTVTemp[i]);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::ClearDepthStencilView(UINT ClearFlags, float Depth, unsigned char Stencil)
{
	struct  VSClearDepthStencilViewPara
	{
		UINT ClearFlags;
		float Depth;
		unsigned char Stencil;
	};
	VSClearDepthStencilViewPara ClearDepthStencilViewPara;
	ClearDepthStencilViewPara.ClearFlags = ClearFlags;
	ClearDepthStencilViewPara.Depth = Depth;
	ClearDepthStencilViewPara.Stencil = Stencil;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11ClearDepthStencilViewCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSClearDepthStencilViewPara, ClearDepthStencilViewPara, ClearDepthStencilViewPara,
	{
		ID3D11DepthStencilView * DSV = NULL;
		m_pDeviceContext->OMGetRenderTargets(0, NULL, &DSV);
		if (DSV)
		{
			m_pDeviceContext->ClearDepthStencilView(DSV, ClearDepthStencilViewPara.ClearFlags, ClearDepthStencilViewPara.Depth, ClearDepthStencilViewPara.Stencil);
		}
		VSMAC_RELEASE(DSV);
	})
	ID3D11DepthStencilView * DSV = NULL;
	m_pDeviceContext->OMGetRenderTargets(0, NULL, &DSV);
	if (DSV)
	{
		m_pDeviceContext->ClearDepthStencilView(DSV, ClearDepthStencilViewPara.ClearFlags, ClearDepthStencilViewPara.Depth, ClearDepthStencilViewPara.Stencil);
	}
	VSMAC_RELEASE(DSV);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::OMSetDepthStencilState(VSDepthStencilStateID *pDepthStencilStateID, UINT StencilRef)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11OMSetDepthStencilStateCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSDepthStencilStateID *, pDepthStencilStateID, pDepthStencilStateID, UINT, StencilRef, StencilRef,
	{
		if (pDepthStencilStateID)
		{
			m_pDeviceContext->OMSetDepthStencilState(pDepthStencilStateID->m_pDepthStencilState, StencilRef);
		}
		else
		{
			m_pDeviceContext->OMSetDepthStencilState(NULL, 0);
		}
	})
	if (pDepthStencilStateID)
	{
		m_pDeviceContext->OMSetDepthStencilState(pDepthStencilStateID->m_pDepthStencilState, StencilRef);
	}
	else
	{
		m_pDeviceContext->OMSetDepthStencilState(NULL, 0);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::RSSetState(VSRasterStateID *pRasterStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11RSSetStateCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSRasterStateID *, pRasterStateID, pRasterStateID,
	{
		if (pRasterStateID)
		{
			m_pDeviceContext->RSSetState(pRasterStateID->m_pRasterState);
		}
		else
		{
			m_pDeviceContext->RSSetState(NULL);
		}
	})
	if (pRasterStateID)
	{
		m_pDeviceContext->RSSetState(pRasterStateID->m_pRasterState);
	}
	else
	{
		m_pDeviceContext->RSSetState(NULL);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::RSSetViewports(D3D11_VIEWPORT& View)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11RSSetStateCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		D3D11_VIEWPORT, View, View,
	{
		m_pDeviceContext->RSSetViewports(1, &View);
	})
	m_pDeviceContext->RSSetViewports(1, &View);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::UseWindowEx(int uiWindowID)
{
	ID3D11RenderTargetView* pRenderTargetView = NULL;
	ID3D11DepthStencilView* pDepthStencilView = NULL;
	if (uiWindowID == -1)
	{
		pRenderTargetView = m_pMainRenderTargetView;
		pDepthStencilView = m_pMainDepthStencilView;
		
	}
	else
	{
		pRenderTargetView = m_pChainRenderTargetView[m_iCurWindowID];
		pDepthStencilView = m_pChainDepthStencilView[uiWindowID];
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11UseWindowExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		ID3D11RenderTargetView*, pRenderTargetView, pRenderTargetView, ID3D11DepthStencilView*, pDepthStencilView, pDepthStencilView,
	{
		m_pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	})
	m_pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END

}
bool VSDX11Renderer::EndRender()
{
	IDXGISwapChain* pChain = NULL;
	if (m_iCurWindowID == -1)
	{
		pChain = m_pMainChain;
	}
	else
	{
		pChain = m_pChain[m_iCurWindowID];
	}

	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(VSDx11EndRenderCommand, IDXGISwapChain*, pChain, pChain,
	{
		HRESULT hResult = NULL;
		hResult = pChain->Present(0, 0);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	HRESULT hResult = NULL;
	hResult = pChain->Present(0, 0);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	return true;
}
void VSDX11Renderer::PSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11PSSetShaderResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSTextureID *, pTextureID, pTextureID,
	{
		if (pTextureID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
			m_pDeviceContext->PSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->PSSetShaderResources(StartSlot, 1, Temp);
		}
	})
	if (pTextureID)
	{
		ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
		m_pDeviceContext->PSSetShaderResources(StartSlot, 1, Temp);
	}
	else
	{
		ID3D11ShaderResourceView* Temp[1] = { NULL };
		m_pDeviceContext->PSSetShaderResources(StartSlot, 1, Temp);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	
}
bool VSDX11Renderer::CreateInputLayout(const VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>& FormatArray, VSVBufferFormatID * pVBufferFormatID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateInputLayoutCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>, FormatArray, FormatArray, 
		VSVBufferFormatID *, pVBufferFormatID, pVBufferFormatID,
	{
		HRESULT hResult = NULL;
		VSArray<D3D11_INPUT_ELEMENT_DESC> Elements;
		D3D11_INPUT_ELEMENT_DESC Element;

		Element.InputSlot = 0;
		Element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		Element.InstanceDataStepRate = 0;
		for (unsigned int i = 0; i < FormatArray.GetNum(); i++)
		{
			VSVertexFormat::VERTEXFORMAT_TYPE &Format = FormatArray[i];
			Element.SemanticName = ms_dwVertexFormatSemantics[Format.Semantics];
			Element.AlignedByteOffset = Format.OffSet;
			Element.Format = (DXGI_FORMAT)ms_dwVertexFormatDateType[Format.DateType];
			Element.SemanticIndex = Format.SemanticsIndex;
			Elements.AddElement(Element);
		}
		ID3DBlob* pCode = VSDX11Renderer::CreateVertexFormatShader(FormatArray);
		if (!pCode)
		{
			return false;
		}
		// Create the vertex input layout.
		hResult = m_pDevice->CreateInputLayout(Elements.GetBuffer(), Elements.GetNum(), pCode->GetBufferPointer(),
			pCode->GetBufferSize(), &pVBufferFormatID->m_pDeclaration);
		VSMAC_RELEASE(pCode);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	HRESULT hResult = NULL;
	VSArray<D3D11_INPUT_ELEMENT_DESC> Elements;
	D3D11_INPUT_ELEMENT_DESC Element;

	Element.InputSlot = 0;
	Element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	Element.InstanceDataStepRate = 0;
	for (unsigned int i = 0; i < FormatArray.GetNum(); i++)
	{
		VSVertexFormat::VERTEXFORMAT_TYPE &Format = FormatArray[i];
		Element.SemanticName = ms_dwVertexFormatSemantics[Format.Semantics];
		Element.AlignedByteOffset = Format.OffSet;
		Element.Format = (DXGI_FORMAT)ms_dwVertexFormatDateType[Format.DateType];
		Element.SemanticIndex = Format.SemanticsIndex;
		Elements.AddElement(Element);
	}
	ID3DBlob* pCode = VSDX11Renderer::CreateVertexFormatShader(FormatArray);
	if (!pCode)
	{
		return false;
	}
	// Create the vertex input layout.
	hResult = m_pDevice->CreateInputLayout(Elements.GetBuffer(), Elements.GetNum(), pCode->GetBufferPointer(),
		pCode->GetBufferSize(), &pVBufferFormatID->m_pDeclaration);
	VSMAC_RELEASE(pCode);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	return true;
}
void VSDX11Renderer::DrawDynamicScreenData(void * pScreenBuffer, unsigned int uiVertexNum, unsigned int uiVertexStride, VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	struct ScreenData
	{
		void* pScreenBuffer;
		VSUSHORT_INDEX * pIndexBuffer;
		unsigned int uiVertexNum;
		unsigned int uiIndexNum;
		unsigned int uiVertexStride;
		ID3D11Buffer** pFontVertexBuffer;
		ID3D11Buffer** pFontIndexBuffer;
	};
	HRESULT hResult = NULL;
	ScreenData Screen;
	Screen.uiVertexNum = uiVertexNum;
	Screen.uiIndexNum = uiIndexNum;
	Screen.uiVertexStride = uiVertexStride;
	if (VSResourceManager::ms_bRenderThread)
	{
		unsigned int uiVertexLen = uiVertexStride * uiVertexNum;
		Screen.pScreenBuffer = (void *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiVertexLen);
		VSMemcpy(Screen.pScreenBuffer, pScreenBuffer, uiVertexLen);

		unsigned int uiIndexLen = sizeof(VSUSHORT_INDEX)* uiIndexNum;
		Screen.pIndexBuffer = (VSUSHORT_INDEX *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiIndexLen);
		VSMemcpy(Screen.pIndexBuffer, pIndexBuffer, uiIndexLen);

	}
	else
	{
		Screen.pScreenBuffer = pScreenBuffer;
		Screen.pIndexBuffer = pIndexBuffer;
	}
	Screen.pFontVertexBuffer = &m_pFontVertexBuffer;
	Screen.pFontIndexBuffer = &m_pFontIndexBuffer;
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11DrawScreenEXCommand,
		ScreenData, Screen, Screen, ID3D11Device*, m_pDevice, m_pDevice, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		{
		unsigned int VertexBytes = Screen.uiVertexNum * Screen.uiVertexStride;
		unsigned int IndexBytes = Screen.uiIndexNum * sizeof(VSUSHORT_INDEX);
		D3D11_BUFFER_DESC VertexBufferDesc;
		(*Screen.pFontVertexBuffer)->GetDesc(&VertexBufferDesc);
		if (VertexBufferDesc.ByteWidth < VertexBytes)
		{
			VertexBufferDesc.ByteWidth = VertexBytes;
			VSMAC_RELEASE((*Screen.pFontVertexBuffer));
			m_pDevice->CreateBuffer(&VertexBufferDesc, NULL, Screen.pFontVertexBuffer);
		}
		D3D11_BUFFER_DESC IndexBufferDesc;
		(*Screen.pFontIndexBuffer)->GetDesc(&IndexBufferDesc);
		if (IndexBufferDesc.ByteWidth < IndexBytes)
		{
			IndexBufferDesc.ByteWidth = IndexBytes;
			VSMAC_RELEASE((*Screen.pFontIndexBuffer));
			m_pDevice->CreateBuffer(&IndexBufferDesc, NULL, Screen.pFontIndexBuffer);
		}
		HRESULT hResult = NULL;
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		hResult = m_pDeviceContext->Map((*Screen.pFontVertexBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		VSMAC_ASSERT(!FAILED(hResult));
		VSMemcpy(MappedResource.pData, (void*)Screen.pScreenBuffer, VertexBytes);
		m_pDeviceContext->Unmap((*Screen.pFontVertexBuffer), 0);


		hResult = m_pDeviceContext->Map((*Screen.pFontIndexBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		VSMAC_ASSERT(!FAILED(hResult));
		VSMemcpy(MappedResource.pData, (void*)Screen.pIndexBuffer, IndexBytes);
		m_pDeviceContext->Unmap((*Screen.pFontIndexBuffer), 0);

		unsigned int Stride = Screen.uiVertexStride;
		unsigned int Offset = 0;
		m_pDeviceContext->IASetIndexBuffer((*Screen.pFontIndexBuffer), DXGI_FORMAT_R16_UINT, 0);
		m_pDeviceContext->IASetVertexBuffers(0, 1, Screen.pFontVertexBuffer, &Stride, &Offset);
		m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pDeviceContext->DrawIndexed(Screen.uiIndexNum, 0, 0);
	})
		unsigned int VertexBytes = Screen.uiVertexNum * Screen.uiVertexStride;
	unsigned int IndexBytes = Screen.uiIndexNum * sizeof(VSUSHORT_INDEX);
	D3D11_BUFFER_DESC VertexBufferDesc;
	(*Screen.pFontVertexBuffer)->GetDesc(&VertexBufferDesc);
	if (VertexBufferDesc.ByteWidth < VertexBytes)
	{
		VertexBufferDesc.ByteWidth = VertexBytes;
		VSMAC_RELEASE((*Screen.pFontVertexBuffer));
		m_pDevice->CreateBuffer(&VertexBufferDesc, NULL, Screen.pFontVertexBuffer);
	}
	D3D11_BUFFER_DESC IndexBufferDesc;
	(*Screen.pFontIndexBuffer)->GetDesc(&IndexBufferDesc);
	if (IndexBufferDesc.ByteWidth < IndexBytes)
	{
		IndexBufferDesc.ByteWidth = IndexBytes;
		VSMAC_RELEASE((*Screen.pFontIndexBuffer));
		m_pDevice->CreateBuffer(&IndexBufferDesc, NULL, Screen.pFontIndexBuffer);
	}
	HRESULT hResult = NULL;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	hResult = m_pDeviceContext->Map((*Screen.pFontVertexBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	VSMAC_ASSERT(!FAILED(hResult));
	VSMemcpy(MappedResource.pData, (void*)Screen.pScreenBuffer, VertexBytes);
	m_pDeviceContext->Unmap((*Screen.pFontVertexBuffer), 0);


	hResult = m_pDeviceContext->Map((*Screen.pFontIndexBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	VSMAC_ASSERT(!FAILED(hResult));
	VSMemcpy(MappedResource.pData, (void*)Screen.pIndexBuffer, IndexBytes);
	m_pDeviceContext->Unmap((*Screen.pFontIndexBuffer), 0);

	unsigned int Stride = Screen.uiVertexStride;
	unsigned int Offset = 0;
	m_pDeviceContext->IASetIndexBuffer((*Screen.pFontIndexBuffer), DXGI_FORMAT_R16_UINT, 0);
	m_pDeviceContext->IASetVertexBuffers(0, 1, Screen.pFontVertexBuffer, &Stride, &Offset);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->DrawIndexed(Screen.uiIndexNum, 0, 0);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::OMSetBlendState(VSBlendStateID* pBlendStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OMSetBlendStateCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSBlendStateID*, pBlendStateID, pBlendStateID,
	{
		VSREAL fBlendColor[4] = { 0.0f };
		if (pBlendStateID)
		{

			m_pDeviceContext->OMSetBlendState(pBlendStateID->m_pBlendState, fBlendColor, 0xffffffff);
		}
		else
		{
			m_pDeviceContext->OMSetBlendState(NULL, fBlendColor, 0xffffffff);
		}
		//m_pDeviceContext->OMSetBlendState(pBlendStateID->m_pBlendState, m_pBlendState->GetBlendDesc().fBlendColor, m_pBlendState->GetBlendDesc().ucSampleMask);
	})
	VSREAL fBlendColor[4] = { 0.0f };
	if (pBlendStateID)
	{

		m_pDeviceContext->OMSetBlendState(pBlendStateID->m_pBlendState, fBlendColor, 0xffffffff);
	}
	else
	{
		m_pDeviceContext->OMSetBlendState(NULL, fBlendColor, 0xffffffff);
	}
	//m_pDeviceContext->OMSetBlendState(pBlendStateID->m_pBlendState, m_pBlendState->GetBlendDesc().fBlendColor, m_pBlendState->GetBlendDesc().ucSampleMask);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::RSSetScissorRects(UINT NumRects, D3D11_RECT *pRects)
{
	D3D11_RECT *pData = pRects;
	if (VSResourceManager::ms_bRenderThread)
	{
		unsigned int uiDataSize = NumRects * sizeof(D3D11_RECT);
		pData = (D3D11_RECT *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiDataSize);
		VSMemcpy(pData, pRects, uiDataSize);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11RSSetScissorRectsCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		D3D11_RECT *, pData, pData, UINT, NumRects, NumRects,
	{
		m_pDeviceContext->RSSetScissorRects(NumRects, pData);
	})
	m_pDeviceContext->RSSetScissorRects(NumRects, pData);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::UpdateSubresource(ID3D11Resource **pDstResource, void *pSrcData, unsigned int uiDataSize)
{
	void *pData = pSrcData;
	if (VSResourceManager::ms_bRenderThread)
	{
		pData = (void *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiDataSize);
		VSMemcpy(pData, pSrcData, uiDataSize);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11UpdateSubresourceCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		void *, pData, pData, ID3D11Resource **, pDstResource, pDstResource,
	{
		m_pDeviceContext->UpdateSubresource(*pDstResource, 0, NULL, pData, 0, 0);
	})
		m_pDeviceContext->UpdateSubresource(*pDstResource, 0, NULL, pData, 0, 0);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::IASetVertexBuffers(VSVBufferID* pVBufferID, unsigned int uiStrides)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11IASetVertexBuffersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSVBufferID*, pVBufferID, pVBufferID, unsigned int, uiStrides, uiStrides,
	{
		if (pVBufferID && uiStrides)
		{
			UINT strides[1] = { uiStrides };
			UINT offset[1] = { 0 };
			ID3D11Buffer* VertexBuffer[1] = { pVBufferID->m_pVertexBuffer };
			m_pDeviceContext->IASetVertexBuffers(0, 1, VertexBuffer, strides, offset);
		}
		else
		{
			m_pDeviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
		}
	})
	if (pVBufferID && uiStrides)
	{
		UINT strides[1] = { uiStrides };
		UINT offset[1] = { 0 };
		ID3D11Buffer* VertexBuffer[1] = { pVBufferID->m_pVertexBuffer };
		m_pDeviceContext->IASetVertexBuffers(0, 1, VertexBuffer, strides, offset);
	}
	else
	{
		m_pDeviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::IASetIndexBuffer(VSIBufferID* pIBufferID, DXGI_FORMAT Format)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11IASetIndexBufferCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSIBufferID*, pIBufferID, pIBufferID, DXGI_FORMAT, Format, Format,
	{
		if (pIBufferID)
		{
			m_pDeviceContext->IASetIndexBuffer(pIBufferID->m_IndexBuffer, Format, 0);
		}
		else
		{
			m_pDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
		}
	})
	if (pIBufferID)
	{
		m_pDeviceContext->IASetIndexBuffer(pIBufferID->m_IndexBuffer, Format, 0);
	}
	else
	{
		m_pDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::GSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11GSSetShaderResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSTextureID *, pTextureID, pTextureID,
		{
		if (pTextureID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
			m_pDeviceContext->GSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->GSSetShaderResources(StartSlot, 1, Temp);
		}
	})
	if (pTextureID)
	{
		ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
		m_pDeviceContext->GSSetShaderResources(StartSlot, 1, Temp);
	}
	else
	{
		ID3D11ShaderResourceView* Temp[1] = { NULL };
		m_pDeviceContext->GSSetShaderResources(StartSlot, 1, Temp);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::DSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11DSSetShaderResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSTextureID *, pTextureID, pTextureID,
		{
		if (pTextureID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
			m_pDeviceContext->DSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->DSSetShaderResources(StartSlot, 1, Temp);
		}
	})
	if (pTextureID)
	{
		ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
		m_pDeviceContext->DSSetShaderResources(StartSlot, 1, Temp);
	}
	else
	{
		ID3D11ShaderResourceView* Temp[1] = { NULL };
		m_pDeviceContext->DSSetShaderResources(StartSlot, 1, Temp);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::HSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11HSSetShaderResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSTextureID *, pTextureID, pTextureID,
		{
		if (pTextureID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
			m_pDeviceContext->HSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->HSSetShaderResources(StartSlot, 1, Temp);
		}
	})
	if (pTextureID)
	{
		ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
		m_pDeviceContext->HSSetShaderResources(StartSlot, 1, Temp);
	}
	else
	{
		ID3D11ShaderResourceView* Temp[1] = { NULL };
		m_pDeviceContext->HSSetShaderResources(StartSlot, 1, Temp);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::VSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11VSSetShaderResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSTextureID *, pTextureID, pTextureID,
	{
		if (pTextureID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
			m_pDeviceContext->VSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->VSSetShaderResources(StartSlot, 1, Temp);
		}
	})
	if (pTextureID)
	{
		ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
		m_pDeviceContext->VSSetShaderResources(StartSlot, 1, Temp);
	}
	else
	{
		ID3D11ShaderResourceView* Temp[1] = { NULL };
		m_pDeviceContext->VSSetShaderResources(StartSlot, 1, Temp);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::VSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11VSSetSamplersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSamplerStateID*, pSamplerStateID, pSamplerStateID,
	{
		if (!pSamplerStateID)
		{
			ID3D11SamplerState* states[1] = { NULL };
			m_pDeviceContext->VSSetSamplers(StartSlot, 1, states);
		}
		else
		{
			ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
			m_pDeviceContext->VSSetSamplers(StartSlot, 1, states);
		}
	})
	if (!pSamplerStateID)
	{
		ID3D11SamplerState* states[1] = { NULL };
		m_pDeviceContext->VSSetSamplers(StartSlot, 1, states);
	}
	else
	{
		ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
		m_pDeviceContext->VSSetSamplers(StartSlot, 1, states);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::DSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11DSSetSamplersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSamplerStateID*, pSamplerStateID, pSamplerStateID,
		{
		if (!pSamplerStateID)
		{
			ID3D11SamplerState* states[1] = { NULL };
			m_pDeviceContext->DSSetSamplers(StartSlot, 1, states);
		}
		else
		{
			ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
			m_pDeviceContext->DSSetSamplers(StartSlot, 1, states);
		}
	})
	if (!pSamplerStateID)
	{
		ID3D11SamplerState* states[1] = { NULL };
		m_pDeviceContext->DSSetSamplers(StartSlot, 1, states);
	}
	else
	{
		ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
		m_pDeviceContext->DSSetSamplers(StartSlot, 1, states);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::HSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11HSSetSamplersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSamplerStateID*, pSamplerStateID, pSamplerStateID,
		{
		if (!pSamplerStateID)
		{
			ID3D11SamplerState* states[1] = { NULL };
			m_pDeviceContext->HSSetSamplers(StartSlot, 1, states);
		}
		else
		{
			ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
			m_pDeviceContext->HSSetSamplers(StartSlot, 1, states);
		}
	})
	if (!pSamplerStateID)
	{
		ID3D11SamplerState* states[1] = { NULL };
		m_pDeviceContext->HSSetSamplers(StartSlot, 1, states);
	}
	else
	{
		ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
		m_pDeviceContext->HSSetSamplers(StartSlot, 1, states);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::GSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11GSSetSamplersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSamplerStateID*, pSamplerStateID, pSamplerStateID,
		{
		if (!pSamplerStateID)
		{
			ID3D11SamplerState* states[1] = { NULL };
			m_pDeviceContext->GSSetSamplers(StartSlot, 1, states);
		}
		else
		{
			ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
			m_pDeviceContext->GSSetSamplers(StartSlot, 1, states);
		}
	})
	if (!pSamplerStateID)
	{
		ID3D11SamplerState* states[1] = { NULL };
		m_pDeviceContext->GSSetSamplers(StartSlot, 1, states);
	}
	else
	{
		ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
		m_pDeviceContext->GSSetSamplers(StartSlot, 1, states);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::PSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11PSSetSamplersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSamplerStateID*, pSamplerStateID, pSamplerStateID,
		{
		if (!pSamplerStateID)
		{
			ID3D11SamplerState* states[1] = { NULL };
			m_pDeviceContext->PSSetSamplers(StartSlot, 1, states);
		}
		else
		{
			ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
			m_pDeviceContext->PSSetSamplers(StartSlot, 1, states);
		}
	})
	if (!pSamplerStateID)
	{
		ID3D11SamplerState* states[1] = { NULL };
		m_pDeviceContext->PSSetSamplers(StartSlot, 1, states);
	}
	else
	{
		ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
		m_pDeviceContext->PSSetSamplers(StartSlot, 1, states);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::SetRenderTargetsEx(VSRenderTargetID *pRenderTarget[], unsigned int uiNum, unsigned int uiBackMacthType)
{
	struct VSSetRenderTargetsExPara
	{
		VSRenderTargetID *pRenderTarget[MAX_RENDER_TARGET];
		unsigned int uiNum;
		unsigned int uiBackMacthType;
	};
	VSSetRenderTargetsExPara SetRenderTargetsExPara;
	for (unsigned int i = 0; i < uiNum; i++)
	{
		SetRenderTargetsExPara.pRenderTarget[i] = pRenderTarget[i];
	}
	SetRenderTargetsExPara.uiNum = uiNum;
	SetRenderTargetsExPara.uiBackMacthType = uiBackMacthType;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11SetRenderTargetsExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSSetRenderTargetsExPara, SetRenderTargetsExPara, SetRenderTargetsExPara,
	{
		ID3D11DepthStencilView * DSV = NULL;
		ID3D11RenderTargetView * RTVTemp[MAX_RENDER_TARGET] = { NULL };
		m_pDeviceContext->OMGetRenderTargets(MAX_RENDER_TARGET, RTVTemp, &DSV);
		for (unsigned int i = 0; i < SetRenderTargetsExPara.uiNum; i++)
		{
			VSRenderTargetID * pRenderTargetID = SetRenderTargetsExPara.pRenderTarget[i];
			pRenderTargetID->m_pSaveRenderTarget = RTVTemp[i];
			RTVTemp[i] = pRenderTargetID->m_pRenderTarget;

		}

		if (SetRenderTargetsExPara.uiBackMacthType == BMT_RENDER_TARGET)
		{
			VSRenderTargetID * pRenderTargetID = SetRenderTargetsExPara.pRenderTarget[0];
			pRenderTargetID->m_pSaveDepthStencilBuffer = DSV;
			m_pDeviceContext->OMSetRenderTargets(SetRenderTargetsExPara.uiNum, RTVTemp, NULL);
		}
		else
		{
			m_pDeviceContext->OMSetRenderTargets(SetRenderTargetsExPara.uiNum, RTVTemp, DSV);
		}

		VSMAC_RELEASE(DSV);
	})
		ID3D11DepthStencilView * DSV = NULL;
	ID3D11RenderTargetView * RTVTemp[MAX_RENDER_TARGET] = { NULL };
	m_pDeviceContext->OMGetRenderTargets(MAX_RENDER_TARGET, RTVTemp, &DSV);
	unsigned int uiNum = 0;
	for (unsigned int i = 0; i < SetRenderTargetsExPara.uiNum; i++)
	{
		uiNum++;
		VSRenderTargetID * pRenderTargetID = SetRenderTargetsExPara.pRenderTarget[i];
		pRenderTargetID->m_pSaveRenderTarget = RTVTemp[i];
		RTVTemp[i] = pRenderTargetID->m_pRenderTarget;

	}
	while (RTVTemp[uiNum] && uiNum < MAX_RENDER_TARGET)
	{
		uiNum++;
	}
	if (SetRenderTargetsExPara.uiBackMacthType == BMT_RENDER_TARGET)
	{
		VSRenderTargetID * pRenderTargetID = SetRenderTargetsExPara.pRenderTarget[0];
		pRenderTargetID->m_pSaveDepthStencilBuffer = DSV;
		m_pDeviceContext->OMSetRenderTargets(uiNum, RTVTemp, NULL);
	}
	else
	{
		m_pDeviceContext->OMSetRenderTargets(uiNum, RTVTemp, DSV);
	}

	VSMAC_RELEASE(DSV);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::EndRenderTargetsEx(VSRenderTargetID *pRenderTarget[], unsigned int uiNum)
{
	struct VSEndRenderTargetsExPara
	{
		VSRenderTargetID *pRenderTarget[MAX_RENDER_TARGET];
		unsigned int uiNum;
	};
	VSEndRenderTargetsExPara EndRenderTargetsExPara;
	for (unsigned int i = 0; i < uiNum; i++)
	{
		EndRenderTargetsExPara.pRenderTarget[i] = pRenderTarget[i];
	}
	EndRenderTargetsExPara.uiNum = uiNum;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11SetRenderTargetsExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSEndRenderTargetsExPara, EndRenderTargetsExPara, EndRenderTargetsExPara,
	{
		ID3D11RenderTargetView * RTVTemp[MAX_RENDER_TARGET] = { NULL };
		ID3D11DepthStencilView * DSV = NULL;
		m_pDeviceContext->OMGetRenderTargets(0, NULL, &DSV);

		for (unsigned int i = 0; i < EndRenderTargetsExPara.uiNum; i++)
		{
			VSRenderTargetID * pRenderTargetID = EndRenderTargetsExPara.pRenderTarget[i];
			VSMAC_ASSERT(pRenderTargetID && pRenderTargetID->m_pRenderTarget);
			RTVTemp[i] = pRenderTargetID->m_pSaveRenderTarget;
			pRenderTargetID->m_pSaveRenderTarget = NULL;
			if (pRenderTargetID->m_pOldTexture && pRenderTargetID->m_pTextureSurface)
			{
				m_pDeviceContext->CopyResource(pRenderTargetID->m_pOldTexture, pRenderTargetID->m_pTextureSurface);
			}
		}
		VSRenderTargetID * pRenderTargetID = EndRenderTargetsExPara.pRenderTarget[0];
		if (pRenderTargetID->m_pSaveDepthStencilBuffer)
		{
			m_pDeviceContext->OMSetRenderTargets(EndRenderTargetsExPara.uiNum, RTVTemp, pRenderTargetID->m_pSaveDepthStencilBuffer);
			pRenderTargetID->m_pSaveDepthStencilBuffer = NULL;
		}
		else
		{
			m_pDeviceContext->OMSetRenderTargets(EndRenderTargetsExPara.uiNum, RTVTemp, DSV);
		}


		VSMAC_RELEASE(DSV);
		for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
		{
			VSMAC_RELEASE(RTVTemp[i]);
		}
	})
	ID3D11RenderTargetView * RTVTemp[MAX_RENDER_TARGET] = { NULL };
	ID3D11DepthStencilView * DSV = NULL;
	m_pDeviceContext->OMGetRenderTargets(0, NULL, &DSV);

	for (unsigned int i = 0; i < EndRenderTargetsExPara.uiNum; i++)
	{
		VSRenderTargetID * pRenderTargetID = EndRenderTargetsExPara.pRenderTarget[i];
		VSMAC_ASSERT(pRenderTargetID && pRenderTargetID->m_pRenderTarget);
		RTVTemp[i] = pRenderTargetID->m_pSaveRenderTarget;
		pRenderTargetID->m_pSaveRenderTarget = NULL;
		if (pRenderTargetID->m_pOldTexture && pRenderTargetID->m_pTextureSurface)
		{
			m_pDeviceContext->CopyResource(pRenderTargetID->m_pOldTexture, pRenderTargetID->m_pTextureSurface);
		}
	}
	VSRenderTargetID * pRenderTargetID = EndRenderTargetsExPara.pRenderTarget[0];
	if (pRenderTargetID->m_pSaveDepthStencilBuffer)
	{
		m_pDeviceContext->OMSetRenderTargets(EndRenderTargetsExPara.uiNum, RTVTemp, pRenderTargetID->m_pSaveDepthStencilBuffer);
		pRenderTargetID->m_pSaveDepthStencilBuffer = NULL;
	}
	else
	{
		m_pDeviceContext->OMSetRenderTargets(EndRenderTargetsExPara.uiNum, RTVTemp, DSV);
	}


	VSMAC_RELEASE(DSV);
	for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
	{
		VSMAC_RELEASE(RTVTemp[i]);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::SetDepthStencilBufferEx(VSDepthStencilID *pDepthStencilID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11SetDepthStencilBufferExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
	VSDepthStencilID *, pDepthStencilID, pDepthStencilID, 
	{
		ID3D11DepthStencilView * DSV = NULL;
		ID3D11RenderTargetView * RTVTemp[MAX_RENDER_TARGET] = { NULL };
		m_pDeviceContext->OMGetRenderTargets(MAX_RENDER_TARGET, RTVTemp, &DSV);
		pDepthStencilID->m_pSaveDepthStencilBuffer = DSV;
		m_pDeviceContext->OMSetRenderTargets(MAX_RENDER_TARGET, RTVTemp, pDepthStencilID->m_pDepthStencilBuffer);
		for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
		{
			VSMAC_RELEASE(RTVTemp[i]);
		}
	})
	ID3D11DepthStencilView * DSV = NULL;
	ID3D11RenderTargetView * RTVTemp[MAX_RENDER_TARGET] = { NULL };
	m_pDeviceContext->OMGetRenderTargets(MAX_RENDER_TARGET, RTVTemp, &DSV);
	pDepthStencilID->m_pSaveDepthStencilBuffer = DSV;
	m_pDeviceContext->OMSetRenderTargets(MAX_RENDER_TARGET, RTVTemp, pDepthStencilID->m_pDepthStencilBuffer);
	for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
	{
		VSMAC_RELEASE(RTVTemp[i]);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::EndDepthStencilBufferEx(VSDepthStencilID *pDepthStencilID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11EndDepthStencilBufferExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSDepthStencilID *, pDepthStencilID, pDepthStencilID,
	{
		ID3D11RenderTargetView * RTVTemp[MAX_RENDER_TARGET] = { NULL };
		m_pDeviceContext->OMGetRenderTargets(MAX_RENDER_TARGET, RTVTemp, NULL);
		m_pDeviceContext->OMSetRenderTargets(MAX_RENDER_TARGET, RTVTemp, pDepthStencilID->m_pSaveDepthStencilBuffer);
		VSMAC_RELEASE(pDepthStencilID->m_pSaveDepthStencilBuffer);
		for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
		{
			VSMAC_RELEASE(RTVTemp[i]);
		}
	})
	ID3D11RenderTargetView * RTVTemp[MAX_RENDER_TARGET] = { NULL };
	m_pDeviceContext->OMGetRenderTargets(MAX_RENDER_TARGET, RTVTemp, NULL);
	m_pDeviceContext->OMSetRenderTargets(MAX_RENDER_TARGET, RTVTemp, pDepthStencilID->m_pSaveDepthStencilBuffer);
	VSMAC_RELEASE(pDepthStencilID->m_pSaveDepthStencilBuffer);
	for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
	{
		VSMAC_RELEASE(RTVTemp[i]);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::VSSetShader(VSVProgramID *pVProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11VSSetShaderCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSVProgramID *,pVProgramID ,pVProgramID,
	{
		if (!pVProgramID)
		{
			m_pDeviceContext->VSSetShader(NULL, NULL, 0);
		}
		else
		{
			m_pDeviceContext->VSSetShader(pVProgramID->m_pVertexShader, NULL, 0);
			if (pVProgramID->m_pShaderBuffer)
			{
				m_pDeviceContext->VSSetConstantBuffers(0, 1, &pVProgramID->m_pShaderBuffer);
			}
		}
	})
	if (!pVProgramID)
	{
		m_pDeviceContext->VSSetShader(NULL, NULL, 0);
	}
	else
	{
		m_pDeviceContext->VSSetShader(pVProgramID->m_pVertexShader, NULL, 0);
		if (pVProgramID->m_pShaderBuffer)
		{
			m_pDeviceContext->VSSetConstantBuffers(0, 1, &pVProgramID->m_pShaderBuffer);
		}
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::GSSetShader(VSGProgramID *pGProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11GSSetShaderCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSGProgramID *, pGProgramID, pGProgramID,
		{
		if (!pGProgramID)
		{
			m_pDeviceContext->GSSetShader(NULL, NULL, 0);
		}
		else
		{
			m_pDeviceContext->GSSetShader(pGProgramID->m_pGeometryShader, NULL, 0);
			if (pGProgramID->m_pShaderBuffer)
			{
				m_pDeviceContext->GSSetConstantBuffers(0, 1, &pGProgramID->m_pShaderBuffer);
			}
		}
	})
	if (!pGProgramID)
	{
		m_pDeviceContext->GSSetShader(NULL, NULL, 0);
	}
	else
	{
		m_pDeviceContext->GSSetShader(pGProgramID->m_pGeometryShader, NULL, 0);
		if (pGProgramID->m_pShaderBuffer)
		{
			m_pDeviceContext->GSSetConstantBuffers(0, 1, &pGProgramID->m_pShaderBuffer);
		}
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::HSSetShader(VSHProgramID *pHProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11HSSetShaderCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSHProgramID *, pHProgramID, pHProgramID,
		{
		if (!pHProgramID)
		{
			m_pDeviceContext->HSSetShader(NULL, NULL, 0);
		}
		else
		{
			m_pDeviceContext->HSSetShader(pHProgramID->m_pHullShader, NULL, 0);
			if (pHProgramID->m_pShaderBuffer)
			{
				m_pDeviceContext->DSSetConstantBuffers(0, 1, &pHProgramID->m_pShaderBuffer);
			}
		}
	})
	if (!pHProgramID)
	{
		m_pDeviceContext->HSSetShader(NULL, NULL, 0);
	}
	else
	{
		m_pDeviceContext->HSSetShader(pHProgramID->m_pHullShader, NULL, 0);
		if (pHProgramID->m_pShaderBuffer)
		{
			m_pDeviceContext->DSSetConstantBuffers(0, 1, &pHProgramID->m_pShaderBuffer);
		}
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::DSSetShader(VSDProgramID *pDProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11DSSetShaderCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSDProgramID *, pDProgramID, pDProgramID,
		{
		if (!pDProgramID)
		{
			m_pDeviceContext->DSSetShader(NULL, NULL, 0);
		}
		else
		{
			m_pDeviceContext->DSSetShader(pDProgramID->m_pDomainShader, NULL, 0);
			if (pDProgramID->m_pShaderBuffer)
			{
				m_pDeviceContext->DSSetConstantBuffers(0, 1, &pDProgramID->m_pShaderBuffer);
			}
		}
	})
	if (!pDProgramID)
	{
		m_pDeviceContext->DSSetShader(NULL, NULL, 0);
	}
	else
	{
		m_pDeviceContext->DSSetShader(pDProgramID->m_pDomainShader, NULL, 0);
		if (pDProgramID->m_pShaderBuffer)
		{
			m_pDeviceContext->DSSetConstantBuffers(0, 1, &pDProgramID->m_pShaderBuffer);
		}
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::PSSetShader(VSPProgramID *pPProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11PSSetShaderCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSPProgramID *, pPProgramID, pPProgramID,
	{
		if (!pPProgramID)
		{
			m_pDeviceContext->PSSetShader(NULL, NULL, 0);
		}
		else
		{
			m_pDeviceContext->PSSetShader(pPProgramID->m_pPixelShader, NULL, 0);
			if (pPProgramID->m_pShaderBuffer)
			{
				m_pDeviceContext->PSSetConstantBuffers(0, 1, &pPProgramID->m_pShaderBuffer);
			}
		}
	})
	if (!pPProgramID)
	{
		m_pDeviceContext->PSSetShader(NULL, NULL, 0);
	}
	else
	{
		m_pDeviceContext->PSSetShader(pPProgramID->m_pPixelShader, NULL, 0);
		if (pPProgramID->m_pShaderBuffer)
		{
			m_pDeviceContext->PSSetConstantBuffers(0, 1, &pPProgramID->m_pShaderBuffer);
		}
	}	
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::OnLoadVShaderFromString(VSVShader * pVShaderProgram, VSVProgramID *pVProgramID)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
// #ifdef _DEBUG
// 	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
// #else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
/*#endif*/
	VSMap<VSString, VSString> Define;
	pVShaderProgram->m_ShaderKey.GetDefine(Define);
	D3D_SHADER_MACRO * pMacro = GetDefine(Define);
	if (pVShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DCompile((LPCSTR)pVShaderProgram->GetBuffer().GetBuffer(), pVShaderProgram->GetBuffer().GetLength(),
			NULL, pMacro, ms_pDx11IncludeShader, pVShaderProgram->GetMainFunName().GetBuffer(),
			ms_cVertexShaderProgramVersion, Flags, 0, &pCode, &pErrors);


	}
	else if (pVShaderProgram->GetResourceName().GetLength())
	{
		VSString RenderAPIPre = VSResourceManager::GetRenderTypeShaderPath(RAT_DIRECTX11);
		VSString Path = VSResourceManager::ms_ShaderPath + RenderAPIPre + pVShaderProgram->GetResourceName().GetString();
		hResult = D3DX11CompileFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx11IncludeShader, pVShaderProgram->GetMainFunName().GetBuffer(),
			ms_cVertexShaderProgramVersion, Flags, 0, NULL, &pCode, &pErrors, NULL);
	}
	else
		VSMAC_ASSERT(0);

	VSMAC_DELETEA(pMacro);
#ifdef _DEBUG
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSResourceManager::ms_OutPutLogPath + _T("VSError.txt");
		Error.Open(VSErrorString.GetBuffer());
		Error.WriteInfo(pVShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		VSOutPutDebugString(pVShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		VSOutPutDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);

	ID3D11ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	VSMAC_ASSERT(!FAILED(hResult));
	if (!pVShaderProgram->m_pUserConstant.GetNum() && !pVShaderProgram->m_pUserSampler.GetNum())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{
				unsigned int uiType = 0;
				if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE1D)
				{
					uiType = VSTexture::TT_1D;
				}
				if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE2D)
				{
					uiType = VSTexture::TT_2D;
				}
				if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE3D)
				{
					uiType = VSTexture::TT_3D;
				}
				if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURECUBE)
				{
					uiType = VSTexture::TT_CUBE;
				}
				else
				{
					VSMAC_ASSERT(0);
				}
				VSUserSampler * pUerSampler = VS_NEW VSUserSampler(resDesc.Name, uiType, resDesc.BindPoint, resDesc.BindCount);
				pVShaderProgram->m_pUserSampler.AddElement(pUerSampler);
			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pVShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);
					ID3D11ShaderReflectionType* pVarType = pVar->GetType();
					VSMAC_ASSERT(pVarType);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					D3D11_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = VSUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = VSUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = VSUserConstant::VT_FLOAT;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					VSUserConstant * pUserConstant =
						VS_NEW VSUserConstant(varDesc.Name, NULL, varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pVShaderProgram->m_pUserConstant.AddElement(pUserConstant);
				}
			}
		}

		pVShaderProgram->m_bCreatePara = true;
	}
	else if (pVShaderProgram->m_bCreatePara == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pVShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pVShaderProgram->m_pUserConstant.GetNum(); i++)
					{
						VSUserConstant * pUserConstant = pVShaderProgram->m_pUserConstant[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							VSMAC_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pVShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &pVProgramID->m_pVertexShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pVShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), pCode->GetBufferSize());
	pVShaderProgram->m_uiArithmeticInstructionSlots = Desc.InstructionCount;
	pVShaderProgram->m_uiTextureInstructionSlots = Desc.TextureBiasInstructions +
		Desc.TextureNormalInstructions + Desc.TextureLoadInstructions + Desc.TextureCompInstructions + Desc.TextureGradientInstructions;
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);

	if (pVShaderProgram->m_uiConstBufferSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = pVShaderProgram->m_uiConstBufferSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pVProgramID->m_pShaderBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
	}
}
void VSDX11Renderer::CreateVShaderFromString(VSVShader * pVShaderProgram, VSVProgramID *pVProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateVShaderFromStringCommand, VSDX11Renderer *, pRenderer, this,
		VSVShader *, pVShaderProgram, pVShaderProgram, VSVProgramID *, pVProgramID, pVProgramID,
	{
		pRenderer->OnLoadVShaderFromString(pVShaderProgram, pVProgramID);
	})
	OnLoadVShaderFromString(pVShaderProgram, pVProgramID);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateVShaderFromCache(VSVShader * pVShaderProgram, VSVProgramID *pVProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateVShaderFromCacheCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSVShader *, pVShaderProgram, pVShaderProgram, VSVProgramID *, pVProgramID, pVProgramID,
	{
		HRESULT hResult = m_pDevice->CreateVertexShader(pVShaderProgram->GetCacheBuffer(), pVShaderProgram->GetCacheBufferSize(), NULL, &pVProgramID->m_pVertexShader);
		VSMAC_ASSERT(!FAILED(hResult));
		if (pVShaderProgram->m_uiConstBufferSize)
		{
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = pVShaderProgram->m_uiConstBufferSize;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;
			HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pVProgramID->m_pShaderBuffer);
			VSMAC_ASSERT(!FAILED(hResult));
		}
	})
	HRESULT hResult = m_pDevice->CreateVertexShader(pVShaderProgram->GetCacheBuffer(), pVShaderProgram->GetCacheBufferSize(), NULL, &pVProgramID->m_pVertexShader);
	VSMAC_ASSERT(!FAILED(hResult));
	if (pVShaderProgram->m_uiConstBufferSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = pVShaderProgram->m_uiConstBufferSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pVProgramID->m_pShaderBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::OnLoadGShaderFromString(VSGShader* pGShaderProgram, VSGProgramID *pGProgramID)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	VSMap<VSString, VSString> Define;
	pGShaderProgram->m_ShaderKey.GetDefine(Define);
	D3D_SHADER_MACRO * pMacro = GetDefine(Define);
	if (pGShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DCompile((LPCSTR)pGShaderProgram->GetBuffer().GetBuffer(), pGShaderProgram->GetBuffer().GetLength(),
			NULL, pMacro, ms_pDx11IncludeShader, pGShaderProgram->GetMainFunName().GetBuffer(),
			ms_cGeometryShaderProgramVersion, Flags, 0, &pCode, &pErrors);


	}
	else if (pGShaderProgram->GetResourceName().GetLength())
	{
		VSString RenderAPIPre = VSResourceManager::GetRenderTypeShaderPath(RAT_DIRECTX11);
		VSString Path = VSResourceManager::ms_ShaderPath + RenderAPIPre + pGShaderProgram->GetResourceName().GetString();
		hResult = D3DX11CompileFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx11IncludeShader, pGShaderProgram->GetMainFunName().GetBuffer(),
			ms_cGeometryShaderProgramVersion, Flags, 0, NULL, &pCode, &pErrors, NULL);
	}
	else
		VSMAC_ASSERT(0);

	VSMAC_DELETEA(pMacro);
#ifdef _DEBUG
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSResourceManager::ms_OutPutLogPath + _T("PSError.txt");
		Error.Open(VSErrorString.GetBuffer());
		Error.WriteInfo(pGShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		VSOutPutDebugString(pGShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		VSOutPutDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);

	ID3D11ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	VSMAC_ASSERT(!FAILED(hResult));
	if (!pGShaderProgram->m_pUserConstant.GetNum() && !pGShaderProgram->m_pUserSampler.GetNum())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{
				unsigned int uiType = 0;
				if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE1D)
				{
					uiType = VSTexture::TT_1D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE2D)
				{
					uiType = VSTexture::TT_2D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE3D)
				{
					uiType = VSTexture::TT_3D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURECUBE)
				{
					uiType = VSTexture::TT_CUBE;
				}
				else
				{
					VSMAC_ASSERT(0);
				}
				VSUserSampler * pUerSampler = VS_NEW VSUserSampler(resDesc.Name, uiType, resDesc.BindPoint, resDesc.BindCount);
				pGShaderProgram->m_pUserSampler.AddElement(pUerSampler);
			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pGShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);
					ID3D11ShaderReflectionType* pVarType = pVar->GetType();
					VSMAC_ASSERT(pVarType);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					D3D11_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = VSUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = VSUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = VSUserConstant::VT_FLOAT;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					VSUserConstant * pUserConstant =
						VS_NEW VSUserConstant(varDesc.Name, NULL, varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pGShaderProgram->m_pUserConstant.AddElement(pUserConstant);
				}
			}
		}

		pGShaderProgram->m_bCreatePara = true;
	}
	else if (pGShaderProgram->m_bCreatePara == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pGShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pGShaderProgram->m_pUserConstant.GetNum(); i++)
					{
						VSUserConstant * pUserConstant = pGShaderProgram->m_pUserConstant[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							VSMAC_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pGShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreateGeometryShader((DWORD*)pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &pGProgramID->m_pGeometryShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pGShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), pCode->GetBufferSize());
	pGShaderProgram->m_uiArithmeticInstructionSlots = Desc.InstructionCount;
	pGShaderProgram->m_uiTextureInstructionSlots = Desc.TextureBiasInstructions +
		Desc.TextureNormalInstructions + Desc.TextureLoadInstructions + Desc.TextureCompInstructions + Desc.TextureGradientInstructions;
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);

	if (pGShaderProgram->m_uiConstBufferSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = pGShaderProgram->m_uiConstBufferSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pGProgramID->m_pShaderBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
	}
}
void VSDX11Renderer::OnLoadHShaderFromString(VSHShader* pHShaderProgram, VSHProgramID *pHProgramID)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	VSMap<VSString, VSString> Define;
	pHShaderProgram->m_ShaderKey.GetDefine(Define);
	D3D_SHADER_MACRO * pMacro = GetDefine(Define);
	if (pHShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DCompile((LPCSTR)pHShaderProgram->GetBuffer().GetBuffer(), pHShaderProgram->GetBuffer().GetLength(),
			NULL, pMacro, ms_pDx11IncludeShader, pHShaderProgram->GetMainFunName().GetBuffer(),
			ms_cGDomainShaderProgramVersion, Flags, 0, &pCode, &pErrors);


	}
	else if (pHShaderProgram->GetResourceName().GetLength())
	{
		VSString RenderAPIPre = VSResourceManager::GetRenderTypeShaderPath(RAT_DIRECTX11);
		VSString Path = VSResourceManager::ms_ShaderPath + RenderAPIPre + pHShaderProgram->GetResourceName().GetString();
		hResult = D3DX11CompileFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx11IncludeShader, pHShaderProgram->GetMainFunName().GetBuffer(),
			ms_cGDomainShaderProgramVersion, Flags, 0, NULL, &pCode, &pErrors, NULL);
	}
	else
		VSMAC_ASSERT(0);

	VSMAC_DELETEA(pMacro);
#ifdef _DEBUG
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSResourceManager::ms_OutPutLogPath + _T("PSError.txt");
		Error.Open(VSErrorString.GetBuffer());
		Error.WriteInfo(pHShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		VSOutPutDebugString(pHShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		VSOutPutDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);

	ID3D11ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	VSMAC_ASSERT(!FAILED(hResult));
	if (!pHShaderProgram->m_pUserConstant.GetNum() && !pHShaderProgram->m_pUserSampler.GetNum())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{
				unsigned int uiType = 0;
				if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE1D)
				{
					uiType = VSTexture::TT_1D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE2D)
				{
					uiType = VSTexture::TT_2D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE3D)
				{
					uiType = VSTexture::TT_3D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURECUBE)
				{
					uiType = VSTexture::TT_CUBE;
				}
				else
				{
					VSMAC_ASSERT(0);
				}
				VSUserSampler * pUerSampler = VS_NEW VSUserSampler(resDesc.Name, uiType, resDesc.BindPoint, resDesc.BindCount);
				pHShaderProgram->m_pUserSampler.AddElement(pUerSampler);
			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pHShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);
					ID3D11ShaderReflectionType* pVarType = pVar->GetType();
					VSMAC_ASSERT(pVarType);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					D3D11_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = VSUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = VSUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = VSUserConstant::VT_FLOAT;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					VSUserConstant * pUserConstant =
						VS_NEW VSUserConstant(varDesc.Name, NULL, varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pHShaderProgram->m_pUserConstant.AddElement(pUserConstant);
				}
			}
		}

		pHShaderProgram->m_bCreatePara = true;
	}
	else if (pHShaderProgram->m_bCreatePara == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pHShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pHShaderProgram->m_pUserConstant.GetNum(); i++)
					{
						VSUserConstant * pUserConstant = pHShaderProgram->m_pUserConstant[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							VSMAC_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pHShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreateHullShader((DWORD*)pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &pHProgramID->m_pHullShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pHShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), pCode->GetBufferSize());
	pHShaderProgram->m_uiArithmeticInstructionSlots = Desc.InstructionCount;
	pHShaderProgram->m_uiTextureInstructionSlots = Desc.TextureBiasInstructions +
		Desc.TextureNormalInstructions + Desc.TextureLoadInstructions + Desc.TextureCompInstructions + Desc.TextureGradientInstructions;
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);

	if (pHShaderProgram->m_uiConstBufferSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = pHShaderProgram->m_uiConstBufferSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pHProgramID->m_pShaderBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
	}
}
void VSDX11Renderer::OnLoadDShaderFromString(VSDShader* pDShaderProgram, VSDProgramID *pDProgramID)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	VSMap<VSString, VSString> Define;
	pDShaderProgram->m_ShaderKey.GetDefine(Define);
	D3D_SHADER_MACRO * pMacro = GetDefine(Define);
	if (pDShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DCompile((LPCSTR)pDShaderProgram->GetBuffer().GetBuffer(), pDShaderProgram->GetBuffer().GetLength(),
			NULL, pMacro, ms_pDx11IncludeShader, pDShaderProgram->GetMainFunName().GetBuffer(),
			ms_cGDomainShaderProgramVersion, Flags, 0, &pCode, &pErrors);


	}
	else if (pDShaderProgram->GetResourceName().GetLength())
	{
		VSString RenderAPIPre = VSResourceManager::GetRenderTypeShaderPath(RAT_DIRECTX11);
		VSString Path = VSResourceManager::ms_ShaderPath + RenderAPIPre + pDShaderProgram->GetResourceName().GetString();
		hResult = D3DX11CompileFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx11IncludeShader, pDShaderProgram->GetMainFunName().GetBuffer(),
			ms_cGDomainShaderProgramVersion, Flags, 0, NULL, &pCode, &pErrors, NULL);
	}
	else
		VSMAC_ASSERT(0);

	VSMAC_DELETEA(pMacro);
#ifdef _DEBUG
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSResourceManager::ms_OutPutLogPath + _T("PSError.txt");
		Error.Open(VSErrorString.GetBuffer());
		Error.WriteInfo(pDShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		VSOutPutDebugString(pDShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		VSOutPutDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);

	ID3D11ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	VSMAC_ASSERT(!FAILED(hResult));
	if (!pDShaderProgram->m_pUserConstant.GetNum() && !pDShaderProgram->m_pUserSampler.GetNum())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{
				unsigned int uiType = 0;
				if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE1D)
				{
					uiType = VSTexture::TT_1D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE2D)
				{
					uiType = VSTexture::TT_2D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE3D)
				{
					uiType = VSTexture::TT_3D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURECUBE)
				{
					uiType = VSTexture::TT_CUBE;
				}
				else
				{
					VSMAC_ASSERT(0);
				}
				VSUserSampler * pUerSampler = VS_NEW VSUserSampler(resDesc.Name, uiType, resDesc.BindPoint, resDesc.BindCount);
				pDShaderProgram->m_pUserSampler.AddElement(pUerSampler);
			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pDShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);
					ID3D11ShaderReflectionType* pVarType = pVar->GetType();
					VSMAC_ASSERT(pVarType);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					D3D11_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = VSUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = VSUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = VSUserConstant::VT_FLOAT;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					VSUserConstant * pUserConstant =
						VS_NEW VSUserConstant(varDesc.Name, NULL, varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pDShaderProgram->m_pUserConstant.AddElement(pUserConstant);
				}
			}
		}

		pDShaderProgram->m_bCreatePara = true;
	}
	else if (pDShaderProgram->m_bCreatePara == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pDShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pDShaderProgram->m_pUserConstant.GetNum(); i++)
					{
						VSUserConstant * pUserConstant = pDShaderProgram->m_pUserConstant[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							VSMAC_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pDShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreateDomainShader((DWORD*)pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &pDProgramID->m_pDomainShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pDShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), pCode->GetBufferSize());
	pDShaderProgram->m_uiArithmeticInstructionSlots = Desc.InstructionCount;
	pDShaderProgram->m_uiTextureInstructionSlots = Desc.TextureBiasInstructions +
		Desc.TextureNormalInstructions + Desc.TextureLoadInstructions + Desc.TextureCompInstructions + Desc.TextureGradientInstructions;
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);

	if (pDShaderProgram->m_uiConstBufferSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = pDShaderProgram->m_uiConstBufferSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pDProgramID->m_pShaderBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
	}
}
void VSDX11Renderer::OnLoadPShaderFromString(VSPShader* pPShaderProgram, VSPProgramID *pPProgramID)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
// #ifdef _DEBUG
// 	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
// #else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
/*#endif*/
	VSMap<VSString, VSString> Define;
	pPShaderProgram->m_ShaderKey.GetDefine(Define);
	D3D_SHADER_MACRO * pMacro = GetDefine(Define);
	if (pPShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DCompile((LPCSTR)pPShaderProgram->GetBuffer().GetBuffer(), pPShaderProgram->GetBuffer().GetLength(),
			NULL, pMacro, ms_pDx11IncludeShader, pPShaderProgram->GetMainFunName().GetBuffer(),
			ms_cPixelShaderProgramVersion, Flags, 0, &pCode, &pErrors);


	}
	else if (pPShaderProgram->GetResourceName().GetLength())
	{
		VSString RenderAPIPre = VSResourceManager::GetRenderTypeShaderPath(RAT_DIRECTX11);
		VSString Path = VSResourceManager::ms_ShaderPath + RenderAPIPre + pPShaderProgram->GetResourceName().GetString();
		hResult = D3DX11CompileFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx11IncludeShader, pPShaderProgram->GetMainFunName().GetBuffer(),
			ms_cPixelShaderProgramVersion, Flags, 0, NULL, &pCode, &pErrors, NULL);
	}
	else
		VSMAC_ASSERT(0);

	VSMAC_DELETEA(pMacro);
#ifdef _DEBUG
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSResourceManager::ms_OutPutLogPath + _T("PSError.txt");
		Error.Open(VSErrorString.GetBuffer());
		Error.WriteInfo(pPShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		VSOutPutDebugString(pPShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		VSOutPutDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);

	ID3D11ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	VSMAC_ASSERT(!FAILED(hResult));
	if (!pPShaderProgram->m_pUserConstant.GetNum() && !pPShaderProgram->m_pUserSampler.GetNum())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{
				unsigned int uiType = 0;
				if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE1D)
				{
					uiType = VSTexture::TT_1D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE2D)
				{
					uiType = VSTexture::TT_2D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE3D)
				{
					uiType = VSTexture::TT_3D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURECUBE)
				{
					uiType = VSTexture::TT_CUBE;
				}
				else
				{
					VSMAC_ASSERT(0);
				}
				VSUserSampler * pUerSampler = VS_NEW VSUserSampler(resDesc.Name, uiType, resDesc.BindPoint, resDesc.BindCount);
				pPShaderProgram->m_pUserSampler.AddElement(pUerSampler);
			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pPShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);
					ID3D11ShaderReflectionType* pVarType = pVar->GetType();
					VSMAC_ASSERT(pVarType);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					D3D11_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = VSUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = VSUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = VSUserConstant::VT_FLOAT;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					VSUserConstant * pUserConstant =
						VS_NEW VSUserConstant(varDesc.Name, NULL, varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pPShaderProgram->m_pUserConstant.AddElement(pUserConstant);
				}
			}
		}

		pPShaderProgram->m_bCreatePara = true;
	}
	else if (pPShaderProgram->m_bCreatePara == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pPShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pPShaderProgram->m_pUserConstant.GetNum(); i++)
					{
						VSUserConstant * pUserConstant = pPShaderProgram->m_pUserConstant[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							VSMAC_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pPShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &pPProgramID->m_pPixelShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pPShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), pCode->GetBufferSize());
	pPShaderProgram->m_uiArithmeticInstructionSlots = Desc.InstructionCount;
	pPShaderProgram->m_uiTextureInstructionSlots = Desc.TextureBiasInstructions +
		Desc.TextureNormalInstructions + Desc.TextureLoadInstructions + Desc.TextureCompInstructions + Desc.TextureGradientInstructions;
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);

	if (pPShaderProgram->m_uiConstBufferSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = pPShaderProgram->m_uiConstBufferSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pPProgramID->m_pShaderBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
	}
}
void VSDX11Renderer::CreatePShaderFromString(VSPShader* pPShaderProgram, VSPProgramID *pPProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreatePShaderFromStringCommand, VSDX11Renderer *, pRenderer, this,
		VSPShader *, pPShaderProgram, pPShaderProgram, VSPProgramID *, pPProgramID, pPProgramID,
	{
		pRenderer->OnLoadPShaderFromString(pPShaderProgram, pPProgramID);
	})
	OnLoadPShaderFromString(pPShaderProgram, pPProgramID);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateGShaderFromString(VSGShader* pGShaderProgram, VSGProgramID *pGProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateGShaderFromStringCommand, VSDX11Renderer *, pRenderer, this,
		VSGShader *, pGShaderProgram, pGShaderProgram, VSGProgramID *, pGProgramID, pGProgramID,
		{
		pRenderer->OnLoadGShaderFromString(pGShaderProgram, pGProgramID);
	})
		OnLoadGShaderFromString(pGShaderProgram, pGProgramID);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateDShaderFromString(VSDShader* pDShaderProgram, VSDProgramID *pDProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateDShaderFromStringCommand, VSDX11Renderer *, pRenderer, this,
		VSDShader *, pDShaderProgram, pDShaderProgram, VSDProgramID *, pDProgramID, pDProgramID,
		{
		pRenderer->OnLoadDShaderFromString(pDShaderProgram, pDProgramID);
	})
		OnLoadDShaderFromString(pDShaderProgram, pDProgramID);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateHShaderFromString(VSHShader* pHShaderProgram, VSHProgramID *pHProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateHShaderFromStringCommand, VSDX11Renderer *, pRenderer, this,
		VSHShader *, pHShaderProgram, pHShaderProgram, VSHProgramID *, pHProgramID, pHProgramID,
		{
		pRenderer->OnLoadHShaderFromString(pHShaderProgram, pHProgramID);
	})
		OnLoadHShaderFromString(pHShaderProgram, pHProgramID);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateGShaderFromCache(VSGShader* pGShaderProgram, VSGProgramID *pGProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateGShaderFromCacheCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSGShader *, pGShaderProgram, pGShaderProgram, VSGProgramID *, pGProgramID, pGProgramID,
		{
		HRESULT hResult = m_pDevice->CreateGeometryShader(pGShaderProgram->GetCacheBuffer(), pGShaderProgram->GetCacheBufferSize(), NULL, &pGProgramID->m_pGeometryShader);
		VSMAC_ASSERT(!FAILED(hResult));
		if (pGShaderProgram->m_uiConstBufferSize)
		{
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = pGShaderProgram->m_uiConstBufferSize;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;
			HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pGProgramID->m_pShaderBuffer);
			VSMAC_ASSERT(!FAILED(hResult));
		}
	})
		HRESULT hResult = m_pDevice->CreateGeometryShader(pGShaderProgram->GetCacheBuffer(), pGShaderProgram->GetCacheBufferSize(), NULL, &pGProgramID->m_pGeometryShader);
	VSMAC_ASSERT(!FAILED(hResult));
	if (pGShaderProgram->m_uiConstBufferSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = pGShaderProgram->m_uiConstBufferSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pGProgramID->m_pShaderBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateHShaderFromCache(VSHShader* pHShaderProgram, VSHProgramID *pHProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateHShaderFromCacheCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSHShader *, pHShaderProgram, pHShaderProgram, VSHProgramID *, pHProgramID, pHProgramID,
		{
		HRESULT hResult = m_pDevice->CreateHullShader(pHShaderProgram->GetCacheBuffer(), pHShaderProgram->GetCacheBufferSize(), NULL, &pHProgramID->m_pHullShader);
		VSMAC_ASSERT(!FAILED(hResult));
		if (pHShaderProgram->m_uiConstBufferSize)
		{
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = pHShaderProgram->m_uiConstBufferSize;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;
			HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pHProgramID->m_pShaderBuffer);
			VSMAC_ASSERT(!FAILED(hResult));
		}
	})
		HRESULT hResult = m_pDevice->CreateHullShader(pHShaderProgram->GetCacheBuffer(), pHShaderProgram->GetCacheBufferSize(), NULL, &pHProgramID->m_pHullShader);
	VSMAC_ASSERT(!FAILED(hResult));
	if (pHShaderProgram->m_uiConstBufferSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = pHShaderProgram->m_uiConstBufferSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pHProgramID->m_pShaderBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateDShaderFromCache(VSDShader* pDShaderProgram, VSDProgramID *pDProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateDShaderFromCacheCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSDShader *, pDShaderProgram, pDShaderProgram, VSDProgramID *, pDProgramID, pDProgramID,
		{
		HRESULT hResult = m_pDevice->CreateDomainShader(pDShaderProgram->GetCacheBuffer(), pDShaderProgram->GetCacheBufferSize(), NULL, &pDProgramID->m_pDomainShader);
		VSMAC_ASSERT(!FAILED(hResult));
		if (pDShaderProgram->m_uiConstBufferSize)
		{
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = pDShaderProgram->m_uiConstBufferSize;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;
			HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pDProgramID->m_pShaderBuffer);
			VSMAC_ASSERT(!FAILED(hResult));
		}
	})
		HRESULT hResult = m_pDevice->CreateDomainShader(pDShaderProgram->GetCacheBuffer(), pDShaderProgram->GetCacheBufferSize(), NULL, &pDProgramID->m_pDomainShader);
	VSMAC_ASSERT(!FAILED(hResult));
	if (pDShaderProgram->m_uiConstBufferSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = pDShaderProgram->m_uiConstBufferSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pDProgramID->m_pShaderBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreatePShaderFromCache(VSPShader* pPShaderProgram, VSPProgramID *pPProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreatePShaderFromCacheCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSPShader *, pPShaderProgram, pPShaderProgram, VSPProgramID *, pPProgramID, pPProgramID,
	{
		HRESULT hResult = m_pDevice->CreatePixelShader(pPShaderProgram->GetCacheBuffer(), pPShaderProgram->GetCacheBufferSize(), NULL, &pPProgramID->m_pPixelShader);
		VSMAC_ASSERT(!FAILED(hResult));
		if (pPShaderProgram->m_uiConstBufferSize)
		{
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = pPShaderProgram->m_uiConstBufferSize;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;
			HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pPProgramID->m_pShaderBuffer);
			VSMAC_ASSERT(!FAILED(hResult));
		}
	})
	HRESULT hResult = m_pDevice->CreatePixelShader(pPShaderProgram->GetCacheBuffer(), pPShaderProgram->GetCacheBufferSize(), NULL, &pPProgramID->m_pPixelShader);
	VSMAC_ASSERT(!FAILED(hResult));
	if (pPShaderProgram->m_uiConstBufferSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = pPShaderProgram->m_uiConstBufferSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pPProgramID->m_pShaderBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	
}
void VSDX11Renderer::CreateTexture1D(VS1DTexture *pTexture, D3D11_TEXTURE1D_DESC &desc, VSTextureID * pTextureID)
{
	struct VSCreateTexture1DPara
	{
		VS1DTexture *pTexture;
		D3D11_TEXTURE1D_DESC desc;
		VSTextureID * pTextureID;
	};
	VSCreateTexture1DPara CreateTexture1DPara;
	CreateTexture1DPara.desc = desc;
	CreateTexture1DPara.pTexture = pTexture;
	CreateTexture1DPara.pTextureID = pTextureID;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11CreateTexture1DCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSCreateTexture1DPara, CreateTexture1DPara, CreateTexture1DPara,
	{
		VS1DTexture *pTexture = CreateTexture1DPara.pTexture;
		D3D11_TEXTURE1D_DESC desc = CreateTexture1DPara.desc;
		VSTextureID * pTextureID = CreateTexture1DPara.pTextureID;

		HRESULT hResult = NULL;
		VSArray<D3D11_SUBRESOURCE_DATA> data;
		data.SetBufferNum(pTexture->GetMipLevel());
		bool HasData = false;
		for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
		{
			if (!pTexture->GetBuffer(i))
			{
				continue;
			}
			HasData = true;
			data[i].pSysMem = pTexture->GetBuffer(i);
			data[i].SysMemPitch = 0;
			data[i].SysMemSlicePitch = 0;
		}

		if (HasData == true)
		{
			VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
			hResult = m_pDevice->CreateTexture1D(&desc, data.GetBuffer(), (ID3D11Texture1D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		else
		{
			hResult = m_pDevice->CreateTexture1D(&desc, NULL, (ID3D11Texture1D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
		RSVdesc.Format = desc.Format;
		RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		RSVdesc.Texture2D.MostDetailedMip = 0;
		RSVdesc.Texture2D.MipLevels = desc.MipLevels;

		HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	HRESULT hResult = NULL;
	VSArray<D3D11_SUBRESOURCE_DATA> data;
	data.SetBufferNum(pTexture->GetMipLevel());
	bool HasData = false;
	for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
	{
		if (!pTexture->GetBuffer(i))
		{
			continue;
		}
		HasData = true;
		data[i].pSysMem = pTexture->GetBuffer(i);
		data[i].SysMemPitch = 0;
		data[i].SysMemSlicePitch = 0;
	}

	if (HasData == true)
	{
		VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
		hResult = m_pDevice->CreateTexture1D(&desc, data.GetBuffer(), (ID3D11Texture1D **)&pTextureID->m_pTexture);
		VSMAC_ASSERT(!FAILED(hResult));
	}
	else
	{
		hResult = m_pDevice->CreateTexture1D(&desc, NULL, (ID3D11Texture1D **)&pTextureID->m_pTexture);
		VSMAC_ASSERT(!FAILED(hResult));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
	RSVdesc.Format = desc.Format;
	RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	RSVdesc.Texture2D.MostDetailedMip = 0;
	RSVdesc.Texture2D.MipLevels = desc.MipLevels;

	HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END

}
void VSDX11Renderer::CreateTextureCube(VSCubeTexture *pTexture, D3D11_TEXTURE2D_DESC &desc, bool bRT, VSTextureID * pTextureID)
{
	struct VSCreateTextureCubePara
	{
		VSCubeTexture *pTexture;
		D3D11_TEXTURE2D_DESC desc;
		bool bRT;
		VSTextureID * pTextureID;
	};
	VSCreateTextureCubePara CreateTextureCubePara;
	CreateTextureCubePara.desc = desc;
	CreateTextureCubePara.pTexture = pTexture;
	CreateTextureCubePara.pTextureID = pTextureID;
	CreateTextureCubePara.bRT = bRT;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11CreateTextureCubeCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSCreateTextureCubePara, CreateTextureCubePara, CreateTextureCubePara,
	{
		VSCubeTexture *pTexture = CreateTextureCubePara.pTexture;
		D3D11_TEXTURE2D_DESC desc = CreateTextureCubePara.desc;
		bool bRT = CreateTextureCubePara.bRT;
		VSTextureID * pTextureID = CreateTextureCubePara.pTextureID;

		if (bRT)
		{
			UINT QualityLevel = 0;
			m_pDevice->CheckMultisampleQualityLevels(desc.Format, desc.SampleDesc.Count, &QualityLevel);
			desc.SampleDesc.Quality = QualityLevel - 1;
		}
		HRESULT hResult = NULL;
		VSArray<D3D11_SUBRESOURCE_DATA> data;
		data.SetBufferNum(pTexture->GetMipLevel());
		bool HasData = false;
		for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
		{
			if (!pTexture->GetBuffer(i))
			{
				continue;
			}
			HasData = true;
			data[i].pSysMem = pTexture->GetBuffer(i);
			data[i].SysMemPitch = pTexture->GetMemPitch(i);
			data[i].SysMemSlicePitch = 0;
		}

		if (HasData == true)
		{
			VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
			hResult = m_pDevice->CreateTexture2D(&desc, data.GetBuffer(), (ID3D11Texture2D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		else
		{
			hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
		RSVdesc.Format = desc.Format;
		RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		RSVdesc.Texture2D.MostDetailedMip = 0;
		RSVdesc.Texture2D.MipLevels = desc.MipLevels;

		HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	if (bRT)
	{
		UINT QualityLevel = 0;
		m_pDevice->CheckMultisampleQualityLevels(desc.Format, desc.SampleDesc.Count, &QualityLevel);
		desc.SampleDesc.Quality = QualityLevel - 1;
	}
	HRESULT hResult = NULL;
	VSArray<D3D11_SUBRESOURCE_DATA> data;
	data.SetBufferNum(pTexture->GetMipLevel());
	bool HasData = false;
	for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
	{
		if (!pTexture->GetBuffer(i))
		{
			continue;
		}
		HasData = true;
		data[i].pSysMem = pTexture->GetBuffer(i);
		data[i].SysMemPitch = pTexture->GetMemPitch(i);
		data[i].SysMemSlicePitch = 0;
	}

	if (HasData == true)
	{
		VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
		hResult = m_pDevice->CreateTexture2D(&desc, data.GetBuffer(), (ID3D11Texture2D **)&pTextureID->m_pTexture);
		VSMAC_ASSERT(!FAILED(hResult));
	}
	else
	{
		hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pTextureID->m_pTexture);
		VSMAC_ASSERT(!FAILED(hResult));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
	RSVdesc.Format = desc.Format;
	RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	RSVdesc.Texture2D.MostDetailedMip = 0;
	RSVdesc.Texture2D.MipLevels = desc.MipLevels;

	HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateTexture3D(VS3DTexture *pTexture, D3D11_TEXTURE3D_DESC &desc, VSTextureID * pTextureID)
{
	struct VSCreateTexture3DPara
	{
		VS3DTexture *pTexture;
		D3D11_TEXTURE3D_DESC desc;
		VSTextureID * pTextureID;
	};
	VSCreateTexture3DPara CreateTexture3DPara;
	CreateTexture3DPara.desc = desc;
	CreateTexture3DPara.pTexture = pTexture;
	CreateTexture3DPara.pTextureID = pTextureID;

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11CreateTexture3DCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSCreateTexture3DPara, CreateTexture3DPara, CreateTexture3DPara,
		{
		VS3DTexture *pTexture = CreateTexture3DPara.pTexture;
		D3D11_TEXTURE3D_DESC desc = CreateTexture3DPara.desc;
		VSTextureID * pTextureID = CreateTexture3DPara.pTextureID;
		HRESULT hResult = NULL;
		VSArray<D3D11_SUBRESOURCE_DATA> data;
		data.SetBufferNum(pTexture->GetMipLevel());
		bool HasData = false;
		for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
		{
			if (!pTexture->GetBuffer(i))
			{
				continue;
			}
			HasData = true;
			data[i].pSysMem = pTexture->GetBuffer(i);
			data[i].SysMemPitch = pTexture->GetMemPitch(i);
			data[i].SysMemSlicePitch = pTexture->GetMemSlicePitch(i);
		}

		if (HasData == true)
		{
			VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
			hResult = m_pDevice->CreateTexture3D(&desc, data.GetBuffer(), (ID3D11Texture3D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		else
		{
			hResult = m_pDevice->CreateTexture3D(&desc, NULL, (ID3D11Texture3D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
		RSVdesc.Format = desc.Format;
		RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		RSVdesc.Texture2D.MostDetailedMip = 0;
		RSVdesc.Texture2D.MipLevels = desc.MipLevels;

		HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	HRESULT hResult = NULL;
	VSArray<D3D11_SUBRESOURCE_DATA> data;
	data.SetBufferNum(pTexture->GetMipLevel());
	bool HasData = false;
	for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
	{
		if (!pTexture->GetBuffer(i))
		{
			continue;
		}
		HasData = true;
		data[i].pSysMem = pTexture->GetBuffer(i);
		data[i].SysMemPitch = pTexture->GetMemPitch(i);
		data[i].SysMemSlicePitch = pTexture->GetMemSlicePitch(i);
	}

	if (HasData == true)
	{
		VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
		hResult = m_pDevice->CreateTexture3D(&desc, data.GetBuffer(), (ID3D11Texture3D **)&pTextureID->m_pTexture);
		VSMAC_ASSERT(!FAILED(hResult));
	}
	else
	{
		hResult = m_pDevice->CreateTexture3D(&desc, NULL, (ID3D11Texture3D **)&pTextureID->m_pTexture);
		VSMAC_ASSERT(!FAILED(hResult));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
	RSVdesc.Format = desc.Format;
	RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
	RSVdesc.Texture2D.MostDetailedMip = 0;
	RSVdesc.Texture2D.MipLevels = desc.MipLevels;

	HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateTexture2D(VS2DTexture *pTexture, D3D11_TEXTURE2D_DESC &desc, bool bRT, VSTextureID * pTextureID)
{
	struct VSCreateTexture2DPara
	{
		VS2DTexture *pTexture;
		D3D11_TEXTURE2D_DESC desc;
		bool bRT;
		VSTextureID * pTextureID;
	};
	VSCreateTexture2DPara CreateTexture2DPara;
	CreateTexture2DPara.desc = desc;
	CreateTexture2DPara.pTexture = pTexture;
	CreateTexture2DPara.pTextureID = pTextureID;
	CreateTexture2DPara.bRT = bRT;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11CreateTexture2DCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSCreateTexture2DPara, CreateTexture2DPara, CreateTexture2DPara,
	{
		VS2DTexture *pTexture = CreateTexture2DPara.pTexture;
		D3D11_TEXTURE2D_DESC desc = CreateTexture2DPara.desc;
		bool bRT = CreateTexture2DPara.bRT;
		VSTextureID * pTextureID = CreateTexture2DPara.pTextureID;
		if (bRT)
		{
			UINT QualityLevel = 0;
			m_pDevice->CheckMultisampleQualityLevels(desc.Format, desc.SampleDesc.Count, &QualityLevel);
			desc.SampleDesc.Quality = QualityLevel - 1;
		}

		HRESULT hResult = NULL;
		VSArray<D3D11_SUBRESOURCE_DATA> data;
		data.SetBufferNum(pTexture->GetMipLevel());
		bool HasData = false;
		for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
		{
			if (!pTexture->GetBuffer(i))
			{
				continue;
			}
			HasData = true;
			data[i].pSysMem = pTexture->GetBuffer(i);
			data[i].SysMemPitch = pTexture->GetMemPitch(i);
			data[i].SysMemSlicePitch = 0;
		}

		if (HasData == true)
		{
			VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
			hResult = m_pDevice->CreateTexture2D(&desc, data.GetBuffer(), (ID3D11Texture2D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		else
		{
			hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
		RSVdesc.Format = desc.Format;
		RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		RSVdesc.Texture2D.MostDetailedMip = 0;
		RSVdesc.Texture2D.MipLevels = desc.MipLevels;

		HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	if (bRT)
	{
		UINT QualityLevel = 0;
		m_pDevice->CheckMultisampleQualityLevels(desc.Format, desc.SampleDesc.Count, &QualityLevel);
		desc.SampleDesc.Quality = QualityLevel - 1;
	}

	HRESULT hResult = NULL;
	VSArray<D3D11_SUBRESOURCE_DATA> data;
	data.SetBufferNum(pTexture->GetMipLevel());
	bool HasData = false;
	for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
	{
		if (!pTexture->GetBuffer(i))
		{
			continue;
		}
		HasData = true;
		data[i].pSysMem = pTexture->GetBuffer(i);
		data[i].SysMemPitch = pTexture->GetMemPitch(i);
		data[i].SysMemSlicePitch = 0;
	}

	if (HasData == true)
	{
		VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
		hResult = m_pDevice->CreateTexture2D(&desc, data.GetBuffer(), (ID3D11Texture2D **)&pTextureID->m_pTexture);
		VSMAC_ASSERT(!FAILED(hResult));
	}
	else
	{
		hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pTextureID->m_pTexture);
		VSMAC_ASSERT(!FAILED(hResult));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
	RSVdesc.Format = desc.Format;
	RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	RSVdesc.Texture2D.MostDetailedMip = 0;
	RSVdesc.Texture2D.MipLevels = desc.MipLevels;

	HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateDepth(D3D11_TEXTURE2D_DESC &desc, VSDepthStencilID * pDepthStencilID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateDepthCommand, ID3D11Device*, m_pDevice, m_pDevice,
		D3D11_TEXTURE2D_DESC, desc, desc, VSDepthStencilID * ,pDepthStencilID, pDepthStencilID,
	{
		UINT QualityLevel = 0;
		m_pDevice->CheckMultisampleQualityLevels(desc.Format, desc.SampleDesc.Count, &QualityLevel);
		desc.SampleDesc.Quality = QualityLevel - 1;

		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pDepthStencilID->m_pTexture);
		VSMAC_ASSERT(!FAILED(hResult));


		D3D11_DEPTH_STENCIL_VIEW_DESC DSVdesc;
		DSVdesc.Format = desc.Format;
		DSVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		DSVdesc.Flags = 0;
		DSVdesc.Texture2D.MipSlice = 0;

		HRESULT hr = m_pDevice->CreateDepthStencilView(pDepthStencilID->m_pTexture, &DSVdesc, &pDepthStencilID->m_pDepthStencilBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	UINT QualityLevel = 0;
	m_pDevice->CheckMultisampleQualityLevels(desc.Format, desc.SampleDesc.Count, &QualityLevel);
	desc.SampleDesc.Quality = QualityLevel - 1;

	HRESULT hResult = NULL;
	hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pDepthStencilID->m_pTexture);
	VSMAC_ASSERT(!FAILED(hResult));


	D3D11_DEPTH_STENCIL_VIEW_DESC DSVdesc;
	DSVdesc.Format = desc.Format;
	DSVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVdesc.Flags = 0;
	DSVdesc.Texture2D.MipSlice = 0;

	HRESULT hr = m_pDevice->CreateDepthStencilView(pDepthStencilID->m_pTexture, &DSVdesc, &pDepthStencilID->m_pDepthStencilBuffer);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END

}
void VSDX11Renderer::OnLoadRenderTargetEx(unsigned int Width, unsigned int Height, DXGI_FORMAT Format, unsigned int Count,
	unsigned int LockFlag, unsigned int uiTextureType, unsigned int uiRenderTargetLevel, unsigned int uiCubeFace,
	VSTextureID * pTextureID, VSRenderTargetID *pRenderTargetID)
{
	struct VSOnLoadRenderTargetExPara
	{
		unsigned int Width;
		unsigned int Height;
		DXGI_FORMAT Format;
		unsigned int Count;
		unsigned int LockFlag;
		unsigned int uiTextureType;
		unsigned int uiRenderTargetLevel;
		unsigned int uiCubeFace;
		VSTextureID * pTextureID;
		VSRenderTargetID *pRenderTargetID;
	};
	VSOnLoadRenderTargetExPara OnLoadRenderTargetExPara;
	OnLoadRenderTargetExPara.Width = Width;
	OnLoadRenderTargetExPara.Height = Height;
	OnLoadRenderTargetExPara.Format = Format;
	OnLoadRenderTargetExPara.Count = Count;
	OnLoadRenderTargetExPara.LockFlag = LockFlag;
	OnLoadRenderTargetExPara.uiTextureType = uiTextureType;
	OnLoadRenderTargetExPara.uiRenderTargetLevel = uiRenderTargetLevel;
	OnLoadRenderTargetExPara.uiCubeFace = uiCubeFace;
	OnLoadRenderTargetExPara.pTextureID = pTextureID;
	OnLoadRenderTargetExPara.pRenderTargetID = pRenderTargetID;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OnLoadRenderTargetExCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSOnLoadRenderTargetExPara, OnLoadRenderTargetExPara, OnLoadRenderTargetExPara,
	{
		unsigned int Width = OnLoadRenderTargetExPara.Width;
		unsigned int Height = OnLoadRenderTargetExPara.Height;
		DXGI_FORMAT Format = OnLoadRenderTargetExPara.Format;
		unsigned int Count = OnLoadRenderTargetExPara.Count;
		unsigned int LockFlag = OnLoadRenderTargetExPara.LockFlag;
		unsigned int uiTextureType = OnLoadRenderTargetExPara.uiTextureType;
		unsigned int uiRenderTargetLevel = OnLoadRenderTargetExPara.uiRenderTargetLevel;
		unsigned int uiCubeFace = OnLoadRenderTargetExPara.uiCubeFace;
		VSTextureID * pTextureID = OnLoadRenderTargetExPara.pTextureID;
		VSRenderTargetID *pRenderTargetID = OnLoadRenderTargetExPara.pRenderTargetID;

		HRESULT hResult = NULL;
		UINT QualityLevel = 0;
		m_pDevice->CheckMultisampleQualityLevels(Format, Count, &QualityLevel);
		unsigned int Quality = QualityLevel - 1;
		bool b1 = false;
		bool b2 = false;
		ID3D11Texture2D * pTempTexture = NULL;
		if (pTextureID)
		{
			D3D11_TEXTURE2D_DESC Desc;
			pTempTexture = ((ID3D11Texture2D *)(pTextureID->m_pTexture));
			pTempTexture->GetDesc(&Desc);
			if (Desc.SampleDesc.Count == Count)
			{
				b1 = true;
			}
			else
			{
				b2 = true;
			}
		}
		else
		{
			b2 = true;
		}

		if (LockFlag == VSBind::LF_READONLY)
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = Width;
			desc.Height = Height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = Format;
			desc.SampleDesc.Count = Count;
			desc.SampleDesc.Quality = Quality;
			desc.BindFlags = 0;
			desc.MiscFlags = 0;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
			hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pRenderTargetID->m_pStagTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}

		pRenderTargetID->m_pOldTexture = pTempTexture;
		if (b1)
		{
			D3D11_RENDER_TARGET_VIEW_DESC desc;
			desc.Format = Format;
			if (uiTextureType == VSTexture::TT_2D)
			{
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = uiRenderTargetLevel;
			}
			else if (uiTextureType == VSTexture::TT_CUBE)
			{
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = uiRenderTargetLevel;
				desc.Texture2DArray.ArraySize = 1;
				desc.Texture2DArray.FirstArraySlice = uiCubeFace;
			}

			hResult = m_pDevice->CreateRenderTargetView(pTempTexture, &desc, &pRenderTargetID->m_pRenderTarget);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		if (b2)
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = Width;
			desc.Height = Height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = Format;
			desc.SampleDesc.Count = Count;
			desc.SampleDesc.Quality = Quality;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET;
			desc.MiscFlags = 0;
			desc.CPUAccessFlags = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;



			hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pRenderTargetID->m_pTextureSurface);
			VSMAC_ASSERT(!FAILED(hResult));

			D3D11_RENDER_TARGET_VIEW_DESC RTVdesc;
			RTVdesc.Format = Format;
			RTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			RTVdesc.Texture2D.MipSlice = 0;

			hResult = m_pDevice->CreateRenderTargetView(pRenderTargetID->m_pTextureSurface, &RTVdesc, &pRenderTargetID->m_pRenderTarget);
			VSMAC_ASSERT(!FAILED(hResult));
		}
	})
	HRESULT hResult = NULL;
	UINT QualityLevel = 0;
	m_pDevice->CheckMultisampleQualityLevels(Format, Count, &QualityLevel);
	unsigned int Quality = QualityLevel - 1;
	bool b1 = false;
	bool b2 = false;
	ID3D11Texture2D * pTempTexture = NULL;
	if (pTextureID)
	{
		D3D11_TEXTURE2D_DESC Desc;
		pTempTexture = ((ID3D11Texture2D *)(pTextureID->m_pTexture));
		pTempTexture->GetDesc(&Desc);
		if (Desc.SampleDesc.Count == Count)
		{
			b1 = true;
		}
		else
		{
			b2 = true;
		}
	}
	else
	{
		b2 = true;
	}

	if (LockFlag == VSBind::LF_READONLY)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = Width;
		desc.Height = Height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = Format;
		desc.SampleDesc.Count = Count;
		desc.SampleDesc.Quality = Quality;
		desc.BindFlags = 0;
		desc.MiscFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;
		hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pRenderTargetID->m_pStagTexture);
		VSMAC_ASSERT(!FAILED(hResult));
	}

	pRenderTargetID->m_pOldTexture = pTempTexture;
	if (b1)
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		desc.Format = Format;
		if (uiTextureType == VSTexture::TT_2D)
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = uiRenderTargetLevel;
		}
		else if (uiTextureType == VSTexture::TT_CUBE)
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.MipSlice = uiRenderTargetLevel;
			desc.Texture2DArray.ArraySize = 1;
			desc.Texture2DArray.FirstArraySlice = uiCubeFace;
		}

		hResult = m_pDevice->CreateRenderTargetView(pTempTexture, &desc, &pRenderTargetID->m_pRenderTarget);
		VSMAC_ASSERT(!FAILED(hResult));
	}
	if (b2)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = Width;
		desc.Height = Height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = Format;
		desc.SampleDesc.Count = Count;
		desc.SampleDesc.Quality = Quality;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = 0;
		desc.CPUAccessFlags = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;



		hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pRenderTargetID->m_pTextureSurface);
		VSMAC_ASSERT(!FAILED(hResult));

		D3D11_RENDER_TARGET_VIEW_DESC RTVdesc;
		RTVdesc.Format = Format;
		RTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		RTVdesc.Texture2D.MipSlice = 0;

		hResult = m_pDevice->CreateRenderTargetView(pRenderTargetID->m_pTextureSurface, &RTVdesc, &pRenderTargetID->m_pRenderTarget);
		VSMAC_ASSERT(!FAILED(hResult));
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::OnLoadVertexBufferEx(unsigned int uiOneVextexSize, unsigned int uiTotalSize, 
	unsigned int CPUAccessFlags, D3D11_USAGE Usage,
	VSVertexBuffer * pVBuffer, VSVBufferID * pVBufferID)
{
	struct VSOnLoadVertexBufferExPara
	{
		unsigned int uiOneVextexSize;
		unsigned int uiTotalSize;
		unsigned int CPUAccessFlags;
		D3D11_USAGE Usage;
		VSVertexBuffer * pVBuffer;
		VSVBufferID * pVBufferID;
	};
	VSOnLoadVertexBufferExPara OnLoadVertexBufferExPara;
	OnLoadVertexBufferExPara.uiOneVextexSize = uiOneVextexSize;
	OnLoadVertexBufferExPara.uiTotalSize = uiTotalSize;
	OnLoadVertexBufferExPara.CPUAccessFlags = CPUAccessFlags;
	OnLoadVertexBufferExPara.Usage = Usage;
	OnLoadVertexBufferExPara.pVBuffer = pVBuffer;
	OnLoadVertexBufferExPara.pVBufferID = pVBufferID;


	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OnLoadVertexBufferExCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSOnLoadVertexBufferExPara, OnLoadVertexBufferExPara, OnLoadVertexBufferExPara,
	{
		unsigned int uiOneVextexSize = OnLoadVertexBufferExPara.uiOneVextexSize;
		unsigned int uiTotalSize = OnLoadVertexBufferExPara.uiTotalSize;
		unsigned int CPUAccessFlags = OnLoadVertexBufferExPara.CPUAccessFlags;
		D3D11_USAGE Usage = OnLoadVertexBufferExPara.Usage;
		VSVertexBuffer * pVBuffer = OnLoadVertexBufferExPara.pVBuffer;
		VSVBufferID * pVBufferID = OnLoadVertexBufferExPara.pVBufferID;

		unsigned char * pTempBufferUsed = VS_NEW unsigned char[uiTotalSize];
		D3D11_BUFFER_DESC bd;

		bd.ByteWidth = uiTotalSize;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.Usage = Usage;
		bd.CPUAccessFlags = CPUAccessFlags;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pTempBufferUsed;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;


		HRESULT hResult = NULL;

		VSDataBuffer * pDate;

		unsigned int iVertexSize = 0;
		for (unsigned int i = 0; i < pVBuffer->GetPositionLevel(); i++)
		{
			pDate = pVBuffer->GetPositionDate(i);
			if (pDate)
			{
				for (unsigned int j = 0; j < pDate->GetNum(); j++)
				{
					VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}

		}

		for (unsigned int i = 0; i < pVBuffer->GetTexCoordLevel(); i++)
		{
			pDate = pVBuffer->GetTexCoordDate(i);
			if (pDate)
			{
				for (unsigned int j = 0; j < pDate->GetNum(); j++)
				{
					VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}
		}

		for (unsigned int i = 0; i < pVBuffer->GetNormalLevel(); i++)
		{
			pDate = pVBuffer->GetNormalDate(i);
			if (pDate)
			{
				for (unsigned int j = 0; j < pDate->GetNum(); j++)
				{
					VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}
		}
		pDate = pVBuffer->GetTangentDate();
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}


		pDate = pVBuffer->GetBinormalDate();
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}
		pDate = pVBuffer->GetPSizeDate();
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		for (unsigned int i = 0; i < pVBuffer->GetColorLevel(); i++)
		{

			pDate = pVBuffer->GetColorDate(i);
			if (pDate)
			{
				for (unsigned int j = 0; j < pDate->GetNum(); j++)
				{
					VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}
		}

		pDate = pVBuffer->GetFogDate();
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		pDate = pVBuffer->GetDepthDate();
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		pDate = pVBuffer->GetBlendWeightDate();
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		pDate = pVBuffer->GetBlendIndicesDate();
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		if (iVertexSize > 0)
		{
			hResult = m_pDevice->CreateBuffer(&bd, &InitData, &pVBufferID->m_pVertexBuffer);
		}
		else
		{
			hResult = m_pDevice->CreateBuffer(&bd, NULL, &pVBufferID->m_pVertexBuffer);
		}
		VSMAC_DELETEA(pTempBufferUsed);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	unsigned char * pTempBufferUsed = VS_NEW unsigned char[uiTotalSize];
	D3D11_BUFFER_DESC bd;

	bd.ByteWidth = uiTotalSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.Usage = Usage;
	bd.CPUAccessFlags = CPUAccessFlags;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pTempBufferUsed;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;


	HRESULT hResult = NULL;

	VSDataBuffer * pDate;

	unsigned int iVertexSize = 0;
	for (unsigned int i = 0; i < pVBuffer->GetPositionLevel(); i++)
	{
		pDate = pVBuffer->GetPositionDate(i);
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

	}

	for (unsigned int i = 0; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		pDate = pVBuffer->GetTexCoordDate(i);
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}
	}

	for (unsigned int i = 0; i < pVBuffer->GetNormalLevel(); i++)
	{
		pDate = pVBuffer->GetNormalDate(i);
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}
	}
	pDate = pVBuffer->GetTangentDate();
	if (pDate)
	{
		for (unsigned int j = 0; j < pDate->GetNum(); j++)
		{
			VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

		}
		iVertexSize += pDate->GetStride();
	}


	pDate = pVBuffer->GetBinormalDate();
	if (pDate)
	{
		for (unsigned int j = 0; j < pDate->GetNum(); j++)
		{
			VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

		}
		iVertexSize += pDate->GetStride();
	}
	pDate = pVBuffer->GetPSizeDate();
	if (pDate)
	{
		for (unsigned int j = 0; j < pDate->GetNum(); j++)
		{
			VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

		}
		iVertexSize += pDate->GetStride();
	}

	for (unsigned int i = 0; i < pVBuffer->GetColorLevel(); i++)
	{

		pDate = pVBuffer->GetColorDate(i);
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}
	}

	pDate = pVBuffer->GetFogDate();
	if (pDate)
	{
		for (unsigned int j = 0; j < pDate->GetNum(); j++)
		{
			VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

		}
		iVertexSize += pDate->GetStride();
	}

	pDate = pVBuffer->GetDepthDate();
	if (pDate)
	{
		for (unsigned int j = 0; j < pDate->GetNum(); j++)
		{
			VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

		}
		iVertexSize += pDate->GetStride();
	}

	pDate = pVBuffer->GetBlendWeightDate();
	if (pDate)
	{
		for (unsigned int j = 0; j < pDate->GetNum(); j++)
		{
			VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

		}
		iVertexSize += pDate->GetStride();
	}

	pDate = pVBuffer->GetBlendIndicesDate();
	if (pDate)
	{
		for (unsigned int j = 0; j < pDate->GetNum(); j++)
		{
			VSMemcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

		}
		iVertexSize += pDate->GetStride();
	}

	if (iVertexSize > 0)
	{
		hResult = m_pDevice->CreateBuffer(&bd, &InitData, &pVBufferID->m_pVertexBuffer);
	}
	else
	{
		hResult = m_pDevice->CreateBuffer(&bd, NULL, &pVBufferID->m_pVertexBuffer);
	}
	VSMAC_DELETEA(pTempBufferUsed);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::OnLoadIndexBufferEx(unsigned int ByteWidth, D3D11_USAGE Usage, unsigned int CPUAccessFlags, void* pBuffer, VSIBufferID * pIBufferID)
{
	struct VSOnLoadIndexBufferExPara
	{
		unsigned int ByteWidth;
		D3D11_USAGE Usage;
		unsigned int CPUAccessFlags;
		void* pBuffer;
		VSIBufferID * pIBufferID;
	};
	VSOnLoadIndexBufferExPara OnLoadIndexBufferExPara;
	OnLoadIndexBufferExPara.ByteWidth = ByteWidth;
	OnLoadIndexBufferExPara.Usage = Usage;
	OnLoadIndexBufferExPara.CPUAccessFlags = CPUAccessFlags;
	OnLoadIndexBufferExPara.pBuffer = pBuffer;
	OnLoadIndexBufferExPara.pIBufferID = pIBufferID;


	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OnLoadIndexBufferExCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSOnLoadIndexBufferExPara, OnLoadIndexBufferExPara, OnLoadIndexBufferExPara,
		{
		unsigned int ByteWidth = OnLoadIndexBufferExPara.ByteWidth;
		D3D11_USAGE Usage = OnLoadIndexBufferExPara.Usage;
		unsigned int CPUAccessFlags = OnLoadIndexBufferExPara.CPUAccessFlags;
		void* pBuffer = OnLoadIndexBufferExPara.pBuffer;
		VSIBufferID * pIBufferID = OnLoadIndexBufferExPara.pIBufferID;

		HRESULT hResult = NULL;
		D3D11_BUFFER_DESC bd;
		bd.Usage = Usage;
		bd.ByteWidth = ByteWidth;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.CPUAccessFlags = CPUAccessFlags;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;




		if (pBuffer)
		{
			hResult = m_pDevice->CreateBuffer(&bd, &InitData, &pIBufferID->m_IndexBuffer);
		}
		else
		{
			hResult = m_pDevice->CreateBuffer(&bd, NULL, &pIBufferID->m_IndexBuffer);
		}
		VSMAC_ASSERT(!FAILED(hResult));
	})
	HRESULT hResult = NULL;
	D3D11_BUFFER_DESC bd;
	bd.Usage = Usage;
	bd.ByteWidth = ByteWidth;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.CPUAccessFlags = CPUAccessFlags;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pBuffer;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;




	if (pBuffer)
	{
		hResult = m_pDevice->CreateBuffer(&bd, &InitData, &pIBufferID->m_IndexBuffer);
	}
	else
	{
		hResult = m_pDevice->CreateBuffer(&bd, NULL, &pIBufferID->m_IndexBuffer);
	}
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
DECLEAR_COUNT_PROFILENODE(Dx11DrawPrimitiveNum, )
DECLEAR_COUNT_PROFILENODE(Dx11DrawGeometryNum, )
DECLEAR_COUNT_PROFILENODE(Dx11DrawPassNum, )
bool VSDX11Renderer::DrawDynamicBufferMesh(VSDynamicBufferGeometry *pDBGeometry)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11DrawDynamicBufferMeshCommand,
		VSGeometry *, pDBGeometry, pDBGeometry, ID3D11DeviceContext *, m_pDeviceContext, m_pDeviceContext,
	{
		ADD_COUNT_PROFILE(Dx11DrawGeometryNum, 1)
		m_pDeviceContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)ms_dwPrimitiveType[pDBGeometry->GetMeshDate()->GetMeshDateType()]);
		while (true)
		{
			HRESULT hResult = NULL;
			unsigned int uiReturn = pDBGeometry->UpdateGeometry();

			if (uiReturn == VSGeometry::UGRI_FAIL)
			{
				break;
			}
			ADD_COUNT_PROFILE(Dx11DrawPassNum, 1)
			if (!pDBGeometry->GetMeshDate()->GetIndexBuffer())
			{
				m_pDeviceContext->Draw(pDBGeometry->GetMeshDate()->GetVertexBuffer()->GetVertexNum(), 0);
			}
			else
			{
				m_pDeviceContext->DrawIndexed(pDBGeometry->GetActiveNum() * 3, 0, 0);
			}
			ADD_COUNT_PROFILE(Dx11DrawPrimitiveNum, pDBGeometry->GetActiveNum())
				VSMAC_ASSERT(!FAILED(hResult));

			if (uiReturn == VSGeometry::UGRI_END)
			{
				break;
			}
		}
	})
	ADD_COUNT_PROFILE(Dx11DrawGeometryNum, 1)
	m_pDeviceContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)ms_dwPrimitiveType[pDBGeometry->GetMeshDate()->GetMeshDateType()]);
	while (true)
	{
		HRESULT hResult = NULL;
		unsigned int uiReturn = pDBGeometry->UpdateGeometry();

		if (uiReturn == VSGeometry::UGRI_FAIL)
		{
			break;
		}
		ADD_COUNT_PROFILE(Dx11DrawPassNum, 1)
		if (!pDBGeometry->GetMeshDate()->GetIndexBuffer())
		{
			m_pDeviceContext->Draw(pDBGeometry->GetMeshDate()->GetVertexBuffer()->GetVertexNum(), 0);
		}
		else
		{
			m_pDeviceContext->DrawIndexed(pDBGeometry->GetActiveNum() * 3, 0, 0);
		}
		ADD_COUNT_PROFILE(Dx11DrawPrimitiveNum, pDBGeometry->GetActiveNum())
			VSMAC_ASSERT(!FAILED(hResult));

		if (uiReturn == VSGeometry::UGRI_END)
		{
			break;
		}
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	return true;
}
bool VSDX11Renderer::DrawMesh(VSGeometry * pGeometry)
{

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11DrawMeshCommand,
		VSGeometry *, pGeometry, pGeometry, ID3D11DeviceContext *, m_pDeviceContext, m_pDeviceContext,
	{
		ADD_COUNT_PROFILE(Dx11DrawGeometryNum, 1)
		ADD_COUNT_PROFILE(Dx11DrawPassNum, 1)
		pGeometry->UpdateGeometry();
		m_pDeviceContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)ms_dwPrimitiveType[pGeometry->GetMeshDate()->GetMeshDateType()]);
		if (!pGeometry->GetMeshDate()->GetIndexBuffer())
		{
			m_pDeviceContext->Draw(pGeometry->GetMeshDate()->GetVertexBuffer()->GetVertexNum(), 0);

		}
		else
		{
			m_pDeviceContext->DrawIndexed(pGeometry->GetActiveNum() * 3, 0, 0);
		}
		ADD_COUNT_PROFILE(Dx11DrawPrimitiveNum, pGeometry->GetActiveNum())
	})
	ADD_COUNT_PROFILE(Dx11DrawGeometryNum, 1)
	ADD_COUNT_PROFILE(Dx11DrawPassNum, 1)
	pGeometry->UpdateGeometry();
	m_pDeviceContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)ms_dwPrimitiveType[pGeometry->GetMeshDate()->GetMeshDateType()]);
	if (!pGeometry->GetMeshDate()->GetIndexBuffer())
	{
		m_pDeviceContext->Draw(pGeometry->GetMeshDate()->GetVertexBuffer()->GetVertexNum(), 0);

	}
	else
	{
		m_pDeviceContext->DrawIndexed(pGeometry->GetActiveNum() * 3, 0, 0);
	}
	ADD_COUNT_PROFILE(Dx11DrawPrimitiveNum, pGeometry->GetActiveNum())
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	return true;


}
bool VSDX11Renderer::DrawMesh1(VSGeometry * pGeometry)
{
	struct VSDrawMesh1Para
	{
		VSGeometry * pGeometry;
		unsigned int uiMeshType;
		unsigned int uiVertexNum;
		unsigned int uiActiveNum;
	};
	VSDrawMesh1Para DrawMesh1Para;
	DrawMesh1Para.pGeometry = pGeometry;
	DrawMesh1Para.uiMeshType = pGeometry->GetMeshDate()->GetMeshDateType();
	DrawMesh1Para.uiActiveNum = pGeometry->GetActiveNum();
	DrawMesh1Para.uiVertexNum = pGeometry->GetMeshDate()->GetVertexBuffer()->GetVertexNum();
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11DrawMesh1Command,
		VSDrawMesh1Para, DrawMesh1Para, DrawMesh1Para, ID3D11DeviceContext *, m_pDeviceContext, m_pDeviceContext,
		{
		ADD_COUNT_PROFILE(Dx11DrawGeometryNum, 1)
		ADD_COUNT_PROFILE(Dx11DrawPassNum, 1)
		m_pDeviceContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)ms_dwPrimitiveType[DrawMesh1Para.uiMeshType]);
		if (!DrawMesh1Para.pGeometry->GetMeshDate()->GetIndexBuffer())
		{
			m_pDeviceContext->Draw(DrawMesh1Para.uiVertexNum, 0);
		}
		else
		{
			m_pDeviceContext->DrawIndexed(DrawMesh1Para.uiActiveNum * 3, 0, 0);
		}
		ADD_COUNT_PROFILE(Dx11DrawPrimitiveNum, DrawMesh1Para.uiActiveNum)
	})
	ADD_COUNT_PROFILE(Dx11DrawGeometryNum, 1)
	ADD_COUNT_PROFILE(Dx11DrawPassNum, 1)
	m_pDeviceContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)ms_dwPrimitiveType[DrawMesh1Para.uiMeshType]);
	if (!DrawMesh1Para.pGeometry->GetMeshDate()->GetIndexBuffer())
	{
		m_pDeviceContext->Draw(DrawMesh1Para.uiVertexNum, 0);
	}
	else
	{
		m_pDeviceContext->DrawIndexed(DrawMesh1Para.uiActiveNum * 3, 0, 0);
	}
	ADD_COUNT_PROFILE(Dx11DrawPrimitiveNum, DrawMesh1Para.uiActiveNum)
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	return true;
}
void VSDX11Renderer::CreateBlendState(const D3D11_BLEND_DESC &desc, VSBlendStateID * pBlendStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateBlendStateCommand, ID3D11Device*, m_pDevice, m_pDevice,
		D3D11_BLEND_DESC, desc, desc, VSBlendStateID *, pBlendStateID, pBlendStateID,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateBlendState(&desc, &pBlendStateID->m_pBlendState);
		VSMAC_ASSERT(!FAILED(hResult));
	})
		HRESULT hResult = NULL;
	hResult = m_pDevice->CreateBlendState(&desc, &pBlendStateID->m_pBlendState);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC depthStencilDesc, VSDepthStencilStateID * pDepthStenciStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateDepthStencilStateCommand, ID3D11Device*, m_pDevice, m_pDevice,
		D3D11_DEPTH_STENCIL_DESC, depthStencilDesc, depthStencilDesc, VSDepthStencilStateID *, pDepthStenciStateID, pDepthStenciStateID,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStenciStateID->m_pDepthStencilState);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	HRESULT hResult = NULL;
	hResult = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStenciStateID->m_pDepthStencilState);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateRasterizerState(const D3D11_RASTERIZER_DESC rasterDesc, VSRasterStateID * pRasterStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateRasterizerStateCommand, ID3D11Device*, m_pDevice, m_pDevice,
		D3D11_RASTERIZER_DESC, rasterDesc, rasterDesc, VSRasterStateID *, pRasterStateID, pRasterStateID,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateRasterizerState(&rasterDesc, &pRasterStateID->m_pRasterState);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	HRESULT hResult = NULL;
	hResult = m_pDevice->CreateRasterizerState(&rasterDesc, &pRasterStateID->m_pRasterState);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::CreateSamplerState(const D3D11_SAMPLER_DESC& sampDesc, VSSamplerStateID * pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateSamplerStateCommand, ID3D11Device*, m_pDevice, m_pDevice,
		D3D11_SAMPLER_DESC, sampDesc, sampDesc, VSSamplerStateID *, pSamplerStateID, pSamplerStateID,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateSamplerState(&sampDesc, &pSamplerStateID->m_pSamplerState);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	HRESULT hResult = NULL;
	hResult = m_pDevice->CreateSamplerState(&sampDesc, &pSamplerStateID->m_pSamplerState);
	VSMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSDX11Renderer::IASetInputLayout(VSVBufferFormatID * pVBufferFormatID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11IASetInputLayoutCommand, ID3D11DeviceContext *, m_pDeviceContext, m_pDeviceContext,
		VSVBufferFormatID *, pVBufferFormatID, pVBufferFormatID,
	{
		if (pVBufferFormatID)
		{
			m_pDeviceContext->IASetInputLayout(pVBufferFormatID->m_pDeclaration);
		}
		else
		{
			m_pDeviceContext->IASetInputLayout(NULL);
		}
	})
	if (pVBufferFormatID)
	{
		m_pDeviceContext->IASetInputLayout(pVBufferFormatID->m_pDeclaration);
	}
	else
	{
		m_pDeviceContext->IASetInputLayout(NULL);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}