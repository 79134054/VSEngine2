#include "VSMatrix3X3W.h"
#include "VSMath.h"
using namespace VSEngine2;
VSMatrix3X3W VSMatrix3X3W::ms_Materix3X3WIdentity;
/*----------------------------------------------------------------*/
VSMatrix3X3W::VSMatrix3X3W(void)
{
	Identity();

}
/*----------------------------------------------------------------*/
VSMatrix3X3W::VSMatrix3X3W(VSREAL m00,VSREAL m01,VSREAL m02,VSREAL m03,
							 VSREAL m10,VSREAL m11,VSREAL m12,VSREAL m13,
							 VSREAL m20,VSREAL m21,VSREAL m22,VSREAL m23,
							 VSREAL m30,VSREAL m31,VSREAL m32,VSREAL m33)
{

	_00 = m00; _01 = m01; _02 = m02; _03 = m03;
	_10 = m10; _11 = m11; _12 = m12; _13 = m13;
	_20 = m20; _21 = m21; _22 = m22; _23 = m23;
	_30 = m30; _31 = m31; _32 = m32; _33 = m33;
}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::CreateTranslate(VSREAL dx, VSREAL dy, VSREAL dz) 
{
	Identity();
	_30 = dx;
	_31 = dy;
	_32 = dz;
}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::CreateTranslate(const VSVector3 & V)
{
	Identity();
	_30 = V.x;
	_31 = V.y;
	_32 = V.z;

}
/*----------------------------------------------------------------*/

void VSMatrix3X3W::CreateFormBillboard(const VSVector3 &vcPos, 
										const VSMatrix3X3 &CameraRotMatrix,
										bool bAxisY) 
{
	VSMatrix3X3 Mat;
	Mat.InverseOf(CameraRotMatrix);
	CreateFrom3X3(Mat);
	_30 = vcPos.x;
	_31 = vcPos.y;
	_32 = vcPos.z;
	_33 = 1.0f;
	/*
	如果只进行按Y轴旋转，那么只需要假设相机矩阵A的Y分量是(0,1,0)
	A逆 * A = E
		x1,y1,z1
	A=( x2,y2,z2	)
		x3,y3,z3

			a1,a2,a3
	A逆=(	b1,b2,b3	)
			c1,c2,c3

	因为A为正交矩阵，从列看，也就是列向量为单位向量，且两两正交，也就是点积为0

	所以A逆也为正交矩阵，从行看。

	其实正交矩阵从行从列看都无所谓，只不过为了和相机矩阵列向量结合，讲解方便

	为了只让广告牌只绕Y轴旋转，我们假设相机矩阵Y轴向量始终为(0,1,0),也就说相机矩阵

	Y分量向上，这样求出的逆矩阵也只能绕Y轴旋转了。

	但我们不用在重新设置相机矩阵，只需要修改逆矩阵就可以。

	假设

		x1,0,z1
	A=( x2,1,z2	)
		x3,0,z3

	（这样改完后，因为X列向量和Z列向量是按照严格相机矩阵过程求得,
	过程为假设Y向上(0,1,0),然后求视点和观察点向量E，它们做叉积，求得右向量R
	然后在用右向量R和E求得上方向向量。

	修改的A，我们这里没有在继续求上方向，所以不能保证A为正交矩阵）

			a1,a2,a3
	A逆=(	0,1,0	)
			c1,c2,c3

	*/
	if(bAxisY)
	{
		_10 = 0.0f;
		_11 = 1.0f;
		_12 = 0.0f;

	}
} // Billboard
/*----------------------------------------------------------------*/
bool VSMatrix3X3W::CreateFromLookDir(const VSVector3 &vcPos,					
									const VSVector3 &vcDir,					
									const VSVector3 &vcWorldUp)
{
	VSVector3 vcTemp, vcUp;
	VSVector3 Dir = vcDir;
	Dir.Normalize();

	//计算朝向方向和上方向点积,因为Dir为单位向量,所以fDot为vcWorld在
	//Dir上的投影
	VSREAL fDot = vcWorldUp.Dot(Dir);
	//计算投影长度,方向和Dir相同的向量
	vcTemp = Dir * fDot;
	//根据向量的平行4边形法则求出up
	/*
			  /|
worldup-->	 / |<---up
			/__|__ 
	vcTemp
	*/				
	vcUp = vcWorldUp - vcTemp;

	//求出长度
	VSREAL fL = vcUp.GetLength();

	// 长度为0,说明 vcDir和worldUp几乎平行,交角几乎为0.
	if (fL < EPSILON_E4) 
	{
		VSVector3 vcY;
		//从新选择向上向量,计算up向量
		vcY.Set(0.0f, 0.0f, 1.0f);

		vcTemp = Dir * Dir.z;
		vcUp = vcY - vcTemp;


		fL = vcUp.GetLength();
		// 长度为0,说明 vcDir和worldUp几乎平行,交角几乎为0.
		if (fL < EPSILON_E4) 
		{
			
			//从新选择向上向量,计算up向量
			vcY.Set(1.0f, 0.0f, 0.0f);

			vcTemp = Dir * Dir.x;
			vcUp = vcY - vcTemp;

			fL = vcUp.GetLength();

			
				if(fL < EPSILON_E4) return 0;
			
		}
	}
	//单位化
	vcUp /= fL;


	VSVector3 vcRight;
	vcRight.Cross(vcUp, Dir);

	CreateInWorldObject(vcRight, vcUp, Dir, vcPos);
	return 1;

}//LookDir
/*----------------------------------------------------------------*/
bool VSMatrix3X3W::CreateFromLookAt(const VSVector3 &vcPos, const VSVector3 &vcLookAt,
									const VSVector3 &vcWorldUp) 
{
	VSVector3 vcDir;
	//计算朝向
	vcDir = vcLookAt - vcPos;
	return CreateFromLookDir(vcPos,vcDir,vcWorldUp);
	
} // LookAt
/*----------------------------------------------------------------*/
void VSMatrix3X3W::CreateInWorldObject(const VSVector3 &U,const VSVector3 &V,const VSVector3 & N,const VSVector3 &Point)
{
	_00 = U.x;
	_10 = U.y;
	_20 = U.z;

	_01 = V.x;
	_11 = V.y;
	_21 = V.z;

	_02 = N.x;
	_12 = N.y;
	_22 = N.z;

	_03=0.0f; _13=0.0f; _23=0.0f; _33=1.0f;

	_30 = -(U.Dot(Point));
	_31 = -(V.Dot(Point));
	_32 = -(N.Dot(Point));

}
/*----------------------------------------------------------------*/
VSMatrix3X3W VSMatrix3X3W::operator +(VSREAL f)const
{

	VSMatrix3X3W Temp;
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(m[0],m[1],m[2],m[3]);
		__m128 _v2 = _mm_set_ps(m[4],m[5],m[6],m[7]);
		__m128 _v3 = _mm_set_ps(m[8],m[9],m[10],m[11]);
		__m128 _v4 = _mm_set_ps(m[12],m[13],m[14],m[15]);
		__m128 _f = _mm_set_ps(f,f,f,f);

		__m128 _r1 = _mm_add_ps(_v1,_f);
		__m128 _r2 = _mm_add_ps(_v2,_f);
		__m128 _r3 = _mm_add_ps(_v3,_f);
		__m128 _r4 = _mm_add_ps(_v4,_f);

		Temp.M[0][0] = _r1.m128_f32[3]; Temp.M[0][1] = _r1.m128_f32[2]; Temp.M[0][2] = _r1.m128_f32[1]; Temp.M[0][3] = _r1.m128_f32[0];
		Temp.M[1][0] = _r2.m128_f32[3]; Temp.M[1][1] = _r2.m128_f32[2]; Temp.M[1][2] = _r2.m128_f32[1]; Temp.M[1][3] = _r2.m128_f32[0];
		Temp.M[2][0] = _r3.m128_f32[3]; Temp.M[2][1] = _r3.m128_f32[2]; Temp.M[2][2] = _r3.m128_f32[1]; Temp.M[2][3] = _r3.m128_f32[0];
		Temp.M[3][0] = _r4.m128_f32[3]; Temp.M[3][1] = _r4.m128_f32[2]; Temp.M[3][2] = _r4.m128_f32[1]; Temp.M[3][3] = _r4.m128_f32[0];
	}
#else
	{
		
		Temp.m[0] = m[0] + f;
		Temp.m[1] = m[1] + f;
		Temp.m[2] = m[2] + f;
		Temp.m[3] = m[3] + f;
		Temp.m[4] = m[4] + f;
		Temp.m[5] = m[5] + f;
		Temp.m[6] = m[6] + f;
		Temp.m[7] = m[7] + f;
		Temp.m[8] = m[8] + f;
		Temp.m[9] = m[9] + f;
		Temp.m[10] = m[10] + f;
		Temp.m[11] = m[11] + f;
		Temp.m[12] = m[12] + f;
		Temp.m[13] = m[13] + f;
		Temp.m[14] = m[14] + f;
		Temp.m[15] = m[15] + f;
		
	}
#endif
	return Temp;

}
/*----------------------------------------------------------------*/
VSMatrix3X3W VSMatrix3X3W::operator -(VSREAL f)const
{
	VSMatrix3X3W Temp;
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(m[0],m[1],m[2],m[3]);
		__m128 _v2 = _mm_set_ps(m[4],m[5],m[6],m[7]);
		__m128 _v3 = _mm_set_ps(m[8],m[9],m[10],m[11]);
		__m128 _v4 = _mm_set_ps(m[12],m[13],m[14],m[15]);
		__m128 _f = _mm_set_ps(f,f,f,f);

		__m128 _r1 = _mm_sub_ps(_v1,_f);
		__m128 _r2 = _mm_sub_ps(_v2,_f);
		__m128 _r3 = _mm_sub_ps(_v3,_f);
		__m128 _r4 = _mm_sub_ps(_v4,_f);

		Temp.M[0][0] = _r1.m128_f32[3]; Temp.M[0][1] = _r1.m128_f32[2]; Temp.M[0][2] = _r1.m128_f32[1]; Temp.M[0][3] = _r1.m128_f32[0];
		Temp.M[1][0] = _r2.m128_f32[3]; Temp.M[1][1] = _r2.m128_f32[2]; Temp.M[1][2] = _r2.m128_f32[1]; Temp.M[1][3] = _r2.m128_f32[0];
		Temp.M[2][0] = _r3.m128_f32[3]; Temp.M[2][1] = _r3.m128_f32[2]; Temp.M[2][2] = _r3.m128_f32[1]; Temp.M[2][3] = _r3.m128_f32[0];
		Temp.M[3][0] = _r4.m128_f32[3]; Temp.M[3][1] = _r4.m128_f32[2]; Temp.M[3][2] = _r4.m128_f32[1]; Temp.M[3][3] = _r4.m128_f32[0];
	}
#else
	{
		Temp.m[0] = m[0] - f;
		Temp.m[1] = m[1] - f;
		Temp.m[2] = m[2] - f;
		Temp.m[3] = m[3] - f;
		Temp.m[4] = m[4] - f;
		Temp.m[5] = m[5] - f;
		Temp.m[6] = m[6] - f;
		Temp.m[7] = m[7] - f;
		Temp.m[8] = m[8] - f;
		Temp.m[9] = m[9] - f;
		Temp.m[10] = m[10] - f;
		Temp.m[11] = m[11] - f;
		Temp.m[12] = m[12] - f;
		Temp.m[13] = m[13] - f;
		Temp.m[14] = m[14] - f;
		Temp.m[15] = m[15] - f;
	}
#endif
	return Temp;

}
/*----------------------------------------------------------------*/
VSMatrix3X3W VSMatrix3X3W::operator *(VSREAL f)const
{
	VSMatrix3X3W Temp;
	#ifdef VS_SSE
	{
		VSFastMul(*this,f,Temp);
	}
	#else
	{
		Temp.m[0] = m[0] * f;
		Temp.m[1] = m[1] * f;
		Temp.m[2] = m[2] * f;
		Temp.m[3] = m[3] * f;
		Temp.m[4] = m[4] * f;
		Temp.m[5] = m[5] * f;
		Temp.m[6] = m[6] * f;
		Temp.m[7] = m[7] * f;
		Temp.m[8] = m[8] * f;
		Temp.m[9] = m[9] * f;
		Temp.m[10] = m[10] * f;
		Temp.m[11] = m[11] * f;
		Temp.m[12] = m[12] * f;
		Temp.m[13] = m[13] * f;
		Temp.m[14] = m[14] * f;
		Temp.m[15] = m[15] * f;
	}
	#endif
	return Temp;
}
/*----------------------------------------------------------------*/
VSMatrix3X3W VSMatrix3X3W::operator +(const VSMatrix3X3W &Matirx)const
{

	VSMatrix3X3W Temp;
	#ifdef VS_SSE
	{
		VSFastAdd(*this,Matirx,Temp);
	}
	#else
	{
		Temp.m[0] = m[0] + Matirx.m[0];
		Temp.m[1] = m[1] + Matirx.m[1];
		Temp.m[2] = m[2] + Matirx.m[2];
		Temp.m[3] = m[3] + Matirx.m[3];
		Temp.m[4] = m[4] + Matirx.m[4];
		Temp.m[5] = m[5] + Matirx.m[5];
		Temp.m[6] = m[6] + Matirx.m[6];
		Temp.m[7] = m[7] + Matirx.m[7];
		Temp.m[8] = m[8] + Matirx.m[8];
		Temp.m[9] = m[9] + Matirx.m[9];
		Temp.m[10] = m[10] + Matirx.m[10];
		Temp.m[11] = m[11] + Matirx.m[11];
		Temp.m[12] = m[12] + Matirx.m[12];
		Temp.m[13] = m[13] + Matirx.m[13];
		Temp.m[14] = m[14] + Matirx.m[14];
		Temp.m[15] = m[15] + Matirx.m[15];
	}
	#endif
	return Temp;	

}
/*----------------------------------------------------------------*/
VSMatrix3X3W VSMatrix3X3W::operator -(const VSMatrix3X3W &Matirx)const
{
	VSMatrix3X3W Temp;
	#ifdef VS_SSE
	{
		VSFastSub(*this,Matirx,Temp);
	}
	#else
	{
		Temp.m[0] = m[0] - Matirx.m[0];
		Temp.m[1] = m[1] - Matirx.m[1];
		Temp.m[2] = m[2] - Matirx.m[2];
		Temp.m[3] = m[3] - Matirx.m[3];
		Temp.m[4] = m[4] - Matirx.m[4];
		Temp.m[5] = m[5] - Matirx.m[5];
		Temp.m[6] = m[6] - Matirx.m[6];
		Temp.m[7] = m[7] - Matirx.m[7];
		Temp.m[8] = m[8] - Matirx.m[8];
		Temp.m[9] = m[9] - Matirx.m[9];
		Temp.m[10] = m[10] - Matirx.m[10];
		Temp.m[11] = m[11] - Matirx.m[11];
		Temp.m[12] = m[12] - Matirx.m[12];
		Temp.m[13] = m[13] - Matirx.m[13];
		Temp.m[14] = m[14] - Matirx.m[14];
		Temp.m[15] = m[15] - Matirx.m[15];
	}
	#endif
	return Temp;	

}


/*----------------------------------------------------------------*/
void VSMatrix3X3W::operator +=(VSREAL f)
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(m[0],m[1],m[2],m[3]);
		__m128 _v2 = _mm_set_ps(m[4],m[5],m[6],m[7]);
		__m128 _v3 = _mm_set_ps(m[8],m[9],m[10],m[11]);
		__m128 _v4 = _mm_set_ps(m[12],m[13],m[14],m[15]);
		__m128 _f = _mm_set_ps(f,f,f,f);

		__m128 _r1 = _mm_add_ps(_v1,_f);
		__m128 _r2 = _mm_add_ps(_v2,_f);
		__m128 _r3 = _mm_add_ps(_v3,_f);
		__m128 _r4 = _mm_add_ps(_v4,_f);

		M[0][0] = _r1.m128_f32[3]; M[0][1] = _r1.m128_f32[2]; M[0][2] = _r1.m128_f32[1]; M[0][3] = _r1.m128_f32[0];
		M[1][0] = _r2.m128_f32[3]; M[1][1] = _r2.m128_f32[2]; M[1][2] = _r2.m128_f32[1]; M[1][3] = _r2.m128_f32[0];
		M[2][0] = _r3.m128_f32[3]; M[2][1] = _r3.m128_f32[2]; M[2][2] = _r3.m128_f32[1]; M[2][3] = _r3.m128_f32[0];
		M[3][0] = _r4.m128_f32[3]; M[3][1] = _r4.m128_f32[2]; M[3][2] = _r4.m128_f32[1]; M[3][3] = _r4.m128_f32[0];
	}
#else
	{
		m[0] = m[0] + f;
		m[1] = m[1] + f;
		m[2] = m[2] + f;
		m[3] = m[3] + f;
		m[4] = m[4] + f;
		m[5] = m[5] + f;
		m[6] = m[6] + f;
		m[7] = m[7] + f;
		m[8] = m[8] + f;
		m[9] = m[9] + f;
		m[10] = m[10] + f;
		m[11] = m[11] + f;
		m[12] = m[12] + f;
		m[13] = m[13] + f;
		m[14] = m[14] + f;
		m[15] = m[15] + f;
	}
#endif


}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::operator -=(VSREAL f)
{
#ifdef VS_SSE
	{
		__m128 _v1 = _mm_set_ps(m[0],m[1],m[2],m[3]);
		__m128 _v2 = _mm_set_ps(m[4],m[5],m[6],m[7]);
		__m128 _v3 = _mm_set_ps(m[8],m[9],m[10],m[11]);
		__m128 _v4 = _mm_set_ps(m[12],m[13],m[14],m[15]);
		__m128 _f = _mm_set_ps(f,f,f,f);

		__m128 _r1 = _mm_sub_ps(_v1,_f);
		__m128 _r2 = _mm_sub_ps(_v2,_f);
		__m128 _r3 = _mm_sub_ps(_v3,_f);
		__m128 _r4 = _mm_sub_ps(_v4,_f);

		M[0][0] = _r1.m128_f32[3]; M[0][1] = _r1.m128_f32[2]; M[0][2] = _r1.m128_f32[1]; M[0][3] = _r1.m128_f32[0];
		M[1][0] = _r2.m128_f32[3]; M[1][1] = _r2.m128_f32[2]; M[1][2] = _r2.m128_f32[1]; M[1][3] = _r2.m128_f32[0];
		M[2][0] = _r3.m128_f32[3]; M[2][1] = _r3.m128_f32[2]; M[2][2] = _r3.m128_f32[1]; M[2][3] = _r3.m128_f32[0];
		M[3][0] = _r4.m128_f32[3]; M[3][1] = _r4.m128_f32[2]; M[3][2] = _r4.m128_f32[1]; M[3][3] = _r4.m128_f32[0];
	}
#else
	{
		m[0] = m[0] - f;
		m[1] = m[1] - f;
		m[2] = m[2] - f;
		m[3] = m[3] - f;
		m[4] = m[4] - f;
		m[5] = m[5] - f;
		m[6] = m[6] - f;
		m[7] = m[7] - f;
		m[8] = m[8] - f;
		m[9] = m[9] - f;
		m[10] = m[10] - f;
		m[11] = m[11] - f;
		m[12] = m[12] - f;
		m[13] = m[13] - f;
		m[14] = m[14] - f;
		m[15] = m[15] - f;
	}
#endif

}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::operator *=(VSREAL f)
{
	#ifdef VS_SSE
	{
		VSFastMul(*this,f,*this);
	}
	#else
	{
		m[0] = m[0] * f;
		m[1] = m[1] * f;
		m[2] = m[2] * f;
		m[3] = m[3] * f;
		m[4] = m[4] * f;
		m[5] = m[5] * f;
		m[6] = m[6] * f;
		m[7] = m[7] * f;
		m[8] = m[8] * f;
		m[9] = m[9] * f;
		m[10] = m[10] * f;
		m[11] = m[11] * f;
		m[12] = m[12] * f;
		m[13] = m[13] * f;
		m[14] = m[14] * f;
		m[15] = m[15] * f;
	}
	#endif
}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::operator +=(const VSMatrix3X3W &Matirx)
{

	VSMatrix3X3W Temp;
	#ifdef VS_SSE
	{
		VSFastAdd(*this,Matirx,*this);
	}
	#else
	{
		m[0] = m[0] + Matirx.m[0];
		m[1] = m[1] + Matirx.m[1];
		m[2] = m[2] + Matirx.m[2];
		m[3] = m[3] + Matirx.m[3];
		m[4] = m[4] + Matirx.m[4];
		m[5] = m[5] + Matirx.m[5];
		m[6] = m[6] + Matirx.m[6];
		m[7] = m[7] + Matirx.m[7];
		m[8] = m[8] + Matirx.m[8];
		m[9] = m[9] + Matirx.m[9];
		m[10] = m[10] + Matirx.m[10];
		m[11] = m[11] + Matirx.m[11];
		m[12] = m[12] + Matirx.m[12];
		m[13] = m[13] + Matirx.m[13];
		m[14] = m[14] + Matirx.m[14];
		m[15] = m[15] + Matirx.m[15];
	}
	#endif

}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::operator -=(const VSMatrix3X3W &Matirx)
{

	VSMatrix3X3W Temp;
	#ifdef VS_SSE
	{
		VSFastSub(*this,Matirx,*this);
	}
	#else
	{
		m[0] = m[0] - Matirx.m[0];
		m[1] = m[1] - Matirx.m[1];
		m[2] = m[2] - Matirx.m[2];
		m[3] = m[3] - Matirx.m[3];
		m[4] = m[4] - Matirx.m[4];
		m[5] = m[5] - Matirx.m[5];
		m[6] = m[6] - Matirx.m[6];
		m[7] = m[7] - Matirx.m[7];
		m[8] = m[8] - Matirx.m[8];
		m[9] = m[9] - Matirx.m[9];
		m[10] = m[10] - Matirx.m[10];
		m[11] = m[11] - Matirx.m[11];
		m[12] = m[12] - Matirx.m[12];
		m[13] = m[13] - Matirx.m[13];
		m[14] = m[14] - Matirx.m[14];
		m[15] = m[15] - Matirx.m[15];
	}
	#endif
}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::CreateFrom3X3(const VSMatrix3X3 & Mat)
{
	Identity();
	for(int i = 0 ; i < 3 ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
		{
			M[i][j] = Mat.M[i][j];
		}
	
	}
	
}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::Add3X3(const VSMatrix3X3 & Mat)
{
	for(int i = 0 ; i < 3 ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
		{
			M[i][j] = Mat.M[i][j];
		}

	}
}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::AddTranslate(const VSVector3 & V)
{
	_30 = V.x;
	_31 = V.y;
	_32 = V.z;
}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::AddTranslate(VSREAL dx, VSREAL dy, VSREAL dz)
{
	_30 = dx;
	_31 = dy;
	_32 = dz;
}
/*----------------------------------------------------------------*/
bool VSMatrix3X3W::CreatePerspective(VSREAL fFov , VSREAL fAspect, VSREAL fZN ,VSREAL fZF)
{
	
	if(ABS(fZF - fZN) < 0.01f)
		return 0;

	VSREAL sinFOV2 = SIN(fFov/2);

	if(ABS(sinFOV2) < EPSILON_E5)
		return 0;

	VSREAL cosFOV2 = COS(fFov/2);

	VSREAL w =  1.0f  *(cosFOV2 / sinFOV2);
	VSREAL h =   fAspect * (cosFOV2 / sinFOV2);
	VSREAL Q = fZF / (fZF - fZN);
	Identity();
	_00 = w;
	_11 = h;
	_22 = Q;
	_23 = 1.0f;
	_32 = -Q * fZN;
	_33 = 0; 
	return 1;

}
/*----------------------------------------------------------------*/
bool VSMatrix3X3W::CreateOrthogonal(VSREAL fW ,VSREAL fH,	VSREAL fZN ,VSREAL fZF)
{
	if(ABS(fZF - fZN) < 0.01f)
		return 0;
	Identity();
	_00 = 2.0f / fW;
	_11 = 2.0f / fH;
	_22 = 1.0f / (fZF - fZN);
	_32 = -fZN / (fZF - fZN);
	return 1;
}
/*----------------------------------------------------------------*/
bool VSMatrix3X3W::CreateViewPort(VSREAL fX,VSREAL fY,VSREAL fWidth,VSREAL fHeight,VSREAL fMinz,VSREAL fMaxz)
{
	if(fMinz > fMaxz)
		return 0;
	Identity();
	_00 = fWidth * 0.5f ;
	_11 = - fHeight * 0.5f ;
	_22 = fMaxz - fMinz;
	_30 = fX + _00;
	_31 = fY - _11;
	_32 = fMinz;

	return 1;
	 
}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::GetRowVector(VSVector3W Row[4])const
{
	for(int i = 0 ; i < 4 ; i++)
	{
		for(int j = 0 ; j < 4 ; j++)
			Row[i].m[j] = M[i][j];

	}

}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::GetColumnVector(VSVector3W Column[4])const
{
	for(int i = 0 ; i < 4 ; i++)
	{
		for(int j = 0 ; j < 4 ; j++)
			Column[i].m[j] = M[j][i];

	}

}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::GetRowVector(VSVector3W &Row0,VSVector3W &Row1,VSVector3W &Row2,VSVector3W &Row3)const
{
	Row0.x = M[0][0] ; Row0.y = M[0][1] ; Row0.z = M[0][2] ; Row0.w = M[0][3] ;
	Row1.x = M[1][0] ; Row1.y = M[1][1] ; Row1.z = M[1][2] ; Row1.w = M[1][3] ;
	Row2.x = M[2][0] ; Row2.y = M[2][1] ; Row2.z = M[2][2] ; Row2.w = M[2][3] ;
	Row3.x = M[3][0] ; Row3.y = M[3][1] ; Row3.z = M[3][2] ; Row3.w = M[3][3] ;
}
/*----------------------------------------------------------------*/
void VSMatrix3X3W::GetColumnVector(VSVector3W &Column0,VSVector3W &Column1,VSVector3W &Column2,VSVector3W &Column3)const
{
	Column0.x = M[0][0] ; Column0.y = M[1][0] ; Column0.z = M[2][0] ; Column0.w = M[3][0] ;
	Column1.x = M[0][1] ; Column1.y = M[1][1] ; Column1.z = M[2][1] ; Column1.w = M[3][1] ;
	Column2.x = M[0][2] ; Column2.y = M[1][2] ; Column2.z = M[2][2] ; Column2.w = M[3][2] ;
	Column3.x = M[0][3] ; Column3.y = M[1][3] ; Column3.z = M[2][3] ; Column3.w = M[3][3] ; 

}
/*----------------------------------------------------------------*/
bool VSMatrix3X3W::operator ==(const VSMatrix3X3W &v)const
{
	for (unsigned int i = 0 ; i < 4 ; i++)
	{
		for (unsigned int j = 0 ; j < 4 ; j++)
		{
			if (ABS(M[i][j] - v.M[i][j]) > EPSILON_E4)
			{
				return false;
			}
		}
		
	}
	return true;
}
/*void VSMatrix3X3W::CreatePointLightPlaneShadow(const VSVector3 & LightPoint,const VSPlane3 &Plane)
{
	VSVector3 PlaneN = Plane.GetN();
	VSREAL fD = Plane.GetfD();
	VSREAL temp = PlaneN * LightPoint + fD;

	_00 = temp - PlaneN.x * LightPoint.x;
	_01 = -PlaneN.x * LightPoint.y;
	_02 = -PlaneN.x * LightPoint.z;
	_03 = -PlaneN.x;

	_10 = -PlaneN.y * LightPoint.x;
	_11 = temp - PlaneN.y * LightPoint.y;
	_12 = -PlaneN.y * LightPoint.z;
	_13 = -PlaneN.y;

	_20 = -PlaneN.z * LightPoint.x;
	_21 = -PlaneN.z * LightPoint.y;
	_22 = temp - PlaneN.z * LightPoint.z;
	_23 = -PlaneN.z;

	_30 = -fD * LightPoint.x ;
	_31 = -fD * LightPoint.y ;
	_32 = -fD * LightPoint.z ;
	_33 = PlaneN * LightPoint;
}
/*----------------------------------------------------------------*/
/*void VSMatrix3X3W::CreateInfiniteLightPlaneShadow(const VSVector3 & LightDir,const VSPlane3 &Plane)
{

	VSVector3 PlaneN = Plane.GetN();
	VSREAL fD = Plane.GetfD();
	VSREAL temp = PlaneN * LightDir;

	_00 = temp - PlaneN.x * LightDir.x;
	_01 = -PlaneN.x * LightDir.y;
	_02 = -PlaneN.x * LightDir.z;
	_03 = -PlaneN.x;

	_10 = -PlaneN.y * LightDir.x;
	_11 = temp - PlaneN.y * LightDir.y;
	_12 = -PlaneN.y * LightDir.z;
	_13 = -PlaneN.y;

	_20 = -PlaneN.z * LightDir.x;
	_21 = -PlaneN.z * LightDir.y;
	_22 = temp - PlaneN.z * LightDir.z;
	_23 = -PlaneN.z;

	_30 = -fD * LightDir.x ;
	_31 = -fD * LightDir.y ;
	_32 = -fD * LightDir.z ;
	_33 = PlaneN * LightDir;
}*/
