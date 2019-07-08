//这个示例的主要目的是，查找内存泄漏，用debug执行
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