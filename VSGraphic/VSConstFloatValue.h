#ifndef VSCONSTFLOATVALUE_H
#define VSCONSTFLOATVALUE_H
#include "VSConstValue.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSConstFloatValue : public VSConstValue
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSConstFloatValue(const VSUsedName & ShowName,VSMaterial * pMaterial,unsigned int uiValueNum,bool bIsCustom);
		virtual ~VSConstFloatValue();
		virtual unsigned int GetType()
		{
			return VSUserConstant::VT_FLOAT;
		}
		void SetValue(unsigned int uiIndex,VSREAL Value);

		virtual bool GetOutPutValueString(VSString &OutString)const;
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual unsigned int GetValueNum()const
		{
			return m_Value.GetNum();
		}
		virtual unsigned int GetSize()const
		{
			return m_Value.GetNum() * sizeof(VSREAL);
		}
		bool GetDeclareString(VSString &OutString,unsigned int uiRegisterID)const;
		virtual void ResetInShaderName();
		VSArray<VSREAL>& GetValue() { return m_Value;}
	protected:
		VSConstFloatValue();
		VSArray<VSREAL> m_Value;

	public:
		enum
		{
			OUT_VALUE,
			OUT_VALUE_X,
			OUT_VALUE_Y,
			OUT_VALUE_Z,
			OUT_VALUE_W,
			OUT_MAX
		};
	};
	DECLARE_Ptr(VSConstFloatValue);
	VSTYPE_MARCO(VSConstFloatValue);

}
#endif