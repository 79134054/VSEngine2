#include "VSSkelectonMeshNode.h"
#include "VSBoneNode.h"
#include "VSAnimTree.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSSkelectonMeshNode,VSModelMeshNode)
BEGIN_ADD_PROPERTY(VSSkelectonMeshNode,VSModelMeshNode)
REGISTER_PROPERTY(m_pSkelecton,Skelecton,VSProperty::F_SAVE_LOAD_CLONE| VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pAnimSet, AnimSet, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pAnimTree, pAnimTree, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pSocketArray, SocketArray, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSkelectonMeshNode)
ADD_PRIORITY(VSGeometry)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSSkelectonMeshNodePtr VSSkelectonMeshNode::Default = NULL;
bool VSSkelectonMeshNode::ms_bIsEnableASYNLoader = true;
bool VSSkelectonMeshNode::ms_bIsEnableGC = true;
VSSkelectonMeshNode::VSSkelectonMeshNode()
{
	m_pSkelecton = NULL;
	m_pAnimSet = NULL;
	m_pAnimSequence = NULL;

	m_pAnimTree = NULL;
	m_pAnimTreeInstance = NULL;
	
}
VSSkelectonMeshNode::~VSSkelectonMeshNode()
{
	if (m_pAnimSet)
	{
		m_pAnimSet->m_AddAnimEvent.RemoveMethod<VSSkelectonMeshNode, &VSSkelectonMeshNode::UpdateLocalAABB>(&(*this));
	}
	m_pSkelecton = NULL;
	m_pAnimSet = NULL;
	m_pAnimSequence = NULL;
	m_pAnimTree = NULL;
	m_pAnimTreeInstance = NULL;

}
bool VSSkelectonMeshNode::InitialDefaultState()
{
	Default = VS_NEW VSSkelectonMeshNode();
	VSGeometryNodePtr GeometryNode = VS_NEW VSGeometryNode();
	Default->AddChild(GeometryNode);
	VSGeometryPtr Geometry = (VSGeometry *)VSObject::CloneCreateObject(VSGeometry::GetDefaultRenderCube());
	GeometryNode->AddChild(Geometry);
	Default->CreateLocalAABB();
	GeometryNode->SetLocalScale(VSVector3(100.0f, 100.0f, 100.0f));

	return true;
}
bool VSSkelectonMeshNode::TerminalDefaultState()
{
	Default = NULL;
	return true;
}
void VSSkelectonMeshNode::SetSkelecton(VSSkelecton * pSkelecton)
{	
	if(m_pSkelecton)
	{
		m_pSkelecton->m_pParent = NULL;
	}
	m_pSkelecton = pSkelecton;
	m_pSkelecton->m_pParent = this;
	
}
VSSocketNode * VSSkelectonMeshNode::GetSocket(const VSUsedName &SocketName)
{
	for (unsigned int i = 0; i < m_pSocketArray.GetNum(); i++)
	{
		if (m_pSocketArray[i]->m_cName == SocketName)
		{
			return m_pSocketArray[i];
		}
	}
	return NULL;
}
VSSocketNode * VSSkelectonMeshNode::CreateSocket(const VSUsedName & BoneName, const VSUsedName &SocketName)
{
	if (!m_pSkelecton)
	{
		return NULL;
	}
	VSBoneNode * pBone = m_pSkelecton->GetBoneNode(BoneName);
	if (!pBone)
	{
		return NULL;
	}

	for (unsigned int i = 0; i < m_pSocketArray.GetNum(); i++)
	{
		if (m_pSocketArray[i]->m_cName == SocketName)
		{
			return NULL;
		}
	}

	VSSocketNode * pSocketNode = VS_NEW VSSocketNode();
	pBone->AddChild(pSocketNode);
	pSocketNode->m_cName = SocketName;
	m_pSocketArray.AddElement(pSocketNode);
	return pSocketNode;
}
void VSSkelectonMeshNode::DeleteSocket(const VSUsedName &SocketName)
{
	for (unsigned int i = 0; i < m_pSocketArray.GetNum(); i++)
	{
		if (m_pSocketArray[i]->m_cName == SocketName)
		{
			VSBoneNode * pSocketParent = (VSBoneNode *)(m_pSocketArray[i]->GetParent());
			VSMAC_ASSERT(pSocketParent);
			pSocketParent->DeleteChild(m_pSocketArray[i]);
			m_pSocketArray.Erase(i);
			return;
		}
	}
}
void VSSkelectonMeshNode::SetAnimSet(VSAnimSet * pAnimSet)
{
	if (m_pAnimSet == pAnimSet)
	{
		return;
	}
	if (m_pAnimSet)
	{
		m_pAnimSet->m_AddAnimEvent.RemoveMethod<VSSkelectonMeshNode, &VSSkelectonMeshNode::UpdateLocalAABB>(&(*this));
	}
	m_pAnimSet = pAnimSet;
	m_pAnimSet->m_AddAnimEvent.AddMethod<VSSkelectonMeshNode, &VSSkelectonMeshNode::UpdateLocalAABB>(&(*this));
	if (m_pAnimTreeInstance)
	{
		m_pAnimTreeInstance->ResetAnimFunction();
	}
	UpdateLocalAABB();
}
void VSSkelectonMeshNode::UpdateLocalAABB()
{
	if (m_pAnimSet && m_pSkelecton)
	{
		VSArray<VSMatrix3X3W> SaveBoneMatrix;
		SaveBoneMatrix.AddBufferNum(m_pSkelecton->GetBoneNum());
		for (unsigned int i = 0; i < m_pSkelecton->GetBoneNum(); i++)
		{
			VSBoneNode * pBone = m_pSkelecton->GetBoneNode(i);
			if (pBone)
			{
				VSTransform T = pBone->GetLocalTransform();
				SaveBoneMatrix[i] = T.GetCombine();
			}
		}

		
		VSVector3 MaxPos = m_pSkelecton->m_OriginLocalBV.GetMaxPoint();
		VSVector3 MinPos = m_pSkelecton->m_OriginLocalBV.GetMinPoint();
		VSTransform SkelectonLocalT = m_pSkelecton->GetLocalTransform();
		for (unsigned int i = 0; i < m_pAnimSet->GetAnimNum(); i++)
		{
			VSAnimR * pAnimR = m_pAnimSet->GetAnim(i);
			while (!pAnimR->IsLoaded()){};

			VSAnim * pAnim = pAnimR->GetResource();
			VSREAL AnimLength = pAnim->GetAnimLength();

			PlayAnim(pAnim->m_cName.GetString(), 1.0f, VSController::RT_CLAMP);

			for (VSREAL f = 0.0f; f < AnimLength + 0.05f; f += 0.05f)
			{
				m_pAnimSequence->Update(f);
				m_pSkelecton->UpdateAll(0.0f);
				for (unsigned int j = 0; j < m_pSkelecton->GetBoneNum(); j++)
				{
					VSBoneNode * pBone = m_pSkelecton->GetBoneNode(j);
					if (pBone)
					{
						//change to local space
						VSVector3 Pos = pBone->GetWorldTranslate() * SkelectonLocalT.GetCombineInverse();
						for (int t = 0; t < 3; t++)
						{
							if (MinPos.m[t] > Pos.m[t])
							{
								MinPos.m[t] = Pos.m[t];
							}

							if (MaxPos.m[t] < Pos.m[t])
							{
								MaxPos.m[t] = Pos.m[t];
							}
						}

					}
				}
				
			}
		}
		m_pAnimSequence = NULL;
		for (unsigned int i = 0; i < m_pSkelecton->GetBoneNum(); i++)
		{
			VSBoneNode * pBone = m_pSkelecton->GetBoneNode(i);
			if (pBone)
			{
				pBone->SetLocalMat(SaveBoneMatrix[i]);
			}
		}
		VSAABB3 SkelectonBV;
		SkelectonBV.Set(MaxPos, MinPos);
		m_pSkelecton->SetLocalBV(SkelectonBV);
	}
}
void VSSkelectonMeshNode::SetAnimTree(VSAnimTreeR * pAnimTree)
{
	if(pAnimTree)
	{
		m_pAnimTree = pAnimTree;
		m_pAnimTree->AddLoadEventObject(this);
	}
}

void VSSkelectonMeshNode::LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data)
{
	if (m_pAnimTree == pResourceProxy)
	{
		m_pAnimTreeInstance = (VSAnimTree *)VSObject::CloneCreateObject(m_pAnimTree->GetResource());
		m_pAnimTreeInstance->SetObject(this);
	}
}
bool VSSkelectonMeshNode::PostLoad(void * pDate)
{
	VSModelMeshNode::PostLoad(pDate);
	if (m_pAnimTree)
	{
		m_pAnimTree->AddLoadEventObject(this);
	}
	
	return true;
}
bool VSSkelectonMeshNode::PostClone(VSObject * pObjectSrc)
{
	VSModelMeshNode::PostClone(pObjectSrc);
	if (m_pAnimTree)
	{
		m_pAnimTree->AddLoadEventObject(this);
	}
	return true;
}
void VSSkelectonMeshNode::SetAnimTreeNodePara(const VSUsedName & ShowName, void * pPara)
{
	if (m_pAnimTreeInstance)
	{
		m_pAnimTreeInstance->SetNodePara(ShowName, pPara);
	}
}
void VSSkelectonMeshNode::UpdateAll(double dAppTime)
{
	VSModelMeshNode::UpdateAll(dAppTime);
}
void VSSkelectonMeshNode::UpdateNodeAll(double dAppTime)
{

	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}
	
	
	UpdateTransform(dAppTime);

	// no visible no update
	if (m_bEnable && m_pSkelecton)
	{
		m_pSkelecton->UpdateNodeAll(dAppTime);
	}
	else if (m_pSkelecton)
	{
		m_pSkelecton->UpdateNoChild(dAppTime);
	}
	
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
void VSSkelectonMeshNode::ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{

	if (!Culler.CullConditionNode(this))
	{
		UpDateView(Culler,dAppTime);
		for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			if(m_pChild[i])
			{

				m_pChild[i]->ComputeVisibleSet(Culler,true,dAppTime);


			}

		}
		if (m_pSkelecton)
		{
			m_pSkelecton->ComputeNodeVisibleSet(Culler, bNoCull, dAppTime);
		}
		
	}

}
void VSSkelectonMeshNode::UpdateWorldBound(double dAppTime)
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
	if (m_pSkelecton)
	{
		if(!bFoundFirstBound)
		{
			m_WorldBV = m_pSkelecton->m_WorldBV;
			bFoundFirstBound = true; 
		}
		else
		{
			VSVector3 MaxPos = m_pSkelecton->m_WorldBV.GetMaxPoint();
			VSVector3 MinPos = m_pSkelecton->m_WorldBV.GetMinPoint();

			VSREAL fA[3];
			m_WorldBV.GetfA(fA);

			MaxPos = MaxPos + VSVector3(fA[0], fA[1], fA[2]);
			MinPos = MinPos - VSVector3(fA[0], fA[1], fA[2]);
			m_WorldBV.Set(MaxPos, MinPos);
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
bool VSSkelectonMeshNode::PlayAnim(const VSString & AnimName,VSREAL fRatio,unsigned int uiRepeatType)
{
	if (m_pAnimSequence == NULL)
	{
		m_pAnimSequence = VS_NEW VSAnimSequenceFunc(this);

		m_bIsStatic = false;

		

	}

	m_pAnimSequence->m_bEnable = true;
	m_pAnimSequence->SetAnim(AnimName);
	m_pAnimSequence->m_uiRepeatType = uiRepeatType;
	m_pAnimSequence->m_dFrequency = fRatio;

	return 1;;
}
void VSSkelectonMeshNode::StopAnim()
{
	if (m_pAnimSequence)
	{
		m_pAnimSequence->m_bEnable = false;
	}
}
void VSSkelectonMeshNode::UpdateController(double dAppTime)
{
	VSModelMeshNode::UpdateController(dAppTime);
	if (m_pAnimTreeInstance && m_bEnable)
	{
		if (m_pAnimSequence)
		{
			m_pAnimSequence->m_bEnable = false;
		}
		m_pAnimTreeInstance->Update(dAppTime);
	}
	if(m_pAnimSequence && m_bEnable && m_pAnimSequence->m_bEnable)
	{
		m_pAnimSequence->ClearFlag();
		m_pAnimSequence->Update(dAppTime);
		m_pAnimSequence->UpDateBone();
	}

}
