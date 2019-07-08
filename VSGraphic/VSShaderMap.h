#ifndef VSSHADERMAP_H
#define VSSHADERMAP_H
#include "VSObject.h"
namespace VSEngine2
{
	DECLARE_Ptr(VSShader);
	DECLARE_Ptr(VSShaderKey);
	typedef VSMapOrder<VSShaderKey,VSShaderPtr> VSShaderSet;
	class VSGRAPHIC_API VSShaderMapLoadSave : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSShaderMapLoadSave(){}
		~VSShaderMapLoadSave(){}
		VSMapOrder<VSUsedName,VSShaderSet> m_ShaderMap;

	};
	DECLARE_Ptr(VSShaderMapLoadSave);
	VSTYPE_MARCO(VSShaderMapLoadSave);
	class VSGRAPHIC_API VSShaderMap
	{
		
	public:
		VSShaderMap(VSString ShaderMapName);
		~VSShaderMap();
	protected:

		
		VSMapOrder<VSUsedName,VSShaderSet> m_ShaderMap;
	public:	
		VSMapOrder<VSUsedName,VSShaderSet>&GetShaderMap()
		{
			return m_ShaderMap;
		}
		void SetShader(const VSUsedName & Name,const VSShaderKey & Key,VSShader * pShader);
		VSShaderSet * GetShaderSet(const VSUsedName &Name);
		void DeleteShaderSet(const VSUsedName & Name);
		VSShader * GetShader(const VSUsedName & Name,const VSShaderKey & Key);
		void DeleteShader(const VSUsedName & Name,const VSShaderKey & Key);
		void Clear()
		{
			m_ShaderMap.Clear();
		}
		void GCResource()
		{

		}
		bool IsRealeseAll()
		{
			return (!m_ShaderMap.GetNum());
		}
		VSString m_ShaderMapName;
	};
	
}
#endif