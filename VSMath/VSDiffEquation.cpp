#include "VSDiffEquation.h"
using namespace VSEngine2;
VSDiffEquation::VSDiffEquation()
{
	m_pFunc = NULL;
	m_Step = 0.0;
	m_BeginT = 0.0;
	m_BeginX = 0.0;
}
/*----------------------------------------------------------------*/
VSDiffEquation::~VSDiffEquation()
{
	m_pFunc = NULL;
	m_Step = 0.0;
	m_BeginT = 0.0;
	m_BeginX = 0.0;
}
/*----------------------------------------------------------------*/
void VSDiffEquation::Update()
{
	if(!m_pFunc)
		return ;
	VSREAL HalfStep = m_Step / (VSREAL)2.0;
	VSREAL T = m_BeginT + HalfStep;
	
	VSREAL X = m_BeginX + m_pFunc->GetFirstDerivative(m_BeginT) * HalfStep;

	m_BeginX = X + m_pFunc->GetFirstDerivative(T) * HalfStep;

	m_BeginT +=m_Step;
}
/*----------------------------------------------------------------*/