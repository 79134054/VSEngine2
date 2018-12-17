#ifndef VSSTACK
#define VSSTACK
#include "VSArray.h"
namespace VSEngine2
{
	template <class T,VSMemManagerFun MMFun = VSMemObject::GetMemManager>
	class VSStack
	{
	public:
		VSStack();
		~VSStack();
		bool GetTop(T & Element);
		void Push(const T & Element);
		bool Pop(T & Element);
		unsigned int GetNum();
	protected:
		VSArray<T,MMFun> m_Array;
	};
	template <class T,VSMemManagerFun MMFun>
	VSStack<T,MMFun>::VSStack()
	{
	}
	template <class T,VSMemManagerFun MMFun>
	VSStack<T,MMFun>::~VSStack()
	{

	}
	template <class T,VSMemManagerFun MMFun>
	bool VSStack<T,MMFun>::GetTop(T & Element)
	{
		if (m_Array.GetNum() == 0)
		{
			return false;
		}
		Element = m_Array[m_Array.GetNum() - 1];
		return true;
	}
	template <class T,VSMemManagerFun MMFun>
	void VSStack<T,MMFun>::Push(const T & Element)
	{
		m_Array.AddElement(Element);
	}
	template <class T,VSMemManagerFun MMFun>
	bool VSStack<T,MMFun>::Pop(T & Element)
	{
		if (m_Array.GetNum() == 0)
		{
			return false;
		}
		Element = m_Array[m_Array.GetNum() - 1];
		m_Array.Erase(m_Array.GetNum() - 1);
		return true;
	}
	template <class T,VSMemManagerFun MMFun>
	unsigned int VSStack<T,MMFun>::GetNum()
	{
		return m_Array.GetNum();
	}
}
#endif