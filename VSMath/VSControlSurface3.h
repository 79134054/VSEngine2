#ifndef VSCONTROLSURFACE3_H
#define VSCONTROLSURFACE3_H
#include "VSSurface3.h"
/*
控制曲面类，所有的控制点曲面都继承这个类，没有实例
*/
namespace VSEngine2
{
	class VSMATH_API VSControlSurface3 : public VSSurface3
	{
	public:
		VSControlSurface3();
		virtual ~VSControlSurface3() = 0;
		//拷贝
		bool Copy(const VSControlSurface3& ControlSurface3);
		//得到点
		virtual VSVector3 GetPoint(VSREAL U,VSREAL V) = 0;
		//求偏导
		virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V) = 0;
		virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V) = 0;
		/****************************FORCEINLINE********************************************/
		//得到控制点
		FORCEINLINE VSVector3 GetControlPoint(unsigned int U,unsigned int V)const;
		//改变控制点
		FORCEINLINE bool ChangeControlPoint(unsigned int U,unsigned int V,const VSVector3& Value);
		//得到控制点指针
		FORCEINLINE const VSVector3 *GetControlPoint()const;
		//获得控制点个数
		FORCEINLINE unsigned int GetControlPointNum()const;
		//获得行数
		FORCEINLINE unsigned int GetRow()const;
		//获得列数
		FORCEINLINE unsigned int GetColumn()const;

		VSREAL GetULength(VSREAL V,VSREAL U1, VSREAL U2,unsigned int uiIterations = 32);
		VSREAL GetVLength(VSREAL U,VSREAL V1, VSREAL V2,unsigned int uiIterations = 32);
		VSREAL GetUTime (VSREAL V,VSREAL fLength, unsigned int uiIterations = 32,VSREAL fTolerance = EPSILON_E4);
		VSREAL GetVTime (VSREAL U,VSREAL fLength, unsigned int uiIterations = 32,VSREAL fTolerance = EPSILON_E4);
	protected:
		VSVector3 * m_pControlPoint;
		unsigned int m_uiControlPointNum;
		unsigned int m_uiRow;
		unsigned int m_uiColumn;
	};

#include "VSControlSurface3.inl"

}
#endif