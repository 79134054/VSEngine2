#ifndef VSCONSTVALUE_H
#define VSCONSTVALUE_H
#include "VSMath.h"
#include "VSShaderFunction.h"
#include "VSUserConstant.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSConstValue : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:

		VSConstValue(const VSUsedName & ShowName,VSMaterial * pMaterial,unsigned int uiValueNum,bool bIsCustom);
		virtual ~VSConstValue();
		virtual unsigned int GetType() = 0;
		virtual bool GetOutPutValueString(VSString &OutString)const = 0;
		virtual bool GetFuntionString(VSString &OutString)const = 0;
		FORCEINLINE bool IsCustom()const
		{
			return m_bIsCustom;
		}
		virtual bool GetDeclareString(VSString &OutString,unsigned int uiRegisterID)const = 0;

		virtual unsigned int GetValueNum()const = 0;

		virtual unsigned int GetSize()const  = 0;
	protected:
		VSConstValue();
		bool m_bIsCustom;
		
	};
	DECLARE_Ptr(VSConstValue);
	VSTYPE_MARCO(VSConstValue);
	
	

}
#endif