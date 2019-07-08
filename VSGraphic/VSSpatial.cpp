#include "VSSpatial.h"
#include "VSController.h"
#include "VSRenderer.h"
#include "VSGeometry.h"
#include "VSNode.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSMaterial.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSSpatial,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSSpatial)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSSpatial, VSObject)
REGISTER_PROPERTY(m_WorldBV,WorldBV,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiCullMode,CullMode,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_World, WorldTransform, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_Local, LocalTransform, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_pParent, Parent, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_bInheritScale, InheritScale, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_bInheritRotate, InheritRotate, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_bInheritTranlate, InheritTranlate, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_bEnable, Enable, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_ControllerArray, ControllerArray, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME );
REGISTER_PROPERTY(m_bIsStatic, IsStatic, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_bIsChanged, IsChanged, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSSpatial::VSSpatial()
{
	

	m_uiCullMode = CM_DYNAMIC;

	m_bIsVisibleUpdate = false;

	//m_bForceNoCull = false;
	m_pParent = NULL;
	m_bInheritScale = true;
	m_bInheritRotate = true;
	m_bInheritTranlate = true;
	m_ControllerArray.Clear();
	m_bIsStatic = 1;
	m_bIsChanged = true;
	m_bEnable = true;
}
VSSpatial::~VSSpatial()
{
	m_pParent = NULL;

	DeleteAllController();
}

void VSSpatial::UpdateAll(double dAppTime)
{
	UpdateNodeAll(dAppTime);

	if (m_bIsVisibleUpdate)
	{
		m_bEnable = false;
	}

}
void VSSpatial::ComputeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{
	if (!m_bIsVisibleUpdate)
	{
		m_bEnable = true;
	}
	if(m_uiCullMode == CM_ALAWAYS)
		return ;
	if(m_uiCullMode == CM_NEVER)
		bNoCull = true;

	unsigned int uiSavePlaneState = Culler.GetPlaneState();
	if(bNoCull)
	{
		ComputeNodeVisibleSet(Culler,bNoCull,dAppTime);
		if (m_bIsVisibleUpdate)
		{
			m_bEnable = true;
		}
	}
	else
	{
		unsigned int uiVSF = Culler.IsVisible(m_WorldBV);
		if (uiVSF == VSCuller::VSF_ALL)
		{
			bNoCull = true;
			ComputeNodeVisibleSet(Culler,bNoCull,dAppTime);
			if (m_bIsVisibleUpdate)
			{
				m_bEnable = true;
			}
		}
		else if (uiVSF == VSCuller::VSF_PARTIAL || Culler.ForceNoCull(this))
		{
			ComputeNodeVisibleSet(Culler,bNoCull,dAppTime);	
			if (m_bIsVisibleUpdate)
			{
				m_bEnable = true;
			}
		}

	}
	Culler.SetPlaneState(uiSavePlaneState);

}
void VSSpatial::UpDateView(VSCuller & Culler,double dAppTime)
{
}
void VSSpatial::SetIsVisibleUpdate(bool bIsVisibleUpdate)
{
	m_bIsVisibleUpdate = bIsVisibleUpdate;
	if (!m_bIsVisibleUpdate)
	{
		m_bEnable = true;
	}
}
const VSAABB3 & VSSpatial::GetWorldAABB()const
{
	return m_WorldBV;

}
const VSVector3 & VSSpatial::GetLocalScale()const
{
	return m_Local.GetScale();
}
const VSVector3 & VSSpatial::GetLocalTranslate()const
{
	return m_Local.GetTranslate();
}
const VSMatrix3X3 & VSSpatial::GetLocalRotate()const
{
	return m_Local.GetRotate();
}

const VSTransform & VSSpatial::GetLocalTransform()
{
	m_Local.GetCombine();
	return m_Local;
}
VSSpatial *VSSpatial::GetParent()const
{
	return m_pParent;

}
void VSSpatial::SetParent(VSSpatial * pParent)
{

	m_pParent = pParent;
	m_bIsChanged = true;
}

void VSSpatial::UpdateTransform(double dAppTime)
{
	if (m_pParent)
	{
		if (!m_pParent->m_bIsStatic)
		{
			m_bIsStatic = 0;
		}
		if (m_pParent->m_bIsChanged)
		{
			m_bIsChanged = true;
		}
	}
	if (m_bIsChanged)
	{
		if (m_pParent)
		{
			unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
			m_World.Product(m_Local, m_pParent->m_World, TransFormFlag);

		}
		else
			m_World = m_Local;
	}
	


	/*updateNodeController*/

}

void VSSpatial::GetLocalDir(VSVector3 &Dir, VSVector3 &Up, VSVector3 & Right)const
{
	m_Local.GetDir(Dir, Up, Right);
}
void VSSpatial::GetWorldDir(VSVector3 &Dir, VSVector3 &Up, VSVector3 & Right)
{
	m_World.GetDir(Dir, Up, Right);
}
const VSVector3 & VSSpatial::GetWorldScale()
{
	return m_World.GetScale();
}
const VSVector3 & VSSpatial::GetWorldTranslate()
{
	return m_World.GetTranslate();
}
const VSMatrix3X3 & VSSpatial::GetWorldRotate()
{
	return m_World.GetRotate();
}
const VSTransform & VSSpatial::GetWorldTransform()
{
	m_World.GetCombine();
	return m_World;
}
void VSSpatial::SetLocalTransform(const VSTransform & LocalTransform)
{
	m_bIsChanged = true;
	m_Local = LocalTransform;
	UpdateAll(0.0f);
}
void VSSpatial::SetLocalMat(const VSMatrix3X3W VSMat)
{
	m_bIsChanged = true;
	m_Local.SetMatrix(VSMat);
	UpdateAll(0.0f);
}
void VSSpatial::SetLocalScale(const VSVector3 & fScale)
{
	m_bIsChanged = true;
	m_Local.SetScale(fScale);
	UpdateAll(0.0f);

}
void VSSpatial::SetLocalTranslate(const VSVector3& Translate)
{
	m_bIsChanged = true;
	m_Local.SetTranslate(Translate);
	UpdateAll(0.0f);

}
void VSSpatial::SetLocalRotate(const VSMatrix3X3 & Rotate)
{
	m_bIsChanged = true;
	m_Local.SetRotate(Rotate);
	UpdateAll(0.0f);

}
void VSSpatial::SetWorldScale(const VSVector3 & fScale)
{
	VSSpatial * pParent = GetParent();
	if (pParent)
	{
		VSTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		VSTransform NewWorld;
		NewWorld = GetWorldTransform();
		NewWorld.SetScale(fScale);
		VSTransform NewLocal;
		unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
		NewLocal.Product(NewWorld, Inv, TransFormFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalScale(fScale);
	}
}
void VSSpatial::SetWorldTranslate(const VSVector3& Translate)
{
	m_bIsChanged = true;

	VSSpatial * pParent = GetParent();
	if (pParent)
	{
		VSTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		VSTransform NewWorld;
		NewWorld = GetWorldTransform();
		NewWorld.SetTranslate(Translate);
		VSTransform NewLocal;
		unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
		NewLocal.Product(NewWorld, Inv, TransFormFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalTranslate(Translate);
	}
}
void VSSpatial::SetWorldRotate(const VSMatrix3X3 & Rotate)
{
	VSSpatial * pParent = GetParent();
	if (pParent)
	{
		VSTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		VSTransform NewWorld;
		NewWorld = GetWorldTransform();
		NewWorld.SetRotate(Rotate);
		VSTransform NewLocal;
		unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
		NewLocal.Product(NewWorld, Inv, TransFormFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalRotate(Rotate);
	}
}
void VSSpatial::SetWorldTransform(const VSTransform & LocalTransform)
{
	VSSpatial * pParent = GetParent();
	if (pParent)
	{
		VSTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		VSTransform NewWorld;
		NewWorld = LocalTransform;

		VSTransform NewLocal;
		unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
		NewLocal.Product(NewWorld, Inv, TransFormFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalTransform(LocalTransform);
	}
}
void VSSpatial::SetWorldMat(const VSMatrix3X3W VSMat)
{
	VSSpatial * pParent = GetParent();
	if (pParent)
	{
		VSTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);
		VSTransform NewWorld;
		NewWorld.SetMatrix(VSMat);
		VSTransform NewLocal;
		unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
		NewLocal.Product(NewWorld, Inv, TransFormFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalMat(VSMat);
	}
}
VSArray<VSPointer<VSController>> * VSSpatial::GetControllerList()
{
	return &m_ControllerArray;
}
void VSSpatial::SetDynamic(bool bIsDynamic)
{
	if (!m_ControllerArray.GetNum())
	{
		m_bIsStatic = !bIsDynamic;
	}
}
bool VSSpatial::AddController(VSController * pController)
{
	if (!pController)
		return false;
	for (unsigned int i = 0; i < m_ControllerArray.GetNum(); i++)
	{
		if (m_ControllerArray[i] == pController)
			return 1;
	}
	if (!pController->SetObject(this))
		return false;
	m_ControllerArray.AddElement(pController);

	m_bIsStatic = 0;
	return 1;
}

VSController * VSSpatial::DeleteController(unsigned int ID)
{
	if (ID >= m_ControllerArray.GetNum())
		return NULL;
	VSController * Temp = m_ControllerArray[ID];
	m_ControllerArray.Erase(ID);

	Temp->SetObject(NULL);
	return Temp;
}

unsigned int VSSpatial::DeleteController(VSController *pController)
{
	if (!pController)
		return VSMAX_INTEGER;
	for (unsigned int i = 0; i < m_ControllerArray.GetNum(); i++)
	{

		if (m_ControllerArray[i] == pController)
		{
			pController->SetObject(NULL);
			m_ControllerArray.Erase(i);

			return i;
		}
	}

	return VSMAX_INTEGER;
}

void VSSpatial::DeleteAllController()
{
	m_ControllerArray.Clear();
}

VSController *VSSpatial::GetController(unsigned int ID)const
{
	if (ID >= m_ControllerArray.GetNum())
		return NULL;
	return m_ControllerArray[ID];
}

void VSSpatial::UpdateController(double dAppTime)
{

	for (unsigned int i = 0; i < m_ControllerArray.GetNum(); i++)
	{
		if (m_ControllerArray[i]->m_bIsVisibleUpdate)
		{
			if (!m_bEnable)
			{
				continue;
			}
		}
		m_ControllerArray[i]->SetObject(this);
		m_ControllerArray[i]->Update(dAppTime);

	}

}

void VSSpatial::ClearAllTime()
{
	for (unsigned int i = 0; i < m_ControllerArray.GetNum(); i++)
	{
		m_ControllerArray[i]->ClearTime();
	}
}
