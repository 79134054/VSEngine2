#ifndef VSTUBESURFACE3_H
#define VSTUBESURFACE3_H
#include "VSCurve3.h"
#include "VSSurface3.h"
/*
��������

*/
namespace VSEngine2
{
	class VSMATH_API VSTubeSurface3 :public VSSurface3
	{
	public:
		VSTubeSurface3();
		~VSTubeSurface3();
		//�������ߣ�����U,V����ĳ���
		bool SetCurve(VSCurve3 * pCurve,VSREAL fN,VSREAL fB);	
		virtual VSVector3 GetPoint(VSREAL U,VSREAL V);
		virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V);
		virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V);
	private:
		VSCurve3 *m_pCurve;
		VSREAL m_fN;
		VSREAL m_fB;

	};


}
#endif