#ifndef VSTEXCOORD_H
#define VSTEXCOORD_H
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSTexCoord : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSTexCoord(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSTexCoord();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		VSTexCoord();
		unsigned int m_uiTexCoordIndex;

	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};
		enum //TexCoord Index
		{
			TI_1,
			TI_2,
			TI_MAX
		};
		FORCEINLINE void SetTexCoordIndex(unsigned int uiIndex)
		{
			if (uiIndex >= TI_MAX)
			{
				return;
			}
			m_uiTexCoordIndex = uiIndex;
		}
	};
	DECLARE_Ptr(VSTexCoord);
	VSTYPE_MARCO(VSTexCoord);
}
#endif