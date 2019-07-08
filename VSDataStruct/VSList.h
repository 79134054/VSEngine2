#ifndef VSLIST_H
#define VSLIST_H
#include "VSContainer.h"
namespace VSEngine2
{
	template <class T>
	struct ListElement
	{
	public:
		ListElement()
		{	
			m_pNext = NULL;
			m_pFront = NULL;
		}
		~ListElement()
		{
			m_pNext = NULL;
			m_pFront = NULL;
		}
		ListElement * m_pNext;
		ListElement * m_pFront;
		T Element;
	};

	template <class T,VSMemManagerFun MMFun = VSMemObject::GetMemManager>
	class VSList : public VSContainer<ListElement<T>,MMFun>
	{
	public:

		VSList(bool bUnique = false);
		~VSList(); 

		void operator= (const VSList& Queue);

		FORCEINLINE unsigned int GetNum()const;

		void AddElement(const T & Element);

		bool GetAndEraseTail(T & Element);

		bool GetTail(T & Element);

		void Clear();

		void Erase(const T & Element);

		bool Has(const T & Element);
		class VSListIterator
		{
		public:
			VSListIterator(ListElement<T> * pNode = NULL)
			{
				m_pNode = pNode;
			}
			VSListIterator(const VSListIterator& Iterator)
			{
				m_pNode = Iterator.m_pNode;
			}
			~VSListIterator()
			{
				m_pNode = NULL;
			}
			void operator= (const VSListIterator& Iterator)
			{
				m_pNode = Iterator.m_pNode;
			}
			bool operator!= (const VSListIterator& Iterator)
			{
				return (m_pNode != Iterator.m_pNode);
			}
			bool operator== (const VSListIterator& Iterator)
			{
				return (m_pNode == Iterator.m_pNode);
			}

			void operator= (ListElement<T> * pNode)
			{
				m_pNode = pNode;
			}
			bool operator!= (ListElement<T> * pNode)
			{
				return (m_pNode != pNode);
			}
			bool operator== (ListElement<T> * pNode)
			{
				return (m_pNode == pNode);
			}

			void operator++()
			{
				if (m_pNode)
				{
					m_pNode = m_pNode->m_pNext;
				}

			}
			void operator--()
			{
				if (m_pNode)
				{
					m_pNode = m_pNode->m_pFront;
				}

			}
			bool IsValid()
			{
				return (m_pNode != NULL);
			}
			T &operator*()
			{
				if (m_pNode)
				{
					return m_pNode->Element;
				}
				else
				{
					VSMAC_ASSERT(0);
					return m_pNode->Element;
				}
			}
		protected:
			ListElement<T> * m_pNode;
			
		};
		VSListIterator Begin()
		{
			return VSListIterator(m_pHead);
		}
		VSListIterator End()
		{
			return VSListIterator();
		}
		VSListIterator Tail()
		{
			return VSListIterator(m_pTail);
		}
	protected:

		ListElement<T> * m_pHead;	
		ListElement<T> * m_pTail;	
		unsigned int m_uiNum;
		bool	m_bUnique;


	private:

	};
	template <class T,VSMemManagerFun MMFun>
	VSList<T,MMFun>::~VSList()
	{
		Clear();	
	}

	template <class T,VSMemManagerFun MMFun>
	VSList<T,MMFun>::VSList(bool bUnique)
	{
		m_pHead = NULL;
		m_pTail = NULL;
		m_bUnique = bUnique;
		m_uiNum = 0;
	}
	template <class T,VSMemManagerFun MMFun>
	void VSList<T,MMFun>::Clear()
	{
		ListElement<T> * pTemp = m_pHead;
		while(pTemp)
		{
			ListElement<T> * pNext = pTemp->m_pNext;
			Delete(pTemp,1);
			pTemp = pNext;
		}
		m_pHead = NULL;
		m_pTail = NULL;
		m_uiNum = 0;
	}
	template <class T,VSMemManagerFun MMFun>
	unsigned int VSList<T,MMFun>::GetNum()const
	{
		return m_uiNum;
	}
	template <class T,VSMemManagerFun MMFun>
	void VSList<T,MMFun>::operator= (const VSList& Queue)
	{
		m_bUnique = Queue.m_bUnique;
		ListElement<T> * pTemp = Queue.m_pHead;
		while(pTemp)
		{
			AddElement(pTemp->Element);
			pTemp = pTemp->m_pNext;

		}
	}
	template <class T,VSMemManagerFun MMFun>
	void VSList<T,MMFun>::AddElement(const T & Element)
	{
		if (m_bUnique)
		{
			if (Has(Element))
			{
				return;
			}
		}
		ListElement<T>* pElem = New(1);
		VS_NEW (pElem) ListElement<T>();
		pElem->Element = Element;
		pElem->m_pFront = m_pTail;
		pElem->m_pNext = NULL;
		if (!m_pHead)
		{
			m_pHead = pElem;
		}
		
		if (!m_pTail)
		{
			m_pTail = pElem;
		}
		else
		{
			m_pTail->m_pNext = pElem;
			m_pTail = pElem;
		}
		m_uiNum++;
	}
	template <class T,VSMemManagerFun MMFun>
	bool VSList<T,MMFun>::GetTail(T & Element)
	{
		if (!m_pTail)
		{
			return false;
		}
		Element = m_pTail->Element;
		return true;
	}
	template <class T,VSMemManagerFun MMFun>
	bool VSList<T,MMFun>::GetAndEraseTail(T & Element)
	{
		if (!m_pTail)
		{
			return false;
		}
		Element = m_pTail->Element;

		if(m_pTail == m_pHead)
		{		
			Delete(m_pTail,1);
			m_pTail = m_pHead = NULL;
		}
		else
		{
			m_pTail->m_pFront->m_pNext = NULL;
			ListElement<T> * pTemp = m_pTail;
			m_pTail = m_pTail->m_pFront;
			Delete(pTemp,1);
		}

		m_uiNum--;
		return true;
	}
	template <class T,VSMemManagerFun MMFun>
	void VSList<T,MMFun>::Erase(const T & Element)
	{
		ListElement<T> * pTemp = m_pHead;
		while(pTemp)
		{
			if (pTemp->Element == Element)
			{
				if (!pTemp->m_pFront && !pTemp->m_pNext)
				{
					m_pHead = NULL;
					m_pTail = NULL;
				}
				else if (!pTemp->m_pFront)
				{
					pTemp->m_pNext->m_pFront = NULL;
				}
				else if(!pTemp->m_pNext)
				{
					pTemp->m_pFront->m_pNext = NULL;;
				}
				else
				{
					pTemp->m_pNext->m_pFront = pTemp->m_pFront;
					pTemp->m_pFront->m_pNext = pTemp->m_pNext;
				}
				Delete(pTemp,1);
				m_uiNum--;
				if (!m_bUnique)
				{
					continue;
				}
				else
				{
					break;
				}

			}
			pTemp = pTemp->m_pNext;

		}
	}
	template <class T,VSMemManagerFun MMFun>
	bool VSList<T,MMFun>::Has(const T & Element)
	{
		ListElement<T> * pTemp = m_pHead;
		while(pTemp)
		{
			if (pTemp->Element == Element)
			{
				return true;
			}
			pTemp = pTemp->m_pNext;
		}
		return false;
	}
}
#endif