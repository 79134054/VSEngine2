#include "VSVector3W.h"
#include "VSMatrix3X3W.h"
using namespace VSEngine2;

/*----------------------------------------------------------------*/
VSVector3W::VSVector3W(void) 
{
	x=0, y=0, z=0, w=1.0f; 
}
/*----------------------------------------------------------------*/
VSVector3W::VSVector3W(VSREAL _x, VSREAL _y, VSREAL _z,VSREAL _w)
{
	x=_x; y=_y; z=_z; w=_w; 
}
/*----------------------------------------------------------------*/
VSVector3W::VSVector3W(unsigned char R,unsigned char G,unsigned char B,unsigned char A)
{
	r = (R * (VSREAL)1.0) / 255;
	g = (G * (VSREAL)1.0) / 255;
	b = (B * (VSREAL)1.0) / 255;
	a = (A * (VSREAL)1.0) / 255;
}
/*----------------------------------------------------------------*/
VSVector3W::VSVector3W(const VSVector3 & V)
{

	Set(V);
}
/*----------------------------------------------------------------*/
void VSVector3W::CreateFromARGB(DWORD ARGB)
{
	unsigned char a,r,g,b;
	VSDWCOLORGetARGB(ARGB,a,r,g,b);
	*this = VSVector3W(r, g, b, a);
}
void VSVector3W::CreateFromBGRA(DWORD BGRA)
{
	unsigned char a,r,g,b;
	VSDWCOLORGetBGRA(BGRA,a,r,g,b);
	*this = VSVector3W(r,g,b,a);
}
void VSVector3W::CreateFromRGBA(DWORD RGBA)
{
	unsigned char a,r,g,b;
	VSDWCOLORGetRGBA(RGBA,a,r,g,b);
	*this = VSVector3W(r,g,b,a);
}
void VSVector3W::CreateFormABGR(DWORD ABGR)
{
	unsigned char a,r,g,b;
	VSDWCOLORGetABGR(ABGR,a,r,g,b);
	*this = VSVector3W(r,g,b,a);
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator * (const VSMatrix3X3W &m) const 
{
	VSVector3W vcResult;

#ifdef VS_SSE
	{
		__m128 _v = _mm_set_ps(x,y,z,w);
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

// 		__m128 _f = _mm_set_ps(_k.m128_f32[3],_k.m128_f32[2],_k.m128_f32[1],0.0f);
// 		__m128 _div_w = _mm_set_ps(_k.m128_f32[0],_k.m128_f32[0],_k.m128_f32[0],1.0f);
// 		__m128 _l = _mm_div_ps(_f,_div_w);
// 		vcResult.x = _l.m128_f32[3];
// 		vcResult.y = _l.m128_f32[2];
// 		vcResult.z = _l.m128_f32[1];

		vcResult.x = _k.m128_f32[3];
		vcResult.y = _k.m128_f32[2];
		vcResult.z = _k.m128_f32[1];
		vcResult.w = _k.m128_f32[0];
	}
#else
	{
		vcResult.x = x*m._00 + y*m._10 + z*m._20 + w * m._30;
		vcResult.y = x*m._01 + y*m._11 + z*m._21 + w * m._31;
		vcResult.z = x*m._02 + y*m._12 + z*m._22 + w * m._32;
		vcResult.w = x*m._03 + y*m._13 + z*m._23 + w * m._33;

// 		vcResult.x = vcResult.x / w;
// 		vcResult.y = vcResult.y / w;
// 		vcResult.z = vcResult.z / w;
	}
#endif


	return vcResult;
}
/*----------------------------------------------------------------*/
void VSVector3W::operator += (const VSVector3W &v)
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,v.w);
		__m128 _v3 = _mm_add_ps(_v2,_v1);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		w = _v3.m128_f32[0];
	}
#else
	{
		x += v.x;   y += v.y;   z += v.z; w += v.w;
	}
#endif
}
/*----------------------------------------------------------------*/
void VSVector3W::operator -= (const VSVector3W &v)
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,v.w);
		__m128 _v3 = _mm_sub_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		w = _v3.m128_f32[0];
	}
#else
	{
		x -= v.x;   y -= v.y;   z -= v.z; w -= v.w;
	}
#endif
}
/*----------------------------------------------------------------*/
void VSVector3W::operator *= (const VSVector3W &v)
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,v.w);
		__m128 _v3 = _mm_mul_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		w = _v3.m128_f32[0];
	}
#else
	{
		x *= v.x;   y *= v.y;   z *= v.z; w *= v.w;
	}
#endif
}
/*----------------------------------------------------------------*/
void VSVector3W::operator /= (const VSVector3W &v)
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,v.w);
		__m128 _v3 = _mm_div_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		w = _v3.m128_f32[0];
	}
#else
	{
		x /= v.x;   y /= v.y;   z /= v.z; w /= v.w;
	}
#endif
}
/*----------------------------------------------------------------*/
void VSVector3W::operator *= (VSREAL f)
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(f,f,f,f);
		__m128 _v3 = _mm_mul_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		w = _v3.m128_f32[0];
	}
#else
	{
		x *= f;   y *= f;   z *= f; w *= f;
	}
#endif
}
/*----------------------------------------------------------------*/
void VSVector3W::operator /= (VSREAL f)
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(f,f,f,f);
		__m128 _v3 = _mm_div_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		w = _v3.m128_f32[0];
	}
#else
	{
		x /= f;   y /= f;   z /= f; w /= f;
	}
#endif
}
/*----------------------------------------------------------------*/
void VSVector3W::operator += (VSREAL f)
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(f,f,f,f);
		__m128 _v3 = _mm_add_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		w = _v3.m128_f32[0];
	}
#else
	{
		x += f;   y += f;   z += f; w += f;
	}
#endif
}
/*----------------------------------------------------------------*/
void VSVector3W::operator -= (VSREAL f)
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(f,f,f,f);
		__m128 _v3 = _mm_sub_ps(_v1,_v2);
		x = _v3.m128_f32[3];
		y = _v3.m128_f32[2];
		z = _v3.m128_f32[1];
		w = _v3.m128_f32[0];
	}
#else
	{
		x -= f;   y -= f;   z -= f; w -= f;
	}
#endif
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator * (VSREAL f)const
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(f,f,f,f);
		__m128 _v3 = _mm_mul_ps(_v1,_v2);
		return VSVector3W(_v3.m128_f32[3],_v3.m128_f32[2], _v3.m128_f32[1],_v3.m128_f32[0]);
	}
#else
	{
		return VSVector3W(x*f, y*f, z*f,w*f);
	}
#endif
	
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator / (VSREAL f)const
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(f,f,f,f);
		__m128 _v3 = _mm_div_ps(_v1,_v2);
		return VSVector3W(_v3.m128_f32[3],_v3.m128_f32[2], _v3.m128_f32[1],_v3.m128_f32[0]);
	}
#else
	{
		return VSVector3W(x/f, y/f, z/f,w/f);
	}
#endif
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator + (VSREAL f)const
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(f,f,f,f);
		__m128 _v3 = _mm_add_ps(_v1,_v2);
		return VSVector3W(_v3.m128_f32[3],_v3.m128_f32[2], _v3.m128_f32[1],_v3.m128_f32[0]);
	}
#else
	{
		return VSVector3W(x+f, y+f, z+f,w+f);
	}
#endif
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator - (VSREAL f)const
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(f,f,f,f);
		__m128 _v3 = _mm_sub_ps(_v1,_v2);
		return VSVector3W(_v3.m128_f32[3],_v3.m128_f32[2], _v3.m128_f32[1],_v3.m128_f32[0]);
	}
#else
	{
		return VSVector3W(x-f, y-f, z-f,w-f);
	}
#endif
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator + (const VSVector3W &v)const
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,v.w);
		__m128 _v3 = _mm_add_ps(_v2,_v1);
		return VSVector3W(_v3.m128_f32[3], _v3.m128_f32[2], _v3.m128_f32[1],_v3.m128_f32[0]);
	}
#else
	{
		return VSVector3W(x+v.x, y+v.y, z+v.z,w+v.w);
	}
#endif
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator - (const VSVector3W &v)const
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,v.w);
		__m128 _v3 = _mm_sub_ps(_v2,_v1);
		return VSVector3W(_v3.m128_f32[3], _v3.m128_f32[2], _v3.m128_f32[1],_v3.m128_f32[0]);
	}
#else
	{
		return VSVector3W(x-v.x, y-v.y, z-v.z,w-v.w);
	}
#endif
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator * (const VSVector3W &v)const
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,v.w);
		__m128 _v3 = _mm_mul_ps(_v2,_v1);
		return VSVector3W(_v3.m128_f32[3], _v3.m128_f32[2], _v3.m128_f32[1],_v3.m128_f32[0]);
	}
#else
	{
		return VSVector3W(x*v.x, y*v.y, z*v.z,w*v.w);
	}
#endif
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator / (const VSVector3W &v)const
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(x,y,z,w);
		__m128 _v2 = _mm_set_ps(v.x,v.y,v.z,v.w);
		__m128 _v3 = _mm_sub_ps(_v2,_v1);
		return VSVector3W(_v3.m128_f32[3], _v3.m128_f32[2], _v3.m128_f32[1],_v3.m128_f32[0]);
	}
#else
	{
		return VSVector3W(x-v.x, y-v.y, z-v.z,w-v.w);
	}
#endif
}
/*----------------------------------------------------------------*/
bool VSVector3W::operator ==(const VSVector3W &v)const
{
	for (unsigned int i = 0 ; i < 4 ; i++)
	{
		if (ABS(m[i] - v.m[i]) > EPSILON_E4)
		{
			return false;
		}
	}
	return true;
// 	return (ABS(x-v.x) < EPSILON_E4 && 
// 		ABS(y-v.y) < EPSILON_E4 &&
// 		ABS(z-v.z) < EPSILON_E4 &&
// 		ABS(w-v.w) < EPSILON_E4 );

}
/*----------------------------------------------------------------*/
DWORD VSVector3W::GetDWARGB()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);
	
	return (DWORD)(((((A)&0xff)<<24)|(((R)&0xff)<<16)|(((G)&0xff)<<8)|((B)&0xff)));
}
/*----------------------------------------------------------------*/
DWORD VSVector3W::GetDWRGBA()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);

	return (DWORD)(((((R)&0xff)<<24)|(((G)&0xff)<<16)|(((B)&0xff)<<8)|((A)&0xff)));

}
/*----------------------------------------------------------------*/
DWORD VSVector3W::GetDWBGRA()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);
	return ((DWORD)((((B)&0xff)<<24)|(((G)&0xff)<<16)|(((R)&0xff)<<8)|((A)&0xff)));
}
/*----------------------------------------------------------------*/
DWORD VSVector3W::GetDWABGR()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);
	return ((DWORD)((((A)&0xff)<<24)|(((B)&0xff)<<16)|(((G)&0xff)<<8)|((R)&0xff)));
}
/*----------------------------------------------------------------*/
void VSVector3W::GetUCColor(unsigned char &A,unsigned char &R,unsigned char &G,unsigned char &B)const
{
	A = (unsigned char)(a * 255);
	R = (unsigned char)(r * 255);
	G = (unsigned char)(g * 255);
	B = (unsigned char)(b * 255);
	
}
/*----------------------------------------------------------------*/


