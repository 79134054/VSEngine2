#ifndef VSB_SPLINECURVE3_H
#define VSB_SPLINECURVE3_H
#include "VSControlCurve3.h"
/*
	B_Spline���ߣ����ڿ��Ƶ���������һ�֣��ɿ��Ƶ㣬�����������ߵĶȣ�����ʽ���,����t��0-1֮��

*/
namespace VSEngine2
{

	class VSMATH_API VSB_SplineCurve3:public VSControlCurve3
	{
	public:
		VSB_SplineCurve3();
		virtual ~VSB_SplineCurve3();
		//����
		bool Copy(const VSB_SplineCurve3 & B_SplineCurve3);
		//���ÿ��Ƶ�
		bool Set(const VSVector3 * pControlPoint,unsigned int uiControlPointNum);
		//�ѿ��Ƶ�任�ɱպϿ���ʽ���Ƶ�
		bool ChangeToOpenClosePoints();
		//�ѿ��Ƶ�任�����ڱպϿ��Ƶ�
		bool ChangeToPeriodicClosePoints(unsigned int uiDegree);
		//���ý�����
		bool SetKnotVector(const VSREAL * pKnotVector,unsigned int uiDegree,unsigned int uiControlPointNum);
		//���������Ⱦ������
		bool CreateOpenUniformKnotVector(unsigned int uiDegree,unsigned int uiControlPointNum);
		//�������ڽ�����
		bool CreatePeriodicKnotVector(unsigned int uiDegree,unsigned int uiControlPointNum);
		//����
		virtual VSVector3	GetFirstDerivative(VSREAL t);
		virtual VSVector3	GetSecondDerivative(VSREAL t);
		virtual VSVector3	GetThirdDerivative(VSREAL t);
		//virtual VSREAL		GetLength(VSREAL t1 ,VSREAL t2);
		//�õ�����ĵ�
		virtual VSVector3	GetPoint(VSREAL t);

	/***************************************FORCEINLINE************************************/

	public:
		//���ý�������һ��ֵ
		FORCEINLINE bool SetKnotVector(unsigned int i,VSREAL Value);
		//�õ�������ָ��
		FORCEINLINE const VSREAL * GetpKnotVector()const;
		//�õ���������һ��ֵ
		FORCEINLINE VSREAL GetKnotVectorValue(unsigned int i)const;
	protected:
		//�����������ʽֵ������
		FORCEINLINE void SetBasisValue(int i, int j, VSREAL Value);
		FORCEINLINE VSREAL GetBasisValue(int i ,int j)const;

		FORCEINLINE void SetBasisFDValue(int i, int j, VSREAL Value);
		FORCEINLINE VSREAL GetBasisFDValue(int i ,int j)const;

		FORCEINLINE void SetBasisSDValue(int i, int j, VSREAL Value);
		FORCEINLINE VSREAL GetBasisSDValue(int i ,int j)const;

		FORCEINLINE void SetBasisTDValue(int i, int j, VSREAL Value);
		FORCEINLINE VSREAL GetBasisTDValue(int i ,int j)const;
	public:
		//���ض�
		FORCEINLINE unsigned int GetDegree()const;
		//���ؽ���������
		FORCEINLINE unsigned int GetKnotNum()const;
	protected:
		int GetKey(VSREAL t)const;
		bool GetFuncValue(VSREAL t,int &i,unsigned int uiOrder);
	protected:
		VSREAL		* m_pKnotVector;
		unsigned int m_uiDegree;
		unsigned int m_uiKnotNum;
		VSREAL		*m_pBasis;
		VSREAL		*m_pBasisFD;
		VSREAL		*m_pBasisSD;
		VSREAL		*m_pBasisTD;
		unsigned int m_uiBasisNum;
		

	};

#include "VSB_SplineCurve3.inl"
}
#endif