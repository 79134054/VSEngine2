#ifndef VSRESOURCEMANAGER_H
#define VSRESOURCEMANAGER_H
#include "VSObject.h"
#include "VSString.h"
#include "VSArray.h"
#include "VSMap.h"
#include "VSName.h"
#include "VSVertexFormat.h"
#include "VSShaderMap.h"
#include "VS1DTexture.h"
#define  USE_VECTOR_TRANSFORM_BONE
#include "VSCubeTexture.h"
#include "VS2DTexture.h"
#include "VS3DTexture.h"
namespace VSEngine2
{

	class VSResouceControll
	{
	public:
		VSResouceControll(unsigned int uiGCMaxTimeCount);
		virtual ~VSResouceControll();
		virtual void GCResource();
	protected:
		VSArray<unsigned int> m_TimeCount;
		unsigned int m_uiGCMaxTimeCount;
	};
	template<class T>
	class VSResourceArrayControll : public VSResouceControll
	{
	public:
		VSResourceArrayControll(unsigned int uiGCMaxTimeCount = 5000);
		~VSResourceArrayControll();
		bool AddResource(const T & R);
		bool DeleteResource(const T & R);
		virtual void GCResource();
		bool IsRealeseAll();
		void ClearAll();
		T  GetResource(unsigned int i);
		void ClearTimeCount(unsigned int i);
		unsigned int GetResourceNum();
	protected:
		VSArrayOrder<T> m_ResourceArray;
	};
	template<class T>
	VSResourceArrayControll<T>::VSResourceArrayControll(unsigned int uiGCMaxTimeCount)
		: VSResouceControll(uiGCMaxTimeCount)
	{

	}
	template<class T>
	VSResourceArrayControll<T>::~VSResourceArrayControll()
	{

	}
	template<class T>
	T  VSResourceArrayControll<T>::GetResource(unsigned int i)
	{
		if (i >= m_ResourceArray.GetNum())
		{
			return NULL;
		}

		return m_ResourceArray[i];
	}
	template<class T>
	unsigned int VSResourceArrayControll<T>::GetResourceNum()
	{
		return m_ResourceArray.GetNum();
	}
	template<class T>
	void VSResourceArrayControll<T>::ClearTimeCount(unsigned int i)
	{
		m_TimeCount[i] = 0;
	}
	template<class T>
	bool VSResourceArrayControll<T>::AddResource(const T & R)
	{
		m_ResourceArray.AddElement(R);
		m_TimeCount.AddElement(0);
		return true;
	}
	template<class T>
	bool VSResourceArrayControll<T>::DeleteResource(const T & R)
	{
		unsigned int i = m_ResourceArray.FindElement(R);
		if (i == m_ResourceArray.GetNum())
		{
			return false;
		}
		else
		{
			m_ResourceArray.Erase(i);
			m_TimeCount.Erase(i);
		}
		return true;
	}
	template<class T>
	void VSResourceArrayControll<T>::GCResource()
	{
		unsigned int i = 0 ;
		while(i < m_ResourceArray.GetNum())
		{
			if(m_ResourceArray[i]->GetRef() == 1)
			{
				m_TimeCount[i]++;
				if (m_TimeCount[i] > m_uiGCMaxTimeCount)
				{
					m_ResourceArray.Erase(i);
					m_TimeCount.Erase(i);
					break;
				}
				else
				{
					i++;
				}			
			}
			else
			{
				m_TimeCount[i] = 0;
				i++;
			}
		}
	}
	template<class T>
	bool VSResourceArrayControll<T>::IsRealeseAll()
	{
		return m_ResourceArray.GetNum() == 0;
	}
	template<class T>
	void VSResourceArrayControll<T>::ClearAll()
	{
		m_ResourceArray.Clear();
	}
	template<class KEY,class VALUE>
	class VSResourceSet : public VSResouceControll
	{
	protected:
		VSMapOrder<KEY,VALUE> m_Resource;
	public:
		VSResourceSet(unsigned int uiGCMaxTimeCount = 5000);
		~VSResourceSet();
		//写在每个加载资源函数里面的，如果存在这个资源，则返回这个资源指针，否则在加载资源函数里面创建这个资源
		VALUE  CheckIsHaveTheResource(const KEY & Key);
		
		bool AddResource(const KEY & Key,VALUE pObject);
		//写在每个资源的析构函数里面
		bool DeleteResource(const KEY & Key);


		unsigned int GetResourceNum();

		bool IsRealeseAll();
		void ClearAll();
		const MapElement<KEY,VALUE> * GetResource(unsigned int i);
		void GCResource();
		unsigned int GetResourceIndexByKey(const KEY & Key)const;


	};
	template<class KEY,class VALUE>
	class VSProxyResourceSet : public VSResourceSet<KEY,VALUE>
	{
	public:
		VSProxyResourceSet(unsigned int uiGCMaxTimeCount = 5000);
		~VSProxyResourceSet();
		void GCResource();
	};
	template<class KEY,class VALUE>
	unsigned int VSResourceSet<KEY,VALUE>::GetResourceIndexByKey(const KEY & Key)const
	{

		unsigned int i = m_Resource.Find(Key);
		return i;


	}
	template<class KEY,class VALUE>
	void VSResourceSet<KEY,VALUE>::GCResource()
	{
		unsigned int i = 0 ; 
		while(i < m_Resource.GetNum())
		{
			const MapElement<KEY,VALUE> & Resource = m_Resource[i];
			if(Resource.Value->GetRef() == 1)
			{
				m_TimeCount[i]++;
				if (m_TimeCount[i] > m_uiGCMaxTimeCount)
				{
					m_Resource.Erase(i);
					m_TimeCount.Erase(i);
					break;
				}
				else
				{
					i++;
				}
			}
			else
			{
				m_TimeCount[i] = 0;
				i++;
			}
		}

	}
	template<class KEY,class VALUE>
	VSResourceSet<KEY,VALUE>::VSResourceSet(unsigned int uiGCMaxTimeCount)
		: VSResouceControll(uiGCMaxTimeCount)
	{
		m_Resource.Clear();
	}
	template<class KEY,class VALUE>
	VSResourceSet<KEY,VALUE>::~VSResourceSet()
	{
		m_Resource.Clear();
	}
	template<class KEY,class VALUE>
	VALUE  VSResourceSet<KEY,VALUE>::CheckIsHaveTheResource(const KEY & Key)
	{
		unsigned int i = m_Resource.Find(Key);
		if(i == m_Resource.GetNum())
			return NULL;
		//Clear TimeCout
		m_TimeCount[i] = 0;
		return m_Resource[i].Value;

	}

	template<class KEY,class VALUE>
	bool VSResourceSet<KEY,VALUE>::AddResource(const KEY & Key,VALUE  pObject)
	{
		if(!pObject)
			return 0;


		m_Resource.AddElement(Key,pObject);
		m_TimeCount.AddElement(0);
		
		return 1;
	}
	template<class KEY,class VALUE>
	bool VSResourceSet<KEY,VALUE>::DeleteResource(const KEY & Key)
	{

		unsigned int i = m_Resource.Find(Key);
		if(i == m_Resource.GetNum())
			return false;
		m_Resource.Erase(i);
		m_TimeCount.Erase(i);
		return true;


	}
	
	template<class KEY,class VALUE>
	unsigned int VSResourceSet<KEY,VALUE>::GetResourceNum()
	{
		return m_Resource.GetNum();
	}

	template<class KEY,class VALUE>
	bool VSResourceSet<KEY,VALUE>::IsRealeseAll()
	{
		return !m_Resource.GetNum();
	}
	template<class KEY,class VALUE>
	void VSResourceSet<KEY,VALUE>::ClearAll()
	{
		m_Resource.Clear();
	}
	template<class KEY,class VALUE>
	const MapElement<KEY,VALUE> * VSResourceSet<KEY,VALUE>::GetResource(unsigned int i)
	{
		VSMAC_ASSERT(i < m_Resource.GetNum());
		if (i >= m_Resource.GetNum())
		{
			return NULL;
		}
		return &m_Resource[i];
	}
	
	template<class KEY,class VALUE>
	VSProxyResourceSet<KEY,VALUE>::VSProxyResourceSet(unsigned int uiGCMaxTimeCount)
		:VSResourceSet<KEY,VALUE>(uiGCMaxTimeCount)
	{
	}
	template<class KEY,class VALUE>
	VSProxyResourceSet<KEY,VALUE>::~VSProxyResourceSet()
	{

	}
	template<class KEY,class VALUE>
	void VSProxyResourceSet<KEY,VALUE>::GCResource()
	{
		unsigned int i = 0;
		while(i < m_Resource.GetNum())
		{
			const MapElement<KEY,VALUE> & Resource = m_Resource[i];
			if(Resource.Value->IsEndableGC() && Resource.Value->IsLoaded() && Resource.Value->GetRef() == 1)
			{
				m_TimeCount[i]++;
				if (m_TimeCount[i] > m_uiGCMaxTimeCount)
				{
					m_Resource.Erase(i);
					m_TimeCount.Erase(i);
				}
				else
				{
					i++;
				}

			}
			else
			{
				m_TimeCount[i] = 0;
				i++;
			}
		}

	}

	class VSTexture;
	class VSDataBuffer;
	class VSFont;
	class VSShaderProgram;
	class VSVertexBuffer;
	class VSVertexFormat;
	class VSGeometry;
	class VSMaterial;
	class VSLight;
	class VSVShader;
	class VSPShader;
	class VSName;
	class VSTexAllState;
	class VSSkelectonMeshNode;
	class VSAnimSet;
	class VSCaptureTexAllState;
	class VSActor;
	class VSSceneMap;
	DECLARE_Ptr(VSTexAllState);
	DECLARE_Ptr(VSName);
	DECLARE_Ptr(VSVShader);
	DECLARE_Ptr(VSPShader);
	DECLARE_Ptr(VSGShader);
	DECLARE_Ptr(VSSkelectonMeshNode);

	DECLARE_Ptr(VSStaticMeshNode);
	DECLARE_Ptr(VSAnimSet);
	DECLARE_Ptr(VSMaterial);
	DECLARE_Ptr(VSFont);
	DECLARE_Ptr(VSVertexFormat);
	DECLARE_Ptr(VSFSM);
	DECLARE_Ptr(VSAnimTree);
	DECLARE_Ptr(VSMorphTree);
	DECLARE_Ptr(VSAnim);


	class VSDynamicBufferGeometry;


	DECLARE_Ptr(VSPostEffectSet);

	DECLARE_Proxy(VSTexAllState);
	DECLARE_Proxy(VSMaterial);
	DECLARE_Proxy(VSAnim);
	DECLARE_Proxy(VSFont);

	DECLARE_Proxy(VSStaticMeshNode);
	DECLARE_Proxy(VSSkelectonMeshNode);
	DECLARE_Proxy(VSPostEffectSet);
	DECLARE_Proxy(VSAnimTree);
	DECLARE_Proxy(VSMorphTree);
	DECLARE_Proxy(VSActor);
	DECLARE_Proxy(VSSceneMap);
	DECLARE_Ptr(VSRenderTarget);
	DECLARE_Ptr(VSDepthStencil);
	DECLARE_Ptr(VSBlendState);
	DECLARE_Ptr(VSDepthStencilState);
	DECLARE_Ptr(VSRasterizerState);
	DECLARE_Ptr(VSSamplerState);
	DECLARE_Ptr(VSDynamicBufferGeometry);
	class VSBlendDesc;
	class VSDepthStencilDesc;
	class VSRasterizerDesc;
	class VSSamplerDesc;
	class VSGCTask;
	struct DynamicBufferIndex
	{
		VSVertexFormat * pVertexFormat;
		unsigned int uiMeshDataType;
		DynamicBufferIndex()
		{
			pVertexFormat = NULL;
			uiMeshDataType = 0;
		}	
	};
	bool operator ==(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2);
	bool operator >(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2);
	bool operator <(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2);
#ifdef DELEGATE_PREFERRED_SYNTAX
	typedef VSDelegateEvent<void(void)> DelayUpdatObjectOneFrame;
	typedef VSDelegateEvent<void(VSREAL)> DelayUpdatObject;
#else
	typedef VSDelegateEvent0<void> DelayUpdatObjectOneFrame;
	typedef VSDelegateEvent1<void,VSREAL> DelayUpdatObject;
#endif
	class VSGRAPHIC_API VSResourceManager 
	{
		//PRIORITY
		DECLARE_PRIORITY
	public:
		VSResourceManager();
		~VSResourceManager();
	
#define GET_INNER_RESOUCE_SET(ResouceName)\
		static VSResourceSet<unsigned int, VS##ResouceName##Ptr> & Get##ResouceName##Set()\
		{\
			static VSResourceSet<unsigned int, VS##ResouceName##Ptr> s_##ResouceName##Set; \
			return s_##ResouceName##Set; \
		}

		//内部管理resource
		GET_INNER_RESOUCE_SET(VertexFormat);
		GET_INNER_RESOUCE_SET(Name);
		GET_INNER_RESOUCE_SET(BlendState);
		GET_INNER_RESOUCE_SET(DepthStencilState);
		GET_INNER_RESOUCE_SET(RasterizerState);
		GET_INNER_RESOUCE_SET(SamplerState);

#define GET_SHADER_MAP(ShaderMapName)\
		static VSShaderMap & Get##ShaderMapName()\
		{\
			static VSShaderMap s_##ShaderMapName(_T(#ShaderMapName)); \
			return s_##ShaderMapName; \
		}
		GET_SHADER_MAP(MaterialShaderMap);
		GET_SHADER_MAP(IndirectShaderMap);
		GET_SHADER_MAP(VertexShaderMap);
		GET_SHADER_MAP(PixelShaderMap);
		GET_SHADER_MAP(GeometryShaderMap);
		GET_SHADER_MAP(NormalDepthShaderMap);
		GET_SHADER_MAP(CubShadowShaderMap);
		GET_SHADER_MAP(VolumeShadowShaderMap);
		GET_SHADER_MAP(ShadowShaderMap);
		GET_SHADER_MAP(DualParaboloidShadowShaderMap);
		GET_SHADER_MAP(InnerVertexShaderMap);
		GET_SHADER_MAP(InnerPixelShaderMap);
		GET_SHADER_MAP(InnerGeometryShaderMap);
		static VSDVGeometry *GetDVGeometry(VSVertexFormat * pVertexFormat, unsigned int MeshDataType, unsigned int VertexNum);

		static void DeleteDynamicBufferGeometry(VSDynamicBufferGeometry * pDBG);

		static void ClearDynamicBufferGeometry();

		static void InitCacheShader(unsigned int RenderTypeAPI);
		static void LoadDefaultResource(unsigned int RenderTypeAPI);
		static const TCHAR* GetRenderTypeShaderPath(unsigned int RenderTypeAPI);

		static bool IsReleaseAll();
		static void CacheResource();
		static void GC();
		template <class T>
		static VSTexAllState * Create2DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer);

		template <class T>
		static VSTexAllState * Create1DTexture(unsigned int uiWidth,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer);

		template <class T>
		static VSTexAllState * CreateCubTexture(unsigned int uiWidth,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer);

		template <class T>
		static VSTexAllState * Create3DTexture(unsigned int uiWidth,unsigned int uiHeight, unsigned int uiLength,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer);

		static VSCaptureTexAllState * CreateCaptureTexture(const VSString & ViewFamilyName, unsigned int uiMipLevel = 1);

		static VSTexAllState * Load2DTexture(const TCHAR *pFileName,VSSamplerStatePtr pSamplerState = NULL,bool bSRGB = false);
		//0 no compress 1 dx3  2 dx5
		static VSTexAllState * Load2DTextureCompress(const TCHAR *pFileName,VSSamplerStatePtr pSamplerState = NULL,
			unsigned int uiCompressType = 0,bool bIsNormal = false,bool bSRGB = false);







		static VSTexAllState * NewLoadTexture(const TCHAR *pFileName);
		static bool NewSaveTexture(VSTexAllState * pTexture, const TCHAR * PathName, bool bSaveResourcePath = false);

		static VSMaterial * NewLoadMaterial(const TCHAR *pFileName);
		static bool NewSaveMaterial(VSMaterial * pMaterial, const TCHAR * PathName, bool bSaveResourcePath = false);


		static VSStaticMeshNode * NewLoadStaticMesh(const TCHAR *pFileName);
		static bool NewSaveStaticMesh(VSStaticMeshNode * pStaticMeshNode, const TCHAR * PathName, bool bSaveResourcePath = false);

		static VSSkelectonMeshNode * NewLoadSkelectonMesh(const TCHAR *pFileName);
		static bool NewSaveSkelectonMeshNode(VSSkelectonMeshNode * pSkelectonMesh, const TCHAR * PathName, bool bSaveResourcePath = false);

		static VSPostEffectSet * NewLoadPostEffect(const TCHAR *pFileName);
		static bool NewSavePostEffect(VSPostEffectSet * pPostEffectSet, const TCHAR * PathName, bool bSaveResourcePath = false);


		static bool NewSaveAction(VSAnim * pAnim, const TCHAR * PathName, bool bSaveResourcePath = false);
		static VSAnim * NewLoadAction(const TCHAR *pFileName);


		static bool NewSaveAnimTree(VSAnimTree * pAnim, const TCHAR * PathName, bool bSaveResourcePath = false);
		static VSAnimTree * NewLoadAnimTree(const TCHAR *pFileName);

		static bool NewSaveMorphTree(VSMorphTree * pMorphTree, const TCHAR * PathName, bool bSaveResourcePath = false);
		static VSMorphTree * NewLoadMorphTree(const TCHAR *pFileName);

		static bool NewSaveActor(VSActor * pActor, const TCHAR * PathName, bool bSaveResourcePath = false);
		static VSActor * NewLoadActor(const TCHAR *pFileName);

		static bool NewSaveMap(VSSceneMap * pFont, const TCHAR * PathName, bool bSaveResourcePath = false);
		static VSSceneMap * NewLoadMap(const TCHAR *pFileName);

		static bool NewSaveFont(VSFont * pFont, const TCHAR * PathName, bool bSaveResourcePath = false);
		static VSFont * NewLoadFont(const TCHAR *pFileName);

		static VSVertexFormat *LoadVertexFormat(VSVertexBuffer * pVertexBuffer,VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> *pFormatArray = NULL);
		static VSName * CreateName(const TCHAR * pChar);
		static VSName * CreateName(const VSString & String);
		static VSVShader * CreateVShader(MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID);
		static VSPShader * CreatePShader(MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID);
		static VSGShader * CreateGShader(MaterialShaderPara &MSPara, unsigned int uiPassType, unsigned int uiShaderID);
		static VSBlendState * CreateBlendState(const VSBlendDesc & BlendDesc);
		static VSDepthStencilState * CreateDepthStencilState(const VSDepthStencilDesc & DepthStencilDesc);
		static VSRasterizerState * CreateRasterizerState(const VSRasterizerDesc &RasterizerDesc);
		static VSSamplerState * CreateSamplerState(const VSSamplerDesc &SamplerDesc);

		
		//asynchronization

		static VSProxyResourceSet<VSUsedName ,VSTexAllStateRPtr> & GetASYNTextureSet()
		{
			static VSProxyResourceSet<VSUsedName ,VSTexAllStateRPtr> s_ASYNTextureSet;
			return s_ASYNTextureSet;
		}
		static VSTexAllStateRPtr ms_DefaultTextureResource;
		static VSProxyResourceSet<VSUsedName ,VSMaterialRPtr> & GetASYNMaterialSet()
		{
			static VSProxyResourceSet<VSUsedName ,VSMaterialRPtr> s_ASYNMaterialSet;
			return s_ASYNMaterialSet;
		}
		static VSMaterialRPtr ms_DefaultMaterialResource;
		static VSMaterialRPtr ms_DefaultOnlyColorMaterialResource;
		static VSStaticMeshNodeRPtr ms_DefaultStaticMeshNodeResource;
		static VSSkelectonMeshNodeRPtr ms_DefaultSkelectonMeshNodeResource;
		static VSProxyResourceSet<VSUsedName ,VSAnimRPtr> & GetASYNAnimSet()
		{
			static VSProxyResourceSet<VSUsedName ,VSAnimRPtr> s_ASYNAnimSet;
			return s_ASYNAnimSet;
		}

		static VSProxyResourceSet<VSUsedName ,VSStaticMeshNodeRPtr> & GetASYNStaticMeshSet()
		{
			static VSProxyResourceSet<VSUsedName ,VSStaticMeshNodeRPtr> s_ASYNStaticMeshSet;
			return s_ASYNStaticMeshSet;
		}
		
		static VSProxyResourceSet<VSUsedName ,VSSkelectonMeshNodeRPtr> & GetASYNSkelectonMeshSet()
		{
			static VSProxyResourceSet<VSUsedName ,VSSkelectonMeshNodeRPtr> s_ASYNSkelectonMeshSet;
			return s_ASYNSkelectonMeshSet;
		}
		static VSProxyResourceSet<VSUsedName ,VSPostEffectSetRPtr> & GetASYNPostEffectSet()
		{
			static VSProxyResourceSet<VSUsedName ,VSPostEffectSetRPtr> s_ASYNPostEffectSet;
			return s_ASYNPostEffectSet;
		}
		static VSProxyResourceSet<VSUsedName ,VSAnimTreeRPtr> & GetASYNAnimTreeSet()
		{
			static VSProxyResourceSet<VSUsedName ,VSAnimTreeRPtr> s_ASYNAnimTreeSet;
			return s_ASYNAnimTreeSet;
		}
		static VSProxyResourceSet<VSUsedName ,VSMorphTreeRPtr> & GetASYNMorphTreeSet()
		{
			static VSProxyResourceSet<VSUsedName ,VSMorphTreeRPtr> s_ASYNMorphTreeSet;
			return s_ASYNMorphTreeSet;
		}
		static VSProxyResourceSet<VSUsedName, VSActorRPtr> & GetASYNActorSet()
		{
			static VSProxyResourceSet<VSUsedName, VSActorRPtr> s_ASYNActorSet;
			return s_ASYNActorSet;
		}
		static VSProxyResourceSet<VSUsedName, VSSceneMapRPtr> & GetASYNMapSet()
		{
			static VSProxyResourceSet<VSUsedName, VSSceneMapRPtr> s_ASYNMapSet;
			return s_ASYNMapSet;
		}
		static VSProxyResourceSet<VSUsedName, VSFontRPtr> & GetASYNFontSet()
		{
			static VSProxyResourceSet<VSUsedName, VSFontRPtr> s_ASYNFontSet;
			return s_ASYNFontSet;
		}
		static VSTexAllStateR * LoadASYN2DTexture(const TCHAR *pFileName,bool IsAsyn,VSSamplerStatePtr pSamplerState = NULL,bool bSRGB = false);
		//0 no compress 1 dx3  2 dx5
		static VSTexAllStateR * LoadASYN2DTextureCompress(const TCHAR *pFileName,bool IsAsyn,VSSamplerStatePtr pSamplerState = NULL,
			unsigned int uiCompressType = 0,bool bIsNormal = false,bool bSRGB = false);

		static VSTexAllStateR * LoadASYNTexture(const TCHAR *pFileName,bool IsAsyn);
		static VSMaterialR * LoadASYNMaterial(const TCHAR *pFileName,bool IsAsyn);

		static VSStaticMeshNodeR * LoadASYNStaticMesh(const TCHAR *pFileName,bool IsAsyn);
		static VSSkelectonMeshNodeR * LoadASYNSkelectonMesh(const TCHAR *pFileName,bool IsAsyn);
		static VSPostEffectSetR * LoadASYNPostEffect(const TCHAR *pFileName,bool IsAsyn);
		static VSAnimR * LoadASYNAction(const TCHAR *pFileName,bool IsAsyn);


		static VSAnimTreeR * LoadASYNAnimTree(const TCHAR *pFileName,bool IsAsyn);
		static VSMorphTreeR * LoadASYNMorphTree(const TCHAR *pFileName,bool IsAsyn);
		static VSActorR * LoadASYNActor(const TCHAR *pFileName, bool IsAsyn);

		static VSSceneMapR * LoadASYNMap(const TCHAR *pFileName, bool IsAsyn);

		static VSFontR * LoadASYNFont(const TCHAR *pFileName, bool IsAsyn);

		static VSResourceProxyBase * LoadResource(const TCHAR *pFileName,bool IsAsyn);

		static void DeleteMapResource(const TCHAR *  pFileName);
		static void DeleteAllMapResource();
		static VSRenderTarget * CreateRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
			unsigned int uiMulSample);
		static VSRenderTarget * CreateRenderTarget(VSTexture * pCreateBy,unsigned int uiMulSample = 0
			,unsigned int uiLevel = 0,unsigned int uiFace = 0);
		static VSDepthStencil * CreateDepthStencil(unsigned int uiWidth,unsigned int uiHeight,
			unsigned int uiMulSample,unsigned int uiFormatType);

		//取得满足条件的，平且没有再使用的RT，为了节省显存空间，一般这种RT和texture绑定的，并且贴图miplevel都是1
		static VSRenderTarget * Get2DRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,unsigned int uiMulSample);

		static void Release2DRenderTarget(VSRenderTarget * pRenderTarget);

		static void Disable2DRenderTarget(VSRenderTarget * &pRenderTarget);

		static void Disable2DRenderTarget(VSRenderTargetPtr &pRenderTarget);

		static bool GetCubRenderTarget(unsigned int uiWidth, unsigned int uiFormatType,unsigned int uiMulSample,VSRenderTarget * OutRT[VSCubeTexture::F_MAX]);
		
		static void ReleaseCubRenderTarget(VSRenderTarget * RT[VSCubeTexture::F_MAX]);

		static void DisableCubRenderTarget(VSRenderTarget * RT[VSCubeTexture::F_MAX]);

		static VSDepthStencil * GetDepthStencil(unsigned int uiWidth,unsigned int uiHeight,unsigned int uiFormatType,unsigned int uiMulSample);

		static void ReleaseDepthStencil(VSDepthStencil * pDepthStencil);

		static void DisableDepthStencil(VSDepthStencil * &pDepthStencil);
		static void DisableDepthStencil(VSDepthStencilPtr &pDepthStencil);

		//Source - Target
		static VSAnim * CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR);

		static VSAnim * CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR, VSREAL fTargetTime = 0.0f);

		static VSAnim * CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR, VSREAL fSourceTime = 0.0f, VSREAL fTargetTime = 0.0f);

		static VSAnim * CreateAnim(VSAnim * pSourceAnim, VSREAL fSourceTime = 0.0f);
		DECLARE_INITIAL_ONLY
	private:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

		static bool CacheName();
		static bool CacheShader();
		static bool CacheVertexFormat();



		static void InitConfig();

		static VSResourceSet<DynamicBufferIndex, VSDVGeometryPtr> &GetDVGeometryArray()
		{
			static VSResourceSet<DynamicBufferIndex, VSDVGeometryPtr> s_DVGeometryArray;
			return s_DVGeometryArray;
		}
		static VSArray<VSDynamicBufferGeometryPtr> ms_SaveDelete;

		static VSResourceArrayControll<VSRenderTargetPtr> &GetRenderTargetArray()
		{
			static VSResourceArrayControll<VSRenderTargetPtr> s_RenderTargetArray;
			return s_RenderTargetArray;
		}
		static VSResourceArrayControll<VSDepthStencilPtr> &GetDepthStencilArray()
		{
			static VSResourceArrayControll<VSDepthStencilPtr> s_RenderTargetArray;
			return s_RenderTargetArray;
		}

		static VSResourceArrayControll<VSRenderTargetPtr> &GetRenderTargetBufferArray()
		{
			static VSResourceArrayControll<VSRenderTargetPtr> s_RenderTargetBufferArray;
			return s_RenderTargetBufferArray;
		}
		static VSResourceArrayControll<VSDepthStencilPtr> &GetDepthStencilBufferArray()
		{
			static VSResourceArrayControll<VSDepthStencilPtr> s_RenderTargetBufferArray;
			return s_RenderTargetBufferArray;
		}
		static bool IsResourceType(VSRtti &Rtti)
		{
			for (unsigned int i = 0; i < ms_ResourceRtti.GetNum();i++)
			{
				if (Rtti.IsDerived(*ms_ResourceRtti[i]))
				{
					return true;
				}
			}
			return false;
		}
	protected:
		static VSCriticalSection ms_TextureCri;
		static VSCriticalSection ms_MaterialCri;
		static VSCriticalSection ms_AnimCri;
		static VSCriticalSection ms_StaticMeshCri;
		static VSCriticalSection ms_SkeletonMeshCri;
		static VSCriticalSection ms_VShaderCri;
		static VSCriticalSection ms_PShaderCri;
		static VSCriticalSection ms_GShaderCri;
		static VSCriticalSection ms_PostEffectCri;
		static VSCriticalSection ms_NameCri;
		static VSCriticalSection ms_AnimTreeCri;
		static VSCriticalSection ms_MorphTreeCri;
		static VSCriticalSection ms_ActorCri;
		static VSCriticalSection ms_MapCri;
		static VSCriticalSection ms_FontCri;
		static VSArray<VSRtti *> ms_ResourceRtti;
	public:
		//help function
		template <class T>
		static bool GetNextMipDate(const T * pInDate,unsigned int uiInDateWidth,unsigned int uiInDateHeight,
			T * pOutDate,unsigned int uiChannel);
		template <class T>
		static bool GetNextMipDate(const T * pInDate,unsigned int uiInDateWidth,unsigned int uiInDateHeight,unsigned int uiInDateLenght,
			T * pOutDate,unsigned int uiChannel);
	public:
		static unsigned int GetGpuSkinBoneNum()
		{
			return ms_uiGpuSkinBoneNum;
		}
		static bool DrawSkelecton()
		{
			return ms_bDrawSkelecton;
		}
		static bool GetActiveDebugDraw()
		{
			return ms_bActiveDebugDraw;
		}

		static bool IsCacheShader()
		{
			return ms_bIsCacheShader;
		}
		static bool IsCacheName()
		{
			return ms_bIsCacheName;
		}
		static bool IsCacheVertexFormat()
		{
			return ms_bIsCacheVertexFormat;

		}

		static bool ms_bRenderThread;
		static bool ms_bUpdateThread;

		static unsigned int ms_CurRenderAPIType;
	protected:
		static unsigned int ms_uiGpuSkinBoneNum;
		static bool ms_bDrawSkelecton;
		static bool ms_bActiveDebugDraw;
		


		static bool ms_bIsCacheShader;
		static bool ms_bIsCacheName;
		static bool ms_bIsCacheVertexFormat;
public:
		static VSString ms_ActorPath;
		static VSString ms_MapPath;
		static VSString ms_FontPath;
		static VSString ms_TexturePath;
		static VSString ms_AnimPath;
		static VSString	ms_MaterialPath;
		static VSString	ms_PostEffectPath;
		static VSString ms_SkelectionMeshPath;
		static VSString ms_MorphPath;
		static VSString ms_OutputShaderCodePath;
		static VSString ms_OutPutLogPath;
		static VSString ms_ShaderPath;
		static VSString ms_StaticMeshPath;
		static VSString ms_TerrainPath;
		static VSString ms_ResourcePath;
		static bool ms_EnableAsynLoad;
protected:
		static VSArray<VSObject *> ms_pRootObject;
		static VSArrayOrder<VSObject *> ms_pGCObject;
public:
	/*
		GC规则：
		默认从VS_NEW创建的OBJECT 不参与GC，所以不会被GC掉，要自己管理。
		只有从Object GetInstance 创建出来的才参与GC，如果想要不参与GC请调用AddRootObject。
		把指针加入属性表，保证要参与GC的Object可以访问的到，否则就会被GC掉。
	
	*/
		static DelayUpdatObjectOneFrame & GetDelayUpdateObjectOneFrame()
		{
			static DelayUpdatObjectOneFrame s_DelayUpdateObjectOneFrame;
			return s_DelayUpdateObjectOneFrame;
		}
		static DelayUpdatObject & GetDelayUpdateObject()
		{
			static DelayUpdatObject s_DelayUpdateObject;
			return s_DelayUpdateObject;
		}
		static void DelayUpdate(VSREAL fTime)
		{
			GetDelayUpdateObjectOneFrame()();
			GetDelayUpdateObjectOneFrame().Reset();
			GetDelayUpdateObject()(fTime);
		}
		static void AddRootObject(VSObject * p)
		{
			p->SetFlag(VSObject::OF_RootObject);
			ms_pRootObject.AddElement(p);
		}
		static void DeleteRootObject(VSObject * p)
		{
			for (unsigned int i = 0; i < ms_pRootObject.GetNum(); i++)
			{
				if (ms_pRootObject[i] == p)
				{
					p->ClearFlag(VSObject::OF_RootObject);
					ms_pRootObject.Erase(i);
					break;
				}
			}
		}
		static void AddGCObject(VSObject * p)
		{
			ms_pGCObject.AddElement(p);
			p->SetFlag(VSObject::OF_GCObject);
		}

		static void GCObject();
		
		protected:
			friend class VSStream;
			static VSGCTask * ms_pCurGCTask;
			static VSGCTask * ms_pEndGCTask;
			static void RunGCTask();
			static void AddCanGCObject(VSArray<VSObject *>& CanGCObject);
		public:
			static void RunAllGCTask();

};
	template <class T>
	bool VSResourceManager::GetNextMipDate(const T * pInDate,unsigned int uiInDateWidth,unsigned int uiInDateHeight,
		T * pOutDate , unsigned int uiChannel)
	{
		VSMAC_ASSERT(pInDate && pOutDate);
		if (!pInDate || !pOutDate)
		{
			return false;
		}

		unsigned int uiOutDateWidth = uiInDateWidth >> 1;
		if (!uiOutDateWidth)
		{
			uiOutDateWidth = 1;
		}
		unsigned int uiOutDateHeight = uiInDateHeight >> 1;
		if (!uiOutDateHeight)
		{
			uiOutDateHeight = 1;
		}
		for (unsigned int i = 0 ;i < uiOutDateWidth ; i++)
		{
			unsigned int uiRow0 = i * 2;
			unsigned int uiRow1 = i * 2 + 1;

			if (uiRow1 >= uiInDateWidth)
			{
				uiRow1 = uiInDateWidth - 1;
			}
			for (unsigned int j = 0 ; j < uiOutDateHeight ; j++)
			{

				unsigned int uiCol0 = j * 2;
				unsigned int uiCol1 = j * 2 + 1;
				if (uiCol1 >= uiInDateHeight)
				{
					uiCol1 = uiInDateHeight - 1;
				}

				for (unsigned int k = 0 ; k < uiChannel ; k++)
				{
					pOutDate[(j + i * uiOutDateHeight) * uiChannel + k] 
					= (T)((pInDate[(uiCol0 + uiRow0 * uiInDateHeight) * uiChannel + k] + pInDate[(uiCol1 + uiRow0 * uiInDateHeight) * uiChannel + k]
					+ pInDate[(uiCol0 + uiRow1 * uiInDateHeight) * uiChannel + k] + pInDate[(uiCol1 + uiRow1 * uiInDateHeight) * uiChannel + k]) * 0.25f);
				}

			}
		}
		return true;
	}
	template <class T>
	bool VSResourceManager::GetNextMipDate(const T * pInDate,unsigned int uiInDateWidth,unsigned int uiInDateHeight,unsigned int uiInDateLength,
		T * pOutDate , unsigned int uiChannel)
	{
		VSMAC_ASSERT(pInDate && pOutDate);
		if (!pInDate || !pOutDate)
		{
			return false;
		}

		unsigned int uiOutDateWidth = uiInDateWidth >> 1;
		if (!uiOutDateWidth)
		{
			uiOutDateWidth = 1;
		}
		unsigned int uiOutDateHeight = uiInDateHeight >> 1;
		if (!uiOutDateHeight)
		{
			uiOutDateHeight = 1;
		}
		unsigned int uiOutDateLength = uiInDateLength >> 1;
		if (!uiOutDateLength)
		{
			uiOutDateLength = 1;
		}
		for (unsigned int s = 0 ; s < uiOutDateLength ; s++)
		{
			unsigned int uiL0 = s * 2;
			unsigned int uiL1 = s * 2 + 1;

			if (uiL1 >= uiOutDateLength)
			{
				uiL1 = uiOutDateLength - 1;
			}

			for (unsigned int i = 0 ;i < uiOutDateWidth ; i++)
			{
				unsigned int uiRow0 = i * 2;
				unsigned int uiRow1 = i * 2 + 1;

				if (uiRow1 >= uiInDateWidth)
				{
					uiRow1 = uiInDateWidth - 1;
				}
				for (unsigned int j = 0 ; j < uiOutDateHeight ; j++)
				{

					unsigned int uiCol0 = j * 2;
					unsigned int uiCol1 = j * 2 + 1;
					if (uiCol1 >= uiInDateHeight)
					{
						uiCol1 = uiInDateHeight - 1;
					}

					for (unsigned int k = 0 ; k < uiChannel ; k++)
					{
						pOutDate[(j + i * uiOutDateHeight + s * uiOutDateHeight * uiOutDateWidth) * uiChannel + k] 
						= (T)((pInDate[(uiCol0 + uiRow0 * uiInDateHeight + uiL0 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol1 + uiRow0 * uiInDateHeight + uiL0 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol0 + uiRow1 * uiInDateHeight + uiL0 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol1 + uiRow1 * uiInDateHeight + uiL0 * uiInDateHeight * uiInDateWidth) * uiChannel + k] +
							pInDate[(uiCol0 + uiRow0 * uiInDateHeight + uiL1 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol1 + uiRow0 * uiInDateHeight + uiL1 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol0 + uiRow1 * uiInDateHeight + uiL1 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol1 + uiRow1 * uiInDateHeight + uiL1 * uiInDateHeight * uiInDateWidth) * uiChannel + k] +
							) * 0.125f);
					}

				}
			}
		}
		
		return true;
	}
	template <class T>
	VSTexAllState * VSResourceManager::Create2DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer)
	{
		if (!uiWidth || !uiHeight)
		{
			return NULL;
		}
		if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight))
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		VSTexAllState * pTexAllState = NULL;
		VS2DTexture * pTexture = NULL;
		pTexture = VS_NEW VS2DTexture(uiWidth,uiHeight,uiFormatType,uiMipLevel,1);
		if(!pTexture)
		{
			return NULL;
		}
		pTexture->CreateRAMDate();
		if (pBuffer)
		{
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			VSMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			T * pLast = (T *)pTexture->GetBuffer(0);
			for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
			{
				T *pNow = (T *)pTexture->GetBuffer(i);

				if(!VSResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
				{
					VSMAC_DELETE(pTexture);
					return NULL;
				}
				pLast = (T *)pTexture->GetBuffer(i);
			}
		}
		

		pTexAllState = VS_NEW VSTexAllState(pTexture);

		return pTexAllState;
	}
	
	template <class T>
	VSTexAllState * VSResourceManager::Create1DTexture(unsigned int uiWidth,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer)
	{
		if (!uiWidth)
		{
			return NULL;
		}
		if (!IsTwoPower(uiWidth))
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		VSTexAllState * pTexAllState = NULL;
		VS1DTexture * pTexture = NULL;
		pTexture = VS_NEW VS1DTexture(uiWidth,uiFormatType,uiMipLevel,1);
		if(!pTexture)
		{
			return NULL;
		}
		pTexture->CreateRAMDate();
		if (pBuffer)
		{
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			VSMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			T * pLast = (T *)pTexture->GetBuffer(0);
			for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
			{
				T *pNow = (T *)pTexture->GetBuffer(i);

				if(!VSResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
				{
					VSMAC_DELETE(pTexture);
					return NULL;
				}
				pLast = (T *)pTexture->GetBuffer(i);
			}
		}

		pTexAllState = VS_NEW VSTexAllState(pTexture);

		return pTexAllState;
	}
	template <class T>
	VSTexAllState * VSResourceManager::CreateCubTexture(unsigned int uiWidth,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer)
	{
		if (!uiWidth)
		{
			return NULL;
		}
		if (!IsTwoPower(uiWidth))
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		VSTexAllState * pTexAllState = NULL;
		VSCubeTexture * pTexture = NULL;
		pTexture = VS_NEW VSCubeTexture(uiWidth,uiFormatType,uiMipLevel,1);
		if(!pTexture)
		{
			return NULL;
		}
		pTexture->CreateRAMDate();
		if (pBuffer)
		{
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			VSMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			for (unsigned int s = 0 ; s < VSCubeTexture::F_MAX ; s++)
			{
				T * pLast = (T *)pTexture->GetFaceBuffer(0,s);
				for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
				{
					T *pNow = (T *)pTexture->GetFaceBuffer(i,s);

					if(!VSResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
					{
						VSMAC_DELETE(pTexture);
						return NULL;
					}
					pLast = (T *)pTexture->GetFaceBuffer(i,s);
				}
			}
		}

		pTexAllState = VS_NEW VSTexAllState(pTexture);

		return pTexAllState;
	}
	template <class T>
	VSTexAllState * VSResourceManager::Create3DTexture(unsigned int uiWidth,unsigned int uiHeight, unsigned int uiLength,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer)
	{
		if (!uiWidth || !uiHeight || !uiLength)
		{
			return NULL;
		}
		if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight) || !IsTwoPower(uiLength))
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		VSTexAllState * pTexAllState = NULL;
		VS3DTexture * pTexture = NULL;
		pTexture = VS_NEW VS3DTexture(uiWidth,uiHeight,uiLength,uiFormatType,uiMipLevel,1);
		if(!pTexture)
		{
			return NULL;
		}
		pTexture->CreateRAMDate();
		if (pBuffer)
		{
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			VSMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			T * pLast = (T *)pTexture->GetBuffer(0);
			for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
			{
				T *pNow = (T *)pTexture->GetBuffer(i);

				if(!VSResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pTexture->GetLength(i - 1),pNow,pTexture->GetChannelPerPixel()))
				{
					VSMAC_DELETE(pTexture);
					return NULL;
				}
				pLast = (T *)pTexture->GetBuffer(i);
			}
		}

		pTexAllState = VS_NEW VSTexAllState(pTexture);

		return pTexAllState;
	}
}
#endif