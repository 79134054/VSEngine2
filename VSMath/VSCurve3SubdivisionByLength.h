#ifndef VSCURVESUBDIVISIONBYLENGTH_H
#define VSCURVESUBDIVISIONBYLENGTH_H
#include "VSCurve3Subdivision.h"
/*
长度划分方法



*/
namespace VSEngine2
{
class VSMATH_API VSCurve3SubdivisionByLength :public VSCurve3Subdivision
{
public:
	VSCurve3SubdivisionByLength(unsigned int PointNum,VSREAL t1,VSREAL t2,unsigned int uiIterations = 32);
	virtual ~VSCurve3SubdivisionByLength();
	//给定划分点个数和迭代次数，划分曲线按长度l1,l2之间
	virtual bool Subdivision();
	void SetPointNum(unsigned int uiPointNum);
	virtual unsigned int GetSubdivisionMethod(){ return SM_LENGTH;}
protected:
	unsigned int m_uiIterations;
};


}

#endif