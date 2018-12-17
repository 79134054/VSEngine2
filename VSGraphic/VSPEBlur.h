#ifndef VSPEBLUR_H
#define VSPEBLUR_H
#include "VSPostEffectFunction.h"
namespace VSEngine2
{
	class VSPEBlur : public VSPostEffectFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSPEBlur(const VSUsedName & ShowName,VSPostEffectSet * pMaterial);
		~VSPEBlur();

	};
	DECLARE_Ptr(VSPEBlur);
	VSTYPE_MARCO(VSPEBlur);
}
#endif