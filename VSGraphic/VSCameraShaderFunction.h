#ifndef VSCAMERASHADERFUNCTION_H
#define VSCAMERASHADERFUNCTION_H
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSStream;

	class VSGRAPHIC_API VSViewWorldDir : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSViewWorldDir(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSViewWorldDir();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		VSViewWorldDir();


	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSViewWorldDir);
	VSTYPE_MARCO(VSViewWorldDir);


	class VSGRAPHIC_API VSCameraWorldPos : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSCameraWorldPos(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSCameraWorldPos();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		VSCameraWorldPos();


	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};


	};
	DECLARE_Ptr(VSCameraWorldPos);
	VSTYPE_MARCO(VSCameraWorldPos);

}
#endif