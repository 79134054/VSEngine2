#ifndef VSPEGRAY_H
#define VSPEGRAY_H
#include "VSPostEffectFunction.h"
namespace VSEngine2
{
	class VSPEGray : public VSPostEffectFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSPEGray (const VSUsedName & ShowName,VSPostEffectSet * pPostEffectSet);
		~VSPEGray ();
		virtual VSPostEffectSceneRender * CreateSceneRender();
		virtual void OnDraw(VSCuller & Culler,double dAppTime);
		enum
		{
			INPUT_COLOR
		};
		enum
		{
			OUT_COLOR
		};
	protected:
		VSPEGray ();

	};
	DECLARE_Ptr(VSPEGray);
	VSTYPE_MARCO(VSPEGray);
}
#endif