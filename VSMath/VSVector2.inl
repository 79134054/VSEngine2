/*----------------------------------------------------------------*/
FORCEINLINE void VSVector2::Set(VSREAL _x, VSREAL _y) 
{
	x=_x; y=_y; 
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSVector2::Normalize(void) 
{

#ifdef VS_SSE
	{
		VSFastNormalize(*this,*this);
	}
#else
	{
		VSREAL f = (VSREAL)SQRT(x*x + y*y);

		if (f != 0.0f) 
		{
			x/=f; y/=f;
		}

	}
#endif
}