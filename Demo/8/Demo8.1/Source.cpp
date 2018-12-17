//展示智能指针的用法。
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
void main()
{
	VSInitSystem();
	VSInitMath();
	VSMain::Initialize();
	VSPointer<A> SmartP1 = NULL;
	A * p = VS_NEW A();
	SmartP1 = p;
	VSPointer<A> SmartP2(VS_NEW A());
	SmartP1 = SmartP2;
	VSMain::Terminate();

}
