#include "VSResourceManager.h"
#include "VSGeometry.h"
#include "VSLight.h"
#include "VSMaterial.h"
#include "VSVShader.h"
#include "VSPShader.h"
#include "VSGShader.h"
#include "VSDShader.h"
#include "VSHShader.h"
#include "VSName.h"
#include "VSTexAllState.h"
#include "VSImage.h"
#include "VSShaderStringFactory.h"
#include "VSMatch.h"
#include "VSNVCompression.h"
#include "VS2DTexture.h"
#include "VSGraphicInclude.h"
#include "VSASYNLoader.h"
#include "VSRenderTarget.h"
#include "VSDepthStencil.h"
#include "VSCubeTexture.h"
#include "VSStream.h"
#include "VSPostEffectSet.h"
#include "VSAnimTree.h"
#include "VSMorphTree.h"
#include "VSCaptureTexAllState.h"
#include "VSProfiler.h"
#include "VSActor.h"
#include "VSSceneMap.h"
#include "VSWorld.h"
#include "VSDynamicBufferGeometry.h"
using namespace VSEngine2;
namespace VSEngine2
{

	class VSGCTask :public VSMemObject
	{
	public:
		VSGCTask(VSArray<VSObject *>& CanGCObject, unsigned int CanGCNum = 50)
		{
			m_CanGCNum = CanGCNum;
			m_CanGCObject = CanGCObject;
			CurClearIndex = 0;
			CurDeleteIndex = 0;
			GCStream.SetStreamFlag(VSStream::AT_CLEAR_OBJECT_PROPERTY_GC);
			m_pNextTask = NULL;
		}
		void Run()
		{
			if (CurClearIndex >= m_CanGCObject.GetNum())
			{
				unsigned int MaxDeleteIndex = CurDeleteIndex + m_CanGCNum;
				for (; CurDeleteIndex < MaxDeleteIndex && CurDeleteIndex < m_CanGCObject.GetNum(); CurDeleteIndex++)
				{
					VSMAC_DELETE(m_CanGCObject[CurDeleteIndex]);
				}
			}
			else
			{
				unsigned int MaxClearIndex = CurClearIndex + m_CanGCNum;
				for (; CurClearIndex < MaxClearIndex && CurClearIndex < m_CanGCObject.GetNum(); CurClearIndex++)
				{
					GCStream.ArchiveAll(m_CanGCObject[CurClearIndex]);
				}
			}
		}
		VSGCTask * m_pNextTask;
		bool IsEnd()
		{
			return CurDeleteIndex >= m_CanGCObject.GetNum();
		}
	private:
		VSArray<VSObject *> m_CanGCObject;
		unsigned int m_CanGCNum;
		unsigned int CurClearIndex;
		unsigned int CurDeleteIndex;
		VSStream GCStream;

	};
}
VSResouceControll::VSResouceControll(unsigned int uiGCMaxTimeCount)
{
	m_uiGCMaxTimeCount = uiGCMaxTimeCount;
}
VSResouceControll::~VSResouceControll()
{

}
void VSResouceControll::GCResource()
{

}
namespace VSEngine2
{
	bool operator ==(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2)
	{
		return DBI1.pVertexFormat == DBI2.pVertexFormat && DBI1.uiMeshDataType == DBI2.uiMeshDataType;
	}
	bool operator >(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2)
	{
		if (DBI1.pVertexFormat > DBI2.pVertexFormat)
		{
			return true;
		}
		else if (DBI1.pVertexFormat == DBI2.pVertexFormat)
		{
			return DBI1.uiMeshDataType > DBI2.uiMeshDataType;
		}
		else
		{
			return false;
		}
	}
	bool operator < (const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2)
	{
		if (DBI1.pVertexFormat < DBI2.pVertexFormat)
		{
			return true;
		}
		else if (DBI1.pVertexFormat == DBI2.pVertexFormat)
		{
			return DBI1.uiMeshDataType < DBI2.uiMeshDataType;
		}
		else
		{
			return false;
		}
	}
}
VSArray<VSDynamicBufferGeometryPtr> VSResourceManager::ms_SaveDelete;
unsigned int VSResourceManager::ms_uiGpuSkinBoneNum = 70;
bool VSResourceManager::ms_bActiveDebugDraw = true;
bool VSResourceManager::ms_bDrawSkelecton = true;
bool VSResourceManager::ms_bRenderThread = false;
bool VSResourceManager::ms_bUpdateThread = false;
bool VSResourceManager::ms_bIsCacheName = false;
bool VSResourceManager::ms_bIsCacheShader = true;
bool VSResourceManager::ms_bIsCacheVertexFormat = false;
VSString VSResourceManager::ms_TexturePath;
VSString VSResourceManager::ms_AnimPath;
VSString VSResourceManager::ms_MaterialPath;
VSString VSResourceManager::ms_SkelectionMeshPath;
VSString VSResourceManager::ms_MorphPath;
VSString VSResourceManager::ms_OutputShaderCodePath;
VSString VSResourceManager::ms_OutPutLogPath;
VSString VSResourceManager::ms_ShaderPath;
VSString VSResourceManager::ms_StaticMeshPath;
VSString VSResourceManager::ms_TerrainPath;
VSString VSResourceManager::ms_ActorPath;
VSString VSResourceManager::ms_MapPath;
VSString VSResourceManager::ms_FontPath;
VSString VSResourceManager::ms_PostEffectPath;
VSString VSResourceManager::ms_ResourcePath;
VSCriticalSection VSResourceManager::ms_TextureCri;
VSCriticalSection VSResourceManager::ms_MaterialCri ;
VSCriticalSection VSResourceManager::ms_AnimCri ;
VSCriticalSection VSResourceManager::ms_StaticMeshCri ;
VSCriticalSection VSResourceManager::ms_SkeletonMeshCri ;
VSCriticalSection VSResourceManager::ms_VShaderCri ;
VSCriticalSection VSResourceManager::ms_PShaderCri ;
VSCriticalSection VSResourceManager::ms_GShaderCri;
VSCriticalSection VSResourceManager::ms_PostEffectCri ;
VSCriticalSection VSResourceManager::ms_NameCri;
VSCriticalSection VSResourceManager::ms_AnimTreeCri;
VSCriticalSection VSResourceManager::ms_MorphTreeCri;
VSCriticalSection VSResourceManager::ms_ActorCri;
VSCriticalSection VSResourceManager::ms_MapCri;
VSCriticalSection VSResourceManager::ms_FontCri;
unsigned int VSResourceManager::ms_CurRenderAPIType = 0;
VSArray<VSObject *>VSResourceManager::ms_pRootObject;
VSArrayOrder<VSObject *>VSResourceManager::ms_pGCObject;
VSGCTask * VSResourceManager::ms_pCurGCTask = NULL;
VSGCTask * VSResourceManager::ms_pEndGCTask = NULL;
VSArray<VSRtti *> VSResourceManager::ms_ResourceRtti;
VSStaticMeshNodeRPtr VSResourceManager::ms_DefaultStaticMeshNodeResource = NULL;
VSSkelectonMeshNodeRPtr VSResourceManager::ms_DefaultSkelectonMeshNodeResource = NULL;
VSTexAllStateRPtr VSResourceManager::ms_DefaultTextureResource = NULL;
VSMaterialRPtr VSResourceManager::ms_DefaultMaterialResource = NULL;
VSMaterialRPtr VSResourceManager::ms_DefaultOnlyColorMaterialResource = NULL;
bool VSResourceManager::ms_EnableAsynLoad = true;
IMPLEMENT_PRIORITY(VSResourceManager)
IMPLEMENT_INITIAL_ONLY_BEGIN(VSResourceManager);
ADD_PRIORITY(VSVertexFormat);
ADD_PRIORITY(VSVShader);
ADD_PRIORITY(VSPShader);
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_ONLY_END
bool VSResourceManager::TerminalDefaultState()
{
	ms_DefaultStaticMeshNodeResource = NULL;
	ms_DefaultSkelectonMeshNodeResource = NULL;
	ms_DefaultTextureResource = NULL;
	ms_DefaultMaterialResource = NULL;
	ms_DefaultOnlyColorMaterialResource = NULL;
	return true;
}
bool VSResourceManager::InitialDefaultState()
{
	InitConfig();
	ms_ResourceRtti.AddElement(&VSAnimTree::ms_Type);
	ms_ResourceRtti.AddElement(&VSAnim::ms_Type);
	ms_ResourceRtti.AddElement(&VSMorphTree::ms_Type);
	ms_ResourceRtti.AddElement(&VSMaterialInstance::ms_Type);
	ms_ResourceRtti.AddElement(&VSShader::ms_Type);
	ms_ResourceRtti.AddElement(&VSMeshNode::ms_Type);
	ms_ResourceRtti.AddElement(&VSPostEffectSet::ms_Type);
	ms_ResourceRtti.AddElement(&VSTexAllState::ms_Type);
	ms_ResourceRtti.AddElement(&VSFont::ms_Type);
	return 1;
}
VSResourceManager::VSResourceManager()
{
	
}
VSResourceManager::~VSResourceManager()
{

}
void VSResourceManager::InitConfig()
{

	VSMatch Match;
	if(!Match.Open(_T("Resource\\Config.txt")))
		return;

	//map
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return;
	} // end if

	if (Match.PatternMatch("['MapPath'][s>0]"))
	{
		ms_MapPath = Match.pstrings[0];
	} // end if

	//actor
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return;
	} // end if

	if (Match.PatternMatch("['ActorPath'][s>0]"))
	{
		ms_ActorPath = Match.pstrings[0];
	} // end if

	//texture
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['TexturePath'][s>0]"))
	{
		ms_TexturePath = Match.pstrings[0];
	} // end if

	// anim
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['AnimPath'][s>0]"))
	{
		ms_AnimPath = Match.pstrings[0];
	} // end if

	//material
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['MaterialPath'][s>0]"))
	{
		ms_MaterialPath = Match.pstrings[0];
	} // end if
	
	//material effect

	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['PostEffectPath'][s>0]"))
	{
		ms_PostEffectPath = Match.pstrings[0];
	} // end if
	
	//skelecton mesh
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['SKelectonMeshPath'][s>0]"))
	{
		ms_SkelectionMeshPath = Match.pstrings[0];
	} // end if

	//Morph
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['MorphPath'][s>0]"))
	{
		ms_MorphPath = Match.pstrings[0];
	} // end if

	//output
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['OutputShaderCodePath'][s>0]"))
	{
		ms_OutputShaderCodePath = Match.pstrings[0];
	} // end if

	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['OutputLogPath'][s>0]"))
	{
		ms_OutPutLogPath = Match.pstrings[0];
	} // end if

	//Shader
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['ShaderPath'][s>0]"))
	{
		ms_ShaderPath = Match.pstrings[0];
	} // end if

	//StaticMesh
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['StaticMeshPath'][s>0]"))
	{
		ms_StaticMeshPath = Match.pstrings[0];
	} // end if

	//Terrain

	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['TerrainPath'][s>0]"))
	{
		ms_TerrainPath = Match.pstrings[0];
	} // end if
	
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['ResourcePath'][s>0]"))
	{
		ms_ResourcePath = Match.pstrings[0];
	}
	
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return;
	} // end if
	if (Match.PatternMatch("['EnableAsynLoad'][s>0]"))
	{
		VSString TrueString = _T("true");
		ms_EnableAsynLoad = (Match.pstrings[0] == TrueString);
	}

	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return;
	} // end if
	if (Match.PatternMatch("['FontPath'][s>0]"))
	{
		ms_FontPath = Match.pstrings[0];
	}
}
VSDVGeometry *VSResourceManager::GetDVGeometry(VSVertexFormat * pVertexFormat, unsigned int MeshDataType, unsigned int VertexNum)
{
	if (!pVertexFormat || !VertexNum || MeshDataType>= VSMeshDate::MDT_MAX)
	{
		return NULL;
	}
	VSDVGeometryPtr  pBuffer = NULL;
	DynamicBufferIndex DBI;
	DBI.pVertexFormat = pVertexFormat;
	DBI.uiMeshDataType = MeshDataType;
	pBuffer = GetDVGeometryArray().CheckIsHaveTheResource(DBI);
	if (pBuffer == NULL)
	{
		pBuffer = NULL;
		pBuffer = VS_NEW VSDVGeometry();
		pBuffer->Create(pVertexFormat->m_FormatArray, MeshDataType, VertexNum);
		GetDVGeometryArray().AddResource(DBI, pBuffer);
	}
	else
	{	
		if (pBuffer->GetMaxRenderVertexNum() < VertexNum)
		{		
			ms_SaveDelete.AddElement(pBuffer.GetObject());
			pBuffer = VS_NEW VSDVGeometry();
			pBuffer->Create(pVertexFormat->m_FormatArray, MeshDataType, VertexNum);
			MapElement<DynamicBufferIndex, VSDVGeometryPtr> * PTemp = (MapElement<DynamicBufferIndex, VSDVGeometryPtr> *)(GetDVGeometryArray().GetResource(GetDVGeometryArray().GetResourceIndexByKey(DBI)));
			PTemp->Value = pBuffer;
		}
	}

	return pBuffer;
}
void VSResourceManager::ClearDynamicBufferGeometry()
{
	for (unsigned int i = 0; i < GetDVGeometryArray().GetResourceNum(); i++)
	{
		GetDVGeometryArray().GetResource(i)->Value->ClearInfo();
	}
	if (VSResourceManager::ms_bRenderThread)
	{
		for (unsigned int i = 0 ; i < VSBind::ms_BindArray.GetNum() ; i++)
		{
			VSBind * pBind  = VSBind::ms_BindArray[i];
			pBind->ASYNClearInfo();
		}
	}
	ms_SaveDelete.Clear();	
}
const TCHAR* VSResourceManager::GetRenderTypeShaderPath(unsigned int RenderTypeAPI)
{
	if (RenderTypeAPI == VSRenderer::RAT_DIRECTX9)
	{
		return _T("Dx9/");
	}
	else if (RenderTypeAPI == VSRenderer::RAT_DIRECTX11)
	{
		return _T("Dx11/");
	}
	else
	{
		return _T("");
	}
}
void VSResourceManager::LoadDefaultResource(unsigned int RenderTypeAPI)
{
	InitCacheShader(RenderTypeAPI);

	ms_DefaultTextureResource = VSTexAllStateR::Create((VSTexAllState *)VSTexAllState::GetDefalut());
	VSMaterial::LoadDefault();
	ms_DefaultMaterialResource = VSMaterialR::Create((VSMaterial *)VSMaterial::GetDefalut());
	VSGeometry::GetDefaultRenderCube()->AddMaterialInstance(ms_DefaultMaterialResource);
	ms_DefaultStaticMeshNodeResource = VSStaticMeshNodeR::Create((VSStaticMeshNode *)VSStaticMeshNode::GetDefalut());
	ms_DefaultSkelectonMeshNodeResource = VSSkelectonMeshNodeR::Create((VSSkelectonMeshNode*)VSSkelectonMeshNode::GetDefalut());
	ms_DefaultOnlyColorMaterialResource = VSMaterialR::Create((VSMaterial *)VSMaterial::GetDefaultOnlyColor());

}
void VSResourceManager::InitCacheShader(unsigned int RenderTypeAPI)
{
	ms_CurRenderAPIType = RenderTypeAPI;
	if (ms_CurRenderAPIType == VSRenderer::RAT_NULL)
	{
		return;
	}
	if (!VSResourceManager::IsCacheShader())
	{
		return;
	}
	VSString RenderAPIPre = VSResourceManager::GetRenderTypeShaderPath(ms_CurRenderAPIType);
#define LOAD_SHADER_CACHE(ShaderMapName) \
	{\
		VSStream LoadStream;\
		VSString FileName = ms_ShaderPath + RenderAPIPre + Get##ShaderMapName().m_ShaderMapName; \
		FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_SHADER);\
		LoadStream.NewLoad(FileName.GetBuffer());\
		VSShaderMapLoadSave * pShaderMapLoadSave = (VSShaderMapLoadSave *)LoadStream.GetObjectByRtti(VSShaderMapLoadSave::ms_Type);\
		if (pShaderMapLoadSave)\
		{\
			Get##ShaderMapName().GetShaderMap() = pShaderMapLoadSave->m_ShaderMap; \
			VSDelete(pShaderMapLoadSave); \
		}\
	}


	LOAD_SHADER_CACHE(MaterialShaderMap);
	LOAD_SHADER_CACHE(IndirectShaderMap);
	LOAD_SHADER_CACHE(VertexShaderMap);
	LOAD_SHADER_CACHE(PixelShaderMap);
	LOAD_SHADER_CACHE(GeometryShaderMap);
	LOAD_SHADER_CACHE(NormalDepthShaderMap);
	LOAD_SHADER_CACHE(CubShadowShaderMap);
	LOAD_SHADER_CACHE(VolumeShadowShaderMap);
	LOAD_SHADER_CACHE(ShadowShaderMap);
	LOAD_SHADER_CACHE(DualParaboloidShadowShaderMap);
	LOAD_SHADER_CACHE(InnerVertexShaderMap);
	LOAD_SHADER_CACHE(InnerPixelShaderMap);
	LOAD_SHADER_CACHE(InnerGeometryShaderMap);
}
bool VSResourceManager::CacheName()
{


	return 1;
}
bool VSResourceManager::CacheVertexFormat()
{

	return 1;
}
void VSResourceManager::AddCanGCObject(VSArray<VSObject *>& CanGCObject)
{
	if (CanGCObject.GetNum() == 0)
	{
		return;
	}
	if (!ms_pCurGCTask)
	{
		ms_pCurGCTask = VS_NEW VSGCTask(CanGCObject);
		ms_pEndGCTask = ms_pCurGCTask;
	}
	else
	{
		ms_pEndGCTask->m_pNextTask = VS_NEW VSGCTask(CanGCObject);
		ms_pEndGCTask = ms_pEndGCTask->m_pNextTask;
	}
}
void VSResourceManager::RunGCTask()
{
	if (ms_pCurGCTask)
	{
		ms_pCurGCTask->Run();
		if (ms_pCurGCTask->IsEnd())
		{
			VSGCTask * Temp = ms_pCurGCTask;
			ms_pCurGCTask = ms_pCurGCTask->m_pNextTask;
			VSMAC_DELETE(Temp);
		}
	}
}
void VSResourceManager::RunAllGCTask()
{
	ms_pRootObject.Clear();
	while (ms_pCurGCTask)
	{
		RunGCTask();
	}
	VSMAC_ASSERT(ms_pGCObject.GetNum() == 0);
}
void VSResourceManager::GCObject()
{
	{
		VSObject::GetObjectManager().PrepareForGC();
	}
	// collect not reachable object
	{
		VSStream GCCollectStream;
		GCCollectStream.SetStreamFlag(VSStream::AT_OBJECT_COLLECT_GC);
		for (unsigned int i = 0; i < ms_pRootObject.GetNum(); i++)
		{
			GCCollectStream.ArchiveAll(ms_pRootObject[i]);
		}
	}

	{
		VSArray<VSObject *> CanGCObject;
		for (unsigned int i = 0; i < ms_pGCObject.GetNum();)
		{
			VSObject * p = ms_pGCObject[i];
			if (p->IsHasFlag(VSObject::OF_UNREACH))
			{
				CanGCObject.AddElement(p);
				ms_pGCObject.Erase(i);
			}
			else
			{
				i++;
			}
		}

		AddCanGCObject(CanGCObject);
		RunGCTask();
	}

}
bool VSResourceManager::CacheShader()
{
	if (ms_CurRenderAPIType == VSRenderer::RAT_NULL)
	{
		return true;
	}
	VSString RenderAPIPre = VSResourceManager::GetRenderTypeShaderPath(ms_CurRenderAPIType);
#define SAVE_SHADER_CACHE(ShaderMapName)\
	{\
		VSStream SaveStream;\
		SaveStream.SetStreamFlag(VSStream::AT_REGISTER);\
		VSShaderMapLoadSave * pShaderMapLoadSave = VS_NEW VSShaderMapLoadSave();\
		pShaderMapLoadSave->m_ShaderMap = Get##ShaderMapName().GetShaderMap(); \
		SaveStream.ArchiveAll(pShaderMapLoadSave);\
		VSString FileName = ms_ShaderPath + RenderAPIPre + Get##ShaderMapName().m_ShaderMapName; \
		FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_SHADER);\
		SaveStream.NewSave(FileName.GetBuffer());\
		VSDelete(pShaderMapLoadSave);\
	}

	SAVE_SHADER_CACHE(MaterialShaderMap);
	SAVE_SHADER_CACHE(IndirectShaderMap);
	SAVE_SHADER_CACHE(VertexShaderMap);
	SAVE_SHADER_CACHE(PixelShaderMap);
	SAVE_SHADER_CACHE(GeometryShaderMap);
	SAVE_SHADER_CACHE(NormalDepthShaderMap);
	SAVE_SHADER_CACHE(CubShadowShaderMap);
	SAVE_SHADER_CACHE(VolumeShadowShaderMap);
	SAVE_SHADER_CACHE(ShadowShaderMap);
	SAVE_SHADER_CACHE(DualParaboloidShadowShaderMap);
	SAVE_SHADER_CACHE(InnerVertexShaderMap);
	SAVE_SHADER_CACHE(InnerPixelShaderMap);
	SAVE_SHADER_CACHE(InnerGeometryShaderMap);
	
	return 1;
}
void VSResourceManager::CacheResource()
{
	GetASYNMorphTreeSet().ClearAll();
	GetASYNAnimTreeSet().ClearAll();
	GetASYNSkelectonMeshSet().ClearAll();	
	GetASYNStaticMeshSet().ClearAll();	
	GetASYNMaterialSet().ClearAll();


	if (VSResourceManager::IsCacheShader())
	{
		CacheShader();

	}
#define CLEAR_SHADER_CACHE(ShaderMapName) 	Get##ShaderMapName().Clear(); 

	CLEAR_SHADER_CACHE(MaterialShaderMap);
	CLEAR_SHADER_CACHE(IndirectShaderMap);
	CLEAR_SHADER_CACHE(VertexShaderMap);
	CLEAR_SHADER_CACHE(PixelShaderMap);
	CLEAR_SHADER_CACHE(GeometryShaderMap);
	CLEAR_SHADER_CACHE(NormalDepthShaderMap);
	CLEAR_SHADER_CACHE(CubShadowShaderMap);
	CLEAR_SHADER_CACHE(VolumeShadowShaderMap);
	CLEAR_SHADER_CACHE(ShadowShaderMap);
	CLEAR_SHADER_CACHE(DualParaboloidShadowShaderMap);
	CLEAR_SHADER_CACHE(InnerVertexShaderMap);
	CLEAR_SHADER_CACHE(InnerPixelShaderMap);
	CLEAR_SHADER_CACHE(InnerGeometryShaderMap);
	GetASYNAnimSet().ClearAll();
	GetASYNTextureSet().ClearAll();
	GetASYNPostEffectSet().ClearAll();

	GetASYNActorSet().ClearAll();
	GetASYNMapSet().ClearAll();

	GetASYNFontSet().ClearAll();
	
	if (VSResourceManager::IsCacheVertexFormat())
	{
		CacheVertexFormat();
	}

	GetVertexFormatSet().ClearAll();

	GetBlendStateSet().ClearAll();
	GetDepthStencilStateSet().ClearAll();
	GetRasterizerStateSet().ClearAll();
	GetSamplerStateSet().ClearAll();

	if (VSResourceManager::IsCacheName())
	{
		CacheName();
	}

	GetNameSet().ClearAll();

	GetRenderTargetArray().ClearAll();
	GetDepthStencilArray().ClearAll();
	GetRenderTargetBufferArray().ClearAll();
	GetDepthStencilBufferArray().ClearAll();
	GetDVGeometryArray().ClearAll();
}
DECLEAR_TIME_PROFILENODE(GCTime,ApplicationUpdate)
void VSResourceManager::GC()
{
	ADD_TIME_PROFILE(GCTime)
	GetASYNTextureSet().GCResource();
	GetASYNAnimSet().GCResource();
	GetASYNMaterialSet().GCResource();
	GetASYNStaticMeshSet().GCResource();
	GetASYNSkelectonMeshSet().GCResource();
	GetASYNPostEffectSet().GCResource();
	GetASYNAnimTreeSet().GCResource();
	GetASYNMorphTreeSet().GCResource();
	GetASYNActorSet().GCResource();
	//GetASYNMapSet().GCResource();

	GetRenderTargetBufferArray().GCResource();
	GetDepthStencilBufferArray().GCResource();
	GCObject();
}
bool VSResourceManager::IsReleaseAll()
{
	if (!GetASYNStaticMeshSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetASYNSkelectonMeshSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetASYNAnimSet().IsRealeseAll())
	{
		return false;
	}
	
	if (!GetASYNTextureSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetASYNMaterialSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetASYNPostEffectSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetASYNAnimTreeSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetASYNMorphTreeSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetASYNActorSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetASYNMapSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetASYNFontSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetVertexFormatSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetMaterialShaderMap().IsRealeseAll())
	{
		return false;
	}
	if (!GetIndirectShaderMap().IsRealeseAll())
	{
		return false;
	}
	if (!GetVertexShaderMap().IsRealeseAll())
	{
		return false;
	}

	if (!GetPixelShaderMap().IsRealeseAll())
	{
		return false;
	}
	
	if (!GetInnerVertexShaderMap().IsRealeseAll())
	{
		return false;
	}

	if (!GetInnerPixelShaderMap().IsRealeseAll())
	{
		return false;
	}

	if (!GetNameSet().IsRealeseAll())
	{
		return false;
	}
	
	if (!GetBlendStateSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetSamplerStateSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetDepthStencilStateSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetRasterizerStateSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetDVGeometryArray().IsRealeseAll())
	{
		return false;
	}
	VSMAC_ASSERT(ms_pRootObject.GetNum() == 0);
	VSMAC_ASSERT(ms_pGCObject.GetNum() == 0);
	return true;
}
VSCaptureTexAllState * VSResourceManager::CreateCaptureTexture(const VSString & ViewFamilyName,unsigned int uiMipLevel)
{
	VSCaptureTexAllState * pCaptureTexture = VS_NEW VSCaptureTexAllState();
	pCaptureTexture->SetViewCapture(ViewFamilyName);

	pCaptureTexture->SetMipLevel(uiMipLevel);

	return pCaptureTexture;
}
VSTexAllState * VSResourceManager::Load2DTexture(const TCHAR *pFileName,VSSamplerStatePtr pSamplerState,bool bSRGB)
{
	
	if (!pFileName)
	{
		
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension; 

	if (!FileName.GetExtension(Extension))
	{
		
		
		return NULL;
	}
	VSTexAllState * pTexAllState = NULL;


	VSImage *pImage = NULL;

	if (Extension == VSImage::ms_ImageFormat[VSImage::IF_BMP])
	{
		pImage = VS_NEW VSBMPImage();
	}
	else if (Extension == VSImage::ms_ImageFormat[VSImage::IF_TGA])
	{
		pImage = VS_NEW VSTGAImage();
	}
	else 
	{
		
		return NULL;
	}
	if(!pImage->Load(FileName.GetBuffer()))
	{
		VSMAC_DELETE(pImage);
		
		return NULL;
	}

	unsigned int uiWidth = pImage->GetWidth();
	unsigned int uiHeight = pImage->GetHeight();
	if (!uiWidth || !uiHeight)
	{
		VSMAC_DELETE(pImage);
		
		return NULL;
	}
	if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight))
	{
		VSMAC_DELETE(pImage);
		
		return NULL;
	}
	VS2DTexture * pTexture = NULL;
	pTexture = VS_NEW VS2DTexture(uiWidth,uiHeight,VSRenderer::SFT_A8R8G8B8,0,1);
	if(!pTexture)
	{
		VSMAC_DELETE(pImage);
		
		return NULL;
	}
	pTexture->CreateRAMDate();
	for (unsigned int cy = 0; cy < uiHeight; cy++) 
	{
		for (unsigned int cx = 0; cx < uiWidth; cx++) 
		{

			unsigned uiIndex = cy * uiWidth + cx;
			unsigned char *pBuffer = pTexture->GetBuffer(0,uiIndex);
			const unsigned char * pImageBuffer = pImage->GetPixel(cx,cy);
			if (pImage->GetBPP() == 8)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[0];
				pBuffer[2] = pImageBuffer[0];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 24)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 32)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = pImageBuffer[3];
			}
		} // for
	} // for
	
	unsigned char * pLast = pTexture->GetBuffer(0);
	for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);

		if(!VSResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
		{
			VSMAC_DELETE(pTexture);
			VSMAC_DELETE(pImage);
			
			return NULL;
		}
		pLast = pTexture->GetBuffer(i);
	}
	if (pImage)
	{
		VSMAC_DELETE(pImage);
	}
	pTexAllState = VS_NEW VSTexAllState(pTexture);

	if (pSamplerState)
	{
		pTexAllState->SetSamplerState(pSamplerState);
	}
	
	pTexAllState->SetSRGBEable(bSRGB);
	return pTexAllState;
}
VSTexAllState * VSResourceManager::Load2DTextureCompress(const TCHAR *pFileName,VSSamplerStatePtr pSamplerState,
											 unsigned int uiCompressType,bool bIsNormal,bool bSRGB)
{
	if (bIsNormal)
	{
		bSRGB = false;
	}
	
	if (!pFileName)
	{
		
		return NULL;
	}
	
	if (uiCompressType > VSNVCompression::NV_MAX)
	{
		
		return NULL;
	}

	VSFileName FileName = pFileName;
	VSString Extension; 

	if (!FileName.GetExtension(Extension))
	{
		
		return NULL;
	}
	VSTexAllState * pTexAllState = NULL;

	VSImage *pImage = NULL;
	if (Extension == VSImage::ms_ImageFormat[VSImage::IF_BMP])
	{
		pImage = VS_NEW VSBMPImage();
	}
	else if (Extension == VSImage::ms_ImageFormat[VSImage::IF_TGA])
	{
		pImage = VS_NEW VSTGAImage();
	}
	else 
	{
		
		return NULL;
	}
	if(!pImage->Load(FileName.GetBuffer()))
	{
		VSMAC_DELETE(pImage);
		
		return NULL;
	}

	unsigned int uiWidth = pImage->GetWidth();
	unsigned int uiHeight = pImage->GetHeight();
	if (!uiWidth || !uiHeight)
	{
		VSMAC_DELETE(pImage);
		
		return NULL;
	}
	if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight))
	{
		VSMAC_DELETE(pImage);
		
		return NULL;
	}
	VS2DTexture * pTexture = NULL;
	pTexture = VS_NEW VS2DTexture(uiWidth, uiHeight, VSRenderer::SFT_A8R8G8B8, 0, 1);
	if(!pTexture)
	{
		VSMAC_DELETE(pImage);
		
		return NULL;
	}
	pTexture->CreateRAMDate();
	for (unsigned int cy = 0; cy < uiHeight; cy++) 
	{
		for (unsigned int cx = 0; cx < uiWidth; cx++) 
		{

			unsigned uiIndex = cy * uiWidth + cx;
			unsigned char *pBuffer = pTexture->GetBuffer(0,uiIndex);
			const unsigned char * pImageBuffer = pImage->GetPixel(cx,cy);
			if (pImage->GetBPP() == 8)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[0];
				pBuffer[2] = pImageBuffer[0];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 24)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 32)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = pImageBuffer[3];
			}
		} // for
	} // for

	unsigned char * pLast = pTexture->GetBuffer(0);
	for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);

		if(!VSResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
		{
			VSMAC_DELETE(pTexture);
			VSMAC_DELETE(pImage);
			
			return NULL;
		}
		pLast = pTexture->GetBuffer(i);
	}
	if (pImage)
	{
		VSMAC_DELETE(pImage);
	}

	unsigned int uiCompressFormat = VSNVCompression::NV_ARGB;
	unsigned int uiVSTextureTpye = VSRenderer::SFT_A8R8G8B8;
	if (uiCompressType == 0)
	{
		uiCompressFormat = VSNVCompression::NV_ARGB;
		uiVSTextureTpye = VSRenderer::SFT_A8R8G8B8;
	}
	else if (uiCompressType == 1)
	{
		uiCompressFormat = VSNVCompression::NV_DXT3;
		uiVSTextureTpye = VSRenderer::SFT_DXT3;
	}
	else if (uiCompressType == 2)
	{
		uiCompressFormat = VSNVCompression::NV_DXT5;
		uiVSTextureTpye = VSRenderer::SFT_DXT5;
	}
	VS2DTexture * pNewTexture = VS_NEW VS2DTexture(uiWidth,uiHeight,uiVSTextureTpye,0,1);
	pNewTexture->CreateRAMDate();
	for (unsigned int i = 0 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);
		VSNVCompression Help;
		Help.Setup(pNow,uiCompressFormat,pTexture->GetWidth(i),pTexture->GetHeight(i),false,bIsNormal);
		Help.Process();
		VSMemcpy(pNewTexture->GetBuffer(i),Help.OutputHandler.m_CompressDate.GetBuffer(),Help.OutputHandler.m_CompressDate.GetNum());
	}
	
	VSMAC_DELETE(pTexture);
	pTexAllState = VS_NEW VSTexAllState(pNewTexture);
	if (pSamplerState)
	{
		pTexAllState->SetSamplerState(pSamplerState);
	}
	pTexAllState->SetSRGBEable(bSRGB);
	return pTexAllState;
}

VSTexAllStateR * VSResourceManager::LoadASYN2DTexture(const TCHAR *pFileName,bool IsAsyn,VSSamplerStatePtr pSamplerState,bool bSRGB)
{
	if (!pFileName)
	{
		return NULL;
	}
	
	VSFileName FileName = pFileName;
	VSString Extension; 
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSImage::ms_ImageFormat[VSImage::IF_BMP] && Extension != VSImage::ms_ImageFormat[VSImage::IF_TGA])
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_TexturePath + FileName;
			
		}
	}
	else
	{
		return NULL;
	}
	ms_TextureCri.Lock();
	VSTexAllStateRPtr pTexAllState = NULL;
	pTexAllState = (VSTexAllStateR *)VSResourceManager::GetASYNTextureSet().CheckIsHaveTheResource(ResourceName);
	if(pTexAllState)
	{
		ms_TextureCri.Unlock();	
		return pTexAllState;
	}
	pTexAllState = VS_NEW VSTexAllStateR();

	pTexAllState->SetResourceName(ResourceName);


	VSResourceManager::GetASYNTextureSet().AddResource(ResourceName,pTexAllState);

	
	if (IsAsyn)
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddTextureLoad(pTexAllState,FileName,false,pSamplerState,false,0,false,bSRGB);
	}
	else
	{	
		VSTexAllState * pTex = Load2DTexture(FileName.GetBuffer(),pSamplerState,bSRGB);
		if (pTex)
		{
			pTexAllState->SetNewResource(pTex);
			pTexAllState->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);

		}
		
	}
	ms_TextureCri.Unlock();
	return pTexAllState;
}
//0 no compress 1 dx3  2 dx5
VSTexAllStateR * VSResourceManager::LoadASYN2DTextureCompress(const TCHAR *pFileName,bool IsAsyn,VSSamplerStatePtr pSamplerState,
												  unsigned int uiCompressType,bool bIsNormal,bool bSRGB)
{
	if (!pFileName)
	{
		return NULL;
	}
	if (uiCompressType > VSNVCompression::NV_MAX)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension; 
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSImage::ms_ImageFormat[VSImage::IF_BMP] && Extension != VSImage::ms_ImageFormat[VSImage::IF_TGA])
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_TexturePath + FileName;
			
		}
	}
	else
	{
		return NULL;
	}
	ms_TextureCri.Lock();
	VSTexAllStateRPtr pTexAllState = NULL;
	pTexAllState = (VSTexAllStateR *)VSResourceManager::GetASYNTextureSet().CheckIsHaveTheResource(ResourceName);
	if(pTexAllState)
	{
		ms_TextureCri.Unlock();
		return pTexAllState;
	}
	pTexAllState = VS_NEW VSTexAllStateR();

	pTexAllState->SetResourceName(ResourceName);


	VSResourceManager::GetASYNTextureSet().AddResource(ResourceName,pTexAllState);

	if (IsAsyn)
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddTextureLoad(pTexAllState,FileName,false,pSamplerState,true,uiCompressType,bIsNormal,bSRGB);
	}
	else
	{	
		VSTexAllState * pTex = Load2DTextureCompress(FileName.GetBuffer(),pSamplerState,uiCompressType,bIsNormal,bSRGB);
		if (pTex)
		{
			pTexAllState->SetNewResource(pTex);
			pTexAllState->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);

		}
		
	}
	ms_TextureCri.Unlock();
	return pTexAllState;
}
VSResourceProxyBase * VSResourceManager::LoadResource(const TCHAR *pFileName,bool IsAsyn)
{
	VSFileName FileName = pFileName;
	VSString Extension; 
	FileName.GetExtension(Extension);
	VSResourceProxyBase * pResource = NULL;
	if (Extension == VSResource::GetFileSuffix(VSResource::RT_TEXTURE))
	{
		pResource = LoadASYNTexture(pFileName,IsAsyn);
	}
	else if (Extension == VSResource::GetFileSuffix(VSResource::RT_MATERIAL))
	{
		pResource = LoadASYNMaterial(pFileName,IsAsyn);
	}
	else if (Extension == VSResource::GetFileSuffix(VSResource::RT_ACTION))
	{
		pResource = LoadASYNAction(pFileName,IsAsyn);
	}
	else if (Extension == VSResource::GetFileSuffix(VSResource::RT_STATIC_MODEL))
	{
		pResource = LoadASYNStaticMesh(pFileName,IsAsyn);
	}
	else if (Extension == VSResource::GetFileSuffix(VSResource::RT_SKELECTON_MODEL))
	{
		pResource = LoadASYNSkelectonMesh(pFileName,IsAsyn);
	}
	else if (Extension == VSResource::GetFileSuffix(VSResource::RT_POSTEFFECT))
	{
		pResource = LoadASYNPostEffect(pFileName,IsAsyn);
	}
	else if (Extension == VSResource::GetFileSuffix(VSResource::RT_ANIMTREE))
	{
		pResource = LoadASYNAnimTree(pFileName, IsAsyn);
	}
	else if (Extension == VSResource::GetFileSuffix(VSResource::RT_MORPHTREE))
	{
		pResource = LoadASYNMorphTree(pFileName, IsAsyn);
	}
	else if (Extension == VSResource::GetFileSuffix(VSResource::RT_ACTOR))
	{
		pResource = LoadASYNActor(pFileName, IsAsyn);
	}
	return pResource;
}
VSTexAllStateR * VSResourceManager::LoadASYNTexture(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension; 
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_TEXTURE))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_TexturePath + FileName;
			
		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_TEXTURE);
		FileName =  ms_TexturePath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_TEXTURE);
		
	}
	ms_TextureCri.Lock();
	VSTexAllStateRPtr pTexAllState = NULL;
	pTexAllState = (VSTexAllStateR *)VSResourceManager::GetASYNTextureSet().CheckIsHaveTheResource(ResourceName);
	if(pTexAllState)
	{
		ms_TextureCri.Unlock();
		return pTexAllState;
	}
	pTexAllState = VS_NEW VSTexAllStateR();

	pTexAllState->SetResourceName(ResourceName);


	VSResourceManager::GetASYNTextureSet().AddResource(ResourceName,pTexAllState);

	
	if (IsAsyn && ms_EnableAsynLoad && pTexAllState->IsEndableASYNLoad())
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddTextureLoad(pTexAllState,FileName);
	}
	else
	{	
		VSTexAllState * pTex = NewLoadTexture(FileName.GetBuffer());
		if (pTex)
		{
			pTexAllState->SetNewResource(pTex);
			pTexAllState->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);
		}
		
	}
	ms_TextureCri.Unlock();
	return pTexAllState;
	
}
VSStaticMeshNodeR * VSResourceManager::LoadASYNStaticMesh(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension; 
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_STATIC_MODEL))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_StaticMeshPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_STATIC_MODEL);
		FileName =  ms_StaticMeshPath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_STATIC_MODEL);

	}
	ms_StaticMeshCri.Lock();
	VSStaticMeshNodeRPtr pStaticMeshNodeR = NULL;
	pStaticMeshNodeR = (VSStaticMeshNodeR *)VSResourceManager::GetASYNStaticMeshSet().CheckIsHaveTheResource(ResourceName);
	if(pStaticMeshNodeR)
	{
		ms_StaticMeshCri.Unlock();	
		return pStaticMeshNodeR;
	}
	pStaticMeshNodeR = VS_NEW VSStaticMeshNodeR();

	pStaticMeshNodeR->SetResourceName(ResourceName);


	VSResourceManager::GetASYNStaticMeshSet().AddResource(ResourceName,pStaticMeshNodeR);


	if (IsAsyn && ms_EnableAsynLoad && pStaticMeshNodeR->IsEndableASYNLoad())
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddStaticMesh(pStaticMeshNodeR,FileName);
	}
	else
	{	
		VSStaticMeshNode * pStaticMeshNode = NewLoadStaticMesh(FileName.GetBuffer());
		if(pStaticMeshNode)
		{
			pStaticMeshNodeR->SetNewResource(pStaticMeshNode);
			pStaticMeshNodeR->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);
		}
	}
	ms_StaticMeshCri.Unlock();	
	return pStaticMeshNodeR;
}
VSSkelectonMeshNodeR * VSResourceManager::LoadASYNSkelectonMesh(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension; 
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_SKELECTON_MODEL))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_SkelectionMeshPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_SKELECTON_MODEL);
		FileName =  ms_SkelectionMeshPath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_SKELECTON_MODEL);

	}
	ms_SkeletonMeshCri.Lock();
	VSSkelectonMeshNodeRPtr pSkelectonMeshNodeR = NULL;
	pSkelectonMeshNodeR = (VSSkelectonMeshNodeR *)VSResourceManager::GetASYNSkelectonMeshSet().CheckIsHaveTheResource(ResourceName);
	if(pSkelectonMeshNodeR)
	{
		ms_SkeletonMeshCri.Unlock();
		return pSkelectonMeshNodeR;
	}
	pSkelectonMeshNodeR = VS_NEW VSSkelectonMeshNodeR();

	pSkelectonMeshNodeR->SetResourceName(ResourceName);


	VSResourceManager::GetASYNSkelectonMeshSet().AddResource(ResourceName,pSkelectonMeshNodeR);


	if (IsAsyn && ms_EnableAsynLoad && pSkelectonMeshNodeR->IsEndableASYNLoad())
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddSkelectonMesh(pSkelectonMeshNodeR,FileName);
	}
	else
	{	
		VSSkelectonMeshNode * pSkelectonMesh = NewLoadSkelectonMesh(FileName.GetBuffer());
		if (pSkelectonMesh)
		{
			pSkelectonMeshNodeR->SetNewResource(pSkelectonMesh);
			pSkelectonMeshNodeR->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);
		}
		}
	ms_SkeletonMeshCri.Unlock();
	return pSkelectonMeshNodeR;
}
VSPostEffectSetR * VSResourceManager::LoadASYNPostEffect(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension; 
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_POSTEFFECT))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_PostEffectPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_POSTEFFECT);
		FileName =  ms_PostEffectPath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_POSTEFFECT);

	}
	ms_PostEffectCri.Lock();
	VSPostEffectSetRPtr pPostEffectSetR = NULL;
	pPostEffectSetR = (VSPostEffectSetR *)VSResourceManager::GetASYNPostEffectSet().CheckIsHaveTheResource(ResourceName);
	if(pPostEffectSetR)
	{
		ms_PostEffectCri.Unlock();
		return pPostEffectSetR;
	}
	pPostEffectSetR = VS_NEW VSPostEffectSetR();

	pPostEffectSetR->SetResourceName(ResourceName);


	VSResourceManager::GetASYNPostEffectSet().AddResource(ResourceName,pPostEffectSetR);


	if (IsAsyn && ms_EnableAsynLoad && pPostEffectSetR->IsEndableASYNLoad())
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		//VSASYNLoadManager::ms_pASYNLoadManager->AddSkelectonMesh(pPostEffectSetR,FileName);
	}
	else
	{	
		VSPostEffectSet * pPostEffectSet = NewLoadPostEffect(FileName.GetBuffer());
		if (pPostEffectSet)
		{
			pPostEffectSetR->SetNewResource(pPostEffectSet);
			pPostEffectSetR->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);
		}
	}
	ms_PostEffectCri.Unlock();
	return pPostEffectSetR;
}
VSAnimR * VSResourceManager::LoadASYNAction(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension; 
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_ACTION))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_AnimPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_ACTION);
		FileName =  ms_AnimPath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_ACTION);

	}
	ms_AnimCri.Lock();
	VSAnimRPtr pAnimR = NULL;
	pAnimR = (VSAnimR *)VSResourceManager::GetASYNAnimSet().CheckIsHaveTheResource(ResourceName);
	if(pAnimR)
	{
		ms_AnimCri.Unlock();
		return pAnimR;
	}
	pAnimR = VS_NEW VSAnimR();

	pAnimR->SetResourceName(ResourceName);


	VSResourceManager::GetASYNAnimSet().AddResource(ResourceName,pAnimR);


	if (IsAsyn && ms_EnableAsynLoad && pAnimR->IsEndableASYNLoad() )
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddAnim(pAnimR,FileName);
	}
	else
	{	
		VSAnim * pAnim = NewLoadAction(FileName.GetBuffer());
		if (pAnim)
		{
			pAnimR->SetNewResource(pAnim);
			pAnimR->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);
		}
		
	}
	ms_AnimCri.Unlock();
	return pAnimR;
}
VSAnimTreeR * VSResourceManager::LoadASYNAnimTree(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension; 
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_ANIMTREE))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_AnimPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_ANIMTREE);
		FileName =  ms_AnimPath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_ANIMTREE);

	}
	ms_AnimTreeCri.Lock();
	VSAnimTreeRPtr pAnimTreeR = NULL;
	pAnimTreeR = (VSAnimTreeR *)VSResourceManager::GetASYNAnimTreeSet().CheckIsHaveTheResource(ResourceName);
	if(pAnimTreeR)
	{
		ms_AnimTreeCri.Unlock();
		return pAnimTreeR;
	}
	pAnimTreeR = VS_NEW VSAnimTreeR();

	pAnimTreeR->SetResourceName(ResourceName);


	VSResourceManager::GetASYNAnimTreeSet().AddResource(ResourceName,pAnimTreeR);


	if (IsAsyn && ms_EnableAsynLoad && pAnimTreeR->IsEndableASYNLoad())
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddAnimTree(pAnimTreeR,FileName);
	}
	else
	{	
		VSAnimTree * pAnimTree = NewLoadAnimTree(FileName.GetBuffer());
		if (pAnimTree)
		{
			pAnimTreeR->SetNewResource(pAnimTree);
			pAnimTreeR->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);

		}

	}
	ms_AnimTreeCri.Unlock();
	return pAnimTreeR;
}
void VSResourceManager::DeleteAllMapResource()
{
	ms_MapCri.Lock();
	for (unsigned int i = 0; i < VSResourceManager::GetASYNMapSet().GetResourceNum(); )
	{
		VSSceneMapRPtr pMapR = VSResourceManager::GetASYNMapSet().GetResource(i)->Value;
		if (pMapR->IsLoaded())
		{
			if (VSWorld::ms_pWorld)
			{
				VSWorld::ms_pWorld->DestroyScene(pMapR->GetResource()->m_Name);
			}
		}
		else
		{
			VSFileName FileName = ms_MapPath + pMapR->GetResourceName().GetString();
			VSASYNLoadManager::ms_pASYNLoadManager->DeleteLoadResource(FileName);
		}
		VSResourceManager::GetASYNMapSet().DeleteResource(pMapR->GetResourceName());
	}
	
	ms_MapCri.Unlock();
}
void VSResourceManager::DeleteMapResource(const TCHAR *  pFileName)
{
	VSFileName FileName = pFileName;
	VSString Extension;
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_MAP))
		{
			return;
		}
		else
		{
			ResourceName = FileName;
			FileName = ms_MapPath + FileName;
		}
	}
	else
	{
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_MAP);
		FileName = ms_MapPath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_MAP);
	}

	ms_MapCri.Lock();
	VSSceneMapRPtr pMapR = (VSSceneMapR *)VSResourceManager::GetASYNMapSet().CheckIsHaveTheResource(ResourceName);
	if (pMapR == NULL)
	{
		return;
	}
	if (pMapR->IsLoaded())
	{
		if (VSWorld::ms_pWorld)
		{
			VSWorld::ms_pWorld->DestroyScene(pMapR->GetResource()->m_Name);
		}	
	}
	else
	{
		VSASYNLoadManager::ms_pASYNLoadManager->DeleteLoadResource(FileName);
	}	
	VSResourceManager::GetASYNMapSet().DeleteResource(ResourceName);
	ms_MapCri.Unlock();

}
VSFontR * VSResourceManager::LoadASYNFont(const TCHAR *pFileName, bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension;
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_FONTTYPE))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName = ms_FontPath + FileName;

		}
	}
	else
	{
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_FONTTYPE);
		FileName = ms_FontPath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_FONTTYPE);

	}
	ms_FontCri.Lock();
	VSFontRPtr pFontR = NULL;
	pFontR = (VSFontR *)VSResourceManager::GetASYNFontSet().CheckIsHaveTheResource(ResourceName);
	if (pFontR)
	{
		ms_FontCri.Unlock();
		//Have Map Return NULL;
		return NULL;
	}
	pFontR = VS_NEW VSFontR();

	pFontR->SetResourceName(ResourceName);


	VSResourceManager::GetASYNFontSet().AddResource(ResourceName, pFontR);


	if (IsAsyn && ms_EnableAsynLoad && pFontR->IsEndableASYNLoad())
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddFont(pFontR, FileName);
	}
	else
	{
		VSFont * PFont = NewLoadFont(FileName.GetBuffer());
		if (PFont)
		{
			pFontR->SetNewResource(PFont);
			pFontR->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);

		}

	}
	ms_FontCri.Unlock();
	return pFontR;
}
VSSceneMapR * VSResourceManager::LoadASYNMap(const TCHAR *pFileName, bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension;
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_MAP))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName = ms_MapPath + FileName;

		}
	}
	else
	{
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_MAP);
		FileName = ms_MapPath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_MAP);

	}
	ms_MapCri.Lock();
	VSSceneMapRPtr pMapR = NULL;
	pMapR = (VSSceneMapR *)VSResourceManager::GetASYNMapSet().CheckIsHaveTheResource(ResourceName);
	if (pMapR)
	{
		ms_MapCri.Unlock();
		//Have Map Return NULL;
		return NULL;
	}
	pMapR = VS_NEW VSSceneMapR();

	pMapR->SetResourceName(ResourceName);


	VSResourceManager::GetASYNMapSet().AddResource(ResourceName, pMapR);


	if (IsAsyn && ms_EnableAsynLoad && pMapR->IsEndableASYNLoad())
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddMap(pMapR, FileName);
	}
	else
	{
		VSSceneMap * pMap = NewLoadMap(FileName.GetBuffer());
		if (pMap)
		{
			pMapR->SetNewResource(pMap);
			pMapR->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);

		}

	}
	ms_MapCri.Unlock();
	return pMapR;
}
VSActorR * VSResourceManager::LoadASYNActor(const TCHAR *pFileName, bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension;
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_ACTOR))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName = ms_ActorPath + FileName;

		}
	}
	else
	{
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_ACTOR);
		FileName = ms_ActorPath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_ACTOR);

	}
	ms_ActorCri.Lock();
	VSActorRPtr pActorR = NULL;
	pActorR = (VSActorR *)VSResourceManager::GetASYNActorSet().CheckIsHaveTheResource(ResourceName);
	if (pActorR)
	{
		ms_ActorCri.Unlock();
		return pActorR;
	}
	pActorR = VS_NEW VSActorR();

	pActorR->SetResourceName(ResourceName);


	VSResourceManager::GetASYNActorSet().AddResource(ResourceName, pActorR);


	if (IsAsyn && ms_EnableAsynLoad && pActorR->IsEndableASYNLoad())
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddActor(pActorR, FileName);
	}
	else
	{
		VSActor * pActor = NewLoadActor(FileName.GetBuffer());
		if (pActor)
		{
			pActorR->SetNewResource(pActor);
			pActorR->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);

		}

	}
	ms_ActorCri.Unlock();
	return pActorR;
}
VSMorphTreeR * VSResourceManager::LoadASYNMorphTree(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension; 
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_MORPHTREE))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName = ms_MorphPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_MORPHTREE);
		FileName = ms_MorphPath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_MORPHTREE);

	}
	ms_MorphTreeCri.Lock();
	VSMorphTreeRPtr pMorphTreeR = NULL;
	pMorphTreeR = (VSMorphTreeR *)VSResourceManager::GetASYNMorphTreeSet().CheckIsHaveTheResource(ResourceName);
	if(pMorphTreeR)
	{
		ms_MorphTreeCri.Unlock();
		return pMorphTreeR;
	}
	pMorphTreeR = VS_NEW VSMorphTreeR();

	pMorphTreeR->SetResourceName(ResourceName);


	VSResourceManager::GetASYNMorphTreeSet().AddResource(ResourceName,pMorphTreeR);


	if (IsAsyn && ms_EnableAsynLoad && pMorphTreeR->IsEndableASYNLoad())
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddMorphTree(pMorphTreeR,FileName);
	}
	else
	{	
		VSMorphTree * pMorphTree = NewLoadMorphTree(FileName.GetBuffer());
		if (pMorphTree)
		{
			pMorphTreeR->SetNewResource(pMorphTree);
			pMorphTreeR->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);

		}

	}
	ms_MorphTreeCri.Unlock();
	return pMorphTreeR;
}
VSMaterialR * VSResourceManager::LoadASYNMaterial(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSFileName FileName = pFileName;
	VSString Extension; 
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSResource::GetFileSuffix(VSResource::RT_MATERIAL))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_MaterialPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_MATERIAL);
		FileName =  ms_MaterialPath + FileName + _T(".") + VSResource::GetFileSuffix(VSResource::RT_MATERIAL);

	}
	ms_MaterialCri.Lock();
	VSMaterialRPtr pMaterialR = NULL;
	pMaterialR = (VSMaterialR *)VSResourceManager::GetASYNMaterialSet().CheckIsHaveTheResource(ResourceName);
	if(pMaterialR)
	{
		ms_MaterialCri.Unlock();
		return pMaterialR;
	}
	pMaterialR = VS_NEW VSMaterialR();

	pMaterialR->SetResourceName(ResourceName);


	VSResourceManager::GetASYNMaterialSet().AddResource(ResourceName,pMaterialR);


	if (IsAsyn && ms_EnableAsynLoad && pMaterialR->IsEndableASYNLoad())
	{
		VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
		VSASYNLoadManager::ms_pASYNLoadManager->AddMaterial(pMaterialR,FileName);
	}
	else
	{	
		VSMaterial * pMaterial = NewLoadMaterial(FileName.GetBuffer());
		if (pMaterial)
		{
			pMaterialR->SetNewResource(pMaterial);
			pMaterialR->Loaded();
		}
		else
		{
			VSMAC_ASSERT(0);

		}
		
	}
	ms_MaterialCri.Unlock();
	return pMaterialR;
}
VSTexAllState * VSResourceManager::NewLoadTexture(const TCHAR *pFileName)
{
	
	if (!pFileName)
	{
		
		return NULL;
	}
	VSStream LoadStream;
	LoadStream.NewLoad(pFileName);
	VSTexAllState * pTexAllState = (VSTexAllState *)LoadStream.GetObjectByRtti(VSTexAllState::ms_Type);
	if (!pTexAllState)
	{		
		return NULL;
	}
	if(!pTexAllState->m_pTex)
	{
		VSMAC_DELETE(pTexAllState);
		
		return NULL;
	}
	return pTexAllState;

}

bool VSResourceManager::NewSaveTexture(VSTexAllState * pTexture, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pTexture || !pTexture->m_pTex || !pTexture->m_pTex->IsStatic())
	{
		return false;
	}

	VSStream SaveStream;
	SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
	SaveStream.ArchiveAll(pTexture);
	VSString FileName = ms_TexturePath + PathName;
	FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_TEXTURE);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}

VSMaterial * VSResourceManager::NewLoadMaterial(const TCHAR *pFileName)
{
	
	if (!pFileName)
	{
		
		return NULL;
	}
	VSStream LoadStream;
	LoadStream.NewLoad(pFileName);
	VSMaterial * pMaterial = (VSMaterial *)LoadStream.GetObjectByRtti(VSMaterial::ms_Type);
	if (!pMaterial)
	{
		return NULL;
	}
	return pMaterial;
}
bool VSResourceManager::NewSaveMaterial(VSMaterial * pMaterial, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pMaterial)
	{
		return false;
	}

	VSStream SaveStream;
	SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
	SaveStream.ArchiveAll(pMaterial);
	VSString FileName = ms_MaterialPath + PathName;
	FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_MATERIAL);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}

VSStaticMeshNode * VSResourceManager::NewLoadStaticMesh(const TCHAR *pFileName)
{
	
	if (!pFileName)
	{
		
		return NULL;
	}
	VSStream LoadStream;
	LoadStream.NewLoad(pFileName);
	VSStaticMeshNode * pStaticMesh = (VSStaticMeshNode *)LoadStream.GetObjectByRtti(VSStaticMeshNode::ms_Type);
	if (!pStaticMesh)
	{
		return NULL;
	}
	return pStaticMesh;
}
bool VSResourceManager::NewSaveStaticMesh(VSStaticMeshNode * pStaticMeshNode, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pStaticMeshNode)
	{
		return false;
	}

	VSStream SaveStream;
	SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
	SaveStream.ArchiveAll(pStaticMeshNode);
	VSString FileName = ms_StaticMeshPath + PathName;
	FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_STATIC_MODEL);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
VSPostEffectSet * VSResourceManager::NewLoadPostEffect(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}
	VSStream LoadStream;
	LoadStream.NewLoad(pFileName);
	VSPostEffectSet * pPostEffectSet = (VSPostEffectSet *)LoadStream.GetObjectByRtti(VSPostEffectSet::ms_Type);
	if (!pPostEffectSet)
	{
		return NULL;
	}
	return pPostEffectSet;
}
bool VSResourceManager::NewSaveAnimTree(VSAnimTree * pAnim, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pAnim)
	{
		return false;
	}
	VSStream SaveStream;
	SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
	SaveStream.ArchiveAll(pAnim);
	VSString FileName = ms_AnimPath + PathName;
	FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_ANIMTREE);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
VSAnimTree * VSResourceManager::NewLoadAnimTree(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	VSStream LoadStream;
	LoadStream.NewLoad(pFileName);
	VSAnimTree * pAnim = (VSAnimTree *)LoadStream.GetObjectByRtti(VSAnimTree::ms_Type);
	if (!pAnim)
	{
		return NULL;
	}
	return pAnim;
}
bool VSResourceManager::NewSaveActor(VSActor * pActor, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pActor)
	{
		return false;
	}
	VSStream SaveStream;
	SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
	SaveStream.ArchiveAll(pActor);
	VSString FileName = ms_ActorPath + PathName;
	FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_ACTOR);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
VSActor * VSResourceManager::NewLoadActor(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	VSStream LoadStream;
	LoadStream.NewLoad(pFileName);
	VSActor * pActor = (VSActor *)LoadStream.GetObjectByRtti(VSActor::ms_Type);
	if (!pActor)
	{
		return NULL;
	}
	return pActor;
}
bool VSResourceManager::NewSaveMap(VSSceneMap * pMap, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pMap)
	{
		return false;
	}
	VSStream SaveStream;
	SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
	SaveStream.ArchiveAll(pMap);
	VSString FileName = ms_MapPath + PathName;
	FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_MAP);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
VSSceneMap * VSResourceManager::NewLoadMap(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	VSStream LoadStream;
	LoadStream.NewLoad(pFileName);
	VSSceneMap * pMap = (VSSceneMap *)LoadStream.GetObjectByRtti(VSSceneMap::ms_Type);
	if (!pMap)
	{
		return NULL;
	}
	return pMap;
}
bool VSResourceManager::NewSaveFont(VSFont * pFont, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pFont)
	{
		return false;
	}
	VSStream SaveStream;
	SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
	SaveStream.ArchiveAll(pFont);
	VSString FileName = ms_FontPath + PathName;
	FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_FONTTYPE);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
VSFont * VSResourceManager::NewLoadFont(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	VSStream LoadStream;
	LoadStream.NewLoad(pFileName);
	VSFont * pFont = (VSFont *)LoadStream.GetObjectByRtti(VSFont::ms_Type);
	if (!pFont)
	{
		return NULL;
	}
	return pFont;
}
bool VSResourceManager::NewSaveMorphTree(VSMorphTree * pMorphTree, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pMorphTree)
	{
		return false;
	}
	VSStream SaveStream;
	SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
	SaveStream.ArchiveAll(pMorphTree);
	VSString FileName = ms_MorphPath + PathName;
	FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_MORPHTREE);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
VSMorphTree * VSResourceManager::NewLoadMorphTree(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	VSStream LoadStream;
	LoadStream.NewLoad(pFileName);
	VSMorphTree * pMorphTree = (VSMorphTree *)LoadStream.GetObjectByRtti(VSMorphTree::ms_Type);
	if (!pMorphTree)
	{
		return NULL;
	}
	return pMorphTree;
}
VSAnim * VSResourceManager::NewLoadAction(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	VSStream LoadStream;
	LoadStream.NewLoad(pFileName);
	VSAnim * pAnim = (VSAnim *)LoadStream.GetObjectByRtti(VSAnim::ms_Type);
	if (!pAnim)
	{
		return NULL;
	}
	return pAnim;
}
bool VSResourceManager::NewSaveAction(VSAnim * pAnim, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pAnim)
	{
		return false;
	}
	VSStream SaveStream;
	SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
	SaveStream.ArchiveAll(pAnim);
 	VSString FileName = ms_AnimPath + PathName;
 	FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_ACTION);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
bool VSResourceManager::NewSavePostEffect(VSPostEffectSet * pPostEffectSet, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pPostEffectSet)
	{
		return false;
	}

	VSStream SaveStream;
	SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
	SaveStream.ArchiveAll(pPostEffectSet);
	VSString FileName = ms_PostEffectPath + PathName;
	FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_POSTEFFECT);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
VSSkelectonMeshNode * VSResourceManager::NewLoadSkelectonMesh(const TCHAR *pFileName)
{
	
	if (!pFileName)
	{
		
		return NULL;
	}
	VSStream LoadStream;
	LoadStream.NewLoad(pFileName);
	VSSkelectonMeshNode * pSkelecton = (VSSkelectonMeshNode *)LoadStream.GetObjectByRtti(VSSkelectonMeshNode::ms_Type);
	if (!pSkelecton)
	{		
		return NULL;
	}
	return pSkelecton;
}
bool VSResourceManager::NewSaveSkelectonMeshNode(VSSkelectonMeshNode * pSkelectonMesh, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pSkelectonMesh)
	{
		return false;
	}

	VSStream SaveStream;
	SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
	SaveStream.ArchiveAll(pSkelectonMesh);
 	VSString FileName = ms_SkelectionMeshPath + PathName;
 	FileName += _T(".") + VSResource::GetFileSuffix(VSResource::RT_SKELECTON_MODEL);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}

VSVertexFormat *VSResourceManager::LoadVertexFormat(VSVertexBuffer * pVertexBuffer,VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> *pFormatArray)
{
	if(!pVertexBuffer && !pFormatArray)
		return NULL;
	if (pVertexBuffer)
	{
		if(pVertexBuffer->m_pVertexFormat)
			return pVertexBuffer->m_pVertexFormat;
	}
	
	VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> FormatArray;
	if (!pFormatArray)
	{	
		if(!pVertexBuffer->GetVertexFormat(FormatArray))
			return	NULL;
		pFormatArray = &FormatArray;
	}
	else
	{
		if (!pFormatArray->GetNum())
		{
			return	NULL;
		}
	}

	unsigned int lVertexFormatCode = CRC32Compute(pFormatArray->GetBuffer(), sizeof(VSVertexFormat::VERTEXFORMAT_TYPE) * pFormatArray->GetNum());
	
	VSVertexFormat * pVertexFormat = NULL;
	pVertexFormat = (VSVertexFormat *)VSResourceManager::GetVertexFormatSet().CheckIsHaveTheResource(lVertexFormatCode);
	if(pVertexFormat)
	{
		if (pVertexBuffer)
		{
			pVertexBuffer->m_pVertexFormat = pVertexFormat;
		}
		return pVertexFormat;
	}
	pVertexFormat = VS_NEW VSVertexFormat();

	VSResourceManager::GetVertexFormatSet().AddResource(lVertexFormatCode,pVertexFormat);
	if (pVertexBuffer)
	{
		pVertexBuffer->m_pVertexFormat = pVertexFormat;
	}
	pVertexFormat->m_FormatArray = *pFormatArray;
	pVertexFormat->m_uiVertexFormatCode = lVertexFormatCode;
	pVertexFormat->LoadResource(VSRenderer::ms_pRenderer);
	return pVertexFormat;

}
VSName * VSResourceManager::CreateName(const TCHAR * pChar)
{
	if (!pChar)
	{
		return NULL;
	}
	ms_NameCri.Lock();
	unsigned int uiCRCCode  = CRC32Compute(pChar,(unsigned int)VSStrLen(pChar));
	VSName * pName = NULL;
	pName = VSResourceManager::GetNameSet().CheckIsHaveTheResource(uiCRCCode);
	if (!pName)
	{
		pName = VS_NEW VSName(pChar,uiCRCCode);
		if (!pName)
		{
			ms_NameCri.Unlock();
			return NULL;
		}

		VSResourceManager::GetNameSet().AddResource(pName->m_uiID,pName);
	}
	ms_NameCri.Unlock();
	return pName;
}
VSName * VSResourceManager::CreateName(const VSString & String)
{

	ms_NameCri.Lock();
	unsigned int uiCRCCode  = CRC32Compute(String.GetBuffer(),String.GetLength());
	VSName * pName = NULL;
	pName = VSResourceManager::GetNameSet().CheckIsHaveTheResource(uiCRCCode);
	if (!pName)
	{
		pName = VS_NEW VSName(String,uiCRCCode);
		if (!pName)
		{
			ms_NameCri.Unlock();
			return NULL;
		}

		VSResourceManager::GetNameSet().AddResource(pName->m_uiID,pName);
	}
	ms_NameCri.Unlock();
	return pName;
}
VSVShader * VSResourceManager::CreateVShader(MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID)
{
	ms_VShaderCri.Lock();
	if (!MSPara.pGeometry || !MSPara.pMaterialInstance)
	{
		ms_VShaderCri.Unlock();
		return NULL;
	}
	
	bool bCreateShaderString = false;
	VSVShader * pVShader = NULL;
	if (MSPara.m_VSShaderPath.GetLength())
	{
		if (!MSPara.m_VMainFunName.GetLength())
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		else
		{
			pVShader = VS_NEW VSVShader(MSPara.m_VSShaderPath.GetBuffer(),MSPara.m_VMainFunName.GetBuffer(),true);
		}
	}
	else
	{	
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		if (!pMaterial)
		{
			ms_VShaderCri.Unlock();
			return NULL;
		}
		pVShader = VS_NEW VSVShader();
		bCreateShaderString = true;
	}

	if(!pVShader)
	{
		ms_VShaderCri.Unlock();
		return NULL;
	}
	if (bCreateShaderString)
	{
		VSString VShaderString;
	#ifdef _DEBUG
		VSString ShaderFileName;
		if(!VSShaderStringFactory::CreateVShaderString(pVShader,MSPara,uiPassType,uiShaderID,VShaderString,ShaderFileName))
	#else
		if(!VSShaderStringFactory::CreateVShaderString(pVShader,MSPara,uiPassType,uiShaderID,VShaderString))
	#endif
		{
			ms_VShaderCri.Unlock();
			VSMAC_DELETE(pVShader);
			return NULL;
		}
	#ifdef _DEBUG
		pVShader->SetResourceName(ShaderFileName);
	#endif
		pVShader->SetShaderString(VShaderString,VSRenderer::GetVShaderProgramMain());
	}
	VSShaderKey::SetMaterialVShaderKey(&pVShader->m_ShaderKey,MSPara,uiPassType);
	VSRenderer::ms_pRenderer->LoadVShaderProgram(pVShader);
	ms_VShaderCri.Unlock();
	return pVShader;
}
VSGShader * VSResourceManager::CreateGShader(MaterialShaderPara &MSPara, unsigned int uiPassType, unsigned int uiShaderID)
{
	ms_GShaderCri.Lock();
	if (!MSPara.pGeometry || !MSPara.pMaterialInstance)
	{
		ms_GShaderCri.Unlock();
		return NULL;
	}

	bool bCreateShaderString = false;
	VSGShader * pGShader = NULL;
	if (MSPara.m_GSShaderPath.GetLength())
	{
		if (!MSPara.m_GMainFunName.GetLength())
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		else
		{
			pGShader = VS_NEW VSGShader(MSPara.m_GSShaderPath.GetBuffer(), MSPara.m_GMainFunName.GetBuffer(), true);
		}
	}
	else
	{
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		if (!pMaterial)
		{
			ms_GShaderCri.Unlock();
			return NULL;
		}
		pGShader = VS_NEW VSGShader();
		bCreateShaderString = true;
	}

	if (!pGShader)
	{
		ms_GShaderCri.Unlock();
		return NULL;
	}

	if (bCreateShaderString)
	{
		VSString GShaderString;
#ifdef _DEBUG
		VSString ShaderFileName;
		if (!VSShaderStringFactory::CreateGShaderString(pGShader, MSPara, uiPassType, uiShaderID, GShaderString, ShaderFileName))
#else
		if (!VSShaderStringFactory::CreateGShaderString(pGShader, MSPara, uiPassType, uiShaderID, GShaderString))
#endif
		{
			ms_GShaderCri.Unlock();
			VSMAC_DELETE(pGShader);
			return NULL;
		}
#ifdef _DEBUG
		pGShader->SetResourceName(ShaderFileName);
#endif
		pGShader->SetShaderString(GShaderString, VSRenderer::GetGShaderProgramMain());
	}


	VSShaderKey::SetMaterialGShaderKey(&pGShader->m_ShaderKey, MSPara, uiPassType);
	VSRenderer::ms_pRenderer->LoadGShaderProgram(pGShader);
	ms_GShaderCri.Unlock();
	return pGShader;
}
VSPShader * VSResourceManager::CreatePShader(MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID)
{
	ms_PShaderCri.Lock();
	if (!MSPara.pGeometry || !MSPara.pMaterialInstance)
	{
		ms_PShaderCri.Unlock();
		return NULL;
	}
	
	bool bCreateShaderString = false;
	VSPShader * pPShader = NULL;
	if (MSPara.m_PSShaderPath.GetLength())
	{
		if (!MSPara.m_PMainFunName.GetLength())
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		else
		{
			pPShader = VS_NEW VSPShader(MSPara.m_PSShaderPath.GetBuffer(),MSPara.m_PMainFunName.GetBuffer(),true);
		}
	}
	else
	{
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		if (!pMaterial)
		{
			ms_PShaderCri.Unlock();
			return NULL;
		}
		pPShader = VS_NEW VSPShader();
		bCreateShaderString = true;
	}
	
	if(!pPShader)
	{
		ms_PShaderCri.Unlock();
		return NULL;
	}

	if (bCreateShaderString)
	{
		VSString PShaderString;
#ifdef _DEBUG
		VSString ShaderFileName;
		if (!VSShaderStringFactory::CreatePShaderString(pPShader,MSPara,uiPassType,uiShaderID,PShaderString,ShaderFileName))
#else
		if (!VSShaderStringFactory::CreatePShaderString(pPShader,MSPara,uiPassType,uiShaderID,PShaderString))
#endif
		{
			ms_PShaderCri.Unlock();
			VSMAC_DELETE(pPShader);
			return NULL;
		}
#ifdef _DEBUG
		pPShader->SetResourceName(ShaderFileName);
#endif
		pPShader->SetShaderString(PShaderString,VSRenderer::GetPShaderProgramMain());
	}
	
	
	VSShaderKey::SetMaterialPShaderKey(&pPShader->m_ShaderKey,MSPara,uiPassType);
	VSRenderer::ms_pRenderer->LoadPShaderProgram(pPShader);
	ms_PShaderCri.Unlock();
	return pPShader;
}


VSBlendState * VSResourceManager::CreateBlendState(const VSBlendDesc & BlendDesc)
{
	unsigned int uiDataSize = 0;
	void * pData = BlendDesc.GetCRC32Data(uiDataSize);
	unsigned int uiHashCode = CRC32Compute(pData, uiDataSize);

	VSBlendState * pBlendState = NULL;
	pBlendState = (VSBlendState *)VSResourceManager::GetBlendStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pBlendState)
	{

		return pBlendState;
	}
	pBlendState = VS_NEW VSBlendState();
	pBlendState->m_BlendDesc = BlendDesc;
	VSResourceManager::GetBlendStateSet().AddResource(uiHashCode,pBlendState);
	return pBlendState;
}
VSDepthStencilState * VSResourceManager::CreateDepthStencilState(const VSDepthStencilDesc & DepthStencilDesc)
{
	unsigned int uiDataSize = 0;
	void * pData = DepthStencilDesc.GetCRC32Data(uiDataSize);
	unsigned int uiHashCode = CRC32Compute(pData, uiDataSize);

	VSDepthStencilState * pDepthStencilState = NULL;
	pDepthStencilState = (VSDepthStencilState *)VSResourceManager::GetDepthStencilStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pDepthStencilState)
	{

		return pDepthStencilState;
	}
	pDepthStencilState = VS_NEW VSDepthStencilState();
	pDepthStencilState->m_DepthStencilDesc = DepthStencilDesc;
	VSResourceManager::GetDepthStencilStateSet().AddResource(uiHashCode,pDepthStencilState);
	return pDepthStencilState;
}
VSRasterizerState * VSResourceManager::CreateRasterizerState(const VSRasterizerDesc &RasterizerDesc)
{
	unsigned int uiDataSize = 0;
	void * pData = RasterizerDesc.GetCRC32Data(uiDataSize);
	unsigned int uiHashCode = CRC32Compute(pData, uiDataSize);

	VSRasterizerState * pRasterizerState = NULL;
	pRasterizerState = (VSRasterizerState *)VSResourceManager::GetRasterizerStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pRasterizerState)
	{

		return pRasterizerState;
	}
	pRasterizerState = VS_NEW VSRasterizerState();
	pRasterizerState->m_RasterizerDesc = RasterizerDesc;
	VSResourceManager::GetRasterizerStateSet().AddResource(uiHashCode,pRasterizerState);
	return pRasterizerState;
}

VSSamplerState * VSResourceManager::CreateSamplerState(const VSSamplerDesc &SamplerDesc)
{
	unsigned int uiDataSize = 0;
	void * pData = SamplerDesc.GetCRC32Data(uiDataSize);
	unsigned int uiHashCode = CRC32Compute(pData, uiDataSize);

	VSSamplerState * pSamplerState = NULL;
	pSamplerState = (VSSamplerState *)VSResourceManager::GetSamplerStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pSamplerState)
	{

		return pSamplerState;
	}
	pSamplerState = VS_NEW VSSamplerState();
	pSamplerState->m_SamplerDesc = SamplerDesc;
	VSResourceManager::GetSamplerStateSet().AddResource(uiHashCode,pSamplerState);
	return pSamplerState;
}
VSRenderTarget * VSResourceManager::CreateRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
									unsigned int uiMulSample)
{
	VSRenderTarget *Temp = VS_NEW VSRenderTarget(uiWidth,uiHeight,uiFormatType,uiMulSample);
	GetRenderTargetArray().AddResource(Temp);
	return Temp;
}
VSRenderTarget * VSResourceManager::CreateRenderTarget(VSTexture * pCreateBy,unsigned int uiMulSample
									,unsigned int uiLevel,unsigned int uiFace)
{
	if (pCreateBy->IsCompress())
	{
		return NULL;
	}
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	VSMAC_ASSERT(uiLevel < pCreateBy->GetMipLevel());
	if (uiLevel >= pCreateBy->GetMipLevel())
	{
		return NULL;
	}
	if (uiFace >= VSCubeTexture::F_MAX)
	{
		return NULL;
	}
	VSRenderTarget *Temp = VS_NEW VSRenderTarget(pCreateBy,uiMulSample,uiLevel,uiFace);
	GetRenderTargetArray().AddResource(Temp);
	return Temp;
}
VSDepthStencil * VSResourceManager::CreateDepthStencil(unsigned int uiWidth,unsigned int uiHeight,
									unsigned int uiMulSample,unsigned int uiFormatType)
{
	VSDepthStencil *Temp = VS_NEW VSDepthStencil(uiWidth,uiHeight,uiMulSample,uiFormatType);
	GetDepthStencilArray().AddResource(Temp);
	return Temp;
}
VSRenderTarget * VSResourceManager::Get2DRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,unsigned int uiMulSample)
{
	VSResourceArrayControll<VSRenderTargetPtr> & RenderTargetArray = GetRenderTargetBufferArray();
	for (unsigned int i = 0 ; i < RenderTargetArray.GetResourceNum() ; i++)
	{
		VSRenderTargetPtr pRt = RenderTargetArray.GetResource(i);
		if (pRt->m_bUsed)
		{
			continue;
		}
		if (pRt->GetCreateBy() && pRt->GetCreateBy()->GetTexType() == VSTexture::TT_2D)
		{
			VS2DTexture * p2DTex = (VS2DTexture *)pRt->GetCreateBy();
			if (p2DTex->HasAnySlot())
			{
				continue;
			}
			if (pRt->m_uiWidth == uiWidth && pRt->m_uiHeight == uiHeight && pRt->m_uiFormatType == uiFormatType && pRt->m_uiMulSample == uiMulSample)
			{
				pRt->m_bUsed = true;
				RenderTargetArray.ClearTimeCount(i);
				return pRt;
			}
		}
	}
	VS2DTexture * pTexture = VS_NEW VS2DTexture(uiWidth,uiHeight,uiFormatType);

	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}

	VSRenderTarget *  pNewRt = VS_NEW VSRenderTarget(pTexture,uiMulSample);
	GetRenderTargetBufferArray().AddResource(pNewRt);

	pNewRt->m_bUsed = true;
	return pNewRt;

}
void VSResourceManager::Release2DRenderTarget(VSRenderTarget * pRenderTarget)
{
	if (pRenderTarget)
	{
		VSMAC_ASSERT(pRenderTarget->m_bUsed == true);
		pRenderTarget->m_bUsed = false;
	}
}
void VSResourceManager::Disable2DRenderTarget(VSRenderTarget * &pRenderTarget)
{
	Release2DRenderTarget(pRenderTarget);
	VS2DTexture * p2DTexture = DynamicCast<VS2DTexture>(pRenderTarget->GetCreateBy());
	VSMAC_ASSERT(p2DTexture);
	p2DTexture->ClearAllSlot();
	pRenderTarget = NULL;
}
void VSResourceManager::Disable2DRenderTarget(VSRenderTargetPtr &pRenderTarget)
{
	Release2DRenderTarget(pRenderTarget);
	VS2DTexture * p2DTexture = DynamicCast<VS2DTexture>(pRenderTarget->GetCreateBy());
	VSMAC_ASSERT(p2DTexture);
	p2DTexture->ClearAllSlot();
	pRenderTarget = NULL;
}
void VSResourceManager::DisableCubRenderTarget(VSRenderTarget * RT[VSCubeTexture::F_MAX])
{
	ReleaseCubRenderTarget(RT);
	VSCubeTexture * pCubTexture = DynamicCast<VSCubeTexture>(RT[0]->GetCreateBy());
	VSMAC_ASSERT(pCubTexture);
	pCubTexture->ClearAllSlot();
	for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ; i++)
	{
		RT[i] = NULL;
	}
}
void VSResourceManager::ReleaseCubRenderTarget(VSRenderTarget * RT[VSCubeTexture::F_MAX])
{
	unsigned int uiUseNum = 0;
	unsigned int uiNullNum = 0;
	for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ; i++)
	{
		if (RT[i])
		{
			uiNullNum++;
			if (RT[i]->m_bUsed == true)
			{
				uiUseNum++;
			}
			RT[i]->m_bUsed = false;
		}
	}
	VSMAC_ASSERT(uiNullNum == VSCubeTexture::F_MAX);
	VSMAC_ASSERT(uiUseNum == VSCubeTexture::F_MAX);
}
void VSResourceManager::ReleaseDepthStencil(VSDepthStencil * pDepthStencil)
{
	if (pDepthStencil)
	{
		VSMAC_ASSERT(pDepthStencil->m_bUsed == true);
		pDepthStencil->m_bUsed = false;
	}
}
void VSResourceManager::DisableDepthStencil(VSDepthStencilPtr &pDepthStencil)
{
	ReleaseDepthStencil(pDepthStencil);
	pDepthStencil = NULL;
}

void VSResourceManager::DisableDepthStencil(VSDepthStencil * &pDepthStencil)
{
	ReleaseDepthStencil(pDepthStencil);
	pDepthStencil = NULL;
}
bool VSResourceManager::GetCubRenderTarget(unsigned int uiWidth,unsigned int uiFormatType,unsigned int uiMulSample,VSRenderTarget * OutRT[VSCubeTexture::F_MAX])
{
	
	VSResourceArrayControll<VSRenderTargetPtr> & RenderTargetArray = GetRenderTargetBufferArray();
	VSCubeTexture * IndexCubTexture = NULL;
	for(unsigned int k = 0 ; k < VSCubeTexture::F_MAX ; k++)
	{
		OutRT[k] = NULL;
	}
	for (unsigned int i = 0 ; i < RenderTargetArray.GetResourceNum() ; i++)
	{
		VSRenderTargetPtr pRt = RenderTargetArray.GetResource(i);
		if (pRt->m_bUsed)
		{
			continue;
		}
		if (pRt->GetCreateBy() && pRt->GetCreateBy()->GetTexType() == VSTexture::TT_CUBE)
		{
			VSCubeTexture * pCubDTex = (VSCubeTexture *)pRt->GetCreateBy();
			if (pCubDTex->HasAnySlot())
			{
				continue;
			}
			if (pRt->m_uiWidth == uiWidth  && pRt->m_uiFormatType == uiFormatType && pRt->m_uiMulSample == uiMulSample)
			{
				VSMAC_ASSERT(pRt->m_uiParam < VSCubeTexture::F_MAX);

				if (!IndexCubTexture)
				{
					IndexCubTexture = pCubDTex;
					if (!OutRT[pRt->m_uiParam])
					{
						OutRT[pRt->m_uiParam] = pRt;
						RenderTargetArray.ClearTimeCount(i);
					}
					else
					{
						for(unsigned int k = 0 ; k < VSCubeTexture::F_MAX ; k++)
						{
							OutRT[k] = NULL;
						}
						return false;
					}
					
				}
				else
				{
					if (IndexCubTexture == pCubDTex)
					{
						if (!OutRT[pRt->m_uiParam])
						{
							RenderTargetArray.ClearTimeCount(i);
							OutRT[pRt->m_uiParam] = pRt;
						}
						else
						{
							for(unsigned int k = 0 ; k < VSCubeTexture::F_MAX ; k++)
							{
								OutRT[k] = NULL;
							}
							return false;
						}
					}
				}
				
				
			}
		}
	}
	unsigned int uiFindNum = 0;
	for(unsigned int k = 0 ; k < VSCubeTexture::F_MAX ; k++)
	{
		if (OutRT[k])
		{
			uiFindNum++;
		}
	}

	if (uiFindNum == 0)
	{
		VSCubeTexture *pCubTexture = VS_NEW VSCubeTexture(uiWidth,uiFormatType);
		for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ; i++)
		{
			VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
			if (uiMulSample >= VSRenderer::MS_MAX)
			{
				return NULL;
			}

			OutRT[i] = VS_NEW VSRenderTarget(pCubTexture,uiMulSample,0,i);
			RenderTargetArray.AddResource(OutRT[i]);
			OutRT[i]->m_bUsed = true;
		}
		return true;
	}
	else if (uiFindNum == VSCubeTexture::F_MAX)
	{
		for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ; i++)
		{
			OutRT[i]->m_bUsed = true;
		}
		return true;
	}
	else // GC will delete some ,so no longer  use any rt
	{
		//set other rt used , so you cat't find it
		for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ; i++)
		{
			if (OutRT[i])
			{
				OutRT[i]->m_bUsed = true;
			}	
		}

		// create new one
		VSCubeTexture *pCubTexture = VS_NEW VSCubeTexture(uiWidth,uiFormatType);
		for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ; i++)
		{
			VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
			if (uiMulSample >= VSRenderer::MS_MAX)
			{
				return NULL;
			}

			OutRT[i] = VS_NEW VSRenderTarget(pCubTexture,uiMulSample,0,i);
			RenderTargetArray.AddResource(OutRT[i]);
			OutRT[i]->m_bUsed = true;
		}
		return true;
	}
}

VSDepthStencil * VSResourceManager::GetDepthStencil(unsigned int uiWidth,unsigned int uiHeight,unsigned int uiFormatType,unsigned int uiMulSample)
{
	VSResourceArrayControll<VSDepthStencilPtr> & DepthStencilArray = GetDepthStencilBufferArray();
	for (unsigned int i = 0 ; i < DepthStencilArray.GetResourceNum() ; i++)
	{
		VSDepthStencilPtr pDS = DepthStencilArray.GetResource(i);
		if (pDS->m_bUsed)
		{
			continue;;
		}
		if (pDS->m_uiWidth == uiWidth && pDS->m_uiHeight == uiHeight && pDS->m_uiFormatType == uiFormatType && pDS->m_uiMulSample == uiMulSample)
		{
			pDS->m_bUsed = true;
			DepthStencilArray.ClearTimeCount(i);
			return pDS;
		}

	}

	VSDepthStencil* pNewDS= VS_NEW VSDepthStencil(uiWidth,uiHeight,uiMulSample,uiFormatType);
	GetDepthStencilBufferArray().AddResource(pNewDS);
	pNewDS->m_bUsed = true;
	return pNewDS;
}
VSAnim * VSResourceManager::CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR)
{
	if (!pSourceAnim || !pTargetAnim || !pBlendAnimR)
	{
		return NULL;
	}

	while (!pBlendAnimR->IsLoaded())
	{

	}
	VSAnim * pBlendAnim = pBlendAnimR->GetResource();

	if (pSourceAnim->IsAdditive() || pTargetAnim->IsAdditive() || pBlendAnim->IsAdditive())
	{
		return NULL;
	}



	VSAnim * pAddAnim = VS_NEW VSAnim();
	pAddAnim->m_pBlendAnim = pBlendAnimR;
	pAddAnim->SetRootMotion(pBlendAnim->IsRootAnim());


	VSREAL fSourceTimeScale = pSourceAnim->GetAnimLength() / pBlendAnim->GetAnimLength();

	VSREAL fTargetTimeScale = pTargetAnim->GetAnimLength() / pBlendAnim->GetAnimLength();

	for (unsigned int i = 0; i < pBlendAnim->GetBoneKeyNum(); i++)
	{
		VSBoneKey * pBlendBoneKey = pSourceAnim->GetBoneKey(i);
		
		if (!pBlendBoneKey)
		{
			continue;
		}
		VSBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pTargetBoneKey = pTargetAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pAddBoneKey = VS_NEW VSBoneKey();
		pAddBoneKey->m_cName = pSourceBoneKey->m_cName;
		if (!pTargetBoneKey || !pSourceBoneKey)
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			pAddBoneKey->m_TranslationArray[0].m_Vector = VSVector3::ms_Zero;
			pAddBoneKey->m_ScaleArray[0].m_Vector = VSVector3::ms_One;
			pAddBoneKey->m_RotatorArray[0].m_Quat = VSQuat();
			return false;
		}
		else
		{
			

			pAddBoneKey->m_TranslationArray.SetBufferNum(pBlendBoneKey->m_TranslationArray.GetNum());
			for (unsigned int j = 0; j < pBlendBoneKey->m_TranslationArray.GetNum(); j++)
			{
				VSREAL fSourceTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime * fSourceTimeScale;
				VSREAL fTargetTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime * fTargetTimeScale;

				pAddBoneKey->m_TranslationArray[j].m_dKeyTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime;
				VSVector3 SourceTranslation = pSourceAnim->GetTranslation(pBlendBoneKey->m_cName, fSourceTime);
				VSVector3 TargetTranslation = pTargetAnim->GetTranslation(pBlendBoneKey->m_cName, fTargetTime);
				pAddBoneKey->m_TranslationArray[j].m_Vector = SourceTranslation - TargetTranslation;
					

			}

			pAddBoneKey->m_ScaleArray.SetBufferNum(pBlendBoneKey->m_ScaleArray.GetNum());
			for (unsigned int j = 0; j < pBlendBoneKey->m_ScaleArray.GetNum(); j++)
			{
				VSREAL fSourceTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime * fSourceTimeScale;
				VSREAL fTargetTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime * fTargetTimeScale;

				pAddBoneKey->m_ScaleArray[j].m_dKeyTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime;

				VSVector3 SourceScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fSourceTime);
				VSVector3 TargetScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fTargetTime);
				VSMAC_ASSERT(SourceScale.x > EPSILON_E4 && SourceScale.y > EPSILON_E4 && SourceScale.z > EPSILON_E4);
				VSMAC_ASSERT(TargetScale.x > EPSILON_E4 && TargetScale.y > EPSILON_E4 && TargetScale.z > EPSILON_E4);
				pAddBoneKey->m_ScaleArray[j].m_Vector = SourceScale / TargetScale;


			}


			pAddBoneKey->m_RotatorArray.SetBufferNum(pBlendBoneKey->m_RotatorArray.GetNum());
			for (unsigned int j = 0; j < pBlendBoneKey->m_RotatorArray.GetNum();j++)
			{
				VSREAL fSourceTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime * fSourceTimeScale;
				VSREAL fTargetTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime * fTargetTimeScale;

				pAddBoneKey->m_RotatorArray[j].m_dKeyTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime;
				VSQuat TargetRoator = pTargetAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime).GetInverse();
				VSQuat SourceRoator = pSourceAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime);
				pAddBoneKey->m_RotatorArray[j].m_Quat = SourceRoator * TargetRoator;

			}
		}
		

		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	
	return pAddAnim;
}

VSAnim * VSResourceManager::CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR,VSREAL fTargetTime)
{

	if (!pSourceAnim || !pTargetAnim)
	{
		return NULL;
	}


	while (!pBlendAnimR->IsLoaded())
	{

	}
	VSAnim * pBlendAnim = pBlendAnimR->GetResource();

	if (pSourceAnim->IsAdditive() || pTargetAnim->IsAdditive() || pBlendAnim->IsAdditive())
	{
		return NULL;
	}

	VSAnim * pAddAnim = VS_NEW VSAnim();
	pAddAnim->m_pBlendAnim = pBlendAnimR;
	pAddAnim->SetRootMotion(pBlendAnim->IsRootAnim());

	VSREAL fSourceTimeScale = pSourceAnim->GetAnimLength() / pBlendAnim->GetAnimLength();

	for (unsigned int i = 0; i < pBlendAnim->GetBoneKeyNum(); i++)
	{
		VSBoneKey * pBlendBoneKey = pBlendAnim->GetBoneKey(i);
		if (!pBlendBoneKey)
		{
			continue;
		}
		VSBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pTargetBoneKey = pTargetAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pAddBoneKey = VS_NEW VSBoneKey();
		pAddBoneKey->m_cName = pSourceBoneKey->m_cName;

		if (!pTargetBoneKey || !pSourceBoneKey)
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			pAddBoneKey->m_TranslationArray[0].m_Vector = VSVector3::ms_Zero;
			pAddBoneKey->m_ScaleArray[0].m_Vector = VSVector3::ms_One;
			pAddBoneKey->m_RotatorArray[0].m_Quat = VSQuat();
			return false;
		}
		else
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(pBlendBoneKey->m_TranslationArray.GetNum());
			VSVector3 TargetTranslation = pTargetAnim->GetTranslation(pBlendBoneKey->m_cName, fTargetTime);
			for (unsigned int j = 0; j < pBlendBoneKey->m_TranslationArray.GetNum(); j++)
			{
				VSREAL fSourceTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime * fSourceTimeScale;

				pAddBoneKey->m_TranslationArray[j].m_dKeyTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime;
				VSVector3 SourceTranslation = pSourceAnim->GetTranslation(pBlendBoneKey->m_cName, fSourceTime);
				pAddBoneKey->m_TranslationArray[j].m_Vector = SourceTranslation - TargetTranslation;


			}

			pAddBoneKey->m_ScaleArray.SetBufferNum(pBlendBoneKey->m_ScaleArray.GetNum());
			VSVector3 TargetScale = pTargetAnim->GetScale(pSourceBoneKey->m_cName, fTargetTime);
			VSMAC_ASSERT(TargetScale.x > EPSILON_E4 && TargetScale.y > EPSILON_E4 && TargetScale.z > EPSILON_E4);
			for (unsigned int j = 0; j < pBlendBoneKey->m_ScaleArray.GetNum(); j++)
			{
				VSREAL fSourceTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime * fSourceTimeScale;


				pAddBoneKey->m_ScaleArray[j].m_dKeyTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime;

				VSVector3 SourceScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fSourceTime);

				VSMAC_ASSERT(SourceScale.x > EPSILON_E4 && SourceScale.y > EPSILON_E4 && SourceScale.z > EPSILON_E4);

				pAddBoneKey->m_ScaleArray[j].m_Vector = SourceScale / TargetScale;


			}

			VSQuat TargetRoator = pTargetAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime).GetInverse();
			pAddBoneKey->m_RotatorArray.SetBufferNum(pBlendBoneKey->m_RotatorArray.GetNum());
			for (unsigned int j = 0; j < pBlendBoneKey->m_RotatorArray.GetNum(); j++)
			{
				VSREAL fSourceTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime * fSourceTimeScale;


				pAddBoneKey->m_RotatorArray[j].m_dKeyTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime;

				VSQuat SourceRoator = pSourceAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime);
				pAddBoneKey->m_RotatorArray[j].m_Quat = SourceRoator * TargetRoator;

			}
		}
		



		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	return pAddAnim;
}
VSAnim * VSResourceManager::CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR,VSREAL fSourceTime, VSREAL fTargetTime)
{
	if (!pSourceAnim || !pTargetAnim)
	{
		return NULL;
	}


	while (!pBlendAnimR->IsLoaded())
	{

	}
	VSAnim * pBlendAnim = pBlendAnimR->GetResource();

	if (pSourceAnim->IsAdditive() || pTargetAnim->IsAdditive() || pBlendAnim->IsAdditive())
	{
		return NULL;
	}

	VSAnim * pAddAnim = VS_NEW VSAnim();
	pAddAnim->m_pBlendAnim = pBlendAnimR;
	pAddAnim->SetRootMotion(pSourceAnim->IsRootAnim());


	for (unsigned int i = 0; i < pBlendAnim->GetBoneKeyNum(); i++)
	{
		VSBoneKey * pBlendBoneKey = pBlendAnim->GetBoneKey(i);
		if (!pBlendBoneKey)
		{
			continue;
		}
		VSBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pTargetBoneKey = pTargetAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pAddBoneKey = VS_NEW VSBoneKey();
		pAddBoneKey->m_cName = pBlendBoneKey->m_cName;

		if (!pTargetBoneKey || !pSourceBoneKey)
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			pAddBoneKey->m_TranslationArray[0].m_Vector = VSVector3::ms_Zero;
			pAddBoneKey->m_ScaleArray[0].m_Vector = VSVector3::ms_One;
			pAddBoneKey->m_RotatorArray[0].m_Quat = VSQuat();
			return false;
		}
		else
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			VSVector3 TargetTranslation = pTargetAnim->GetTranslation(pBlendBoneKey->m_cName, fTargetTime);
			pAddBoneKey->m_TranslationArray[0].m_dKeyTime = 0.0f;
			VSVector3 SourceTranslation = pSourceAnim->GetTranslation(pBlendBoneKey->m_cName, fSourceTime);
			pAddBoneKey->m_TranslationArray[0].m_Vector = SourceTranslation - TargetTranslation;



			pAddBoneKey->m_ScaleArray.SetBufferNum(1);
			VSVector3 TargetScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fTargetTime);
			VSMAC_ASSERT(TargetScale.x > EPSILON_E4 && TargetScale.y > EPSILON_E4 && TargetScale.z > EPSILON_E4);
			pAddBoneKey->m_ScaleArray[0].m_dKeyTime = 0.0f;
			VSVector3 SourceScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fSourceTime);
			VSMAC_ASSERT(SourceScale.x > EPSILON_E4 && SourceScale.y > EPSILON_E4 && SourceScale.z > EPSILON_E4);
			pAddBoneKey->m_ScaleArray[0].m_Vector = SourceScale / TargetScale;



			VSQuat TargetRoator = pTargetAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime).GetInverse();
			pAddBoneKey->m_RotatorArray.SetBufferNum(1);
			pAddBoneKey->m_RotatorArray[0].m_dKeyTime = 0.0f;
			VSQuat SourceRoator = pSourceAnim->GetQuat(pBlendBoneKey->m_cName, fSourceTime);
			pAddBoneKey->m_RotatorArray[0].m_Quat = SourceRoator * TargetRoator;


		}




		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	return pAddAnim;
}

VSAnim * VSResourceManager::CreateAnim(VSAnim * pSourceAnim, VSREAL fSourceTime)
{
	if (!pSourceAnim)
	{
		return NULL;
	}



	VSAnim * pAddAnim = VS_NEW VSAnim();
	pAddAnim->SetRootMotion(pSourceAnim->IsRootAnim());


	for (unsigned int i = 0; i < pSourceAnim->GetBoneKeyNum(); i++)
	{
		VSBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(i);
		if (!pSourceBoneKey)
		{
			continue;
		}


		VSBoneKey * pAddBoneKey = VS_NEW VSBoneKey();
		pAddBoneKey->m_cName = pSourceBoneKey->m_cName;

		pAddBoneKey->m_TranslationArray.SetBufferNum(1);

		VSVector3 Translate = pSourceAnim->GetTranslation(pSourceBoneKey->m_cName, fSourceTime);
		pAddBoneKey->m_TranslationArray[0].m_dKeyTime = 0.0F;
		pAddBoneKey->m_TranslationArray[0].m_Vector = Translate;

		

		pAddBoneKey->m_ScaleArray.SetBufferNum(1);
		VSVector3 Scale = pSourceAnim->GetScale(pSourceBoneKey->m_cName, fSourceTime);

		VSMAC_ASSERT(Scale.x > EPSILON_E4 && Scale.y > EPSILON_E4 && Scale.z > EPSILON_E4);

		pAddBoneKey->m_ScaleArray[0].m_dKeyTime = 0.0f;
		pAddBoneKey->m_ScaleArray[0].m_Vector = Scale;



		pAddBoneKey->m_RotatorArray.SetBufferNum(1);
		VSQuat Rotator = pSourceAnim->GetQuat(pSourceBoneKey->m_cName, fSourceTime);


		pAddBoneKey->m_RotatorArray[0].m_dKeyTime = 0.0f;
		pAddBoneKey->m_RotatorArray[0].m_Quat = Rotator;


		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	return pAddAnim;
}