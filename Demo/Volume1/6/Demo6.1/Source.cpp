//演示自定义类的初始化顺序
#include <VSObject.h>
#include <VSProperty.h>
#include <iostream>
using namespace VSEngine2;
void main()
{
	VSInitSystem();
	VSInitMath();
	VSMain::Initialize();
	
	VSMain::Terminate();
	getchar();
}