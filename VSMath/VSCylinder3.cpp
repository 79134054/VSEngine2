#include "VSCylinder3.h"
using namespace VSEngine2;
VSCylinder3::VSCylinder3(VSVector3 Center,VSVector3 Dir,VSREAL fRadius, VSREAL fHalfHeight)
{
	m_Center = Center;
	m_Dir = Dir;
	m_fRadius = fRadius;
	m_fHalfHeight = fHalfHeight;
}
VSCylinder3::~VSCylinder3()
{

}