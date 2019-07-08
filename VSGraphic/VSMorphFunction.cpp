#include "VSMorphFunction.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSMorphFunction,VSMorphBaseFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSMorphFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSMorphFunction,VSMorphBaseFunction)
END_ADD_PROPERTY
VSMorphFunction::VSMorphFunction()
{

}
VSMorphFunction::VSMorphFunction(const VSUsedName & ShowName,VSMorphTree * pMorphTree)
:VSMorphBaseFunction(ShowName,pMorphTree)
{

}
VSMorphFunction::~VSMorphFunction()
{

}
void VSMorphFunction::UpdateGeometryDate(unsigned int GeometryIndex)
{
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			VSMorphFunction *pMorphFunction = (VSMorphFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pMorphFunction)
			{
				pMorphFunction->UpdateGeometryDate(GeometryIndex);
			}
		}
	}
}
void VSMorphFunction::UpdateVertexDate(unsigned int uiVertexIndex)
{
	for (unsigned int i = 0 ; i < m_pInput.GetNum() ;i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			VSMorphFunction *pMorphFunction = (VSMorphFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if(pMorphFunction)
			{
				pMorphFunction->UpdateVertexDate(uiVertexIndex);
			}
		}
	}
}
void VSMorphFunction::ClearChangeFlag()
{
	for (unsigned int i = 0 ; i < MAX_NUM_POS3 ; i++)
	{
		m_bPosChange[i] = false;
	}
	

	for (unsigned int i = 0 ; i < MAX_NUM_NORMAL3 ; i++)
	{
		m_bNormalChange[i] = false;
	}

	m_bTangentChange = false;
	m_bBinormalChange = false;


	for (unsigned int i = 0 ; i < MAX_NUM_COLOR ; i++)
	{
		m_bColorChange[i] = false;
	}


}