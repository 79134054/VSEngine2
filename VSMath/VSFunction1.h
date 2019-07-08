#ifndef VSFUNCTION1_H
#define VSFUNCTION2_H
#include "VSMath.h"
/*
	һԪ����

*/
namespace VSEngine2
{
	class VSMATH_API VSFunction1 : public VSMemObject
	{
	public:
		VSFunction1();
		virtual ~VSFunction1() = 0;
		//��ֵ
		virtual VSREAL GetValue(VSREAL fParameter) = 0 ;
		//����
		virtual VSREAL GetFirstDerivative(VSREAL fParameter) = 0;
		virtual VSREAL GetSecondDerivative(VSREAL fParameter) = 0;
		virtual VSREAL GetThirdDerivative(VSREAL fParameter) = 0;
		//�����
		VSREAL Integration(VSREAL t1 , VSREAL t2,unsigned int uiIterator);
		//��ֵ
		void GetExtremumValue(VSREAL t1 , VSREAL t2,VSREAL & Min,VSREAL &Max,unsigned int uiLevel);
		//�Ƿ��и�
		bool ISHaveRoot(VSREAL t1 , VSREAL t2,unsigned int uiIterator);
		//�����
		bool ComputeRoot(VSREAL t1 , VSREAL t2,unsigned int uiLevel);
		//���ظ�ָ��
		FORCEINLINE VSREAL* GetRoot()const;
		//���ظ�����
		FORCEINLINE unsigned int GetRootNum()const;
	protected:
		typedef struct INTERVAL
		{
			VSREAL t1;
			VSREAL t2;
			VSREAL value1;
			VSREAL value2;
			INTERVAL * next;
		};

		INTERVAL  *m_pHead;
		INTERVAL  *m_pNext;
		VSREAL	*m_pRoot;
		unsigned int m_uiRootNum;
		unsigned int m_uiIndex;
		void Root(VSREAL t1 ,VSREAL t2,VSREAL Value1,VSREAL Value2,unsigned int uiLevel);
		void ExtremumValue(VSREAL t1 , VSREAL t2,VSREAL Value1,VSREAL Value2,VSREAL & Min,VSREAL &Max,unsigned int uiLevel);
	};


}
#endif