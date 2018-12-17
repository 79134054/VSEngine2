
/*----------------------------------------------------------------*/
FORCEINLINE void VSVector3W::Set(VSREAL _x, VSREAL _y, VSREAL _z, VSREAL _w) 
{
	x=_x; y=_y; z=_z; w=_w;
}
VSVector3 VSVector3W::GetV3()const
{
	return VSVector3(x,y,z);
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSVector3W::GetV3(VSVector3 & V)const
{

	V.x = x;
	V.y = y;
	V.z = z;
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSVector3W::Set(const VSVector3 & V)
{
	x = V.x; 
	y = V.y; 
	z = V.z; 
	w = 1.0f;
}
