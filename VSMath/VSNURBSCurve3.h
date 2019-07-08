#ifndef VSNURBSCURVE3_H
#define VSNURBSCURVE3_H
#include "VSB_SplineCurve3.h"
/*
NURB曲线类
*/
namespace VSEngine2
{
class VSMATH_API VSNURBSCurve3 : public VSB_SplineCurve3
{
public:
	VSNURBSCurve3();
	virtual ~VSNURBSCurve3();
	//设置权重
	bool Set(const VSREAL * pWeight,unsigned int uiControlPointNum);
	//拷贝
	bool Copy(const VSNURBSCurve3 &NURBSCurve3);
	/***************************************FORCEINLINE************************************/
	//改变权重
	FORCEINLINE bool ChangeWeight(unsigned int i,VSREAL Weight);
	//获取权重指针
	FORCEINLINE const VSREAL * GetWeight()const;
	//得到权重
	FORCEINLINE VSREAL GetWeight(unsigned int i)const;
	//取得导数
	virtual VSVector3	GetFirstDerivative(VSREAL t);
	virtual VSVector3	GetSecondDerivative(VSREAL t);
	virtual VSVector3	GetThirdDerivative(VSREAL t);
	//virtual VSREAL		GetLength(VSREAL t1 ,VSREAL t2);
	//得到点
	virtual VSVector3	GetPoint(VSREAL t);
protected:
	bool Get(unsigned int Order,VSREAL t ,VSVector3 *D0,VSVector3 *D1,VSVector3 *D2,VSVector3 *D3);
private:
	VSREAL *m_pWeight;

};
#include "VSNURBSCurve3.inl"
}
#endif