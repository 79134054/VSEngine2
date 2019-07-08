#include "TestClone.h"
#include "VSProperty.h"
#include <iostream>
using namespace VSEngine2;
IMPLEMENT_RTTI(VSTestObject1,VSObject)
BEGIN_ADD_PROPERTY(VSTestObject1,VSObject);
REGISTER_PROPERTY(m_fTO1Float, TO1Float, VSProperty::F_SAVE_LOAD_CLONE)
BEGIN_REGISTER_FUNCTION_ONEPARAMETER(VSTestObject1, Test, VSFunction::F_DEFAULT, int, int, s)
BEGIN_REGISTER_VOID_FUNCTION_ONEPARAMETER(VSTestObject1, Test2, VSFunction::F_DEFAULT, VSString , s)
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
int VSTestObject1::Test(int s)
{
	return s + 2;
}
void VSTestObject1::Test2(VSString s)
{
	std::cout << s.GetBuffer() << std::endl;
}
IMPLEMENT_RTTI(VSTestObject2, VSTestObject1)
BEGIN_ADD_PROPERTY(VSTestObject2, VSTestObject1);
REGISTER_PROPERTY(m_TestInt, TestInt, VSProperty::F_SAVE_LOAD_CLONE)
BEGIN_REGISTER_FUNCTION_ONEPARAMETER(VSTestObject2, Test, VSFunction::F_DEFAULT, int, int, s)
BEGIN_REGISTER_VOID_FUNCTION_TWOPARAMETER(VSTestObject2, Test3, VSFunction::F_DEFAULT, char, s, VSTestObject2 *, p)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTestObject2)
IMPLEMENT_INITIAL_END
VSTestObject2::VSTestObject2()
{
	m_TestInt = 11;

}
VSTestObject2::~VSTestObject2()
{

}
int VSTestObject2::Test(int s)
{
	return s + 3;
}
void VSTestObject2::Test3(char s, VSTestObject2 * p)
{
	p->m_TestInt = m_TestInt + s;
}
