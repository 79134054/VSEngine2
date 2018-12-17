#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
using namespace rapidxml;

namespace VSEngine2
{
#define PROPERTY_MEMORY_ALIGNMENT sizeof(void *)
#define PROPERTY_STATIC_POOL_SIZE (64 * 1024)
#define PROPERTY_DYNAMIC_POOL_SIZE (64 * 1024)
#define PROPERTY_ROOT_NAME "Properties"
#define PROPERTY_DEFAULT_CASESENSITIVE false

	inline void* PropAlloc(size_t size)
	{
		return AppMalloc(size);
	}
	inline void PropFree(void* data)
	{
		return AppFree(data);
	}

	//struct PropertyUserData
	//{
	//	void* mPtr;
	//	int mRef;
	//	PropertyUserData() :mPtr(NULL), mRef(0) {}
	//};

	class BaseObject;
	class Property;
	struct ENGINE_API PropertyValue
	{
		enum EPropertyValueType
		{
			PVT_Unknown = -1,
			PVT_Bool = 0,
			PVT_Integer,
			PVT_UnsignedInteger,
			PVT_Real,
			PVT_String,
			PVT_Object,
			//PT_Resource,
			PVT_Count,
		};
		EPropertyValueType m_type;
		union
		{
			bool m_bool;
			int m_int;
			unsigned int m_uint;
			double m_real;
			BaseObject* m_obj;
		};
		String m_string;
		PropertyValue();
		PropertyValue(EPropertyValueType inType) : m_type(inType) {}
		PropertyValue(bool inVal) : m_type(PVT_Bool), m_bool(inVal) {}
		PropertyValue(short inVal) : m_type(PVT_Integer), m_int((int)inVal) {}
		PropertyValue(unsigned short inVal) : m_type(PVT_UnsignedInteger), m_uint((unsigned int)inVal) {}
		PropertyValue(int inVal) : m_type(PVT_Integer), m_int(inVal) {}
		PropertyValue(unsigned int inVal) : m_type(PVT_UnsignedInteger), m_uint(inVal) {}
		PropertyValue(float inVal) : m_type(PVT_Real), m_real((double)inVal) {}
		PropertyValue(double inVal) : m_type(PVT_Real), m_real((double)inVal) {}
		PropertyValue(const TCHAR* inVal) : m_type(PVT_String), m_string(inVal) {}
		PropertyValue(const String& inVal) : m_type(PVT_String), m_string(inVal) {}
		PropertyValue(BaseObject* inVal) : m_type(PVT_Object), m_obj(inVal) {}
		PropertyValue(const PropertyValue& inVal);

		PropertyValue& operator = (const PropertyValue& inVal);
		bool operator == (const PropertyValue& inVal)const;
		bool operator!=(const PropertyValue& inVal)const { return !(*this == inVal); };

		bool GetValue(bool& outVal)const { if (m_type==PVT_Bool) { outVal = m_bool; return true; }; return false; };
		bool GetValue(short& outVal)const { if (m_type==PVT_Integer) { outVal = (short)m_int; return true; }; return false; };
		bool GetValue(unsigned short& outVal)const { if (m_type==PVT_UnsignedInteger) { outVal = (unsigned short)m_uint; return true; }; return false; };
		bool GetValue(int& outVal)const { if (m_type==PVT_Integer) { outVal = m_int; return true; }; return false; };
		bool GetValue(unsigned int& outVal)const { if (m_type==PVT_UnsignedInteger) { outVal = m_uint; return true; }; return false; };
		bool GetValue(float& outVal)const { if (m_type==PVT_Real) { outVal = (float)m_real; return true; }; return false; };
		bool GetValue(double& outVal)const { if (m_type==PVT_Real) { outVal = m_real; return true; }; return false; };
		bool GetValue(String& outVal)const { if (m_type==PVT_String) { outVal = m_string; return true; }; return false; };
		bool GetValue(BaseObject*& outVal)const { if (m_type==PVT_Object) { outVal = m_obj; return true; }; return false; };

		bool GetValueToString(String& outVal)const;
		bool SetValueFromString(const String& inVal);
		void Swap(PropertyValue& outVal);
		void Clear();
		static PropertyValue::EPropertyValueType MapPropertyType(const String& inType);
		static String MapPropertyString(PropertyValue::EPropertyValueType inType);
	};

	struct VSSYSTEM_API PropertyAttribute
	{
		PropertyAttribute()
		{
		}
		PropertyAttribute(const TCHAR* inName, const TCHAR* inValue)
			:m_name(inName), m_value(inValue)
		{
		}
		template<typename _Ty>
		bool GetValue(_Ty& outValue)
		{
			bool bOk = false;
			outValue = StringTo<_Ty>(m_value, &bOk);
			return bOk;
		}
		String m_name;
		String m_value;
	};
	class VSSYSTEM_API IPropertyNode
	{
		friend class PropertyGroup;
		friend class PropertyMemoryPool;
	protected:
		IPropertyNode();
	public:
		enum EPropertyFlag
		{
			PF_HasMin			= BIT(0),
			PF_HasMax			= BIT(1),
			PF_HasStep			= BIT(2),
			PF_HasCategory		= BIT(3),
			PF_HasDefaultValue	= BIT(4),
			PF_HasDescription	= BIT(5),
			PF_HasDisplayName	= BIT(6),
			PF_HasReadOnly		= BIT(7),
		};
		//enum EPropertyUserType
		//{
		//	PUT_Unknown = -1,
		//	PUT_Color = 0,
		//	PUT_ColorFlt,
		//};

		enum EPropertyNodeType
		{
			PNT_Property = 0,
			PNT_PropertyGroup,
		};

		const String& GetName()const;
		void SetName(const String& inName);
		IPropertyNode* GetParent() const;
		int GetAttributeCount()const;
		virtual void Clear() = 0;
		virtual EPropertyNodeType GetNodeType()const = 0;
		virtual Property* AsProperty() { return NULL; }
		virtual PropertyGroup* AsPropertyGroup() { return NULL; }
		bool AddAttribute(const TCHAR* inName, const TCHAR* inValue, bool inUpdateIfExist=true, bool inCaseSensitive=PROPERTY_DEFAULT_CASESENSITIVE);
		bool AddAttribute(const PropertyAttribute& inAttr, bool inUpdateIfExist=true, bool inCaseSensitive=PROPERTY_DEFAULT_CASESENSITIVE);
		bool RemoveAttribute(const String& inName, bool inCaseSensitive=PROPERTY_DEFAULT_CASESENSITIVE);
		bool FindAttribute(const String& inName, PropertyAttribute* outVal=NULL, bool inCaseSensitive=PROPERTY_DEFAULT_CASESENSITIVE);
		const PropertyAttribute* FirstAttribute()const;
		const PropertyAttribute* NextAttribute()const;
		unsigned int GetPropertyFlags()const;
		bool HasPropertyFlag(EPropertyFlag inFlag)const;
		IPropertyNode* PreviousSibling(const String& name="", bool case_sensitive = true)const;
		IPropertyNode* NextSibling(const String& name="", bool case_sensitive = true)const;
		void SetUserData(void* inData);
		void* GetUserData()const;
	private:
		void TryUpdateFlags(const String& name, bool inEnable);
		void SetFlag(EPropertyFlag inFlag, bool inEnable);
		PropertyAttribute* GetAttribute(const String& inName, bool inCaseSensitive=PROPERTY_DEFAULT_CASESENSITIVE);
	protected:
		String m_name;
		IPropertyNode *m_parent;
		IPropertyNode *m_prevSibling;
		IPropertyNode *m_nextSibling;
		eastl::list<PropertyAttribute> m_attributes;
		mutable eastl::list<PropertyAttribute>::const_iterator m_iter;
		unsigned int m_propertyFlags;
		void* m_userData;
	};

	class ENGINE_API PropertyMemoryPool
	{
	public:
		typedef void *(AllocFunc)(size_t);
		typedef void (FreeFunc)(void *);

		PropertyMemoryPool();
		virtual ~PropertyMemoryPool();
		Property *AllocateProperty(const String& name, const PropertyValue& value);
		Property *AllocateProperty(const String& name);
		template<typename _Ty>
		Property *AllocateProperty(const String& name, const _Ty& value)
		{
			void *memory = AllocateAligned(sizeof(Property));
			Property *node = ::new(memory) Property();
			node->SetName(name);
			node->SetValue<_Ty>(value);
			return node;
		}
		PropertyGroup *AllocatePropertyGroup(const String& name);
		virtual void Clear();
		void SetAllocator(AllocFunc *af, FreeFunc *ff);

		IPropertyNode* CloneNode(IPropertyNode* inCurrentNode, bool inRecurse=true);
		void DestroyNode(IPropertyNode* inCurrentNode, bool inRecurse=true);
	private:
		struct header
		{
			char *previous_begin;
		};
		void Init();
		char *Align(char *ptr);
		char *AllocateRaw(size_t size);
		void *AllocateAligned(size_t size);

		char *m_begin;
		char *m_ptr;  
		char *m_end;  
		char m_staticMemory[PROPERTY_STATIC_POOL_SIZE];
		AllocFunc *m_allocFunc;                        
		FreeFunc *m_freeFunc;                          
	};

	class ENGINE_API Property : public IPropertyNode
	{
		friend class PropertyMemoryPool;
	private:
		Property() {}
	public:
		const PropertyValue& GetValue()const;
		void SetValue(const PropertyValue& inValue);
		template<typename _Ty>
		bool GetValue(_Ty& outValue)const
		{
			return m_value.GetValue(outValue);
		}
		template<typename _Ty>
		void SetValue(const _Ty& inVal)
		{
			m_value = PropertyValue(inVal);
		}
		PropertyValue::EPropertyValueType GetValueType()const { return m_value.m_type; }
		virtual EPropertyNodeType GetNodeType()const;
		virtual Property* AsProperty();
		virtual void Clear();
	private:
		PropertyValue m_value;
	};

	class ENGINE_API PropertyGroup : public IPropertyNode
	{
		friend class PropertyMemoryPool;
	private:
		PropertyGroup();
	public:
		virtual EPropertyNodeType GetNodeType()const;
		virtual PropertyGroup* AsPropertyGroup();
		virtual void Clear();
		IPropertyNode *FirstChildNode(const String& name="", bool case_sensitive = true)const;
		IPropertyNode *LastChildNode(const String& name="", bool case_sensitive = true)const;
		void PrependNode(IPropertyNode *child);
		void AppendNode(IPropertyNode *child);
		void InsertNode(IPropertyNode *where, IPropertyNode *child);
		void RemoveFirstChildNode();
		void RemoveLastChildNode();
		void RemoveNode(IPropertyNode *where);
		void RemoveAllNodes();
	private:
		IPropertyNode *m_firstNode;
		IPropertyNode *m_lastNode;
	};

	class ENGINE_API XmlPropertyDocument : public PropertyMemoryPool
	{
	public:
		XmlPropertyDocument();
		IPropertyNode* LoadFromFile(const String& inFilePath, const String& inRootName="");
		bool SaveToFile(const String& inFilePath, IPropertyNode* inSaveNode=NULL);
		IPropertyNode* Load(const String& inString, const String& inRootName="");
		String Save(IPropertyNode* inSaveNode=NULL);
		virtual void Clear();
	private:
		IPropertyNode* RecurseLoadXml(xml_node<TCHAR>* inNode, PropertyGroup* inParentNode);
		void RecurseSaveXml(IPropertyNode* inCurrentNode, xml_node<TCHAR>* inParentNode, xml_document<TCHAR>& inoutDoc);
	private:
		IPropertyNode* mpRoot;
	};
}