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
	//ͨ����ת�����ת�ǹ�����Ԫ��
	void CreateAxisAngle(const VSVector3& Axis,VSREAL fAngle);
	//��ŷ���ǹ�����Ԫ��
	void  CreateEuler(VSREAL fRoll, VSREAL fPitch, VSREAL fYaw);
	//��λ��
	void  Normalize();
	//����
	VSQuat  GetConjugate()const;
	//�õ�ŷ����
	void  GetEulers(VSREAL &fRoll, VSREAL &fPitch, VSREAL &fYaw)const;
	//����Ԫ���õ��任����
	void  GetMatrix(VSMatrix3X3 &Matrix)const;
	//�õ�����
	VSREAL GetLength(void)const;
	//ȡ����
	VSQuat GetInverse()const;
	//ȡ����ת�����ת��
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
	//����
	VSREAL Dot(const VSQuat& q)const;
	//����
	VSQuat operator ~(void) const;

	//��q2��q1��ת�����Ԫ��
	void Rotate(const VSQuat &q1, const VSQuat &q2);

	//��תһ������
	VSVector3 Rotate(const VSVector3 &v)const;
	//��ֵ
	void Slerp(VSREAL t,const VSQuat & q1,const VSQuat & q2);
	//������2ά���Ͳ�ֵ
	void TriangleSlerp(VSREAL t1,VSREAL t2, const VSQuat & q1,const VSQuat & q2,const VSQuat & q3);
	//��Ԫ��������ֵ
	void Slerp(VSREAL t,const VSQuat & q1,const VSQuat & q2,const VSQuat & s1,const VSQuat & s2);
	void SlerpSValueOf(const VSQuat & q1,const VSQuat & q2,const VSQuat & q3);
	//����
	VSQuat Pow(VSREAL exp)const;
	//����eΪ�׵Ķ���
	VSQuat Ln()const;
	//����eΪ��ָ��
	VSQuat Exp()const;



}; // class
/*----------------------------------------------------------------*/
}
#endif