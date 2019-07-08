//这个示例的主要目的是，演示在容器里面初始化会调用构造函数，大家可以在构造函数里面下个断点看看
#include <VSMemManager.h>
#include <VSArray.h>
using namespace VSEngine2;
class MyClass
{
public:
	MyClass();
	~MyClass();

	int i;
};

MyClass::MyClass()
{
	i = 0;
}

MyClass::~MyClass()
{
	i = 2;
}
void main()
{
	VSArray<MyClass> k1;
	k1.SetBufferNum(3);
	k1[0].i = 3;
	k1.AddBufferNum(5);

}