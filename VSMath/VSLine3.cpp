#include "VSLine3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSLine3::VSLine3()
{


}
/*----------------------------------------------------------------*/
VSLine3::VSLine3(const VSVector3 & Orig,const VSVector3 &Dir)
{
	Set(Orig,Dir);

}
/*----------------------------------------------------------------*/
VSLine3::~VSLine3()
{


}
/*----------------------------------------------------------------*/
bool VSLine3::GetParameter(const VSVector3 &Point,VSREAL &fLineParameter )const
{
	VSVector3 Temp = Point - m_Orig;
	if(Temp.IsParallel(m_Dir))
	{
		fLineParameter = Temp.GetLength();
		return 1;
	}
	else
		return 0;
}
/*----------------------------------------------------------------*/
void VSLine3::Transform(const VSLine3 &Line,const VSMatrix3X3W &Mat)
{
	m_Dir = Mat.Apply3X3(Line.m_Dir);
	m_Dir.Normalize();
	m_Orig = Line.m_Orig * Mat;

}