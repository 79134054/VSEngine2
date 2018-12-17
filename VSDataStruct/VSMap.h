#ifndef VSMAP_H
#define VSMAP_H
#include "VSContainer.h"
namespace VSEngine2
{
	
	template<class KEY,class VALUE,VSMemManagerFun MMFun = VSMemObject::GetMemManager>
	class VSMap : public VSContainer<MapElement<KEY,VALUE>,MMFun>
	{
	public:
		enum
		{
			DEFAULT_GROWBY = 10
		};
		
		VSMap(unsigned int uiGrowBy = DEFAULT_GROWBY);
		~VSMap();

		void SetBufferNum(unsigned int uiBufferNum);
		void AddBufferNum(unsigned int uiBufferNum);
		void operator= (const VSMap<KEY,VALUE,MMFun>& Map);


		FORCEINLINE unsigned int GetNum()const;

		FORCEINLINE unsigned int GetBufferNum()const;

		FORCEINLINE MapElement<KEY,VALUE> * GetBuffer()const;
		FORCEINLINE void SetGrowBy(unsigned int uiGrowBy);
		
		template<class KEY1,class VALUE1>
		void AddElement(const MapElement<KEY1,VALUE1> & Element);
		void AddElement(const KEY& Key , const VALUE &Value);

		template<class KEY1,class VALUE1,VSMemManagerFun MMFun1>
		void AddElement(const VSMap<KEY1,VALUE1,MMFun1> & Map,unsigned int uiBegin,unsigned int uiEnd);

		MapElement<KEY,VALUE>& operator[] (unsigned int i)const;

		void Clear();

		FORCEINLINE unsigned int GetSize()const;

		void Erase(unsigned int i);

		void Erase(unsigned int uiBegin,unsigned int uiEnd);

		template <class N>
		void Sort(unsigned int uiBegin,unsigned int uiEnd,N Compare);
	
		void Sort(unsigned int uiBegin,unsigned int uiEnd);
		unsigned int Find(const KEY& Key)const ;

		unsigned int FindValueIndex(const VALUE &Value)const;

		void Destroy();
	protected:

		MapElement<KEY,VALUE> * m_pBuffer;
		unsigned int m_uiGrowBy;
		unsigned int m_uiCurUse;
		unsigned int m_uiBufferNum;
		unsigned int m_uiAllocNum;
	};

	template<class KEY,class VALUE,VSMemManagerFun MMFun = VSMemObject::GetMemManager>
	class VSMapOrder : public VSMap<KEY,VALUE,MMFun>
	{
	public:
		VSMapOrder(unsigned int uiGrowBy = DEFAULT_GROWBY);
		~VSMapOrder();

		template<class KEY1,class VALUE1>
		void AddElement(const MapElement<KEY1,VALUE1> & Element);

		void AddElement(const KEY& Key , const VALUE &Value);


		unsigned int Find(const KEY& Key)const ;

	protected:
		template<class KEY1,class VALUE1>
		unsigned int Process(unsigned int uiIndex0,unsigned int uiIndex1,const MapElement<KEY1,VALUE1> & Element);

		unsigned int FindElement(unsigned int uiIndex0,unsigned int uiIndex1,const KEY& Key)const;
	};
	#include "VSMap.inl"
#define  MAP_ERASE(MAP,KEY)  MAP.Erase(MAP.Find(KEY));
}
#endif