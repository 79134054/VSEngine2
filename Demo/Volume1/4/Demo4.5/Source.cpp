//这个示例演示了VSStack的使用
#include <VSMemManager.h>
#include <VSStack.h>
#include <VSList.h>
#include <VSQueue.h>
#include <iostream>
using namespace VSEngine2;
void main()
{
	VSStack<int> Stack;
	Stack.Push(10);
	Stack.Push(3);
	Stack.Push(6);
	Stack.Push(8);
	Stack.Push(2);
	while (Stack.GetNum() > 0)
	{
		int Temp = 0;
		Stack.Pop(Temp);
		std::cout << Temp << std::endl;
	}
		

	std::cout << "------------------------------" << std::endl;


	getchar();
}