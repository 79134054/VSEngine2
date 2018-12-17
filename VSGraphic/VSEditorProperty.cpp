#include "VSEditorProperty.h"
#include "VSProperty.h"
using namespace VSEngine2;
void VSECheckBox::CallBackValue(bool Value)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&Value);
	}
}
void VSELable::CallBackValue(VSString & Str)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&Str);
	}
}
void VSECombo::CallBackValue(VSString & Str)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&Str);
	}
}
void VSESlider::CallBackValue(unsigned int uiValue)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&uiValue);
	}
}
void VSEText::CallBackValue(VSString & Str)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&Str);
	}
}
void VSEColorTable::CallBackValue(VSColorRGBA & Color)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&Color);
	}
}
VSEditorUIPropertyCreator::VSEditorUIPropertyCreator()
{
	m_pCreate = NULL;
}
VSEditorUIPropertyCreator::~VSEditorUIPropertyCreator()
{

}
VSEditorUIPropertyCreator& VSEditorUIPropertyCreator::GetInstance()
{
	static VSEditorUIPropertyCreator EditorUICreator;
	return EditorUICreator;
}

void VSEditorUIPropertyCreator::Set(CreateEditorUIProperty pCreate)
{
	m_pCreate = pCreate;
}
VSEditorSingle * VSEditorUIPropertyCreator::CreateUIProperty(VSEditorSingle::CUSTOM_UI_TYPE type,VSString Name)
{
	return (*m_pCreate)(type,Name);
}
bool VSEditorProperty::CallBackValue(VSEditorSingle * pElem, void * pValue)
{
	if (pElem)
	{
		m_pOwner->ValueChange(pElem->GetName());
		return true;
	}
	return false;
	
}
namespace VSEngine2
{
VSEditorElement * CreateObjectEditorElement(VSObject * pObject,VSString & Name)
{
	VSRtti & Rtti = pObject->GetType();
	VSECollection * pParent = NULL;
	if (Rtti.GetPropertyNum() > 0)
	{
		pParent = (VSECollection *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_COLLECTION,Name);
	}
	for (unsigned int i = 0 ; i < Rtti.GetPropertyNum() ; i++)
	{
		VSProperty * pProperty = Rtti.GetProperty(i);
		if (pProperty->GetFlag() & VSProperty::F_REFLECT_NAME)
		{
			VSString Name = pProperty->GetName().GetString();
			pProperty->AddEditorElement(pObject,pParent,Name);
		}

	}
	return pParent;
}
void  ReleaseObjectEditorElement(VSECollection * Root)
{
	for (unsigned int i = 0; i < Root->ChildElement.GetNum(); i++)
	{
		if (Root->ChildElement[i]->IsCollection())
		{
			ReleaseObjectEditorElement((VSECollection*)Root->ChildElement[i]);
		}
		else
		{
			VSMAC_DELETE(Root->ChildElement[i]);
		}
	}
	VSMAC_DELETE(Root);
}
}