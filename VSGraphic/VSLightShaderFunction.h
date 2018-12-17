#ifndef VSLIGHTSHADERFUNCTION_H
#define VSLIGHTSHADERFUNCTION_H
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSLightColor : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSLightColor(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSLightColor();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		VSLightColor();


	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSLightColor);
	VSTYPE_MARCO(VSLightColor);

	class VSGRAPHIC_API VSLightSpecular : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSLightSpecular(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSLightSpecular();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		VSLightSpecular();


	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSLightSpecular);
	VSTYPE_MARCO(VSLightSpecular);


	class VSGRAPHIC_API VSLightDir : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSLightDir(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSLightDir();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		VSLightDir();


	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSLightDir);
	VSTYPE_MARCO(VSLightDir);


	class VSGRAPHIC_API VSLightShaderFunction : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		enum
		{
			IN_DIFFUSE_COLOR,
			IN_ALPHA,
			IN_MAX
		};
		VSLightShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSLightShaderFunction();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName(){};
		virtual bool IsValidNodeToThis(VSShaderFunction * pShaderFunction);


	protected:
		VSLightShaderFunction();
	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};

	};
	DECLARE_Ptr(VSLightShaderFunction);
	VSTYPE_MARCO(VSLightShaderFunction);
}
#endif