#ifndef VSCONTROLCURVE3_H
#define VSCONTROLCURVE3_H
#include "VSCurve3.h"
/*
控制点曲线类，所有的控制点类型曲线都集成于它，没有实例

*/
namespace VSEngine2
{
class VSMATH_API VSControlCurve3:public VSCurve3
{
public:
	enum {B_SPLINE,NURBS,BEZIER};
	VSControlCurve3();
	virtual ~VSControlCurve3() = 0;
	virtual unsigned int GetCurveType(){return CT_CONTROL;}
	//拷贝
	bool Copy(const VSControlCurve3 & ControlCurve3);
	/***************************************FORCEINLINE************************************/
	//得到控制点指针
	FORCEINLINE const VSVector3 * GetpControlPoint()const;
	//获得某个控制点
	FORCEINLINE VSVector3 GetControlPoint(unsigned int i)const;
	//改变某个控制点
	FORCEINLINE bool ChangeControlPoint(unsigned int i,const VSVector3 & ControlPoint);
	//得到控制点个数
	FORCEINLINE unsigned int GetControlPointNum()const;
	//得到曲线类型
	FORCEINLINE unsigned int GetType()const;
	//获得FRENET3个方向
	virtual void	GetFrenetFrameBNT(VSREAL t,VSVector3 & B,VSVector3 & N,VSVector3 &T);
	//求导数
	virtual VSVector3	GetFirstDerivative(VSREAL t)= 0 ;
	virtual VSVector3	GetSecondDerivative(VSREAL t)= 0;
	virtual VSVector3	GetThirdDerivative(VSREAL t)= 0;
	//求长度
	virtual VSREAL		GetLength(VSREAL t1 ,VSREAL t2,unsigned int uiIterations = 32);
	//得到点
	virtual VSVector3	GetPoint(VSREAL t) = 0;
	//得到总长度
	virtual VSREAL		GetTotalLength(unsigned int uiIterations);
	//根据长度求时间
	virtual VSREAL GetTime (VSREAL fLength, unsigned int uiIterations = 32,
						VSREAL fTolerance = EPSILON_E4);
	//得到曲率
	virtual VSREAL GetCurvature(VSREAL fTime);
	//得到转矩
	virtual VSREAL GetTorsion(VSREAL fTime);
protected:
	unsigned int m_CurveType;
	VSVector3 * m_pControlPoint;
	unsigned int m_uiControlPointNum;
};
#include "VSControlCurve3.inl"
}
#endif