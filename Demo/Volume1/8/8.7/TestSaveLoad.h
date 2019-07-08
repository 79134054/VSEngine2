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
	class VSTestObject : public VSObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		VSTestObject();
		~VSTestObject();
		DECLARE_INITIAL
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		int m_TestInt;
		VSREAL m_TestFloat;

	};
	DECLARE_Ptr(VSTestObject);
	VSTYPE_MARCO(VSTestObject);
	class VSTestSaveLoad : public VSObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		VSTestSaveLoad();
		~VSTestSaveLoad();

		DECLARE_INITIAL
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		VSArray<int>	m_ArrayInt;
		int				m_Int;
		VSREAL*			m_FloatData;
		unsigned int	m_FloatDataNum;
		VSREAL*			m_FixedFloatData;
		//VSUsedName		m_Name; Remove This Property
		TestStruct		m_TestStruct;
		//VSTestObject*	m_pTestObject1; Remove This Property
		VSTestObjectPtr m_pTestObject2;
		VSTestObject	m_TestObject3;
		VSString		m_TestString;
		VSUsedName		m_AddName; // add This Property
		enum TestEnum
		{
			TE_1,
			TE_2,
			TE_3,
			TE_MAX
		};
		TestEnum m_EnumTest;
	};
	DECLARE_Ptr(VSTestSaveLoad);
	VSTYPE_MARCO(VSTestSaveLoad);

};
#endif