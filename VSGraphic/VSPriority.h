#ifndef VSPRIORITY_H
#define VSPRIORITY_H
#include "VSArray.h"
#include "VSGraphic.h"
#include "VSPriority.marc"
namespace VSEngine2
{
	//template<class T>
	class VSGRAPHIC_API VSPriority
	{
	public:
		VSPriority();
		VSPriority(unsigned int uiPriorityNum);
		~VSPriority();
		bool AddPriorityThan(VSPriority * pPriority);
		unsigned int GetPriorityNum();

		VSPriority & operator= (VSPriority& rPriority);
		bool operator== (VSPriority& rPriority);
		bool operator!= (VSPriority& rPriority);

	
		bool operator >(VSPriority& rPriority);
		bool operator <(VSPriority& rPriority);
		
		bool operator >=(VSPriority& rPriority);
		bool operator <=(VSPriority& rPriority);

	private:
		bool CheckIsCircle(const VSPriority *pChecked)const;
		unsigned int CheckPriorityNum();
		//0�ȱ�ʾ���ȼ������Ҳ����û�����ȼ������m_pPriorityThanList�޳�Ա����ʾ������޼���
		//������Ҫ�������ȼ���
		unsigned int m_uiPriorityNum;
		VSArray<VSPriority *> m_pPriorityThanList;
	
	
	};


}
#endif