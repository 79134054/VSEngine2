#ifndef VSCURVESET3_H
#define VSCURVESET3_H
#include "VSCurve3.h"
#include "VSSegment3.h"
/*
	ÇúÏß¼¯ºÏ


*/
namespace VSEngine2
{
	class VSMATH_API VSCurveSet3 : public VSMemObject
	{
	public:
		VSCurveSet3();
		~VSCurveSet3();
		void Clear();
		bool Set(VSCurve3 * pCurve);
		bool	GetFrenetFrameBNT(VSREAL t,VSVector3 & B,VSVector3 & N,VSVector3 &T);
		bool	GetFixedUpBNT(VSREAL t ,VSVector3 & B,VSVector3 & N,VSVector3& T);
		VSVector3	GetFirstDerivative(VSREAL t);
		VSVector3	GetSecondDerivative(VSREAL t);
		VSVector3	GetThirdDerivative(VSREAL t);
		VSREAL		GetLength(VSREAL t1 ,VSREAL t2,unsigned int uiIterations);
		VSVector3	GetPoint(VSREAL t);
		VSREAL		GetTotalLength(unsigned int uiIterations);
		VSREAL GetTime (VSREAL fLength, int iIterations = 32,
						VSREAL fTolerance = EPSILON_E4);
		unsigned int GetIndex(VSREAL &t);
		bool GetElement(unsigned int Index,VSCurve3 * &Element );
		bool GetElement(VSREAL &t,VSCurve3 * &Element);

		FORCEINLINE unsigned int GetCurveNum()const{return m_uiNum;}
	protected:
		typedef struct ELEMENT
		{
			VSCurve3 * Ptr;
			ELEMENT * next;
		};
		ELEMENT * m_pHead;
		ELEMENT * m_pNext;
		unsigned int m_uiNum;
	};


}
#endif