#include "VSDebugDraw.h"
#include "VSAABB3.h"
#include "VSSphere3.h"
#include "VSOBB3.h"
#include "VSMaterialPass.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
VSDebugDraw::VSDebugDraw()
{


	VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> ForamtArray;
	VSVertexFormat::VERTEXFORMAT_TYPE Pos;
	Pos.DateType = VSDataBuffer::DT_FLOAT32_3;
	Pos.OffSet = 0;
	Pos.Semantics = VSVertexFormat::VF_POSITION;
	Pos.SemanticsIndex = 0;
	ForamtArray.AddElement(Pos);

	VSVertexFormat::VERTEXFORMAT_TYPE Color;
	Color.DateType = VSDataBuffer::DT_COLOR;
	Color.OffSet = 12;
	Color.Semantics = VSVertexFormat::VF_COLOR;
	Color.SemanticsIndex = 0;
	ForamtArray.AddElement(Color);


	m_pDrawVertexFormat = VSResourceManager::LoadVertexFormat(NULL, &ForamtArray);

	m_pOnlyVertexColor = VSMaterialR::Create((VSMaterial *)VSMaterial::GetDefaultOnlyVertexColor());
	m_pOnlyVertexColorDisableDepth = VSMaterialR::Create((VSMaterial *)VSMaterial::GetDefaultOnlyVertexColorDisableDepth());

	m_bEnable = true;
}
VSDebugDraw::~VSDebugDraw()
{
	m_pOnlyVertexColor = NULL;
	m_pOnlyVertexColorDisableDepth = NULL;

	m_pDrawVertexFormat = NULL;
}

void VSDebugDraw::AddDebugLine(const VSVector3 & P1,const VSVector3 & P2,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}

	DebugVertexType V[2];
	V[0].Pos = P1;
	V[0].Color = Color;

	V[1].Pos = P2;
	V[1].Color = Color;
	if (bDepth)
	{
		DepthDebugLineArray.AddElement(V[0]);
		DepthDebugLineArray.AddElement(V[1]);

	}
	else
	{
		NoDepthDebugLineArray.AddElement(V[0]);
		NoDepthDebugLineArray.AddElement(V[1]);

	}

}
void VSDebugDraw::AddDebugTriangle(const VSVector3 & P1,const VSVector3 & P2,const VSVector3 &P3,
								  const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
}
void VSDebugDraw::AddDebugLineAABB(const VSAABB3 & AABB,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	VSVector3 Point[8];
	AABB.GetPoint(Point);
	AddDebugLine(Point[0],Point[1],Color,bDepth);
	AddDebugLine(Point[1],Point[2],Color,bDepth);
	AddDebugLine(Point[2],Point[3],Color,bDepth);
	AddDebugLine(Point[3],Point[0],Color,bDepth);
	AddDebugLine(Point[4],Point[5],Color,bDepth);
	AddDebugLine(Point[5],Point[6],Color,bDepth);
	AddDebugLine(Point[6],Point[7],Color,bDepth);
	AddDebugLine(Point[7],Point[4],Color,bDepth);
	AddDebugLine(Point[0],Point[4],Color,bDepth);
	AddDebugLine(Point[1],Point[5],Color,bDepth);
	AddDebugLine(Point[2],Point[6],Color,bDepth);
	AddDebugLine(Point[3],Point[7],Color,bDepth);
}
void VSDebugDraw::AddDebugLineSphere(const VSSphere3 & Sphere,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	for (unsigned int i = 0 ; i < 360 ; i++)
	{
		VSVector3 Point1 = Sphere.m_Center + VSVector3(GetFastSin(i),GetFastCos(i),0.0f) * Sphere.m_fRadius;	
		VSVector3 Point2 = Sphere.m_Center + VSVector3(GetFastSin(i + 1),GetFastCos(i + 1),0.0f) * Sphere.m_fRadius;
		AddDebugLine(Point1,Point2,Color,bDepth);
	}
	for (unsigned int i = 0 ; i < 360 ; i++)
	{
		VSVector3 Point1 = Sphere.m_Center + VSVector3(GetFastSin(i),0.0f,GetFastCos(i)) * Sphere.m_fRadius;	
		VSVector3 Point2 = Sphere.m_Center + VSVector3(GetFastSin(i + 1),0.0f,GetFastCos(i + 1)) * Sphere.m_fRadius;
		AddDebugLine(Point1,Point2,Color,bDepth);
	}
	for (unsigned int i = 0 ; i < 360 ; i++)
	{
		VSVector3 Point1 = Sphere.m_Center + VSVector3(0.0f,GetFastSin(i),GetFastCos(i)) * Sphere.m_fRadius;	
		VSVector3 Point2 = Sphere.m_Center + VSVector3(0.0f,GetFastSin(i + 1),GetFastCos(i + 1)) * Sphere.m_fRadius;
		AddDebugLine(Point1,Point2,Color,bDepth);
	}
}
void VSDebugDraw::AddDebugLineOBB(const VSOBB3 & OBB,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	VSVector3 Point[8];
	OBB.GetPoint(Point);
	AddDebugLine(Point[0],Point[1],Color,bDepth);
	AddDebugLine(Point[1],Point[2],Color,bDepth);
	AddDebugLine(Point[2],Point[3],Color,bDepth);
	AddDebugLine(Point[3],Point[0],Color,bDepth);
	AddDebugLine(Point[4],Point[5],Color,bDepth);
	AddDebugLine(Point[5],Point[6],Color,bDepth);
	AddDebugLine(Point[6],Point[7],Color,bDepth);
	AddDebugLine(Point[7],Point[4],Color,bDepth);
	AddDebugLine(Point[0],Point[4],Color,bDepth);
	AddDebugLine(Point[1],Point[5],Color,bDepth);
	AddDebugLine(Point[2],Point[6],Color,bDepth);
	AddDebugLine(Point[3],Point[7],Color,bDepth);

}
void VSDebugDraw::AddDebugTriangleAABB(const VSAABB3 & AABB,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	VSVector3 Point[8];
	AABB.GetPoint(Point);

}
void VSDebugDraw::AddDebugTriangleSphere(const VSSphere3 & Sphere,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}

}
void VSDebugDraw::AddDebugTriangleOBB(const VSOBB3 & OBB,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	VSVector3 Point[8];
	OBB.GetPoint(Point);

}
void VSDebugDraw::DrawDebugInfo(VSCamera * pCamera)
{
	if (!m_bEnable)
	{
		return ;
	}

	
	VSDVGeometry *  pBuffer = VSResourceManager::GetDVGeometry(m_pDrawVertexFormat, VSMeshDate::MDT_LINE, DepthDebugLineArray.GetNum() + NoDepthDebugLineArray.GetNum());

	if (pBuffer)
	{
		pBuffer->Add(DepthDebugLineArray.GetBuffer(), sizeof(DebugVertexType)* DepthDebugLineArray.GetNum());
		pBuffer->ClearAllMaterialInstance();
		pBuffer->AddMaterialInstance(m_pOnlyVertexColor);
		pBuffer->Draw(pCamera);

		pBuffer->Add(NoDepthDebugLineArray.GetBuffer(), sizeof(DebugVertexType)* NoDepthDebugLineArray.GetNum());
		pBuffer->ClearAllMaterialInstance();
		pBuffer->AddMaterialInstance(m_pOnlyVertexColorDisableDepth);
		pBuffer->Draw(pCamera);
	}
	

	DepthDebugLineArray.Clear();
	NoDepthDebugLineArray.Clear();

}