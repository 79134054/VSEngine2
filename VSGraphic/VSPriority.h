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
		//0既表示优先级别最大，也或者没有优先级。如果m_pPriorityThanList无成员，表示最大有限级别
		//否则需要计算优先级别
		unsigned int m_uiPriorityNum;
		VSArray<VSPriority *> m_pPriorityThanList;
	
	
	};


}
#endif