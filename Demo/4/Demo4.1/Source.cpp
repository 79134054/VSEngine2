//���ʾ������ҪĿ���ǣ���ʾ�����������ʼ������ù��캯������ҿ����ڹ��캯�������¸��ϵ㿴��
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