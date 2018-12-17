#ifndef VSCAPTURETEXALLSTATE_H
#define VSCAPTURETEXALLSTATE_H
#include "VSTexAllState.h"
#include "VSViewFamily.h"
#include "VSCubeTexture.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSCaptureTexAllState : public VSTexAllState
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	public:
		VSCaptureTexAllState();

		virtual ~VSCaptureTexAllState();

		bool SetViewCapture(const VSString & ViewCaptureName);

		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void ForceUpdate(bool OnlyUpdateOneTime);
		virtual void NotifyEndDraw();
		virtual bool BeforeSave( void * pDate = NULL);
		virtual bool PostSave( void * pDate = NULL);
		void SetMipLevel(unsigned int uiMipLevel);
		VSCaptureViewFamily * GetViewFamily();
	protected:
		VSString  m_ViewCaptureName;
		bool m_bDynamic;
		VSTexturePtr m_pStaticTexture;
		unsigned int m_uiMipLevel;
		void CreateStaticTexture();
	};
	DECLARE_Ptr(VSCaptureTexAllState);
	VSTYPE_MARCO(VSCaptureTexAllState);
	DECLARE_Proxy(VSCaptureTexAllState);
}
#endif