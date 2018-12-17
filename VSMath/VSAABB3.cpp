#include "VSAABB3.h"
using namespace VSEngine2;
const VSVector3 VSAABB3::m_A[3] = {VSVector3(1.0f,0.0f,0.0f),VSVector3(0.0f,1.0f,0.0f),VSVector3(0.0f,0.0f,1.0f)};
/*----------------------------------------------------------------*/
VSAABB3::VSAABB3()
{
	m_fA[0] = m_fA[1] = m_fA[2] = 0.0f;
}
/*----------------------------------------------------------------*/
VSAABB3::~VSAABB3()
{
	   
}
/*----------------------------------------------------------------*/
VSAABB3::VSAABB3(const VSVector3 & Max , const VSVector3 & Min)
{
	Set(Max,Min);
}
/*----------------------------------------------------------------*/
VSAABB3::VSAABB3(const VSVector3 & Center,VSREAL fA0,VSREAL fA1,VSREAL fA2)
{
	Set(Center,fA0,fA1,fA2);
}
/*----------------------------------------------------------------*/
VSAABB3::VSAABB3(const VSVector3 & Center,VSREAL fA[3])
{
	Set(Center,fA);
}
/*----------------------------------------------------------------*/
void VSAABB3::GetPlane(VSPlane3 pPlanes[6])const
{
	VSVector3 vcN;

	// 右面
	vcN = m_A[0];
	pPlanes[0].Set(vcN, m_Max);

	// 左面
	vcN = m_A[0] * (-1.0f);
	pPlanes[1].Set(vcN, m_Min);

	// 前面
	vcN = m_A[2] * (-1.0f);
	pPlanes[2].Set(vcN, m_Min);

	// 后面
	vcN = m_A[2];
	pPlanes[3].Set(vcN, m_Max);

	// 上面
	vcN = m_A[1];
	pPlanes[4].Set(vcN, m_Max);

	// 下面
	vcN = m_A[1] * (-1.0f);
	pPlanes[5].Set(vcN, m_Min);
}
/*----------------------------------------------------------------*/
void VSAABB3::GetPoint(VSVector3 Point[8])const
{
	Point[0].Set(m_Center.x + m_fA[0], m_Center.y + m_fA[1] ,m_Center.z + m_fA[2]);
	Point[1].Set(m_Center.x - m_fA[0], m_Center.y + m_fA[1] ,m_Center.z + m_fA[2]);
	Point[2].Set(m_Center.x - m_fA[0], m_Center.y - m_fA[1] ,m_Center.z + m_fA[2]);
	Point[3].Set(m_Center.x + m_fA[0], m_Center.y - m_fA[1] ,m_Center.z + m_fA[2]);
	Point[4].Set(m_Center.x + m_fA[0], m_Center.y + m_fA[1] ,m_Center.z - m_fA[2]);
	Point[5].Set(m_Center.x - m_fA[0], m_Center.y + m_fA[1] ,m_Center.z - m_fA[2]);
	Point[6].Set(m_Center.x - m_fA[0], m_Center.y - m_fA[1] ,m_Center.z - m_fA[2]);
	Point[7].Set(m_Center.x + m_fA[0], m_Center.y - m_fA[1] ,m_Center.z - m_fA[2]);
}
/*----------------------------------------------------------------*/
void VSAABB3::GetRectangle(VSRectangle3 Rectangle[6])const
{
	VSVector3 Center[6];
	Center[0].Set(m_Center.x + m_fA[0],m_Center.y,m_Center.z);
	Center[1].Set(m_Center.x - m_fA[0],m_Center.y,m_Center.z);
	Center[2].Set(m_Center.x,m_Center.y + m_fA[1],m_Center.z);
	Center[3].Set(m_Center.x,m_Center.y - m_fA[1],m_Center.z);
	Center[4].Set(m_Center.x,m_Center.y,m_Center.z + m_fA[2]);
	Center[5].Set(m_Center.x,m_Center.y,m_Center.z - m_fA[2]);
	//right
	Rectangle[0].Set(Center[0],m_A[1],m_A[2],m_fA[1],m_fA[2]);
	//left
	Rectangle[1].Set(Center[1],m_A[1],m_A[2] * (-1),m_fA[1],m_fA[2]);

	//up
	Rectangle[2].Set(Center[4],m_A[2],m_A[0],m_fA[2],m_fA[1]);
	//down
	Rectangle[3].Set(Center[5],m_A[2],m_A[0] * (-1),m_fA[2],m_fA[0]);

	//front
	Rectangle[4].Set(Center[2],m_A[1],m_A[0] * (-1),m_fA[1],m_fA[0]);
	//back    
	Rectangle[5].Set(Center[3],m_A[1],m_A[0],m_fA[1],m_fA[0]);


}
/*----------------------------------------------------------------*/
bool VSAABB3::GetParameter(const VSVector3 &Point,VSREAL fAABBParameter[3])const
{
	for(int i = 0 ; i < 3 ; i++)
	{
		fAABBParameter[i] = Point.m[i] - m_Center.m[i];
	
	}
	for(int i = 0 ; i < 3 ; i++)
		if(ABS(fAABBParameter[i]) > m_fA[i])
			return 0;
	
	return 1;
}
/*----------------------------------------------------------------*/
void VSAABB3::Transform(const VSAABB3 &AABB, 
						const VSMatrix3X3W &m,bool bHasProject)
{
	if (bHasProject == false)
	{
		m_Max = m_Min = m.GetTranslation();
		if(m._00 > 0.0f)
		{
			m_Min.x += m._00 * AABB.m_Min.x;
			m_Max.x += m._00 * AABB.m_Max.x;

		}
		else
		{
			m_Min.x += m._00 * AABB.m_Max.x;
			m_Max.x += m._00 * AABB.m_Min.x;
		}

		if(m._01 > 0.0f)
		{
			m_Min.y += m._01 * AABB.m_Min.x;
			m_Max.y += m._01 * AABB.m_Max.x;

		}
		else
		{
			m_Min.y += m._01 * AABB.m_Max.x;
			m_Max.y += m._01 * AABB.m_Min.x;
		}

		if(m._02 > 0.0f)
		{
			m_Min.z += m._02 * AABB.m_Min.x;
			m_Max.z += m._02 * AABB.m_Max.x;

		}
		else
		{
			m_Min.z += m._02 * AABB.m_Max.x;
			m_Max.z += m._02 * AABB.m_Min.x;
		}

		if(m._10 > 0.0f)
		{
			m_Min.x += m._10 * AABB.m_Min.y;
			m_Max.x += m._10 * AABB.m_Max.y;

		}
		else
		{
			m_Min.x += m._10 * AABB.m_Max.y;
			m_Max.x += m._10 * AABB.m_Min.y;
		}

		if(m._11 > 0.0f)
		{
			m_Min.y += m._11 * AABB.m_Min.y;
			m_Max.y += m._11 * AABB.m_Max.y;

		}
		else
		{
			m_Min.y += m._11 * AABB.m_Max.y;
			m_Max.y += m._11 * AABB.m_Min.y;
		}

		if(m._12 > 0.0f)
		{
			m_Min.z += m._12 * AABB.m_Min.y;
			m_Max.z += m._12 * AABB.m_Max.y;

		}
		else
		{
			m_Min.z += m._12 * AABB.m_Max.y;
			m_Max.z += m._12 * AABB.m_Min.y;
		}

		if(m._20 > 0.0f)
		{
			m_Min.x += m._20 * AABB.m_Min.z;
			m_Max.x += m._20 * AABB.m_Max.z;

		}
		else
		{
			m_Min.x += m._20 * AABB.m_Max.z;
			m_Max.x += m._20 * AABB.m_Min.z;
		}

		if(m._21 > 0.0f)
		{
			m_Min.y += m._21 * AABB.m_Min.z;
			m_Max.y += m._21 * AABB.m_Max.z;

		}
		else
		{
			m_Min.y += m._21 * AABB.m_Max.z;
			m_Max.y += m._21 * AABB.m_Min.z;
		}

		if(m._22 > 0.0f)
		{
			m_Min.z += m._22 * AABB.m_Min.z;
			m_Max.z += m._22 * AABB.m_Max.z;

		}
		else
		{
			m_Min.z += m._22 * AABB.m_Max.z;
			m_Max.z += m._22 * AABB.m_Min.z;
		}
	}
	else
	{
		
		VSVector3 Point[8];
		AABB.GetPoint(Point);

		m_Max = VSVector3(-VSMAX_REAL ,-VSMAX_REAL ,-VSMAX_REAL);
		m_Min = VSVector3(VSMAX_REAL ,VSMAX_REAL ,VSMAX_REAL);
		for (unsigned int i = 0 ; i < 8 ; i++)
		{
			VSVector3 New = Point[i] * m;
			
			m_Min.x = Min(m_Min.x, New.x);
			m_Min.y = Min(m_Min.y, New.y);
			m_Min.z = Min(m_Min.z, New.z);
			m_Max.x = Max(m_Max.x, New.x);
			m_Max.y = Max(m_Max.y, New.y);
			m_Max.z = Max(m_Max.z, New.z);
		}
	}
	Set(m_Max,m_Min);
}
/*----------------------------------------------------------------*/
VSAABB3 VSAABB3::MergAABB(const VSAABB3 &AABB)const
{
	VSAABB3 Temp;
	Temp.m_Max = m_Max;
	Temp.m_Min = m_Min;
	if(Temp.m_Min.x > AABB.m_Min.x)
	{
		Temp.m_Min.x = AABB.m_Min.x;
	}
	if(Temp.m_Min.y > AABB.m_Min.y)
	{
		Temp.m_Min.y = AABB.m_Min.y;
	}
	if(Temp.m_Min.z > AABB.m_Min.z)
	{
		Temp.m_Min.z = AABB.m_Min.z;
	}

	if(Temp.m_Max.x < AABB.m_Max.x)
	{
		Temp.m_Max.x = AABB.m_Max.x;
	}
	if(Temp.m_Max.y < AABB.m_Max.y)
	{
		Temp.m_Max.y = AABB.m_Max.y;
	}
	if(Temp.m_Max.z < AABB.m_Max.z)
	{
		Temp.m_Max.z = AABB.m_Max.z;
	}
	Temp.Set(Temp.m_Max,Temp.m_Min);
	return Temp;
}
/*----------------------------------------------------------------*/
void VSAABB3::CreateAABB(const VSVector3 * const pPointArray,unsigned int uiPointNum)
{
	if(!pPointArray || !uiPointNum)
		return;
	m_Min = pPointArray[0]; 
	m_Max = pPointArray[0];
	for (unsigned int i = 1; i < uiPointNum; i++)
	{
		if (pPointArray[i].x < m_Min.x)
		{
			m_Min.x = pPointArray[i].x;
		}
		else if (pPointArray[i].x > m_Max.x)
		{
			m_Max.x = pPointArray[i].x;
		}

		if (pPointArray[i].y < m_Min.y)
		{
			m_Min.y = pPointArray[i].y;
		}
		else if (pPointArray[i].y > m_Max.y)
		{
			m_Max.y = pPointArray[i].y;
		}

		if (pPointArray[i].z < m_Min.z)
		{
			m_Min.z = pPointArray[i].z;
		}
		else if (pPointArray[i].z > m_Max.z)
		{
			m_Max.z = pPointArray[i].z;
		}
	}
	m_Center = (m_Min + m_Max)/2.0f;

	VSVector3 Temp = (m_Max - m_Min)/2.0f;

	m_fA[0] = Temp.x;
	m_fA[1] = Temp.y;
	m_fA[2] = Temp.z;
	

} 
void VSAABB3::GetQuadAABB(VSAABB3 AABB[4])const
{
	VSVector3 Point[8];
	GetPoint(Point);
	VSVector3 Mid[4];
	Mid[0] = (m_Center + Point[0]) * 0.5;
	Mid[1] = (m_Center + Point[1]) * 0.5;
	Mid[2] = (m_Center + Point[4]) * 0.5;
	Mid[3] = (m_Center + Point[5]) * 0.5;
	VSREAL fA[3] = {m_fA[0] * 0.5f , m_fA[1], m_fA[2] * 0.5f};
	for (unsigned int i = 0 ; i < 4 ; i++)
	{
		Mid[i].y = m_Center.y;
		AABB[i].Set(Mid[i],fA);
	}
	
}
VSAABB3 VSAABB3::GetMin(const VSAABB3 & AABB) const
{
	VSAABB3 Temp;
	Temp.m_Max = m_Max;
	Temp.m_Min = m_Min;
	if (Temp.m_Min.x < AABB.m_Min.x)
	{
		Temp.m_Min.x = AABB.m_Min.x;
	}
	if (Temp.m_Min.y < AABB.m_Min.y)
	{
		Temp.m_Min.y = AABB.m_Min.y;
	}
	if (Temp.m_Min.z < AABB.m_Min.z)
	{
		Temp.m_Min.z = AABB.m_Min.z;
	}

	if (Temp.m_Max.x > AABB.m_Max.x)
	{
		Temp.m_Max.x = AABB.m_Max.x;
	}
	if (Temp.m_Max.y > AABB.m_Max.y)
	{
		Temp.m_Max.y = AABB.m_Max.y;
	}
	if (Temp.m_Max.z > AABB.m_Max.z)
	{
		Temp.m_Max.z = AABB.m_Max.z;
	}
	Temp.Set(Temp.m_Max, Temp.m_Min);
	return Temp;
}
void VSAABB3::GetOctAABB(VSAABB3 AABB[8])const
{
	VSVector3 Point[8];
	GetPoint(Point);
	VSVector3 Mid[8];

	VSREAL fA[3] = {m_fA[0] * 0.5f , m_fA[1] * 0.5f , m_fA[2] * 0.5f};
	for (unsigned int i = 0 ; i < 8 ; i++)
	{
		Mid[i] = (m_Center + Point[i]) * 0.5;
		AABB[i].Set(Mid[i],fA);
	}

}