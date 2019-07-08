#ifndef VSCURVELENGTHFUNCTION1_H
#define VSCURVELENGTHFUNCTION1_H
#include "VSFunction1.h"
#include "VSCurve3.h"
/*
求曲线长度函数
*/
namespace VSEngine2
{
	//the function is sqrt(dy/dt * dy/dt + dx/dt * dx/dt);
	class VSMATH_API VSCurveLengthFunction1:public VSFunction1
	{
	public:
		VSCurveLengthFunction1();
		~VSCurveLengthFunction1();
		//得到值
		virtual VSREAL GetValue(VSREAL fParameter);
		//求导数 to do 
		virtual VSREAL GetFirstDerivative(VSREAL fParameter);
		virtual VSREAL GetSecondDerivative(VSREAL fParameter);
		virtual VSREAL GetThirdDerivative(VSREAL fParameter);
		//设置曲线
		FORCEINLINE void SetCurve(VSCurve3 *pCurve){if(!pCurve) return ; m_pCurve = pCurve;}
		FORCEINLINE VSCurve3 *GetCurve(){return m_pCurve;}
	protected:
		VSCurve3 *m_pCurve;
	};


}
#endif