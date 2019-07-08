/*
函数反射调用的例子，通过函数名字和传递参数来调用函数
*/

#include <VSObject.h>
#include "TestClone.h"
#include <VSStream.h>
#include <iostream>
#include <VSEnumTrans.h>
using namespace VSEngine2;
using namespace std;
#include <VSProperty.h>


void main()
{
 	VSInitSystem();
 	VSInitMath();
	VSMain::Initialize();
	VSObject * Temp = VS_NEW VSTestObject2();
	int para = 3;
	int t = 0;
	VSUsedName TempName = _T("Test");
	Temp->Process(TempName,&para,&t , 1);

	VSUsedName TempName2 = _T("Test2");
	VSString  KK = _T("Fuck");
	Temp->Process(TempName2, &KK);

	VSUsedName TempName3 = _T("Test3");

	char c = 3;
	Temp->CallVoidFun(TempName3,c, Temp);

	VSMAC_DELETE(Temp);
 	VSMain::Terminate();
	getchar();
}
