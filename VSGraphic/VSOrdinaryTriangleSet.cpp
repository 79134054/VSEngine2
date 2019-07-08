#include "VSOrdinaryTriangleSet.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSOrdinaryTriangleSet,VSTriangleSet)
BEGIN_ADD_PROPERTY(VSOrdinaryTriangleSet,VSTriangleSet)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOrdinaryTriangleSet)
IMPLEMENT_INITIAL_END
VSOrdinaryTriangleSet::VSOrdinaryTriangleSet()
{

}
VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const VSAABB3 & AABB)
{
	VSVector3 VertexDate[8];
	AABB.GetPoint(VertexDate);
	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;
	pVertex->SetDate(VertexDate,8,VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);
	VSUSHORT_INDEX Index[36];
	//前面的2个三角形
	Index[0]=0;  Index[1]=1;  Index[2]=2;
	Index[3]=0;  Index[4]=2;  Index[5]=3;
	
	//后面的2个三角形
	Index[6]=4;   Index[7]=6;   Index[8]=5;
	Index[9]=4;   Index[10]=7;  Index[11]=6;
	
	//右侧面的2个三角形
	Index[12]=0;  Index[13]=3;  Index[14]=7;
	Index[15]=0;  Index[16]=7;  Index[17]=4;
	//左侧面的2个三角形
	Index[18]=1;  Index[19]=5;  Index[20]=6;
	Index[21]=1;  Index[22]=6;  Index[23]=2;
	//顶面的2个三角形	
	Index[24]=0;  Index[25]=5;  Index[26]=1;
	Index[27]=0;  Index[28]=4;  Index[29]=5;
	//底面的2个三角形
	Index[30]=2;  Index[31]=6;  Index[32]=7;
	Index[33]=2;  Index[34]=7;  Index[35]=3;
	
	VSDataBuffer *pIndexDateBuffer = VS_NEW VSDataBuffer;
	pIndexDateBuffer->SetDate(Index,36,VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = VS_NEW VSIndexBuffer();
	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);

}
VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const VSOBB3 & OBB)
{
	VSVector3 VertexDate[8];
	OBB.GetPoint(VertexDate);
	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;
	pVertex->SetDate(VertexDate,8,VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);
	VSUSHORT_INDEX Index[36];
	//前面的2个三角形
	Index[0]=0;  Index[1]=1;  Index[2]=2;
	Index[3]=0;  Index[4]=2;  Index[5]=3;

	//后面的2个三角形
	Index[6]=4;   Index[7]=6;   Index[8]=5;
	Index[9]=4;   Index[10]=7;  Index[11]=6;

	//右侧面的2个三角形
	Index[12]=0;  Index[13]=3;  Index[14]=7;
	Index[15]=0;  Index[16]=7;  Index[17]=4;
	//左侧面的2个三角形
	Index[18]=1;  Index[19]=5;  Index[20]=6;
	Index[21]=1;  Index[22]=6;  Index[23]=2;
	//顶面的2个三角形	
	Index[24]=0;  Index[25]=5;  Index[26]=1;
	Index[27]=0;  Index[28]=4;  Index[29]=5;
	//底面的2个三角形
	Index[30]=2;  Index[31]=6;  Index[32]=7;
	Index[33]=2;  Index[34]=7;  Index[35]=3;

	VSDataBuffer *pIndexDateBuffer = VS_NEW VSDataBuffer;
	pIndexDateBuffer->SetDate(Index,36,VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = VS_NEW VSIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const VSPlane3 &Plane)
{
	VSMatrix3X3W Mat;
	Mat.CreateFromLookDir(Plane.GetPoint(),Plane.GetN());

	VSVector3 A0(Mat.M[0][0],Mat.M[0][1],Mat.M[0][2]);
	VSVector3 A1(Mat.M[1][0],Mat.M[1][1],Mat.M[1][2]);

	VSVector3 VertexDate[4];
	VertexDate[0] = Plane.GetPoint() + A0 * DRAW_MATH_ELEMENT_LENGTH + A1 * DRAW_MATH_ELEMENT_LENGTH;
	VertexDate[1] = Plane.GetPoint() - A0 * DRAW_MATH_ELEMENT_LENGTH + A1 * DRAW_MATH_ELEMENT_LENGTH;
	VertexDate[2] = Plane.GetPoint() - A0 * DRAW_MATH_ELEMENT_LENGTH - A1 * DRAW_MATH_ELEMENT_LENGTH;
	VertexDate[3] = Plane.GetPoint() + A0 * DRAW_MATH_ELEMENT_LENGTH - A1 * DRAW_MATH_ELEMENT_LENGTH;

	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;
	pVertex->SetDate(VertexDate,4,VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);

	VSUSHORT_INDEX Index[6];

	Index[0]=0;  Index[1]=1;  Index[2]=2;
	Index[3]=0;  Index[4]=2;  Index[5]=3;

	VSDataBuffer *pIndexDateBuffer = VS_NEW VSDataBuffer;
	pIndexDateBuffer->SetDate(Index,6,VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = VS_NEW VSIndexBuffer();
	
	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const VSTriangle3 &Triangle)
{
	VSVector3 VertexDate[3];
	Triangle.GetPoint(VertexDate);
	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;
	pVertex->SetDate(VertexDate,3,VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);

	VSUSHORT_INDEX Index[3];
	Index[0]=0;  Index[1]=1;  Index[2]=2;
	VSDataBuffer *pIndexDateBuffer = VS_NEW VSDataBuffer;
	pIndexDateBuffer->SetDate(Index,3,VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = VS_NEW VSIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const VSRectangle3 &Rectangle)
{
	VSVector3 VertexDate[4];
	Rectangle.GetPoint(VertexDate);
	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;
	pVertex->SetDate(VertexDate,4,VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);

	VSUSHORT_INDEX Index[6];

	Index[0]=0;  Index[1]=2;  Index[2]=1;
	Index[3]=0;  Index[4]=3;  Index[5]=2;

	VSDataBuffer *pIndexDateBuffer = VS_NEW VSDataBuffer;
	pIndexDateBuffer->SetDate(Index,6,VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = VS_NEW VSIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const VSPolygon3 &Polyon)
{
	VSMAC_ASSERT(Polyon.GetpPoint() && Polyon.GetPointNum());

	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;
	pVertex->SetDate(Polyon.GetpPoint(),Polyon.GetPointNum(),VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);


	VSDataBuffer *pIndexDateBuffer = VS_NEW VSDataBuffer;
	pIndexDateBuffer->SetDate(Polyon.GetpIndex(),Polyon.GetIndexNum(),VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = VS_NEW VSIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const VSSphere3 & Sphere)
{
	VSREAL LongitudeAngle = 10;//经度线上的划分角
	VSREAL LatitudeAngle = 10;//维度线上的划分角

	VSREAL AnglePI = 180.0f;
	unsigned int LongitudeNum = 0;
	while(AnglePI > 0)
	{
		LongitudeNum++ ;
		AnglePI -= LongitudeAngle;
	}
	LongitudeNum++;
	unsigned int LatitudeNum = unsigned int(360.0f / LatitudeAngle) + 1;

	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;
	pVertex->CreateEmptyBuffer(LongitudeNum * LatitudeNum,VSDataBuffer::DT_FLOAT32_3);

	VSVector3 * pVertexDate = (VSVector3 *)pVertex->GetDate();
	VSREAL i = 0 ;
	for(unsigned int k = 0 ; k < LongitudeNum ; k++)
	{
		if(i > 180)
			i = 180;
		VSREAL iRadian = AngleToRadian(i);
		for(VSREAL j = 0 ; j <= 360.0f; j = j + LatitudeAngle)
		{


			VSREAL jRadian = AngleToRadian(j);
			pVertexDate->x = Sphere.m_fRadius * SIN(iRadian) * COS(jRadian);
			pVertexDate->z = Sphere.m_fRadius * SIN(iRadian) * SIN(jRadian);
			pVertexDate->y = Sphere.m_fRadius * COS(iRadian);

			*pVertexDate += Sphere.m_Center;
			pVertexDate++;
		}
		i = i + LongitudeAngle;
	}
	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);

	VSDataBuffer *pIndexDateBuffer = VS_NEW VSDataBuffer;
	pIndexDateBuffer->CreateEmptyBuffer((LongitudeNum - 1) * LatitudeNum * 2 * 3,VSDataBuffer::DT_USHORT);
	VSUSHORT_INDEX * pIndexData = (VSUSHORT_INDEX *)pIndexDateBuffer->GetDate();
	for (unsigned int i = 0 ; i < LongitudeNum - 1 ; i++)
	{
		unsigned int z_width_poly= i * LatitudeNum ;
		unsigned int z_width = i * LatitudeNum;
		for(unsigned int j = 0 ; j < LatitudeNum ; j++)
		{
			unsigned int z_width_x = z_width + j;
			unsigned int z_width_xX2 = (z_width_poly + j) * 2;
			if(j != LatitudeNum - 1)
			{
				pIndexData[z_width_xX2 * 3 + 0] = z_width_x;
				pIndexData[z_width_xX2 * 3 + 2] = z_width_x + LatitudeNum;
				pIndexData[z_width_xX2 * 3 + 1] = z_width_x + 1;

				pIndexData[(z_width_xX2 + 1) * 3 + 0] = z_width_x + 1 + LatitudeNum;
				pIndexData[(z_width_xX2 + 1) * 3 + 2] = z_width_x + 1;
				pIndexData[(z_width_xX2 + 1) * 3 + 1] = z_width_x + LatitudeNum;
			}
			else
			{
				pIndexData[z_width_xX2 * 3 + 0] = z_width_x;
				pIndexData[z_width_xX2 * 3 + 2] = z_width_x + LatitudeNum;
				pIndexData[z_width_xX2 * 3 + 1] = z_width_x - LatitudeNum + 1;

				pIndexData[(z_width_xX2 + 1) * 3 + 0] = z_width_x + 1;
				pIndexData[(z_width_xX2 + 1) * 3 + 2] = z_width_x - LatitudeNum + 1;
				pIndexData[(z_width_xX2 + 1) * 3 + 1] = z_width_x + LatitudeNum;
			}

		}
	}

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = VS_NEW VSIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
VSOrdinaryTriangleSet::~VSOrdinaryTriangleSet()
{

}
VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(VSSurface3 * pSurface ,VSSurface3Subdivision * pSubidvision)
{
	VSMAC_ASSERT(pSubidvision);
	VSMAC_ASSERT(pSurface);
	if(!pSubidvision->GetPoints())
	{
		pSubidvision->SetSurface(pSurface);
		bool bResult = pSubidvision->Subdivision();
		VSMAC_ASSERT(bResult);
	}
	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;
	unsigned int Row = pSubidvision->GetRow();
	unsigned int Column = pSubidvision->GetColumn();
	pVertex->SetDate(pSubidvision->GetPoints(), Row * Column,VSDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,VSVertexFormat::VF_POSITION);

	VSDataBuffer *pIndexDateBuffer = VS_NEW VSDataBuffer;
	pIndexDateBuffer->CreateEmptyBuffer((Row - 1) * (Column - 1) * 2 * 3,VSDataBuffer::DT_USHORT);
	VSUSHORT_INDEX * pIndexData = (VSUSHORT_INDEX *)pIndexDateBuffer->GetDate();

	for (unsigned int i = 0 ; i < Row - 1 ; i++)
	{
		unsigned int z_width_poly= i * (Column - 1);
		unsigned int z_width = i * Column;
		for(unsigned int j = 0 ; j < Column - 1 ; j++)
		{
			unsigned int z_width_x = z_width + j;
			unsigned int z_width_xX2 = (z_width_poly + j) * 2;
			pIndexData[z_width_xX2 * 3 + 0] = z_width_x;
			pIndexData[z_width_xX2 * 3 + 2] = z_width_x + Column;
			pIndexData[z_width_xX2 * 3 + 1] = z_width_x + 1;

			pIndexData[(z_width_xX2 + 1) * 3 + 0] = z_width_x + 1 + Column;
			pIndexData[(z_width_xX2 + 1) * 3 + 2] = z_width_x + 1;
			pIndexData[(z_width_xX2 + 1) * 3 + 1] = z_width_x + Column;

		}
	}
	
	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = VS_NEW VSIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
