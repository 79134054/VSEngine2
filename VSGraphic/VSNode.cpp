#include "VSNode.h"
#include "VSMath.h"
#include "VSGeometry.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSNodeComponent.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSNode, VSSpatial)
BEGIN_ADD_PROPERTY(VSNode,VSSpatial)
REGISTER_PROPERTY(m_pChild, Child, VSProperty::F_SAVE_LOAD_CLONE  | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
VSNode::VSNode()
{
	DeleteAllChild();

}
VSNode::~VSNode()
{
	m_pChild.Clear();
}

unsigned int VSNode::AddChild(VSSpatial * pChild)
{
	if(!pChild)
		return VSMAX_INTEGER;
	for (unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{
		if (m_pChild[i] == pChild)
		{
			return i;
		}
	}
	pChild->SetParent(this);	
	m_pChild.AddElement(pChild);
	return m_pChild.GetNum() - 1;
}
unsigned int VSNode::DeleteChild(VSSpatial *pChild)
{
	if(!pChild)
		return VSMAX_INTEGER;
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{
		
		if(m_pChild[i] == pChild)
		{
			
			pChild->SetParent(NULL);
			m_pChild.Erase(i);
			return i;
		}
	}

	return VSMAX_INTEGER;
}
bool VSNode::DeleteChild(unsigned int i)
{
	if(i >= m_pChild.GetNum())
		return 0;
	VSSpatial * Temp = m_pChild[i];
	
	Temp->SetParent(NULL);
	m_pChild.Erase(i);


	return 1;
}

VSSpatial * VSNode::GetChild(unsigned int i)const
{
	if(i >= (unsigned int)m_pChild.GetNum())
		return NULL;
	return m_pChild[i];
}

void VSNode::DeleteAllChild()
{
	
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{
		m_pChild[i]->SetParent(NULL);
	}
	m_pChild.Clear();
}
void VSNode::CreateLocalAABB()
{
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++) 
	{ 	
		if(m_pChild[i])
		{
			m_pChild[i]->CreateLocalAABB();
		}

	} 

}
void VSNode::UpdateWorldBound(double dAppTime)
{
	bool bFoundFirstBound = false; 
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++) 
	{ 	
		if(m_pChild[i])
		{
			if(!bFoundFirstBound)
			{
				m_WorldBV = m_pChild[i]->m_WorldBV;
				bFoundFirstBound = true; 
			}
			else
			{

				m_WorldBV = m_WorldBV.MergAABB(m_pChild[i]->m_WorldBV);
			}
		}

	} 
	if (!bFoundFirstBound)
	{
		VSREAL fA[3];
		m_WorldBV.GetfA(fA);
		m_WorldBV.Set(GetWorldTranslate(), fA);
	}
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;
	}
	
}
void VSNode::UpdateNodeAll(double dAppTime)
{
	
	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}
	

	UpdateTransform(dAppTime);
		

	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)	
	{ 
		if (m_pChild[i]) 
			m_pChild[i]->UpdateNodeAll(dAppTime); 
	} 


	if(m_bIsChanged)
	{
		UpdateWorldBound(dAppTime); 
	}
	m_bIsChanged = false;
	
}
void VSNode::ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{

	UpDateView(Culler,dAppTime);

	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		if (m_pChild[i])
		{

			m_pChild[i]->ComputeVisibleSet(Culler, bNoCull, dAppTime);

		}

	}
}

void VSNode::SetIsVisibleUpdate(bool bIsVisibleUpdate)
{
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{

		m_pChild[i]->SetIsVisibleUpdate(bIsVisibleUpdate);

	}
	m_bIsVisibleUpdate = bIsVisibleUpdate;
	if (!m_bIsVisibleUpdate)
	{
		m_bEnable = true;
	}
}
