#include "VSVariant.h"
#include "VSName.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
TCHAR * VSVariant::ms_VariantMap[V_MAX] = 
{
	_T("Null"),
	_T("Bool"),
	_T("Int"),
	_T("Real"),
	_T("Vector2"),
	_T("Vector3"),
	_T("Vector3W"),
	_T("Matrix3X3"),
	_T("Matrix3X3W"),
	_T("String"),
	_T("Object")
};
unsigned int VSVariant::MapVariantType(const VSString& inType)
{
	for (unsigned int i = 0 ; i < V_MAX ; i++)
	{
		if (inType == ms_VariantMap[i])
		{
			return i;
		}
	}
	return V_MAX;
}
VSString VSVariant::MapVariantString(unsigned int inType)
{
	if (inType >= V_MAX)
	{
		return _T("");
	}
	return ms_VariantMap[inType];
}
VSVariant::VSVariant(unsigned int Type)
{
	m_uiType = Type;	
	m_pP = NULL;
}
VSVariant::~VSVariant()
{
	Clear();
}
void VSVariant::Clear()
{
	if (m_uiType == V_STRING || m_uiType == V_OBJECT)
	{
		if(m_pP)
		{
			((VSReference *)m_pP)->DecreRef();
			m_pP = NULL;
		}			
	}
	m_uiType = V_NULL;	
}
VSVariant::VSVariant(bool b)
{
	*this = b;
}
VSVariant::VSVariant(int i)
{
	*this = i;
}
VSVariant::VSVariant(VSREAL f)
{
	*this = f;
}
VSVariant::VSVariant(const VSVector2 & v)
{
	*this = v;
}
VSVariant::VSVariant(const VSVector3 & v)
{
	*this = v;
}
VSVariant::VSVariant(const VSVector3W & v)
{
	*this = v;
}
VSVariant::VSVariant(const VSMatrix3X3 & m)
{
	*this = m;
}
VSVariant::VSVariant(const VSMatrix3X3W & m)
{
	*this = m;
}
VSVariant::VSVariant(VSObject * p)
{
	*this = p;
}
VSVariant::VSVariant(const TCHAR * s)
{
	*this = s;
}
VSVariant::VSVariant(const VSVariant & Va)
{
	*this = Va;
}
bool VSVariant::operator==(const VSVariant& rhs) const
{
	if (m_uiType != rhs.m_uiType)
	{
		return false;
	}
	switch (m_uiType)
	{
	case V_BOOL:
		return GetBool() == rhs.GetBool();
	case V_INT:
		return GetInt() == rhs.GetInt();
	case V_REAL:
		return GetReal() == rhs.GetReal();
	case V_VECTOR2:
		return GetVector2() == rhs.GetVector2();
	case V_VECTOR3:
		return GetVector3() == rhs.GetVector3();
	case V_VECTOR3W:
		return GetVector3W() == rhs.GetVector3W();
	case V_MATRIX3X3:
		return GetMatrix3X3() == rhs.GetMatrix3X3();
	case V_MATRIX3X3W:
		return GetMatrix3X3W() == rhs.GetMatrix3X3W();
 	case V_STRING:
 		return m_pP ==rhs.m_pP;
 	case V_OBJECT:
 		return GetObjectPtr() == rhs.GetObjectPtr();
	default:
		return false;
	}
}
bool VSVariant::operator==(bool b)const
{
	if (m_uiType != V_BOOL)
	{
		return false;
	}
	return GetBool() == b;
}
bool VSVariant::operator==(int i)const
{
	if (m_uiType != V_INT)
	{
		return false;
	}
	return GetInt() == i;	
}
bool VSVariant::operator==(VSREAL f)const
{
	if (m_uiType != V_REAL)
	{
		return false;
	}
	return GetReal() == f;
}
bool VSVariant::operator==(const VSVector2 & v)const
{
	if (m_uiType != V_VECTOR2)
	{
		return false;
	}
	return GetVector2() == v;
}
bool VSVariant::operator==(const VSVector3 & v)const
{
	if (m_uiType != V_VECTOR3)
	{
		return false;
	}
	return GetVector3() == v;
}
bool VSVariant::operator==(const VSVector3W & v)const
{
	if (m_uiType != V_VECTOR3W)
	{
		return false;
	}
	return GetVector3W() == v;
}
bool VSVariant::operator==(const VSMatrix3X3 & m)const
{
	if (m_uiType != V_MATRIX3X3)
	{
		return false;
	}
	return GetMatrix3X3() == m;
}
bool VSVariant::operator==(const VSMatrix3X3W & m)const
{
	if (m_uiType != V_MATRIX3X3W)
	{
		return false;
	}
	return GetMatrix3X3W() == m;
}
bool VSVariant::operator==(const VSObject * p)const
{
	if (m_uiType != V_OBJECT)
	{
		return false;
	}
	return GetObjectPtr() == p;
}
bool VSVariant::operator==(const TCHAR * s)const
{
	if (m_uiType != V_STRING)
	{
		return false;
	}
	return ((VSName *)m_pP)->GetString() == s;
	
}
VSVariant &VSVariant::operator=(const VSVariant &Va)
{
	Clear();
	m_uiType = Va.m_uiType;
	if (Va.m_uiType == V_STRING || Va.m_uiType == V_OBJECT)
	{
		m_pP = Va.m_pP;
		if (m_pP)
		{
			((VSReference *)m_pP)->IncreRef();
		}
		
	}
	else
	{
		VSMemcpy(m_fVec,Va.m_fVec,sizeof(m_fVec));
	}
	return *this;
}
VSVariant &VSVariant::operator=(bool b)
{
	Clear();
	m_uiType = V_BOOL;
	m_bB = b;
	return *this;
}
VSVariant &VSVariant::operator=(int i)
{
	Clear();
	m_uiType = V_INT;
	m_iI = i;
	return *this;
}
VSVariant &VSVariant::operator=(VSREAL f)
{
	Clear();
	m_uiType = V_REAL;
	m_fVec[0] = f;
	return *this;
}
VSVariant &VSVariant::operator=(const VSVector2 & v)
{
	Clear();
	m_uiType = V_VECTOR2;
	*((VSVector2 *)m_fVec) = v;
	return *this;
}	
VSVariant &VSVariant::operator=(const VSVector3 & v)
{
	Clear();
	m_uiType = V_VECTOR3;
	*((VSVector3 *)m_fVec) = v;
	return *this;
}
VSVariant &VSVariant::operator=(const VSVector3W & v)
{
	Clear();
	m_uiType = V_VECTOR3W;
	*((VSVector3W *)m_fVec) = v;
	return *this;
}
VSVariant &VSVariant::operator=(const VSMatrix3X3 & m)
{
	Clear();
	m_uiType = V_MATRIX3X3;
	*((VSMatrix3X3 *)m_fVec) = m;
	return *this;
}
VSVariant &VSVariant::operator=(const VSMatrix3X3W & m)
{
	Clear();
	m_uiType = V_MATRIX3X3W;
	*((VSMatrix3X3W *)m_fVec) = m;
	return *this;
}
VSVariant &VSVariant::operator=(VSObject * p)
{
	Clear();
	m_uiType = V_OBJECT;
	m_pP = p;
	if (m_pP)
	{
		((VSReference *)m_pP)->IncreRef();
	}
	
	return *this;
}
VSVariant &VSVariant::operator=(const TCHAR * s)
{
	Clear();
	m_uiType = V_STRING;
	m_pP = (void *)VSResourceManager::CreateName(s);
	if (m_pP)
	{
		((VSReference *)m_pP)->IncreRef();
	}
	return *this;
}
unsigned int VSVariant::GetType() const
{
	return m_uiType;
}
bool VSVariant::IsValid()const
{
	return m_uiType != V_NULL;
}
bool VSVariant::GetBool() const
{
	VSMAC_ASSERT(m_uiType == V_BOOL);
	return m_bB;
}
int VSVariant::GetInt() const
{
	VSMAC_ASSERT(m_uiType == V_INT);
	return m_iI;
}
VSREAL VSVariant::GetReal() const
{
	VSMAC_ASSERT(m_uiType == V_REAL);
	return m_fVec[0];
}
const VSVector2& VSVariant::GetVector2()const
{
	VSMAC_ASSERT(m_uiType == V_VECTOR2);
	return *((VSVector2*)m_fVec);
}
const VSVector3& VSVariant::GetVector3() const
{
	VSMAC_ASSERT(m_uiType == V_VECTOR3);
	return *((VSVector3*)m_fVec);
}
const VSVector3W& VSVariant::GetVector3W() const
{
	VSMAC_ASSERT(m_uiType == V_VECTOR3W);
	return *((VSVector3W*)m_fVec);
}
const VSMatrix3X3& VSVariant::GetMatrix3X3() const
{
	VSMAC_ASSERT(m_uiType == V_MATRIX3X3);
	return *((VSMatrix3X3*)m_fVec);
}
const VSMatrix3X3W& VSVariant::GetMatrix3X3W() const
{
	VSMAC_ASSERT(m_uiType == V_MATRIX3X3W);
	return *((VSMatrix3X3W*)m_fVec);
}
const TCHAR* VSVariant::GetString()const
{
	VSMAC_ASSERT(m_uiType == V_STRING);
	return ((VSName *)m_pP)->GetBuffer();
}
VSObject* VSVariant::GetObjectPtr()const
{
	VSMAC_ASSERT(m_uiType == V_OBJECT);
	return (VSObject *)m_pP;
}
bool VSVariant::GetValueToString(VSString& outVal)const
{
	return true;
}
bool VSVariant::SetValueFromString(const VSString& inVal)
{
	return true;
}