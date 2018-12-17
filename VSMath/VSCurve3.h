#ifndef VSCURVE3_H
#define VSCURVE3_H
#include "VSVector3.h"
#include "VSMatrix3X3.h"
/*
������

*/
namespace VSEngine2
{
	class VSMATH_API VSCurve3 : public VSMemObject
	{
	public:
		enum //CURVE TYPE
		{
			CT_SEGMENT,
			CT_CONTROL,
			CT_MAX
		};
		VSCurve3();
		virtual ~VSCurve3() = 0;

		virtual unsigned int GetCurveType() = 0;
		//���FrenetFrame3������
		virtual void GetFrenetFrameBNT(VSREAL t,VSVector3 & B,VSVector3 & N,VSVector3 &T) = 0;
		//���FixedUp3������
		void	GetFixedUpBNT(VSREAL t ,VSVector3 & B,VSVector3 & N,VSVector3& T);
		//����
		virtual VSVector3	GetFirstDerivative(VSREAL t)= 0 ;
		virtual VSVector3	GetSecondDerivative(VSREAL t)= 0;
		virtual VSVector3	GetThirdDerivative(VSREAL t)= 0;
		//�󳤶�
		virtual VSREAL		GetLength(VSREAL t1 ,VSREAL t2,unsigned int uiIterations)= 0;
		//���
		virtual VSVector3	GetPoint(VSREAL t) = 0;
		//���ܳ�
		virtual VSREAL		GetTotalLength(unsigned int uiIterations) = 0;
		//���ݳ��ȵõ�ʱ��
		virtual VSREAL GetTime (VSREAL fLength, unsigned int iIterations = 32,
				VSREAL fTolerance = EPSILON_E4) = 0;
		//������
		virtual VSREAL GetCurvature(VSREAL fTime) = 0;
		//��ת��
		virtual VSREAL GetTorsion(VSREAL fTime) = 0;
	
	};
}
#endif