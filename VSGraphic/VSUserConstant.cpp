#include "VSUserConstant.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSUserConstant,VSObject)
BEGIN_ADD_PROPERTY(VSUserConstant,VSObject)
REGISTER_PROPERTY(m_uiValueType,ValueType,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterIndex,RegisterIndex,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterNum,RegisterNum,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_Name,Name,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_NameInShader, NameInShader, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY_DATA(m_pDate,m_uiSize,Date);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSUserConstant)
IMPLEMENT_INITIAL_END
VSUserConstant::VSUserConstant()
{
	m_pDate = NULL;
	m_uiRegisterIndex = 0;
	m_uiValueType = VSMAX_INTEGER;
	m_uiRegisterNum = 0;
	m_uiSize = 0;
}
VSUserConstant::VSUserConstant(const VSUsedName & Name,const void * pDate,
							   unsigned int uiSize,
							  unsigned int uiRegisterIndex,unsigned int uiRegisterNum,
							   unsigned int uiValueType)
{
	VSMAC_ASSERT(uiValueType < VT_MAX && uiSize > 0 && uiRegisterNum > 0);

	m_Name = Name;
	m_uiValueType = uiValueType;
	m_uiSize = uiSize;
	m_pDate = new unsigned char[uiSize];
	m_uiRegisterIndex = uiRegisterIndex;	
	m_uiRegisterNum = uiRegisterNum;
	VSMAC_ASSERT(m_pDate);

	if(pDate)
	{
		VSMemcpy(m_pDate,pDate,uiSize);
	}
	else
	{
		VSMemset(m_pDate,0,uiSize);
	}
	m_NameInShader = Name;
}
VSUserConstant::VSUserConstant(const VSUsedName & Name, const VSUsedName & NameInShader, const void * pDate,
	unsigned int uiSize,
	unsigned int uiRegisterIndex, unsigned int uiRegisterNum,
	unsigned int uiValueType)
{
	VSMAC_ASSERT(uiValueType < VT_MAX && uiSize > 0 && uiRegisterNum > 0);

	m_Name = Name;
	m_uiValueType = uiValueType;
	m_uiSize = uiSize;
	m_pDate = new unsigned char[uiSize];
	m_uiRegisterIndex = uiRegisterIndex;
	m_uiRegisterNum = uiRegisterNum;
	VSMAC_ASSERT(m_pDate);

	if (pDate)
	{
		VSMemcpy(m_pDate, pDate, uiSize);
	}
	else
	{
		VSMemset(m_pDate, 0, uiSize);
	}
	m_NameInShader = NameInShader;
}
VSUserConstant::~VSUserConstant()
{
	VSMAC_DELETEA(m_pDate);

}


bool VSUserConstant::SetDate(void *pElementDate)
{
	if(!pElementDate)
		return 0;
	void * pDest =(unsigned char * )m_pDate;
	VSMemcpy(pDest,pElementDate,m_uiSize);

	return 1;
}
