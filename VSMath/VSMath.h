#ifndef VSENGINE_MATH_H
#define VSENGINE_MATH_H
#include "VSSystem.h"
#include <windows.h> 
#include <math.h>  
#include <xmmintrin.h>
#include <emmintrin.h>
#include "VSMemManager.h"
#ifdef VSMATH_EXPORTS
	#define  VSMATH_API __declspec(dllexport) 
#else
	#define  VSMATH_API __declspec(dllimport)
#endif
namespace VSEngine2
{
#define  VSFLOAT 
#ifdef VSFLOAT 
#define VSREAL 		float
#define SIN(x)		sinf(x)
#define COS(x)		cosf(x)
#define TAN(x)		tanf(x)
#define ASIN(x)		asinf(x)
#define ACOS(x)		acosf(x)
#define ATAN(x)		atanf(x)
#define ATAN2(y,x)	atan2f(y,x)
#define SQRT(x)		sqrtf(x)
#define POW(x,y)	powf(x,y)
#define CEIL(x)		ceilf(x);
#define EXP(x)		expf(x);
#define FMOD(x,y)	fmodf(x,y)
#else ifdef VSDOUBLE
#define VSREAL 		double
#define SIN(x)		sin(x)
#define COS(x)		cos(x)
#define TAN(x)		tan(x)
#define ASIN(x)		asin(x)
#define ACOS(x)		acos(x)
#define ATAN(x) 	atan(x)
#define ATAN2(y,x) 	atan2(y,x)
#define SQRT(x)		sqrt(x)
#define POW(x,y)	pow(x,y)
#define CEIL(x)		ceil(x);
#define EXP(x)		exp(x);
#define FMOD(x,y)	fmod(x,y)
#endif

#define EPSILON_E3 (VSREAL)(1E-3) 
#define EPSILON_E4 (VSREAL)(1E-4) 
#define EPSILON_E5 (VSREAL)(1E-5)
#define EPSILON_E6 (VSREAL)(1E-6)


#define VSFRONT			0
#define VSBACK			1
#define VSON			2

#define VSCLIPPED		3
#define VSCULLED		4
#define VSVISIBLE		5

#define VSINTERSECT		3
#define VSOUT			4
#define VSIN			5

#define VSNOINTERSECT	6

#define VSMIN_CHAR		(-128)      
#define VSMAX_CHAR		127       
#define VSMIN_INT		(-2147483647 - 1) 
#define VSMAX_INT       2147483647    
#define VSMAX_REAL		3.402823466e+38F
#define VSMIN_REAL		1.175494351e-38F
#define VSMAX_INTEGER	0xffffffff
#define VSREAL_1		0.99999f		
#define VSPI			3.14159265f
#define VSPI2			1.5707963f
#define VS2PI			6.2831853f

#define VSSHADOWON		0
#define VSSHADOWBACK	1
#define VSNOSHADOW		2


#define VSUSHORT_INDEX	unsigned short

#define VSUINT_INDEX	unsigned int
FORCEINLINE unsigned int CalcAlign(unsigned int n, unsigned int align)
{
	return ((n + align - 1) & (~(align - 1)));
}
FORCEINLINE VSREAL RadianToAngle(VSREAL Radian)
{
	return ( Radian * 180.0f ) / VSPI ;
}
FORCEINLINE VSREAL AngleToRadian(VSREAL Angle)
{
	return ( Angle * VSPI ) /  180.0f;
}
FORCEINLINE bool IsTwoPower(unsigned int uiN)
{
	return !(uiN & (uiN - 1));
}
FORCEINLINE unsigned short FloatToHalf
(
 VSREAL Value
 )
{
	unsigned int Result;

	unsigned int IValue = ((unsigned int *)(&Value))[0];
	unsigned int Sign = (IValue & 0x80000000U) >> 16U;
	IValue = IValue & 0x7FFFFFFFU;      // Hack off the sign

	if (IValue > 0x47FFEFFFU)
	{
		// The number is too large to be represented as a half.  Saturate to infinity.
		Result = 0x7FFFU;
	}
	else
	{
		if (IValue < 0x38800000U)
		{
			// The number is too small to be represented as a normalized half.
			// Convert it to a denormalized value.
			unsigned int Shift = 113U - (IValue >> 23U);
			IValue = (0x800000U | (IValue & 0x7FFFFFU)) >> Shift;
		}
		else
		{
			// Rebias the exponent to represent the value as a normalized half.
			IValue += 0xC8000000U;
		}

		Result = ((IValue + 0x0FFFU + ((IValue >> 13U) & 1U)) >> 13U)&0x7FFFU; 
	}
	return (unsigned short)(Result|Sign);
}
FORCEINLINE VSREAL HalfToFloat
(
 unsigned short Value
 )
{


	unsigned int Mantissa;
	unsigned int Exponent;
	unsigned int Result;

	Mantissa = (unsigned int)(Value & 0x03FF);

	if ((Value & 0x7C00) != 0)  // The value is normalized
	{
		Exponent = (unsigned int)((Value >> 10) & 0x1F);
	}
	else if (Mantissa != 0)     // The value is denormalized
	{
		// Normalize the value in the resulting VSREAL
		Exponent = 1;

		do
		{
			Exponent--;
			Mantissa <<= 1;
		} while ((Mantissa & 0x0400) == 0);

		Mantissa &= 0x03FF;
	}
	else                        // The value is zero
	{
		Exponent = (unsigned int)-112;
	}

	Result = ((Value & 0x8000) << 16) | // Sign
		((Exponent + 112) << 23) | // Exponent
		(Mantissa << 13);          // Mantissa

	return *(VSREAL*)&Result;
}
//f must [0,1]
FORCEINLINE unsigned int CompressUnitFloat(VSREAL f, unsigned int Bit = 16)
{
	unsigned int nIntervals = 1 << Bit;
	VSREAL scaled = f * (nIntervals - 1);
	unsigned int rounded = (unsigned int)(scaled + 0.5f);
	if (rounded > nIntervals - 1)
	{
		rounded = nIntervals - 1;
	}
	return rounded;
	
}
FORCEINLINE unsigned int CompressFloat(VSREAL f, VSREAL Max , VSREAL Min ,unsigned int Bit = 16)
{
	VSREAL Unitf = (f - Min)/(Max - Min);
	return CompressUnitFloat(Unitf,Bit);
}
FORCEINLINE VSREAL DecompressUnitFloat(unsigned int quantized,unsigned int Bit = 16)
{
	unsigned int nIntervals = 1 << Bit;
	VSREAL IntervalSize = 1.0f / (nIntervals - 1);
	
	return quantized * IntervalSize;
}
FORCEINLINE VSREAL DecompressFloat(unsigned int quantized,VSREAL Max , VSREAL Min ,unsigned int Bit = 16)
{
	VSREAL Unitf = DecompressUnitFloat(quantized,Bit);
	return (Min + Unitf * (Max - Min));
}
FORCEINLINE  VSREAL ClampAxis(VSREAL Angle)
{
	// returns Angle in the range (-360,360)
	Angle = FMOD(Angle, 360.f);

	if (Angle < 0.f)
	{
		// shift to [0,360) range
		Angle += 360.f;
	}

	return Angle;
}


FORCEINLINE  VSREAL NormalizeAxis(VSREAL Angle)
{
	// returns Angle in the range [0,360)
	Angle = ClampAxis(Angle);

	if (Angle > 180.f)
	{
		// shift to (-180,180]
		Angle -= 360.f;
	}

	return Angle;
}

//-360.....360 can use
FORCEINLINE VSREAL ClampAngle(VSREAL AngleDegrees, VSREAL MinAngleDegrees, VSREAL MaxAngleDegrees)
{
	VSREAL MaxDelta = ClampAxis(MaxAngleDegrees - MinAngleDegrees) * 0.5f;			// 0..180
	VSREAL RangeCenter = ClampAxis(MinAngleDegrees + MaxDelta);						// 0..360
	VSREAL DeltaFromCenter = NormalizeAxis(AngleDegrees - RangeCenter);				// -180..180

	// maybe clamp to nearest edge
	if (DeltaFromCenter > MaxDelta)
	{
		return NormalizeAxis(RangeCenter + MaxDelta);
	}
	else if (DeltaFromCenter < -MaxDelta)
	{
		return NormalizeAxis(RangeCenter - MaxDelta);
	}

	// already in range, just return it
	return NormalizeAxis(AngleDegrees);
}

FORCEINLINE int VSRand() { return rand(); }
FORCEINLINE void VSRandInit(int Seed) { srand(Seed); }
FORCEINLINE float VSFRand() { return VSRand() / (float)RAND_MAX; }
//初始化CPU信息
bool VSMATH_API VSInitCPU(void);
bool VSMATH_API VSInitMath();
void VSMATH_API VSInitCRCTable();
FORCEINLINE VSREAL VSMATH_API GetFastSin(unsigned int i);
FORCEINLINE VSREAL VSMATH_API GetFastCos(unsigned int i);
unsigned int VSMATH_API CRC32Compute( const void *pData, unsigned int uiDateSize );
FORCEINLINE VSMATH_API unsigned int Rounding(VSREAL fNum);
extern bool g_bSSE;
extern VSREAL FastSin[361];
extern VSREAL FastCos[361];
#define VS_SSE
}
#endif