#ifndef VSTEST_SAVE_LOAD_H
#define VSTEST_SAVE_LOAD_H
#include "VSObject.h"
#include "VSName.h"
namespace VSEngine2
{

	class VSTestObject1 : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VSTestObject1();
		~VSTestObject1();
		DECLARE_INITIAL
	public:
		VSREAL m_fTO1Float;
		int Test(int s);
		void Test2(VSString s);
	};
	DECLARE_Ptr(VSTestObject1);
	VSTYPE_MARCO(VSTestObject1);
	class VSTestObject2 : public VSTestObject1
	{
		
		//RTTI
		DECLARE_RTTI;
	public:
		VSTestObject2();
		~VSTestObject2();
		DECLARE_INITIAL
	public:
		int m_TestInt;
		int Test(int s);
		void Test3(char s, VSTestObject2 * p);
	};
	DECLARE_Ptr(VSTestObject2);
	VSTYPE_MARCO(VSTestObject2);

};
#endif