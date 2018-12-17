
/*----------------------------------------------------------------*/
FORCEINLINE void VSVector3::Set(VSREAL _x, VSREAL _y, VSREAL _z) 
{
	x=_x; y=_y; z=_z; 
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSVector3::GetLength(void)const
{
#ifdef VS_SSE
		return VSFastLength(*this);
#else
		return SQRT(x*x + y*y + z*z);
#endif
	
}
/*----------------------------------------------------------------*/

FORCEINLINE VSREAL VSVector3::GetSqrLength(void) const 
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(x,y,z,0);
		__m128 _v3 = _mm_mul_ps(_v1,_v2);
		return _v3.m128_f32[3] + _v3.m128_f32[2] + _v3.m128_f32[1];

	}
	#else
	{
		return (x*x + y*y + z*z); 
	}
	#endif
}

/*----------------------------------------------------------------*/

FORCEINLINE void VSVector3::Negate(void) 
{
	x = -x;  y = -y;  z = -z;
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSVector3::Normalize(void) 
{

	#ifdef VS_SSE
	{
		VSFastNormalize(*this,*this);
	}
	#else
	{
		VSREAL f = (VSREAL)SQRT(x*x + y*y + z*z);

		if (f != 0.0f) 
		{
			x/=f; y/=f; z/=f;
		}

	}
	#endif
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSVector3::Cross(const VSVector3 &v1, const VSVector3 &v2) 
{

	#ifdef VS_SSE
	{
		VSFastCross(v1,v2,*this);
	}
	#else
	{
		x = v1.y * v2.z - v1.z * v2.y;
		y = v1.z * v2.x - v1.x * v2.z;
		z = v1.x * v2.y - v1.y * v2.x;
	}
	#endif

	
}
/*----------------------------------------------------------------*/
