#ifndef VSSHADERKEY_H
#define VSSHADERKEY_H
#include "VSName.h"
#include "VSLight.h"
namespace VSEngine2
{
	class VSGeometry;
	class VSVertexBuffer;
	class VSStream;
	DECLARE_Ptr(VSGeometry);
	DECLARE_Ptr(VSVertexBuffer);
	DECLARE_Ptr(VSMaterialInstance);
	DECLARE_Ptr(VSLight);

	struct MaterialShaderPara 
	{
		MaterialShaderPara()
		{
			pGeometry = NULL;
			pShadowLight = NULL;
			pCamera = NULL;
			pMaterialInstance = NULL;
			LightArray.Clear();
			uiPassId = 0;

		}
		~MaterialShaderPara()
		{
			pGeometry = NULL;
			pShadowLight = NULL;
			pCamera = NULL;
			pMaterialInstance = NULL;
			LightArray.Clear();
			uiPassId = 0;

		}
		VSCamera* pCamera;
		VSMaterialInstance* pMaterialInstance;
		VSArray<VSLight*>	LightArray;
		unsigned int uiPassId;
		VSGeometry* pGeometry;
		VSLight * pShadowLight;
		VSString m_VSShaderPath;
		VSString m_PSShaderPath;
		VSString m_GSShaderPath;
		VSString m_VMainFunName;
		VSString m_PMainFunName;
		VSString m_GMainFunName;
		VSColorRGBA m_SkyLightUpColor;
		VSColorRGBA m_SkyLightDownColor;
	};
	class VSStream;
	class VSGRAPHIC_API VSShaderKey : public VSObject
	{
		
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSShaderKey();
		~VSShaderKey();
	protected:
		VSMapOrder<VSUsedName,unsigned int> m_KeyMap;
	public:
		enum
		{

			MAX_LIGTH_TYPE_MASK = 3,
			MAX_LIGHT_TYPE_NUM = (1 << MAX_LIGTH_TYPE_MASK) - 1
		};
		bool IsHaveTheKey(const VSUsedName & Name,unsigned int &uiKeyId);
		void SetTheKey(const VSUsedName & Name,unsigned int Value);

	public:


		static void SetMaterialVShaderKey(VSShaderKey * pKey,MaterialShaderPara & MSPara,unsigned int uiPassType);

		static void SetMaterialPShaderKey(VSShaderKey * pKey,MaterialShaderPara & MSPara,unsigned int uiPassType);

		static void SetMaterialGShaderKey(VSShaderKey * pKey, MaterialShaderPara & MSPara, unsigned int uiPassType);
		struct ShadowKeyInfo
		{
			ShadowKeyInfo()
			{
				uiShadowNum = 0;
				uiVolumeShadowNum = 0;
				uiDualParaboloidShadowNum = 0 ;
				uiCSMShadowNum = 0;
			}
			unsigned int uiShadowNum;
			unsigned int uiVolumeShadowNum;
			unsigned int uiDualParaboloidShadowNum;
			unsigned int uiCSMShadowNum;
		};
		static unsigned int GenerateKey(const VSArray<VSLight*> & LightArray,ShadowKeyInfo ShadowInfo[VSLight::LT_MAX],unsigned int &uiLightFunctionKey);

		static unsigned int GetLightNum(unsigned int uiLightKey,unsigned int uiLightType);
		
		VSGRAPHIC_API friend bool operator == (const VSShaderKey & Key1,const VSShaderKey & Key2);
		VSGRAPHIC_API friend bool operator > (const VSShaderKey & Key1,const VSShaderKey & Key2);
		VSGRAPHIC_API friend bool operator < (const VSShaderKey & Key1,const VSShaderKey & Key2);

		//ÖØÔØ=²Ù×÷·û
		void operator =(const VSShaderKey &ShaderKey);

		void GetDefine(VSMap<VSString,VSString> & Define);

		void Clear();
	};
	VSTYPE_MARCO(VSShaderKey);
}
#endif