#include <VSObject.h>
#include <VSProperty.h>
#include <iostream>
using namespace VSEngine2;
class B : public VSObject
{
	DECLARE_RTTI
	DECLARE_INITIAL
public:
	virtual ~B();
	B();
	static bool InitialDefaultState();
	static bool TerminalDefaultState();
};