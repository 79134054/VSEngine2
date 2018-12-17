#ifndef VSDIFFEQUATION_H
#define VSDIFFEQUATION_H
#include "VSFunction1.h"
/*
	��һԪ΢�ַ���


*/
namespace VSEngine2
{
class VSMATH_API VSDiffEquation
{
public:
	VSDiffEquation();
	~VSDiffEquation();
	//����ԭ����
	FORCEINLINE void SetFunction1(VSFunction1 * Func){if(!Func) return ; m_pFunc = Func;}
	FORCEINLINE VSFunction1 *GetFunc(){return m_pFunc;}
    //���ò���
	FORCEINLINE void SetStep(VSREAL Step){if(ABS(Step) < EPSILON_E4) return; m_Step = Step;}
	FORCEINLINE VSREAL GetStep(){return m_Step;}
	//���ÿ�ʼ�Ա���
	FORCEINLINE void SetBeginT(VSREAL BeginT){m_BeginT = BeginT;}
	FORCEINLINE VSREAL GetBeginT(){return m_BeginT;}
	//���ÿ�ʼֵ
	FORCEINLINE void SetBeginX(VSREAL BeginX){m_BeginX = BeginX;}
	FORCEINLINE VSREAL GetBeginX(){return m_BeginX;}
	//����
	void Update();
protected:
	VSFunction1 * m_pFunc;
	VSREAL m_Step;
	VSREAL m_BeginT;
	VSREAL m_BeginX;

};



}
#endif