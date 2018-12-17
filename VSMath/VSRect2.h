#ifndef VSRECT2_H
#define VSRECT2_H
#include "VSMath.h"
#include "VSVector2.h"
namespace VSEngine2
{
	class VSMATH_API VSRect2
	{
	public:
		VSVector2 m_Min;
		VSVector2 m_Max;
		VSRect2(void);
		VSRect2(const VSVector2& Min, const VSVector2 &Max);

	};


}
#endif