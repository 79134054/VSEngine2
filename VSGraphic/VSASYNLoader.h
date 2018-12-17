#ifndef VSASYNLOADER_H
#define VSASYNLOADER_H
#include "VSThread.h"
#include "VSSafeQueue.h"
#include "VSResourceManager.h"
namespace VSEngine2
{

	class VSGRAPHIC_API VSResourceJob : public VSMemObject
	{
	public:
		enum //Load state
		{
			LS_PENDING,
			LS_LOADED,
			LS_FAIL,
		};
		enum // JOB TYPE
		{
			JT_TEXTURE,
			JT_MATERIAL,
			JT_ANIM,
			JT_STATIC_MESH,
			JT_SKELECTON_MESH,
			JT_ANIMTREE,
			JT_MORPHTREE,
			JT_ACTOR,
			JT_MAP,
			JT_FONT,
		};
		enum // JOB STATE
		{
			JS_NONE,
			JS_DELETE,

		};
		VSResourceJob();
		virtual ~VSResourceJob() = 0;
		unsigned int m_uiLoadState;
		unsigned int m_uiJobType;
		unsigned int m_uiJobState;
		virtual bool Process() = 0;
		virtual bool Load() = 0;
	
	};
	class VSGRAPHIC_API VSFileRJob :public VSResourceJob
	{
	public:
		VSFileRJob();
		virtual ~VSFileRJob() = 0;
		virtual bool Process() = 0;
		virtual bool Load();
		VSFileName  m_FileName;
	protected:
		unsigned char * m_pBuffer;
		unsigned int m_uiSize;
	};
	class VSGRAPHIC_API VSTextureRJob : public VSFileRJob
	{
	public:
		VSTextureRJob(VSTexAllStateRPtr& pProxy);
		virtual ~VSTextureRJob();
		VSTexAllStateRPtr m_pProxy;
		bool m_bEngineType;
		bool m_bCompress;
		unsigned int m_uiCompressType;
		bool m_bIsNormal;
		bool m_bSRGB;
		VSSamplerStatePtr	m_pSamplerState;
		
	
		virtual bool Process();
	protected:
		VSTexAllState * LoadTexture();
		VSTexAllState * Load2DTexture();
		VSTexAllState * Load2DTextureCompress();

	};
	class VSGRAPHIC_API VSMaterialRJob : public VSFileRJob
	{
	public:
		VSMaterialRJob(VSMaterialRPtr& pProxy);
		virtual ~VSMaterialRJob();
		VSMaterialRPtr m_pProxy;
		virtual bool Process();
	}; 
	class VSGRAPHIC_API VSAnimRJob : public VSFileRJob
	{
	public:
		VSAnimRJob(VSAnimRPtr& pProxy);
		virtual ~VSAnimRJob();
		VSAnimRPtr m_pProxy;	
		virtual bool Process();
	}; 
	class VSGRAPHIC_API VSStaticMeshRJob : public VSFileRJob
	{
	public:
		VSStaticMeshRJob(VSStaticMeshNodeRPtr& pProxy);
		virtual ~VSStaticMeshRJob();
		VSStaticMeshNodeRPtr m_pProxy;

		virtual bool Process();
	}; 
	class VSGRAPHIC_API VSSkelectonMeshRJob : public VSFileRJob
	{
	public:
		VSSkelectonMeshRJob(VSSkelectonMeshNodeRPtr& pProxy);
		virtual ~VSSkelectonMeshRJob();
		VSSkelectonMeshNodeRPtr m_pProxy;
		virtual bool Process();
	}; 
	class VSGRAPHIC_API VSAnimTreeRJob : public VSFileRJob
	{
	public:
		VSAnimTreeRJob(VSAnimTreeRPtr& pProxy);
		virtual ~VSAnimTreeRJob();
		VSAnimTreeRPtr m_pProxy;
		virtual bool Process();
	}; 
	class VSGRAPHIC_API VSMorphTreeRJob : public VSFileRJob
	{
	public:
		VSMorphTreeRJob(VSMorphTreeRPtr& pProxy);
		virtual ~VSMorphTreeRJob();
		VSMorphTreeRPtr m_pProxy;
		virtual bool Process();
	}; 
	class VSGRAPHIC_API VSActorRJob : public VSFileRJob
	{
	public:
		VSActorRJob(VSActorRPtr& pProxy);
		virtual ~VSActorRJob();
		VSActorRPtr m_pProxy;
		virtual bool Process();
	};
	class VSGRAPHIC_API VSSceneMapRJob : public VSFileRJob
	{
	public:
		VSSceneMapRJob(VSSceneMapRPtr& pProxy);
		virtual ~VSSceneMapRJob();
		VSSceneMapRPtr m_pProxy;
		virtual bool Process();
	};
	class VSGRAPHIC_API VSFontRJob : public VSFileRJob
	{
	public:
		VSFontRJob(VSFontRPtr& pProxy);
		virtual ~VSFontRJob();
		VSFontRPtr m_pProxy;
		virtual bool Process();
	};
	class VSGRAPHIC_API VSResourceLoaderThread : public VSThread
	{
	public:
		VSResourceLoaderThread();
		virtual ~VSResourceLoaderThread();
		void AddJob(VSResourceJob* m_pJob);
		void Stop();
	protected:
		virtual const TCHAR* GetThreadName()
		{
			return _T("VSResourceLoaderThread");
		}
		VSSafeQueue<VSResourceJob*> m_pResourceQueue;
		virtual void Run();	

	};
	
	class VSGRAPHIC_API VSASYNLoadManager
	{
	public:
		VSASYNLoadManager();
		~VSASYNLoadManager();
		void Update(double AppTime);
		void AddTextureLoad(VSTexAllStateRPtr& pTexture,VSFileName & FileName,
							bool bEngineType = true,
							VSSamplerStatePtr	pSamplerState = NULL,					
							bool bCompress = false,
							unsigned int uiCompressType = 0,
							bool bIsNormal = false,
							bool bSRGB = false);
		void AddMaterial(VSMaterialRPtr& pMaterial,VSFileName & FileName);
		void AddAnim(VSAnimRPtr& pAnim,VSFileName & FileName);
		void AddStaticMesh(VSStaticMeshNodeRPtr& pStaticMesh,VSFileName & FileName);
		void AddSkelectonMesh(VSSkelectonMeshNodeRPtr& pSkelectonMesh,VSFileName & FileName);
		void AddAnimTree(VSAnimTreeRPtr& pAnimTree,VSFileName & FileName);
		void AddMorphTree(VSMorphTreeRPtr& pMorphTree,VSFileName & FileName);
		void AddActor(VSActorRPtr& pActor, VSFileName & FileName);
		void AddMap(VSSceneMapRPtr& pMap, VSFileName & FileName);
		void AddFont(VSFontRPtr& pMap, VSFileName & FileName);
		void DeleteLoadResource(VSFileName & FileName);
		static VSASYNLoadManager * ms_pASYNLoadManager;
	protected:
		VSResourceLoaderThread m_ResourceLoadThread;
		VSArray<VSResourceJob * > m_ResourceJobArray;
		void AddJob(VSResourceJob* m_pJob);

	};
}
#endif