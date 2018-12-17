#include "VSPolygon3.h"
using namespace VSEngine2;
VSPolygon3::VSPolygon3()
{
	m_PointNum = 0;		//点个数
	m_IndexNum = 0;		//索引个数
	m_Flag = 0;			// 标志位
	m_pPoint = NULL;		// 点列表
	m_pIndex = NULL;		// 索引列表
}
/*----------------------------------------------------------------*/
VSPolygon3::~VSPolygon3()
{
	VSMAC_DELETEA(m_pPoint);
	VSMAC_DELETEA(m_pIndex);
}
/*----------------------------------------------------------------*/
VSPolygon3::VSPolygon3(const VSVector3 *pPoint,unsigned int PointNum)
{
	Set(pPoint,PointNum);
}
/*----------------------------------------------------------------*/
bool VSPolygon3::CopyFrom(const VSPolygon3 & Polygon)
{
	if(!Polygon.m_pPoint || !Polygon.m_PointNum || !Polygon.m_pIndex || !Polygon.m_IndexNum)
		return 0;
	m_PointNum = Polygon.m_PointNum;	
	m_IndexNum = Polygon.m_IndexNum;		
	m_Flag = Polygon.m_Flag;
	VSMAC_DELETEA(m_pPoint);
	VSMAC_DELETEA(m_pIndex);
	m_pPoint = VS_NEW VSVector3[m_PointNum];
	if(!m_pPoint)
		return 0;
	m_pIndex = VS_NEW VSUSHORT_INDEX[m_IndexNum];
	if(!m_pIndex)
		return 0;
	VSMemcpy(m_pPoint,Polygon.m_pPoint,sizeof(VSVector3) * m_PointNum);
	VSMemcpy(m_pIndex,Polygon.m_pIndex,sizeof(VSUSHORT_INDEX) * m_IndexNum);
	return 1;

}
/*----------------------------------------------------------------*/
void VSPolygon3::SwapFaces()
{

	VSUSHORT_INDEX *pIndis = new VSUSHORT_INDEX[m_IndexNum];


	for (unsigned int i = 0; i < m_IndexNum ; i++)
		pIndis[m_IndexNum - i - 1] = m_pIndex[i];

	m_Flag = !m_Flag;

	VSPlane3::Set(m_N * (-1.0f),m_fD);

	VSMAC_DELETEA(m_pIndex);
	m_pIndex = pIndis;
}