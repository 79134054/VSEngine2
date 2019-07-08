#ifndef VSCYLINDERSURFACE3_H
#define VSCYLINDERSURFACE3_H
#include "VSSurface3.h"
#include "VSSegmentCurve3.h"
/*
	ÖùÐÎÇúÃæ

*/
namespace VSEngine2
{
class VSMATH_API VSCylinderSurface3:public VSSurface3
{
public:

	VSCylinderSurface3();
	~VSCylinderSurface3();
	bool SetCurve(VSCurve3 * pCurve1,VSSegmentCurve3 *pCurve2);
	virtual VSVector3 GetPoint(VSREAL U,VSREAL V);
	virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V);
	virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V);
private:
	VSCurve3 *m_pCurve1;
	VSSegmentCurve3 *m_pCurve2;



};


}
#endif