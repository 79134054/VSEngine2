#ifndef VSRTTI_H
#define VSRTTI_H
#include"VSRtti.marc"
#include "VSString.h"
#include "VSGraphic.h"
#include "VSMemManager.h"
#include "VSArray.h"

namespace VSEngine2
{
class VSObject;
typedef VSObject *(*CreateObjectFun)();
class VSProperty;
class VSFunction;
class VSGRAPHIC_API VSRtti : public VSMemObject
{
public:
	VSRtti(const TCHAR * pcRttiName,VSRtti *pBase,CreateObjectFun COF);
	~VSRtti();


	FORCEINLINE const VSString &GetName()const;
	FORCEINLINE bool IsSameType(const VSRtti &Tpye) const;
	FORCEINLINE bool IsDerived(const VSRtti &Tpye) const;
	FORCEINLINE VSRtti* GetBase()const
	{
		return m_pBase;
	}
	VSProperty * GetProperty(unsigned int uiIndex)const;
	VSProperty *GetProperty(const VSString & PropertyName)const;
	unsigned int GetPropertyNum()const;
	void AddProperty(VSProperty * pProperty);
	void AddProperty(VSRtti & Rtti);
	friend class VSObject;
	typedef VSObject *(*CreateObjectFun)();
	void ClearProperty();

	VSFunction * GetFunction(unsigned int uiIndex)const;
	unsigned int GetFunctionNum()const;
	void AddFunction(VSFunction * pProperty);
	void AddFunction(VSRtti & Rtti);
	void ClearFunction();
private:
	VSString		m_cRttiName;
	VSRtti*	m_pBase;
	VSArray<VSProperty *> m_PropertyArray;
	VSArray<VSFunction *> m_FunctionArray;
	CreateObjectFun m_CreateFun;
};

#include"VSRtti.inl"
}
#endif