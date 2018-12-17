#include "VSGeometryNode.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSGeometryNode,VSNode)
BEGIN_ADD_PROPERTY(VSGeometryNode,VSNode)
REGISTER_PROPERTY(m_pMorphSet, MorphSet, VSProperty::F_SAVE_LOAD_COPY | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSGeometryNode)
IMPLEMENT_INITIAL_END
VSGeometryNode::VSGeometryNode()
{
}
VSGeometryNode::~VSGeometryNode()
{
	if (m_pMorphSet)
	{
		m_pMorphSet->m_AddMorphEvent.RemoveMethod<VSGeometryNode, &VSGeometryNode::UpdateLocalAABB>(&(*this));
	}
}

VSGeometry * VSGeometryNode::GetGeometry(unsigned int i)
{
	if (i >= m_pChild.GetNum())
	{
		return NULL;
	}
	else
	{
		VSGeometry * pGeometry= DynamicCast<VSGeometry>(m_pChild[i]);
		return pGeometry;
	}
}
unsigned int VSGeometryNode::GetNormalGeometryNum()
{
	unsigned int k = 0;

	for (unsigned int i =0 ; i < m_pChild.GetNum() ;i++)
	{
		VSGeometry * pGeometry= DynamicCast<VSGeometry>(m_pChild[i]);
		if (pGeometry && pGeometry->GetGeometryUseType() == VSGeometry::GUT_NORMAL)
		{
			
			k++;
			

		}

	}
	return k ;
}
VSGeometry * VSGeometryNode::GetNormalGeometry(unsigned int index)
{
	unsigned int k = 0;
	for (unsigned int i =0 ; i < m_pChild.GetNum() ;i++)
	{
		VSGeometry * pGeometry= DynamicCast<VSGeometry>(m_pChild[i]);
		if (pGeometry && pGeometry->GetGeometryUseType() == VSGeometry::GUT_NORMAL)
		{
			if (k == index)
			{
				return pGeometry;
			}
			else
			{
				k++;
			}
			
		}
		
	}
	return NULL;
}
void VSGeometryNode::SetMorphSet(VSMorphSet * pMorphSet)
{
	if (!pMorphSet)
	{
		m_pMorphSet = NULL;
		return;
	}
	else
	{
		if (GetNormalGeometryNum() != pMorphSet->GetBufferNum())
		{
			return;
		}
		for (unsigned int i = 0; i < pMorphSet->GetMorphNum(); i++)
		{
			for (unsigned int j = 0; j < pMorphSet->GetBufferNum(); j++)
			{
				unsigned int VertexNum = pMorphSet->GetMorph(i)->GetVertexNum(j);
				VSGeometry * pGeometry = (VSGeometry *)GetNormalGeometry(j);
				if (VertexNum && pGeometry)
				{
					if (VertexNum != pGeometry->GetVertexNum())
					{
						return;
					}
				}
			}

		}

	}

	if (m_pMorphSet)
	{
		m_pMorphSet->m_AddMorphEvent.RemoveMethod<VSGeometryNode, &VSGeometryNode::UpdateLocalAABB>(&(*this));
	}
	m_pMorphSet = pMorphSet;
	m_pMorphSet->m_AddMorphEvent.AddMethod<VSGeometryNode, &VSGeometryNode::UpdateLocalAABB>(&(*this));
	UpdateLocalAABB();
}
void VSGeometryNode::UpdateLocalAABB()
{
	if (!m_pMorphSet)
	{
		return;
	}
	for (unsigned int i = 0; i < m_pMorphSet->GetMorphNum();i++)
	{
		VSMorph *  pMorph = m_pMorphSet->GetMorph(i);
		for (unsigned int j = 0; j < GetNormalGeometryNum(); j++)
		{
			VSGeometry * NormalGeometry = GetNormalGeometry(j);
			NormalGeometry->AddMorphAABB(pMorph->GetBuffer(j));
		}
	}
}
bool VSGeometryNode::PostLoad(void * pDate)
{
	if (VSNode::PostLoad(pDate) == false)
	{
		return false;
	}
	if (!m_pMorphSet)
	{
		return true;
	}
	else
	{
		for (unsigned int j = 0; j < GetNormalGeometryNum(); j++)
		{
			VSGeometry * NormalGeometry = GetNormalGeometry(j);
			NormalGeometry->CreateMorphMeshData();
		}
		return true;
	}
}
bool VSGeometryNode::PostClone(VSObject * pObjectSrc)
{
	if (VSNode::PostClone(pObjectSrc) == false)
	{
		return false;
	}
	return true;
}