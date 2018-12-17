#ifndef VSDEPTHSTENCIL_H
#define VSDEPTHSTENCIL_H
#include "VSBind.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSDepthStencil : public VSBind
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
		
	public:
		friend class VSResourceManager;
	protected:
		VSDepthStencil();
		VSDepthStencil(unsigned int uiWidth,unsigned int uiHeight,
			unsigned int uiMulSample,unsigned int uiFormatType);
	public:
		
		virtual ~VSDepthStencil();
		FORCEINLINE unsigned int GetWidth()const
		{
			return m_uiWidth;
		}
		FORCEINLINE unsigned int GetHeight()const
		{
			return m_uiHeight;
		}
		FORCEINLINE unsigned int GetMulSample()const
		{
			return m_uiMulSample;
		}
		FORCEINLINE unsigned int GetFormatType()const
		{
			return m_uiFormatType;
		}
		bool m_bUsed;
	protected:

		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiMulSample;
		unsigned int m_uiFormatType;
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);

	};
	DECLARE_Ptr(VSDepthStencil);
	VSTYPE_MARCO(VSDepthStencil);
}
#endif