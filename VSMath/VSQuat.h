#ifndef VSQUAT_H
#define VSQUAT_H
namespace VSEngine2
{
class VSMatrix3X3;
class VSVector3;
class  VSMATH_API VSQuat 
{
public:
	union
	{
		VSREAL m[4];
		struct  
		{
			VSREAL x, y, z, w;
		};
	};

	VSQuat(void);
	VSQuat(VSREAL _x, VSREAL _y, VSREAL _z, VSREAL _w);
	void Set(VSREAL _x, VSREAL _y, VSREAL _z, VSREAL _w);
	//通过旋转轴和旋转角构造四元数
	void CreateAxisAngle(const VSVector3& Axis,VSREAL fAngle);
	//由欧拉角构造四元数
	void  CreateEuler(VSREAL fRoll, VSREAL fPitch, VSREAL fYaw);
	//单位化
	void  Normalize();
	//求共轭
	VSQuat  GetConjugate()const;
	//得到欧拉角
	void  GetEulers(VSREAL &fRoll, VSREAL &fPitch, VSREAL &fYaw)const;
	//从四元数得到变换矩阵
	void  GetMatrix(VSMatrix3X3 &Matrix)const;
	//得到长度
	VSREAL GetLength(void)const;
	//取得逆
	VSQuat GetInverse()const;
	//取得旋转轴和旋转角
	void GetAxisAngle(VSVector3 & Axis , VSREAL & fAngle)const;
	void    operator /= (VSREAL f);
	VSQuat operator /  (VSREAL f)const;

	void    operator *= (VSREAL f);
	VSQuat operator *  (VSREAL f)const;

	VSQuat operator *  (const VSVector3 &v) const;

	VSQuat operator *  (const VSQuat &q) const;
	void    operator *= (const VSQuat &q);

	void    operator += (const VSQuat &q);
	VSQuat operator +  (const VSQuat &q) const;

	void    operator -= (const VSQuat &q);
	VSQuat operator -  (const VSQuat &q) const;

	bool operator ==(const VSQuat &q)const;
	//求点积
	VSREAL Dot(const VSQuat& q)const;
	//求共轭
	VSQuat operator ~(void) const;

	//求q2绕q1旋转后的四元数
	void Rotate(const VSQuat &q1, const VSQuat &q2);

	//旋转一个向量
	VSVector3 Rotate(const VSVector3 &v)const;
	//插值
	void Slerp(VSREAL t,const VSQuat & q1,const VSQuat & q2);
	//三角形2维球型插值
	void TriangleSlerp(VSREAL t1,VSREAL t2, const VSQuat & q1,const VSQuat & q2,const VSQuat & q3);
	//四元数样条插值
	void Slerp(VSREAL t,const VSQuat & q1,const VSQuat & q2,const VSQuat & s1,const VSQuat & s2);
	void SlerpSValueOf(const VSQuat & q1,const VSQuat & q2,const VSQuat & q3);
	//求幂
	VSQuat Pow(VSREAL exp)const;
	//求以e为底的对数
	VSQuat Ln()const;
	//求以e为底指数
	VSQuat Exp()const;



}; // class
/*----------------------------------------------------------------*/
}
#endif