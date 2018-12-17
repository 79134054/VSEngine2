#include <VSObject.h>
#include <VSProperty.h>
#include <iostream>
using namespace VSEngine2;
class A : public VSObject
{
	DECLARE_RTTI
	DECLARE_INITIAL
public:
	virtual ~A();
	A();
	static bool InitialDefaultState();
	static bool TerminalDefaultState();
};