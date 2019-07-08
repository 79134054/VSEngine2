#ifndef VSSEGMENTCURVE3_H
#define VSSEGMENTCURVE3_H
#include "VSCurve3.h"
#include "VSSegment3.h"
/*
ÇúÏß¶Î

*/
namespace VSEngine2
{
	class VSMATH_API VSSegmentCurve3:public VSCurve3
	{
	public:
		VSSegmentCurve3();
		~VSSegmentCurve3();
		void Set(const VSSegment3 &Segment);
		VSSegment3 & GetSegment();
		virtual unsigned int GetCurveType(){return CT_SEGMENT;}
		virtual void GetFrenetFrameBNT(VSREAL t,VSVector3 & B,VSVector3 & N,VSVector3 &T);
		FORCEINLINE virtual VSVector3	GetFirstDerivative(VSREAL t);
		FORCEINLINE virtual VSVector3	GetSecondDerivative(VSREAL t);
		FORCEINLINE virtual VSVector3	GetThirdDerivative(VSREAL t);
		FORCEINLINE virtual VSREAL		GetLength(VSREAL t1 ,VSREAL t2,unsigned int uiIterations);
		FORCEINLINE virtual VSVector3	GetPoint(VSREAL t);
		FORCEINLINE virtual VSREAL		GetTotalLength(unsigned int uiIterations);
		FORCEINLINE virtual VSREAL GetTime (VSREAL fLength, unsigned int uiIterations = 32,
										VSREAL fTolerance = EPSILON_E4);
		FORCEINLINE virtual VSREAL GetCurvature(VSREAL fTime);
		FORCEINLINE virtual VSREAL GetTorsion(VSREAL fTime);

	private:
		VSSegment3 m_Segment;
	};

#include "VSSegmentCurve3.inl"
}
#endif