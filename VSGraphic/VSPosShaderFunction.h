#ifndef VSPOSSHADERFUNCTION_H
#define VSPOSSHADERFUNCTION_H
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSWorldPos : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSWorldPos(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSWorldPos();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(VSString &OutString)const;
	protected:
		VSWorldPos();


	public:
		enum
		{
			OUT_VALUE,
			OUT_COLOR_R,
			OUT_COLOR_G,
			OUT_COLOR_B,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSWorldPos);
	VSTYPE_MARCO(VSWorldPos);


	class VSGRAPHIC_API VSViewPos : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSViewPos(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSViewPos();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(VSString &OutString)const;
	protected:
		VSViewPos();


	public:
		enum
		{
			OUT_VALUE,
			OUT_COLOR_R,
			OUT_COLOR_G,
			OUT_COLOR_B,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSViewPos);
	VSTYPE_MARCO(VSViewPos);

	class VSGRAPHIC_API VSProjectPos : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSProjectPos(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSProjectPos();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(VSString &OutString)const;
	protected:
		VSProjectPos();


	public:
		enum
		{
			OUT_VALUE,
			OUT_COLOR_R,
			OUT_COLOR_G,
			OUT_COLOR_B,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSProjectPos);
	VSTYPE_MARCO(VSProjectPos);
}
#endif