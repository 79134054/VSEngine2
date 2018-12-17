#include "VSMath.h"
/*----------------------------------------------------------------*/
FORCEINLINE void VSMatrix3X3W::Identity(void) 
{
	VSMemset(m, 0, sizeof(VSMatrix3X3W));
	_00 = _11 = _22 = _33 = 1.0f;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSMatrix3X3W::GetTranslation(void)const
{ 
	return VSVector3(_30, _31, _32);
}
/*----------------------------------------------------------------*/
FORCEINLINE VSMatrix3X3W VSMatrix3X3W::operator * (const VSMatrix3X3W &Matirx) const 
{
	VSMatrix3X3W mResult;

	mResult.M[0][0] = 0;
	mResult.M[1][1] = 0;
	mResult.M[2][2] = 0;
	mResult.M[3][3] = 0;
#ifdef VS_SSE
	VSFastMul(*this,Matirx,mResult);
#else
	for(unsigned char i = 0; i < 4; i++) 
		for(unsigned char j = 0; j < 4; j++) 
			for(unsigned int k = 0 ; k < 4 ; k++)
				mResult.M[i][j] += M[i][k] * Matirx.M[k][j]; 
#endif		

	return mResult;
} 
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3W VSMatrix3X3W::operator * (const VSVector3W &vc)const
{
	VSVector3W vcResult;


	vcResult.x = vc.x * _00 + vc.y * _10 + vc.z * _20 + _30;
	vcResult.y = vc.x * _01 + vc.y * _11 + vc.z * _21 + _31;
	vcResult.z = vc.x * _02 + vc.y * _12 + vc.z * _22 + _32;
	vcResult.w = vc.x * _03 + vc.y * _13 + vc.z * _23 + _33;

	vcResult.x = vcResult.x / vcResult.w;
	vcResult.y = vcResult.y / vcResult.w;
	vcResult.z = vcResult.z / vcResult.w;
	vcResult.w = 1.0f;

	return vcResult;

}
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSMatrix3X3W::operator * (const VSVector3 &vc) const
{
	VSVector3 vcResult;

	vcResult.x = vc.x * _00 + vc.y * _10 + vc.z * _20 + _30;
	vcResult.y = vc.x * _01 + vc.y * _11 + vc.z * _21 + _31;
	vcResult.z = vc.x * _02 + vc.y * _12 + vc.z * _22 + _32;
	
	VSREAL w = vc.x * _03 + vc.y * _13 + vc.z * _23 + _33;

	vcResult.x = vcResult.x / w;
	vcResult.y = vcResult.y / w;
	vcResult.z = vcResult.z / w;


	return vcResult;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSMatrix3X3W::Apply3X3(const VSVector3 &v)const 
{
	VSREAL x,y,z;
	x = v.x * _00 + v.y * _10 + v.z * _20;
	y = v.x * _01 + v.y * _11 + v.z * _21;
	z = v.x * _02 + v.y * _12 + v.z * _22;

	return VSVector3(x,y,z);
}
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSMatrix3X3W::ApplyTranlaste(const VSVector3 &Point)const
{

	return VSVector3(Point.x + _30,Point.y + _31,Point.z + _32);
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSMatrix3X3W::TransposeOf(const VSMatrix3X3W &Matrix) 
{
	_00 = Matrix._00;
	_10 = Matrix._01;
	_20 = Matrix._02;
	_30 = Matrix._03;

	_01 = Matrix._10;
	_11 = Matrix._11;
	_21 = Matrix._12;
	_31 = Matrix._13;

	_02 = Matrix._20;
	_12 = Matrix._21;
	_22 = Matrix._22;
	_32 = Matrix._23;

	_03 = Matrix._30;
	_13 = Matrix._31;
	_23 = Matrix._32;
	_33 = Matrix._33;
}
/*----------------------------------------------------------------*/
VSMatrix3X3W VSMatrix3X3W::GetTranspose()const
{
	VSMatrix3X3W Temp;
	Temp.TransposeOf(*this);
	return Temp;
}
/*----------------------------------------------------------------*/
VSMatrix3X3W VSMatrix3X3W::GetInverse()const
{
	VSMatrix3X3W Temp;
	Temp.InverseOf(*this);
	return Temp;
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSMatrix3X3W::InverseOf(const VSMatrix3X3W &Matirx) 
{

	VSMatrix3X3W mTrans;
	VSREAL    fTemp[12], fDet;


	mTrans.TransposeOf(Matirx);


	fTemp[ 0]  = mTrans._22 * mTrans._33;
	fTemp[ 1]  = mTrans._23 * mTrans._32;
	fTemp[ 2]  = mTrans._21 * mTrans._33;
	fTemp[ 3]  = mTrans._23 * mTrans._31;
	fTemp[ 4]  = mTrans._21 * mTrans._32;
	fTemp[ 5]  = mTrans._22 * mTrans._31;
	fTemp[ 6]  = mTrans._20 * mTrans._33;
	fTemp[ 7]  = mTrans._23 * mTrans._30;
	fTemp[ 8]  = mTrans._20 * mTrans._32;
	fTemp[ 9]  = mTrans._22 * mTrans._30;
	fTemp[10]  = mTrans._20 * mTrans._31;
	fTemp[11]  = mTrans._21 * mTrans._30;


	_00  = fTemp[0]*mTrans._11 + fTemp[3]*mTrans._12 + fTemp[4] *mTrans._13;
	_00 -= fTemp[1]*mTrans._11 + fTemp[2]*mTrans._12 + fTemp[5] *mTrans._13;
	_01  = fTemp[1]*mTrans._10 + fTemp[6]*mTrans._12 + fTemp[9] *mTrans._13;
	_01 -= fTemp[0]*mTrans._10 + fTemp[7]*mTrans._12 + fTemp[8] *mTrans._13;
	_02  = fTemp[2]*mTrans._10 + fTemp[7]*mTrans._11 + fTemp[10]*mTrans._13;
	_02 -= fTemp[3]*mTrans._10 + fTemp[6]*mTrans._11 + fTemp[11]*mTrans._13;
	_03  = fTemp[5]*mTrans._10 + fTemp[8]*mTrans._11 + fTemp[11]*mTrans._12;
	_03 -= fTemp[4]*mTrans._10 + fTemp[9]*mTrans._11 + fTemp[10]*mTrans._12;
	_10  = fTemp[1]*mTrans._01 + fTemp[2]*mTrans._02 + fTemp[5] *mTrans._03;
	_10 -= fTemp[0]*mTrans._01 + fTemp[3]*mTrans._02 + fTemp[4] *mTrans._03;
	_11  = fTemp[0]*mTrans._00 + fTemp[7]*mTrans._02 + fTemp[8] *mTrans._03;
	_11 -= fTemp[1]*mTrans._00 + fTemp[6]*mTrans._02 + fTemp[9] *mTrans._03;
	_12  = fTemp[3]*mTrans._00 + fTemp[6]*mTrans._01 + fTemp[11]*mTrans._03;
	_12 -= fTemp[2]*mTrans._00 + fTemp[7]*mTrans._01 + fTemp[10]*mTrans._03;
	_13  = fTemp[4]*mTrans._00 + fTemp[9]*mTrans._01 + fTemp[10]*mTrans._02;
	_13 -= fTemp[5]*mTrans._00 + fTemp[8]*mTrans._01 + fTemp[11]*mTrans._02;


	fTemp[ 0]  = mTrans._02 * mTrans._13;
	fTemp[ 1]  = mTrans._03 * mTrans._12;
	fTemp[ 2]  = mTrans._01 * mTrans._13;
	fTemp[ 3]  = mTrans._03 * mTrans._11;
	fTemp[ 4]  = mTrans._01 * mTrans._12;
	fTemp[ 5]  = mTrans._02 * mTrans._11;
	fTemp[ 6]  = mTrans._00 * mTrans._13;
	fTemp[ 7]  = mTrans._03 * mTrans._10;
	fTemp[ 8]  = mTrans._00 * mTrans._12;
	fTemp[ 9]  = mTrans._02 * mTrans._10;
	fTemp[10]  = mTrans._00 * mTrans._11;
	fTemp[11]  = mTrans._01 * mTrans._10;


	_20  = fTemp[0] *mTrans._31 + fTemp[3] *mTrans._32 + fTemp[4] *mTrans._33;
	_20 -= fTemp[1] *mTrans._31 + fTemp[2] *mTrans._32 + fTemp[5] *mTrans._33;
	_21  = fTemp[1] *mTrans._30 + fTemp[6] *mTrans._32 + fTemp[9] *mTrans._33;
	_21 -= fTemp[0] *mTrans._30 + fTemp[7] *mTrans._32 + fTemp[8] *mTrans._33;
	_22  = fTemp[2] *mTrans._30 + fTemp[7] *mTrans._31 + fTemp[10]*mTrans._33;
	_22 -= fTemp[3] *mTrans._30 + fTemp[6] *mTrans._31 + fTemp[11]*mTrans._33;
	_23  = fTemp[5] *mTrans._30 + fTemp[8] *mTrans._31 + fTemp[11]*mTrans._32;
	_23 -= fTemp[4] *mTrans._30 + fTemp[9] *mTrans._31 + fTemp[10]*mTrans._32;
	_30  = fTemp[2] *mTrans._22 + fTemp[5] *mTrans._23 + fTemp[1] *mTrans._21;
	_30 -= fTemp[4] *mTrans._23 + fTemp[0] *mTrans._21 + fTemp[3] *mTrans._22;
	_31  = fTemp[8] *mTrans._23 + fTemp[0] *mTrans._20 + fTemp[7] *mTrans._22;
	_31 -= fTemp[6] *mTrans._22 + fTemp[9] *mTrans._23 + fTemp[1] *mTrans._20;
	_32  = fTemp[6] *mTrans._21 + fTemp[11]*mTrans._23 + fTemp[3] *mTrans._20;
	_32 -= fTemp[10]*mTrans._23 + fTemp[2] *mTrans._20 + fTemp[7] *mTrans._21;
	_33  = fTemp[10]*mTrans._22 + fTemp[4] *mTrans._20 + fTemp[9] *mTrans._21;
	_33 -= fTemp[8] *mTrans._21 + fTemp[11]*mTrans._22 + fTemp[5] *mTrans._20;

	fDet = mTrans._00*_00 + mTrans._01*_01 + mTrans._02*_02 + mTrans._03*_03;

	fDet = 1/fDet;

	_00 *= fDet;  
	_01 *= fDet;  
	_02 *= fDet;  
	_03 *= fDet;

	_10 *= fDet;  
	_11 *= fDet;  
	_12 *= fDet;  
	_13 *= fDet;

	_20 *= fDet;  
	_21 *= fDet;  
	_22 *= fDet;  
	_23 *= fDet;

	_30 *= fDet;  
	_31 *= fDet;  
	_32 *= fDet;  
	_33 *= fDet;


} // func
/*----------------------------------------------------------------*/
FORCEINLINE void VSMatrix3X3W::Get3X3(VSMatrix3X3 & Mat)const
{
	for(int i = 0 ; i < 3 ; i++)
		for(int j = 0 ; j < 3 ; j++)
		{
			Mat.M[i][j] = M[i][j];
		
		}

}
/*----------------------------------------------------------------*/
FORCEINLINE void VSMatrix3X3W::SetZero()
{
	VSMemset(M,0,sizeof(VSMatrix3X3W));
}
