#include "VSCuller.h"
#include "VSSpatial.h"
#include "VSGeometry.h"
#include "VSGraphicInclude.h"
#include "VSSceneManager.h"
#include "VSPointLight.h"
#include "VSGeometry.h"
#include "VSSpotLight.h"
#include "VSMaterial.h"
#include "VSSortLight.h"
using namespace VSEngine2;
VSRenderContext VSRenderContext::ms_RenderContextNULL;
DECLEAR_TIME_PROFILENODE(CullerSort, ViewFamilyUpdate)
VSCuller::VSCuller(int iUseMaterialIndex)
{
	m_uiPlaneNum = 0;
	m_uiPlaneState =0xffffffff;
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		for (unsigned int i = 0 ; i < VST_MAX ; i++)
		{
			m_VisibleSet[j][i].Clear();
		}
	}
	
	m_LightSet.Clear();
	m_pCamera = NULL;
	m_iUseMaterialIndex = iUseMaterialIndex;
}
VSCuller::~VSCuller()
{

}
bool VSCuller::PushPlane(const VSPlane3 & Plane)
{
	if(m_uiPlaneNum > VS_MAX_PLANE_NUM)
		return false;
	m_Plane[m_uiPlaneNum] = Plane;
	m_uiPlaneNum++;
	return true;
}
bool VSCuller::PushCameraPlane(VSCamera &Camera)
{
	VSPlane3 Plane[VSCamera::CP_MAX];
	Camera.GetPlane(Plane);
	for(unsigned int i =0 ; i < VSCamera::CP_MAX; i++)
	{
		if(!PushPlane(Plane[i]))
			return 0;
	}
	VSArray<VSPlane3> & CustomCullPlane = Camera.m_CustomCullPlane;
	for (unsigned int i = 0; i < CustomCullPlane.GetNum(); i++)
	{
		if (!PushPlane(CustomCullPlane[i]))
			return 0;
	}
	m_pCamera = &Camera;
	return 1;
}
bool VSCuller::PopPlane(VSPlane3 &Plane)
{
	if(!m_uiPlaneNum)
		return false;
	Plane = m_Plane[m_uiPlaneNum];
	m_uiPlaneNum--;
	return true;
}
bool VSCuller::InsertObject(VSRenderContext &VisibleContext,unsigned int uiVisibleSetType,unsigned int uiRenderGroup)
{
	if(uiVisibleSetType >= VST_MAX || uiRenderGroup >= RG_MAX)
		return 0;
	m_VisibleSet[uiRenderGroup][uiVisibleSetType].AddElement(VisibleContext);
	return 1;
}
bool VSCuller::HasLight(VSLight * pLight)
{
	return m_LightSet.FindElement(pLight) != m_LightSet.GetNum();
}
void VSCuller::GetAndSortLight()
{
	if (GetLightNum() == 0)
	{
		return;
	}
	for (unsigned int uiRenderGroup = 0; uiRenderGroup < RG_MAX; uiRenderGroup++)
	{
		for (unsigned int t = 0; t <= VST_MAX; t++)
		{
			for (unsigned int j = 0; j < GetVisibleNum(t, uiRenderGroup); j++)
			{
				VSRenderContext& VisibleContext = GetVisibleSpatial(j, t, uiRenderGroup);

				VSGeometry * pGeometry = VisibleContext.m_pGeometry;

				for (unsigned int l = 0; l < GetLightNum(); l++)
				{
					VSLight * pLight = GetLight(l);
					if (pLight->IsRelative(pGeometry))
					{
						if (pLight->GetLightType() == VSLight::LT_SKY)
						{
							VisibleContext.m_pInDirectLight.AddElement(pLight);
						}
						else
						{
							VisibleContext.m_pDirectLight.AddElement(pLight);
						}

					}
				}
				VisibleContext.m_pInDirectLight.Sort(0, VisibleContext.m_pInDirectLight.GetNum() - 1, LightPriority());
				VisibleContext.m_pDirectLight.Sort(0, VisibleContext.m_pDirectLight.GetNum() - 1, LightPriority());
			}
		}
	}
}
unsigned int VSCuller::IsVisible(const VSSphere3 &S,bool bClearState)
{
	if(!m_uiPlaneNum)
		return 0;
	int iP = m_uiPlaneNum - 1;
	unsigned int uiMask = 1 << iP;
	unsigned int uiPlaneInNum = 0;
	for (unsigned int i = 0; i < m_uiPlaneNum; i++, iP--, uiMask >>= 1)
	{
		if (m_uiPlaneState & uiMask)
		{
			int iSide = S.RelationWith(m_Plane[iP]);

			if (iSide == VSFRONT)
			{
				if (bClearState == true)
				{
					ClearPlaneState();
				}
				return VSF_NONE;
			}

			if (iSide == VSBACK)
			{

				m_uiPlaneState &= ~uiMask;
				uiPlaneInNum++;
			}
		}
		else
		{
			uiPlaneInNum++;
		}
	}
	if (uiPlaneInNum == m_uiPlaneNum)
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_ALL;
	}
	else
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_PARTIAL;
	}
}
unsigned int VSCuller::IsVisible(const VSVector3 &Point,bool bClearState)
{
	if(!m_uiPlaneNum)
		return 0;
	int iP = m_uiPlaneNum - 1;
	unsigned int uiMask = 1 << iP;
	unsigned int uiPlaneInNum = 0;
	for (unsigned int i = 0; i < m_uiPlaneNum; i++, iP--, uiMask >>= 1)
	{
		if (m_uiPlaneState & uiMask)
		{
			int iSide = Point.RelationWith(m_Plane[iP]);

			if (iSide == VSFRONT)
			{
				if (bClearState == true)
				{
					ClearPlaneState();
				}
				return VSF_NONE;
			}

			if (iSide == VSBACK)
			{

				m_uiPlaneState &= ~uiMask;
				uiPlaneInNum++;
			}
		}
		else
		{
			uiPlaneInNum++;
		}
	}
	if (uiPlaneInNum == m_uiPlaneNum)
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_ALL;
	}
	else
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_PARTIAL;
	}
}
unsigned int VSCuller::IsVisible(const VSAABB3 &BV,bool bClearState)
{
	if(!m_uiPlaneNum)
		return 0;
	int iP = m_uiPlaneNum - 1;
	unsigned int uiMask = 1 << iP;
	unsigned int uiPlaneInNum = 0;
	for (unsigned int i = 0; i < m_uiPlaneNum; i++, iP--, uiMask >>= 1)
	{
		if (m_uiPlaneState & uiMask)
		{
			int iSide = BV.RelationWith(m_Plane[iP]);

			if (iSide == VSFRONT)
			{
				if (bClearState == true)
				{
					ClearPlaneState();
				}
				return VSF_NONE;
			}

			if (iSide == VSBACK)
			{
				
				m_uiPlaneState &= ~uiMask;
				uiPlaneInNum++;
			}
		}
		else
		{
			uiPlaneInNum++;
		}
	}
	if (uiPlaneInNum == m_uiPlaneNum)
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_ALL;
	}
	else
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_PARTIAL;
	}
}
bool VSCuller::RenderPriority::operator()(VSRenderContext & p1,VSRenderContext & p2)
{
	unsigned int uiMaterialAddr1 = 0;
	unsigned int uiMaterialAddr2 = 0;
	unsigned int uiMeshDateAddr1 = 0;
	unsigned int uiMeshDateAddr2 = 0;

	VSMaterial * pMaterial1 = p1.m_pMaterialInstance->GetMaterial();
	VSMaterial * pMaterial2 = p2.m_pMaterialInstance->GetMaterial();
	uiMaterialAddr1 = (unsigned int)pMaterial1;
	uiMaterialAddr2 = (unsigned int)pMaterial2;



	uiMeshDateAddr1 = (unsigned int)(p1.m_pGeometry->GetMeshDate());

	uiMeshDateAddr2 = (unsigned int)(p2.m_pGeometry->GetMeshDate());

	if (pMaterial1->m_uiCustomLayer == pMaterial2->m_uiCustomLayer)
	{
		if (uiMaterialAddr1 > uiMaterialAddr2)
		{
			return 1;
		}
		else if (uiMaterialAddr1 < uiMaterialAddr2)
		{
			return 0;
		}
		else
		{
			if (uiMeshDateAddr1 > uiMeshDateAddr2)
			{
				return 1;
			}
			else if (uiMeshDateAddr1 < uiMeshDateAddr2)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}
	else if (pMaterial1->m_uiCustomLayer > pMaterial2->m_uiCustomLayer)
	{
		return 0;
	}
	else 
	{
		return 1;
	}


}
void VSCuller::Sort()
{
	ADD_TIME_PROFILE(CullerSort);
	VSCamera * pCamera = m_pCamera;
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		if (m_VisibleSet[j][VST_BASE].GetNum() > 0)
		{
			m_VisibleSet[j][VST_BASE].Sort(0,m_VisibleSet[j][VST_BASE].GetNum() - 1,RenderPriority());

		}
		
		if (m_VisibleSet[j][VST_ALPHABLEND].GetNum() > 0 && pCamera)
		{
			m_VisibleSet[j][VST_ALPHABLEND].Sort(0,m_VisibleSet[j][VST_BASE].GetNum() - 1,AlphaPriority(pCamera));

		}

		if (m_VisibleSet[j][VST_ALPHATEST].GetNum() > 0)
		{
			m_VisibleSet[j][VST_ALPHATEST].Sort(0,m_VisibleSet[j][VST_ALPHATEST].GetNum() - 1,RenderPriority());
		}
	}
	
}
bool VSCuller::AlphaPriority::operator()(VSRenderContext & p1,VSRenderContext & p2)
{
	if (!m_pCamera)
	{
		return 1;
	}
	VSVector3 vLength1 = m_pCamera->GetWorldTranslate() - p1.m_pGeometry->GetWorldTranslate(); 
	VSVector3 vLength2 = m_pCamera->GetWorldTranslate() - p2.m_pGeometry->GetWorldTranslate();

	if (vLength1.GetSqrLength() > vLength2.GetSqrLength())
	{
		return 0;
	}
	return 1;

}
void VSCuller::InsertLight(VSLight * pLight)
{
	if (pLight)
	{
		m_LightSet.AddElement(pLight);
	}
}
bool VSCuller::ForceNoCull(const VSSpatial * pSpatial)
{
	return false;
}
bool VSCuller::CullConditionNode(const VSMeshNode * pMeshNode)
{
	return false;
}
bool VSCuller::CullGeometry(VSGeometry * pGeometry)
{
	VSShadowVolumeGeometry * pSVG = DynamicCast<VSShadowVolumeGeometry>(pGeometry);
	if (pSVG)
	{
		return true;
	}
	return false;

}
void VSCuller::GetAllVisibleAABB(unsigned int uiVisibleSetType, unsigned int uiRenderGroup, VSArray<VSAABB3> & AABBArray, unsigned int GetFlag)
{

	for (unsigned int i = 0 ; i < m_VisibleSet[uiRenderGroup][uiVisibleSetType].GetNum() ;i++)
	{
		VSGeometry * pGeometry = m_VisibleSet[uiRenderGroup][uiVisibleSetType][i].m_pGeometry;
		if (GetFlag & GF_NO_CASTER_SHADOW)
		{
			if (pGeometry->GetMeshNode()->m_bCastShadow == false)
			{
				continue;
			}
		}

		AABBArray.AddElement(pGeometry->GetWorldAABB());
		
		
	}

}
VSShadowCuller::VSShadowCuller(int iUseMaterialIndex)
:VSCuller(iUseMaterialIndex)
{

}
VSShadowCuller::~VSShadowCuller()
{

}
bool VSShadowCuller::CullConditionNode(const VSMeshNode * pMeshNode)
{
	if (pMeshNode->m_bCastShadow == true)
	{
		return false;
	}
	return true;
}
bool VSShadowCuller::TestSweptSphere(VSSphere3 & Sphere,VSVector3 & SweptDir)
{
	//  algorithm -- get all 12 intersection points of the swept sphere with the view frustum
	//  for all points >0, displace sphere along the sweep driection.  if the displaced sphere
	//  is inside the frustum, return TRUE.  else, return FALSE
	float displacements[12];
	int cnt = 0;
	float a, b;

	for (int i=0; i<6; i++)
	{
		if (SweptSpherePlaneIntersect(a, b, m_Plane[i], Sphere, SweptDir))
		{
			if (a>=0.f)
				displacements[cnt++] = a;
			if (b>=0.f)
				displacements[cnt++] = b;
		}
	}

	for (int i=0; i<cnt; i++)
	{
		VSSphere3 displacedSphere = Sphere;
		displacedSphere.m_Center += SweptDir * displacements[i];
		displacedSphere.m_fRadius *= 1.1f;
		if(TestSphere(displacedSphere))
		{
			return true;
		}
	}
	return false;
}
bool VSShadowCuller::SweptSpherePlaneIntersect(float& t0, float& t1, VSPlane3 &Plane, VSSphere3 & Shpere, VSVector3 & SweptDir)
{
	float b_dot_n = Plane.GetN().Dot(Shpere.m_Center) + Plane.GetfD();
	float d_dot_n = Plane.GetN().Dot(SweptDir);

	if (d_dot_n == 0.f)
	{
		if (b_dot_n <= Shpere.m_fRadius)
		{
			//  effectively infinity
			t0 = 0.f;
			t1 = 1e32f;
			return true;
		}
		else
			return false;
	}
	else
	{
		float tmp0 = ( Shpere.m_fRadius - b_dot_n) / d_dot_n;
		float tmp1 = (-Shpere.m_fRadius - b_dot_n) / d_dot_n;
		t0 = min(tmp0, tmp1);
		t1 = max(tmp0, tmp1);
		return true;
	}
}
bool VSShadowCuller::TestSphere(VSSphere3 & Shpere) const
{	
	for (int i = 0; i < 6 ; i++)
	{
		if (Shpere.RelationWith(m_Plane[i]) == VSFRONT)
		{
			return false;
		}
	}
	return true;
}
void VSShadowCuller::Sort()
{
	// 
}
VSVolumeShadowMapCuller::VSVolumeShadowMapCuller(int iUseMaterialIndex)
:VSShadowCuller(iUseMaterialIndex)
{

}
VSVolumeShadowMapCuller::~VSVolumeShadowMapCuller()
{

}
bool VSVolumeShadowMapCuller::CullConditionNode(const VSMeshNode * pMeshNode)
{
	if (pMeshNode->m_bCastShadow == true || pMeshNode->m_bReceiveShadow == true)
	{
		return false;
	}
	return true;
}
bool VSVolumeShadowMapCuller::ForceNoCull(const VSSpatial * pSpatial)
{
	if (m_pLocalLight->GetLightType() == VSLight::LT_POINT)
	{
		VSPointLight * pPointLight = (VSPointLight *)m_pLocalLight;
		VSAABB3 AABB(pPointLight->GetWorldTranslate(), pPointLight->m_Range, pPointLight->m_Range, pPointLight->m_Range);
		if (AABB.RelationWith(pSpatial->GetWorldAABB()) == VSINTERSECT)
		{
			return true;
		}
	}
	else if (m_pLocalLight->GetLightType() == VSLight::LT_DIRECTION)
	{
		VSAABB3  aabb = pSpatial->GetWorldAABB();

		VSVector3 Center = aabb.GetCenter();
		VSVector3 Temp = aabb.GetMaxPoint() - Center;

		VSSphere3 TempSphere(Center, Temp.GetLength());

		VSVector3 SweptDir, Up, Right;
		m_pLocalLight->GetWorldDir(SweptDir, Up, Right);
		if (TestSweptSphere(TempSphere, SweptDir))
			return true;
	}
	return false;
}
bool VSVolumeShadowMapCuller::CullGeometry(VSGeometry * pGeometry)
{
	
	VSShadowVolumeGeometry * pSVG = DynamicCast<VSShadowVolumeGeometry>(pGeometry);
	if (VSRenderer::ms_pRenderer->IsSupportGS())
	{
		if (!pSVG)
		{
			return true;
		}
		return false;
	}
	else
	{
		if (pSVG)
		{
			return false;
		}
		return true;
	}
	
}
VSDirShadowMapCuller::VSDirShadowMapCuller(int iUseMaterialIndex)
:VSShadowCuller(iUseMaterialIndex)
{

}
VSDirShadowMapCuller::~VSDirShadowMapCuller()
{

}
bool VSDirShadowMapCuller::ForceNoCull(const VSSpatial * pSpatial)
{

	VSAABB3  aabb = pSpatial->GetWorldAABB();

	VSVector3 Center = aabb.GetCenter();
	VSVector3 Temp = aabb.GetMaxPoint() - Center;

	VSSphere3 TempSphere(Center,Temp.GetLength());

	VSDirectionLight * pDirLight = (VSDirectionLight *)m_pLocalLight;
	VSVector3 Dir, Up, Right;
	pDirLight->GetWorldDir(Dir, Up, Right);
	if (TestSweptSphere(TempSphere, Dir))
	 	return true;
	return false;

}


