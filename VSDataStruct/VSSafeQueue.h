#ifndef VSSAFEQUEUE_H
#define VSSAFEQUEUE_H
#include "VSQueue.h"
#include "VSSynchronize.h"
namespace VSEngine2
{

	template <class T>
	class VSSafeQueue : public VSMemObject
	{
	public:

		VSSafeQueue(bool bUnique = false);
		~VSSafeQueue(); 
		void Enqueue(const T & Element);
		void Dequeue(T & Element);
		void GetTop(T & Element);
		void Clear();
		void Erase(const T & Element);
		bool IsEmpty();
	protected:
		VSQueue<T> m_Queue; 
		VSCriticalSection m_CriticalSec;

	private:

	};
	template <class T>
	VSSafeQueue<T>::~VSSafeQueue()
	{
		
	}

	template <class T>
	VSSafeQueue<T>::VSSafeQueue(bool bUnique)
	{
	}
	template <class T>
	void VSSafeQueue<T>::Clear()
	{
		m_CriticalSec.Lock();
		m_Queue.Clear();
		m_CriticalSec.Unlock();
	}

	template <class T>
	void VSSafeQueue<T>::Enqueue(const T & Element)
	{

		m_CriticalSec.Lock();
		m_Queue.Enqueue(Element);
		m_CriticalSec.Unlock();
	}
	template <class T>
	void VSSafeQueue<T>::GetTop(T & Element)
	{
		m_CriticalSec.Lock();
		m_Queue.GetTop(Element);
		m_CriticalSec.Unlock();
	}
	template <class T>
	void VSSafeQueue<T>::Dequeue(T & Element)
	{
		m_CriticalSec.Lock();
		m_Queue.Dequeue(Element);
		m_CriticalSec.Unlock();
	}
	template <class T>
	void VSSafeQueue<T>::Erase(const T & Element)
	{
		m_CriticalSec.Lock();
		m_Queue.Erase(Element);
		m_CriticalSec.Unlock();
	}
	template <class T>
	bool VSSafeQueue<T>::IsEmpty()
	{
		bool bEmpty = false;
		m_CriticalSec.Lock();
		if (m_Queue.GetNum() == 0)
		{
			bEmpty = true;
		}
		m_CriticalSec.Unlock();
		return bEmpty;
		
	}
}
#endif