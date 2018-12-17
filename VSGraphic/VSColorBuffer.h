#ifndef VSCOLORBUFFER_H
#define VSCOLORBUFFER_H
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSColorBuffer : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSColorBuffer(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSColorBuffer();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetInputValueString(VSString &InputString)const;
		virtual bool GetOutPutValueString(VSString &OutString)const;
	protected:
		VSColorBuffer();

	public:
		enum
		{
			IN_TEXCOORD,
			IN_MAX
		};
		enum
		{
			OUT_COLOR,
			OUT_COLOR_R,
			OUT_COLOR_G,
			OUT_COLOR_B,
			OUT_MAX
		};
	};
	DECLARE_Ptr(VSColorBuffer);
	VSTYPE_MARCO(VSColorBuffer);
}
#endif