#include "VSOrdinaryLineSet.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSOrdinaryLineSet,VSLineSet)
BEGIN_ADD_PROPERTY(VSOrdinaryLineSet,VSLineSet)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOrdinaryLineSet)
IMPLEMENT_INITIAL_END
VSOrdinaryLineSet::VSOrdinaryLineSet()
{

}
VSOrdinaryLineSet::~VSOrdinaryLineSet()
{

}
VSOrdinaryLineSet::VSOrdinaryLineSet(VSControlCurve3 * pControlCurve,VSCurve3Subdivision * pSubidvision)
{
	VSMAC_ASSERT(pSubidvision && pControlCurve);
	if(!pSubidvision->GetPoints())
	{
		pSubidvision->SetSubCurve(pControlCurve);
		pSubidvision->Subdivision();
	}
	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;

	pVertex->SetDate(pSubidvision->GetPoints(), pSubidvision->GetPointNum(),VSDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);

	bool bResult = CreateIndex(LT_OPEN);
	VSMAC_ASSERT(bResult);
}
VSOrdinaryLineSet::VSOrdinaryLineSet(const VSLine3 & Line)
{
	VSVector3 Verts[2];
	Verts[0] = Line.GetOrig() - Line.GetDir() * DRAW_MATH_ELEMENT_LENGTH;
	Verts[1] = Line.GetOrig() + Line.GetDir() * DRAW_MATH_ELEMENT_LENGTH;

	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;

	pVertex->SetDate(Verts, 2,VSDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);
	bool bResult = CreateIndex(LT_OPEN);
	VSMAC_ASSERT(bResult);
}
VSOrdinaryLineSet::VSOrdinaryLineSet(const VSRay3 & Ray)
{
	VSVector3 Verts[2];
	Verts[0] = Ray.GetOrig();
	Verts[1] = Verts[0] + Ray.GetDir() * DRAW_MATH_ELEMENT_LENGTH;
	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;

	pVertex->SetDate(Verts, 2,VSDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);
	bool bResult = CreateIndex(LT_OPEN);
	VSMAC_ASSERT(bResult);
}
VSOrdinaryLineSet::VSOrdinaryLineSet(const VSSegment3 & Segment)
{
	VSVector3 Verts[2];
	Verts[0] = Segment.GetOrig();
	Verts[1] = Segment.GetEnd();
	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;

	pVertex->SetDate(Verts, 2,VSDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);
	bool bResult = CreateIndex(LT_OPEN);
	VSMAC_ASSERT(bResult);

}
