//这个示例演示了VSMap的使用
#include <VSMemManager.h>
#include <VSMap.h>
#include <iostream>
using namespace VSEngine2;
void main()
{
	VSMap<int, char> Temp;
	Temp.AddElement(16, 'a');
	Temp.AddElement(1, 'b');
	Temp.AddElement(3, 's');
	Temp.AddElement(7, 'm');
	Temp.AddElement(0, 'k');
	Temp.AddElement(-4, 'c');

	for (unsigned int i = 0; i < Temp.GetNum(); i++)
	{
		std::cout << "Key :" << Temp[i].Key << "    Value :"<<Temp[i].Value << std::endl;
	}


	std::cout << "------------------------------" << std::endl;
	Temp.Sort(0, Temp.GetNum() - 1);


	for (unsigned int i = 0; i < Temp.GetNum(); i++)
	{
		std::cout << "Key :" << Temp[i].Key << "    Value :" << Temp[i].Value << std::endl;
	}

	getchar();
}