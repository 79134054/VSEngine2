#include "VSObject.h"
#include "VSStream.h"
#include "VSProperty.h"
#include "VSGraphicInclude.h"
#include "VSProfiler.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoParent_NoCreateFun(VSObject)
VSMapOrder<VSUsedName,FactoryFunction> VSObject::ms_ClassFactory;
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY_ROOT(VSObject)
REGISTER_PROPERTY(m_uiFlag, Flag,VSProperty::F_CLONE);
END_ADD_PROPERTY
DECLEAR_NOCLEAR_COUNT_PROFILENODE(ObjectNum,)
VSFastObjectManager::VSFastObjectManager()
{
	m_FreeTable.SetBufferNum(MAX_OBJECT_NUM);
	for (unsigned int i = 0; i < MAX_OBJECT_NUM; i++)
	{
		m_FreeTable[i] = i;
		m_ObjectArray[i] = NULL;
	}
	m_ObjectArray[MAX_OBJECT_NUM] = NULL;
}
VSFastObjectManager::~VSFastObjectManager()
{

}
unsigned int VSFastObjectManager::AddObject(VSObject * p)
{
	VSMAC_ASSERT(m_FreeTable.GetNum() > 0);
	unsigned int ID = m_FreeTable[m_FreeTable.GetNum() - 1];
	m_ObjectArray[ID] = p;
	m_FreeTable.Erase(m_FreeTable.GetNum() - 1);
	ADD_COUNT_PROFILE(ObjectNum, 1)
	return ID;
}
void VSFastObjectManager::DeleteObject(VSObject * p)
{
	if (m_ObjectArray[p->m_uiObjectID] != NULL)
	{
		m_FreeTable.AddElement(p->m_uiObjectID);
		m_ObjectArray[p->m_uiObjectID] = NULL;
		ADD_COUNT_PROFILE(ObjectNum, -1)
		p->m_uiObjectID = MAX_OBJECT_FLAG;
	}
	
}
bool VSFastObjectManager::IsClear()
{
	return m_FreeTable.GetNum() == MAX_OBJECT_NUM;
}
unsigned int VSFastObjectManager::GetObjectNum()
{
	return MAX_OBJECT_NUM - m_FreeTable.GetNum();
}
void VSFastObjectManager::PrepareForGC()
{
	for (unsigned int i = 0; i < MAX_OBJECT_NUM; i++)
	{
		VSObject * p = m_ObjectArray[i];
		if (p)
		{
			p->ClearFlag(VSObject::OF_REACH);
			p->SetFlag(VSObject::OF_UNREACH);
		}
	}
}
VSObject* VSObject::CloneCreateObject(VSObject * pObject)
{
	VSMap<VSObject *,VSObject*> CloneMap; 
	VSObject * pNewObject =  _CloneCreateObject(pObject,CloneMap);
	for (unsigned int i = 0 ; i < CloneMap.GetNum() ;i++)
	{
		CloneMap[i].Value->PostClone(CloneMap[i].Key);
	}
	return pNewObject;
}
void VSObject::CloneObject(VSObject * pObjectSrc,VSObject * pObjectDest)
{
	VSMap<VSObject *,VSObject*> CloneMap; 
	_CloneObject(pObjectSrc,pObjectDest,CloneMap);
	for (unsigned int i = 0 ; i < CloneMap.GetNum() ;i++)
	{
		CloneMap[i].Value->PostClone(CloneMap[i].Key);
	}
}
VSObject* VSObject::_CloneCreateObject(VSObject * pObject,VSMap<VSObject *,VSObject*>& CloneMap)
{
	VSObject* pNewObject = NULL;
	if (pObject)
	{
		unsigned int uiIndex = CloneMap.Find(pObject);

		if (uiIndex == CloneMap.GetNum())
		{
			VSRtti & SrcRtti = pObject->GetType();
			if (pObject->IsHasFlag(OF_GCObject))
			{
				pNewObject = VSObject::GetInstance(SrcRtti.GetName());
			}
			else
			{
				pNewObject = VSObject::GetNoGCInstance(SrcRtti.GetName());
			}
			CloneMap.AddElement(pObject,pNewObject);
			VSRtti & DestRtti = pNewObject->GetType();

			for (unsigned int i = 0 ; i < SrcRtti.GetPropertyNum() ; i++)
			{
				VSProperty * pProperty = SrcRtti.GetProperty(i);
				if (pProperty->GetFlag() & VSProperty::F_CLONE)
				{
					pProperty->CloneData(pObject,pNewObject,CloneMap);
				}
				else if(pProperty->GetFlag() & VSProperty::F_COPY)
				{
					pProperty->CopyData(pObject,pNewObject);
				}

			}
		}
		else
		{
			pNewObject = CloneMap[uiIndex].Value;
		}
	}
	return pNewObject;
}
void VSObject::_CloneObject(VSObject * pObjectSrc,VSObject * pObjectDest,VSMap<VSObject *,VSObject*>& CloneMap)
{
	if (!pObjectSrc)
	{
		return ;
	}
	unsigned int uiIndex = CloneMap.Find(pObjectSrc);

	if (uiIndex == CloneMap.GetNum())
	{
		VSRtti & SrcRtti = pObjectSrc->GetType();
		VSRtti & DestRtti = pObjectDest->GetType();
		for (unsigned int i = 0 ; i < SrcRtti.GetPropertyNum() ; i++)
		{
			VSProperty * pProperty = SrcRtti.GetProperty(i);
			if (pProperty->GetFlag() & VSProperty::F_CLONE)
			{
				pProperty->CloneData(pObjectSrc,pObjectDest,CloneMap);
			}
			else if(pProperty->GetFlag() & VSProperty::F_COPY)
			{
				pProperty->CopyData(pObjectSrc,pObjectDest);
			}
		}
	}
	else
	{
		VSMAC_ASSERT(0);
	}
}
/********************************************************************************/
VSObject * VSObject::GetNoGCInstance(const VSString& sRttiName)
{
	unsigned int i = ms_ClassFactory.Find(sRttiName);
	if (i == ms_ClassFactory.GetNum())
		return NULL;

	VSObject * pObject = ms_ClassFactory[i].Value();
	return pObject;

}
VSObject * VSObject::GetInstance(const VSString& sRttiName)
{
	unsigned int i = ms_ClassFactory.Find(sRttiName);
	if(i == ms_ClassFactory.GetNum())
		return NULL;

	VSObject * pObject = ms_ClassFactory[i].Value();
	VSResourceManager::AddGCObject(pObject);
	return pObject;

}
VSObject * VSObject::GetInstance(const VSRtti& Rtti)
{
	VSObject * pObject = (Rtti.m_CreateFun)();
	VSResourceManager::AddGCObject(pObject);
	return pObject;
}
/********************************************************************************/
VSObject::VSObject()
{
	m_uiFlag = 0;
	m_uiObjectID = GetObjectManager().AddObject(this);
	


}//end construct
VSObject::VSObject(const VSObject & object)
{
	m_uiFlag = object.m_uiFlag;
	m_uiObjectID = GetObjectManager().AddObject(this);
}
VSObject & VSObject::operator =(const VSObject &object)
{
	m_uiFlag = object.m_uiFlag;
	return *this;
}
/********************************************************************************/

VSObject::~VSObject()
{
	GetObjectManager().DeleteObject(this);
}//end destruct


bool VSObject::IsSameType(const VSObject *pObject)const
{

	return pObject && GetType().IsSameType(pObject->GetType());

}//end IsSameType

/********************************************************************************/

bool VSObject::IsDerived(const VSObject *pObject)const
{
	return pObject && GetType().IsDerived(pObject->GetType());
}//IsDerived

/********************************************************************************/

bool VSObject::IsSameType(const VSRtti &Type)const
{
	
	return GetType().IsSameType(Type);
}//IsSameType

/********************************************************************************/

bool VSObject::IsDerived(const VSRtti &Type)const
{
	return GetType().IsDerived(Type);
	
}//IsDerived

void VSObject::LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data)
{

}

bool VSObject::BeforeSave( void * pDate)
{
	return 1;
}
bool VSObject::PostSave( void * pDate)
{
	return 1;
}
bool VSObject::PostLoad( void * pDate)
{
	return 1;
}
bool VSObject::PostClone(VSObject * pObjectSrc)
{
	return 1;
}
void VSObject::ValueChange(VSString & Name)
{

}
void VSObject::DelayUpdate()
{

}
bool VSObject::Process(VSUsedName & FunName, void * para, void *ret, int ParaNum)
{
	VSRtti & t = GetType();
	for (unsigned int i = 0; i < t.GetFunctionNum(); i++)
	{
		VSFunction * p = t.GetFunction(i);
		if (p->GetName() == FunName)
		{
			if (ParaNum == -1)
			{
				p->ObjectFun(this, p, para, ret);
				return true;
			}
			else if (p->GetPropertyNum() == ParaNum)
			{
				p->ObjectFun(this, p, para, ret);
				return true;
			}
			
		}
	}
	return false;
}