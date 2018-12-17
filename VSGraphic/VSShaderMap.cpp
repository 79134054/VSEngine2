#include "VSShaderMap.h"
#include "VSGeometry.h"
#include "VSBoneNode.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSShaderMapLoadSave,VSObject)
BEGIN_ADD_PROPERTY(VSShaderMapLoadSave,VSObject)
REGISTER_PROPERTY(m_ShaderMap,ShaderMap,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSShaderMapLoadSave)
IMPLEMENT_INITIAL_END
VSShaderMap::VSShaderMap(VSString ShaderMapName)
{
	m_ShaderMapName = ShaderMapName;
}
VSShaderMap::~VSShaderMap()
{
	Clear();
}
void VSShaderMap::SetShader(const VSUsedName & Name,const VSShaderKey & Key,VSShader * pShader)
{
	unsigned int index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		m_ShaderMap[index].Value.AddElement(Key,pShader);
	}
	else
	{
		VSShaderSet Temp;
		Temp.AddElement(Key,pShader);
		m_ShaderMap.AddElement(Name,Temp);
	}
}
VSShaderSet * VSShaderMap::GetShaderSet(const VSUsedName &Name)
{
	unsigned int index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		return &m_ShaderMap[index].Value;
	}
	else
	{
		return NULL;
	}
}
void VSShaderMap::DeleteShaderSet(const VSUsedName & Name)
{
	unsigned int index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		m_ShaderMap.Erase(index);
	}
}
VSShader * VSShaderMap::GetShader(const VSUsedName & Name,const VSShaderKey &Key)
{
	unsigned int index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		VSShaderSet * pShaderSet = &m_ShaderMap[index].Value;
		unsigned int index2 =(*pShaderSet).Find(Key);
		if (index2 != (*pShaderSet).GetNum())
		{
			return (*pShaderSet)[index2].Value;
		}
	}
	return NULL;
}
void VSShaderMap::DeleteShader(const VSUsedName & Name,const VSShaderKey &Key)
{
	unsigned int index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		VSShaderSet * pShaderSet = &m_ShaderMap[index].Value;
		unsigned int index2 = (*pShaderSet).Find(Key);
		if (index2 != (*pShaderSet).GetNum())
		{
			return (*pShaderSet).Erase(index2);
		}
	}
}