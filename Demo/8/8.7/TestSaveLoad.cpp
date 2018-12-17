#include "TestSaveLoad.h"
#include "VSProperty.h"
using namespace VSEngine2;

IMPLEMENT_RTTI(VSTestObject,VSObject)
BEGIN_ADD_PROPERTY(VSTestObject,VSObject);
REGISTER_PROPERTY(m_TestInt,TestInt,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_TestFloat,TestFloat,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTestObject)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool VSTestObject::InitialDefaultState()
{
	return 1;
}
bool VSTestObject::TerminalDefaultState()
{

	return 1;
}
VSTestObject::VSTestObject()
{
	m_TestInt = 11;
	m_TestFloat = 0.143f;
}
VSTestObject::~VSTestObject()
{
}

IMPLEMENT_RTTI(VSTestSaveLoad,VSObject)
BEGIN_ADD_PROPERTY(VSTestSaveLoad,VSObject);
REGISTER_PROPERTY(m_Int,TestInt,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ArrayInt,TestArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY_DATA(m_FloatData,m_FloatDataNum,TestDataBuffer)
REGISTER_PROPERTY_FIXED_DATA(m_FixedFloatData,4, FixedFloatData,false)
REGISTER_PROPERTY(m_TestStruct,TestStruct,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pTestObject2,TestObject2,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_TestObject3,TestObject3,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_TestString,TestString,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_AddName, AddName, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_ENUM_PROPERTY(m_EnumTest, EnumTest, TestEnum, VSProperty::F_SAVE_LOAD_CLONE)
BEGIN_ADD_ENUM
ADD_ENUM(TestEnum, TE_1)
ADD_ENUM(TestEnum, TE_2)
ADD_ENUM(TestEnum, TE_3)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTestSaveLoad)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END

bool VSTestSaveLoad::InitialDefaultState()
{
	return 1;
}
bool VSTestSaveLoad::TerminalDefaultState()
{

	return 1;
}
VSTestSaveLoad::VSTestSaveLoad()
{
	m_pTestObject2 = NULL;
	m_FloatData = NULL;
	
	m_AddName = VSUsedName::ms_cGammaCorrect;
	m_Int = 0;

	m_FloatDataNum = 0;

	m_TestString = VSString::ms_StringNULL;

	m_EnumTest = VSTestSaveLoad::TE_3;

	m_FixedFloatData = VS_NEW VSREAL[4];
	for (unsigned int i = 0; i < 4 ; i++)
	{
		m_FixedFloatData[i] = 1.0f * i;
	}
}
VSTestSaveLoad::~VSTestSaveLoad()
{
	VSMAC_DELETEA(m_FixedFloatData);
	VSMAC_DELETEA(m_FloatData);

	m_pTestObject2 = NULL;
}