#ifndef VSMORPHFUNCTION_H
#define VSMORPHFUNCTION_H
#include "VSVector2.h"
#include "VSMorphTree.h"
#include "VSMorphBaseFunction.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSMorphFunction : public VSMorphBaseFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		virtual ~VSMorphFunction() = 0;
		VSMorphFunction(const VSUsedName & ShowName,VSMorphTree * pMorphTree);
		virtual void UpdateGeometryDate(unsigned int GeometryIndex);
		virtual void UpdateVertexDate(unsigned int uiVertexIndex);
		virtual void ClearChangeFlag();

		enum
		{
			MAX_NUM_POS3		= 2,
			MAX_NUM_NORMAL3		= 2,
			MAX_NUM_COLOR		= 2
		};
	protected:
		VSMorphFunction();
		/*
			顶点格式的设置每个部分都可以设置多个channel，但Morph只支持固定的常用channel，在区分方面：
			会在VSMorphSequence上进行判断，也就是说，如果morphset的morph对应的部分channel不符合morph顶点条件，
			则不会调相对应的set函数，这样flag就为false，在MorphMainfuntion里面就不会混合。
		*/
		VSVector3	m_Pos[MAX_NUM_POS3];
		bool		m_bPosChange[MAX_NUM_POS3];

		VSVector3	m_Normal[MAX_NUM_NORMAL3];
		bool		m_bNormalChange[MAX_NUM_NORMAL3];
		VSVector3W	m_Tangent;
		bool		m_bTangentChange;
		VSVector3	m_Binormal;
		bool		m_bBinormalChange;

		DWORD		m_Color[MAX_NUM_COLOR];
		bool		m_bColorChange[MAX_NUM_COLOR];

	public:
		FORCEINLINE void SetPos(const VSVector3 & Pos,unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_POS3)
			{
				m_Pos[uiLevel] = Pos;
				m_bPosChange[uiLevel] = true;
			}
		}

		FORCEINLINE void SetNormal(const VSVector3 & Normal,unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_NORMAL3)
			{
				m_Normal[uiLevel] = Normal;
				m_bNormalChange[uiLevel] = true;
			}
		}
		FORCEINLINE void SetTangent(const VSVector3W &Tangent)
		{
			m_Tangent = Tangent;
			m_bTangentChange = true;
		}
		FORCEINLINE void SetBinormal(const VSVector3 &Binormal)
		{
			m_Binormal= Binormal;
			m_bBinormalChange = true;
		}

		FORCEINLINE void SetColor(const DWORD Color, unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_NORMAL3)
			{
				m_Color[uiLevel] = Color;
				m_bColorChange[uiLevel] = true;
			}
		}

		FORCEINLINE VSVector3 * GetPos(unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_POS3 && m_bPosChange[uiLevel])
			{
				return &m_Pos[uiLevel];
			}
			return NULL;
		}

		FORCEINLINE VSVector3 * GetNormal(unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_NORMAL3 && m_bNormalChange[uiLevel])
			{
				return &m_Normal[uiLevel];
			}
			return NULL;
		}
		FORCEINLINE VSVector3W * GetTangent()
		{
			if (m_bTangentChange)
			{
				return &m_Tangent;
			}
			return NULL;
		}
		FORCEINLINE VSVector3 * GetBinormal()
		{
			if (m_bBinormalChange)
			{
				return &m_Binormal;
			}
			return NULL;	
		}

		FORCEINLINE DWORD * GetColor(unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_COLOR && m_bColorChange[uiLevel])
			{
				return &m_Color[uiLevel];
			}
			return NULL;
		}

	};
	DECLARE_Ptr(VSMorphFunction);
	VSTYPE_MARCO(VSMorphFunction);
}
#endif
