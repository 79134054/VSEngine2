//这个示例演示了VSQueue的使用
#include <VSMemManager.h>
#include <VSStack.h>
#include <VSList.h>
#include <VSQueue.h>
#include <iostream>
using namespace VSEngine2;
void main()
{
	VSQueue<int> Queue;
	Queue.Enqueue(10);
	Queue.Enqueue(3);
	Queue.Enqueue(6);
	Queue.Enqueue(8);
	Queue.Enqueue(2);
	while (Queue.GetNum() > 0)
	{
		int Temp = 0;
		Queue.Dequeue(Temp);
		std::cout << Temp << std::endl;
	}
		

	std::cout << "------------------------------" << std::endl;


	getchar();
}