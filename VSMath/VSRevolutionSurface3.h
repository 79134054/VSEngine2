#ifndef VSREVOLUTIONSURFACE3
#define VSREVOLUTIONSURFACE3
#include "VSCurve3.h"
#include "VSSurface3.h"
/*
��ת����

*/
namespace VSEngine2
{
	class VSMATH_API VSRevolutionSurface3 :public VSSurface3
	{
	public:
		enum{XAXIS,YAXIS,ZAXIS};
		enum{XYPLANE,XZPLANE,YZPLANE};
		VSRevolutionSurface3();
		~VSRevolutionSurface3();
		//�������ߣ���ת�ᣬ����ƽ��
		bool SetCurve(VSCurve3 * pCurve,unsigned int Axis,unsigned int uiPlane);	
		virtual VSVector3 GetPoint(VSREAL U,VSREAL V);
		virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V);
		virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V);
	private:
		VSCurve3 *m_pCurve;
		unsigned int m_uiAxis;
		unsigned int m_uiPlane;
	
	
	};

}
#endif