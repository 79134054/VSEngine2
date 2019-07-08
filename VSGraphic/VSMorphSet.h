#ifndef VSMORPHSET_H
#define VSMORPHSET_H
#include "VSObject.h"
#include "VSVertexBuffer.h"
#include "VSName.h"
#include <VSDelegateList.h>
namespace VSEngine2
{
	/*
		每个Morph数据都有对应的GeometryNode，GeometryNode下有几个节点（几个Geometry，Mesh）,对应的Morph
		也就有几个Buffer。Morph的每个Buffer是和GeometryNode的节点一一对应的。在把Morph导入到MorphSet时候，所有的Morph的Buffer都要一样，
		并且对应Buffer如果有数据，对应的顶点个数也要一样。
		再为GeometryNode设置MorphSet的时候，要检查GeometryNode的节点和MorphSetBuffer是否一样，还要检查每个Mesh的顶点个数
		是否和每个Morph的Buffer的顶点一样（当这个Buffer不为空的时候）

	*/
	class VSStream;

	class VSGRAPHIC_API VSMorph : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		VSMorph();
		virtual ~VSMorph();
		FORCEINLINE unsigned int GetBufferNum()const
		{
			return m_pVertexBufferArray.GetNum();
		}
		FORCEINLINE unsigned int GetVertexNum(unsigned int uiBufferID)const
		{
			if (uiBufferID >= m_pVertexBufferArray.GetNum())
			{
				return 0;
			}
			if (m_pVertexBufferArray[uiBufferID] == NULL)
			{
				return 0;
			}
			return m_pVertexBufferArray[uiBufferID]->GetVertexNum();
		}
		FORCEINLINE VSVertexBuffer * GetBuffer(unsigned int uiBufferID)const
		{
			if (uiBufferID >= m_pVertexBufferArray.GetNum())
			{
				return NULL;
			}

			return m_pVertexBufferArray[uiBufferID];
		}
		void ReSizeBuffer(unsigned int uiBufferSize)
		{
			m_pVertexBufferArray.SetBufferNum(uiBufferSize);
		}
		void SetVertexBuffer(unsigned int uiBufferID,VSVertexBuffer * pBuffer)
		{
			if (uiBufferID >= m_pVertexBufferArray.GetNum())
			{
				return;
			}
			m_pVertexBufferArray[uiBufferID] = pBuffer;

		}
		VSUsedName m_cName;
	protected:
		//element is null that is this geometry no morph
		VSArray<VSVertexBufferPtr> m_pVertexBufferArray;
	};
	DECLARE_Ptr(VSMorph);
	VSTYPE_MARCO(VSMorph);

#ifdef DELEGATE_PREFERRED_SYNTAX
	typedef VSDelegateEvent<void(void)> AddMorphEventType;
#else
	typedef VSDelegateEvent0<void> AddMorphEventType;
#endif
	class VSGRAPHIC_API VSMorphSet: public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		VSMorphSet();
		virtual ~VSMorphSet();
		bool SetMorph(VSMorph * pMorph);
		VSMorph * GetMorph(const VSUsedName & MorphName)const;
		VSMorph * GetMorph(unsigned int i)const;
		FORCEINLINE unsigned int GetMorphNum()const
		{
			return m_pMorphArray.GetNum();
		}
		FORCEINLINE unsigned int GetBufferNum()const
		{
			if (m_pMorphArray.GetNum())
			{
				return m_pMorphArray[0]->GetBufferNum();
			}
			else
				return 0;
		}
		AddMorphEventType m_AddMorphEvent;
	protected:
		VSArray<VSMorphPtr> m_pMorphArray;
	};
	DECLARE_Ptr(VSMorphSet);
	VSTYPE_MARCO(VSMorphSet);

}
#endif