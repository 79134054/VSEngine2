#ifndef VSUSERSAMPLER_H
#define VSUSERSAMPLER_H
#include "VSMemManager.h"
#include "VSMath.h"
#include "VSObject.h"
#include "VSTexAllState.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSUserSampler : public VSObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		
		VSUserSampler(const VSUsedName & Name,unsigned int uiTexType,unsigned int uiRegisterIndex,unsigned int uiRegisterNum);
		virtual ~VSUserSampler();

		FORCEINLINE unsigned int GetTexType()const
		{
			return m_uiTexType;
		}
		
		FORCEINLINE VSTexAllState * GetTex(unsigned int uiIndex)const
		{
			return m_pTextureArray[uiIndex];
		}
		FORCEINLINE const VSUsedName & GetName()const
		{
			return m_Name;
		}
		FORCEINLINE void SetTex(VSTexAllState * pTex,unsigned int uiIndex)
		{
			if (uiIndex >= m_uiRegisterNum)
			{
				return ;
			}
			m_pTextureArray[uiIndex] = pTex;
		}
		FORCEINLINE unsigned int GetRegisterIndex()const
		{
			return m_uiRegisterIndex;
		}
		FORCEINLINE unsigned int GetRegisterNum()const
		{
			return m_uiRegisterNum;
		}
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(VSObject * pObjectSrc);
	protected:
		friend class VSRenderer;
		VSUserSampler();
	private:

		unsigned int m_uiTexType;
		VSArray<VSTexAllStatePtr> m_pTextureArray;
		VSUsedName		m_Name;
		unsigned int m_uiRegisterIndex;
		unsigned int m_uiRegisterNum;
	};
	VSTYPE_MARCO(VSUserSampler);
	DECLARE_Ptr(VSUserSampler);
}
#endif