//这个示例演示了VSList的使用
#include <VSMemManager.h>
#include <VSStack.h>
#include <VSList.h>
#include <VSQueue.h>
#include <iostream>
using namespace VSEngine2;
void main()
{
	VSList<int> List;
	List.AddElement(5);
	List.AddElement(0);
	List.AddElement(5);
	List.AddElement(12);
	List.AddElement(8);
	List.AddElement(1);

	for (VSList<int>::VSListIterator t = List.Begin(); t != List.End(); ++t)
	{
		std::cout << *t << std::endl;
	}

	std::cout << "------------------------------" << std::endl;

	VSList<int> List1(true);
	List1.AddElement(5);
	List1.AddElement(0);
	List1.AddElement(5);
	List1.AddElement(12);
	List1.AddElement(8);
	List1.AddElement(1);

	List1.Erase(1);
	for (VSList<int>::VSListIterator t = List1.Begin(); t.IsValid(); ++t)
	{
		std::cout << *t << std::endl;
	}
	getchar();
}