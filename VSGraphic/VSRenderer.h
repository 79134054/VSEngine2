#ifndef VSRENDERER_H
#define VSRENDERER_H
#include "VSVector2.h"
#include "VSVector3W.h"
#include "VSRenderState.h"
#include "VSSamplerState.h"
#include "VSShaderKey.h"
#include "VSCamera.h"
namespace VSEngine2
{
	class VSCamera;
	class VSBind;
	class VSFont;
	class VSCamera;
	class VSTexAllState;
	class VSGeometry;
	class VSSpatial;
	class VS2DTexture;
	class VS3DTexture;
	class VSMaterial;
	class VSMaterialInstance;
	class VSMeshDate;
	class VS2DTexSampler;
	class VS1DTexSampler;
	class VSPutNode;
	class VSLight;
	class VSGeometry;
	class VSIndexBuffer;
	class VSMaterialInstance;
	class VSMul;
	class VSDiv;
	class VSDot;
	class VSSub;
	class VSAdd;
	class VSColorBuffer;
	class VSMakeValue;
	class VSSaturate;
	DECLARE_Ptr(VSDVGeometry);
	DECLARE_Ptr(VSTexture);
	DECLARE_Ptr(VSVertexFormat);
	DECLARE_Proxy(VSFont);
	typedef struct VSDISPLAYMODE_STURCT
	{
		UINT            uiWidth;			//显示分辨率
		UINT            uiHeight;
		UINT            uiRefreshRate;		//刷新率
		UINT			uiBitFormat;			
		VSString			StringExpress;		//字符串表示
	}VSDisplayMode, *VSDisplayModePtr;

	typedef struct VSDEVICEINFO_STURCT
	{
		UINT		uiMaxMultisample;
		UINT		uiMaxTextureWidth;
		UINT		uiMaxTextureHeight;
		UINT		uiMaxUseClipPlane;
		UINT		uiMaxTexture;
		UINT		fMaxAnisotropy;
		UINT		VertexShaderVersion;
		UINT		PixelShaderVersion;
		UINT		uiMaxRTNum;	
	}VSDeviceInfo,*VSDeviceInfoPtr;

	typedef struct VSADAPTERINFO_STURCT
	{
		unsigned int		m_uiAdapter;
		VSString			AdapterName;					
		VSArray<VSDisplayMode>	DisplayMode;	
		UINT			uiDisplayModeNum;
		//HAL SW REF
		VSDeviceInfo	DeviceInfo[3];	
	}VSAdapterInfo,*VSAdapterInfoPtr;
	class VSGRAPHIC_API VSRenderer
	{
	public:
		VSRenderer();
		virtual ~VSRenderer() = 0;
		friend class VSVertexBuffer;
		friend class VSVertexFormat;
		friend class VSDataBuffer;
		friend class VSDepthStencil;
		friend class VSTexture;
		friend class VSRenderTarget;
		friend class VSIndexBuffer;
		friend class VSCustomMaterial;
		friend class VSVShader;
		friend class VSPShader;
		friend class VSGShader;
		friend class VSDShader;
		friend class VSHShader;
		friend class VSCubeTexture;
		friend class VSSamplerState;
		friend class VSBlendState;
		friend class VSDepthStencilState;
		friend class VSRasterizerState;
		friend class VSRenderThreadSys;
		enum	//Render API Type
		{
			RAT_NULL,
			RAT_OPENGL,
			RAT_DIRECTX9,
			RAT_DIRECTX10,
			RAT_DIRECTX11,
			RAT_SOFTWARE,
			RAT_MAX
		};
		enum //function return information
		{
			FRI_FAIL,		
			FRI_SUCCESS,
			FRI_SAMERESOURCE
		};
		enum //Multisample
		{
			MS_NONE,
			MS_2,
			MS_4,
			MS_8,
			MS_16,
			MS_MAX
		};
		enum	//Surface Format Type
		{
			SFT_A8R8G8B8,      //BGRA
			SFT_D16,          
			SFT_D24X8,         
			SFT_D32,           
			SFT_A32B32G32R32F, //RGBA
			SFT_A16B16G16R16F, //RGBA
			SFT_G16R16F, 
			SFT_R16F, 
			SFT_R32F,
			SFT_DISCARD,
			SFT_D24S8,
			SFT_G32R32F,//RG
			SFT_R5G6B5,//BGR
			SFT_A8,
			SFT_DXT3,	//BC2
			SFT_DXT5,	//BC3
			SFT_MAX
		};
		
		
		virtual int GetRendererType () const = 0;
		virtual const TCHAR * GetRendererStringType() const = 0;
		// 颜色 深度 模板 buffer
		FORCEINLINE void SetClearColor (const VSColorRGBA& ClearColor);
		FORCEINLINE const VSColorRGBA& GetClearColor () const;

		FORCEINLINE void SetClearDepth (VSREAL fClearDepth);
		FORCEINLINE VSREAL GetClearDepth () const;

		FORCEINLINE void SetClearStencil (unsigned int uiClearValue);
		FORCEINLINE unsigned int GetClearStencil () const;
		


		

		enum //Clear Flag
		{
			CF_NONE = 0,
			CF_COLOR = 1 << 0,
			CF_DEPTH = 1 << 1,
			CF_STENCIL = 1 << 2,
			CF_USE_MAX = CF_COLOR | CF_DEPTH | CF_STENCIL
		};

		virtual void ClearBuffers(unsigned int uiClearFlag) = 0;
		virtual void ClearBackBuffer () = 0;
		virtual void ClearZBuffer () = 0;
		virtual void ClearStencilBuffer () = 0;
		virtual void ClearBuffers () = 0;
		
		
		virtual void ClearBuffers(unsigned int uiClearFlag,int iXPos, int iYPos, int iWidth,
			int iHeight) = 0;
		virtual void ClearBackBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight) = 0;
		virtual void ClearZBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight) = 0;
		virtual void ClearStencilBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight) = 0;
		virtual void ClearBuffers (int iXPos, int iYPos, int iWidth,
			int iHeight) = 0;

		void DrawText(int iX, int iY, const DWORD rColor,const TCHAR* acText, ...);		
		virtual bool SetViewPort(VSViewPort *pViewPort = NULL) = 0;

		
		void SetFont(VSFontR * pFont);

		virtual bool UseWindow(int uiWindowID = -1) = 0;
		
		virtual bool CooperativeLevel() = 0;
		virtual bool BeginRendering() = 0;
		virtual bool EndRendering() = 0;
		
		virtual void DeviceLost() = 0;
		virtual void ResetDevice() = 0;
		virtual bool ChangeScreenSize(unsigned int uiWidth,unsigned int uiHeight,bool bWindow) = 0;

		virtual const VSAdapterInfo* GetAdapterInfo(unsigned int & uiAdapterNum) = 0;
		static unsigned int GetBytesPerPixel(unsigned int uiFormatType);
		static unsigned int GetChannelPerPixel(unsigned int uiFormatType);
		static const VSString & GetVShaderProgramMain();
		static const VSString & GetPShaderProgramMain();
		static const VSString & GetGShaderProgramMain();
		FORCEINLINE unsigned int GetScreenWith()const;
		FORCEINLINE unsigned int GetScreenHeight()const;
		FORCEINLINE unsigned int GetDisplayFormat()const;

		FORCEINLINE unsigned int GetCurAnisotropy()const;
		FORCEINLINE unsigned int GetCurMultisample()const;
		FORCEINLINE unsigned int GetMaxMultisample()const;
		FORCEINLINE unsigned int GetDepthStencilFormat()const;
		FORCEINLINE bool IsWindowed()const;
		FORCEINLINE unsigned int WindoweNum()const;
		FORCEINLINE unsigned int GetMaxTextureWidth()const;
		FORCEINLINE unsigned int GetMaxTextureHeight()const;
		
		FORCEINLINE unsigned int GetSuitableMultisample(unsigned int Multisample)const;
		FORCEINLINE unsigned int GetMaxUseClipPlane()const;
		FORCEINLINE unsigned int GetMaxTexture()const;
		FORCEINLINE unsigned int GetMaxVTexture()const;
		FORCEINLINE unsigned int GetMaxGTexture()const;
		FORCEINLINE unsigned int GetBufferFormat()const;
		FORCEINLINE unsigned int GetMaxAnisotropy()const;

		FORCEINLINE unsigned int GetMaxRTNum()const;

		FORCEINLINE VSString GetAdapterName()const;

		FORCEINLINE unsigned int GetCurRTWidth()const;
		FORCEINLINE unsigned int GetCurRTHeight()const;
		FORCEINLINE unsigned int GetCurRTMultisampler()const;
		//Load Release
		bool LoadVShaderProgram (VSVShader * pVShaderProgram);
		bool ReleaseVShaderProgram(VSVShader * pVShaderProgram);

		bool LoadPShaderProgram (VSPShader * pPShaderProgram);
		bool ReleasePShaderProgram (VSPShader* pPShaderProgram);

		bool LoadGShaderProgram(VSGShader * pGShaderProgram);
		bool ReleaseGShaderProgram(VSGShader * pGShaderProgram);

		bool LoadDShaderProgram(VSDShader * pDShaderProgram);
		bool ReleaseDShaderProgram(VSDShader * pDShaderProgram);

		bool LoadHShaderProgram(VSHShader * pHShaderProgram);
		bool ReleaseHShaderProgram(VSHShader * pHShaderProgram);

		bool LoadTexture (VSTexture * pTexture);
		bool ReleaseTexture (VSTexture * pTexture);

		bool LoadVBuffer(VSVertexBuffer * pVBuffer);
		bool ReleaseVBuffer(VSVertexBuffer * pVBuffer);
		

		bool LoadIBuffer (VSIndexBuffer* pIBuffer);
		bool ReleaseIBuffer (VSIndexBuffer* pIBuffer);
	
		bool LoadDepthStencil (VSDepthStencil * pDepthStencil);
		bool ReleaseDepthStencil (VSDepthStencil * pDepthStencil);
		
		bool LoadRenderTarget (VSRenderTarget  * pRenderTarget);
		bool ReleaseRenderTarget  (VSRenderTarget * pRenderTarget);

		bool LoadBlendState (VSBlendState * pBlendState);
		bool ReleaseBlendState  (VSBlendState * pBlendState);

		bool LoadDepthStencilState (VSDepthStencilState * pDepthStencilState);
		bool ReleaseDepthStencilState  (VSDepthStencilState * pDepthStencilState);

		bool LoadRasterizerState(VSRasterizerState * pRasterizerState);
		bool ReleaseRasterizerState  (VSRasterizerState * pRasterizerState);

		bool LoadSamplerState(VSSamplerState * pSamplerState);
		bool ReleaseSamplerState  (VSSamplerState * pSamplerState);

		virtual bool DrawMesh(VSGeometry * pGeometry,VSRenderState * pRenderState,VSVShader * pVShader, VSPShader * pPShader,VSGShader * pGShader) = 0;
		struct SCREEN_QUAD_TYPE
		{
			VSVector3 Point;
		};
		struct SCREEN_FONT_TYPE
		{
			VSVector3 Point;
			VSVector2 UV;
			DWORD Color;
		};
		virtual bool DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]) = 0;
		virtual bool DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer,unsigned int uiVertexNum,
								VSUSHORT_INDEX * pIndexBuffer,unsigned int uiIndexNum) = 0;
		virtual bool DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
			VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum) = 0;
		void SetUseState(VSRenderState & RenderState,unsigned int uiRenderStateInheritFlag);
		void ClearUseState();
		FORCEINLINE const VSRenderState & GetUseState() const
		{
			return m_UseState;
		}
		FORCEINLINE unsigned int GetRenderStateInheritFlag()const
		{
			return m_uiRenderStateInheritFlag;
		}
		void SetRenderState(VSRenderState & RenderState);
		virtual unsigned int SetBlendState(VSBlendState * pBlendState,bool bForceSet = false) = 0;
		virtual unsigned int SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet = false) = 0;
		virtual unsigned int SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet = false) = 0;
		virtual void SetClipPlane(VSArray<VSPlane3> &Plane,bool bEnable) = 0;
		virtual void SetScissorRect(VSArray<VSRect2> &Rect,bool bEnable) = 0;

		virtual void SetTexAllState(VSTexAllState * pTexAllState,unsigned int i);
		virtual void SetVTexAllState(VSTexAllState * pTexAllState,unsigned int i);
		virtual void SetGTexAllState(VSTexAllState * pTexAllState, unsigned int i);
		virtual void SetDTexAllState(VSTexAllState * pTexAllState, unsigned int i);
		virtual void SetHTexAllState(VSTexAllState * pTexAllState, unsigned int i);

		virtual unsigned int SetVShader(VSVShader * pVShader) = 0;
		virtual unsigned int SetPShader(VSPShader * pPShader) = 0;
		virtual unsigned int SetGShader(VSGShader * pGShader) = 0;
		virtual unsigned int SetDShader(VSDShader * pDShader) = 0;
		virtual unsigned int SetHShader(VSHShader * pHShader) = 0;

		virtual unsigned int SetVBuffer(VSVertexBuffer * pVBuffer) = 0;
		virtual unsigned int SetIBuffer (VSIndexBuffer* pIBuffer) = 0;

		virtual unsigned int SetTexture (VSTexture* pTexture,unsigned int i) = 0;
		virtual unsigned int SetVTexture (VSTexture* pTexture,unsigned int i) = 0;
		virtual unsigned int SetGTexture(VSTexture* pTexture, unsigned int i) = 0;
		virtual unsigned int SetDTexture(VSTexture* pTexture, unsigned int i) = 0;
		virtual unsigned int SetHTexture(VSTexture* pTexture, unsigned int i) = 0;

		unsigned int SetMesh(VSMeshDate *pMeshDate);



		virtual unsigned int SetSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false) = 0;
		virtual unsigned int SetVSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false) = 0;
		virtual unsigned int SetGSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false) = 0;
		virtual unsigned int SetDSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false) = 0;
		virtual unsigned int SetHSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false) = 0;

		virtual bool SetRenderTargets(VSRenderTarget * pRenderTargets[], unsigned int uiNum);
		virtual bool EndRenderTargets(VSRenderTarget * pRenderTargets[], unsigned int uiNum);
		virtual bool SetRenderTarget(VSRenderTarget *pRenderTarget,unsigned int i);
		virtual bool EndRenderTarget(VSRenderTarget *pRenderTarget,unsigned int i);
		virtual bool SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer) = 0;
		virtual bool EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer) = 0;
		virtual unsigned int GetTextureCompressLen(unsigned int uiLen) = 0;

		void SetVSTextureRTTextureUsed(VSTexture * pTexture, unsigned int uiSlot, bool b);
		void SetPSTextureRTTextureUsed(VSTexture * pTexture, unsigned int uiSlot, bool b);
		void SetGSTextureRTTextureUsed(VSTexture * pTexture, unsigned int uiSlot, bool b);
		void SetDSTextureRTTextureUsed(VSTexture * pTexture, unsigned int uiSlot, bool b);
		void SetHSTextureRTTextureUsed(VSTexture * pTexture, unsigned int uiSlot, bool b);

		void RenderStateToDefault();
		
		void EnableTextureRTUsed(VSTexture * pTexture);
		void DisableTextureRTUsed(VSTexture * pTexture);

		bool CheckIsTextureCanSet(VSTexture * pTexture);
		struct ChildWindowInfo
		{
			HWND	m_hHwnd;
			unsigned int m_uiWidth;
			unsigned int m_uiHeight;
			bool		 m_bDepth;
		};
		ChildWindowInfo * GetChildWindowInfo(int uiID);

		virtual bool IsSupportMulBufferSwtich()const = 0;
		virtual bool IsSupportGS()const = 0;
protected:
		
		HWND	m_hMainWindow;          // 主窗口						
		ChildWindowInfo * m_pChildWindowInfo;
		int		m_iNumChildWindow;          // 窗口个数
		int		m_iCurWindowID;
		bool	m_bIsRendering;


		VSColorRGBA m_ClearColor;
		VSREAL m_fClearDepth;
		unsigned int m_uiClearStencil;
	

		VSFontRPtr			m_pFont;
		

		VSVertexFormat*		m_pVertexFormat;
		VSVertexBuffer*		m_pVertexBuffer;
		VSIndexBuffer*		m_pIndexBuffer;
		VSVShader*			m_pVShader;
		VSPShader*			m_pPshader;
		VSGShader*			m_pGShader;
		VSDShader*			m_pDShader;
		VSHShader*			m_pHShader;

		VSVertexFormatPtr   m_pQuadVertexFormat;	

		VSTexture *m_pVTex[TEXLEVEL];
		VSSamplerState *	m_pVSamplerState[TEXLEVEL];

		VSTexture *m_pPTex[TEXLEVEL];
		VSSamplerState *	m_pPSamplerState[TEXLEVEL];

		
		VSTexture *m_pGTex[TEXLEVEL];
		VSSamplerState *	m_pGSamplerState[TEXLEVEL];

		VSTexture *m_pDTex[TEXLEVEL];
		VSSamplerState *	m_pDSamplerState[TEXLEVEL];

		VSTexture *m_pHTex[TEXLEVEL];
		VSSamplerState *	m_pHSamplerState[TEXLEVEL];

		VSRenderState m_LocalRenderState;
		
		VSBlendState * m_pBlendState;
		VSDepthStencilState * m_pDepthStencilState;
		VSRasterizerState * m_pRasterizerState;
		bool				m_bClipPlaneEnable;
		bool				m_bScissorRectEnable;
		VSRenderState		m_UseState;
		unsigned int		m_uiRenderStateInheritFlag;

		enum //BACK_MACTH_TYPE
		{
			BMT_NONE = 0x00,
			BMT_RENDER_TARGET = 0x01,
		};
		unsigned int		m_uiBackMacthType;
		virtual void SetVShaderConstant(VSVShader * pShader) = 0;
		virtual void SetVShaderSampler(VSVShader * pShader) = 0;

		virtual void SetGShaderConstant(VSGShader * pShader) = 0;
		virtual void SetGShaderSampler(VSGShader * pShader) = 0;

		virtual void SetDShaderConstant(VSDShader * pShader) = 0;
		virtual void SetDShaderSampler(VSDShader * pShader) = 0;

		virtual void SetHShaderConstant(VSHShader * pShader) = 0;
		virtual void SetHShaderSampler(VSHShader * pShader) = 0;

		virtual void SetPShaderConstant(VSPShader * pShader) = 0;
		virtual void SetPShaderSampler(VSPShader * pShader) = 0;

		virtual bool SetVShaderProgram(VSVShader*	pVShaderProgram) = 0;
		virtual bool SetPShaderProgram(VSPShader*	pPShaderProgram) = 0;
		virtual bool SetGShaderProgram(VSGShader*	pGShaderProgram) = 0;
		virtual bool SetDShaderProgram(VSDShader*	pDShaderProgram) = 0;
		virtual bool SetHShaderProgram(VSHShader*	pHShaderProgram) = 0;
		//Load Release
		virtual bool OnLoadVShaderProgram (VSVShader * pVShaderProgram,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseVShaderProgram(VSResourceIdentifier * pVShaderProgramID) = 0;

		virtual bool OnLoadPShaderProgram (VSPShader* pPShaderProgram,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleasePShaderProgram(VSResourceIdentifier* pGShaderProgramID) = 0;

		virtual bool OnLoadGShaderProgram(VSGShader * pGShaderProgram, VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseGShaderProgram(VSResourceIdentifier * pGShaderProgramID) = 0;

		virtual bool OnLoadDShaderProgram(VSDShader * pDShaderProgram, VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseDShaderProgram(VSResourceIdentifier * pDShaderProgramID) = 0;

		virtual bool OnLoadHShaderProgram(VSHShader * pHShaderProgram, VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseHShaderProgram(VSResourceIdentifier * pHShaderProgramID) = 0;

		virtual bool OnLoadTexture (VSTexture * pTexture,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseTexture (VSResourceIdentifier * pTextureID) = 0;

		virtual bool OnLoadDepthStencil (VSDepthStencil * pDepthStencil,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseDepthStencil (VSResourceIdentifier * pDepthStencilID) = 0;
		
		virtual bool OnLoadRenderTarget (VSRenderTarget * pRenderTarget,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseRenderTarget (VSResourceIdentifier * pRenderTarget) = 0;

		virtual bool OnLoadVBufferDate (VSVertexBuffer * pVBuffer,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseVBufferDate (VSResourceIdentifier* pVBufferID) = 0;

		virtual bool OnLoadVBufferFormat (VSVertexFormat * pVertexFormat,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseVBufferFormat (VSResourceIdentifier* pVBufferID) = 0;

		virtual bool OnLoadIBuffer (VSIndexBuffer* pIBuffer,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseIBuffer (VSResourceIdentifier* pIBufferID) = 0;

		virtual bool OnLoadBlendState (VSBlendState * pBlendState,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseBlendState  (VSResourceIdentifier * pBlendStateID) = 0;

		virtual bool OnLoadDepthStencilState (VSDepthStencilState * pDepthStencilState,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseDepthStencilState  (VSResourceIdentifier * pDepthStencilStateID) = 0;

		virtual bool OnLoadRasterizerState(VSRasterizerState * pRasterizerState,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseRasterizerState  (VSResourceIdentifier * pRasterizerStateID) = 0;

		virtual bool OnLoadSamplerState(VSSamplerState * pSamplerState,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseSamplerState  (VSResourceIdentifier * pSamplerStateID) = 0;

		virtual unsigned int SetVertexFormat(VSVertexFormat * pVertexFormat) = 0;

		virtual void * Lock(VSVertexBuffer * pVertexBuffer) = 0;
		virtual void UnLock(VSVertexBuffer * pVertexBuffer) = 0;

		virtual void * Lock(VSIndexBuffer * pIndexBuffer) = 0;
		virtual void UnLock(VSIndexBuffer * pIndexBuffer) = 0;

		virtual void * Lock(VSTexture * pTexture,unsigned int uiLevel,unsigned int uiFace) = 0;
		virtual void UnLock(VSTexture * pTexture,unsigned int uiLevel,unsigned int uiFace) = 0;

		virtual void * Lock(VSRenderTarget * pRenderTarget) = 0;
		virtual void UnLock(VSRenderTarget * pRenderTarget) = 0;

		virtual void * Lock(VSDepthStencil * pDepthStencil) = 0;
		virtual void UnLock(VSDepthStencil * pDepthStencil) = 0;

		virtual bool CopyResourceBuffer(VS2DTexture * pSource,VSCubeTexture * pDest,unsigned int uiFace) = 0;
		
		bool SetDefaultValue();
		bool ReleaseDefaultValue();

		//shader 
		FORCEINLINE static DWORD ShaderVersion(UINT Major,UINT Minor); 
	
		
		

		virtual bool ReleaseBindObjectResource();
	protected:
		enum	//device type
		{
			DT_HAL = 0,
			DT_REF = 1,
			DT_MAX
		};
		

		unsigned int	m_uinAdapter;
		unsigned int	m_uiDevType;
		VSString		m_AdapterName;
		UINT		m_uiScreenWidth;
		UINT		m_uiScreenHeight;
		UINT		m_uiCurRTWidth;
		UINT		m_uiCurRTHeight;
		UINT		m_uiDisplayFormat;
		UINT		m_uiBufferFormat;
		bool		m_bWindowed;


		UINT		m_uiDepthStencilFormat;
		UINT		m_uiCurAnisotropy;
		UINT		m_uiCurMultisample;
		UINT		m_uiCurRTMultisampler;
		DWORD		m_dwMultisampleQuality;
		UINT		m_uiMaxMultisample;		
		UINT		m_uiMaxTextureWidth;
		UINT		m_uiMaxTextureHeight;

		UINT		m_uiMaxUseClipPlane;
		UINT		m_uiMaxTexture;
		UINT		m_uiMaxVTexture;
		UINT		m_uiMaxGTexture;
		UINT		m_uiMaxDTexture;
		UINT		m_uiMaxHTexture;
		UINT		m_uiMaxAnisotropy;

		UINT		m_uiMaxRTNum;
		
	

		static VSString ms_VShaderProgramMain;
		static VSString ms_PShaderProgramMain;
		static VSString ms_GShaderProgramMain;

		static unsigned int ms_uiBytesPerPixel[SFT_MAX];
		static unsigned int ms_uiChannelPerPixel[SFT_MAX];
		static SCREEN_QUAD_TYPE ms_FullScreen[4];
		static VSUSHORT_INDEX ms_FullScreenI[6];
		VSArray<SCREEN_FONT_TYPE> m_FontVertex;
		VSArray<VSUSHORT_INDEX> m_FontIndex;

		VSViewPort m_CurViewPort;
	//shader string
	public:
		//shader function
		virtual VSString TexColorBuffer(const VSColorBuffer * pColorBuffer) const = 0;
		virtual VSString Tex2D(const VS2DTexSampler * p2DTexSampler) const= 0;
		virtual VSString Tex1D(const VS1DTexSampler * p1DTexSampler) const= 0;
		virtual VSString Mul(const VSMul * pMul) const= 0;
		virtual VSString Div(const VSDiv * pDiv) const = 0;
		virtual VSString Dot(const VSDot * pDot) const= 0;
		virtual VSString Sub(const VSSub * pSub) const= 0;
		virtual VSString Add(const VSAdd * pAdd) const = 0;
		virtual VSString MakeValue(const VSMakeValue * pMakeValue) const = 0;
		virtual VSString Saturate(const VSSaturate * pSaturate) const = 0;
		//shader key word
		enum //Float Index
		{
			FI_1,
			FI_2,
			FI_3,
			FI_4,
			FI_MAX

		};
		VSString Float(unsigned int uiIndex)const;
		virtual VSString Float()const =  0;
		virtual VSString Float2()const =  0;
		virtual VSString Float3()const =  0;
		virtual VSString Float4()const =  0;
		virtual VSString Return()const = 0;

		virtual VSString FloatConst(const VSString & Value1)const =  0;
		virtual VSString Float2Const(const VSString & Value1,const VSString & Value2)const =  0;
		virtual VSString Float3Const(const VSString & Value1,
						const VSString & Value2,const VSString & Value3)const =  0;
		virtual VSString Float4Const(const VSString & Value1,const VSString & Value2,
					const VSString & Value3,const VSString & Value4)const =  0;

		virtual void CustomPointLightUseString(VSString & PointLightUseString ,VSString & PointAttenuationDivString) = 0;

		virtual void CustomSpotLightUseString(VSString & SpotLightUseString ,VSString & SpotAttenuationDivString) = 0;

		enum //Value Element
		{
			VE_NONE = 0,
			VE_A = BIT(0),
			VE_R = BIT(1),
			VE_G = BIT(2),
			VE_B = BIT(3),
			DF_ALL = 0X0F
		};
		virtual VSString GetValueElement(const VSPutNode * pPutNode,
			unsigned char uiVE)const = 0;
		virtual VSString GetValueElement(const VSString & InputString,
			unsigned char uiVE)const = 0;
		
		virtual VSString GetDeclareValue(const VSString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const = 0;
		virtual VSString GetDeclareSampler(const VSString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex)const = 0;
		
		//shader custom function
		virtual void CreateVInputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString) = 0;
		virtual void CreateVOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString) = 0;
		virtual void CreateVUserConstant(VSVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString) = 0;
		virtual void CreateVFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString) = 0;

		virtual void SetMaterialVShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,VSVShader *pVShader) = 0;

		virtual void CreatePInputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString) = 0;
		virtual void CreatePOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString) = 0;
		virtual void CreatePUserConstant(VSPShader * pPShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString) = 0;
		virtual void CreatePFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString) = 0;

		virtual void SetMaterialPShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,VSPShader *pPShader) = 0;

		virtual void CreateGInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			VSString & OutString) = 0;
		virtual void CreateGOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			VSString & OutString) = 0;
		virtual void CreateGUserConstant(VSGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
			VSString & OutString) = 0;
		virtual void CreateGFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
			VSString & OutString) = 0;
		virtual void SetMaterialGShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, VSGShader *pGShader) = 0;

		virtual void GetIncludeShader(VSString &OutString) = 0;
		
		virtual void GetDynamicShader(VSString &OutString) = 0;

// 		enum //GpuSkin Flag
// 		{
// 			GKF_POSITION,
// 			GKF_POSITION_NORMAL,
// 			GKF_POSITION_BUMP,
// 			GKF_MAX
// 		};
// 		virtual void GpuSkin(unsigned int uiFlag,
// 						const VSString & BlendIndex,const VSString &BlendWeight,const BoneMatrixName,
// 							const VSString & InputPos,const VSString &InputNormal,const VSString &InputTangent,const VSString &InputBinormal,
// 							VSString & OutPos,VSString &Outormal,VSString &OutTangent,VSString &OutBinormal)const = 0;

		virtual void SkyLight(const VSString &WorldNormal,const VSString &UpColor,const VSString & DownColor, const VSString & DiffuseColor,VSString & OutString) = 0;
		virtual void DirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
									const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
									VSString & OutString)const = 0;
		virtual void CustomDirectionalLight(int iLightNum,const VSString &CustomString,VSArray<VSString> ShadowString,VSString & OutString)const = 0;

		virtual void OrenNayarDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
									const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
									VSString & OutString)const = 0;
		virtual void MinnaertDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Power,
									const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
									VSString & OutString)const = 0;
		virtual void StraussDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,const VSString &Smoothness,
									const VSString &Metalness,const VSString &Transparency,const VSString &K,
									const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
									VSString & OutString)const = 0;
		virtual void ShirleyDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,
									const VSString &NU,const VSString &NV,const VSString &Rs,
									const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
									VSString & OutString)const = 0;
		virtual void SchlickDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
										const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
										VSString & OutString)const = 0;
		virtual void CookTorranceDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular , 
										const VSString &Roughness , const VSString &C , const VSString &R,
										const VSString &WorldNormal,const VSString &WorldCameraDir,unsigned int uiSpecularType,VSArray<VSString> ShadowString,
										VSString & OutString)const = 0;

		virtual void IsotropicWardDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular , 
			const VSString &Roughness ,
			const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
			VSString & OutString)const = 0;
		virtual void AnisotropicWardDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular , 
			const VSString &RoughnessX ,const VSString &RoughnessY ,
			const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
			VSString & OutString)const = 0;

		virtual void PointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
							const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
							VSString & OutString)const = 0;
		virtual void CustomPointLight(int iLightNum,const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const = 0;

		virtual void OrenNayarPointLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
							const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
							VSString & OutString)const = 0;
		virtual void MinnaertPointLight(int iLightNum,const VSString &Diffuse,const VSString &Power,
							const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
							VSString & OutString)const = 0;
		virtual void StraussPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,const VSString &Smoothness,
							const VSString &Metalness,const VSString &Transparency,const VSString &K,
							const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
							VSString & OutString)const = 0;
		virtual void ShirleyPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,
							const VSString &NU,const VSString &NV,const VSString &Rs,
							const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
							VSString & OutString)const = 0;
		virtual void SchlickPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
							const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
							VSString & OutString)const = 0;
		virtual void CookTorrancePointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &Roughness , const VSString &C , const VSString &R,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,unsigned int uiSpecularType,VSArray<VSString> ShadowString,
			VSString & OutString)const = 0;

		virtual void IsotropicWardPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &Roughness ,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
			VSString & OutString)const = 0;
		virtual void AnisotropicWardPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &RoughnessX ,const VSString &RoughnessY ,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
			VSString & OutString)const = 0;

		virtual void SpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
								VSString & OutString)const = 0;
		virtual void CustomSpotLight(int iLightNum,const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const = 0;
		virtual void OrenNayarSpotLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
								VSString & OutString)const = 0;
		virtual void MinnaertSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Power,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
								VSString & OutString)const = 0;
		virtual void StraussSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,const VSString &Smoothness,
								const VSString &Metalness,const VSString &Transparency,const VSString &K,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
								VSString & OutString)const = 0;
		virtual void ShirleySpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,
								const VSString &NU,const VSString &NV,const VSString &Rs,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
								VSString & OutString)const = 0;
		virtual void SchlickSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
								VSString & OutString)const = 0;
		virtual void CookTorranceSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &Roughness , const VSString &C , const VSString &R,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,unsigned int uiSpecularType,VSArray<VSString> ShadowString,
			VSString & OutString)const = 0;
		virtual void IsotropicWardSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &Roughness ,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
			VSString & OutString)const = 0;
		virtual void AnisotropicWardSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
			const VSString &RoughnessX ,const VSString &RoughnessY ,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
			VSString & OutString)const = 0;

		virtual void LocalToWorldPos(const VSString & LocalPos,VSString &OutString)const = 0;
		virtual void WorldToViewPos(const VSString & LocalPos,VSString &OutString)const = 0;

		virtual void LocalToWorldNormal(const VSString & LocalNormal,VSString &OutString)const = 0;

		virtual void WorldToViewNormal(const VSString & WorldNormal,VSString &OutString)const = 0;

		virtual void GetWorldViewDir(VSString &OutString)const = 0;

		virtual void LocalToViewNormal(const VSString & LocalNormal,VSString &OutString)const = 0;
		virtual void TransProjPos(const VSString & Pos,const VSString & Matrix,VSString &OutString)const = 0;
		virtual void BumpNormal(const VSString &TexNormal,VSString &OutString)const = 0;

		virtual void ComputeDir(const VSString & Orgin,const VSString & End,VSString &OutString)const = 0;
		virtual void ComputeLength(const VSString & Orgin,const VSString & End,VSString &OutString)const = 0;
		virtual void ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,const VSString &BoneVector,
					 const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const = 0;

		virtual void BoneTranPos( const VSString & LocalPos,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const = 0;
		virtual void BoneTranNormal( const VSString & LocalNormal,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const = 0;

		virtual void DecodeNormal1(const VSString & Normal,VSString &OutString) const= 0;
		virtual void RecodeNormal1(const VSString & Normal,VSString &OutString) const = 0;
		virtual void DecodeNormal2(const VSString & Normal,const VSString & Tengent,VSString &OutString) const= 0;

		virtual void DecodeNormal3(const VSString & Normal,VSString &OutString) const = 0;
		virtual void RecodeNormal3(const VSString & Normal,VSString &OutString) const = 0;

		virtual void Reciprocal(const VSString & Value, VSString &OutString)const = 0;
		virtual void Clip(const VSString & Value,VSString &OutString)const = 0;
		virtual void GreaterZeroPow(const VSString & Value,VSREAL Exp,VSString &OutString) const= 0;
		virtual void SaturatePow(const VSString & Value,VSREAL Exp,VSString &OutString) const= 0;
		virtual void Saturate(const VSString & Value,VSString &OutString)const = 0;

		virtual void PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler , const VSString & ViewMatrix,
									const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler ,
			const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void PointLightVolumeShadow(const VSString & PointLightName , const VSString & VolumeShadowSampler , 
									const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void PointLightDualParaboloidShadow(const VSString & PointLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const = 0;
		virtual void DirectionLightVolumeShadow(const VSString & DirLightName , const VSString & VolumeShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void DirectionLightCSMShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, const VSString & ViewPos,const VSString &FarZ,VSString& OutString)const = 0;

		virtual void DirectionLightShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void SpotLightShadow(const VSString & SpotLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void TranLightToTemp(VSArray<VSLight *> LightArray,VSString & OutString)const = 0;

		virtual void GetLightFunction(VSArray<VSLight *> LightArray,const VSString & WorldPos,VSString & OutString)const = 0;

		virtual void EncodeReflect(const VSString & Mip , const VSString & Pow , VSString& OutString) = 0;

		virtual void DecodeReflect(const VSString & Mip , const VSString & Pow , const VSString & Value,VSString& OutString) = 0;

	public:
		static VSRenderer * ms_pRenderer;
	
	};

#include "VSRenderer.inl"
}


#endif