#ifndef VSGENERALIZEDCYLINDERSURFACE3_H
#define VSGENERALIZEDCYLINDERSURFACE3_H
#include "VSCurve3.h"
#include "VSSurface3.h"
/*
	Ò»°ãÖùÐÎÇúÃæ

*/
namespace VSEngine2
{
	class VSMATH_API VSGeneralizedCylinderSurface3 :public VSSurface3
	{
	public:

		VSGeneralizedCylinderSurface3();
		~VSGeneralizedCylinderSurface3();
		bool SetCurve(VSCurve3 * pCurve1,VSCurve3 *pCurve2);
		virtual VSVector3 GetPoint(VSREAL U,VSREAL V);
		virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V);
		virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V);
	private:
		VSCurve3 *m_pCurve1;
		VSCurve3 *m_pCurve2;
	
	};


}
#endif