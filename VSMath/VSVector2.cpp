#include "VSVector2.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSVector2::VSVector2(void) 
{
	x=0;y=0;
}
/*----------------------------------------------------------------*/
VSVector2::VSVector2(VSREAL _x, VSREAL _y)
{
	x=_x; y=_y; 
}
/*----------------------------------------------------------------*/
void VSVector2::operator += (const VSVector2 &v)
{
	x += v.x;   y += v.y; 
}
/*----------------------------------------------------------------*/


VSVector2 VSVector2::operator + (const VSVector2 &v) const 
{
	return VSVector2(x+v.x, y+v.y);
}
/*----------------------------------------------------------------*/
void VSVector2::operator -= (const VSVector2 &v) 
{
	x -= v.x;   y -= v.y; 

}
/*----------------------------------------------------------------*/

VSVector2 VSVector2::operator - (const VSVector2 &v) const 
{
	return VSVector2(x-v.x, y-v.y);
}
/*----------------------------------------------------------------*/

void VSVector2::operator *= (VSREAL f) 
{
	x *= f;   y *= f;  
}
/*----------------------------------------------------------------*/

void VSVector2::operator /= (VSREAL f) 
{
	x /= f;   y /= f; 
}
/*----------------------------------------------------------------*/

VSVector2 VSVector2::operator * (VSREAL f) const 
{
	return VSVector2(x*f, y*f);
}
/*----------------------------------------------------------------*/

VSVector2 VSVector2::operator / (VSREAL f) const 
{
	return VSVector2(x/f, y/f);
}
/*----------------------------------------------------------------*/

void VSVector2::operator += (VSREAL f) 
{
	x += f;   y += f; 
}
/*----------------------------------------------------------------*/

void VSVector2::operator -= (VSREAL f) 
{
	x -= f;   y -= f; 
}
/*----------------------------------------------------------------*/

VSVector2 VSVector2::operator + (VSREAL f) const 
{
	return VSVector2(x+f, y+f);
}
/*----------------------------------------------------------------*/

VSVector2 VSVector2::operator - (VSREAL f) const 
{
	return VSVector2(x-f, y-f);
}
/*----------------------------------------------------------------*/

VSREAL VSVector2::operator * (const VSVector2 &v) const 
{
	return (v.x*x + v.y*y);
}
/*----------------------------------------------------------------*/
bool VSVector2::operator ==(const VSVector2 &v)const
{
	for (unsigned int i = 0 ; i < 2 ; i++)
	{
		if (ABS(m[i] - v.m[i]) > EPSILON_E4)
		{
			return false;
		}
	}
	return true;
// 	return (ABS(x-v.x) < EPSILON_E4 && 
// 		ABS(y-v.y) < EPSILON_E4);

}