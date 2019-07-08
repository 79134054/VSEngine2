#ifndef VSARRAY_H
#define VSARRAY_H
#include "VSContainer.h"
namespace VSEngine2
{
	
	template <class T,VSMemManagerFun MMFun = VSMemObject::GetMemManager>
	class VSArray : public VSContainer<T,MMFun>
	{
	public:
		enum
		{
			DEFAULT_GROWBY = 10
		};
		VSArray(unsigned int uiGrowBy = DEFAULT_GROWBY);
		~VSArray();
		VSArray(const VSArray& Array);
		void SetBufferNum(unsigned int uiBufferNum);
		void AddBufferNum(unsigned int uiBufferNum);
		void operator= (const VSArray<T,MMFun>& Array);


		FORCEINLINE unsigned int GetNum()const;

		FORCEINLINE unsigned int GetBufferNum()const;

		FORCEINLINE T * GetBuffer()const;
		FORCEINLINE void SetGrowBy(unsigned int uiGrowBy);
		void AddElement(const T & Element);
		template <class N,VSMemManagerFun MMFunN>
		void AddElement(const VSArray<N,MMFunN> & Array,unsigned int uiBegin,unsigned int uiEnd);
		T& operator[] (unsigned int i)const;

		void Clear();

		FORCEINLINE unsigned int GetSize()const;

		void Erase(unsigned int i);
	
		void Erase(unsigned int uiBegin,unsigned int uiEnd);

		template <class N>
		void Sort(unsigned int uiBegin,unsigned int uiEnd,N Compare);
	
		void Sort(unsigned int uiBegin,unsigned int uiEnd);
		void Destroy();
		//���������Ҫע�⣺���T������ָ�룬����Element ��ָ�뵽����ָ�����ʽת������������ָ�����Ĵ���
		//�����ٹ����У�reference��1��1�����ԭ�������referenceΪ0��������ٶ��󣬵��³���
		unsigned int FindElement(const T & Element);
	protected:

		T * m_pBuffer;
		unsigned int m_uiGrowBy;
		unsigned int m_uiCurUse;
		unsigned int m_uiBufferNum;
		unsigned int m_uiAllocNum;

		
	private:

	};
	//��������
	template <class T,VSMemManagerFun MMFun = VSMemObject::GetMemManager>
	class VSArrayOrder : public VSArray<T,MMFun>
	{
	public:
		VSArrayOrder(unsigned int uiGrowBy = DEFAULT_GROWBY);
		~VSArrayOrder();
		void AddElement(const T & Element);

		//���������Ҫע�⣺���T������ָ�룬����Element ��ָ�뵽����ָ�����ʽת������������ָ�����Ĵ���
		//�����ٹ����У�reference��1��1�����ԭ�������referenceΪ0��������ٶ��󣬵��³���
		unsigned int FindElement(const T & Element);

		void ReSort(unsigned int i);
	protected:

		unsigned int Process(unsigned int uiIndex0,unsigned int uiIndex1,const T & Element);

		unsigned int FindElement(unsigned int uiIndex0,unsigned int uiIndex1,const T & Element);
	};
#include "VSArray.inl"
#define  ARRAY_ERASE(ARRAY,ELEMENT)  ARRAY.Erase(ARRAY.FindElement(ELEMENT));
	
}
#endif