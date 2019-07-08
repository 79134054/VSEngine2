#ifndef VSINDEXBUFFER_H
#define VSINDEXBUFFER_H
#include "VSObject.h"
#include "VSBind.h"
#include "VSDataBuffer.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSIndexBuffer : public VSBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		virtual ~VSIndexBuffer();
		VSIndexBuffer();
		VSIndexBuffer(unsigned int uiNum,unsigned int uiDT = VSDataBuffer::DT_USHORT);
		bool SetDate(VSDataBuffer * pDate);
	public:
		

		virtual void *Lock();
		virtual void UnLock();

		virtual	bool LoadResource(VSRenderer * pRender);
		virtual void ClearInfo();
		FORCEINLINE VSDataBuffer *GetIndexDate()const {return m_pDate;}

		unsigned int GetNum()const;
		virtual unsigned int GetByteSize()const;
		FORCEINLINE void * GetLockDatePtr()const
		{
			return m_pLockDate;
		}
		FORCEINLINE unsigned int GetDateType()const
		{
			return m_uiDT;
		}
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
		VSDataBufferPtr m_pDate;
		unsigned int m_uiNum;
		unsigned int m_uiDT;
		void * m_pLockDate;
	};
	DECLARE_Ptr(VSIndexBuffer);
	VSTYPE_MARCO(VSIndexBuffer);
}
#endif