//不要把智能指针作为参数和返回值，否则有可能出现内存泄漏
#include <VSObject.h>
#include <iostream>
using namespace VSEngine2;
class A : public VSReference
{
public:
	A();
	~A();

private:

};

A::A()
{

}

A::~A()
{

}
void Test(VSPointer<A> Smartp)
{

}
void main()
{
	VSInitSystem();
	VSInitMath();
	VSMain::Initialize();

	A * p = VS_NEW A();
	Test(p);

	VSMain::Terminate();

}
