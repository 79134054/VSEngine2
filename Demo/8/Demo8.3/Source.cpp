//智能指针的循环引用
#include <VSObject.h>
#include <iostream>
using namespace VSEngine2;
DECLARE_Ptr(B);
class A : public VSReference
{
public:
	BPtr m_b;

};
DECLARE_Ptr(A);
class B : public VSReference
{
public:
	APtr m_a;

};
DECLARE_Ptr(B);
void main()
{
 	VSInitSystem();
 	VSInitMath();
	VSMain::Initialize();
	APtr SmartP1 = VS_NEW A();
	BPtr SmartP2 = VS_NEW B();
	SmartP1->m_b = SmartP2;
	SmartP2->m_a = SmartP1;
 	VSMain::Terminate();

}
