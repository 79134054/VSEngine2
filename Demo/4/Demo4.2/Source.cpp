//这个示例演示了VSArrayOrder的使用
#include <VSMemManager.h>
#include <VSArray.h>
#include <iostream>
using namespace VSEngine2;
void main()
{
	VSArrayOrder<int> Temp;

	Temp.AddElement(10);
	Temp.AddElement(2);
	Temp.AddElement(3);
	Temp.AddElement(5);
	Temp.AddElement(2);
	Temp.AddElement(7);

	for (unsigned int i = 0; i < Temp.GetNum(); i++)
	{
		std::cout << Temp[i] << std::endl;
	}

	std::cout << "------------------------------" << std::endl;
	unsigned int i = Temp.FindElement(7);
	Temp.Erase(i);

	for (unsigned int i = 0; i < Temp.GetNum(); i++)
	{
		std::cout << Temp[i] << std::endl;
	}
	getchar();
}