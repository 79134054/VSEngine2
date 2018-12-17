#ifndef VSPEMATERIAL_H
#define VSPEMATERIAL_H
#include "VSPostEffectFunction.h"
namespace VSEngine2
{
	DECLARE_Ptr(VSMaterialInstance);
	DECLARE_Ptr(VSMaterial);
	DECLARE_Proxy(VSMaterial);
	class VSPEMaterial : public VSPostEffectFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		VSPEMaterial(const VSUsedName & ShowName, VSPostEffectSet * pPostEffectSet);
		~VSPEMaterial();
		virtual VSPostEffectSceneRender * CreateSceneRender();
		virtual void OnDraw(VSCuller & Culler, double dAppTime);
		void SetMaterial(VSMaterialR * pMaterial);
		enum
		{
			INPUT_COLOR
		};
		enum
		{
			OUT_COLOR
		};

	protected:
		VSPEMaterial();
		VSMaterialInstancePtr	m_pMaterialInstance;
	};
	DECLARE_Ptr(VSPEMaterial);
	VSTYPE_MARCO(VSPEMaterial);
	
}
#endif