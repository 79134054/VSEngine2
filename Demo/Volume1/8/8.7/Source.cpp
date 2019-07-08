/*
展示不同版本兼容，可以看到对原来的类去掉了一些变量，但依然可以加载。读者可以对比Demo8.6，少了m_pTestObject1和m_Name，不过m_pTestObject1是指针类型，
它本来指向的对象没有被任何指针引用，这时会出现内存泄漏，不过引擎内部把这种情况处理，具体方法后面章节会讲解。
*/
#include <VSObject.h>
#include "TestSaveLoad.h"
#include <VSStream.h>
#include <iostream>
#include <VSEnumTrans.h>
#include <VSResourceManager.h>
#include <VSGraphicInclude.h>
using namespace VSEngine2;


void main()
{
 	VSInitSystem();
 	VSInitMath();
	VSMain::Initialize();
	
	VSTestSaveLoadPtr pTestSaveLoad = NULL;
	VSStream TestStream;
	TestStream.NewLoad("test_stream");
	pTestSaveLoad = (VSTestSaveLoad *)TestStream.GetObjectByRtti(VSTestSaveLoad::ms_Type);
	//print Array
	std::cout << "****************Print Array****************" << std::endl;
	for (unsigned int i = 0; i < pTestSaveLoad->m_ArrayInt.GetNum();i++)
	{
		std::cout << pTestSaveLoad->m_ArrayInt[i] << " ";
	}
	std::cout << std::endl;

	//Print Int
	std::cout << "****************Print Int****************" << std::endl;
	std::cout << pTestSaveLoad->m_Int << std::endl;

	//Test Data
	std::cout << "****************Print Data****************" << std::endl;
	std::cout << "Print Data Size" << pTestSaveLoad->m_FloatDataNum << std::endl;
	for (unsigned int i = 0; i < pTestSaveLoad->m_FloatDataNum; i++)
	{
		std::cout << pTestSaveLoad->m_FloatData[i] << " ";
	}
	std::cout << std::endl;

	//Test Data
	std::cout << "****************Print FixedData****************" << std::endl;
	for (unsigned int i = 0; i < 4; i++)
	{
		std::cout << pTestSaveLoad->m_FixedFloatData[i] << " ";
	}
	std::cout << std::endl;

	//Test Custom Save Load     Remove this Property
	//std::cout << "****************Print Custom Save Load****************" << std::endl;
	//std::cout << pTestSaveLoad->m_Name.GetBuffer() << std::endl;


	//Test Add New Property
	std::cout << "****************Print Custom Save Load****************" << std::endl;
	std::cout << pTestSaveLoad->m_AddName.GetBuffer() << std::endl;

	//Test Struct
	std::cout << "****************Print Struct****************" << std::endl;
	std::cout << pTestSaveLoad->m_TestStruct.m_sFloat << " " << pTestSaveLoad->m_TestStruct.m_sInt << std::endl;


	//Test Object*   Remove this Property
	//std::cout << "**************** Print Object*  ****************" << std::endl;
	//std::cout << pTestSaveLoad->m_pTestObject1->m_TestFloat << " " << pTestSaveLoad->m_pTestObject1->m_TestInt << std::endl;

	//Test ObjectPtr*
	std::cout << "****************Print ObjectPtr**************** " << std::endl;
	std::cout << pTestSaveLoad->m_pTestObject2->m_TestFloat << " " << pTestSaveLoad->m_pTestObject2->m_TestInt << std::endl;

	//Test Object
	std::cout << "****************Print Object**************** " << std::endl;
	std::cout << pTestSaveLoad->m_TestObject3.m_TestFloat << " " << pTestSaveLoad->m_TestObject3.m_TestInt << std::endl;

	//Test String
	std::cout << "****************Print String****************" << std::endl;
	std::cout << pTestSaveLoad->m_TestString.GetBuffer() << std::endl;
	
	//Test Enum
	VSArray<VSString> TestEnumString;
	GET_ENUMARRAY(TestEnum, TestEnumString)
	std::cout << "****************Print Enum*******************" << std::endl;
	std::cout << TestEnumString[pTestSaveLoad->m_EnumTest].GetBuffer() << std::endl;
	pTestSaveLoad = NULL;
 	VSMain::Terminate();
	getchar();
}
