#ifndef VSFASTFUNCTION_H
#define VSFASTFUNCTION_H
#include "VSMath.h"
namespace VSEngine2
{
	class VSMatrix3X3W;
	//sse fast function	
	VSMATH_API void VSFastMul(const VSMatrix3X3W & InM1,const VSMatrix3X3W & InM2,VSMatrix3X3W & OutM);
	VSMATH_API void VSFastAdd(const VSMatrix3X3W & InM1,const VSMatrix3X3W & InM2,VSMatrix3X3W & OutM);
	VSMATH_API void VSFastSub(const VSMatrix3X3W & InM1,const VSMatrix3X3W & InM2,VSMatrix3X3W & OutM);
	VSMATH_API void VSFastMul(const VSMatrix3X3W & InM1,VSREAL f,VSMatrix3X3W & OutM);
	VSMATH_API void VSFastLerp(const VSMatrix3X3W & InM1,const VSMatrix3X3W & InM2,VSREAL f,VSMatrix3X3W & OutM);

	class VSVector2;
	class VSVector3;
	class VSVector3W;


	VSMATH_API VSREAL VSFastLength(const VSVector2 &vec);
	VSMATH_API VSREAL VSFastLength(const VSVector3 &vec);
	VSMATH_API VSREAL VSFastLength(const VSVector3W &vec);


	VSMATH_API void VSFastCross(const VSVector3 &InV1, const VSVector3 &InV2,VSVector3 &OutV);

	VSMATH_API void VSFastNormalize(const VSVector2 &InV,VSVector2 &OutV);
	VSMATH_API void VSFastNormalize(const VSVector3 &InV,VSVector3 &OutV);
	VSMATH_API void VSFastNormalize(const VSVector3W &InV,VSVector3W &OutV);
}
#endif

