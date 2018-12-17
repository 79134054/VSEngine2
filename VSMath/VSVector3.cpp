#include "VSVector3.h"
#include "VSMatrix3X3.h"
#include "VSMatrix3X3W.h"
#include "VSQuat.h"
using namespace VSEngine2;
const VSVector3 VSVector3::ms_Up = VSVector3(0.0f, 1.0f, 0.0f);
const VSVector3 VSVector3::ms_Right = VSVector3(1.0f, 0.0f, 0.0f);
const VSVector3 VSVector3::ms_Front = VSVector3(0.0f, 0.0f, 1.0f);
const VSVector3 VSVector3::ms_Zero = VSVector3(0.0f, 0.0f, 0.0f);
const VSVector3 VSVector3::ms_One = VSVector3(1.0f, 1.0f, 1.0f);
/*----------------------------------------------------------------*/
VSVector3::VSVector3(void) 
{
	x=0;y=0; z=0; 
}
/*----------------------------------------------------------------*/
VSVector3::VSVector3(VSREAL _x, VSREAL _y, VSREAL _z)
{
	x=_x; y=_y; z=_z;
}
/*----------------------------------------------------------------*/

FORCEINLINE VSREAL VSVector3::AngleWith(VSVector3 &v)
{
	return (VSREAL)acos( ((*this).Dot(v)) / (this->GetLength()*v.GetLength()) );
}



/*----------------------------------------------------------------*/

FORCEINLINE void VSVector3::Create(const VSVector3 &v1, const VSVector3 &v2) 
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(v1.x,v1.y,v1.z,0);
		__m128 _v2 = _mm_set_ps(v2.x,v2.y,v2.z,0);
		__m128 _v3 = _mm_sub_ps(_v2,_v1);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
	}
	#else
	{
		x = v2.x - v1.x;
		y = v2.y - v1.y;
		z = v2.z - v1.z;
	}
	#endif
}
/*----------------------------------------------------------------*/
VSVector3 & VSVector3::operator += (const VSVector3 &v)
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,0);
		__m128 _v3 = _mm_add_ps(_v2,_v1);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		return *this;
	}
	#else
	{
		x += v.x;   y += v.y;   z += v.z;
		return *this;
	}
	#endif
}
/*----------------------------------------------------------------*/


VSVector3 VSVector3::operator + (const VSVector3 &v) const 
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,0);
		__m128 _v3 = _mm_add_ps(_v2,_v1);
		return VSVector3(_v3.m128_f32[3], _v3.m128_f32[2], _v3.m128_f32[1]);
	}
	#else
	{
		return VSVector3(x+v.x, y+v.y, z+v.z);
	}
	#endif
}
/*----------------------------------------------------------------*/


VSVector3 & VSVector3::operator -= (const VSVector3 &v)
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,0);
		__m128 _v3 = _mm_sub_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		return *this;
	}
	#else
	{
		x -= v.x;   y -= v.y;   z -= v.z;
		return *this;
	}
	#endif
}
/*----------------------------------------------------------------*/

VSVector3 VSVector3::operator - (const VSVector3 &v) const 
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,0);
		__m128 _v3 = _mm_sub_ps(_v1,_v2);
		return VSVector3(_v3.m128_f32[3], _v3.m128_f32[2], _v3.m128_f32[1]);
	}
	#else
	{
		return VSVector3(x-v.x, y-v.y, z-v.z);
	}
	#endif
}
VSVector3 VSVector3::operator / (const VSVector3 &v)const
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,1.0f);
		__m128 _v3 = _mm_div_ps(_v1,_v2);
		return VSVector3(_v3.m128_f32[3], _v3.m128_f32[2], _v3.m128_f32[1]);
	}
	#else
	{
		return VSVector3(x / v.x, y / v.y, z / v.z);
	}
	#endif
}
VSVector3 & VSVector3::operator /= (const VSVector3 &v)
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x, y, z, 0);
		__m128 _v2 = _mm_set_ps(v.x, v.y, v.z, 1.0f);
		__m128 _v3 = _mm_div_ps(_v1, _v2);
		*this = VSVector3(_v3.m128_f32[3], _v3.m128_f32[2], _v3.m128_f32[1]);
		return *this;
	}
#else
	{
		*this =  VSVector3(x / v.x, y / v.y, z / v.z);
		return *this;
	}
#endif
}
VSVector3 VSVector3::operator * (const VSVector3 &v) const
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x, y, z, 0);
		__m128 _v2 = _mm_set_ps(v.x, v.y, v.z, 0);
		__m128 _v3 = _mm_mul_ps(_v1, _v2);
		return VSVector3(_v3.m128_f32[3], _v3.m128_f32[2], _v3.m128_f32[1]);
	}
	#else
	{
		return return VSVector3(x * v.x, y * v.y, z * v.z);
	}
	#endif
}
VSVector3 & VSVector3::operator *= (const VSVector3 &v) 
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x, y, z, 0);
		__m128 _v2 = _mm_set_ps(v.x, v.y, v.z, 0);
		__m128 _v3 = _mm_mul_ps(_v1, _v2);
		*this =  VSVector3(_v3.m128_f32[3], _v3.m128_f32[2], _v3.m128_f32[1]);
		return *this;
	}
#else
	{
		*this =   VSVector3(x * v.x, y * v.y, z * v.z);
		return *this;
	}
#endif
}
/*----------------------------------------------------------------*/

VSVector3 & VSVector3::operator *= (VSREAL f)
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(f,f,f,0);
		__m128 _v3 = _mm_mul_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		return *this;
	}
	#else
	{
		x *= f;   y *= f;   z *= f;
		return *this;
	}
	#endif
}
/*----------------------------------------------------------------*/

VSVector3 & VSVector3::operator /= (VSREAL f)
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(f,f,f,1.0f);
		__m128 _v3 = _mm_div_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		return *this;
	}
	#else
	{
		x /= f;   y /= f;   z /= f;
		return *this;
	}
	#endif
}
/*----------------------------------------------------------------*/

VSVector3 VSVector3::operator * (VSREAL f) const 
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(f,f,f,0);
		__m128 _v3 = _mm_mul_ps(_v1,_v2);
		return VSVector3(_v3.m128_f32[3],_v3.m128_f32[2], _v3.m128_f32[1]);
	}
	#else
	{
		return VSVector3(x*f, y*f, z*f);
	}
	#endif
}
/*----------------------------------------------------------------*/

VSVector3 VSVector3::operator / (VSREAL f) const 
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(f,f,f,1.0f);
		__m128 _v3 = _mm_div_ps(_v1,_v2);
		return VSVector3(_v3.m128_f32[3],_v3.m128_f32[2], _v3.m128_f32[1]);
	}
	#else
	{
		return VSVector3(x/f, y/f, z/f);
	}
	#endif
}
/*----------------------------------------------------------------*/

VSVector3 & VSVector3::operator += (VSREAL f)
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(f,f,f,0);
		__m128 _v3 = _mm_add_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		return *this;
	}
	#else
	{
		x += f;   y += f;   z += f;
		return *this;
	}
	#endif
}
/*----------------------------------------------------------------*/

VSVector3 & VSVector3::operator -= (VSREAL f)
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(f,f,f,0);
		__m128 _v3 = _mm_sub_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		return *this;
	}
	#else
	{
		x -= f;   y -= f;   z -= f;
		return *this;
	}
	#endif
}
/*----------------------------------------------------------------*/

VSVector3 VSVector3::operator + (VSREAL f) const 
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(f,f,f,0);
		__m128 _v3 = _mm_add_ps(_v1,_v2);
		return VSVector3(_v3.m128_f32[3],_v3.m128_f32[2], _v3.m128_f32[1]);
	}
	#else
	{
		return VSVector3(x+f, y+f, z+f);
	}
	#endif
}
/*----------------------------------------------------------------*/

VSVector3 VSVector3::operator - (VSREAL f) const 
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,0);
		__m128 _v2 = _mm_set_ps(f,f,f,0);
		__m128 _v3 = _mm_sub_ps(_v1,_v2);
		return VSVector3(_v3.m128_f32[3],_v3.m128_f32[2], _v3.m128_f32[1]);
	}
	#else
	{
		return VSVector3(x-f, y-f, z-f);
	}
	#endif
}
/*----------------------------------------------------------------*/
VSREAL VSVector3::Dot(const VSVector3 &v)const
{
	#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x, y, z, 0);
		__m128 _v2 = _mm_set_ps(v.x, v.y, v.z, 0);
		__m128 _v3 = _mm_mul_ps(_v1, _v2);
		return _v3.m128_f32[3] + _v3.m128_f32[2] + _v3.m128_f32[1];
	}
	#else
	{
		return (v.x*x + v.y*y + v.z*z);
	}
	#endif
}
/*----------------------------------------------------------------*/
bool VSVector3::operator ==(const VSVector3 &v)const
{
	for (unsigned int i = 0 ; i < 3 ; i++)
	{
		if (ABS(m[i] - v.m[i]) > EPSILON_E4)
		{
			return false;
		}
	}
	return true;
// 	return (ABS(x-v.x) < EPSILON_E4 && 
// 			ABS(y-v.y) < EPSILON_E4 &&
// 			ABS(z-v.z) < EPSILON_E4 );


}
/*----------------------------------------------------------------*/
VSQuat VSVector3::operator * (const VSQuat &q) const 
{
	#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(q.w,q.w,q.w,-q.x);
		__m128 _q1 = _mm_set_ps(q.z,q.x,q.y,-q.y);
		__m128 _q2 = _mm_set_ps(-q.y,-q.z,-q.x,-q.z);

		__m128 _m0 = _mm_set_ps(x,y,z,x);
		__m128 _m1 = _mm_set_ps(y,z,x,y);
		__m128 _m2 = _mm_set_ps(z,x,y,z);

		__m128 _r0 = _mm_mul_ps(_q0,_m0);
		__m128 _r1 = _mm_mul_ps(_q1,_m1);
		__m128 _r2 = _mm_mul_ps(_q2,_m2);

		__m128 _r = _mm_add_ps(_r0,_mm_add_ps(_r1,_r2));

		return VSQuat(  _r.m128_f32[3] ,_r.m128_f32[2],_r.m128_f32[1],_r.m128_f32[0]);
	}
	#else
	{
		return VSQuat(  q.w*x + q.z*y - q.y*z,
					q.w*y + q.x*z - q.z*x,
					q.w*z + q.y*x - q.x*y,
					-(q.x*x + q.y*y + q.z*z) );
	}
	#endif
}
/*----------------------------------------------------------------*/
VSVector3 VSVector3::operator * (const VSMatrix3X3 &m) const 
{
	VSVector3 vcResult;
	
	#ifdef VS_SSE
	{
		__m128 _v = _mm_set_ps(x,y,z,0);
		__m128 _m0 = _mm_set_ps(m._00,m._10,m._20,0);
		__m128 _m1 = _mm_set_ps(m._01,m._11,m._21,0);
		__m128 _m2 = _mm_set_ps(m._02,m._12,m._22,0);

		__m128 r0 = _mm_mul_ps(_v,_m0);
		__m128 r1 = _mm_mul_ps(_v,_m1);
		__m128 r2 = _mm_mul_ps(_v,_m2);

		vcResult.x = r0.m128_f32[3] + r0.m128_f32[2] + r0.m128_f32[1];
		vcResult.y = r1.m128_f32[3] + r1.m128_f32[2] + r1.m128_f32[1];
		vcResult.z = r2.m128_f32[3] + r2.m128_f32[2] + r2.m128_f32[1];
	}
	#else
	{
		vcResult.x = x*m._00 + y*m._10 + z*m._20;
		vcResult.y = x*m._01 + y*m._11 + z*m._21;
		vcResult.z = x*m._02 + y*m._12 + z*m._22;
	}
	#endif




	return vcResult;
}
/*----------------------------------------------------------------*/
bool VSVector3::IsParallel(const VSVector3 & Vector)const
{
	VSREAL t0,t1;
	bool temp = 0;
	t0 = x * Vector.y;
	t1 = y * Vector.x;
	
	if( ABS(t0 - t1) > EPSILON_E4)
		temp = 1;

	t0 = y * Vector.z;
	t1 = z * Vector.y;

	if( ABS(t0 - t1) > EPSILON_E4 && temp)
		return 1;
	else
		return 0;
}
/*----------------------------------------------------------------*/
VSVector3 VSVector3::operator * (const VSMatrix3X3W &m)const
{
	VSVector3 vcResult;

	#ifdef VS_SSE
	{
		__m128 _v = _mm_set_ps(x,y,z,1.0f);
		__m128 _m0 = _mm_set_ps(m._00,m._10,m._20,m._30);
		__m128 _m1 = _mm_set_ps(m._01,m._11,m._21,m._31);
		__m128 _m2 = _mm_set_ps(m._02,m._12,m._22,m._32);
		__m128 _m3 = _mm_set_ps(m._03,m._13,m._23,m._33);

		__m128 r0 = _mm_mul_ps(_v,_m0);
		__m128 r1 = _mm_mul_ps(_v,_m1);
		__m128 r2 = _mm_mul_ps(_v,_m2);
		__m128 r3 = _mm_mul_ps(_v,_m3);

		__m128 _s0 = _mm_set_ps(r0.m128_f32[3],r1.m128_f32[3],r2.m128_f32[3],r3.m128_f32[3]);
		__m128 _s1 = _mm_set_ps(r0.m128_f32[2],r1.m128_f32[2],r2.m128_f32[2],r3.m128_f32[2]);
		__m128 _s2 = _mm_set_ps(r0.m128_f32[1],r1.m128_f32[1],r2.m128_f32[1],r3.m128_f32[1]);
		__m128 _s3 = _mm_set_ps(r0.m128_f32[0],r1.m128_f32[0],r2.m128_f32[0],r3.m128_f32[0]);

		__m128 _k = _mm_add_ps(_s0,_mm_add_ps(_s1,_mm_add_ps(_s2,_s3)));
		
		__m128 _f = _mm_set_ps(_k.m128_f32[3],_k.m128_f32[2],_k.m128_f32[1],0.0f);
		__m128 _div_w = _mm_set_ps(_k.m128_f32[0],_k.m128_f32[0],_k.m128_f32[0],1.0f);
		__m128 _l = _mm_div_ps(_f,_div_w);
		vcResult.x = _l.m128_f32[3];
		vcResult.y = _l.m128_f32[2];
		vcResult.z = _l.m128_f32[1];
	}
	#else
	{
		vcResult.x = x*m._00 + y*m._10 + z*m._20 + m._30;
		vcResult.y = x*m._01 + y*m._11 + z*m._21 + m._31;
		vcResult.z = x*m._02 + y*m._12 + z*m._22 + m._32;
		VSREAL w = x*m._03 + y*m._13 + z*m._23 + m._33;

		vcResult.x = vcResult.x / w;
		vcResult.y = vcResult.y / w;
		vcResult.z = vcResult.z / w;
	}
	#endif
	return vcResult;
}
VSVector3 VSVector3::ReflectDir(const VSVector3 &N)const
{
	return N * Dot(N) * 2 + *this;
}
