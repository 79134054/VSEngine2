#include "A.h"
#include "B.h"
IMPLEMENT_RTTI(A, VSObject)
NO_PROPERTY(A)
IMPLEMENT_INITIAL_BEGIN(A)
ADD_PRIORITY(B)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState);
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState);
IMPLEMENT_INITIAL_END
A::~A()
{

}
A::A()
{

}
bool A::InitialDefaultState()
{
	std::cout << "This is A Initial" << std::endl;
	return true;
}
bool A::TerminalDefaultState()
{
	std::cout << "This is A Terminal" << std::endl;
	return true;
}
