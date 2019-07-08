#include "stdafx.h"
#include "g_property.h"
#include "core/io/iosystem.h"

namespace MAY
{
	PropertyValue::PropertyValue()
		: m_type(PVT_Unknown) 
	{
		m_bool = false;
		m_int = 0;
		m_uint = 0;
		m_real = 0;
		m_obj = NULL;
	}

	PropertyValue::PropertyValue(const PropertyValue& inVal)
	{
		*this = inVal;
	}
	PropertyValue& PropertyValue::operator = (const PropertyValue& inVal)
	{
		m_type = inVal.m_type;
		switch (m_type)
		{
		case PVT_Bool:
			m_bool = inVal.m_bool;
			break;
		case PVT_Integer:
			m_int = inVal.m_int;
			break;
		case PVT_UnsignedInteger:
			m_uint = inVal.m_uint;
			break;
		case PVT_Real:
			m_real = inVal.m_real;
			break;
		case PVT_String:
			m_string = inVal.m_string;
			break;
		case PVT_Object:
			m_obj = inVal.m_obj;
			break;
		case PVT_Unknown:
			break;
		default:
			assert(false); //@FIXME: throw exception~
		}
		return *this;
	}
	bool PropertyValue::operator == (const PropertyValue& inVal)const
	{
		if (inVal.m_type != inVal.m_type)
			return false;
		switch (m_type)
		{
		case PVT_Bool:
			return m_bool == inVal.m_bool;
			break;
		case PVT_Integer:
			return m_int == inVal.m_int;
			break;
		case PVT_UnsignedInteger:
			return m_uint == inVal.m_uint;
			break;
		case PVT_Real:
			return m_real == inVal.m_real;
			break;
		case PVT_String:
			return lstrcmp(m_string.AsCharPtr(), inVal.m_string.AsCharPtr()) == 0;
			break;
		case PVT_Object:
			return m_obj == inVal.m_obj;
			break;
		case PVT_Unknown:
			return true;
			break;
		default:
			assert(false); //@FIXME: throw exception~
		}
		return false;
	}
	void PropertyValue::Swap(PropertyValue& outVal)
	{
		PropertyValue temp;
		temp = *this;
		*this = outVal;
		outVal = temp;
	}
	void PropertyValue::Clear()
	{
		m_type = PVT_Unknown;
	}
	PropertyValue::EPropertyValueType PropertyValue::MapPropertyType(const String& inType)
	{
		if (inType.Equals("Bool", false))
		{
			return PVT_Bool;
		}
		else if (inType.Equals("Int", false))
		{
			return PVT_Integer;
		}
		else if (inType.Equals("UInt", false))
		{
			return PVT_UnsignedInteger;
		}
		else if (inType.Equals("Real", false))
		{
			return PVT_Real;
		}
		else if (inType.Equals("String", false))
		{
			return PVT_String;
		}
		else if (inType.Equals("Object", false))
		{
			return PVT_Object;
		}
		return PVT_Unknown;
	}
	String PropertyValue::MapPropertyString(PropertyValue::EPropertyValueType inType)
	{
		switch(inType)
		{
		case PVT_Bool: return "Bool";
		case PVT_Integer: return "Int";
		case PVT_UnsignedInteger: return "UInt";
		case PVT_Real: return "Real";
		case PVT_String: return "String";
		case PVT_Object: return "Object";
		case PVT_Unknown: return "Unknown";
		default:
			assert(false);
		}
		return "";
	}
	bool PropertyValue::GetValueToString(String& outVal)const
	{
		switch (m_type)
		{
		case PVT_Bool:
			outVal.SetBool(m_bool);
			break;
		case PVT_Integer:
			outVal.SetInt(m_int);
			break;
		case PVT_UnsignedInteger:
			outVal.SetUInt(m_uint);
			break;
		case PVT_Real:
			outVal.SetFloat((float)m_real);
			break;
		case PVT_String:
			outVal = m_string;
			break;
		case PVT_Unknown:
			return false;
			break;
		default:
			assert(false); //@FIXME: throw exception~
		}
		return true;
	}
	bool PropertyValue::SetValueFromString(const String& inVal)
	{
		bool bRet = false;
		switch (m_type)
		{
		case PVT_Bool:
			m_bool = inVal.As<bool>(&bRet);
			break;
		case PVT_Integer:
			m_int = inVal.As<int>(&bRet);
			break;
		case PVT_UnsignedInteger:
			m_uint = inVal.As<uint>(&bRet);
			break;
		case PVT_Real:
			m_real = inVal.As<double>(&bRet);
			break;
		case PVT_String:
			m_string = inVal;
			bRet = true;
			break;
		case PVT_Unknown:
			bRet = true;
			break;
		//todo: add object
		//case PVT_Object:
		//	bRet = StringTo<bool>(inVal, m_bool);
		//	break;
		default:
			bRet = false;
		}
		return bRet;
	}

	IPropertyNode::IPropertyNode()
		: m_name(0)
		, m_parent(NULL)
		, m_prevSibling(NULL)
		, m_nextSibling(NULL)
		, m_propertyFlags(0)
		, m_userData(NULL)
	{
	}
	const String& IPropertyNode::GetName()const
	{
		return m_name;
	}
	void IPropertyNode::SetName(const String& inName)
	{
		m_name = inName;
	}
	IPropertyNode* IPropertyNode::GetParent() const
	{
		return m_parent;
	}
	int IPropertyNode::GetAttributeCount()const
	{
		return m_attributes.size();
	}
	bool IPropertyNode::FindAttribute(const String& inName, PropertyAttribute* outVal, bool inCaseSensitive)
	{
		for (eastl::list<PropertyAttribute>::iterator itr = m_attributes.begin(); itr != m_attributes.end(); ++itr)
		{
			if (inName.Equals(itr->m_name, inCaseSensitive))
			{
				if (outVal)
					*outVal = *itr;
				return true;
			}
		}
		return false;
	}
	PropertyAttribute* IPropertyNode::GetAttribute(const String& inName, bool inCaseSensitive)
	{
		for (eastl::list<PropertyAttribute>::iterator itr = m_attributes.begin(); itr != m_attributes.end(); ++itr)
		{
			if (inName.Equals(itr->m_name, inCaseSensitive))
				return &(*itr);
		}
		return NULL;
	}
	bool IPropertyNode::AddAttribute(const TCHAR* inName, const TCHAR* inValue, bool inUpdateIfExist, bool inCaseSensitive)
	{
		PropertyAttribute kAttr(inName, inValue);
		return AddAttribute(kAttr, inUpdateIfExist, inCaseSensitive);
	}
	bool IPropertyNode::AddAttribute(const PropertyAttribute& inAttr, bool inUpdateIfExist, bool inCaseSensitive)
	{
		PropertyAttribute* pAttr = GetAttribute(inAttr.m_name, inCaseSensitive);
		if (pAttr == NULL)
		{
			TryUpdateFlags(inAttr.m_name, true);
			m_attributes.push_back(inAttr);
		}
		else if (inUpdateIfExist)
		{
			TryUpdateFlags(inAttr.m_name, true);
			*pAttr = inAttr;
		}
		else
		{
			return false;
		}
		return true;
	}
	bool IPropertyNode::RemoveAttribute(const String& inName, bool inCaseSensitive)
	{
		for (eastl::list<PropertyAttribute>::iterator itr = m_attributes.begin(); itr != m_attributes.end(); ++itr)
		{
			if (inName.Equals(itr->m_name, inCaseSensitive))
			{
				m_attributes.erase(itr);
				TryUpdateFlags(inName, false);
				return true;
			}
		}
		return false;
	}
	void IPropertyNode::TryUpdateFlags(const String& name, bool inEnable)
	{
		if (name.Equals("Min", false))
		{
			SetFlag(PF_HasMin, inEnable);
		}
		else if (name.Equals("Max", false))
		{
			SetFlag(PF_HasMax, inEnable);
		}
		else if (name.Equals("Step", false))
		{
			SetFlag(PF_HasStep, inEnable);
		}
		else if (name.Equals("Category", false))
		{
			SetFlag(PF_HasCategory, inEnable);
		}
		else if (name.Equals("DefaultValue", false))
		{
			SetFlag(PF_HasDefaultValue, inEnable);
		}
		else if (name.Equals("Description", false))
		{
			SetFlag(PF_HasDescription, inEnable);
		}
		else if (name.Equals("DisplayName", false))
		{
			SetFlag(PF_HasDisplayName, inEnable);
		}
		else if (name.Equals("ReadOnly", false))
		{	
			SetFlag(PF_HasReadOnly, inEnable);
		}
	}
	const PropertyAttribute* IPropertyNode::FirstAttribute()const
	{
		m_iter = m_attributes.begin();
		if (m_iter != m_attributes.end())
			return &(*m_iter);
		return NULL;
	}
	const PropertyAttribute* IPropertyNode::NextAttribute()const
	{
		m_iter++;
		if (m_iter != m_attributes.end())
			return &(*m_iter);
		return NULL;
	}
	uint IPropertyNode::GetPropertyFlags()const
	{
		return m_propertyFlags;
	}
	bool IPropertyNode::HasPropertyFlag(IPropertyNode::EPropertyFlag inFlag)const
	{
		return (m_propertyFlags & inFlag) != 0;
	}
	void IPropertyNode::SetFlag(EPropertyFlag inFlag, bool inEnable)
	{
		if (inEnable)
			m_propertyFlags |= inFlag;
		else
			m_propertyFlags &= ~inFlag;
	}
	IPropertyNode* IPropertyNode::PreviousSibling(const String& name, bool case_sensitive) const
	{
		assert(this->m_parent);   
		if (name.GetLength() == 0)
			return m_prevSibling;
		for (IPropertyNode *sibling = m_prevSibling; sibling; sibling = sibling->m_prevSibling)
			if (sibling->GetName().Equals(name, case_sensitive))
				return sibling;
		return 0;
	}
	IPropertyNode* IPropertyNode::NextSibling(const String& name, bool case_sensitive) const
	{
		assert(this->m_parent);  
		if (name.GetLength() == 0)
			return m_nextSibling;
		for (IPropertyNode *sibling = m_nextSibling; sibling; sibling = sibling->m_nextSibling)
			if (sibling->GetName().Equals(name, case_sensitive))
				return sibling;
		return 0;
	}

	void IPropertyNode::SetUserData(void* inData)
	{
		m_userData = inData;
	}

	void* IPropertyNode::GetUserData()const
	{
		return m_userData;
	}

	PropertyMemoryPool::PropertyMemoryPool()
		: m_allocFunc(PropAlloc)
		, m_freeFunc(PropFree)
	{
		Init();
	}
	PropertyMemoryPool::~PropertyMemoryPool()
	{
		Clear();
	}
	Property *PropertyMemoryPool::AllocateProperty(
		const String& name,
		const PropertyValue& value
		)
	{
		void *memory = AllocateAligned(sizeof(Property));
		Property *node = ::new(memory) Property();
		node->SetName(name);
		node->SetValue(value);
		return node;
	}
	PropertyGroup *PropertyMemoryPool::AllocatePropertyGroup(const String& name)
	{
		void *memory = AllocateAligned(sizeof(PropertyGroup));
		PropertyGroup *node = ::new(memory) PropertyGroup();
		node->SetName(name);
		return node;
	}
	void PropertyMemoryPool::Clear()
	{
		while (m_begin != m_staticMemory)
		{
			char *previous_begin = reinterpret_cast<header *>(Align(m_begin))->previous_begin;
			if (m_freeFunc)
				m_freeFunc(m_begin);
			else
				MAY_DELETEA(m_begin);
			m_begin = previous_begin;
		}
		Init();
	}
	void PropertyMemoryPool::SetAllocator(AllocFunc *af, FreeFunc *ff)
	{
		assert(m_begin == m_staticMemory && m_ptr == Align(m_begin)); 
		m_allocFunc = af;
		m_freeFunc = ff;
	}
	IPropertyNode* PropertyMemoryPool::CloneNode(IPropertyNode* inCurrentNode, bool inRecurse)
	{
		if (inCurrentNode == NULL)
			return NULL;
		if (Property* pProp = inCurrentNode->AsProperty())
		{
			Property* pResult = AllocateProperty(pProp->GetName(), pProp->GetValue());
			for (const PropertyAttribute* pAttr = inCurrentNode->FirstAttribute(); pAttr; pAttr = inCurrentNode->NextAttribute())
			{
				pResult->AddAttribute(*pAttr);
			}
			return pResult;
		}
		else if (PropertyGroup* pPropGroup = inCurrentNode->AsPropertyGroup())
		{
			PropertyGroup* pResult = AllocatePropertyGroup(pPropGroup->GetName());
			for (const PropertyAttribute* pAttr = inCurrentNode->FirstAttribute(); pAttr; pAttr = inCurrentNode->NextAttribute())
			{
				pResult->AddAttribute(*pAttr);
			}
			if (inRecurse)
			{
				for (IPropertyNode* child = pPropGroup->FirstChildNode(); child; child = child->NextSibling())
				{
					pResult->AppendNode(CloneNode(child, true));
				}
			}
			return pResult;
		}
		return NULL;
	}

	void PropertyMemoryPool::DestroyNode(IPropertyNode* inCurrentNode, bool inRecurse)
	{
		if (inCurrentNode == NULL)
			return;
		if (Property* pProp = inCurrentNode->AsProperty())
		{
			MAY_DELETE(pProp);
		}
		else if (PropertyGroup* pPropGroup = inCurrentNode->AsPropertyGroup())
		{
			if (inRecurse)
			{
				for (IPropertyNode* child = pPropGroup->FirstChildNode(); child; child = child->NextSibling())
				{
					DestroyNode(child, true);
					MAY_DELETE(child);
				}
			}
			MAY_DELETE(pPropGroup);
		}
	}

	void PropertyMemoryPool::Init()
	{
		m_begin = m_staticMemory;
		m_ptr = Align(m_begin);
		m_end = m_staticMemory + sizeof(m_staticMemory);
	}
	char *PropertyMemoryPool::Align(char *ptr)
	{
		std::size_t alignment = ((PROPERTY_MEMORY_ALIGNMENT - (std::size_t(ptr) & (PROPERTY_MEMORY_ALIGNMENT - 1))) & (PROPERTY_MEMORY_ALIGNMENT - 1));
		return ptr + alignment;
	}
	char *PropertyMemoryPool::AllocateRaw(std::size_t size)
	{
		void *memory;   
		if (m_allocFunc)   
		{
			memory = m_allocFunc(size);
			assert(memory);
		}
		else
		{
			memory = MAY_NEW char[size];
		}
		return static_cast<char *>(memory);
	}
	void *PropertyMemoryPool::AllocateAligned(std::size_t size)
	{
		char *result = Align(m_ptr);

		if (result + size > m_end)
		{
			std::size_t pool_size = PROPERTY_DYNAMIC_POOL_SIZE;
			if (pool_size < size)
				pool_size = size;

			std::size_t alloc_size = sizeof(header) + (2 * PROPERTY_MEMORY_ALIGNMENT - 2) + pool_size;
			char *raw_memory = AllocateRaw(alloc_size);

			char *pool = Align(raw_memory);
			header *new_header = reinterpret_cast<header *>(pool);
			new_header->previous_begin = m_begin;
			m_begin = raw_memory;
			m_ptr = pool + sizeof(header);
			m_end = raw_memory + alloc_size;

			result = Align(m_ptr);
		}

		m_ptr = result + size;
		return result;
	}

	const PropertyValue& Property::GetValue()const
	{
		return m_value;
	}
	void Property::SetValue(const PropertyValue& inValue)
	{
		m_value = inValue;
	}
	IPropertyNode::EPropertyNodeType Property::GetNodeType()const
	{
		return PNT_Property; 
	}
	Property* Property::AsProperty()
	{
		return this; 
	}
	void Property::Clear() 
	{
		m_value.Clear();
	}

	PropertyGroup::PropertyGroup()
		: m_firstNode(NULL)
		, m_lastNode(NULL)
	{
	}
	IPropertyNode::EPropertyNodeType PropertyGroup::GetNodeType()const 
	{
		return PNT_PropertyGroup; 
	}
	PropertyGroup* PropertyGroup::AsPropertyGroup()
	{
		return this; 
	}
	void PropertyGroup::Clear()
	{
		RemoveAllNodes();
	}
	IPropertyNode* PropertyGroup::FirstChildNode(const String& name, bool case_sensitive)const
	{
		if (name.GetLength() == 0)
			return m_firstNode;
		for (IPropertyNode *child = m_firstNode; child; child = child->NextSibling())
			if (child->GetName().Equals(name, case_sensitive))
				return child;
		return 0;
	}

	IPropertyNode* PropertyGroup::LastChildNode(const String& name, bool case_sensitive) const
	{
		assert(m_firstNode); 
		if (name.GetLength() == 0)
			return m_lastNode;
		for (IPropertyNode *child = m_lastNode; child; child = child->PreviousSibling())
			if (child->GetName().Equals(name, case_sensitive))
				return child;
		return 0;
	}

	void PropertyGroup::PrependNode(IPropertyNode *child)
	{
		if (m_firstNode)
		{
			child->m_nextSibling = m_firstNode;
			m_firstNode->m_prevSibling = child;
		}
		else
		{
			child->m_nextSibling = 0;
			m_lastNode = child;
		}
		m_firstNode = child;
		child->m_parent = this;
		child->m_prevSibling = 0;
	}
	void PropertyGroup::AppendNode(IPropertyNode *child)
	{
		if (m_firstNode)
		{
			child->m_prevSibling = m_lastNode;
			m_lastNode->m_nextSibling = child;
		}
		else
		{
			child->m_prevSibling = 0;
			m_firstNode = child;
		}
		m_lastNode = child;
		child->m_parent = this;
		child->m_nextSibling = 0;
	}
	void PropertyGroup::InsertNode(IPropertyNode *where, IPropertyNode *child)
	{
		assert(!where || where->GetParent() == this);
		if (where == m_firstNode)
			PrependNode(child);
		else if (where == 0)
			AppendNode(child);
		else
		{
			child->m_prevSibling = where->m_prevSibling;
			child->m_nextSibling = where;
			where->m_prevSibling->m_nextSibling = child;
			where->m_prevSibling = child;
			child->m_parent = this;
		}
	}
	void PropertyGroup::RemoveFirstChildNode()
	{
		assert(m_firstNode);
		IPropertyNode *child = m_firstNode;
		m_firstNode = child->m_nextSibling;
		if (child->m_nextSibling)
			child->m_nextSibling->m_prevSibling = 0;
		else
			m_lastNode = 0;
		child->m_parent = 0;
	}
	void PropertyGroup::RemoveLastChildNode()
	{
		assert(m_firstNode);
		IPropertyNode *child = m_lastNode;
		if (child->m_prevSibling)
		{
			m_lastNode = child->m_prevSibling;
			child->m_prevSibling->m_nextSibling = 0;
		}
		else
			m_firstNode = 0;
		child->m_parent = 0;
	}

	void PropertyGroup::RemoveNode(IPropertyNode *where)
	{
		assert(where && where->GetParent() == this);
		assert(m_firstNode);
		if (where == m_firstNode)
			RemoveFirstChildNode();
		else if (where == m_lastNode)
			RemoveLastChildNode();
		else
		{
			where->m_prevSibling->m_nextSibling = where->m_nextSibling;
			where->m_nextSibling->m_prevSibling = where->m_prevSibling;
			where->m_parent = 0;
		}
	}
	void PropertyGroup::RemoveAllNodes()
	{
		for (IPropertyNode *node = m_firstNode; node; node = node->m_nextSibling)
			node->m_parent = 0;
		m_firstNode = 0;
	}

	XmlPropertyDocument::XmlPropertyDocument()
		:mpRoot(NULL)
	{
	}
	IPropertyNode* XmlPropertyDocument::LoadFromFile(const String& inFilePath, const String& inRootName)
	{
		if (IoSystem::Instance()->IsFileExist(inFilePath) == false)
			return NULL;
		Stream* fp = IoSystem::Instance()->OpenStream(inFilePath, Stream::ReadAccess);

		size_t datalen = fp->GetSize();
		if (datalen == 0)
		{
			fp->Close();
			return NULL;
		}
		char *pBuff = MAY_NEW char[datalen+1];
		memset(pBuff, 0, datalen+1);
		int iRet = fp->Read(pBuff, datalen);
		pBuff[datalen] = NULL;
		if (iRet <= 0)
			return NULL;
		fp->Close();

		mpRoot = Load(pBuff, inRootName);
		MAY_DELETEA(pBuff);
		return mpRoot;
	}
	bool XmlPropertyDocument::SaveToFile(const String& inFilePath, IPropertyNode* inSaveNode)
	{
		IPropertyNode* pNode = mpRoot;
		if (inSaveNode)
			pNode = inSaveNode;
		if (pNode == NULL)
			return false;

		String strXml = Save(pNode);
		if (strXml.GetLength() == 0)
			return false;
		Stream *fp = IoSystem::Instance()->OpenStream(inFilePath.AsCharPtr(), Stream::WriteAccess);
		if (fp == NULL)
			return false;
		fp->Write(strXml.AsCharPtr(), (strXml.GetLength()) * sizeof(TCHAR));
		fp->Close();
		return true;
	}
	String XmlPropertyDocument::Save(IPropertyNode* inSaveNode)
	{
		xml_document<TCHAR> kDoc;
		if (inSaveNode != NULL)
			RecurseSaveXml(inSaveNode, NULL, kDoc);
		std::string str;
		rapidxml::print(std::back_inserter(str), kDoc, 0);   
		return str.c_str();
	}
	void XmlPropertyDocument::RecurseSaveXml(IPropertyNode* inCurrentNode, xml_node<TCHAR>* inParentNode, xml_document<TCHAR>& inoutDoc)
	{
		if (inCurrentNode == NULL)
			return;
		xml_node<TCHAR>* pXmlNode = inoutDoc.allocate_node(node_element, inCurrentNode->GetName().AsCharPtr());
		if (inParentNode == NULL)
			inoutDoc.append_node(pXmlNode);
		bool bHasType = false;
		for (const PropertyAttribute* pAttr = inCurrentNode->FirstAttribute(); pAttr; pAttr = inCurrentNode->NextAttribute())
		{
			if (pAttr->m_name == "Type")
				bHasType = true;
			xml_attribute<TCHAR>* pXmlAttr = inoutDoc.allocate_attribute(pAttr->m_name.AsCharPtr(), pAttr->m_value.AsCharPtr()); 
			pXmlNode->append_attribute(pXmlAttr);
		}
		if (Property* pProp = inCurrentNode->AsProperty())
		{
			String str;
			pProp->GetValue().GetValueToString(str);
			pXmlNode->value(inoutDoc.allocate_string(str.AsCharPtr(), str.GetLength()+1));
			if (bHasType == false)
			{
				String tmpType = PropertyValue::MapPropertyString(pProp->GetValueType());
				xml_attribute<TCHAR>* pXmlAttrValueType = inoutDoc.allocate_attribute("Type", inoutDoc.allocate_string(tmpType.AsCharPtr())); 
				pXmlNode->append_attribute(pXmlAttrValueType);
			}
			if (inParentNode)
				inParentNode->append_node(pXmlNode);
		}
		else if (PropertyGroup* pPropGroup = inCurrentNode->AsPropertyGroup())
		{
			for (IPropertyNode* pChildNode = pPropGroup->FirstChildNode(); pChildNode; pChildNode = pChildNode->NextSibling())
			{
				RecurseSaveXml(pChildNode, pXmlNode, inoutDoc);
			}
			if (inParentNode)
				inParentNode->append_node(pXmlNode);
		}
	}
	IPropertyNode* XmlPropertyDocument::Load(const String& inString, const String& inRootName)
	{
		xml_document<TCHAR> kDoc;
		kDoc.parse<0>((TCHAR*)inString.AsCharPtr());
		String strRootName = inRootName;
		if (strRootName == "")
			strRootName = PROPERTY_ROOT_NAME;
		xml_node<TCHAR>* pRootElem = kDoc.first_node(strRootName.AsCharPtr());
		if (pRootElem == NULL)
			return NULL;

		return RecurseLoadXml(pRootElem, NULL);
	}
	IPropertyNode* XmlPropertyDocument::RecurseLoadXml(xml_node<TCHAR>* inCurrentNode, PropertyGroup* inParentNode)
	{
		if (inCurrentNode == NULL || inCurrentNode->name() == NULL)
			return NULL;
		eastl::vector<PropertyAttribute> kAttrArray;
		PropertyValue::EPropertyValueType kPropertyType = PropertyValue::PVT_Unknown;
		for (xml_attribute<TCHAR>* pAttr = inCurrentNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			PropertyAttribute tmp = PropertyAttribute(pAttr->name(), pAttr->value());
			if (tmp.m_name.Equals("Type", false))
				kPropertyType = PropertyValue::MapPropertyType(tmp.m_value);
			kAttrArray.push_back(tmp);
		}
		if (inCurrentNode->first_node() == NULL || inCurrentNode->first_node()->type() == node_data)
		{
			PropertyValue kPropValue;
			kPropValue.m_type = kPropertyType;
			kPropValue.SetValueFromString(inCurrentNode->value());
			Property* pProp = AllocateProperty(inCurrentNode->name(), kPropValue);
			for (uint i = 0; i < kAttrArray.size(); ++i)
			{
				pProp->AddAttribute(kAttrArray[i], true, true);
			}
			if (inParentNode)
			{
				inParentNode->AppendNode(pProp);
			}
			return pProp;
		}
		else
		{
			PropertyGroup* pPropGroup = AllocatePropertyGroup(inCurrentNode->name());
			for (uint i = 0; i < kAttrArray.size(); ++i)
			{
				pPropGroup->AddAttribute(kAttrArray[i], true, true);
			}
			if (inParentNode)
			{
				inParentNode->AppendNode(pPropGroup);
			}
			for (xml_node<TCHAR>* pChildNode = inCurrentNode->first_node(); pChildNode; pChildNode = pChildNode->next_sibling())
			{
				RecurseLoadXml(pChildNode, pPropGroup);
			}
			return pPropGroup;
		}
		return NULL;
	}
	void XmlPropertyDocument::Clear()
	{
		PropertyMemoryPool::Clear();
		mpRoot = NULL;
	}
}