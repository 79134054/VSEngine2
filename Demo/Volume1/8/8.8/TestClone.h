#ifndef VSTEST_SAVE_LOAD_H
#define VSTEST_SAVE_LOAD_H
#include "VSObject.h"
#include "VSName.h"
namespace VSEngine2
{
	struct TestStruct
	{
		int m_sInt;
		VSREAL m_sFloat;
		TestStruct()
		{
			m_sInt = 1;
			m_sFloat = 4.0f;
		}
	};
	class VSTestObject1 : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VSTestObject1();
		~VSTestObject1();
		DECLARE_INITIAL
	public:
		VSREAL m_fTO1Float;
	};
	DECLARE_Ptr(VSTestObject1);
	VSTYPE_MARCO(VSTestObject1);
	class VSTestObject2 : public VSObject
	{
		
		//RTTI
		DECLARE_RTTI;
	public:
		VSTestObject2();
		~VSTestObject2();
		DECLARE_INITIAL
	public:
		int m_TestInt;
		VSTestObject1 m_TestObject;
		VSTestObject1 * m_TestCloneObject;
		VSTestObject1Ptr m_TestCopyObject;

	};
	DECLARE_Ptr(VSTestObject2);
	VSTYPE_MARCO(VSTestObject2);
	class VSTestCLoneCopy : public VSObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		VSTestCLoneCopy();
		~VSTestCLoneCopy();

		DECLARE_INITIAL
	public:
		VSArray<int>	m_ArrayInt;
		int				m_Int;
		VSREAL*			m_FloatData;
		unsigned int	m_FloatDataNum;
		VSREAL*			m_FixedFloatData;
		VSUsedName		m_Name;
		TestStruct		m_TestStruct;
		VSTestObject2*	m_pTestObjectClone;
		VSTestObject2*	m_pTestObjectCopy;
		VSTestObject2Ptr m_pTestObjectClonePtr;
		VSString		m_TestString;
	};
	DECLARE_Ptr(VSTestCLoneCopy);
	VSTYPE_MARCO(VSTestCLoneCopy);

};
#endif