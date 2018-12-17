#ifndef VSNURBSSURFACE3_H
#define VSNURBSSURFACE3_H
#include "VSB_SplineSurface3.h"
/*
NURB����

*/
namespace VSEngine2
{
	class VSMATH_API VSNURBSSurface3:public VSB_SplineSurface3
	{
	public:
		VSNURBSSurface3();
		~VSNURBSSurface3();
		//����Ȩ��
		bool Set(const VSREAL * pWeight,unsigned int uiRow,unsigned int uiColumn);
		//����
		bool Copy(const VSNURBSSurface3 & NURBSSurface3);
		/***************************************FORCEINLINE************************************/
		//�ı�Ȩ��
		FORCEINLINE bool ChangeWeight(unsigned int U,unsigned int V,VSREAL Weight);
		//���Ȩ��ָ��
		FORCEINLINE const VSREAL * GetWeight()const;
		//�õ�Ȩ��
		FORCEINLINE VSREAL GetWeight(unsigned int U,unsigned int V)const;
		//ȡ�õ�
		virtual VSVector3 GetPoint(VSREAL U,VSREAL V);
		//�õ�ƫ��
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