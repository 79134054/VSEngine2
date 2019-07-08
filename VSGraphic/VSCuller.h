#ifndef VSCULLER_H
#define	VSCULLER_H
#include "VSPlane3.h"
#include "VSObject.h"
#include "VSSphere3.h"
namespace VSEngine2
{
	/*
		列表里存储所有可见节点，每个节点存储自己叶子对应的叶子几点在可以见列表的下标，现在去掉了这个功能，、
		列表里面只存储叶子节点。
	*/
	class VSSpatial;
	class VSMaterialInstance;
	class VSMaterial;
	class VSMeshNode;
	class VSGeometry;
	class VSCamera;
	class VSLight;
	class VSRenderContext
	{
	public:
		VSRenderContext()
		{
			m_pGeometry = NULL;
			m_pMaterialInstance = NULL;
			m_uiPassId = 0;
			m_pMaterial = NULL;
			m_pMeshNode = NULL;
		}
		~VSRenderContext()
		{

		}
		static VSRenderContext ms_RenderContextNULL;
		VSGeometry * m_pGeometry;
		VSMaterialInstance * m_pMaterialInstance;
		VSMaterial * m_pMaterial;
		unsigned int m_uiPassId;
		VSMeshNode * m_pMeshNode;
		VSArray<VSLight *> m_pInDirectLight;
		VSArray<VSLight *> m_pDirectLight;
	};
	class VSGRAPHIC_API VSCuller : public VSMemObject
	{
	public:
		enum //CULLER TYPE
		{
			CUT_MAIN,
			CUT_SHADOW,
			CUT_MAX
		};
		VSCuller(int iUseMaterialIndex = -1);
		virtual ~VSCuller();
		enum
		{
			VS_MAX_PLANE_NUM = 32
		};
		enum //VisibleSet Type
		{
			VST_BASE,
			VST_ALPHATEST,
			VST_ALPHABLEND,	
			VST_COMBINE,
			VST_MAX
		};
		enum	//Render Group
		{
			RG_BACK,
			RG_NORMAL,
			RG_FRONT,		
			RG_MAX
		};
		enum	// VisibleSet Flag
		{
			VSF_NONE,
			VSF_ALL,
			VSF_PARTIAL,
			VSF_MAX
			
		};
		enum //Get Flag
		{
			GF_ALL = 0,
			GF_NO_CASTER_SHADOW = 1,
			GF_MAX

		};
		bool PushPlane(const VSPlane3 & Plane);
		bool PushCameraPlane(VSCamera &Camera);
		FORCEINLINE void SetCamera(VSCamera &Camera);
		bool PopPlane(VSPlane3 &Plane);
		FORCEINLINE void ClearAllPlane();

		bool InsertObject(VSRenderContext &VisibleContext,unsigned int uiVisibleSetType = VST_BASE,unsigned int uiRenderGroup = RG_NORMAL);
		FORCEINLINE unsigned int GetPlaneNum()const;
		FORCEINLINE void SetPlaneState(unsigned int uiPlaneState);
		FORCEINLINE unsigned int GetPlaneState()const;
		FORCEINLINE unsigned int GetVisibleNum(unsigned int uiVisibleSetType = VST_BASE,unsigned int uiRenderGroup = RG_NORMAL)const;
		FORCEINLINE unsigned int GetRenderGroupVisibleNum(unsigned int uiRenderGroup = RG_NORMAL)const;
		FORCEINLINE unsigned int GetAllVisibleNum()const;
		FORCEINLINE void ClearVisibleSet();
		FORCEINLINE void ClearPlaneState();

		FORCEINLINE unsigned int GetLightNum()const;
		FORCEINLINE VSLight * GetLight(unsigned int i)const;
		FORCEINLINE void ClearLight();
		void InsertLight(VSLight * pLight);
		bool HasLight(VSLight * pLight);
		virtual unsigned int IsVisible(const VSAABB3 &BV,bool bClearState = false);
		virtual unsigned int IsVisible(const VSSphere3 &S,bool bClearState = false);
		virtual unsigned int IsVisible(const VSVector3 & Point,bool bClearState = false);
		FORCEINLINE void SetUseMaterialIndex(int iUseMaterialIndex)
		{
			m_iUseMaterialIndex = iUseMaterialIndex;
		}

		FORCEINLINE int GetUseMaterialIndex()const
		{
			return m_iUseMaterialIndex;
		}
		virtual unsigned int GetCullerType()const
		{
			return CUT_MAIN;
		}
		virtual bool ForceNoCull(const VSSpatial * pSpatial);
		virtual bool CullConditionNode(const VSMeshNode * pMeshNode);
		FORCEINLINE VSRenderContext& GetVisibleSpatial(unsigned int i,unsigned int uiVisibleSetType = VST_BASE,unsigned int uiRenderGroup = RG_NORMAL)const;
		FORCEINLINE VSCamera * GetCamera()const;
		class RenderPriority
		{
		public:
			RenderPriority(){}
			~RenderPriority(){}
			bool operator()(VSRenderContext & p1,VSRenderContext & p2);


		};
		class AlphaPriority
		{
		public:
			AlphaPriority(VSCamera * pCamera){m_pCamera = pCamera;}
			~AlphaPriority(){}
			bool operator()(VSRenderContext & p1,VSRenderContext & p2);
		protected:
			VSCamera *m_pCamera;
		};
		friend class VSShadowCuller;
		friend class VSVolumeShadowMapCuller;
		virtual void Sort();


		virtual bool CullGeometry(VSGeometry * pGeometry);

		void GetAllVisibleAABB(unsigned int uiVisibleSetType, unsigned int uiRenderGroup, VSArray<VSAABB3> & AABBArray, unsigned int GetFlag = GF_ALL);

		void GetAndSortLight();
	protected:
		unsigned int m_uiPlaneNum;
		VSPlane3	m_Plane[VS_MAX_PLANE_NUM];
		unsigned int m_uiPlaneState;
		VSArray<VSRenderContext> m_VisibleSet[RG_MAX][VST_MAX];
		VSArray<VSLight *> m_LightSet;
		VSCamera *m_pCamera;
		int m_iUseMaterialIndex;		
	};
#include "VSCuller.inl"
	class VSLocalLight;
	class VSGRAPHIC_API VSShadowCuller : public VSCuller
	{
	public:
		// use light Camera
		VSShadowCuller(int iUseMaterialIndex = -1);
		virtual ~VSShadowCuller();
		virtual unsigned int GetCullerType()const
		{
			return CUT_SHADOW;
		}
		VSLocalLight * m_pLocalLight;
		virtual bool CullConditionNode(const VSMeshNode * pMeshNode);
		virtual void Sort();
	protected:
		bool TestSweptSphere(VSSphere3 & Sphere, VSVector3 & SweptDir);
		bool SweptSpherePlaneIntersect(float& t0, float& t1, VSPlane3 &Plane, VSSphere3 & Shpere, VSVector3 & SweptDir);
		bool TestSphere(VSSphere3 & Shpere) const;
	};
	class VSGRAPHIC_API VSVolumeShadowMapCuller : public VSShadowCuller
	{
	public://use Culler Camera to Cull
		VSVolumeShadowMapCuller(int iUseMaterialIndex = -1);
		virtual ~VSVolumeShadowMapCuller();
		virtual bool CullConditionNode(const VSMeshNode * pMeshNode);
		virtual bool ForceNoCull(const VSSpatial * pSpatial);	
		virtual bool CullGeometry(VSGeometry * pGeometry);
	};

	class VSGRAPHIC_API VSDirShadowMapCuller : public VSShadowCuller
	{
	public:

		VSDirShadowMapCuller(int iUseMaterialIndex = -1);
		virtual ~VSDirShadowMapCuller();
		virtual bool ForceNoCull(const VSSpatial * pSpatial);
	
	};
}
#endif