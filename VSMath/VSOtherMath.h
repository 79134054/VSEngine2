#ifndef VSOTHERMATH_H
#define VSOTHERMATH_H
#include "VSVector3.h"
#include "VSMatrix3X3.h"
#include "VSQuat.h"
#include "VSVector3W.h"
namespace VSEngine2
{
	bool VSMATH_API IsUniformScale(VSREAL fScale);
	bool VSMATH_API IsZeroTranslate(const VSVector3 & Translate);
	bool VSMATH_API IsIdentityRotate(const VSMatrix3X3 & Rotate);
	bool VSMATH_API IsIdentityRotate(const VSQuat & Rotate);

// 	template< typename T>
// 	T LineInterpolation (T t1, T t2, VSREAL t)
// 	{
// 		return t1 + (t2 - t1) * t;
// 	}
// 	VSQuat LineInterpolation (VSQuat t1, VSQuat t2, VSREAL t)
// 	{
// 		VSREAL fCos = t2.Dot(t1);
// 		if(fCos < 0.0f)
// 		{
// 			t1 *= -1.0f;
// 		}
// 		return t1 + (t2 - t1) * t;
// 	}
	VSREAL VSMATH_API LineInterpolation (VSREAL t1, VSREAL t2, VSREAL t);

	VSVector3 VSMATH_API LineInterpolation (const VSVector3& t1, const VSVector3& t2, VSREAL t);

	VSQuat VSMATH_API LineInterpolation(const VSQuat& t1, const VSQuat& t2, VSREAL t);

	VSVector3W VSMATH_API LineInterpolation(const VSVector3W& t1, const VSVector3W& t2, VSREAL t);
}
#endif
