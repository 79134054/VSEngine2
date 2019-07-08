#ifndef VSNURBSSURFACE3_H
#define VSNURBSSURFACE3_H
#include "VSB_SplineSurface3.h"
/*
NURB曲面

*/
namespace VSEngine2
{
	class VSMATH_API VSNURBSSurface3:public VSB_SplineSurface3
	{
	public:
		VSNURBSSurface3();
		~VSNURBSSurface3();
		//设置权重
		bool Set(const VSREAL * pWeight,unsigned int uiRow,unsigned int uiColumn);
		//拷贝
		bool Copy(const VSNURBSSurface3 & NURBSSurface3);
		/***************************************FORCEINLINE************************************/
		//改变权重
		FORCEINLINE bool ChangeWeight(unsigned int U,unsigned int V,VSREAL Weight);
		//获得权重指针
		FORCEINLINE const VSREAL * GetWeight()const;
		//得到权重
		FORCEINLINE VSREAL GetWeight(unsigned int U,unsigned int V)const;
		//取得点
		virtual VSVector3 GetPoint(VSREAL U,VSREAL V);
		//得到偏导
		virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V);
		virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V);

	protected:
		bool Get(unsigned int Order,VSREAL U,VSREAL V ,VSVector3 *D0,VSVector3 *DU,VSVector3 *DV);
	private:
		VSREAL *m_pWeight;
	};
#include "VSNURBSSurface3.inl"
}
#endif