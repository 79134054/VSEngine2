#include "VSMath.h"
#include "VSQuat.h"
#include "VSVector3.h"
#include "VSMatrix3X3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSQuat::VSQuat(void) 
{ 
	x=0.0f, y=0.0f, z=0.0f, w=1.0f; 
}
/*----------------------------------------------------------------*/
VSQuat::VSQuat(VSREAL _x, VSREAL _y, VSREAL _z, VSREAL _w)
{ 
	x=_x; y=_y; z=_z; w=_w; 
}
/*----------------------------------------------------------------*/
void VSQuat::Set(VSREAL _x, VSREAL _y, VSREAL _z, VSREAL _w)
{
	x=_x; y=_y; z=_z; w=_w; 
}
/*----------------------------------------------------------------*/
void VSQuat::CreateEuler(VSREAL fRoll, VSREAL fPitch, VSREAL fYaw)
{
	VSREAL cX, cY, cZ, sX, sY, sZ, cXcZ, sXsZ, cXsZ, sXcZ;

	fPitch *= 0.5f;
	fYaw   *= 0.5f;
	fRoll  *= 0.5f;

	cX = COS(fPitch);
	cY = COS(fYaw);
	cZ = COS(fRoll);

	sX = SIN(fPitch);
	sY = SIN(fYaw);
	sZ = SIN(fRoll);

	cXcZ = cX * cZ;
	sXsZ = sX * sZ;
	cXsZ = cX * sZ;
	sXcZ = sX * cZ;
	
	w = cXcZ * cY + sXsZ * sY;
	x = sXcZ * cY + cXsZ * sY;
	y = cXcZ * sY - sXsZ * cY;
	z = cXsZ * cY - sXcZ * sY;

} // MakeFromEuler
/*----------------------------------------------------------------*/
void VSQuat::Normalize(void) 
{
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _q0q0 = _mm_mul_ps(_q0,_q0);
		VSREAL Invm = 1.0f/ SQRT(_q0q0.m128_f32[3] + _q0q0.m128_f32[2] + _q0q0.m128_f32[1] + _q0q0.m128_f32[0]);
		__m128 _Invm = _mm_set_ps(Invm,Invm,Invm,Invm);
		__m128 _r = _mm_mul_ps(_Invm,_q0);
		x = _r.m128_f32[3];
		y = _r.m128_f32[2];
		z = _r.m128_f32[1];
		w = _r.m128_f32[0];
	}
#else
	{
		VSREAL m=SQRT(x*x + y*y + z*z + w*w);
		VSREAL Invm = 1.0f/ m;
		x *= Invm; y *= Invm; z *= Invm; w *= Invm;
	}
#endif
}
/*----------------------------------------------------------------*/
VSQuat VSQuat::GetConjugate()const
{
	
	return VSQuat(-x,-y,-z,w);
}
/*----------------------------------------------------------------*/
void VSQuat::GetEulers(VSREAL &fRoll, VSREAL &fPitch, VSREAL &fYaw)const
{

	VSREAL wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;


	x2 = x + x;
	y2 = y + y;
	z2 = z + z;

	xx = x * x2;
	xy = x * y2;
	xz = x * z2;

	yy = y * y2;
	yz = y * z2;
	zz = z * z2;

	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	VSREAL _00 = 1.0f - (yy + zz);//
	VSREAL _01 = xy + wz;//


	VSREAL _10 = xy - wz;//
	VSREAL _11 = 1.0f - (xx + zz);//


	VSREAL _20 = xz + wy;//
	VSREAL _21 = yz - wx;//
	VSREAL _22 = 1.0f - (xx + yy);//

	if (_21 > 1.0f)
		fPitch = -VSPI2;
	else if (_21 < -1.0f)
		fPitch = VSPI2;
	else
		fPitch = ASIN(-_21);

	if (fabs(_21) > VSREAL_1)
	{
		fYaw = 0;
		fRoll = ATAN2(-_10, _00);

	}
	else
	{
		fRoll = ATAN2(_01, _11);
		fYaw = ATAN2(_20, _22);

	}


} // EulerFromQuat
/*----------------------------------------------------------------*/
void VSQuat::GetMatrix(VSMatrix3X3 &Mat)const
{
	VSREAL wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;


	x2 = x + x; 
	y2 = y + y; 
	z2 = z + z;

	xx = x * x2;   
	xy = x * y2;  
	xz = x * z2;

	yy = y * y2;   
	yz = y * z2;   
	zz = z * z2;

	wx = w * x2;   
	wy = w * y2;   
	wz = w * z2;

	Mat._00 = 1.0f - (yy + zz);
	Mat._01 = xy + wz;
	Mat._02 = xz - wy;

	Mat._10 = xy - wz;
	Mat._11 = 1.0f - (xx + zz);
	Mat._12 = yz + wx;

	Mat._20 = xz + wy;
	Mat._21 = yz - wx;
	Mat._22 = 1.0f - (xx + yy);
} // GetMatrix
/*----------------------------------------------------------------*/
VSREAL VSQuat::GetLength(void)const 
{
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _q0q0 = _mm_mul_ps(_q0,_q0);
		return SQRT(_q0q0.m128_f32[3] + _q0q0.m128_f32[2] + _q0q0.m128_f32[1] + _q0q0.m128_f32[0]);
	}
#else
	{
		return SQRT(x*x + y*y + z*z + w*w);
	}
#endif
}
/*----------------------------------------------------------------*/
void VSQuat::operator /= (VSREAL f) 
{ 
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(f,f,f,f);
		__m128 _r = _mm_div_ps(_q0,_f);
		x = _r.m128_f32[3];
		y = _r.m128_f32[2];
		z = _r.m128_f32[1];
		w = _r.m128_f32[0];
	}
#else
	{
		w /= f;  x /= f;  y /= f;  z /= f; 
	}
#endif
}

/*----------------------------------------------------------------*/
VSQuat VSQuat::operator / (VSREAL f) const
{
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(f,f,f,f);
		__m128 _r = _mm_div_ps(_q0,_f);
		return VSQuat(_r.m128_f32[3],_r.m128_f32[2],_r.m128_f32[1],_r.m128_f32[0]);
	}
#else
	{
		return VSQuat(x/f, y/f, z/f, w/f);
	}
#endif
}
/*----------------------------------------------------------------*/

void VSQuat::operator *= (VSREAL f) 
{ 
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(f,f,f,f);
		__m128 _r = _mm_mul_ps(_q0,_f);
		x = _r.m128_f32[3];
		y = _r.m128_f32[2];
		z = _r.m128_f32[1];
		w = _r.m128_f32[0];
	}
#else
	{
		w *= f;  x *= f;  y *= f;  z *= f; 
	}
#endif
}
/*----------------------------------------------------------------*/
VSQuat VSQuat::operator * (VSREAL f) const
{
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(f,f,f,f);
		__m128 _r = _mm_mul_ps(_q0,_f);
		return VSQuat(_r.m128_f32[3],_r.m128_f32[2],_r.m128_f32[1],_r.m128_f32[0]);
	}
#else
	{
		return VSQuat(x*f, y*f, z*f, w*f);
	}
#endif
}
/*----------------------------------------------------------------*/


void VSQuat::operator += (const VSQuat &q) 
{ 
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(q.x,q.y,q.z,q.w);
		__m128 _r = _mm_add_ps(_q0,_f);
		x = _r.m128_f32[3];
		y = _r.m128_f32[2];
		z = _r.m128_f32[1];
		w = _r.m128_f32[0];
	}
#else
	{
		w += q.w;  x += q.x;  y += q.y;  z += q.z; 
	}
#endif
}
/*----------------------------------------------------------------*/
VSQuat VSQuat::operator + (const VSQuat &q) const
{
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(q.x,q.y,q.z,q.w);
		__m128 _r = _mm_add_ps(_q0,_f);
		return VSQuat(_r.m128_f32[3],_r.m128_f32[2],_r.m128_f32[1],_r.m128_f32[0]);
	}
#else
	{
		return VSQuat(x+q.x, y+q.y, z+q.z, w+q.w);
	}
#endif
}
/*----------------------------------------------------------------*/
void VSQuat::operator -= (const VSQuat &q) 
{ 
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(q.x,q.y,q.z,q.w);
		__m128 _r = _mm_sub_ps(_q0,_f);
		x = _r.m128_f32[3];
		y = _r.m128_f32[2];
		z = _r.m128_f32[1];
		w = _r.m128_f32[0];
	}
#else
	{
		w -= q.w;  x -= q.x;  y -= q.y;  z -= q.z; 
	}
#endif
}
/*----------------------------------------------------------------*/
VSQuat VSQuat::operator - (const VSQuat &q) const
{
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(q.x,q.y,q.z,q.w);
		__m128 _r = _mm_sub_ps(_q0,_f);
		return VSQuat(_r.m128_f32[3],_r.m128_f32[2],_r.m128_f32[1],_r.m128_f32[0]);
	}
#else
	{	
		return VSQuat(x-q.x, y-q.y, z-q.z, w-q.w);
	}
#endif
}
/*----------------------------------------------------------------*/
void VSQuat::operator *= (const VSQuat &q) 
{
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(w, w, w, w);
		__m128 _s0 = _mm_set_ps(q.w, q.x, q.y, q.z);

		__m128 _q1 = _mm_set_ps(-x, x, y, z);
		__m128 _s1 = _mm_set_ps(q.x, q.w, q.w, q.w);

		__m128 _q2 = _mm_set_ps(-y, z, x, y);
		__m128 _s2 = _mm_set_ps(q.y, q.y, q.z, q.x);

		__m128 _q3 = _mm_set_ps(-z, -y, -z, -x);
		__m128 _s3 = _mm_set_ps(q.z, q.z, q.x, q.y);

		__m128 _q0s0 = _mm_mul_ps(_q0,_s0);
		__m128 _q1s1 = _mm_mul_ps(_q1,_s1);
		__m128 _q2s2 = _mm_mul_ps(_q2,_s2);
		__m128 _q3s3 = _mm_mul_ps(_q3,_s3);

		__m128 _r = _mm_add_ps(_q0s0,_mm_add_ps(_q1s1,_mm_add_ps(_q2s2,_q3s3)));

		w = _r.m128_f32[3];
		x = _r.m128_f32[2];
		y = _r.m128_f32[1];
		z = _r.m128_f32[0];
		

	}
#else
	{

		w = w*q.w - x*q.x - y*q.y - z*q.z;
		x = w*q.x + x*q.w + z*q.y - y*q.z;
		y = w*q.y + y*q.w + x*q.z - z*q.x;
		z = w*q.z + z*q.w + y*q.x - x*q.y;
	}
#endif
}
/*----------------------------------------------------------------*/


VSQuat VSQuat::operator * (const VSQuat &q) const 
{
	VSQuat qResult;
#ifdef VS_SSE
	{
		__m128 _q0 = _mm_set_ps(w,w,w,w);
		__m128 _s0 = _mm_set_ps(q.w,q.x,q.y,q.z);

		__m128 _q1 = _mm_set_ps(-x,x,y,z);
		__m128 _s1 = _mm_set_ps(q.x,q.w,q.w,q.w);

		__m128 _q2 = _mm_set_ps(-y,z,x,y);
		__m128 _s2 = _mm_set_ps(q.y,q.y,q.z,q.x);

		__m128 _q3 = _mm_set_ps(-z,-y,-z,-x);
		__m128 _s3 = _mm_set_ps(q.z,q.z,q.x,q.y);

		__m128 _q0s0 = _mm_mul_ps(_q0,_s0);
		__m128 _q1s1 = _mm_mul_ps(_q1,_s1);
		__m128 _q2s2 = _mm_mul_ps(_q2,_s2);
		__m128 _q3s3 = _mm_mul_ps(_q3,_s3);

		__m128 _r = _mm_add_ps(_q0s0,_mm_add_ps(_q1s1,_mm_add_ps(_q2s2,_q3s3)));

		qResult.w = _r.m128_f32[3];
		qResult.x = _r.m128_f32[2];
		qResult.y = _r.m128_f32[1];
		qResult.z = _r.m128_f32[0];
		

	}
#else
	{
		qResult.w = w*q.w - x*q.x - y*q.y - z*q.z;
		qResult.x = w*q.x + x*q.w + z*q.y - y*q.z;
		qResult.y = w*q.y + y*q.w + x*q.z - z*q.x;
		qResult.z = w*q.z + z*q.w + y*q.x - x*q.y;
	}
#endif
	return qResult;
}
/*----------------------------------------------------------------*/
VSQuat VSQuat::operator * (const VSVector3 &v) const 
{
	VSQuat t(v.x, v.y, v.z, 0);
	return (*this) * t;
}
/*----------------------------------------------------------------*/
bool VSQuat::operator ==(const VSQuat &q)const
{
	for (unsigned int i = 0 ; i < 4 ; i++)
	{
		if (ABS(m[i] - q.m[i]) > EPSILON_E4)
		{
			return false;
		}
	}
	return true;
}
/*----------------------------------------------------------------*/
void VSQuat::Rotate(const VSQuat &q1, const VSQuat &q2) 
{ 
	VSQuat m1 = q1;
	VSQuat m2 = q2;
	m1.Normalize();
	m2.Normalize();
	VSQuat t = (~m1)*m2*m1;
	x=t.x; y=t.y; z=t.z; w=t.w; 
}
/*----------------------------------------------------------------*/
VSVector3 VSQuat::Rotate(const VSVector3 &v)const 
{ 
	VSQuat t(x, y, z, w);
	t.Normalize();
	VSQuat r = (~t)*v*t;
	return VSVector3(r.x, r.y, r.z); 
}
/*----------------------------------------------------------------*/
VSREAL VSQuat::Dot(const VSQuat& q)const
{
#ifdef VS_SSE
	{
	__m128 _q0 = _mm_set_ps(x,y,z,w);
	__m128 _f = _mm_set_ps(q.x,q.y,q.z,q.w);
	__m128 _r = _mm_mul_ps(_q0,_f);
	return _r.m128_f32[3] + _r.m128_f32[1] + _r.m128_f32[2] +_r.m128_f32[0];
	}
#else
	{
	return (w * q.w + x * q.x + y * q.y + z * q.z);
	}
#endif

}
/*----------------------------------------------------------------*/
VSQuat VSQuat::operator ~(void) const
{ 
	return VSQuat(-x, -y, -z, w); 
}
/*----------------------------------------------------------------*/
VSQuat VSQuat::GetInverse()const
{
	return (GetConjugate()/GetLength());

}
/*----------------------------------------------------------------*/
void VSQuat::CreateAxisAngle(const VSVector3& Axis,VSREAL fAngle)
{
	VSREAL fTemp = fAngle/2.0f;
	VSREAL Sin = SIN(fTemp);
	VSREAL Cos = COS(fTemp);
	
	VSVector3 N = Axis;
	N.Normalize();
	
	w = Cos;
	x = Sin * N.x;
	y = Sin * N.y;
	z = Sin * N.z;
}
/*----------------------------------------------------------------*/
void VSQuat::GetAxisAngle(VSVector3 & Axis , VSREAL & fAngle)const
{

	VSQuat QTemp = *this;
	VSREAL fHalfAngle = ACOS(QTemp.w);

	fAngle = fHalfAngle * 2.0f;

	VSREAL SinHalfAngle = 1.0f - QTemp.w * QTemp.w;

	VSREAL InvSinHalfAngle = 1.0f / SinHalfAngle;
	Axis.x = QTemp.x * InvSinHalfAngle;
	Axis.y = QTemp.y * InvSinHalfAngle;
	Axis.z = QTemp.z * InvSinHalfAngle;


}
/*----------------------------------------------------------------*/
void VSQuat::Slerp(VSREAL t,const VSQuat & q1,const VSQuat & q2)
{
	if(t <= 0)  
	{
		(*this) = q1;
		return ;
	}
	if(t >= 1)  
	{
		(*this) = q2;
		return;
	}

	

	VSQuat q1Temp,q2Temp;
	q1Temp = q1;
	q2Temp = q2;
	q1Temp.Normalize();
	q2Temp.Normalize();
	VSREAL fCos = q1Temp.Dot(q2Temp);
	if(fCos < 0.0f)
	{
		q1Temp *= -1.0f;
		fCos = -fCos;
	}
	VSREAL k1 ,k2;
	if(fCos > VSREAL_1)
	{
		k1 = 1.0f - t;
		k2 = t;
	}
	else
	{
		VSREAL fSin = SQRT(1.0f - fCos * fCos);
		VSREAL fAngle = ATAN2(fSin, fCos);
		VSREAL fInvSin = 1.0f / fSin;
		k1 = SIN((1.0f - t) * fAngle) * fInvSin;
		k2 = SIN(t * fAngle) * fInvSin;

	
	}
	x = k1 * q1Temp.x + k2 * q2Temp.x;
	y = k1 * q1Temp.y + k2 * q2Temp.y;
	z = k1 * q1Temp.z + k2 * q2Temp.z;
	w = k1 * q1Temp.w + k2 * q2Temp.w;


}
/*----------------------------------------------------------------*/
VSQuat VSQuat::Pow(VSREAL exp)const
{
	if(ABS(w) > VSREAL_1)
		return *this;

	VSREAL alpha = ACOS(w);
	VSREAL newAlpha = alpha * exp;

	VSREAL mult = SIN(newAlpha) / SIN(alpha);
	return VSQuat(x * mult , y * mult , z * mult,COS(newAlpha)); 
}
/*----------------------------------------------------------------*/
VSQuat VSQuat::Ln()const
{
	if(ABS(w) > VSREAL_1)
		return VSQuat(x , y , z , 0.0f);;
	VSREAL alpha = ACOS(w);

	VSREAL Sin = SIN(alpha);
	VSREAL temp = alpha / Sin;
	return VSQuat( temp * x  , temp * y ,temp * z  , 0.0f);
	
}
/*----------------------------------------------------------------*/
VSQuat VSQuat::Exp()const
{
	VSREAL alpha = GetLength();
	VSREAL Sin = SIN(alpha);
	if(ABS(Sin) > EPSILON_E4)
	{
		VSREAL temp = Sin / alpha;
		return VSQuat(x * temp , y * temp , z * temp,COS(alpha));
	
	}
	else
	{
		return VSQuat(x  , y , z ,COS(alpha));
	}
}
/*----------------------------------------------------------------*/
void VSQuat::SlerpSValueOf(const VSQuat & q1,const VSQuat & q2,const VSQuat & q3)
{
	VSQuat q2Inv = q2.GetInverse();
	VSQuat K1 = q1 * q2;
	VSQuat K2 = q3 * q2;

	VSQuat K3 = ( K1.Ln() + K2.Ln() ) * (-0.25f);
	*this = q2 * K3.Exp();
	

}
/*----------------------------------------------------------------*/
void VSQuat::Slerp(VSREAL t,const VSQuat & q1,const VSQuat & q2,const VSQuat & s1,const VSQuat & s2)
{
	VSREAL fSlerpT = 2.0f * t * (1.0f - t);
	VSQuat SlerpP;
	SlerpP.Slerp(t,q1,q2);
	VSQuat SlerpQ;
	SlerpQ.Slerp(t,s1,s2);
	Slerp(fSlerpT,SlerpP,SlerpQ);

}
/*----------------------------------------------------------------*/
void VSQuat::TriangleSlerp(VSREAL t1,VSREAL t2, const VSQuat & q1,const VSQuat & q2,const VSQuat & q3)
{
	*this = q1.Pow(t1) + q2.Pow(t2) + q3.Pow(1.0f - t1 - t2);
}