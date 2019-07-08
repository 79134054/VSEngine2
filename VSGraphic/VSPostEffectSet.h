#ifndef VSPOSTEFFECTSET_H
#define VSPOSTEFFECTSET_H
#include "VSObject.h"
#include "VSResource.h"
#include "VSCuller.h"
namespace VSEngine2
{
	class VSPostEffectFunction;
	class VSPEBeginFunction;
	class VSPEEndFunction;
	class VSScene;
	class VSCamera;
	class VS2DTexture;
	class VSRenderTarget;
	class VSStream;
	class VSGRAPHIC_API VSPostEffectSet : public VSObject , public VSResource
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	protected:
		VSPostEffectSet();
	public:
		VSPostEffectSet(const VSUsedName &ShowName);
		virtual ~VSPostEffectSet();

	public:


	
		void AddPostEffectFunction(VSPostEffectFunction * pPostEffectFunction);
		void DeletePostEffectFunction(VSPostEffectFunction * pPostEffectFunction);
		virtual unsigned int GetResourceType()const
		{
			return RT_POSTEFFECT;
		}
		void Draw(VSCuller & Culler,double dAppTime);
		VSRenderTarget * GetEndTarget(unsigned int i);
		void SetBeginTargetArray(VSArray<VSRenderTarget *> * pBeginTargetArray);
		void GetRT(unsigned int uiWidth,unsigned int uiHeight);
		void DisableRT();
		static const VSPostEffectSet *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		VSPostEffectFunction * GetPEFunctionFromShowName(const VSUsedName & ShowName);

		bool SetEndTarget(VSRenderTarget *pEndTarget);
	protected:
		VSArray<VSPostEffectFunction *> m_pPostEffectFunctionArray;
		VSPEBeginFunction*  m_pPEBeginFunc;
		VSPEEndFunction * m_pPEEndFunc;
		VSUsedName m_ShowName;
		static VSPointer<VSPostEffectSet> Default;
	};
	DECLARE_Ptr(VSPostEffectSet);
	VSTYPE_MARCO(VSPostEffectSet);
	DECLARE_Proxy(VSPostEffectSet);
	class VSPEGray;
	class VSGRAPHIC_API VSPESetGray : public VSPostEffectSet
	{
	public:
		VSPESetGray(const VSUsedName &ShowName);
		virtual ~VSPESetGray();
	};
	DECLARE_Ptr(VSPESetGray);
	VSTYPE_MARCO(VSPESetGray);

	DECLARE_Ptr(VSMaterial);
	DECLARE_Proxy(VSMaterial);
	class VSGRAPHIC_API VSPESetMaterialAndGray : public VSPostEffectSet
	{
	public:
		VSPESetMaterialAndGray(const VSUsedName &ShowName,VSMaterialR * pMaterial);
		virtual ~VSPESetMaterialAndGray();
	};
	DECLARE_Ptr(VSPESetMaterialAndGray);
	VSTYPE_MARCO(VSPESetMaterialAndGray);

}
#endif