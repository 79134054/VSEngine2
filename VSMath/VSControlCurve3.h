#ifndef VSCONTROLCURVE3_H
#define VSCONTROLCURVE3_H
#include "VSCurve3.h"
/*
���Ƶ������࣬���еĿ��Ƶ��������߶�����������û��ʵ��

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
	//����
	bool Copy(const VSControlCurve3 & ControlCurve3);
	/***************************************FORCEINLINE************************************/
	//�õ����Ƶ�ָ��
	FORCEINLINE const VSVector3 * GetpControlPoint()const;
	//���ĳ�����Ƶ�
	FORCEINLINE VSVector3 GetControlPoint(unsigned int i)const;
	//�ı�ĳ�����Ƶ�
	FORCEINLINE bool ChangeControlPoint(unsigned int i,const VSVector3 & ControlPoint);
	//�õ����Ƶ����
	FORCEINLINE unsigned int GetControlPointNum()const;
	//�õ���������
	FORCEINLINE unsigned int GetType()const;
	//���FRENET3������
	virtual void	GetFrenetFrameBNT(VSREAL t,VSVector3 & B,VSVector3 & N,VSVector3 &T);
	//����
	virtual VSVector3	GetFirstDerivative(VSREAL t)= 0 ;
	virtual VSVector3	GetSecondDerivative(VSREAL t)= 0;
	virtual VSVector3	GetThirdDerivative(VSREAL t)= 0;
	//�󳤶�
	virtual VSREAL		GetLength(VSREAL t1 ,VSREAL t2,unsigned int uiIterations = 32);
	//�õ���
	virtual VSVector3	GetPoint(VSREAL t) = 0;
	//�õ��ܳ���
	virtual VSREAL		GetTotalLength(unsigned int uiIterations);
	//���ݳ�����ʱ��
	virtual VSREAL GetTime (VSREAL fLength, unsigned int uiIterations = 32,
						VSREAL fTolerance = EPSILON_E4);
	//�õ�����
	virtual VSREAL GetCurvature(VSREAL fTime);
	//�õ�ת��
	virtual VSREAL GetTorsion(VSREAL fTime);
protected:
	unsigned int m_CurveType;
	VSVector3 * m_pControlPoint;
	unsigned int m_uiControlPointNum;
};
#include "VSControlCurve3.inl"
}
#endif