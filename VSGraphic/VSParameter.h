#ifndef VSPARAMETER_H
#define VSPARAMETER_H
#include <tchar.h>
namespace VSEngine2
{
	class VSParameter
	{
	public:
		VSParameter()
		{
			m_uiGpuSkinBoneNum = 50;
		}
		~VSParameter()
		{
		
		}
		VSParameter(const TCHAR * pFileName)
		{

			m_uiGpuSkinBoneNum = 50;
		}
		void SaveToFile(const TCHAR * pFileName)
		{
			
		}
		unsigned int m_uiGpuSkinBoneNum;




	};
	class VSGlobalParameter
	{
	public:
		VSGlobalParameter();
		~VSGlobalParameter();
		static unsigned int GetGpuSkinBoneNum()
		{
			return ms_GpuSkinBoneNum;
		}
		static bool DrawSkelecton()
		{
			return ms_bDrawSkelecton;
		}

		static bool IsCacheTexture()
		{
			return ms_bIsCacheTexture;
		}
		static bool IsCacheShader()
		{
			return ms_bIsCacheShader;
		}
		static bool IsCacheName()
		{
			return ms_bIsCacheName;
		}
		static bool IsCacheVertexFormat()
		{
			return ms_bIsCacheVertexFormat;

		}
		static bool IsCacheFont()
		{
			return ms_bIsCacheFont;
		}
		static bool IsCacheSkelectionMesh()
		{
			return ms_bIsCacheSkelectionMesh;
		}
		static bool IsCacheStaticMesh()
		{
			return ms_bIsCacheStaticMesh;
		}
		static bool IsCacheAnimSet()
		{

			return ms_bIsCacheAnimSet;
		}
		static bool IsCacheMaterial()
		{
			return ms_bIsCacheMaterial;
		}
	protected:
		static unsigned int ms_GpuSkinBoneNum;
		static bool ms_bDrawSkelecton;

		static bool ms_bIsCacheTexture;
		static bool ms_bIsCacheShader;
		static bool ms_bIsCacheName;
		static bool ms_bIsCacheVertexFormat;
		static bool ms_bIsCacheFont;
		static bool ms_bIsCacheSkelectionMesh;
		static bool ms_bIsCacheStaticMesh;
		static bool ms_bIsCacheAnimSet;
		static bool ms_bIsCacheMaterial;
	
	
	};

}
#endif