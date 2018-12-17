#ifndef VSCURVESUBDIVISIONBYERROR_H
#define VSCURVESUBDIVISIONBYERROR_H
#include "VSCurve3Subdivision.h"
/*
	���ַ���



*/
namespace VSEngine2
{
	class VSMATH_API VSCurve3SubdivisionByError :public VSCurve3Subdivision
	{
	public:
		VSCurve3SubdivisionByError(VSREAL t1 = 0.0f,VSREAL t2 = 1.0f,
									unsigned int Level = 5, VSREAL error = EPSILON_E4);
		~VSCurve3SubdivisionByError();
		//ָ��ݹ��κ���� ��������t1,t2֮��
		virtual bool Subdivision();
		

		virtual unsigned int GetSubdivisionMethod(){ return SM_ERROR;}
	protected:
		typedef struct  POINTLIST
		{
			VSVector3 Point;
			POINTLIST * next;
			VSREAL	  t;
		};
		POINTLIST * m_pPointHead;
		POINTLIST * m_pNext;
		bool SubdivisionBisect(unsigned int Level, VSREAL error,VSREAL t1,VSREAL t2,
							const VSVector3 &Point1,const VSVector3 &Point2);
		void Clear();
		unsigned int m_uiLevel;
		VSREAL m_fError;
	};


}
#endif