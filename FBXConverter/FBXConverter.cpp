#ifndef FBXCONVERTER_H
#define FBXCONVERTER_H
#include "VSApplication.h"
#include <fbxsdk.h>
#include <fbxsdk/fileio/fbximporter.h>
#include <fbxsdk/core/fbxmanager.h>
#include "VSTriangleSet.h"
#include "VSIndexBuffer.h"
#include "VSBoneNode.h"
namespace VSEngine2
{

//SourcePath DestPath -s(static mesh)/-d(dynamic mesh)/-a(action) -c(compress) -v(shadow volume) -r(root motion) -m(morph) -n(use FbxNormal)
	class VSFBXConverter : public VSConsoleApplication
	{
	public:
		enum //EXPORT TYPE
		{
			ET_STATIC_MESH,
			ET_SKELECTON_MESH,
			ET_ACTION,
			ET_MAX
		};
		enum //EXPORT PARA
		{
			EP_UV_COMPRESS = 0X01,
			EP_NORMAL_COMPRESS = 0X02,
			EP_SKIN_COMPRESS = 0X04,
			EP_ACTION_COMPRESS = 0X08,
		};
		VSFBXConverter();
		virtual ~VSFBXConverter();
		DLCARE_APPLICATION(VSFBXConverter);
	public:
		virtual bool PreInitial();
		virtual bool OnInitial();	
		virtual bool OnTerminal();
		void Reset();
	protected:
		VSString m_SourceDir;
		VSString m_DestDir;
		FbxManager *m_pFbxSdkManager;
		FbxScene* m_pFbxScene;
		FbxImporter* m_pFbxImporter;
		bool ExportFile(const TCHAR * pSourceFile,const TCHAR * pDestFile);
		bool GetMeshNode(FbxNode* pNode);
		void MaxMatToVSMat(const FbxAMatrix &MaxMat,VSMatrix3X3W & VSMat);
		void GetSkeleton(FbxNode* pNode,VSBoneNode * pParentBoneNode = NULL);
		void GetAnim(FbxNode* pNode); 
		void ReadVertex(FbxGeometryBase* pMesh, int ctrlPointIndex, VSVector3 & Vertex);
		void ReadColor(FbxGeometryBase* pMesh, int ctrlPointIndex, int vertexCounter, VSColorRGBA &Color);
		void ReadUV(FbxGeometryBase* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, VSVector2 & UV);
		void ReadNormal(FbxGeometryBase* pMesh, int ctrlPointIndex, int vertexCounter, VSVector3 &Normal);
		void ReadTangent(FbxGeometryBase* pMesh, int ctrlPointIndex, int vertexCounter, VSVector3 &Tangent);
		void ReadBinormal(FbxGeometryBase* pMesh, int ctrlPointIndex, int vertexCounter, VSVector3 &Tangent);
		void GetTriangleMaterialIndex(FbxGeometryBase* pMesh, int triangleCount, VSArray<int> & TriangleMaterialIndex);
		void GetTriangleSmGroupIndex(FbxGeometryBase* pMesh, int triangleCount, VSArray<int> & TriangleSmGroupIndex);
		void CreateTangentAndBinormal(const VSVector3 & Point0, const VSVector3 & Point1,const VSVector3 &Point2,
			const VSVector2 & Texcoord0,const VSVector2 & Texcoord1,const VSVector2 TexCoord2,
			const VSVector3 &Normal,VSVector3 & Tangent,VSVector3 & Binromal);
		void ClearAllVertexInfo();

		bool CreateMesh(VSString & Name,unsigned int TexCoordNum, bool HasSkin);
		void Orthogonal(VSVector3 & Normal,VSVector3 & Tangent,VSVector3 & Binromal);
		void BoneSkin(FbxSkin *pSkin,VSArray<VSString> & Bone,VSArray<VSREAL>& Weight,unsigned int VertexIndex);
		void GetOffSetMatrix(FbxSkin *pSkin);
		bool ProcessMesh(FbxNode* pNode);  
		bool IsBoneNumAccept(FbxSkin *pSkin,VSArray<VSUsedName> & Bone,unsigned int VertexIndex);

		VSMatrix3X3W GetHelpMaterix();


		bool GetShadowVolumeData(VSVertexBuffer * pVertexBuffer , VSIndexBuffer * pIndexBuffer, bool HasSkin);

		void AddMorph();
		unsigned int m_CurExportType;
		unsigned int m_CurExportPara;
		bool		 m_bHasShadowVolume;
		bool		m_bRootMotion;
		bool		m_bUseFbxNormal;
		bool		m_bHasMorph;
		VSMeshNodePtr m_pNode;
		VSGeometryNodePtr m_pGeoNode;
		VSSkelectonPtr m_pSkeleton;
		VSAnimPtr	m_pAnim;
		VSMorphSetPtr m_pMorphSet;

		VSArray<VSVector3> m_VertexArray;
		VSArray<VSColorRGBA>m_ColorArray;
		VSArray<DWORD> m_VertexSmGroupArray;
		VSArray<VSVector2> m_TexCoordArray[TEXLEVEL];
		VSArray<VSVector3> m_NormalArray;
		VSArray<VSVector3> m_TangentArray;
		VSArray<VSVector3> m_BinormalArray;

		VSArray<VSArray<VSVector3>> m_MorphVertexArray;
		VSArray<VSArray<VSColorRGBA>> m_MorphColorArray;
		VSArray<VSArray<VSVector3>> m_MorphNormalArray;
		VSArray<VSArray<VSVector3>> m_MorphTangentArray;
		VSArray<VSArray<VSVector3>> m_MorphBinormalArray;
		VSArray<VSString> m_MorphName;
		VSArray<VSUINT_INDEX> m_IndexArray;

		VSArray<VSUsedName> m_MeshBoneNode;
		VSArray<VSVector3W> m_BoneIndex;
		VSArray<VSVector3W> m_BoneWeight;
		
		struct  MorphType
		{
			unsigned int uiGeometryIndex;
			VSString MorphName;
			VSVertexBufferPtr pVertexBuffer;
		};
		VSArray<MorphType> m_MorphTargetList;
	};
	IMPLEMENT_APPLICATION(VSFBXConverter);
	void VSFBXConverter::Reset()
	{
		
		
		m_pNode = NULL;
		m_pGeoNode = NULL;
		m_pSkeleton = NULL;
		m_pAnim = NULL;
		m_pMorphSet = NULL;
		ClearAllVertexInfo();
		m_MorphTargetList.Clear();
		m_MorphVertexArray.Clear();
		m_MorphColorArray.Clear();
		m_MorphNormalArray.Clear();
		m_MorphTangentArray.Clear();
		m_MorphBinormalArray.Clear();

	}
	VSFBXConverter::VSFBXConverter()
	{
		m_pFbxSdkManager = NULL;
		m_pFbxScene = NULL;
		m_pFbxImporter = NULL;

		m_CurExportPara = EP_UV_COMPRESS | EP_NORMAL_COMPRESS | EP_SKIN_COMPRESS | EP_ACTION_COMPRESS;
		m_CurExportType = ET_MAX;

		m_pNode = NULL;
		m_pGeoNode = NULL;
		m_pSkeleton = NULL;
		m_pAnim = NULL;
		m_pMorphSet = NULL;

		m_bHasShadowVolume = false;
		m_bUseFbxNormal = false;
		m_bHasMorph = false;
	}
	void VSFBXConverter::ClearAllVertexInfo()
	{
		m_VertexArray.Clear();
		m_ColorArray.Clear();
		for(unsigned int i = 0 ; i < TEXLEVEL ; i++)
			m_TexCoordArray[i].Clear();
		m_NormalArray.Clear();
		m_TangentArray.Clear();
		m_BinormalArray.Clear();
		m_IndexArray.Clear();
		m_VertexSmGroupArray.Clear();
		m_MeshBoneNode.Clear();
		m_BoneIndex.Clear();
		m_BoneWeight.Clear();

		for (unsigned int i = 0; i < m_MorphVertexArray.GetNum() ; i++)
		{
			m_MorphVertexArray[i].Clear();
			m_MorphColorArray[i].Clear();
			m_MorphNormalArray[i].Clear();
			m_MorphTangentArray[i].Clear();
			m_MorphBinormalArray[i].Clear();
		}
		

	}
	VSFBXConverter::~VSFBXConverter()
	{
		
		
	}
	bool VSFBXConverter::OnTerminal()
	{
		if (!VSConsoleApplication::OnTerminal())
		{
			return false;
		}
		Reset();
		if (m_pFbxSdkManager)
		{
			m_pFbxSdkManager->Destroy();
			m_pFbxSdkManager = NULL;
		}
		return true;
	}
	bool VSFBXConverter::PreInitial()
	{
		if (!VSConsoleApplication::PreInitial())
		{
			return false;
		}
		m_pFbxSdkManager = FbxManager::Create();
		if (!m_pFbxSdkManager)
		{
			return false;
		}

		FbxIOSettings *pIOSettings = FbxIOSettings::Create(m_pFbxSdkManager, IOSROOT);
		m_pFbxSdkManager->SetIOSettings(pIOSettings);

		int lSDKMajor,  lSDKMinor,  lSDKRevision;
		FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
		VSOutPutDebugString("FBX SDK version is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
		
		m_pFbxScene = FbxScene::Create(m_pFbxSdkManager, "");

		m_pFbxImporter = FbxImporter::Create(m_pFbxSdkManager, "");


		return true;
	}
	bool VSFBXConverter::ExportFile(const TCHAR * pSourceFile,const TCHAR * pDestFile)
	{

		int lFileMajor, lFileMinor, lFileRevision;
		const bool lImportStatus = m_pFbxImporter->Initialize(pSourceFile, -1, m_pFbxSdkManager->GetIOSettings());
		m_pFbxImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
		VSOutPutDebugString("pFile FBX version is %d.%d.%d\n",lFileMajor, lFileMinor, lFileRevision);
		printf("pFile FBX version is %d.%d.%d\n", lFileMajor, lFileMinor, lFileRevision);
		if (!lImportStatus)
		{
			VSOutPutDebugString("Error: %s\n", m_pFbxImporter->GetStatus().GetErrorString());
			printf("Error: %s\n", m_pFbxImporter->GetStatus().GetErrorString());
			return false;
		}

		if(!m_pFbxImporter->Import(m_pFbxScene))
		{
			m_pFbxImporter->Destroy();
			VSOutPutDebugString("Error: Import Scene\n");
			printf("Error: Import Scene\n");
			return false;
		}
		FbxAxisSystem CurSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem VSEngineSystem(FbxAxisSystem::eZAxis, (FbxAxisSystem::EFrontVector)(-FbxAxisSystem::eParityOdd), FbxAxisSystem::eRightHanded);
		VSEngineSystem.ConvertScene(m_pFbxScene);
		bool bIsError = false;
		if (m_CurExportType == ET_STATIC_MESH)
		{
			m_pNode = VS_NEW VSStaticMeshNode();

			m_pGeoNode = VS_NEW VSGeometryNode();

			//默认MAX YZ两个方向和引擎中YZ方向是反向的
			VSMatrix3X3 M1(1.0f,0.0f,0.0f,
							0.0f,0.0f,1.0f,
							0.0f,1.0f,0.0f);
			m_pGeoNode->SetLocalRotate(M1);
			m_pNode->AddChild(m_pGeoNode);

			if (m_bHasMorph)
			{
				m_pMorphSet = VS_NEW VSMorphSet();
			}
			if (GetMeshNode(m_pFbxScene->GetRootNode()) == false)
			{
				bIsError = true;
			}
		}
		else if (m_CurExportType == ET_SKELECTON_MESH)
		{
			m_pNode = VS_NEW VSSkelectonMeshNode();
			m_pSkeleton = VS_NEW VSSkelecton();
			GetSkeleton(m_pFbxScene->GetRootNode());
			VSMatrix3X3 M1(1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 0.0f);
			m_pSkeleton->SetLocalRotate(M1);
			m_pSkeleton->CreateBoneArray();

			m_pGeoNode = VS_NEW VSGeometryNode();
			m_pNode->AddChild(m_pGeoNode);
			if (m_bHasMorph)
			{
				m_pMorphSet = VS_NEW VSMorphSet();
			}
			if (GetMeshNode(m_pFbxScene->GetRootNode()) == false)
			{
				bIsError = true;
			}
		}
		else if(m_CurExportType == ET_ACTION)
		{
			m_pAnim = VS_NEW VSAnim();
			VSString PathNameAndSuffix = pDestFile;
			VSString NameAndSuffix;
			if (NameAndSuffix.GetString(PathNameAndSuffix, _T('/'), -1, false) == false)
			{
				NameAndSuffix.GetString(PathNameAndSuffix, _T('\\'), -1, false);
			}
			VSString AnimName;

			AnimName.GetString(NameAndSuffix,'.',-1);			
			m_pAnim->m_cName = AnimName;
			GetAnim(m_pFbxScene->GetRootNode());
			m_pAnim->SetRootMotion( m_bRootMotion);
			
		}
		else
		{
			bIsError = true;
		}
		
		
		m_pFbxScene->Clear();
		m_pFbxImporter->Destroy();
		VSOutPutDebugString("Saving Resource\n");
		printf("Saving Resource\n");
		if (bIsError == false)
		{
			if (m_CurExportType == ET_STATIC_MESH)
			{
				if (m_pNode)
				{
					
					m_pNode->CreateLocalAABB();
					m_pNode->UpdateAll(0.0f);
					AddMorph();
					VSResourceManager::NewSaveStaticMesh(StaticCast<VSStaticMeshNode>(m_pNode), pDestFile);
				}
				else
				{
					return false;
				}
			}
			else if (m_CurExportType == ET_SKELECTON_MESH)
			{
				if (m_pSkeleton && m_pNode)
				{
					VSMeshNode * m_pSkelectonMesh = m_pNode;
					((VSSkelectonMeshNode *)m_pSkelectonMesh)->SetSkelecton(m_pSkeleton);
					//更新完骨架，在创建AABB
					m_pNode->CreateLocalAABB();
					m_pNode->UpdateAll(0.0f);
					AddMorph();
					m_pSkeleton->CreateLocalAABB();
					VSResourceManager::NewSaveSkelectonMeshNode(StaticCast<VSSkelectonMeshNode>(m_pNode), pDestFile);

				}
				else
				{
					return false;
				}
			}
			else if (m_CurExportType == ET_ACTION)
			{
				if (m_pAnim)
				{

					m_pAnim->ComputeAnimLength();
					if (m_CurExportPara & EP_ACTION_COMPRESS)
					{
						m_pAnim->Compress();
					}
					VSResourceManager::NewSaveAction(m_pAnim, pDestFile);

				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			VSOutPutDebugString("OutPut Resource Error\n");
			printf("OutPut Resource Error\n");
			return false;
		}
		VSOutPutDebugString("OutPut Resource Finish\n");
		printf("OutPut Resource Finish\n");
		return true;
	}
	void VSFBXConverter::AddMorph()
	{
		if (m_pMorphSet && m_MorphTargetList.GetNum())
		{
			for (unsigned int i = 0; i < m_MorphTargetList.GetNum(); i++)
			{
				unsigned int j = 0;
				for (; j < m_pMorphSet->GetMorphNum(); j++)
				{
					VSMorph * pMorph = m_pMorphSet->GetMorph(j);
					if (pMorph->m_cName == m_MorphTargetList[i].MorphName)
					{
						pMorph->SetVertexBuffer(m_MorphTargetList[i].uiGeometryIndex, m_MorphTargetList[i].pVertexBuffer);
						m_pMorphSet->SetMorph(pMorph);
						break;
					}

				}
				if (j == m_pMorphSet->GetMorphNum())
				{
					VSMorphPtr pMorph = VS_NEW VSMorph();
					pMorph->m_cName = m_MorphTargetList[i].MorphName;
					pMorph->ReSizeBuffer(m_pGeoNode->GetNormalGeometryNum());
					pMorph->SetVertexBuffer(m_MorphTargetList[i].uiGeometryIndex, m_MorphTargetList[i].pVertexBuffer);
					m_pMorphSet->SetMorph(pMorph);
					VSOutPutDebugString("MorphName:%s\n", m_MorphTargetList[i].MorphName.GetBuffer());
					printf("MorphName:%s\n", m_MorphTargetList[i].MorphName.GetBuffer());
				}
			}
			m_pGeoNode->SetMorphSet(m_pMorphSet);
		}
	}
	VSMatrix3X3W VSFBXConverter::GetHelpMaterix()
	{
		 
		if (m_CurExportType == ET_STATIC_MESH)
		{
			VSMatrix3X3W M1(1.0f,0.0f,0.0f,0.0f,
							0.0f,0.0f,1.0f,0.0f,
							0.0f,1.0f,0.0f,0.0f,
							0.0f,0.0f,0.0f,1.0f);
			return M1;
		}
		else /*if(m_CurExportType == ET_ACTION || m_CurExportType == ET_SKELECTON_MESH)*/
		{
			VSMatrix3X3W M1(1.0f,0.0f,0.0f,0.0f,
							0.0f,1.0f,0.0f,0.0f,
							0.0f,0.0f,-1.0f,0.0f,
							0.0f,0.0f,0.0f,1.0f);
			return M1;
		}

	}
	void VSFBXConverter::ReadVertex(FbxGeometryBase* pMesh, int ctrlPointIndex, VSVector3 & Vertex)
	{  
		FbxVector4* pCtrlPoint = pMesh->GetControlPoints();  

		Vertex.x = (VSREAL)pCtrlPoint[ctrlPointIndex][0];  
		Vertex.y = (VSREAL)pCtrlPoint[ctrlPointIndex][1];  
		Vertex.z = (VSREAL)pCtrlPoint[ctrlPointIndex][2]; 

	}  
	void VSFBXConverter::ReadNormal(FbxGeometryBase* pMesh, int ctrlPointIndex, int vertexCounter, VSVector3 &Normal)
	{
		if(pMesh->GetElementNormalCount() < 1)  
		{  
			return;  
		}  

		FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(0);  
		switch(leNormal->GetMappingMode())  
		{  
				case FbxGeometryElement::eByControlPoint:
				{  
					switch(leNormal->GetReferenceMode())  
					{  
						case FbxGeometryElement::eDirect:  
							{  
								Normal.x = (VSREAL)leNormal->GetDirectArray().GetAt(ctrlPointIndex)[0];  
								Normal.y = (VSREAL)leNormal->GetDirectArray().GetAt(ctrlPointIndex)[1];  
								Normal.z = (VSREAL)leNormal->GetDirectArray().GetAt(ctrlPointIndex)[2];  
							}  
						break;  
						case FbxGeometryElement::eIndexToDirect:  
							{  
								int id = leNormal->GetIndexArray().GetAt(ctrlPointIndex);  
								Normal.x = (VSREAL)leNormal->GetDirectArray().GetAt(id)[0];  
								Normal.y = (VSREAL)leNormal->GetDirectArray().GetAt(id)[1];  
								Normal.z = (VSREAL)leNormal->GetDirectArray().GetAt(id)[2];  
							}  
							break;  
						default:  
							break;  
					}  
				}  
				break;  
			case FbxGeometryElement::eByPolygonVertex:  
				{  
					switch(leNormal->GetReferenceMode())  
					{  
						case FbxGeometryElement::eDirect:  
						{  
							Normal.x = (VSREAL)leNormal->GetDirectArray().GetAt(vertexCounter)[0];  
							Normal.y = (VSREAL)leNormal->GetDirectArray().GetAt(vertexCounter)[1];  
							Normal.z = (VSREAL)leNormal->GetDirectArray().GetAt(vertexCounter)[2];  
						}  
							break;  
						case FbxGeometryElement::eIndexToDirect:  
						{  
							int id = leNormal->GetIndexArray().GetAt(vertexCounter);  
							Normal.x = (VSREAL)leNormal->GetDirectArray().GetAt(id)[0];  
							Normal.y = (VSREAL)leNormal->GetDirectArray().GetAt(id)[1];  
							Normal.z = (VSREAL)leNormal->GetDirectArray().GetAt(id)[2];  
						}  
							break;  
						default:  
							break;  
						  
					}  
				break;  
			}  
		}
 
	}
	void VSFBXConverter::ReadTangent(FbxGeometryBase* pMesh, int ctrlPointIndex, int vertexCounter, VSVector3 &Tangent)
	{
		if(pMesh->GetElementTangentCount() < 1)  
		{  
			return;  
		}  

		FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(0);  
		switch(leTangent->GetMappingMode())  
		{  
		case FbxGeometryElement::eByControlPoint:  
			{  
				switch(leTangent->GetReferenceMode())  
				{  
				case FbxGeometryElement::eDirect:  
					{  
						Tangent.x = (VSREAL)leTangent->GetDirectArray().GetAt(ctrlPointIndex)[0];  
						Tangent.y = (VSREAL)leTangent->GetDirectArray().GetAt(ctrlPointIndex)[1];  
						Tangent.z = (VSREAL)leTangent->GetDirectArray().GetAt(ctrlPointIndex)[2];  
					}  
					break;  
				case FbxGeometryElement::eIndexToDirect:  
					{  
						int id = leTangent->GetIndexArray().GetAt(ctrlPointIndex);  
						Tangent.x = (VSREAL)leTangent->GetDirectArray().GetAt(id)[0];  
						Tangent.y = (VSREAL)leTangent->GetDirectArray().GetAt(id)[1];  
						Tangent.z = (VSREAL)leTangent->GetDirectArray().GetAt(id)[2];  
					}  
					break;  
				default:  
					break;  
				}  
			}  
			break;  
		case FbxGeometryElement::eByPolygonVertex:  
			{  
				switch(leTangent->GetReferenceMode())  
				{  
				case FbxGeometryElement::eDirect:  
					{  
						Tangent.x = (VSREAL)leTangent->GetDirectArray().GetAt(vertexCounter)[0];  
						Tangent.y = (VSREAL)leTangent->GetDirectArray().GetAt(vertexCounter)[1];  
						Tangent.z = (VSREAL)leTangent->GetDirectArray().GetAt(vertexCounter)[2];  
					}  
					break;  
				case FbxGeometryElement::eIndexToDirect:  
					{  
						int id = leTangent->GetIndexArray().GetAt(vertexCounter);  
						Tangent.x = (VSREAL)leTangent->GetDirectArray().GetAt(id)[0];  
						Tangent.y = (VSREAL)leTangent->GetDirectArray().GetAt(id)[1];  
						Tangent.z = (VSREAL)leTangent->GetDirectArray().GetAt(id)[2];  
					}  
					break;  
				default:  
					break;  

				}  
				break;  
			}  
		}
	}
	void VSFBXConverter::ReadBinormal(FbxGeometryBase* pMesh, int ctrlPointIndex, int vertexCounter, VSVector3 &Binormal)
	{
		if(pMesh->GetElementBinormalCount() < 1)  
		{  
			return;  
		}  

		FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(0);  
		switch(leBinormal->GetMappingMode())  
		{  
		case FbxGeometryElement::eByControlPoint:  
			{  
				switch(leBinormal->GetReferenceMode())  
				{  
				case FbxGeometryElement::eDirect:  
					{  
						Binormal.x = (VSREAL)leBinormal->GetDirectArray().GetAt(ctrlPointIndex)[0];  
						Binormal.y = (VSREAL)leBinormal->GetDirectArray().GetAt(ctrlPointIndex)[1];  
						Binormal.z = (VSREAL)leBinormal->GetDirectArray().GetAt(ctrlPointIndex)[2];  
					}  
					break;  
				case FbxGeometryElement::eIndexToDirect:  
					{  
						int id = leBinormal->GetIndexArray().GetAt(ctrlPointIndex);  
						Binormal.x = (VSREAL)leBinormal->GetDirectArray().GetAt(id)[0];  
						Binormal.y = (VSREAL)leBinormal->GetDirectArray().GetAt(id)[1];  
						Binormal.z = (VSREAL)leBinormal->GetDirectArray().GetAt(id)[2];  
					}  
					break;  
				default:  
					break;  
				}  
			}  
			break;  
		case FbxGeometryElement::eByPolygonVertex:  
			{  
				switch(leBinormal->GetReferenceMode())  
				{  
				case FbxGeometryElement::eDirect:  
					{  
						Binormal.x = (VSREAL)leBinormal->GetDirectArray().GetAt(vertexCounter)[0];  
						Binormal.y = (VSREAL)leBinormal->GetDirectArray().GetAt(vertexCounter)[1];  
						Binormal.z = (VSREAL)leBinormal->GetDirectArray().GetAt(vertexCounter)[2];  
					}  
					break;  
				case FbxGeometryElement::eIndexToDirect:  
					{  
						int id = leBinormal->GetIndexArray().GetAt(vertexCounter);  
						Binormal.x = (VSREAL)leBinormal->GetDirectArray().GetAt(id)[0];  
						Binormal.y = (VSREAL)leBinormal->GetDirectArray().GetAt(id)[1];  
						Binormal.z = (VSREAL)leBinormal->GetDirectArray().GetAt(id)[2];  
					}  
					break;  
				default:  
					break;  

				}  
				break;  
			}  
		}
	}
	void VSFBXConverter::ReadColor(FbxGeometryBase* pMesh, int ctrlPointIndex, int vertexCounter, VSColorRGBA &Color)
	{  
		if(pMesh->GetElementVertexColorCount() < 1)  
		{  
			return;  
		}  

		FbxGeometryElementVertexColor* pVertexColor = pMesh->GetElementVertexColor(0);  
		switch(pVertexColor->GetMappingMode())  
		{  
		case FbxGeometryElement::eByControlPoint:  
			{  
				switch(pVertexColor->GetReferenceMode())  
				{  
				case FbxGeometryElement::eDirect:  
					{  
						Color.x = (VSREAL)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mRed;  
						Color.y = (VSREAL)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mGreen;  
						Color.z = (VSREAL)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mBlue;  
						Color.w = (VSREAL)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;  
					}  
					break;  

				case FbxGeometryElement::eIndexToDirect:  
					{  
						int id = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);  
						Color.x = (VSREAL)pVertexColor->GetDirectArray().GetAt(id).mRed;  
						Color.y = (VSREAL)pVertexColor->GetDirectArray().GetAt(id).mGreen;  
						Color.z = (VSREAL)pVertexColor->GetDirectArray().GetAt(id).mBlue;  
						Color.w = (VSREAL)pVertexColor->GetDirectArray().GetAt(id).mAlpha;  
					}  
					break;  

				default:  
					break;  
				}  
			}  
			break;  

		case FbxGeometryElement::eByPolygonVertex:  
			{  
				switch (pVertexColor->GetReferenceMode())  
				{  
				case FbxGeometryElement::eDirect:  
					{  
						Color.x = (VSREAL)pVertexColor->GetDirectArray().GetAt(vertexCounter).mRed;  
						Color.y = (VSREAL)pVertexColor->GetDirectArray().GetAt(vertexCounter).mGreen;  
						Color.z = (VSREAL)pVertexColor->GetDirectArray().GetAt(vertexCounter).mBlue;  
						Color.w = (VSREAL)pVertexColor->GetDirectArray().GetAt(vertexCounter).mAlpha;  
					}  
					break;  
				case FbxGeometryElement::eIndexToDirect:  
					{  
						int id = pVertexColor->GetIndexArray().GetAt(vertexCounter);  
						Color.x = (VSREAL)pVertexColor->GetDirectArray().GetAt(id).mRed;  
						Color.y = (VSREAL)pVertexColor->GetDirectArray().GetAt(id).mGreen;  
						Color.z = (VSREAL)pVertexColor->GetDirectArray().GetAt(id).mBlue;  
						Color.w = (VSREAL)pVertexColor->GetDirectArray().GetAt(id).mAlpha;  
					}  
					break;  
				default:  
					break;  
				}  
			}  
			break;  
		}  
	}  

	void VSFBXConverter::ReadUV(FbxGeometryBase* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, VSVector2 & UV)
	{  

		FbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(uvLayer);  

		switch(pVertexUV->GetMappingMode())  
		{  
		case FbxGeometryElement::eByControlPoint:  
			{  
				switch(pVertexUV->GetReferenceMode())  
				{  
				case FbxGeometryElement::eDirect:  
					{  
						UV.x = (VSREAL)pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[0];  
						UV.y = (VSREAL)pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[1];  
					}  
					break;  

				case FbxGeometryElement::eIndexToDirect:  
					{  
						int id = pVertexUV->GetIndexArray().GetAt(ctrlPointIndex);  
						UV.x = (VSREAL)pVertexUV->GetDirectArray().GetAt(id)[0];  
						UV.y = (VSREAL)pVertexUV->GetDirectArray().GetAt(id)[1];  
					}  
					break;  

				default:  
					break;  
				}  
			}  
			break;  

		case FbxGeometryElement::eByPolygonVertex:  
			{  
				switch (pVertexUV->GetReferenceMode())  
				{  
				case FbxGeometryElement::eDirect:
					{
						UV.x = (VSREAL)pVertexUV->GetDirectArray().GetAt(textureUVIndex)[0];
						UV.y = (VSREAL)pVertexUV->GetDirectArray().GetAt(textureUVIndex)[1];
					}
					break;
				case FbxGeometryElement::eIndexToDirect:  
					{  
						UV.x = (VSREAL)pVertexUV->GetDirectArray().GetAt(textureUVIndex)[0];
						UV.y = (VSREAL)pVertexUV->GetDirectArray().GetAt(textureUVIndex)[1];
// 						int id = pVertexUV->GetIndexArray().GetAt(textureUVIndex);
// 						UV.x = (VSREAL)pVertexUV->GetDirectArray().GetAt(id)[0];
// 						UV.y = (VSREAL)pVertexUV->GetDirectArray().GetAt(id)[1];
					}  
					break;  

				default:  
					break;  
				}  
			}  
			break;  
		}  
	} 
	void VSFBXConverter::GetTriangleSmGroupIndex(FbxGeometryBase* pMesh, int triangleCount, VSArray<int> & TriangleSmGroupIndex)
	{  
		FbxLayerElementSmoothing *pSmoothing = pMesh->GetElementSmoothing();
		if (pSmoothing)
		{
			bool bDirectSm = (pSmoothing->GetReferenceMode() == FbxLayerElement::eDirect);

			for(int triangleIndex = 0 ; triangleIndex < triangleCount ; ++triangleIndex)  
			{  
				int SmIndex = bDirectSm ? triangleIndex : pSmoothing->GetIndexArray().GetAt(triangleIndex);
				int iSmoothing = pSmoothing->GetDirectArray().GetAt(SmIndex);

				TriangleSmGroupIndex[triangleIndex] = iSmoothing;  
			}  
		}
		
	}  
	void VSFBXConverter::GetTriangleMaterialIndex(FbxGeometryBase* pMesh, int triangleCount, VSArray<int> & TriangleMaterialIndex)
	{  
		FbxLayerElementMaterial * pMaterial = pMesh->GetElementMaterial();
		if (!pMaterial)
		{
			return ;
		}

		for(int triangleIndex = 0 ; triangleIndex < triangleCount ; ++triangleIndex)  
		{  
			int materialIndex = pMaterial->GetIndexArray().GetAt(triangleIndex);  

			TriangleMaterialIndex[triangleIndex] = materialIndex;  
		}

	}  
	bool VSFBXConverter::IsBoneNumAccept(FbxSkin *pSkin,VSArray<VSUsedName> & Bone,unsigned int VertexIndex)
	{
		VSArray<VSString> TempBone;
		for (unsigned int i = 0 ; i < Bone.GetNum() ; i++)
		{
			TempBone.AddElement(Bone[i].GetString());
		}
		unsigned int BoneNum = TempBone.GetNum();
		int iClusterCount = pSkin->GetClusterCount();
		for (int iCluster = 0; iCluster < iClusterCount; iCluster++)
		{
			FbxCluster *pCluster = pSkin->GetCluster(iCluster);
			FbxNode* pFbxBone = pCluster->GetLink();
			int *iControlPointIndex = pCluster->GetControlPointIndices();
			int iControlPointCount = pCluster->GetControlPointIndicesCount();
			double *pWeights = pCluster->GetControlPointWeights();
			for (int i_Index = 0; i_Index < iControlPointCount; i_Index++)
			{
				if (iControlPointIndex[i_Index] == VertexIndex)
				{
					unsigned int j = 0;
					for(j = 0 ; j < TempBone.GetNum() ; j++)
					{
						if(pFbxBone->GetName() == TempBone[j])
						{
							break;
						}
					}
					if(j == TempBone.GetNum())
					{
						VSString BoneName = pFbxBone->GetName();
						TempBone.AddElement(BoneName);	
						
					}
				}

			}
		}
		if (TempBone.GetNum() > VSResourceManager::GetGpuSkinBoneNum())
		{
			return false;
		}
		return true;

	}
	void VSFBXConverter::BoneSkin(FbxSkin *pSkin,VSArray<VSString> & Bone,VSArray<VSREAL>& Weight,unsigned int VertexIndex)
	{
		int iClusterCount = pSkin->GetClusterCount();
		for (int iCluster = 0; iCluster < iClusterCount; iCluster++)
		{
			FbxCluster *pCluster = pSkin->GetCluster(iCluster);
			FbxNode* pFbxBone = pCluster->GetLink();
			int *iControlPointIndex = pCluster->GetControlPointIndices();
			int iControlPointCount = pCluster->GetControlPointIndicesCount();
			double *pWeights = pCluster->GetControlPointWeights();
			for (int i_Index = 0; i_Index < iControlPointCount; i_Index++)
			{
				if (iControlPointIndex[i_Index] == VertexIndex)
				{
					unsigned int j = 0;
					for(j = 0 ; j < Bone.GetNum() ; j++)
					{
						if(pFbxBone->GetName() == Bone[j])
						{
							Weight[j] += (VSREAL)pWeights[i_Index];
							break;
						}
					}
					if(j == Bone.GetNum())
					{
						VSString BoneName = pFbxBone->GetName();
						Bone.AddElement(BoneName);	
						Weight.AddElement((VSREAL)pWeights[i_Index]);
					}
				}
				
			}
		}

	}

	void VSFBXConverter::GetOffSetMatrix(FbxSkin *pSkin)
	{
		int iClusterCount = pSkin->GetClusterCount();
		for (int iCluster = 0; iCluster < iClusterCount; iCluster++)
		{
			FbxCluster *pCluster = pSkin->GetCluster(iCluster);
			FbxNode* pFbxBone = pCluster->GetLink();
			VSBoneNode * pNode = m_pSkeleton->GetBoneNode(pFbxBone->GetName());
			VSMAC_ASSERT(pNode);
			FbxAMatrix FbxMat,FbxMatLink;
			pCluster->GetTransformLinkMatrix(FbxMatLink);
			pCluster->GetTransformMatrix(FbxMat);
			FbxAMatrix Comine = FbxMatLink.Inverse() * FbxMat;
			VSMatrix3X3W VSMat;
			MaxMatToVSMat(Comine,VSMat);
			pNode->m_OffSetMatrix = VSMat;
			
		}

	}
	bool VSFBXConverter::ProcessMesh(FbxNode* pNode)
	{
		FbxMesh* pMesh = pNode->GetMesh();  

		FbxSkin *pFBXSkin = NULL;
		FbxBlendShape * pMorph = NULL;
		
		m_MorphVertexArray.Clear();
		m_MorphColorArray.Clear();
		m_MorphNormalArray.Clear();
		m_MorphTangentArray.Clear();
		m_MorphBinormalArray.Clear();
		if (m_CurExportType == ET_SKELECTON_MESH)
		{
			int nCountDeformer = pMesh->GetDeformerCount();


			for(int i = 0 ; i < nCountDeformer ; ++i)  
			{  
				FbxDeformer*  pFBXDeformer = pMesh->GetDeformer(i);  

				if(pFBXDeformer == NULL)  
				{  
					continue;  
				}  
				if (pFBXDeformer->GetDeformerType() == FbxDeformer::eSkin && !pFBXSkin)
				{  
					// only use first skin
					pFBXSkin = (FbxSkin*)(pFBXDeformer);
					continue;  
				}  
				else if (pFBXDeformer->GetDeformerType() == FbxDeformer::eBlendShape && !pMorph)
				{
					pMorph = (FbxBlendShape *)(pFBXDeformer);
					continue;
				}			
			}  
			if (!pFBXSkin)
			{
				VSOutPutDebugString("Error : No Skin Info\n");
				printf("Error : No Skin Info\n");
				return false;
			}
			GetOffSetMatrix(pFBXSkin);
		}
		else if (m_CurExportType == ET_STATIC_MESH)
		{
			int nCountDeformer = pMesh->GetDeformerCount();


			for (int i = 0; i < nCountDeformer; ++i)
			{
				FbxDeformer*  pFBXDeformer = pMesh->GetDeformer(i);

				if (pFBXDeformer == NULL)
				{
					continue;
				}
				if (pFBXDeformer->GetDeformerType() == FbxDeformer::eBlendShape && !pMorph)
				{
					pMorph = (FbxBlendShape *)(pFBXDeformer);
					continue;
				}
			}
		}
		FbxGeometryConverter converter(m_pFbxSdkManager); 
		FbxNodeAttribute* ConvertedNode = converter.Triangulate(pMesh, true);
		if (ConvertedNode != NULL && ConvertedNode->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			pMesh = (FbxMesh*)ConvertedNode;
		}
		else
		{
			pMesh = NULL;
		}
		if(pMesh == NULL)  
		{  
			VSOutPutDebugString("Error : No Fbx Mesh Info\n");
			printf("Error : No Fbx Mesh Info Info\n");
			return false; 
		}  
		unsigned int MorphCount = 0;
		if (pMorph)
		{
			int ShapeChannelCount = pMorph->GetBlendShapeChannelCount();
			m_MorphName.Clear();
			for (int ShapeIndex = 0; ShapeIndex < ShapeChannelCount; ShapeIndex++)
			{
				FbxBlendShapeChannel * pBlendShapeChannel = pMorph->GetBlendShapeChannel(ShapeIndex);
				if (pBlendShapeChannel->GetTargetShapeCount() > 0)
				{
					m_MorphName.AddElement(pBlendShapeChannel->GetName());
				}
				
			}

			m_MorphVertexArray.SetBufferNum(m_MorphName.GetNum());
			m_MorphColorArray.SetBufferNum(m_MorphName.GetNum());
			m_MorphNormalArray.SetBufferNum(m_MorphName.GetNum());
			m_MorphTangentArray.SetBufferNum(m_MorphName.GetNum());
			m_MorphBinormalArray.SetBufferNum(m_MorphName.GetNum());
		}


		int triangleCount = pMesh->GetPolygonCount(); 

		VSOutPutDebugString("Triangle Count %d\n", triangleCount);
		printf("Triangle Count %d\n", triangleCount);

		VSArray<int> TriangleMaterialIndex;
		TriangleMaterialIndex.SetBufferNum(triangleCount);
		VSMemset(TriangleMaterialIndex.GetBuffer() , 0 ,triangleCount * sizeof(int));
		GetTriangleMaterialIndex(pMesh,triangleCount,TriangleMaterialIndex);  

		VSArray<int> TriangleSmGroupIndex;
		TriangleSmGroupIndex.SetBufferNum(triangleCount);
		VSMemset(TriangleSmGroupIndex.GetBuffer() , 0 ,triangleCount * sizeof(int));
		GetTriangleSmGroupIndex(pMesh,triangleCount,TriangleSmGroupIndex);
		

		int  TexCoordNum = pMesh->GetElementUVCount();
		VSArray<VSVector3> FaceNormalArray;
		VSArray<VSVector3> FaceTangentArray;
		VSArray<VSVector3> FaceBinromalArray;

		VSArray<VSArray<VSVector3>> MorphFaceNormalArray;
		VSArray<VSArray<VSVector3>> MorphFaceTangentArray;
		VSArray<VSArray<VSVector3>> MorphFaceBinromalArray;
		if (pMorph)
		{

			MorphFaceNormalArray.SetBufferNum(m_MorphName.GetNum());
			MorphFaceTangentArray.SetBufferNum(m_MorphName.GetNum());
			MorphFaceBinromalArray.SetBufferNum(m_MorphName.GetNum());

		}
		if (!m_bUseFbxNormal)
		{
			FaceNormalArray.SetBufferNum(triangleCount);
			if (TexCoordNum)
			{
				FaceTangentArray.SetBufferNum(triangleCount);
				FaceBinromalArray.SetBufferNum(triangleCount);
			}

			VSOutPutDebugString("Read Vertex UV Normal Info\n");
			printf("Read Vertex UV Normal Info\n");
			for (int i = 0; i < triangleCount; ++i)
			{
				VSVector3 V[3];
				VSVector2 TV[3];
				for (int j = 2; j >= 0; j--)
				{
					int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);

					// Read the vertex  
					ReadVertex(pMesh, ctrlPointIndex, V[j]);

					if (TexCoordNum > 0)
					{	// use uv0 to compute TNB
						ReadUV(pMesh, ctrlPointIndex, pMesh->GetTextureUVIndex(i, j), 0, TV[j]);
					}

				}
				VSVector3 N1 = V[0] - V[1];
				VSVector3 N2 = V[0] - V[2];

				VSVector3 T, B, N;
				N.Cross(N1, N2);
				FaceNormalArray[i] = N;
				FaceNormalArray[i].Normalize();
				if (TexCoordNum)
				{
					CreateTangentAndBinormal(V[0], V[1], V[2], TV[0], TV[1], TV[2], N, T, B);

					FaceTangentArray[i] = T;
					FaceTangentArray[i].Normalize();
					FaceBinromalArray[i] = B;
					FaceBinromalArray[i].Normalize();
				}
			}
			
			if (pMorph)
			{
				int ShapeChannelCount = pMorph->GetBlendShapeChannelCount();
				for (int ShapeIndex = 0; ShapeIndex < ShapeChannelCount; ShapeIndex++)
				{				
					if(pMorph->GetBlendShapeChannel(ShapeIndex)->GetTargetShapeCount() == 0)
						continue;
					//only Get index 0
					FbxShape * pShape = pMorph->GetBlendShapeChannel(ShapeIndex)->GetTargetShape(0);
					MorphFaceNormalArray[ShapeIndex].SetBufferNum(triangleCount);
					if (TexCoordNum)
					{
						MorphFaceTangentArray[ShapeIndex].SetBufferNum(triangleCount);
						MorphFaceBinromalArray[ShapeIndex].SetBufferNum(triangleCount);
					}

					VSOutPutDebugString("Read Vertex Morph UV Normal Info\n");
					printf("Read Vertex Morph UV Normal Info\n");
					for (int i = 0; i < triangleCount; ++i)
					{
						VSVector3 V[3];
						VSVector2 TV[3];
						for (int j = 2; j >= 0; j--)
						{
							int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);

							// Read the vertex  
							ReadVertex(pShape, ctrlPointIndex, V[j]);

							if (TexCoordNum > 0)
							{	// use uv0 to compute TNB
								ReadUV(pMesh, ctrlPointIndex, pMesh->GetTextureUVIndex(i, j), 0, TV[j]);
							}

						}
						VSVector3 N1 = V[0] - V[1];
						VSVector3 N2 = V[0] - V[2];

						VSVector3 T, B, N;
						N.Cross(N1, N2);
						MorphFaceNormalArray[ShapeIndex][i] = N;
						MorphFaceNormalArray[ShapeIndex][i].Normalize();
						if (TexCoordNum)
						{
							CreateTangentAndBinormal(V[0], V[1], V[2], TV[0], TV[1], TV[2], N, T, B);

							MorphFaceTangentArray[ShapeIndex][i] = T;
							MorphFaceTangentArray[ShapeIndex][i].Normalize();
							MorphFaceBinromalArray[ShapeIndex][i] = B;
							MorphFaceBinromalArray[ShapeIndex][i].Normalize();
						}
					}
				}
			}
		}

		int MaterialCount = pNode->GetMaterialCount();
		for (int k = 0 ; (k == 0 || k < MaterialCount) ; k++)
		{
			ClearAllVertexInfo();
			for(int i = 0 ; i < triangleCount ; ++i)  
			{
				if(TriangleMaterialIndex[i] == k)
				{
					if (pFBXSkin)
					{
						for (int j = 2; j >= 0; j--)
						{
							int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);
							if (!IsBoneNumAccept(pFBXSkin, m_MeshBoneNode, ctrlPointIndex))
							{
								VSString Name = pNode->GetName();
								if (MaterialCount > 0)
								{
									Name = Name + _T("_") + pNode->GetMaterial(k)->GetName() + +_T("_SubMesh");
								}
								CreateMesh(Name, TexCoordNum, (pFBXSkin != NULL));
								ClearAllVertexInfo();
								break;
								

							}
						}
					}
					
					for(int j = 2 ; j >=0 ; j--)
					{
						// Read the vertex 
						VSVector3 V;			
						int ctrlPointIndex = pMesh->GetPolygonVertex(i , j);  
						 
						ReadVertex(pMesh , ctrlPointIndex , V); 

						// Read Morph Vertex
						VSArray<VSVector3> MorphV;
						if (pMorph)
						{
							int ShapeChannelCount = pMorph->GetBlendShapeChannelCount();
							MorphV.SetBufferNum(m_MorphName.GetNum());

							for (int ShapeIndex = 0; ShapeIndex < ShapeChannelCount; ShapeIndex++)
							{
								if (pMorph->GetBlendShapeChannel(ShapeIndex)->GetTargetShapeCount() == 0)
									continue;
								FbxShape * pShape = pMorph->GetBlendShapeChannel(ShapeIndex)->GetTargetShape(0);
								  
								ReadVertex(pShape, ctrlPointIndex, MorphV[ShapeIndex]);
							}
						}

						//read uv
						VSArray<VSVector2> UVArray;
						for (int uv = 0 ; uv < TexCoordNum ;uv++)
						{
							VSVector2 UV;
							ReadUV(pMesh , ctrlPointIndex , pMesh->GetTextureUVIndex(i, j) ,uv,UV); 
							UVArray.AddElement(UV);
						}

						//read color
						VSColorRGBA Color;
						ReadColor(pMesh,ctrlPointIndex,j + 3 * i,Color);

						// Read Morph Color
						VSArray<VSColorRGBA> MorphColor;
						if (pMorph)
						{
							int ShapeChannelCount = pMorph->GetBlendShapeChannelCount();
							MorphColor.SetBufferNum(m_MorphName.GetNum());

							for (int ShapeIndex = 0; ShapeIndex < ShapeChannelCount; ShapeIndex++)
							{
								if (pMorph->GetBlendShapeChannel(ShapeIndex)->GetTargetShapeCount() == 0)
									continue;
								FbxShape * pShape = pMorph->GetBlendShapeChannel(ShapeIndex)->GetTargetShape(0);
								 
								ReadColor(pShape, ctrlPointIndex, j + 3 * i, MorphColor[ShapeIndex]);
							}
						}

						//Read Normal
						VSVector3 N, T, B;
						if (m_bUseFbxNormal)
						{
							ReadNormal(pMesh, ctrlPointIndex, j + 3 * i, N);
							if (TexCoordNum)
							{
								ReadTangent(pMesh, ctrlPointIndex, j + 3 * i, T);
								ReadBinormal(pMesh, ctrlPointIndex, j + 3 * i, B);
							}
						}
						else
						{
							N = FaceNormalArray[i];
							if (TexCoordNum)
							{
								T = FaceTangentArray[i];
								B = FaceBinromalArray[i];
							}
						}
											
						// Read Morph Normal
						VSArray<VSVector3> MorphN, MorphT, MorphB;
						if (pMorph)
						{
							int ShapeChannelCount = pMorph->GetBlendShapeChannelCount();
							MorphN.SetBufferNum(m_MorphName.GetNum());
							MorphT.SetBufferNum(m_MorphName.GetNum());
							MorphB.SetBufferNum(m_MorphName.GetNum());

							for (int ShapeIndex = 0; ShapeIndex < ShapeChannelCount; ShapeIndex++)
							{
								if (pMorph->GetBlendShapeChannel(ShapeIndex)->GetTargetShapeCount() == 0)
									continue;
								FbxShape * pShape = pMorph->GetBlendShapeChannel(ShapeIndex)->GetTargetShape(0);
								
								if (m_bUseFbxNormal)
								{
									ReadNormal(pShape, ctrlPointIndex, j + 3 * i, MorphN[ShapeIndex]);
									if (TexCoordNum)
									{
										ReadTangent(pShape, ctrlPointIndex, j + 3 * i, MorphT[ShapeIndex]);
										ReadBinormal(pShape, ctrlPointIndex, j + 3 * i, MorphB[ShapeIndex]);
									}
								}
								else
								{
									MorphN[ShapeIndex] = MorphFaceNormalArray[ShapeIndex][i];
									if (TexCoordNum)
									{
										MorphT[ShapeIndex] = MorphFaceTangentArray[ShapeIndex][i];
										MorphB[ShapeIndex] = MorphFaceBinromalArray[ShapeIndex][i];
									}
								}
							}
						}

						// Sum Same Vertex And SmGroup Normal
						for(unsigned int l = 0 ; l < m_VertexArray.GetNum() ; l++)
						{
							if(m_VertexArray[l] == V)
							{
								if(m_VertexSmGroupArray[l] == TriangleSmGroupIndex[i])
								{
									m_NormalArray[l] = N + m_NormalArray[l];
									N = m_NormalArray[l];
									if(TexCoordNum)
									{
										m_TangentArray[l] = T + m_TangentArray[l];
										T = m_TangentArray[l];

										m_BinormalArray[l] = B + m_BinormalArray[l];
										B = m_BinormalArray[l];
									}

									if (pMorph)
									{
										int ShapeChannelCount = pMorph->GetBlendShapeChannelCount();
										for (int ShapeIndex = 0; ShapeIndex < ShapeChannelCount; ShapeIndex++)
										{
											m_MorphNormalArray[ShapeIndex][l] = MorphN[ShapeIndex] + m_MorphNormalArray[ShapeIndex][l];
											MorphN[ShapeIndex] = m_MorphNormalArray[ShapeIndex][l];
											if (TexCoordNum)
											{
												m_MorphTangentArray[ShapeIndex][l] = MorphT[ShapeIndex] + m_MorphTangentArray[ShapeIndex][l];
												MorphT[ShapeIndex] = m_MorphTangentArray[ShapeIndex][l];

												m_MorphBinormalArray[ShapeIndex][l] = MorphB[ShapeIndex] + m_MorphBinormalArray[ShapeIndex][l];
												MorphB[ShapeIndex] = m_MorphBinormalArray[ShapeIndex][l];
											}
										}
									}

								}
							}
						}
						unsigned int f = 0;			
						for(f = 0 ; f < m_VertexArray.GetNum() ; f++)
						{
							if(m_VertexArray[f] == V)
							{
								if(m_VertexSmGroupArray[f] == TriangleSmGroupIndex[i])
								{

									int uiChannel = 0 ;
									for(uiChannel = 0;  uiChannel < TexCoordNum; uiChannel++)
									{
										if(m_TexCoordArray[uiChannel][f] == UVArray[uiChannel])
										{
											continue;
										}
										else
										{
											break;
										}
									}
									if(uiChannel == TexCoordNum)
									{
										break;
									}
								}
							}

						}//for f vertex

						//如果f走出了上面的循环等于 m_VertexArray.GetNum()表明根本没有这个顶点，则添加
						if(f == m_VertexArray.GetNum())
						{
							//分别添加位置，GroupID,纹理坐标，法向量
							m_VertexArray.AddElement(V);
							if (pMorph)
							{
								int ShapeChannelCount = pMorph->GetBlendShapeChannelCount();
								for (int ShapeIndex = 0; ShapeIndex < ShapeChannelCount; ShapeIndex++)
								{
									m_MorphVertexArray[ShapeIndex].AddElement(MorphV[ShapeIndex]);
								}
							}
							m_VertexSmGroupArray.AddElement(TriangleSmGroupIndex[i]);
							for(int uiChannel = 0;  uiChannel < TexCoordNum; uiChannel++)
							{
								m_TexCoordArray[uiChannel].AddElement(UVArray[uiChannel]);
							}
							m_NormalArray.AddElement(N);
							if(TexCoordNum)
							{
								m_TangentArray.AddElement(T);
								m_BinormalArray.AddElement(B);
							}

							if (pMorph)
							{
								int ShapeChannelCount = pMorph->GetBlendShapeChannelCount();
								for (int ShapeIndex = 0; ShapeIndex < ShapeChannelCount; ShapeIndex++)
								{
									m_MorphNormalArray[ShapeIndex].AddElement(MorphN[ShapeIndex]);
									if (TexCoordNum)
									{
										m_MorphBinormalArray[ShapeIndex].AddElement(MorphB[ShapeIndex]);
										m_MorphTangentArray[ShapeIndex].AddElement(MorphT[ShapeIndex]);
									}
								}
							}

							if (pFBXSkin)
							{
								//获得影响当前顶点的所有骨头和权重
								VSArray<VSString> BoneTemp;
								VSArray<VSREAL>  Weight;
								BoneSkin(pFBXSkin,BoneTemp,Weight,ctrlPointIndex);

								//没有骨头影响该顶点
								if(BoneTemp.GetNum() == 0)
								{
									return false;
								}
								//如果影响整个顶点的骨头大于4个，则把权重小的骨头去掉，减少到4个骨头
								while(BoneTemp.GetNum() > 4)
								{
									VSREAL MinWeight = Weight[0];
									unsigned int MinWeightIndex = 0;
									for(unsigned int uiBoneTemp = 1 ; uiBoneTemp < BoneTemp.GetNum() ; uiBoneTemp++)
									{
										if(Weight[uiBoneTemp] < MinWeight)
										{
											MinWeight = Weight[uiBoneTemp];
											MinWeightIndex = uiBoneTemp;
										}

									}

									BoneTemp.Erase(MinWeightIndex);
									Weight.Erase(MinWeightIndex);					 	
								}

								//再一次过滤权重过小的骨头，这次过滤后，不能保证影响顶点的骨头数为4个
								for(unsigned int uiBoneTemp = 0 ; uiBoneTemp < BoneTemp.GetNum() ; uiBoneTemp++)
								{
									if(Weight[uiBoneTemp] < EPSILON_E4)
									{
										BoneTemp.Erase(uiBoneTemp);
										Weight.Erase(uiBoneTemp);	
										uiBoneTemp--;
									}

								}

								//重新计算权重
								VSREAL TotleWeight = 0;
								for(unsigned int uiBoneTemp = 0 ; uiBoneTemp < BoneTemp.GetNum() ; uiBoneTemp++)
								{
									TotleWeight += Weight[uiBoneTemp];
								}
								for(unsigned int uiBoneTemp = 0 ; uiBoneTemp < BoneTemp.GetNum() ; uiBoneTemp++)
								{
									if(TotleWeight > EPSILON_E4)
										Weight[uiBoneTemp] = Weight[uiBoneTemp] /TotleWeight;
								}

								//把骨头放入Mesh骨头列表，并未这个顶点生成相对于这个骨头列表的索引和权重
								//一个顶点最多支持4个骨头，所以用VSVector3W
								VSVector3W BoneIndexTemp(0.0f,0.0f,0.0f,0.0f);
								VSVector3W BoneWeightTemp(0.0f,0.0f,0.0f,0.0f);
								for (unsigned int uiBoneTemp = 0 ; uiBoneTemp < BoneTemp.GetNum() ; uiBoneTemp++)
								{
									VSBoneNode * pBoneNode = m_pSkeleton->GetBoneNode(BoneTemp[uiBoneTemp]);
									VSMAC_ASSERT(pBoneNode);
									//没有找到对应的骨头
									if(!pBoneNode)
									{
										return false;
									}
									
									unsigned int uiBoneIndex = 0;
									for(uiBoneIndex = 0 ; uiBoneIndex < m_MeshBoneNode.GetNum() ; uiBoneIndex++)
									{
										if(m_MeshBoneNode[uiBoneIndex] == BoneTemp[uiBoneTemp])
										{
											break;
										}

									}
									//把影响这个MESH的所有骨头都添加到骨头列表中
									if(uiBoneIndex == m_MeshBoneNode.GetNum())
									{
										m_MeshBoneNode.AddElement(BoneTemp[uiBoneTemp]);
									}
									//记录影响整个顶点的骨头所在骨头列表中的索引和权重
									BoneIndexTemp.m[uiBoneTemp] = uiBoneIndex * 1.0f;
									BoneWeightTemp.m[uiBoneTemp] = Weight[uiBoneTemp];

								}
								//加入权重和骨头索引
								m_BoneIndex.AddElement(BoneIndexTemp);
								m_BoneWeight.AddElement(BoneWeightTemp);
							}
						}

						//记录三角形面的索引
						m_IndexArray.AddElement(f);
					}//for j triangle index
				}
			}//for i triangle num

			VSString Name = pNode->GetName();
			if (MaterialCount > 0)
			{
				Name = Name + _T("_") + pNode->GetMaterial(k)->GetName();
			}
			
			CreateMesh(Name,TexCoordNum, (pFBXSkin != NULL));

		}//for k sub mesh
		
		
		return true;

		
	}

	void VSFBXConverter::Orthogonal(VSVector3 & Normal,VSVector3 & Tangent,VSVector3 & Binromal)
	{

		VSVector3 N1 = Tangent;
		VSVector3 N2 = Binromal;

		Tangent = N1 - Normal * (( N1 * Normal) / (Normal * Normal));

		Binromal = N2 - Normal * (( N2 * Normal) / (Normal * Normal)) - Tangent * (( N2 * Tangent) / (Tangent * Tangent));

		Normal.Normalize();
		Tangent.Normalize();
		Binromal.Normalize();

	}
	bool VSFBXConverter::CreateMesh(VSString & Name,unsigned int TexCoordNum, bool HasSkin)
	{
		if(m_VertexArray.GetNum() == 0)
			return 0;

		VSOutPutDebugString("CreateMesh %s\n", Name.GetBuffer());
		printf("CreateMesh %s\n", Name.GetBuffer());

		
		VSOutPutDebugString("Orthogonal T B N \n");
		printf("Orthogonal T B N \n");
		//正交化
		if (TexCoordNum && !m_bUseFbxNormal)
		{
			for(unsigned int v = 0 ; v < m_VertexArray.GetNum() ; v++)
			{
				Orthogonal(m_NormalArray[v],m_TangentArray[v],m_BinormalArray[v]);
			}
			for (unsigned int ShapeIndex = 0; ShapeIndex < m_MorphVertexArray.GetNum(); ShapeIndex++)
			{
				for (unsigned int v = 0; v < m_MorphVertexArray[ShapeIndex].GetNum(); v++)
				{
					Orthogonal(m_MorphNormalArray[ShapeIndex][v], m_MorphTangentArray[ShapeIndex][v], m_MorphBinormalArray[ShapeIndex][v]);
				}
			}
		}
		

		VSOutPutDebugString("Vertex Num %d\n", m_VertexArray.GetNum());
		printf("Vertex Num %d\n", m_VertexArray.GetNum());
		//创建位置BUFFER
		VSDataBufferPtr pVertexDate = NULL;
		pVertexDate = VS_NEW VSDataBuffer;
		if(!pVertexDate)
			return 0;

	
		pVertexDate->SetDate(&m_VertexArray[0],(unsigned int)m_VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);

		VSArray<VSDataBufferPtr> pMorphVertexData;
		pMorphVertexData.SetBufferNum(m_MorphVertexArray.GetNum());
		for (unsigned int ShapeIndex = 0; ShapeIndex < m_MorphVertexArray.GetNum(); ShapeIndex++)
		{
			if (m_MorphVertexArray[ShapeIndex].GetNum() == 0)
			{
				pMorphVertexData[ShapeIndex] = NULL;
			}
			else
			{
				pMorphVertexData[ShapeIndex] = VS_NEW VSDataBuffer;
				if (!pMorphVertexData[ShapeIndex])
					return 0;

				pMorphVertexData[ShapeIndex]->SetDate(&m_MorphVertexArray[ShapeIndex][0], (unsigned int)m_MorphVertexArray[ShapeIndex].GetNum(), VSDataBuffer::DT_FLOAT32_3);
			}
			
		}

		//创建纹理坐标BUFFER
		VSDataBufferPtr pTexcoord[TEXLEVEL];
		for (unsigned int uiChannel = 0 ;  uiChannel < TexCoordNum ; uiChannel++)
		{
			for (unsigned int i = 0 ; i < m_TexCoordArray[uiChannel].GetNum() ; i++)
			{
				m_TexCoordArray[uiChannel][i].y = 1.0f - m_TexCoordArray[uiChannel][i].y;
			}
		}
		for(unsigned int uiChannel = 0 ;  uiChannel < TexCoordNum ; uiChannel++)
		{
			pTexcoord[uiChannel] = VS_NEW VSDataBuffer;
			if(!pTexcoord)
				return 0;
			if (m_CurExportPara & EP_UV_COMPRESS)
			{
				VSArray<DWORD> CompressDate;
				CompressDate.SetBufferNum(m_TexCoordArray[uiChannel].GetNum());
				for (unsigned int i = 0 ; i < m_TexCoordArray[uiChannel].GetNum() ;i++)
				{
					unsigned short U = FloatToHalf(m_TexCoordArray[uiChannel][i].x);
					unsigned short V = FloatToHalf(m_TexCoordArray[uiChannel][i].y);
					CompressDate[i] = (DWORD)((V << 16 )|U) ;
				}
				pTexcoord[uiChannel]->SetDate(&CompressDate[0],CompressDate.GetNum(),VSDataBuffer::DT_FLOAT16_2);
			}
			else
			{
				pTexcoord[uiChannel]->SetDate(&m_TexCoordArray[uiChannel][0],(unsigned int)m_TexCoordArray[uiChannel].GetNum(),VSDataBuffer::DT_FLOAT32_2);
			}
		}


		//创建法向量BUFFER
		VSDataBufferPtr pNormalDate = NULL;
		pNormalDate = VS_NEW VSDataBuffer;
		if(!pNormalDate)
			return 0;

		if (m_CurExportPara & EP_NORMAL_COMPRESS)
		{
			VSArray<DWORD> CompressDate;
			CompressDate.SetBufferNum(m_NormalArray.GetNum());
			for (unsigned int i = 0 ; i < m_NormalArray.GetNum() ;i++)
			{
				VSVector3W Temp(m_NormalArray[i]);
				Temp = (Temp + 1.0f) * 0.5f;
				CompressDate[i] = Temp.GetDWABGR();
			}
			pNormalDate->SetDate(&CompressDate[0],CompressDate.GetNum(),VSDataBuffer::DT_UBYTE4N);
		}
		else
		{
			pNormalDate->SetDate(&m_NormalArray[0],(unsigned int)m_NormalArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);
		}

		VSArray<VSDataBufferPtr> pMorphNormalDate;
		pMorphNormalDate.SetBufferNum(m_MorphVertexArray.GetNum());
		for (unsigned int ShapeIndex = 0; ShapeIndex < m_MorphVertexArray.GetNum(); ShapeIndex++)
		{
			if (m_MorphVertexArray[ShapeIndex].GetNum() == 0)
			{
				pMorphNormalDate[ShapeIndex] = NULL;
			}
			else
			{
				pMorphNormalDate[ShapeIndex] = VS_NEW VSDataBuffer;
				if (!pMorphNormalDate[ShapeIndex])
					return 0;
				if (m_CurExportPara & EP_NORMAL_COMPRESS)
				{
					VSArray<DWORD> CompressDate;
					CompressDate.SetBufferNum(m_MorphNormalArray[ShapeIndex].GetNum());
					for (unsigned int i = 0; i < m_MorphNormalArray[ShapeIndex].GetNum(); i++)
					{
						VSVector3W Temp(m_MorphNormalArray[ShapeIndex][i]);
						Temp = (Temp + 1.0f) * 0.5f;
						CompressDate[i] = Temp.GetDWABGR();
					}
					pMorphNormalDate[ShapeIndex]->SetDate(&CompressDate[0], CompressDate.GetNum(), VSDataBuffer::DT_UBYTE4N);
				}
				else
				{
					pMorphNormalDate[ShapeIndex]->SetDate(&m_MorphNormalArray[ShapeIndex][0], (unsigned int)m_MorphNormalArray[ShapeIndex].GetNum(), VSDataBuffer::DT_FLOAT32_3);
				}
			}
			
			
		}


		//创建切向量BUFFER
		VSDataBufferPtr pTangentDate = NULL;
		VSArray<VSDataBufferPtr> pMorphTangentDate;
	
		if(TexCoordNum)
		{
			pTangentDate = VS_NEW VSDataBuffer;
			if(!pTangentDate)
				return 0;
			if (m_CurExportPara & EP_NORMAL_COMPRESS)
			{
				VSArray<DWORD> CompressDate;
				CompressDate.SetBufferNum(m_TangentArray.GetNum());
				for (unsigned int i = 0 ; i < m_TangentArray.GetNum() ;i++)
				{
					VSVector3W Temp(m_TangentArray[i]);
					VSVector3 Binormal;
					Binormal.Cross(m_NormalArray[i],m_TangentArray[i]);
					Temp.w = Binormal.Dot(m_BinormalArray[i]);
					if (Temp.w > 0)
					{
						Temp.w = 1.0f;
					}
					else
					{
						Temp.w = -1.0f;
					}
					Temp = (Temp + 1.0f) * 0.5f;
					CompressDate[i] = Temp.GetDWABGR();
				}
				pTangentDate->SetDate(&CompressDate[0],CompressDate.GetNum(),VSDataBuffer::DT_UBYTE4N);
			}
			else
			{
				pTangentDate->SetDate(&m_TangentArray[0],(unsigned int)m_TangentArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);
			}

			pMorphTangentDate.SetBufferNum(m_MorphVertexArray.GetNum());
			for (unsigned int ShapeIndex = 0; ShapeIndex < m_MorphVertexArray.GetNum(); ShapeIndex++)
			{
				if (m_MorphVertexArray[ShapeIndex].GetNum() == 0)
				{
					pMorphTangentDate[ShapeIndex] = NULL;
				}
				else
				{
					pMorphTangentDate[ShapeIndex] = VS_NEW VSDataBuffer;
					if (!pMorphTangentDate[ShapeIndex])
						return 0;

					if (m_CurExportPara & EP_NORMAL_COMPRESS)
					{
						VSArray<DWORD> CompressDate;
						CompressDate.SetBufferNum(m_MorphTangentArray[ShapeIndex].GetNum());
						for (unsigned int i = 0; i < m_MorphTangentArray[ShapeIndex].GetNum(); i++)
						{
							VSVector3W Temp(m_MorphTangentArray[ShapeIndex][i]);
							VSVector3 Binormal;
							Binormal.Cross(m_MorphNormalArray[ShapeIndex][i], m_MorphTangentArray[ShapeIndex][i]);
							Temp.w = Binormal.Dot(m_MorphBinormalArray[ShapeIndex][i]);
							if (Temp.w > 0)
							{
								Temp.w = 1.0f;
							}
							else
							{
								Temp.w = -1.0f;
							}
							Temp = (Temp + 1.0f) * 0.5f;
							CompressDate[i] = Temp.GetDWABGR();
						}
						pMorphTangentDate[ShapeIndex]->SetDate(&CompressDate[0], CompressDate.GetNum(), VSDataBuffer::DT_UBYTE4N);
					}
					else
					{
						pMorphTangentDate[ShapeIndex]->SetDate(&m_MorphTangentArray[ShapeIndex][0], (unsigned int)m_MorphTangentArray[ShapeIndex].GetNum(), VSDataBuffer::DT_FLOAT32_3);
					}
				}
				
			}

		}

		VSDataBufferPtr pBinormalDate = NULL;
		VSArray<VSDataBufferPtr> pMorphBinormalData;

		if(TexCoordNum && !(m_CurExportPara & EP_NORMAL_COMPRESS))
		{
			pBinormalDate = VS_NEW VSDataBuffer;
			if(!pBinormalDate)
				return 0;
			pBinormalDate->SetDate(&m_BinormalArray[0],(unsigned int)m_BinormalArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);


			
			pMorphBinormalData.SetBufferNum(m_MorphVertexArray.GetNum());
			for (unsigned int ShapeIndex = 0; ShapeIndex < m_MorphVertexArray.GetNum(); ShapeIndex++)
			{
				pMorphBinormalData[ShapeIndex] = VS_NEW VSDataBuffer;
				if (!pMorphBinormalData[ShapeIndex])
					return 0;

				pMorphBinormalData[ShapeIndex]->SetDate(&m_MorphBinormalArray[ShapeIndex][0], (unsigned int)m_MorphBinormalArray[ShapeIndex].GetNum(), VSDataBuffer::DT_FLOAT32_3);
			}
		}

		//添加权重
		VSDataBufferPtr pBoneWeight = NULL;
		if(HasSkin)
		{
			pBoneWeight = VS_NEW VSDataBuffer;
			if(!pBoneWeight)
				return 0;
			if (m_CurExportPara & EP_SKIN_COMPRESS)
			{
				VSArray<DWORD> CompressDate;
				CompressDate.SetBufferNum(m_BoneWeight.GetNum());
				for (unsigned int i = 0 ; i < m_BoneWeight.GetNum() ;i++)
				{

					CompressDate[i] = m_BoneWeight[i].GetDWABGR();
				}
				pBoneWeight->SetDate(&CompressDate[0],CompressDate.GetNum(),VSDataBuffer::DT_UBYTE4N);
			}
			else
			{
				pBoneWeight->SetDate(&m_BoneWeight[0],(unsigned int)m_BoneWeight.GetNum(),VSDataBuffer::DT_FLOAT32_4);
			}

		}

		//添加骨头索引
		VSDataBufferPtr pBoneIndex = NULL;
		if(HasSkin)
		{
			pBoneIndex = VS_NEW VSDataBuffer;
			if(!pBoneIndex)
				return 0;
			if (m_CurExportPara & EP_SKIN_COMPRESS)
			{
				VSArray<DWORD> CompressDate;
				CompressDate.SetBufferNum(m_BoneIndex.GetNum());
				for (unsigned int i = 0 ; i < m_BoneIndex.GetNum() ;i++)
				{
					unsigned char R = (unsigned char)m_BoneIndex[i].r;
					unsigned char G = (unsigned char)m_BoneIndex[i].g;
					unsigned char B = (unsigned char)m_BoneIndex[i].b;
					unsigned char A = (unsigned char)m_BoneIndex[i].a;

					CompressDate[i] = VSDWCOLORABGR(A,R,G,B);
				}
				pBoneIndex->SetDate(&CompressDate[0],CompressDate.GetNum(),VSDataBuffer::DT_UBYTE4);
			}
			else
			{
				pBoneIndex->SetDate(&m_BoneIndex[0],(unsigned int)m_BoneIndex.GetNum(),VSDataBuffer::DT_FLOAT32_4);
			}


		}
		//创建索引BUUFER
		VSDataBufferPtr pIndex = NULL;
		pIndex = VS_NEW VSDataBuffer;
		if(!pIndex)
			return 0;
		if (m_VertexArray.GetNum() > 65535)
		{
			pIndex->SetDate(&m_IndexArray[0],(unsigned int)m_IndexArray.GetNum(),VSDataBuffer::DT_UINT);
		}
		else
		{
			VSArray<VSUSHORT_INDEX> IndexArrayTemp;
			IndexArrayTemp.SetBufferNum(m_IndexArray.GetNum());
			for (unsigned int i = 0 ; i < m_IndexArray.GetNum() ;i++)
			{
				IndexArrayTemp[i] = m_IndexArray[i];
			}
			pIndex->SetDate(&IndexArrayTemp[0],(unsigned int)IndexArrayTemp.GetNum(),VSDataBuffer::DT_USHORT);
		}


		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = NULL;
		pVertexBuffer = VS_NEW VSVertexBuffer(true);
		if(!pVertexBuffer)
			return 0;

		//添加顶点
		pVertexBuffer->SetDate(pVertexDate,VSVertexFormat::VF_POSITION);
		//添加纹理坐标
		for(unsigned int uiChannel = 0 ;  uiChannel < TexCoordNum ; uiChannel++)
		{
			if(pTexcoord[uiChannel])
				pVertexBuffer->SetDate(pTexcoord[uiChannel],VSVertexFormat::VF_TEXCOORD);
		}
		//添加法向量
		pVertexBuffer->SetDate(pNormalDate,VSVertexFormat::VF_NORMAL);
		if(TexCoordNum)
		{
			pVertexBuffer->SetDate(pTangentDate,VSVertexFormat::VF_TANGENT);
			pVertexBuffer->SetDate(pBinormalDate,VSVertexFormat::VF_BINORMAL);
		}
		if(HasSkin)
		{
			pVertexBuffer->SetDate(pBoneIndex,VSVertexFormat::VF_BLENDINDICES);
			pVertexBuffer->SetDate(pBoneWeight,VSVertexFormat::VF_BLENDWEIGHT);
		}

		VSArray<VSVertexBufferPtr> pMorphVertexBuffer;
		pMorphVertexBuffer.SetBufferNum(m_MorphVertexArray.GetNum());
		for (unsigned int ShapeIndex = 0; ShapeIndex < m_MorphVertexArray.GetNum(); ShapeIndex++)
		{
			if (m_MorphVertexArray[ShapeIndex].GetNum() == 0)
			{
				pMorphVertexBuffer[ShapeIndex] = NULL;
			}
			else
			{

				pMorphVertexBuffer[ShapeIndex] = VS_NEW VSVertexBuffer(true);
				if (!pMorphVertexBuffer[ShapeIndex])
					return 0;

				//添加顶点
				pMorphVertexBuffer[ShapeIndex]->SetDate(pMorphVertexData[ShapeIndex], VSVertexFormat::VF_POSITION);

				//添加法向量
				pMorphVertexBuffer[ShapeIndex]->SetDate(pMorphNormalDate[ShapeIndex], VSVertexFormat::VF_NORMAL);
				if (TexCoordNum)
				{
					pMorphVertexBuffer[ShapeIndex]->SetDate(pMorphTangentDate[ShapeIndex], VSVertexFormat::VF_TANGENT);
					if (pMorphBinormalData.GetNum() > 0)
					{
						pMorphVertexBuffer[ShapeIndex]->SetDate(pMorphBinormalData[ShapeIndex], VSVertexFormat::VF_BINORMAL);
					}

				}
			}
		}


		//创建Mesh
		VSTriangleSetPtr pVSMesh = NULL ;
		pVSMesh = VS_NEW VSTriangleSet();
		if(!pVSMesh)
			return 0;

		//设置顶点和索引BUFFER
		pVSMesh->SetVertexBuffer(pVertexBuffer);

		VSIndexBufferPtr pIndexBuffer = VS_NEW VSIndexBuffer();

		pIndexBuffer->SetDate(pIndex);
		pVSMesh->SetIndexBuffer(pIndexBuffer);

		VSGeometryPtr pGeometry = VS_NEW VSGeometry();

		pGeometry->SetMeshDate(pVSMesh);
		pGeometry->m_GeometryName = Name.GetBuffer();
		pGeometry->SetAffectBoneArray(m_MeshBoneNode);
	
		m_pGeoNode->AddChild(pGeometry);

		
		for (unsigned int ShapeIndex = 0; ShapeIndex < m_MorphVertexArray.GetNum(); ShapeIndex++)
		{
			if (pMorphVertexBuffer[ShapeIndex] == NULL)
				continue;
			MorphType MT;
			MT.uiGeometryIndex = m_pGeoNode->GetNormalGeometryNum() - 1;
			MT.MorphName = m_MorphName[ShapeIndex];
			MT.pVertexBuffer = pMorphVertexBuffer[ShapeIndex];
			m_MorphTargetList.AddElement(MT);
		}
		
		if (m_bHasShadowVolume)
		{
			VSOutPutDebugString("Create Shadow Volume\n");
			printf("Create Shadow Volume\n");
			VSVertexBufferPtr pSVertexBuffer = NULL;
			pSVertexBuffer = VS_NEW VSVertexBuffer(true);
			if(!pSVertexBuffer)
				return 0;

			VSIndexBufferPtr pSIndexBuffer = VS_NEW VSIndexBuffer();
			if (!pSIndexBuffer)
			{
				return false;
			}

			if (GetShadowVolumeData(pSVertexBuffer,pSIndexBuffer,HasSkin) == false)
			{
				return false;
			}
			VSTriangleSetPtr pSVSMesh = NULL ;
			pSVSMesh = VS_NEW VSTriangleSet();
			if (!pSVSMesh)
			{
				return 0;
			}
			pSVSMesh->SetVertexBuffer(pSVertexBuffer);
			pSVSMesh->SetIndexBuffer(pSIndexBuffer);
			VSShadowVolumeGeometryPtr pSVG = VS_NEW VSShadowVolumeGeometry();

			pSVG->SetMeshDate(pSVSMesh);
			pSVG->m_GeometryName = Name.GetBuffer();
			pSVG->SetAffectBoneArray(m_MeshBoneNode);

			m_pGeoNode->AddChild(pSVG);
			
		}
		VSOutPutDebugString("Create Mesh Finish\n");
		printf("Create Mesh Finish\n");
		return 1;
	}
#define MERGE_COS_ANGLE_PROXIMITY 0.95f
#define FLOAT_EQUALITY_FUDGE 0.001f
	class EdgePositions {
	public:
		VSVector3 p0, p1;

		bool operator == ( const EdgePositions& rhs ) const
		{
			return ( rhs.p0 == p0 && rhs.p1 == p1 );
		}
		bool operator < ( const EdgePositions& rhs ) const
		{
			if ( p0 < rhs.p0 ) return true;
			if ( p0 > rhs.p0 ) return false;
			if ( p1 < rhs.p1 ) return true;
			return false;
		}
	};


	class EdgeData {
	public:
		VSVector3 n0, n1;
		DWORD i0, i1;
	};

	bool VSFBXConverter::GetShadowVolumeData(VSVertexBuffer * pVertexBuffer , VSIndexBuffer * pIndexBuffer, bool HasSkin)
	{

		 DWORD             dwNumSavedEdges = 0;

		VSMap<EdgePositions, EdgeData> edgeMap;

		VSArray<VSVector3> SVVertexArray;
		VSArray<VSUINT_INDEX> SVIndexArray;
		VSArray<VSVector3W> SVBoneIndex;
		VSArray<VSVector3W> SVBoneWeight;
		VSArray<VSVector3> SVNormalArray;

		DWORD dwNumFaces = m_IndexArray.GetNum() / 3;
		for ( DWORD faceIndex = 0; faceIndex < dwNumFaces; faceIndex++ )
		{
			WORD wIndex0 = m_IndexArray[3*faceIndex+0];
			WORD wIndex1 = m_IndexArray[3*faceIndex+1];
			WORD wIndex2 = m_IndexArray[3*faceIndex+2];

			VSVector3 v0 = m_VertexArray[wIndex0];
			VSVector3 v1 = m_VertexArray[wIndex1];
			VSVector3 v2 = m_VertexArray[wIndex2];

			VSVector3 faceNormal;
			faceNormal.Cross(v2 - v0 , v1 - v0);
			faceNormal.Normalize();
			VSVector3 shadowVertices[3];
			VSVector3 shadowVerticesNormal[3];
			VSVector3W shadowVerticesBoneIndex[3];
			VSVector3W shadowVerticesBoneWeight[3];
			for ( DWORD vertIndex = 0; vertIndex < 3; vertIndex++ )
			{
				shadowVertices[vertIndex] = 
					m_VertexArray[ m_IndexArray[3 * faceIndex + vertIndex] ];
				// There was a time when a typo (.p instead of .n) caused me 6 hours...
				shadowVerticesNormal[vertIndex] = faceNormal;
				if (HasSkin)
				{
					shadowVerticesBoneIndex[vertIndex] = m_BoneIndex[ m_IndexArray[3 * faceIndex + vertIndex] ];

					shadowVerticesBoneWeight[vertIndex] = m_BoneWeight[ m_IndexArray[3 * faceIndex + vertIndex] ];
				}
				
			}

			EdgePositions edgePositions[3];
			EdgeData sharedEdgesData[3];
			EdgePositions edgeToFind;
			BOOL edgeFound[3];
			BOOL quadRequired[3];

			for ( DWORD edgeIndex = 0; edgeIndex < 3; edgeIndex++ )
			{
				edgePositions[edgeIndex].p0 = shadowVertices[edgeIndex];
				edgePositions[edgeIndex].p1 = shadowVertices[( edgeIndex + 1 ) % 3];

				// Find shared edge with reverse vertices
				edgeToFind.p0 = edgePositions[edgeIndex].p1;
				edgeToFind.p1 = edgePositions[edgeIndex].p0;
				unsigned int edgeMapIter = edgeMap.Find( edgeToFind );

				// Did we find it?
				if ( edgeMapIter != edgeMap.GetNum() ) // Found!
				{
					edgeFound[edgeIndex] = TRUE;
					sharedEdgesData[edgeIndex] = edgeMap[edgeMapIter].Value;
					edgeMap.Erase( edgeMapIter );

					// We can optimize away coplanar shared edges. e.g. their normals are 
					// almost parallel
					if ( (faceNormal .Dot(sharedEdgesData[edgeIndex].n0)) < MERGE_COS_ANGLE_PROXIMITY )
					{
						quadRequired[edgeIndex] = TRUE;
					}
					else
					{
						quadRequired[edgeIndex] = FALSE;
						dwNumSavedEdges++;
					}
				}
				else  // New
					edgeFound[edgeIndex] = FALSE;
			}


			DWORD index[3]; // Selected index of vertices
			for ( DWORD vertIndex=0; vertIndex<3; vertIndex++ )
			{
				// Vertex creation
				DWORD edgeIndex0  = vertIndex;              // 0, 1, 2
				DWORD edgeIndex1  = ( vertIndex + 2 ) % 3;  // 2, 0, 1
				// We check whether the current edge had been optimized away
				// If it is, we just need to copy the index from the shared edge since 
				// the current vertex is gone
				if ( ( edgeFound[edgeIndex0] ) && ( !quadRequired[edgeIndex0] ) )
				{
					index[vertIndex] = sharedEdgesData[edgeIndex0].i1;
				}
				else if ( ( edgeFound[edgeIndex1] ) && ( !quadRequired[edgeIndex1] ) )
				{
					index[vertIndex] = sharedEdgesData[edgeIndex1].i0;
				}
				else  // No optimization, new vertex OR degenerate quad required
				{
					// Current size = index
					index[vertIndex] = SVVertexArray.GetNum();
					// Store current vertex
					SVVertexArray.AddElement( shadowVertices[vertIndex] );
					SVNormalArray.AddElement( shadowVerticesNormal[vertIndex]);
					if (HasSkin)
					{
						SVBoneIndex.AddElement( shadowVerticesBoneIndex[vertIndex]);
						SVBoneWeight.AddElement( shadowVerticesBoneWeight[vertIndex]);
					}
					
				}

				// Index creation
				SVIndexArray.AddElement( index[vertIndex] );
			}


			// Time to create the degenerate quads
			for ( DWORD edgeIndex = 0; edgeIndex < 3; edgeIndex++ )
			{
				EdgeData edgeData;
				edgeData.i0   = index[edgeIndex];             // 0, 1, 2
				edgeData.i1   = index[( edgeIndex + 1 ) % 3]; // 1, 2, 0
				edgeData.n0   = faceNormal;
				if ( edgeFound[edgeIndex] ) // Shared edge found
				{
					// Handle 3 cases: Insert full quad, insert left side tri or insert 
					// right side tri
					// If current edge had NOT been optimized away
					if ( ( sharedEdgesData[edgeIndex].i0 != edgeData.i1 ) && 
						( sharedEdgesData[edgeIndex].i1 != edgeData.i0 ) )
					{
						SVIndexArray.AddElement( edgeData.i1 );
						SVIndexArray.AddElement( edgeData.i0 );
						SVIndexArray.AddElement( sharedEdgesData[edgeIndex].i0 );
						SVIndexArray.AddElement( sharedEdgesData[edgeIndex].i1 );
						SVIndexArray.AddElement( sharedEdgesData[edgeIndex].i0 );
						SVIndexArray.AddElement( edgeData.i0 );
					}
					else if ( sharedEdgesData[edgeIndex].i1 != edgeData.i0 )
					{
						SVIndexArray.AddElement( edgeData.i1 );
						SVIndexArray.AddElement( edgeData.i0 );
						SVIndexArray.AddElement( sharedEdgesData[edgeIndex].i1 );
					}
					else if( sharedEdgesData[edgeIndex].i0 != edgeData.i1 )
					{
						SVIndexArray.AddElement( edgeData.i1 );
						SVIndexArray.AddElement( edgeData.i0 );
						SVIndexArray.AddElement( sharedEdgesData[edgeIndex].i0 );
					}
					// All other cases meant that the current edge had been optimized away 
					// and only the shared edge is left. The shared edge is left alone with 
					// additional degenerate triangles added
				}
				else  // New edge
				{
					edgeMap.AddElement(edgePositions[edgeIndex],edgeData) ;
				}
			}
		}

		if ( edgeMap.GetNum() > 0 )
		{
			VSOutPutDebugString("error -------------No Close Mesh\n");
			printf("error -------------No Close Mesh\n");
			return false;
		}

		VSDataBufferPtr pVertexDate = NULL;
		pVertexDate = VS_NEW VSDataBuffer;
		if(!pVertexDate)
			return 0;
		pVertexDate->SetDate(&SVVertexArray[0],(unsigned int)SVVertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);


		//创建法向量BUFFER
		VSDataBufferPtr pNormalDate = NULL;
		pNormalDate = VS_NEW VSDataBuffer;
		if(!pNormalDate)
			return 0;

		if (m_CurExportPara & EP_NORMAL_COMPRESS)
		{
			VSArray<DWORD> CompressDate;
			CompressDate.SetBufferNum(SVNormalArray.GetNum());
			for (unsigned int i = 0 ; i < SVNormalArray.GetNum() ;i++)
			{
				VSVector3W Temp(SVNormalArray[i]);
				Temp = (Temp + 1.0f) * 0.5f;
				CompressDate[i] = Temp.GetDWABGR();
			}
			pNormalDate->SetDate(&CompressDate[0],CompressDate.GetNum(),VSDataBuffer::DT_UBYTE4N);
		}
		else
		{
			pNormalDate->SetDate(&SVNormalArray[0],(unsigned int)SVNormalArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);
		}

		//添加权重
		VSDataBufferPtr pBoneWeight = NULL;
		if(HasSkin)
		{
			pBoneWeight = VS_NEW VSDataBuffer;
			if(!pBoneWeight)
				return 0;
			if (m_CurExportPara & EP_SKIN_COMPRESS)
			{
				VSArray<DWORD> CompressDate;
				CompressDate.SetBufferNum(SVBoneWeight.GetNum());
				for (unsigned int i = 0 ; i < SVBoneWeight.GetNum() ;i++)
				{

					CompressDate[i] = SVBoneWeight[i].GetDWABGR();
				}
				pBoneWeight->SetDate(&CompressDate[0],CompressDate.GetNum(),VSDataBuffer::DT_UBYTE4N);
			}
			else
			{
				pBoneWeight->SetDate(&SVBoneWeight[0],(unsigned int)SVBoneWeight.GetNum(),VSDataBuffer::DT_FLOAT32_4);
			}

		}

		//添加骨头索引
		VSDataBufferPtr pBoneIndex = NULL;
		if(HasSkin)
		{
			pBoneIndex = VS_NEW VSDataBuffer;
			if(!pBoneIndex)
				return 0;
			if (m_CurExportPara & EP_SKIN_COMPRESS)
			{
				VSArray<DWORD> CompressDate;
				CompressDate.SetBufferNum(SVBoneIndex.GetNum());
				for (unsigned int i = 0 ; i < SVBoneIndex.GetNum() ;i++)
				{
					unsigned char R = (unsigned char)SVBoneIndex[i].r;
					unsigned char G = (unsigned char)SVBoneIndex[i].g;
					unsigned char B = (unsigned char)SVBoneIndex[i].b;
					unsigned char A = (unsigned char)SVBoneIndex[i].a;

					CompressDate[i] = VSDWCOLORABGR(A,R,G,B);
				}
				pBoneIndex->SetDate(&CompressDate[0],CompressDate.GetNum(),VSDataBuffer::DT_UBYTE4);
			}
			else
			{
				pBoneIndex->SetDate(&SVBoneIndex[0],(unsigned int)SVBoneIndex.GetNum(),VSDataBuffer::DT_FLOAT32_4);
			}


		}


		//添加顶点
		pVertexBuffer->SetDate(pVertexDate,VSVertexFormat::VF_POSITION);

		//添加法向量
		pVertexBuffer->SetDate(pNormalDate,VSVertexFormat::VF_NORMAL);

		if(HasSkin)
		{
			pVertexBuffer->SetDate(pBoneIndex,VSVertexFormat::VF_BLENDINDICES);
			pVertexBuffer->SetDate(pBoneWeight,VSVertexFormat::VF_BLENDWEIGHT);
		}

		
		//创建索引BUUFER
		VSDataBufferPtr pIndex = NULL;
		pIndex = VS_NEW VSDataBuffer;
		if(!pIndex)
			return 0;
		if (SVVertexArray.GetNum() > 65535)
		{
			pIndex->SetDate(&SVIndexArray[0],(unsigned int)SVIndexArray.GetNum(),VSDataBuffer::DT_UINT);
		}
		else
		{
			VSArray<VSUSHORT_INDEX> IndexArrayTemp;
			IndexArrayTemp.SetBufferNum(SVIndexArray.GetNum());
			for (unsigned int i = 0 ; i < SVIndexArray.GetNum() ;i++)
			{
				IndexArrayTemp[i] = SVIndexArray[i];
			}
			pIndex->SetDate(&IndexArrayTemp[0],(unsigned int)IndexArrayTemp.GetNum(),VSDataBuffer::DT_USHORT);
		}

		pIndexBuffer->SetDate(pIndex);
		return true;
	}
	void VSFBXConverter::CreateTangentAndBinormal(const VSVector3 & Point0, const VSVector3 & Point1,const VSVector3 &Point2,
		const VSVector2 & Texcoord0,const VSVector2 & Texcoord1,const VSVector2 TexCoord2,
		const VSVector3 &Normal ,VSVector3 & Tangent,VSVector3 & Binromal)
	{



		VSVector3 N1 = (	(Point0 - Point1) * (Texcoord0.y - TexCoord2.y) - (Point0 - Point2) * (Texcoord0.y - Texcoord1.y))/
			((Texcoord0.x - Texcoord1.x) * (Texcoord0.y - TexCoord2.y) - (Texcoord0.x - TexCoord2.x) * (Texcoord0.y - Texcoord1.y));

		Tangent = N1;
		VSVector3 N2 =(	(Point0 - Point1) * (Texcoord0.x - TexCoord2.x) - (Point0 - Point2) * (Texcoord0.x - Texcoord1.x))/
			((Texcoord0.x - Texcoord1.x) * (Texcoord0.y - TexCoord2.y) - (Texcoord0.x - TexCoord2.x) * (Texcoord0.y - Texcoord1.y));

		Binromal = N2;

	}
	bool VSFBXConverter::GetMeshNode(FbxNode* pNode)  
	{  

		if(pNode->GetNodeAttribute())  
		{  
			switch(pNode->GetNodeAttribute()->GetAttributeType())  
			{  
			case FbxNodeAttribute::eMesh :
				VSOutPutDebugString("Process Fbx Mesh Node\n");
				printf("Process Fbx Mesh Node\n");
				if (ProcessMesh(pNode) == false)
				{
					return false;
				}
				break;  
			}  
		}  

		for(int i = 0 ; i < pNode->GetChildCount() ; ++i)  
		{  
			if (GetMeshNode(pNode->GetChild(i)) == false)
			{
				return false;
			}
		}  
		return true;
	}  
	void VSFBXConverter::MaxMatToVSMat(const FbxAMatrix &MaxMat,VSMatrix3X3W & VSMat)
	{

		for (unsigned int i = 0 ; i < 4 ; i++)
		{
			for (unsigned int j = 0 ; j < 4 ; j++)
			{
				VSMat.M[i][j] = (VSREAL)MaxMat.Get(i,j);
			}
		}

// 		VSMatrix3X3W NewMat = GetHelpMaterix() * VSMat * GetHelpMaterix();
// 		VSMat = NewMat;

// 		VSMatrix3X3 ChangeYZ(1.0f,0.0f,0.0f,
// 			0.0f,0.0f,1.0f,
// 			0.0f,1.0f,0.0f);
// 		VSMatrix3X3 Rot;
// 
// 		FbxVector4 R = MaxMat.GetRow(0);
// 		Rot.M[0][0] = (VSREAL)R[0]; Rot.M[0][1] = (VSREAL)R[1]; Rot.M[0][2] = (VSREAL)R[2];
// 
// 		R = MaxMat.GetRow(1);
// 		Rot.M[1][0] = (VSREAL)R[0]; Rot.M[1][1] = (VSREAL)R[1]; Rot.M[1][2] = (VSREAL)R[2];
// 
// 		R = MaxMat.GetRow(2);
// 		Rot.M[2][0] = (VSREAL)R[0]; Rot.M[2][1] = (VSREAL)R[1]; Rot.M[2][2] = (VSREAL)R[2];
// 
// 		VSMatrix3X3 Combine = ChangeYZ * Rot * ChangeYZ;
// 
// 
// 		VSMat.Add3X3(Combine);
// 		R = MaxMat.GetRow(3);
// 		VSMat.AddTranslate((VSREAL)R[0],(VSREAL)R[2],(VSREAL)R[1]);

	}
	void VSFBXConverter::GetAnim(FbxNode* pNode)
	{
		const FbxLongLong ANIMATION_STEP_30 = 1539538600; // 30frames/seconds
		const FbxLongLong ANIMATION_STEP_15 = 3079077200; // 30frames/seconds
		FbxTime step = ANIMATION_STEP_30;
		if (m_CurExportPara & EP_ACTION_COMPRESS)
		{
			step = ANIMATION_STEP_15;
		}
		VSBoneNodePtr pBoneNode = NULL;
		if(pNode->GetNodeAttribute())  
		{  
			FbxNodeAttribute::EType ATyep = pNode->GetNodeAttribute()->GetAttributeType();
			const char * NodeName = pNode->GetName();
			switch (ATyep)
			{  
			case FbxNodeAttribute::eNull:
			case FbxNodeAttribute::eSkeleton:  
				{
					VSBoneKeyPtr pBoneKey = VS_NEW VSBoneKey();
					pBoneKey->m_cName = pNode->GetName();

					FbxTimeSpan timeSpan;
					m_pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(timeSpan);
					FbxTime start = timeSpan.GetStart();
					FbxTime end = timeSpan.GetStop();
					for (FbxTime i = start ; i < end ; i = i + step)
					{
						FbxAMatrix Combine = pNode->EvaluateLocalTransform(i);


						VSMatrix3X3W VSMat;
						MaxMatToVSMat(Combine,VSMat);

						FbxLongLong MS = i.GetMilliSeconds() - start.GetMilliSeconds();

						VSKeyTimeVector KeyTimeTran;
						KeyTimeTran.m_dKeyTime = MS * 1.0f;
						VSKeyTimeQuaternion KeyTimeQuat;
						KeyTimeQuat.m_dKeyTime = MS * 1.0f;
						VSKeyTimeVector KeyTimeScale;
						KeyTimeScale.m_dKeyTime = MS * 1.0f;

						VSMatrix3X3 ScaleAndRotator;
						VSMat.Get3X3(ScaleAndRotator);
						VSVector3 Scale;
						ScaleAndRotator.GetScaleAndRotated(Scale);
						VSVector3 Tran = VSMat.GetTranslation();
						KeyTimeTran.m_Vector = Tran;
						KeyTimeQuat.m_Quat = ScaleAndRotator.GetQuat();
						KeyTimeScale.m_Vector = Scale;

						pBoneKey->m_TranslationArray.AddElement(KeyTimeTran);
						pBoneKey->m_RotatorArray.AddElement(KeyTimeQuat);
						pBoneKey->m_ScaleArray.AddElement(KeyTimeScale);
					}
					
					FbxAMatrix Combine = pNode->EvaluateLocalTransform(end);



					VSMatrix3X3W VSMat;
					MaxMatToVSMat(Combine,VSMat);



					FbxLongLong MS = end.GetMilliSeconds() - start.GetMilliSeconds();

					VSKeyTimeVector KeyTimeTran;
					KeyTimeTran.m_dKeyTime = MS * 1.0f;
					VSKeyTimeQuaternion KeyTimeQuat;
					KeyTimeQuat.m_dKeyTime = MS * 1.0f;
					VSKeyTimeVector KeyTimeScale;
					KeyTimeScale.m_dKeyTime = MS * 1.0f;

					VSMatrix3X3 ScaleAndRotator;
					VSMat.Get3X3(ScaleAndRotator);
					VSVector3 Scale;
					ScaleAndRotator.GetScaleAndRotated(Scale);
					VSVector3 Tran = VSMat.GetTranslation();
					KeyTimeTran.m_Vector = Tran;
					KeyTimeQuat.m_Quat = ScaleAndRotator.GetQuat();
					KeyTimeScale.m_Vector = Scale;


					pBoneKey->m_TranslationArray.AddElement(KeyTimeTran);
					pBoneKey->m_RotatorArray.AddElement(KeyTimeQuat);
					pBoneKey->m_ScaleArray.AddElement(KeyTimeScale);

					
					m_pAnim->AddBoneKey(pBoneKey);					

				} 
				break;  
			}  
		}  
		for(int i = 0 ; i < pNode->GetChildCount() ; ++i)  
		{  
			GetAnim(pNode->GetChild(i));  
		} 
	}
	void VSFBXConverter::GetSkeleton(FbxNode* pNode,VSBoneNode * pParentBoneNode)
	{

		VSBoneNodePtr pBoneNode = NULL;
		if(pNode->GetNodeAttribute())  
		{  
			switch(pNode->GetNodeAttribute()->GetAttributeType())  
			{  
			case FbxNodeAttribute::eSkeleton:
				{
					pBoneNode = VS_NEW VSBoneNode();

					FbxTimeSpan timeSpan;
					m_pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(timeSpan);
					FbxTime start = timeSpan.GetStart();
					FbxTime end = timeSpan.GetStop();
					pBoneNode->m_cName = pNode->GetName();


					FbxAMatrix Combine = pNode->EvaluateLocalTransform(start);
					
					VSMatrix3X3W VSMat;
					MaxMatToVSMat(Combine,VSMat);
					pBoneNode->SetLocalMat(VSMat);
					if(pParentBoneNode)
					{
						pParentBoneNode->AddChild(pBoneNode);
					}
					else
					{
						m_pSkeleton->AddChild(pBoneNode);
					}
				} 
				break;  
			}  
		}  
		for(int i = 0 ; i < pNode->GetChildCount() ; ++i)  
		{  
			GetSkeleton(pNode->GetChild(i),pBoneNode);  
		} 
	}
	bool VSFBXConverter::OnInitial()
	{
		if (!VSConsoleApplication::OnInitial())
		{
			return false;
		}
		m_bIsRunning = false;
		if (m_pCommand)
		{
			if (m_pCommand->GetName(_T("-s")))
			{
				VSOutPutDebugString("Type : Static Mesh\n");
				printf("Type : Static Mesh\n");
				m_CurExportType = ET_STATIC_MESH;
			}

			if (m_pCommand->GetName(_T("-d")))
			{
				VSOutPutDebugString("Type : Dynamic Skin Mesh\n");
				printf("Type : Dynamic Skin Mesh\n");
				m_CurExportType = ET_SKELECTON_MESH;
			}

			if (m_pCommand->GetName(_T("-a")))
			{
				VSOutPutDebugString("Type : Acton\n");
				printf("Type : TActon\n");
				m_CurExportType = ET_ACTION;
			}
			if (m_CurExportType == ET_MAX)
			{
				VSOutPutDebugString("Error No Type\n");
				printf("Error No Type\n");
				return false;
			}
			if (m_pCommand->GetName(_T("-c")))
			{
				VSOutPutDebugString("Compress UV Normal SkinInfo Action \n");
				printf("Compress UV Normal SkinInfo Action \n");
				m_CurExportPara = EP_UV_COMPRESS | EP_NORMAL_COMPRESS | EP_SKIN_COMPRESS | EP_ACTION_COMPRESS;
			}
			else
			{
				VSOutPutDebugString("No Compress\n");
				printf("No Compress\n");
				m_CurExportPara = 0;
			}
			m_bHasShadowVolume = false;
			if (m_pCommand->GetName(_T("-v")))
			{
				VSOutPutDebugString("Have Shadow Volume Info\n");
				printf("Have Shadow Volume Info\n");
				m_bHasShadowVolume = true;
			}
			else
			{
				VSOutPutDebugString("No Shadow Volume Info\n");
				printf("No Shadow Volume Info\n");
			}
			m_bRootMotion = false;
			if (m_pCommand->GetName(_T("-r")))
			{
				VSOutPutDebugString("Have Root Motion\n");
				printf("Have Root Motion Info\n");
				m_bRootMotion = true;
			}
			else
			{
				VSOutPutDebugString("No Root Motion\n");
				printf("No Root Motion Info\n");
			}
			m_bUseFbxNormal = false;
			if (m_pCommand->GetName(_T("-n")))
			{
				VSOutPutDebugString("Use Fbx Normal\n");
				printf("Use Fbx Normal\n");
				m_bUseFbxNormal = true;
			}
			else
			{
				VSOutPutDebugString("Use Compute Normal\n");
				printf("Use Compute Normal\n");
			}
			m_bHasMorph = false;
			if (m_pCommand->GetName(_T("-m")))
			{
				VSOutPutDebugString("Have Morph Target\n");
				printf("Have Morph Target\n");
				m_bHasMorph = true;
			}
			else
			{
				VSOutPutDebugString("No Morph Target\n");
				printf("No Morph Target\n");
			}
			VSString SourcePath;
			VSString DestPath;
			if (!m_pCommand->GetString(_T("SourcePath"), SourcePath) || !m_pCommand->GetString(_T("DestPath"), DestPath))
			{
				VSOutPutDebugString("Error : No SourcePath or No DestPath\n");
				printf("No Error : No SourcePath or No DestPath\n");
				return false;
			}
			else
			{
				VSString FullName = DestPath + _T(".");;
				if (m_CurExportType == ET_STATIC_MESH)
				{
					FullName += VSResource::GetFileSuffix(VSResource::RT_STATIC_MODEL);
				}
				else if (m_CurExportType == ET_SKELECTON_MESH)
				{
					FullName += VSResource::GetFileSuffix(VSResource::RT_SKELECTON_MODEL);
				}
				else if (m_CurExportType == ET_ACTION)
				{
					FullName += VSResource::GetFileSuffix(VSResource::RT_ACTION);
				}
				else
				{
					return false;
				}
				VSOutPutDebugString("SourcePath : %s\n", SourcePath.GetBuffer());
				printf("SourcePath : %s\n", SourcePath.GetBuffer());

				VSOutPutDebugString("DestPath : %s\n", FullName.GetBuffer());
				printf("DestPath : %s\n", FullName.GetBuffer());

				return ExportFile(SourcePath.GetBuffer(), FullName.GetBuffer());
			}
			
		}
		
		return false;
	}
}
#endif