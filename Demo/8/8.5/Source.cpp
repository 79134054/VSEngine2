/*
展示了序列化存储，这个例子中展示了几乎所有支持的类型，结构体，自定义类型，指针，智能指针，枚举，还有常类型。最后文件存在bin\ test_stream文件。
*/
#include <VSObject.h>
#include "TestSaveLoad.h"
#include <VSStream.h>
#include <iostream>
using namespace VSEngine2;


void main()
{
 	VSInitSystem();
 	VSInitMath();
	VSMain::Initialize();
	
	TestStruct test_struct;
	test_struct.m_sFloat = 12.3f;
	test_struct.m_sInt = 56;

	VSTestSaveLoadPtr TestSaveLoad = VS_NEW VSTestSaveLoad();
	//test VSArray
	TestSaveLoad->m_ArrayInt.AddElement(3);
	TestSaveLoad->m_ArrayInt.AddElement(4);
	TestSaveLoad->m_ArrayInt.AddElement(6);
	TestSaveLoad->m_ArrayInt.AddElement(302);
	TestSaveLoad->m_ArrayInt.AddElement(378);

	//Test Int
	TestSaveLoad->m_Int = -12;

	//Test Data
	TestSaveLoad->m_FloatDataNum = 4;
	TestSaveLoad->m_FloatData = VS_NEW VSREAL[TestSaveLoad->m_FloatDataNum];
	TestSaveLoad->m_FloatData[0] = -4.23f;
	TestSaveLoad->m_FloatData[1] = -0.004f;
	TestSaveLoad->m_FloatData[2] = 837.1f;
	TestSaveLoad->m_FloatData[3] = 56.04f;

	//Test fixed Data
	TestSaveLoad->m_FixedFloatData[0] = -14.23f;
	TestSaveLoad->m_FixedFloatData[1] = -10.004f;
	TestSaveLoad->m_FixedFloatData[2] = 1837.1f;
	TestSaveLoad->m_FixedFloatData[3] = 156.04f;

	//Test Custom Save Load
	TestSaveLoad->m_Name = _T("fuck");

	//Test Struct
	TestSaveLoad->m_TestStruct = test_struct;

	//Test Object*
	TestSaveLoad->m_pTestObject1 = VS_NEW VSTestObject();
	TestSaveLoad->m_pTestObject1->m_TestFloat = 0.844f;
	TestSaveLoad->m_pTestObject1->m_TestInt = -32;

	//Test ObjectPtr
	TestSaveLoad->m_pTestObject2 = VS_NEW VSTestObject();
	TestSaveLoad->m_pTestObject2->m_TestFloat = 0.53f;
	TestSaveLoad->m_pTestObject2->m_TestInt = 72;
	
	//Test Object
	TestSaveLoad->m_TestObject3.m_TestFloat = 0.779f;
	TestSaveLoad->m_TestObject3.m_TestInt = 48;
	
	//Test String
	TestSaveLoad->m_TestString = _T("i'm string");

	//Test Enmu
	TestSaveLoad->m_EnumTest = VSTestSaveLoad::TE_2;

	VSStream TestStream;
	TestStream.SetStreamFlag(VSStream::AT_REGISTER);
	TestStream.ArchiveAll(TestSaveLoad);
	TestStream.NewSave("test_stream");

	TestSaveLoad = NULL;
	
 	VSMain::Terminate();
}
