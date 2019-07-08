#ifndef VSCONTROLSURFACE3_H
#define VSCONTROLSURFACE3_H
#include "VSSurface3.h"
/*
���������࣬���еĿ��Ƶ����涼�̳�����࣬û��ʵ��
*/
namespace VSEngine2
{
	class VSMATH_API VSControlSurface3 : public VSSurface3
	{
	public:
		VSControlSurface3();
		virtual ~VSControlSurface3() = 0;
		//����
		bool Copy(const VSControlSurface3& ControlSurface3);
		//�õ���
		virtual VSVector3 GetPoint(VSREAL U,VSREAL V) = 0;
		//��ƫ��
		virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V) = 0;
		virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V) = 0;
		/****************************FORCEINLINE********************************************/
		//�õ����Ƶ�
		FORCEINLINE VSVector3 GetControlPoint(unsigned int U,unsigned int V)const;
		//�ı���Ƶ�
		FORCEINLINE bool ChangeControlPoint(unsigned int U,unsigned int V,const VSVector3& Value);
		//�õ����Ƶ�ָ��
		FORCEINLINE const VSVector3 *GetControlPoint()const;
		//��ÿ��Ƶ����
		FORCEINLINE unsigned int GetControlPointNum()const;
		//�������
		FORCEINLINE unsigned int GetRow()const;
		//�������
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