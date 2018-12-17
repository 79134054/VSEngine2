#ifndef VSGEOMETRY_H
#define VSGEOMETRY_H
#include "VSSpatial.h"
#include "VSArray.h"
#include "VSVertexBuffer.h"
namespace VSEngine2
{

	class VSSkelecton;
	class VSMeshNode;
	class VSStream;
	DECLARE_Ptr(VSBoneNode);
	DECLARE_Ptr(VSMorphSet);
	DECLARE_Ptr(VSMeshDate);
	class VSGRAPHIC_API VSGeometry : public VSSpatial
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum	// Update Geometry Renturn Information
		{
			 UGRI_FAIL,
			 UGRI_CONTINUME,
			 UGRI_END,			 
			 UGRI_MAX
		};
		enum // Geometry Use Type
		{
			GUT_NORMAL,
			GUT_SHADOW_VOLUME,
			GUT_MAX
		};
		VSGeometry();
		virtual ~VSGeometry();
		
		void SetMeshDate(VSMeshDate * pMeshDate);	
		VSMeshDate * GetMeshDate()const;
		VSMeshDate * GetOriginMeshDate()const;

		unsigned int GetActiveNum();
		bool SetActiveNum(unsigned int uiActiveNum);
		unsigned int GetTotleNum()const;
		unsigned int GetMeshDateType();



		virtual void CreateLocalAABB();

		void SetAffectBoneArray(const VSArray<VSBoneNode *> & pBoneNodeArray);
		void SetAffectBoneArray(const VSArray<VSUsedName> & BoneNodeArray);
		FORCEINLINE unsigned int GetAffectBoneNum()const
		{
			return m_pBoneNode.GetNum();
		}
		FORCEINLINE VSArray<VSVector3W>& GetSkinWeightBuffer()
		{
			return m_SkinWeightBuffer;
		}
		FORCEINLINE VSBoneNode * GetAffectBone(unsigned int i)const
		{
			if(i < m_pBoneNode.GetNum())
				return m_pBoneNode[i];
			else
				return NULL;
		}
		VSSkelecton * GetAffectSkelecton()const;

		VSMeshNode * GetMeshNode()const;

		virtual void CreateClodMesh();
		virtual void RemoveClodMesh();
		unsigned int GetVertexNum()const;


		//更新和视点无关的Mesh 信息
		virtual unsigned int UpdateGeometry();

		virtual bool PostLoad( void * pDate);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data = 0);
		FORCEINLINE void SetLocalBV(const VSAABB3 & BV)
		{
			m_LocalBV = BV;
		}
		FORCEINLINE VSAABB3 GetLocalBV()const
		{
			return m_LocalBV;
		}
		VSUsedName m_GeometryName;
		virtual bool IsSwapCull();
		virtual unsigned int GetGeometryUseType()
		{
			return GUT_NORMAL;
		}
		static void LoadDefault();
		static VSGeometry * GetDefaultQuad()
		{
			return ms_Quad;
		}
		static VSGeometry * GetDefaultCub()
		{
			return ms_DefaultCub;
		}
		static VSGeometry * GetDefaultCubCone()
		{
			return ms_DefaultCubCone;
		}
		static VSGeometry * GetDefaultCone()
		{
			return ms_DefaultCone;
		}
		static VSGeometry *GetDefaultRenderCube()
		{
			return ms_DefaultRenderCube;
		}
		bool SetMaterialInstance(VSMaterialInstance * pMaterial, unsigned int uiIndex);
		bool SetMaterialInstance(VSMaterialR * pMaterial, unsigned int uiIndex);

		bool SetUseMaterialInstance(unsigned int uiIndex);
		unsigned int AddMaterialInstance(VSMaterialR * pMaterial);
		unsigned int AddMaterialInstance(VSMaterialInstance * pMaterial);


		void ClearAllMaterialInstance();
		void DeleteMaterialInstance(unsigned int i);
		unsigned int DeleteMaterialInstance(VSMaterialInstance * pMaterial);

		VSMaterialInstance * GetUseMaterialInstance()const;

		VSMaterialInstance * GetMaterialInstance(unsigned int i)const;
		void AddMorphAABB(VSVertexBuffer * pMorphVertexBuffer);
		void CreateMorphMeshData();
	protected:
		//friend class VSRenderer;
		VSArray<VSMaterialInstancePtr> m_pMaterialInstance;
		unsigned int m_uiCurUseMaterial;

		virtual void UpDateView(VSCuller & Culler, double dAppTime);
		virtual void UpdateWorldBound(double dAppTime);//更新世界边界体 
		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
		virtual void UpdateOther(double dAppTime);
		VSMeshDatePtr m_pMeshDate;
		VSMeshDatePtr m_pMorphMeshDate;
		unsigned int m_uiActiveNum;
		VSArray<VSBoneNode *> m_pBoneNode;

		VSArray<VSUsedName> m_BoneName;
		VSAABB3	m_LocalBV;
		void LinkBoneNode();
		VSArray<VSVector3W> m_SkinWeightBuffer;
		
		static VSPointer<VSGeometry> ms_Quad;
		static VSPointer<VSGeometry> ms_DefaultCub;
		static VSPointer<VSGeometry> ms_DefaultCubCone;
		static VSPointer<VSGeometry> ms_DefaultCone;
		static VSPointer<VSGeometry> ms_DefaultRenderCube;
		VSREAL m_fCLodPercent;
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	};
	DECLARE_Ptr(VSGeometry);
	VSTYPE_MARCO(VSGeometry);

	class VSGRAPHIC_API VSShadowVolumeGeometry : public VSGeometry
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSShadowVolumeGeometry();
		virtual ~VSShadowVolumeGeometry();
		virtual unsigned int GetGeometryUseType()
		{
			return GUT_SHADOW_VOLUME;
		}
		virtual void CreateClodMesh(){};
		virtual void RemoveClodMesh(){};
	};
	DECLARE_Ptr(VSShadowVolumeGeometry);
	VSTYPE_MARCO(VSShadowVolumeGeometry);
};
#endif