//这个示例演示了C++代理的使用
#include <VSMemManager.h>
#include <VSDelegateList.h>
#include <iostream>
using namespace VSEngine2;
#ifdef DELEGATE_PREFERRED_SYNTAX
typedef VSDelegateEvent<int(int, int)> MouseMoveEvent;
typedef VSDelegateEvent<int(int, int, int)> Event1;
#else
typedef VSDelegateEvent2<int,int, int> MouseMoveEvent;
typedef VSDelegateEvent3<int,int, int, int> Event1;
#endif

int sss(int a, int b)
{
	std::cout << "i am sss" << std::endl;
	return 1;
}
int fff(int a, int b, int c)
{
	std::cout << "i am fff" << std::endl;
	return 1;
}
class A
{
public:
	int LL(int a, int b)const
	{
		std::cout << "i am LL" << std::endl;
		return 1;
	}
	int LLL(int a, int b, int c)
	{
		std::cout << "i am LLL" << std::endl;
		return 1;
	}
};
void m()
{
	A a;
	MouseMoveEvent MouseMove;
	MouseMove += MouseMoveEvent::Handler::FromFun<&sss>();
	MouseMove += MouseMoveEvent::Handler::FromMethod<A, &A::LL>(&a);
	MouseMove(1, 2);

	Event1 E1;
	E1.AddFun<&fff>();
	E1.AddMethod<A, &A::LLL>(&a);
	E1(2,4,3);

}

void main()
{
	m();

	getchar();
}