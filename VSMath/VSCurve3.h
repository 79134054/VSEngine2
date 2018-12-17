#ifndef VSCURVE3_H
#define VSCURVE3_H
#include "VSVector3.h"
#include "VSMatrix3X3.h"
/*
曲线类

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
		//获得FrenetFrame3个方向
		virtual void GetFrenetFrameBNT(VSREAL t,VSVector3 & B,VSVector3 & N,VSVector3 &T) = 0;
		//获得FixedUp3个方向
		void	GetFixedUpBNT(VSREAL t ,VSVector3 & B,VSVector3 & N,VSVector3& T);
		//求导数
		virtual VSVector3	GetFirstDerivative(VSREAL t)= 0 ;
		virtual VSVector3	GetSecondDerivative(VSREAL t)= 0;
		virtual VSVector3	GetThirdDerivative(VSREAL t)= 0;
		//求长度
		virtual VSREAL		GetLength(VSREAL t1 ,VSREAL t2,unsigned int uiIterations)= 0;
		//求点
		virtual VSVector3	GetPoint(VSREAL t) = 0;
		//求总长
		virtual VSREAL		GetTotalLength(unsigned int uiIterations) = 0;
		//根据长度得到时间
		virtual VSREAL GetTime (VSREAL fLength, unsigned int iIterations = 32,
				VSREAL fTolerance = EPSILON_E4) = 0;
		//求曲率
		virtual VSREAL GetCurvature(VSREAL fTime) = 0;
		//求转矩
		virtual VSREAL GetTorsion(VSREAL fTime) = 0;
	
	};
}
#endif