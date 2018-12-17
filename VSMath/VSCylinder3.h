#ifndef VSCYLINDER3
#define VSCYLINDER3
#include "VSVector3.h"
namespace VSEngine2
{
	class VSMATH_API VSCylinder3
	{
	public: 
		VSCylinder3(VSVector3 Center,VSVector3 Dir,VSREAL fRadius, VSREAL fHalfHeight);
		~VSCylinder3();
		VSVector3	m_Center;
		VSVector3	m_Dir;
		VSREAL		m_fRadius;
		VSREAL		m_fHalfHeight;
	};
}

#endif