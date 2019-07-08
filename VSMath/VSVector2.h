#ifndef VSVECTOR2_H
#define VSVECTOR2_H
#include "VSMath.h"
#include "VSFastFunction.h"
namespace VSEngine2
{
	class VSMATH_API VSVector2
	{
	public:
		union
		{
			VSREAL m[2];
			struct  
			{
				VSREAL x, y;
			};
		};
		VSVector2(void);
		VSVector2(VSREAL _x, VSREAL _y);

		/*************************************FORCEINLINE************************************************/
		FORCEINLINE void  Set(VSREAL _x, VSREAL _y);
		
		void operator += (const VSVector2 &v);          
		void operator -= (const VSVector2 &v);          

		void operator *= (VSREAL f);                     
		void operator /= (VSREAL f);                     
		void operator += (VSREAL f);                     
		void operator -= (VSREAL f);                     

		VSREAL     operator * (const VSVector2 &v)const; //µã»ý

		bool operator ==(const VSVector2 &v)const;


		VSVector2 operator * (VSREAL f)const;            
		VSVector2 operator / (VSREAL f)const;            
		VSVector2 operator + (VSREAL f)const;            
		VSVector2 operator - (VSREAL f)const;            



		VSVector2 operator + (const VSVector2 &v)const; 
		VSVector2 operator - (const VSVector2 &v)const; 

		void Normalize();
	};
	FORCEINLINE bool VS2DIntersect(VSVector2 & A1,VSVector2 & B1,VSVector2 & A2 ,VSVector2 & B2, VSVector2 & Out)
	{
		VSREAL denominator = (B1.y * (A2.x - B2.x) + A1.y * (B2.x - A2.x) + (A1.x - B1.x) * (A2.y - B2.y));

		if(ABS(denominator) < 0.0001)
			return false;

		Out = VSVector2((-B1.x * A2.y * B2.x + A1.y * B1.x * (B2.x - A2.x) + B1.x * A2.x * B2.y + A1.x * (B1.y * A2.x - B2.y * A2.x - B1.y * B2.x + A2.y * B2.x)) / denominator,
			( B1.y * (-A2.y * B2.x + A1.x * (A2.y - B2.y) + A2.x * B2.y) + A1.y * (A2.y * B2.x - A2.x * B2.y + B1.x * (B2.y - A2.y))) / denominator);

		return true;
	}
#include "VSVector2.inl"

}
#endif