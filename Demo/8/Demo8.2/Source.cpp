//��Ҫ������ָ����Ϊ�����ͷ���ֵ�������п��ܳ����ڴ�й©
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
