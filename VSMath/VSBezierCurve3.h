#ifndef VSBEZIERCURVE3_H
#define VSBEZIERCURVE3_H
#include "VSControlCurve3.h"
/*
Bezier曲线，参数t在0-1之间由控制点和多项式组成

*/
namespace VSEngine2
{
class VSMATH_API VSBezierCurve3 : public  VSControlCurve3
{
public:
	VSBezierCurve3();
	virtual ~VSBezierCurve3();
	//设置控制点
	bool Set(const VSVector3 * pControlPoint,unsigned int uiControlPointNum);
	//拷贝
	bool Copy(const VSBezierCurve3 & BezierCurve3);
	//求导
	virtual VSVector3	GetFirstDerivative(VSREAL t);
	virtual VSVector3	GetSecondDerivative(VSREAL t);
	virtual VSVector3	GetThirdDerivative(VSREAL t);
	//virtual VSREAL		GetLength(VSREAL t1 ,VSREAL t2);
	//求点
	virtual VSVector3	GetPoint(VSREAL t);
private:
	FORCEINLINE VSREAL GetC(int i,int j)const;
	FORCEINLINE VSREAL SetC(int i,int j,VSREAL Value);

	VSREAL * m_C;
	VSVector3 * m_FDValue;
	VSVector3 * m_SDValue;
	VSVector3 * m_TDValue;

};
#include "VSBezierCurve3.inl"

}
#endif