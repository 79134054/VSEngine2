/*
对象的克隆和拷贝
*/

#include <VSObject.h>
#include "TestClone.h"
#include <VSStream.h>
#include <iostream>
#include <VSEnumTrans.h>
using namespace VSEngine2;
using namespace std;

void main()
{
 	VSInitSystem();
 	VSInitMath();
	VSMain::Initialize();
	VSTestObject1* pTO1_1 = VS_NEW VSTestObject1();
	pTO1_1->m_fTO1Float = 1.0f;
	VSTestObject1* pTO1_2 = VS_NEW VSTestObject1();
	pTO1_2->m_fTO1Float = 2.0f;

	VSTestObject2Ptr pTO2 = VS_NEW VSTestObject2();

	pTO2->m_TestCloneObject = pTO1_1;
	pTO2->m_TestCopyObject = pTO1_2;
	pTO2->m_TestInt = 123;
	pTO2->m_TestObject.m_fTO1Float = 15.0f;

	VSObject * pCloneTestObject2 = VSObject::CloneCreateObject(pTO2);
	
	VSMAC_DELETE(pCloneTestObject2);


	VSTestCLoneCopy * pTestCloneCopy1 = VS_NEW VSTestCLoneCopy();
	
	pTestCloneCopy1->m_ArrayInt.AddElement(7);
	pTestCloneCopy1->m_ArrayInt.AddElement(27);
	pTestCloneCopy1->m_ArrayInt.AddElement(75);
	pTestCloneCopy1->m_ArrayInt.AddElement(723);
	pTestCloneCopy1->m_Int = 234;
	pTestCloneCopy1->m_FloatData = VS_NEW VSREAL[5];
	pTestCloneCopy1->m_FloatDataNum = 5;
	for (unsigned int i = 0; i < 5; i++)
	{
		pTestCloneCopy1->m_FloatData[i] = i * 3.0f;
	}
	for (unsigned int i = 0; i < 4; i++)
	{
		pTestCloneCopy1->m_FixedFloatData[i] = i * 2.0f;
	}
	pTestCloneCopy1->m_Name = VSUsedName::ms_cMainScene;
	pTestCloneCopy1->m_TestStruct.m_sFloat = 11.11f;
	pTestCloneCopy1->m_TestStruct.m_sInt = 1111;
	pTestCloneCopy1->m_pTestObjectClone = pTO2;
	pTestCloneCopy1->m_pTestObjectCopy = pTO2;
	pTestCloneCopy1->m_pTestObjectClonePtr = pTO2;
	pTestCloneCopy1->m_TestString = _T("Fuck");

	VSObject * pTestCloneCopy2 = VSObject::CloneCreateObject(pTestCloneCopy1);
	VSMAC_DELETE(pTestCloneCopy2);
	VSMAC_DELETE(pTestCloneCopy1);
	pTO2 = NULL;
 	VSMain::Terminate();
	getchar();
}
