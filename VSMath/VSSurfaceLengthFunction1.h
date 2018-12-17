#ifndef VSSURFACELENGTHFUNCTION1_H
#define VSSURFACELENGTHFUNCTION1_H
#include "VSFunction1.h"
#include "VSSurface3.h"
/*
求曲线长度函数
*/
namespace VSEngine2
{
	//the function is sqrt(dy/dt * dy/dt + dx/dt * dx/dt);
	class VSMATH_API VSSurfaceLengthFunction1:public VSFunction1
	{
	public:
		enum // length type
		{
			LT_U_LEN,
			LT_V_LEN
		};
		VSSurfaceLengthFunction1(unsigned int uiLengthType,VSREAL fTypePara);
		~VSSurfaceLengthFunction1();
		//得到值
		virtual VSREAL GetValue(VSREAL fParameter);
		//求导数 to do 
		virtual VSREAL GetFirstDerivative(VSREAL fParameter);
		virtual VSREAL GetSecondDerivative(VSREAL fParameter);
		virtual VSREAL GetThirdDerivative(VSREAL fParameter);
		//设置曲线
		FORCEINLINE void SetSurface(VSSurface3 *pSurface){if(!pSurface) return ; m_pSurface = pSurface;}
		FORCEINLINE VSSurface3 *GetSurface(){return m_pSurface;}
	protected:
		VSSurface3 *m_pSurface;
		VSREAL m_fTypePara;
		unsigned int m_uiLengthType;
	};


}
#endif