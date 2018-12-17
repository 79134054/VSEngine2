#include "VSRect2.h"
using namespace VSEngine2;
VSRect2::VSRect2(void)
{

}
VSRect2::VSRect2(const VSVector2& Min, const VSVector2 &Max)
{
	m_Min = Min;
	m_Max = Max;
}