#ifndef VSFONT_H
#define VSFONT_H
#include "VSObject.h"
#include "VSTexAllState.h"
#include "VS2DTexture.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSFont : public VSObject, public VSResource
	{
		//PRIORITY
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum
		{
			ASCII_START = 32,
			ASCII_END = 126,
			ASCII_NUM = ASCII_END - ASCII_START + 1
		};
		VSFont(const TCHAR* acFace, unsigned int uiSize, bool bBold,
				bool bItalic);
		virtual ~VSFont();
		virtual unsigned int GetResourceType()const
		{
			return RT_FONTTYPE;
		}
	protected:
		VSFont();
	public:


		static bool InitialDefaultState();
		static bool TerminalDefaultState();

	protected:
		
		static VSPointer<VSFont> Default;
	public:
		friend class VSRenderer;
		static const VSFont *GetDefalut()
		{
			return Default;
		}
		FORCEINLINE const VSString & GetFace()const {return m_Face;}
		FORCEINLINE unsigned int GetSize()const {return m_uiSize;}
		FORCEINLINE bool GetBold()const {return m_bBold;}
		FORCEINLINE bool GetItalic()const {return m_bItalic;}
		FORCEINLINE bool IsValid()const
		{
			return m_pFontTex != NULL && !m_pCharacterData && m_pFontTexAllState != NULL;
		}
		void SetFontData(VS2DTexture * pTexture,VSREAL * pCharacterData);
		FORCEINLINE VSREAL GetFontData(unsigned char Index)const
		{
			return m_pCharacterData[Index];
		}
		VSTexAllState * GetTexAllState()const
		{
			return m_pFontTexAllState;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(VSObject * pObjectSrc);
	protected:
		VSString m_Face;
		unsigned int m_uiSize;
		bool m_bBold;
		bool m_bItalic;
		VS2DTexturePtr m_pFontTex;
		VSREAL *m_pCharacterData;
		VSTexAllStatePtr m_pFontTexAllState;
	};
	DECLARE_Ptr(VSFont);
	DECLARE_Proxy(VSFont);
	VSTYPE_MARCO(VSFont);


}
#endif