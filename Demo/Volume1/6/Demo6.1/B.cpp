#include "B.h"
IMPLEMENT_RTTI(B, VSObject)
NO_PROPERTY(B)
IMPLEMENT_INITIAL_BEGIN(B)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState);
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState);
IMPLEMENT_INITIAL_END
B::~B()
{

}
B::B()
{

}
bool B::InitialDefaultState()
{
	std::cout << "This is B Initial" << std::endl;
	return true;
}
bool B::TerminalDefaultState()
{
	std::cout << "This is B Terminal" << std::endl;
	return true;
}