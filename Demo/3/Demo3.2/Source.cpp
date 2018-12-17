//演示了栈内存分配器的使用方式，一旦退出当前函数，申请的栈内存的空间才会释放
#include <VSMemManager.h>
using namespace VSEngine2;
struct MyStruct
{
	int a;
	char c;
};
void Fun()
{

}
void Fun1()
{

}
void Fun2()
{
	Fun1();
}

void Fun3()
{
	Fun2();
}
void Fun4()
{

}
void Fun5()
{
	VSStackMemAlloc<MyStruct> Temp(3);
	MyStruct * p = Temp.GetPtr();
	p[2].a = 1;
	p[2].c = 'k';
	Fun3();
}

void main()
{
	VSStackMemAlloc<MyStruct> Temp(2);
	Fun4();
	Fun5();
	MyStruct * p = Temp.GetPtr();
	p[1].a = 1;
	p[1].c = 'k';

	getchar();
}