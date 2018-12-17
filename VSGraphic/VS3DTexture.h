#ifndef VS3DTEXTURE_H
#define VS3DTEXTURE_H
#include "VSTexture.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VS3DTexture:public VSTexture
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VS3DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiLength,
			unsigned int uiFormatType,unsigned int uiMipLevel = 0,bool bIsStatic = true);
		virtual ~VS3DTexture();

		virtual unsigned int GetTexType()const{return TT_3D;}
		virtual void SetMipLevel();
	protected:
		VS3DTexture();

	};
	DECLARE_Ptr(VS3DTexture);
	VSTYPE_MARCO(VS3DTexture);
	
}
#endif