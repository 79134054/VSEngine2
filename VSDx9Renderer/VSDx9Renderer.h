#ifndef VSDX9RENDERER_H
#define VSDX9RENDERER_H
#ifdef VSDX9RENDERER_EXPORTS
#define  VSDX9RENDERER_API __declspec(dllexport) 
#else
#define  VSDX9RENDERER_API __declspec(dllimport)
#endif
#include <d3d9.h>
#include <d3dx9tex.h>
#include "VSRenderer.h"
#include "VSResourceManager.h"

namespace VSEngine2
{
	#define VSMAC_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
	class VSDx9ShaderInclude;
	class VSVProgramID;
	class VSPProgramID;
	class VSDX9RENDERER_API VSDX9Renderer : public VSRenderer
	{
	public:
		VSDX9Renderer(HWND hMainWindow,unsigned int uiScreenWidth,unsigned int uiScreenHeight,bool bIsWindowed,
					unsigned int uiAnisotropy,unsigned int uiMultisample,
					ChildWindowInfo * pChildWindow,int uiNumChildWindow);
		virtual ~VSDX9Renderer();
		virtual int GetRendererType () const;
		virtual const TCHAR * GetRendererStringType() const;
		virtual void ClearBackBuffer ();
		virtual void ClearZBuffer ();
		virtual void ClearStencilBuffer ();
		virtual void ClearBuffers ();
		virtual void ClearBuffers(unsigned int uiClearFlag);
		//virtual void DisplayBackBuffer ();
		virtual void DeviceLost();
		virtual void ResetDevice();
		virtual bool ChangeScreenSize(unsigned int uiWidth,unsigned int uiHeight,bool bWindow);

		virtual void ClearBackBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearZBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearStencilBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearBuffers (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearBuffers(unsigned int uiClearFlag,int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual unsigned int GetTextureCompressLen(unsigned int uiLen);	

		virtual bool CooperativeLevel();
		virtual bool BeginRendering();
		virtual bool EndRendering();
		virtual bool DrawMesh(VSGeometry * pGeometry, VSRenderState * pRenderState, VSVShader * pVShader, VSPShader * pPShader, VSGShader * pGShader);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer,unsigned int uiVertexNum,
								VSUSHORT_INDEX * pIndexBuffer,unsigned int uiIndexNum);
		virtual bool DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
								VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum);
	protected:
		
// 		virtual bool SetVProgramConstant (VSString &NameInShaderProgram,void * pDate,unsigned int uiSize);
// 		virtual bool SetPProgramConstant (VSString &NameInShaderProgram,void * pDate,unsigned int uiSize);
		virtual bool SetVProgramConstant (unsigned int uiStartRegister,void * pDate,
											unsigned int RegisterNum,unsigned int uiType);
		virtual bool SetPProgramConstant (unsigned int uiStartRegister,void * pDate,
											unsigned int RegisterNum,unsigned int uiType);
		//Load Release
		virtual bool OnLoadVShaderProgram (VSVShader * pVShaderProgram,VSResourceIdentifier *&pID);
		virtual bool OnReleaseVShaderProgram(VSResourceIdentifier * pVShaderProgramID);

		virtual bool OnLoadPShaderProgram (VSPShader* pPShaderProgram,VSResourceIdentifier *&pID);
		virtual bool OnReleasePShaderProgram (VSResourceIdentifier* pPShaderProgramID);

		virtual bool OnLoadGShaderProgram(VSGShader * pGShaderProgram, VSResourceIdentifier *&pID);
		virtual bool OnReleaseGShaderProgram(VSResourceIdentifier * pGShaderProgramID);

		virtual bool OnLoadDShaderProgram(VSDShader * pDShaderProgram, VSResourceIdentifier *&pID);
		virtual bool OnReleaseDShaderProgram(VSResourceIdentifier * pDShaderProgramID);

		virtual bool OnLoadHShaderProgram(VSHShader * pHShaderProgram, VSResourceIdentifier *&pID);
		virtual bool OnReleaseHShaderProgram(VSResourceIdentifier * pHShaderProgramID);

		virtual bool OnLoadTexture (VSTexture * pTexture,VSResourceIdentifier *&pID);
		virtual bool OnReleaseTexture (VSResourceIdentifier * pTextureID);

		virtual bool OnLoadDepthStencil (VSDepthStencil * pDepthStencil,VSResourceIdentifier *&pID);
		virtual bool OnReleaseDepthStencil (VSResourceIdentifier * pDepthStencilID);
		
		virtual bool OnLoadRenderTarget (VSRenderTarget * pRenderTarget,VSResourceIdentifier *&pID);
		virtual bool OnReleaseRenderTarget (VSResourceIdentifier * pRenderTarget);

		virtual bool OnLoadVBufferDate (VSVertexBuffer * pVBuffer,VSResourceIdentifier *&pID);
		virtual bool OnReleaseVBufferDate (VSResourceIdentifier* pVBufferID);

		virtual bool OnLoadVBufferFormat (VSVertexFormat * pVertexFormat,VSResourceIdentifier *&pID);
		virtual bool OnReleaseVBufferFormat (VSResourceIdentifier* pVBufferID);

		virtual bool OnLoadIBuffer (VSIndexBuffer* pIBuffer,VSResourceIdentifier *&pID);
		virtual bool OnReleaseIBuffer (VSResourceIdentifier* pIBufferID);

		virtual bool OnLoadBlendState (VSBlendState * pBlendState,VSResourceIdentifier *&pID);
		virtual bool OnReleaseBlendState  (VSResourceIdentifier * pBlendStateID);

		virtual bool OnLoadDepthStencilState (VSDepthStencilState * pDepthStencilState,VSResourceIdentifier *&pID);
		virtual bool OnReleaseDepthStencilState  (VSResourceIdentifier * pDepthStencilStateID);

		virtual bool OnLoadRasterizerState(VSRasterizerState * pRasterizerState,VSResourceIdentifier *&pID);
		virtual bool OnReleaseRasterizerState  (VSResourceIdentifier * pRasterizerStateID);

		virtual bool OnLoadSamplerState(VSSamplerState * pRasterizerState,VSResourceIdentifier *&pID);
		virtual bool OnReleaseSamplerState  (VSResourceIdentifier * pRasterizerStateID);

		virtual bool SetVShaderProgram(VSVShader*	pVShaderProgram);
		virtual bool SetPShaderProgram(VSPShader*	pPShaderProgram);
		virtual bool SetGShaderProgram(VSGShader*	pGShaderProgram);
		virtual bool SetDShaderProgram(VSDShader*	pDShaderProgram);
		virtual bool SetHShaderProgram(VSHShader*	pHShaderProgram);

		virtual unsigned int SetVertexFormat(VSVertexFormat * pVertexFormat);

		

		
		virtual void SetVShaderConstant(VSVShader * pShader);
		virtual void SetVShaderSampler(VSVShader * pShader);
		virtual void SetPShaderConstant(VSPShader * pShader);
		virtual void SetPShaderSampler(VSPShader * pShader);
		virtual void SetGShaderConstant(VSGShader * pShader);
		virtual void SetGShaderSampler(VSGShader * pShader);
		virtual void SetDShaderConstant(VSDShader * pShader);
		virtual void SetDShaderSampler(VSDShader * pShader);
		virtual void SetHShaderConstant(VSHShader * pShader);
		virtual void SetHShaderSampler(VSHShader * pShader);
		D3DXMACRO * GetDefine(VSMap<VSString,VSString> & Define);


		virtual void * Lock(VSVertexBuffer * pVertexBuffer);
		virtual void UnLock(VSVertexBuffer * pVertexBuffer);
		virtual void * Lock(VSIndexBuffer * pIndexBuffer);
		virtual void UnLock(VSIndexBuffer * pIndexBuffer);
		virtual void * Lock(VSTexture * pTexture,unsigned int uiLevel,unsigned int uiFace);
		virtual void UnLock(VSTexture * pTexture,unsigned int uiLevel,unsigned int uiFace);
		virtual void * Lock(VSRenderTarget * pRenderTarget);
		virtual void UnLock(VSRenderTarget * pRenderTarget);

		virtual void * Lock(VSDepthStencil * pDepthStencil);
		virtual void UnLock(VSDepthStencil * pDepthStencil);
		virtual bool CopyResourceBuffer(VS2DTexture * pSource,VSCubeTexture * pDest,unsigned int uiFace);
	private:
		void OnLoadVShaderFromString(VSVShader * pVShaderProgram,VSVProgramID *pID);
		void OnLoadPShaderFromString(VSPShader* pPShaderProgram,VSPProgramID *pID);
	private:
// 		DX9_API_FUN_TWOPARAMETER(SetRenderState,D3DRENDERSTATETYPE,State,DWORD,Value)
// 		DX9_API_FUN_END
 		bool SetRenderState(D3DRENDERSTATETYPE State,DWORD Value);
 		bool SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value);
		bool Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,VSREAL Z,DWORD Stencil);
		bool SetClipPlane(DWORD Index,CONST VSREAL* pPlane);
		bool SetScissorRect(CONST RECT* pRect);
		bool SetViewport(CONST D3DVIEWPORT9* pViewport);
		bool SetVertexShaderConstant(unsigned int uiStartRegister,void * pDate,
									unsigned int RegisterNum,unsigned int uiType);
		bool SetPixelShaderConstant(unsigned int uiStartRegister,void * pDate,
			unsigned int RegisterNum,unsigned int uiType);
		bool DrawDynamicBufferMesh(VSDynamicBufferGeometry *pDBGeometry);
		bool DrawMesh(VSGeometry * pGeometry);
		bool DrawMesh1(VSGeometry * pGeometry);
		bool DrawScreenEX2(SCREEN_QUAD_TYPE * pScreenBuffer,unsigned int uiVertexNum,
			VSUSHORT_INDEX * pIndexBuffer,unsigned int uiNumTri);
		bool DrawScreenEX1(SCREEN_QUAD_TYPE ScreenQuad[4]);
		
		bool DrawScreenFontEX(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
			VSUSHORT_INDEX * pIndexBuffer, unsigned int uiNumTri);
		bool BeginRender();
		bool EndRender();
		bool UseWindowEx(int uiWindowID);


		//resource 

		bool SetVertexShader(IDirect3DVertexShader9** pShader);
		bool SetPixelShader(IDirect3DPixelShader9** pShader);
		bool SetTexture(DWORD Stage,IDirect3DBaseTexture9** pTexture);
		bool SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** pStreamData,UINT OffsetInBytes,UINT Stride);
		bool SetVertexDeclaration(IDirect3DVertexDeclaration9** pDecl);
		bool SetIndices(IDirect3DIndexBuffer9** pIndexData);
		bool SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget);
		bool SetDepthStencilSurface(IDirect3DSurface9** pNewZStencil);
		bool SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget,IDirect3DSurface9** ppRenderTarget);
		bool EndRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget);
		bool SetDepthStencilSurface(IDirect3DSurface9** pNewZStencil,IDirect3DSurface9** ppZStencilSurface);
		bool EndDepthStencilSurface(IDirect3DSurface9** pNewZStencil);

		bool GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget);
		bool GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface);
		bool StretchRect(IDirect3DSurface9** pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9** pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);
		bool CopyResourceBuffer(unsigned int uiLevel,LPDIRECT3DTEXTURE9 * pSource, LPDIRECT3DCUBETEXTURE9 * pDest,CONST RECT* pRect,D3DTEXTUREFILTERTYPE Filter ,D3DCUBEMAP_FACES CubFace);
		
		void CreateIndexBuffer(unsigned int Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,void * pCopyBuffer,DWORD LockFlag);
		void CreateVertexBuffer(UINT Length,DWORD Usage,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,DWORD Flags,VSVertexBuffer * pVBuffer,unsigned int uiOneVextexSize);
		void CreateVertexDeclaration(const VSArray<D3DVERTEXELEMENT9> & Element, IDirect3DVertexDeclaration9** ppDecl);
		void CreateDepthStencilSurface(D3DFORMAT FormatType ,D3DMULTISAMPLE_TYPE MultiSampleTypes,
			unsigned int uiWidth ,unsigned int uiHeight,IDirect3DSurface9** ppSurface);
		void GetSurfaceLevel(LPDIRECT3DTEXTURE9 * pTexture,unsigned int uiLevel,IDirect3DSurface9** ppSurfaceLevel);
		void GetCubeMapSurface(LPDIRECT3DCUBETEXTURE9 * pCubTexture,D3DCUBEMAP_FACES FaceType,UINT Level,IDirect3DSurface9** ppCubeMapSurface);
		void CreateRenderTarget(D3DFORMAT FormatType,D3DMULTISAMPLE_TYPE MultiSampleType,unsigned int uiWidth, unsigned int uiHeight,IDirect3DSurface9** ppSurface);
		void Create2DTexture(VSTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		void Create1DTexture(VSTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		void CreateCubeTexture(VSTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		void CreateVolumeTexture(VSTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		
		
		void CreateVShaderFromString(VSVShader * pVShaderProgram,VSVProgramID *pID);
		void CreateVShaderFromCache(VSVShader * pVShaderProgram,VSVProgramID *pID);

		void CreatePShaderFromString(VSPShader* pPShaderProgram,VSPProgramID *pID);
		void CreatePShaderFromCache(VSPShader* pPShaderProgram,VSPProgramID *pID);

		void GetShaderInstruction(ID3DXBuffer * pShader,unsigned int &uiArithmeticInstructionSlots ,unsigned int &uiTextureInstructionSlots);
	public:


		//Enable Disable
		virtual void SetTexAllState(VSTexAllState * pTexAllState,unsigned int i);
		virtual unsigned int SetVShader(VSVShader * pVShader);
		virtual unsigned int SetPShader(VSPShader * pPShader);
		virtual unsigned int SetGShader(VSGShader * pGShader);
		virtual unsigned int SetDShader(VSDShader * pDShader);
		virtual unsigned int SetHShader(VSHShader * pHShader);

		virtual unsigned int SetVBuffer(VSVertexBuffer * pVBuffer);
		virtual unsigned int SetIBuffer (VSIndexBuffer * pIBuffer);
		virtual unsigned int SetTexture (VSTexture* pTexture,unsigned int i);
		virtual unsigned int SetVTexture (VSTexture* pTexture,unsigned int i);
		virtual unsigned int SetGTexture(VSTexture* pTexture, unsigned int i);
		virtual unsigned int SetDTexture(VSTexture* pTexture, unsigned int i);
		virtual unsigned int SetHTexture(VSTexture* pTexture, unsigned int i);

		virtual unsigned int SetSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetVSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetGSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetDSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetHSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);

		virtual unsigned int SetBlendState(VSBlendState * pBlendState, bool bForceSet = false);
		virtual unsigned int SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet = false);
		virtual unsigned int SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet = false);
		virtual void SetClipPlane(VSArray<VSPlane3> &Plane,bool bEnable);
		virtual void SetScissorRect(VSArray<VSRect2> &Rect,bool bEnable);

		virtual bool SetRenderTarget(VSRenderTarget *pRenderTarget,unsigned int i);
		virtual bool EndRenderTarget(VSRenderTarget *pRenderTarget,unsigned int i);
		virtual bool SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer);
		virtual bool EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer);
		virtual bool SetViewPort(VSViewPort *pViewPort = NULL);
		virtual bool UseWindow(int uiWindowID);
		
		virtual const VSAdapterInfo* GetAdapterInfo(unsigned int & uiAdapterNum);
		virtual bool IsSupportMulBufferSwtich()const;
		virtual bool IsSupportGS()const;
	protected:
		
		static VSDx9ShaderInclude * ms_pDx9IncludeShader;
		static LPDIRECT3D9 ms_pMain;
		LPDIRECT3DDEVICE9 m_pDevice;
		LPDIRECT3DSWAPCHAIN9  * m_pChain;	
		LPDIRECT3DSWAPCHAIN9  m_pMainChain;
		D3DPRESENT_PARAMETERS m_Present;

		LPDIRECT3DSURFACE9	m_pMainDepthStencilBuffer;
		LPDIRECT3DSURFACE9*	m_pChainnDepthStencilBuffer;

		static DWORD ms_dwAlphaBlend[];
		static DWORD ms_dwAlphaBlendOP[];
		static DWORD ms_dwCullType[];



		static DWORD ms_dwStencilOperation[];

		static DWORD ms_dwCompare[];

		static DWORD ms_dwTextureFormatType[];
		static DWORD ms_dwTextureFilterType[];
		static DWORD ms_dwTextureAddrType[];

		static DWORD ms_dwPrimitiveType[];
		static DWORD ms_dwDevTypes[]; 
		static DWORD ms_dwMultiSampleTypes[]; 
		static DWORD ms_dwCubeMapFace[];
		static DWORD ms_dwLockFlag[];
		static DWORD ms_dwVertexFormatDateType[];
		static DWORD ms_dwVertexFormatSemantics[];

		static TCHAR ms_cVertexShaderProgramVersion[];
		static TCHAR ms_cPixelShaderProgramVersion[];

		enum
		{
			MAX_SHADER_BUFFER = 1024,
			MAX_TEXTURE_BUFFER = 16
		};
		VSREAL	m_fFloatShaderBuffer[MAX_SHADER_BUFFER];
		int		m_iIntShaderBuffer[MAX_SHADER_BUFFER];
		bool	m_bBoolShaderBuffer[MAX_SHADER_BUFFER];
		VSTexAllState * m_pTexAllStateBuffer[MAX_TEXTURE_BUFFER];

		bool m_bSRGB[TEXLEVEL];
		//PRIORITY
		DECLARE_PRIORITY
		DECLARE_INITIAL_ONLY
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

	public:
		enum	//Register Type
		{
			RT_B,
			RT_C,
			RT_S,
			RT_MAX
		};
		//function in d3d  so output is string
		virtual VSString TexColorBuffer(const VSColorBuffer * pColorBuffer) const;
		virtual VSString Tex2D(const VS2DTexSampler * p2DTexSampler) const;
		virtual VSString Tex1D(const VS1DTexSampler * p1DTexSampler) const;
		virtual VSString Mul(const VSMul * pMul) const;
		virtual VSString Dot(const VSDot * pDot) const;
		virtual VSString Sub(const VSSub * pSub) const;
		virtual VSString Add(const VSAdd * pAdd) const;
		virtual VSString Div(const VSDiv * pDiv) const;
		virtual VSString MakeValue(const VSMakeValue * pMakeValue) const;
		virtual VSString Saturate(const VSSaturate * pSaturate) const;
		//
		virtual VSString Sampler(unsigned int uiType)const;
		virtual VSString Float()const;
		virtual VSString Float2()const;
		virtual VSString Float3()const;
		virtual VSString Float4()const;
		virtual VSString Return()const;
		virtual VSString FloatConst(const VSString & Value1)const;
		virtual VSString Float2Const(const VSString & Value1,const VSString & Value2)const;
		virtual VSString Float3Const(const VSString & Value1,
			const VSString & Value2,const VSString & Value3)const;
		virtual VSString Float4Const(const VSString & Value1,const VSString & Value2,
			const VSString & Value3,const VSString & Value4)const;

		virtual void CustomPointLightUseString(VSString & PointLightUseString ,VSString & PointAttenuationDivString);
		virtual void CustomSpotLightUseString(VSString & SpotLightUseString ,VSString & SpotAttenuationDivString);

		virtual VSString GetDeclareValue(const VSString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const;
		virtual VSString GetDeclareSampler(const VSString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex)const;

		virtual VSString SetRegister(unsigned int uiRegisterType,unsigned int uiRegisterIndex)const;
		virtual VSString GetValueElement(const VSPutNode * pPutNode,
										unsigned char uiVE)const;
		virtual VSString GetValueElement(const VSString & InputString,
			unsigned char uiVE)const;

		virtual void CreateVInputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString);
		virtual void CreateVOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString);
		virtual void CreateVUserConstant(VSVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString);
		virtual void CreateVFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
									VSString & OutString);	
		virtual void SetMaterialVShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,VSVShader *pVShader);


		virtual void CreatePInputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString);
		virtual void CreatePOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString);
		virtual void CreatePUserConstant(VSPShader* pPShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString);
		virtual void CreatePFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
									VSString & OutString);	
		virtual void SetMaterialPShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,VSPShader *pPShader);

		virtual void CreateGInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			VSString & OutString);
		virtual void CreateGOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			VSString & OutString);
		virtual void CreateGUserConstant(VSGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
			VSString & OutString);
		virtual void CreateGFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
			VSString & OutString);
		virtual void SetMaterialGShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, VSGShader *pGShader);
		/************************************************************************************/
		virtual void GetIncludeShader(VSString &OutString);

		virtual void GetDynamicShader(VSString &OutString);
		virtual void SkyLight(const VSString &WorldNormal,const VSString &UpColor,const VSString & DownColor,const VSString & DiffuseColor,VSString & OutString);
		//fun is custorm output is paramter
		virtual void DirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
			const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,VSString & OutString)const;

		virtual void CustomDirectionalLight(int iLightNum,const VSString &CustomString,VSArray<VSString> ShadowString,VSString & OutString)const;

		virtual void OrenNayarDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
					const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
					VSString & OutString)const;
		virtual void MinnaertDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Power,
											const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
											VSString & OutString)const;

		virtual void StraussDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,const VSString &Smoothness,
												const VSString &Metalness,const VSString &Transparency,const VSString &K,
												const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
												VSString & OutString)const;
		virtual void ShirleyDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,
											const VSString &NU,const VSString &NV,const VSString &Rs,
											const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
											VSString & OutString)const ;
		virtual void SchlickDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
											const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
											VSString & OutString)const;
		virtual void CookTorranceDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular , 
			const VSString &Roughness , const VSString &C , const VSString &R,
			const VSString &WorldNormal,const VSString &WorldCameraDir,unsigned int uiSpecularType,VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void IsotropicWardDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular , 
			const VSString &Roughness ,
			const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void AnisotropicWardDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular , 
			const VSString &RoughnessX ,const VSString &RoughnessY ,
			const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
			VSString & OutString)const;

		virtual void PointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
								VSString & OutString)const;
		virtual void CustomPointLight(int iLightNum,const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const;

		virtual void OrenNayarPointLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void MinnaertPointLight(int iLightNum,const VSString &Diffuse,const VSString &Power,
										const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
										VSString & OutString)const;
		
		virtual void StraussPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,const VSString &Smoothness,
										const VSString &Metalness,const VSString &Transparency,const VSString &K,
										const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
										VSString & OutString)const;

		virtual void ShirleyPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,
										const VSString &NU,const VSString &NV,const VSString &Rs,
										const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
										VSString & OutString)const;
		virtual void SchlickPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
										const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
										VSString & OutString)const;
		virtual void CookTorrancePointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &Roughness , const VSString &C , const VSString &R,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,unsigned int uiSpecularType,VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void IsotropicWardPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &Roughness ,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void AnisotropicWardPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &RoughnessX ,const VSString &RoughnessY ,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
			VSString & OutString)const;


		virtual void SpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
								VSString & OutString)const;
		virtual void CustomSpotLight(int iLightNum,const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const;

		virtual void OrenNayarSpotLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
										const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
										VSString & OutString)const;
		virtual void MinnaertSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Power,
										const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
										VSString & OutString)const;

		virtual void StraussSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,const VSString &Smoothness,
										const VSString &Metalness,const VSString &Transparency,const VSString &K,
										const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
										VSString & OutString)const;
		virtual void ShirleySpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,
										const VSString &NU,const VSString &NV,const VSString &Rs,
										const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
										VSString & OutString)const;
		virtual void SchlickSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
										const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
										VSString & OutString)const ;
		virtual void CookTorranceSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &Roughness , const VSString &C , const VSString &R,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,unsigned int uiSpecularType,VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void IsotropicWardSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &Roughness ,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void AnisotropicWardSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &RoughnessX ,const VSString &RoughnessY ,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
			VSString & OutString)const;

		virtual void LocalToWorldPos(const VSString & LocalPos,VSString &OutString)const;
		virtual void WorldToViewPos(const VSString & WorldPos,VSString &OutString)const;
		virtual void LocalToWorldNormal(const VSString & LocalNormal,VSString &OutString)const;
		virtual void WorldToViewNormal(const VSString & WorldNormal,VSString &OutString)const;
		virtual void LocalToViewNormal(const VSString & LocalNormal,VSString &OutString)const;
		virtual void GetWorldViewDir(VSString &OutString)const;
		virtual void TransProjPos(const VSString & Pos,const VSString & Matrix,VSString &OutString)const;
		virtual void BumpNormal(const VSString &TexNormal,VSString &OutString)const;

		virtual void ComputeDir(const VSString & Orgin,const VSString & End,VSString &OutString)const;
		virtual void ComputeLength(const VSString & Orgin,const VSString & End,VSString &OutString)const;
		virtual void GetLightType(unsigned int i,VSString &OutString)const;
		virtual void GetLightTypeTemp(VSString &OutString)const;
		virtual void ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,const VSString &BoneVector,
			 const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const;

		virtual void BoneTranPos( const VSString & LocalPos,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const;
		virtual void BoneTranNormal( const VSString & LocalNormal,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const;

		virtual void DecodeNormal1(const VSString & Normal,VSString &OutString) const;
		virtual void RecodeNormal1(const VSString & Normal,VSString &OutString) const;
		virtual void DecodeNormal2(const VSString & Normal,const VSString & Tengent,VSString &OutString) const;
		virtual void DecodeNormal3(const VSString & Normal,VSString &OutString) const;
		virtual void RecodeNormal3(const VSString & Normal,VSString &OutString) const;
		virtual void EncodeReflect(const VSString & Mip , const VSString & Pow , VSString& OutString);
		virtual void DecodeReflect(const VSString & Mip , const VSString & Pow , const VSString & Value,VSString& OutString);

		virtual void Reciprocal(const VSString & Value, VSString &OutString)const;
		virtual void Clip(const VSString & ClipValue,VSString &OutString)const;
		virtual void GreaterZeroPow(const VSString & Value,VSREAL Exp,VSString &OutString) const;
		virtual void SaturatePow(const VSString & Value,VSREAL Exp,VSString &OutString) const;
		virtual void Saturate(const VSString & Value,VSString &OutString)const;
		virtual void PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler , const VSString & ViewMatrix,
			const VSString & WorldPos, VSString &OutString)const;

		virtual void PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler ,
			const VSString & WorldPos, VSString &OutString)const;

		virtual void PointLightVolumeShadow(const VSString & PointLightName , const VSString & VolumeShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const;

		virtual void PointLightDualParaboloidShadow(const VSString & PointLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const;
		virtual void DirectionLightVolumeShadow(const VSString & DirLightName , const VSString & VolumeShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const;
		virtual void DirectionLightCSMShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, const VSString & ViewPos,const VSString &FarZ,VSString& OutString)const;
		virtual void DirectionLightShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const;
		virtual void SpotLightShadow(const VSString & SpotLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const;


		virtual void TranLightToTemp(VSArray<VSLight *> LightArray,VSString & OutString)const;
		virtual void GetLightFunction(VSArray<VSLight *> LightArray,const VSString & WorldPos,VSString & OutString)const;
	private:
		void CreateVInputDeclarePosition(VSVertexBuffer * pVBuffer,
			VSString & OutString);
		void CreateVInputDeclareTexCoord(VSVertexBuffer * pVBuffer,
			VSString & OutString);
		void CreateVInputDeclareNormal(VSVertexBuffer * pVBuffer,
			VSString & OutString);
		void CreateVInputDeclareColor(VSVertexBuffer * pVBuffer,
			VSString & OutString);
		void CreateVInputDeclareSkin(VSVertexBuffer * pVBuffer,
			VSString & OutString);


		void CreateVOutputDeclarePosition(
			VSString & OutString);
		void CreateVOutputDeclareTexCoord(VSVertexBuffer * pVBuffer,unsigned int& ID,
			VSString & OutString);
		void CreateVOutputDeclareNormal(VSVertexBuffer * pVBuffer,unsigned int& ID,
			VSString & OutString);
		void CreateVOutputDeclareColor(VSVertexBuffer * pVBuffer,
			VSString & OutString);

		void CreateVOutputDeclareLocalPos(unsigned int& ID,
			VSString & OutString);

		void CreateVOutputDeclareWorldPos(unsigned int& ID,
			VSString & OutString);

		void CreateVOutputDeclareProjectPos(unsigned int& ID,
			VSString & OutString);

		void CreateVOutputDeclareProjectZ(unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantWorldViewProjectMatrix(VSShader * pShader,unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantLightShadowMatrix(VSShader * pShader,unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantProjectMatrix(VSShader * pShader, unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantViewProjectMatrix(VSShader * pShader,unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantViewMatrix(VSShader * pShader,unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantLightWorldPos(VSShader * pShader,unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantLightDirection(VSShader * pShader,unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantWorldViewMatrix(VSShader * pShader,unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantFarZ(VSShader * pShader,unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantSkyLightUpColor(VSShader * pShader,unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantSkyLightDownColor(VSShader * pShader,unsigned int& uiRegisterID,
			VSString & OutString);

		void CreateUserConstantLight(VSShader * pShader,MaterialShaderPara &MSPara,unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantPointLightRange(VSShader * pShader,unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantSkin(VSGeometry * pGeometry,VSShader * pShader,unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantWorldMatrix(VSShader * pShader,unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantCameraWorldPos(VSShader * pShader,unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantDirectionLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
			VSString & OutString);
		void CreateUserConstantPointLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
			VSString & OutString);
		void CreateUserConstantSpotLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
			VSString & OutString);

		void CreateUserConstantInvRTWidth(VSShader * pShader,unsigned int& uiRegisterID,
			VSString & OutString);

		void CreateUserConstantShadow(VSPShader* pPShader,VSArray<VSLight*> & LightArray,unsigned int &ID,VSString & OutString);
		void CreateUserConstantLightFuntion(VSPShader* pPShader,VSArray<VSLight*> & LightArray,unsigned int &ID,VSString & OutString);
		void CreateLightModleLookUpTable(VSPShader* pPShader,MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID,VSString & OutString);

		void SetUserConstantLightWorldPos(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantLightWorldDirection(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantWorldViewProjectMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserLightShadowMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);

		void SetUserConstantWorldViewMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantSkin(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantWorldMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantViewProjectMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantViewMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantProjectMatrix(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantCameraPos(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantSkyUpColor(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantSkyDownColor(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantLight(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantShadowSampler(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantLightFuntionSampler(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantFarZ(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantPointLightRange(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantInvRTWidth(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);

		void CreateVFuntionPositionAndNormal(MaterialShaderPara &MSPara,VSString & FuntionBody);
		void CreateVFuntionDualParaboloidPositionAndNormal(MaterialShaderPara &MSPara,VSString & FuntionBody);

		void CreateVFuntionVolumeShadowPositionAndNormal(MaterialShaderPara &MSPara,VSString & FuntionBody,unsigned int uiPass);
		void CreateVFuntionLocalPosition(MaterialShaderPara &MSPara,VSString & FuntionBody);
		void CreateVFuntionPosition(MaterialShaderPara &MSPara,VSString & FuntionBody);	
		void CreateVFuntionColor(MaterialShaderPara &MSPara,VSString & FuntionBody);
		void CreateVFuntionTexCoord(MaterialShaderPara &MSPara,VSString & FuntionBody);
		void CreateVFuntionProjectZ(MaterialShaderPara &MSPara,VSString & FuntionBody);
		void CreateVFuntionPost(MaterialShaderPara &MSPara,VSString & FuntionBody);
		void CreateVFuntionWorldPosition(MaterialShaderPara &MSPara,VSString & FuntionBody);
		void CreateVFuntionProjectPosition(MaterialShaderPara &MSPara,VSString & FuntionBody);

		void CreatePInputDeclareTexCoord(VSVertexBuffer * pVBuffer,unsigned int& ID,
			VSString & OutString);
		void CreatePInputDeclareNormal(VSVertexBuffer * pVBuffer,unsigned int& ID,
			VSString & OutString);
		void CreatePInputDeclareColor(VSVertexBuffer * pVBuffer,
			VSString & OutString);

		void CreatePInputDeclareLocalPos(unsigned int& ID,
			VSString & OutString);

		void CreatePInputDeclareProjectZ(unsigned int& ID,
			VSString & OutString);

		void CreatePInputDeclareWorldPos(unsigned int& ID,
			VSString & OutString);
		void CreatePInputDeclareProjectPos(unsigned int& ID,
			VSString & OutString);

		void CreateGBufferSampler(VSPShader* pPShader, MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID, VSString & OutString);
		protected:
			static VSString ms_IncludeShader;

			static VSString ms_DynamicShader;

			static VSAdapterInfo	ms_AdapterInfo[5];
			static UINT			ms_uiAdapterNum;
	};


}
#endif