#include "TestClone.h"
#include "VSProperty.h"
using namespace VSEngine2;

IMPLEMENT_RTTI(VSTestObject1,VSObject)
BEGIN_ADD_PROPERTY(VSTestObject1,VSObject);
REGISTER_PROPERTY(m_fTO1Float, TO1Float, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTestObject1)
IMPLEMENT_INITIAL_END
VSTestObject1::VSTestObject1()
{
	m_fTO1Float = 0.143f;
}
VSTestObject1::~VSTestObject1()
{
}
IMPLEMENT_RTTI(VSTestObject2, VSObject)
BEGIN_ADD_PROPERTY(VSTestObject2, VSObject);
REGISTER_PROPERTY(m_TestInt, TestInt, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_TestObject, TestObject, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_TestCloneObject, TestCloneObject, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_TestCopyObject, TestCopyObject, VSProperty::F_SAVE_LOAD_COPY)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTestObject2)
IMPLEMENT_INITIAL_END
VSTestObject2::VSTestObject2()
{
	m_TestInt = 11;
	m_TestCopyObject = NULL;
	m_TestCloneObject = NULL;
}
VSTestObject2::~VSTestObject2()
{
	m_TestCopyObject = NULL;
	VSMAC_DELETE(m_TestCloneObject);
}

IMPLEMENT_RTTI(VSTestCLoneCopy, VSObject)
BEGIN_ADD_PROPERTY(VSTestCLoneCopy, VSObject);
REGISTER_PROPERTY(m_Int, TestInt, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ArrayInt, TestArray, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY_DATA(m_FloatData,m_FloatDataNum,TestDataBuffer)
REGISTER_PROPERTY_FIXED_DATA(m_FixedFloatData,4, FixedFloatData,false)
REGISTER_PROPERTY(m_Name,TestName,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_TestStruct,TestStruct,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pTestObjectClone, TestObjectClone, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pTestObjectCopy, TestObjectCopy, VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_pTestObjectClonePtr, TestObjectClonePtr, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_TestString,TestString,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTestCLoneCopy)
IMPLEMENT_INITIAL_END
VSTestCLoneCopy::VSTestCLoneCopy()
{
	m_pTestObjectClone = NULL;
	m_FloatData = NULL;
	m_pTestObjectCopy = NULL;

	m_Int = 0;

	m_FloatDataNum = 0;

	m_Name = VSUsedName::ms_cGammaCorrect;

	m_TestString = VSString::ms_StringNULL;

	m_FixedFloatData = VS_NEW VSREAL[4];
	for (unsigned int i = 0; i < 4 ; i++)
	{
		m_FixedFloatData[i] = 1.0f * i;
	}
}
VSTestCLoneCopy::~VSTestCLoneCopy()
{
	VSMAC_DELETEA(m_FixedFloatData);
	VSMAC_DELETEA(m_FloatData);
	m_pTestObjectClone = NULL;
	m_pTestObjectCopy = NULL;
	m_pTestObjectClonePtr = NULL;
}