#ifndef VSB_SPLINECURVE3_H
#define VSB_SPLINECURVE3_H
#include "VSControlCurve3.h"
/*
	B_Spline曲线，属于控制点类型曲线一种，由控制点，节向量，曲线的度，多项式组成,参数t在0-1之间

*/
namespace VSEngine2
{

	class VSMATH_API VSB_SplineCurve3:public VSControlCurve3
	{
	public:
		VSB_SplineCurve3();
		virtual ~VSB_SplineCurve3();
		//复制
		bool Copy(const VSB_SplineCurve3 & B_SplineCurve3);
		//设置控制点
		bool Set(const VSVector3 * pControlPoint,unsigned int uiControlPointNum);
		//把控制点变换成闭合开放式控制点
		bool ChangeToOpenClosePoints();
		//把控制点变换成周期闭合控制点
		bool ChangeToPeriodicClosePoints(unsigned int uiDegree);
		//设置节向量
		bool SetKnotVector(const VSREAL * pKnotVector,unsigned int uiDegree,unsigned int uiControlPointNum);
		//建立开发等距节向量
		bool CreateOpenUniformKnotVector(unsigned int uiDegree,unsigned int uiControlPointNum);
		//建立周期节向量
		bool CreatePeriodicKnotVector(unsigned int uiDegree,unsigned int uiControlPointNum);
		//求导数
		virtual VSVector3	GetFirstDerivative(VSREAL t);
		virtual VSVector3	GetSecondDerivative(VSREAL t);
		virtual VSVector3	GetThirdDerivative(VSREAL t);
		//virtual VSREAL		GetLength(VSREAL t1 ,VSREAL t2);
		//得到上面的点
		virtual VSVector3	GetPoint(VSREAL t);

	/***************************************FORCEINLINE************************************/

	public:
		//设置节向量的一个值
		FORCEINLINE bool SetKnotVector(unsigned int i,VSREAL Value);
		//得到节向量指针
		FORCEINLINE const VSREAL * GetpKnotVector()const;
		//得到节向量的一个值
		FORCEINLINE VSREAL GetKnotVectorValue(unsigned int i)const;
	protected:
		//方便求出多项式值而设置
		FORCEINLINE void SetBasisValue(int i, int j, VSREAL Value);
		FORCEINLINE VSREAL GetBasisValue(int i ,int j)const;

		FORCEINLINE void SetBasisFDValue(int i, int j, VSREAL Value);
		FORCEINLINE VSREAL GetBasisFDValue(int i ,int j)const;

		FORCEINLINE void SetBasisSDValue(int i, int j, VSREAL Value);
		FORCEINLINE VSREAL GetBasisSDValue(int i ,int j)const;

		FORCEINLINE void SetBasisTDValue(int i, int j, VSREAL Value);
		FORCEINLINE VSREAL GetBasisTDValue(int i ,int j)const;
	public:
		//返回度
		FORCEINLINE unsigned int GetDegree()const;
		//返回节向量个数
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