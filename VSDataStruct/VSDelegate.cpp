#include "VSDelegateList.h"
using namespace VSEngine2;
// DECLARE_DELEGATE(VSDelegate)
// DECLARE_DELEGATE_ONE(VSDelegate1)
// DECLARE_DELEGATE_TWO(VSDelegate2)
// DECLARE_DELEGATE_THREE(VSDelegate3)
// VSDelegate<int> a;
// VSDelegate1<int,int> f;
// VSDelegate2<int,int,int> c;
// int ss(int k)
// {
// 	return 1;
// }
// int sss(int k,int m)
// {
// 	return 1;
// }
// void m()
// {
// 	f = VSDelegate1<int,int>::FromFun<&ss>();
// 	c = VSDelegate2<int,int,int>::FromFun<&sss>();
// }

// using namespace VSEngine2;
// VSDelegate0<int> a;
// VSDelegate1<int,int> b;
// VSDelegate2<int,int,int> c;
// VSDelegate3<int,int,int,int> d;
using namespace VSEngine2;

#ifdef DELEGATE_PREFERRED_SYNTAX
typedef VSDelegateEvent<int(int, int)> MouseMoveEvent;
#else
typedef VSDelegateEvent2<int, int, int> MouseMoveEvent;
#endif
int sss(int k, int m)
{
	return 1;
}
class A
{
public:
	int LL(int a, int b)const
	{
		return 1;
	}
};
void m()
{
	A a;
	MouseMoveEvent MouseMove;
	MouseMove += MouseMoveEvent::Handler::FromFun<&sss>();
	MouseMove += MouseMoveEvent::Handler::FromMethod<A,&A::LL>(&a);

	MouseMove(1, 2);
}