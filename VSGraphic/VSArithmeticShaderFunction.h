#ifndef VSARITHMETICSHADERFUNCTION_H
#define VSARITHMETICSHADERFUNCTION_H
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSStream;

	class VSGRAPHIC_API VSMul : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSMul(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSMul();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		VSMul();


	public:
		enum
		{
			IN_A,
			IN_B,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSMul);
	VSTYPE_MARCO(VSMul);


	class VSGRAPHIC_API VSDot : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSDot(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSDot();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		VSDot();


	public:
		enum
		{
			IN_A,
			IN_B,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};

	};
	DECLARE_Ptr(VSDot);
	VSTYPE_MARCO(VSDot);

	class VSGRAPHIC_API VSSub : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSSub(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSSub();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		VSSub();


	public:
		enum
		{
			IN_A,
			IN_B,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSSub);
	VSTYPE_MARCO(VSSub);

	class VSGRAPHIC_API VSAdd : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSAdd(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSAdd();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		VSAdd();


	public:
		enum
		{
			IN_A,
			IN_B,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSAdd);
	VSTYPE_MARCO(VSAdd);

	class VSGRAPHIC_API VSMakeValue : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSMakeValue(const VSUsedName & ShowName, VSMaterial * pMaterial,unsigned int uiInputNum);
		virtual ~VSMakeValue();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		VSMakeValue();
	public:
		enum
		{
			IN_A,
			IN_B,
			IN_C,
			IN_D,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};
	};
	DECLARE_Ptr(VSMakeValue);
	VSTYPE_MARCO(VSMakeValue);

	class VSGRAPHIC_API VSDiv : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSDiv(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSDiv();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		VSDiv();


	public:
		enum
		{
			IN_A,
			IN_B,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSDiv);
	VSTYPE_MARCO(VSDiv);


	class VSGRAPHIC_API VSSaturate : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSSaturate(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSSaturate();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		VSSaturate();


	public:
		enum
		{
			IN_VALUE,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSSaturate);
	VSTYPE_MARCO(VSSaturate);
}
#endif