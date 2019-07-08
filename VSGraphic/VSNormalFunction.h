#ifndef VSNORMALSHADERFUNCTION_H
#define VSNORMALSHADERFUNCTION_H
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSWorldNormal : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSWorldNormal(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSWorldNormal();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(VSString &OutString)const;
	protected:
		VSWorldNormal();


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
	DECLARE_Ptr(VSWorldNormal);
	VSTYPE_MARCO(VSWorldNormal);


	class VSGRAPHIC_API VSViewNormal : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSViewNormal(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSViewNormal();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(VSString &OutString)const;
	protected:
		VSViewNormal();


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
	DECLARE_Ptr(VSViewNormal);
	VSTYPE_MARCO(VSViewNormal);
}
#endif