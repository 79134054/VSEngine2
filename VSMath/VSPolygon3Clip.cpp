#include "VSPolygon3.h"
#include "VSAABB3.h"
#include "VSPlane3.h"
#include "VSSegment3.h"
using namespace VSEngine2;
void VSPolygon3::Clip(const VSPlane3 &Plane, 
					 VSPolygon3 &pFront,
					 VSPolygon3 &pBack)const
{


	int			nNumFront=0,     // 前面多边形点的个数
				nNumBack=0,      // 后面多边形点的个数
				nLoop=0,
				nCurrent=0;

	VSVector3 *pvcFront = VS_NEW VSVector3[m_PointNum * 3];
	VSVector3 *pvcBack  = VS_NEW VSVector3[m_PointNum * 3];

	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
		{
			j = 0;
		}
		int iFlagi = Plane.RelationWith(m_pPoint[i]);
		int iFlagj = Plane.RelationWith(m_pPoint[j]);
		if(iFlagi == VSON)
		{
			pvcFront[nNumFront] = m_pPoint[i];
			nNumFront++;
			pvcBack[nNumBack] = m_pPoint[i];
			nNumBack++;
		}
		else
		{
			VSSegment3 Segment;
			Segment.Set(m_pPoint[i],m_pPoint[j]);
			VSREAL t;
			if(Segment.RelationWith(Plane,0,t) == VSINTERSECT && iFlagj != VSON)
			{
				VSVector3 Hit = Segment.GetParameterPoint(t);
				pvcFront[nNumFront] = Hit;
				nNumFront++;
				pvcBack[nNumBack] = Hit;
				nNumBack++;

				
			
			}
			
			if (iFlagi == VSFRONT) 
			{
				
					pvcFront[nNumFront++] = m_pPoint[i];
			} 
			else if (iFlagi == VSBACK) 
			{
				
					pvcBack[nNumBack++] = m_pPoint[i];
			} 
		
		}
	}


	
	pFront.Set(pvcFront,nNumFront);
	if (pFront.m_N.Dot(m_N) < 0.0f)
		pFront.SwapFaces();
	pBack.Set(pvcBack, nNumBack);
	if (pBack.m_N.Dot(m_N) < 0.0f)
		pBack.SwapFaces();

	VSMAC_DELETEA(pvcFront);
	VSMAC_DELETEA(pvcBack);



}

void VSPolygon3::Clip(const VSAABB3 &ABBB)
{
	VSPlane3 Plane[6];
	ABBB.GetPlane(Plane);
	VSPolygon3 Front,Back;
	for (int i = 0 ; i < 6 ; i++) 
	{
		if ( RelationWith(Plane[i]) == VSINTERSECT) 
		{ 
			Clip(Plane[i], Front, Back);
			CopyFrom(Back);
		}
	}
	
}