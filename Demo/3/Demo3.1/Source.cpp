//���ʾ������ҪĿ���ǣ������ڴ�й©����debugִ��
#include <VSMemManager.h>
void Fun()
{
	char * k = VS_NEW char[10];
}
void main()
{
	int * a = VS_NEW int;
	*a = 5;
	Fun();
}