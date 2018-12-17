#ifndef  VSCURVESUBDIVISIONBYUNIFORM_H
#define  VSCURVESUBDIVISIONBYUNIFORM_H
#include "VSCurve3Subdivision.h"
/*
均匀划分法



*/
namespace VSEngine2
{
class VSMATH_API VSCurve3SubdivisionByUniform :public VSCurve3Subdivision
{
public:
	VSCurve3SubdivisionByUniform(unsigned int PointNum,VSREAL t1 = 0.0f,VSREAL t2 = 1.0f);
	virtual ~VSCurve3SubdivisionByUniform();
	//给定点的个数划分曲线 t1,t2之间
	virtual bool Subdivision();
	void SetPointNum(unsigned int uiPointNum);
	virtual unsigned int GetSubdivisionMethod(){ return SM_UNIFORM;}
};


}
#endif