#include "VSMorphMainFunction.h"
#include "VSSwitchNode.h"
#include "VSGeometryNode.h"
#include "VSGeometry.h"
#include "VSMorphFunction.h"
#include "VSGraphicInclude.h"
#include "VSRenderThread.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSMorphMainFunction,VSMorphBaseFunction)
BEGIN_ADD_PROPERTY(VSMorphMainFunction,VSMorphBaseFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMorphMainFunction)
IMPLEMENT_INITIAL_END
VSMorphMainFunction::VSMorphMainFunction(const VSUsedName & ShowName,VSMorphTree * pMorphTree)
:VSMorphBaseFunction(ShowName,pMorphTree)
{
	VSString InputName = _T("Morph");
	VSInputNode * pInputNode = NULL;

	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_MORPH,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

}
VSMorphMainFunction::VSMorphMainFunction()
{

}
VSMorphMainFunction::~VSMorphMainFunction()
{

}
bool VSMorphMainFunction::Update(double dAppTime)
{
	if(!VSMorphBaseFunction::Update(dAppTime))
		return false;

	if(!m_pInput[0]->GetOutputLink())
	{
		return false;
	}
	VSMorphFunction *pMorphFunction = (VSMorphFunction *)m_pInput[0]->GetOutputLink()->GetOwner();
	if(!pMorphFunction)
	{
		return false;
	}
	if(!pMorphFunction->Update(dAppTime))
		return false;

	bool bRenderThread = VSResourceManager::ms_bRenderThread && VSRenderThreadSys::ms_pRenderThreadSys->IsRunning();

	if (bRenderThread)
	{
		RenderThreadUpdate(pMorphFunction);
	}
	else
	{
		MainThreadUpdate(pMorphFunction);
	}
	return true;

}
void VSMorphMainFunction::RenderThreadUpdate(VSMorphFunction *pMorphFunction)
{
	bool bCompressData = m_pOwner->IsCompressData();
	//for all the geometry
	for (unsigned int i = 0; i < m_pGeomeNode->GetNormalGeometryNum(); i++)
	{
		VSGeometry *  pGeome = (VSGeometry *)m_pGeomeNode->GetNormalGeometry(i);
		if (pGeome)
		{

			unsigned int uiVertexNum = pGeome->GetVertexNum();
			m_pOwner->ClearChangeFlag();
			//must OriginMeshData
			VSMeshDate * pMeshDate = pGeome->GetOriginMeshDate();
			if (!pMeshDate)
			{
				continue;
			}
			VSVertexBuffer * pVertexBuffer = pMeshDate->GetVertexBuffer();
			if (!pVertexBuffer)
			{
				continue;
			}

			//look for VRAM addr
			VSVector3 * pPosDest[VSMorphFunction::MAX_NUM_POS3] = { NULL };

			void * pNormalDest[VSMorphFunction::MAX_NUM_NORMAL3] = { NULL };
			void * pTangentDest = NULL;
			VSVector3 *	pBinormalDest = NULL;
			DWORD	*pColorDest[VSMorphFunction::MAX_NUM_COLOR] = { NULL };

			unsigned int uiPosLevel = pVertexBuffer->GetPositionLevel();
			for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_POS3 && uiLevel < uiPosLevel; uiLevel++)
			{
				pPosDest[uiLevel] = (VSVector3 *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiVertexNum * sizeof(VSVector3));
			}

			unsigned int uiNormalLevel = pVertexBuffer->GetNormalLevel();
			for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3 && uiLevel < uiNormalLevel; uiLevel++)
			{
				if (bCompressData)
				{
					pNormalDest[uiLevel] = (void *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiVertexNum * sizeof(DWORD));
				}
				else
				{
					pNormalDest[uiLevel] = (void *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiVertexNum * sizeof(VSVector3));
				}
			}



			if (pVertexBuffer->HaveTangentInfo())
			{
				if (bCompressData)
				{
					pTangentDest = (void *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiVertexNum * sizeof(DWORD));
				}
				else
				{
					pTangentDest = (void *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiVertexNum * sizeof(VSVector3));
				}
			}



			if (pVertexBuffer->HaveBinormalInfo())
			{
				pBinormalDest = (VSVector3 *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiVertexNum * sizeof(VSVector3));
			}

			unsigned int uiColorLevel = pVertexBuffer->GetColorLevel();
			for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_COLOR && uiLevel < uiColorLevel; uiLevel++)
			{
				pColorDest[uiLevel] = (DWORD *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiVertexNum * sizeof(DWORD));
			}

			
			struct MorphDataStructType
			{
				VSVector3 * pPosDest[VSMorphFunction::MAX_NUM_POS3];
				void * pNormalDest[VSMorphFunction::MAX_NUM_NORMAL3];
				void * pTangentDest;
				VSVector3 *	pBinormalDest;
				DWORD	*pColorDest[VSMorphFunction::MAX_NUM_COLOR];
				bool	bCompressData;
				unsigned int uiVertexNum;
			};

			MorphDataStructType MorphData;
			for (unsigned int n = 0; n < VSMorphFunction::MAX_NUM_POS3; n++)
			{
				MorphData.pPosDest[n] = pPosDest[n];
			}
			for (unsigned int n = 0; n < VSMorphFunction::MAX_NUM_NORMAL3; n++)
			{
				MorphData.pNormalDest[n] = pNormalDest[n];
			}
			for (unsigned int n = 0; n < VSMorphFunction::MAX_NUM_COLOR; n++)
			{
				MorphData.pColorDest[n] = pColorDest[n];
			}

			MorphData.pTangentDest = pTangentDest;
			MorphData.pBinormalDest = pBinormalDest;
			MorphData.bCompressData = bCompressData;
			MorphData.uiVertexNum = uiVertexNum;

			pMorphFunction->UpdateGeometryDate(i);

			if (!MorphData.pPosDest[0] && !MorphData.pColorDest[0])
			{
				continue;
			}
			for (unsigned int j = 0; j < uiVertexNum; j++)
			{

				pMorphFunction->UpdateVertexDate(j);


				for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_POS3; uiLevel++)
				{
					VSVector3 * pVerSrc = pMorphFunction->GetPos(uiLevel);
					if (pPosDest[uiLevel] && pVerSrc)
					{
						*pPosDest[uiLevel] = *pVerSrc;
						pPosDest[uiLevel]++;
					}
					else
					{
						MorphData.pPosDest[uiLevel] = NULL;
					}
				}


				for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
				{
					VSVector3 * pNormalSrc = pMorphFunction->GetNormal(uiLevel);
					if (pNormalDest[uiLevel] && pNormalSrc)
					{
						if (bCompressData)
						{
							VSVector3W Normal(*pNormalSrc);
							DWORD * pTempDest = (DWORD *)pNormalDest[uiLevel];
							*pTempDest = Normal.GetDWABGR();
						}
						else
						{
							VSVector3 * pTempDest = (VSVector3 *)pNormalDest[uiLevel];
							*pTempDest = *pNormalSrc;
						}
						pNormalDest[uiLevel] = (((unsigned char *)pNormalDest[uiLevel]) + ((bCompressData == 1)? sizeof(DWORD) : sizeof(VSVector3)));

					}
					else
					{
						MorphData.pNormalDest[uiLevel] = NULL;
					}
				}

				VSVector3W * pTangentSrc = pMorphFunction->GetTangent();
				if (pTangentDest && pTangentSrc)
				{

					if (bCompressData)
					{
						DWORD * pTempDest = (DWORD *)pTangentDest;
						*pTempDest = (*pTangentSrc).GetDWABGR();
					}
					else
					{
						VSVector3 * pTempDest = (VSVector3 *)pTangentDest;
						*pTempDest = (*pTangentSrc).GetV3();
					}
					pTangentDest = (((unsigned char *)pTangentDest) + ((bCompressData == 1) ? sizeof(DWORD) : sizeof(VSVector3)));
				}
				else
				{
					MorphData.pTangentDest = NULL;
				}


				VSVector3 * pBinormalSrc = pMorphFunction->GetBinormal();
				if (pBinormalDest && pBinormalSrc)
				{			

					*pBinormalDest = *pBinormalSrc;
					pBinormalDest = (VSVector3 *)(((unsigned char *)pBinormalDest) + pVertexBuffer->GetOneVertexSize());
				}
				else
				{
					MorphData.pBinormalDest = NULL;
				}




				for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_COLOR; uiLevel++)
				{
					DWORD * pColorSrc = pMorphFunction->GetColor(uiLevel);
					if (pColorDest[uiLevel] && pColorSrc)
					{
						*pColorDest[uiLevel] = *pColorSrc;
						pColorDest[uiLevel]++;
					}
					else
					{
						MorphData.pColorDest[uiLevel] = NULL;
					}
				}

			}


			// use render data to render thread
			pMeshDate = pGeome->GetMeshDate();
			if (!pMeshDate)
			{
				continue;
			}
			pVertexBuffer = pMeshDate->GetVertexBuffer();
			if (!pVertexBuffer)
			{
				continue;
			}


			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(UpdateMorphDataCommand,VSVertexBuffer *,pVertexBuffer,pVertexBuffer,MorphDataStructType,MorphData,MorphData,
			{
				VSVector3 * pPosDest[VSMorphFunction::MAX_NUM_POS3] = { NULL };

				void * pNormalDest[VSMorphFunction::MAX_NUM_NORMAL3] = { NULL };
				void * pTangentDest = NULL;
				VSVector3 *	pBinormalDest = NULL;
				DWORD	*pColorDest[VSMorphFunction::MAX_NUM_COLOR] = { NULL };

				pVertexBuffer->Lock();

				for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_POS3; uiLevel++)
				{
					pPosDest[uiLevel] = (VSVector3 *)pVertexBuffer->GetLockPositionDate(uiLevel);
				}

				for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
				{
					pNormalDest[uiLevel] = pVertexBuffer->GetLockNormalDate(uiLevel);
				}
				pTangentDest = pVertexBuffer->GetLockTangentDate();
				pBinormalDest = (VSVector3 *)pVertexBuffer->GetLockBinormalDate();

				for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_COLOR; uiLevel++)
				{
					pColorDest[uiLevel] = (DWORD *)pVertexBuffer->GetLockColorDate(uiLevel);
				}

				for (unsigned int j = 0; j < MorphData.uiVertexNum; j++)
				{
					for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_POS3; uiLevel++)
					{
						if (MorphData.pPosDest[uiLevel] && pPosDest[uiLevel])
						{
							*pPosDest[uiLevel] = *MorphData.pPosDest[uiLevel];
							pPosDest[uiLevel] = (VSVector3 *)(((unsigned char *)pPosDest[uiLevel]) + pVertexBuffer->GetOneVertexSize());
							MorphData.pPosDest[uiLevel]++;
						}
					}


					for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
					{
						if (pNormalDest[uiLevel] && MorphData.pNormalDest[uiLevel])
						{
							if (MorphData.bCompressData)
							{
								DWORD * pTempDest = (DWORD *)pNormalDest[uiLevel];
								DWORD * pSource = (DWORD *)MorphData.pNormalDest[uiLevel];
								*pTempDest = *pSource;
							}
							else
							{
								VSVector3 * pTempDest = (VSVector3 *)pNormalDest[uiLevel];
								VSVector3 * pSource = (VSVector3 *)MorphData.pNormalDest[uiLevel];
								*pTempDest = *pSource;
							}
							pNormalDest[uiLevel] = (((unsigned char *)pNormalDest[uiLevel]) + pVertexBuffer->GetOneVertexSize());
							MorphData.pNormalDest[uiLevel] = (((unsigned char *)MorphData.pNormalDest[uiLevel]) +
								((MorphData.bCompressData == 1) ? sizeof(DWORD) : sizeof(VSVector3)));
						}

					}

					{
						if (pTangentDest && MorphData.pTangentDest)
						{
							if (MorphData.bCompressData)
							{
								DWORD * pTempDest = (DWORD *)pTangentDest;
								DWORD * pSource = (DWORD *)MorphData.pTangentDest;
								*pTempDest = *pSource;
							}
							else
							{
								VSVector3 * pTempDest = (VSVector3 *)pTangentDest;
								VSVector3 * pSource = (VSVector3 *)MorphData.pTangentDest;
								*pTempDest = *pSource;
							}
							pTangentDest = (((unsigned char *)pTangentDest) + pVertexBuffer->GetOneVertexSize());
							MorphData.pTangentDest = (((unsigned char *)MorphData.pTangentDest) +
								((MorphData.bCompressData == 1) ? sizeof(DWORD) : sizeof(VSVector3)));
						}

					}

					{
						if (pBinormalDest && MorphData.pBinormalDest)
						{
							*pBinormalDest = *MorphData.pBinormalDest;

							pBinormalDest = (VSVector3 *)(((unsigned char *)pBinormalDest) + pVertexBuffer->GetOneVertexSize());
							MorphData.pBinormalDest++;
						}
					}




					for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_COLOR; uiLevel++)
					{
						if (pColorDest[uiLevel] && MorphData.pColorDest[uiLevel])
						{
							*pColorDest[uiLevel] = *MorphData.pColorDest[uiLevel];
							pColorDest[uiLevel] = (DWORD *)(((unsigned char *)pColorDest[uiLevel]) + pVertexBuffer->GetOneVertexSize());
							MorphData.pColorDest[uiLevel]++;
						}
					}
				}
				
				pVertexBuffer->UnLock();
			})
			VSMAC_ASSERT(0);
			ENQUEUE_UNIQUE_RENDER_COMMAND_END
		}
	}
}
void VSMorphMainFunction::MainThreadUpdate(VSMorphFunction *pMorphFunction)
{
	bool bCompressData = m_pOwner->IsCompressData();
	//for all the geometry
	for (unsigned int i = 0; i < m_pGeomeNode->GetNormalGeometryNum(); i++)
	{
		VSGeometry *  pGeome = (VSGeometry *)m_pGeomeNode->GetNormalGeometry(i);
		if (pGeome)
		{

			unsigned int uiVertexNum = pGeome->GetVertexNum();
			m_pOwner->ClearChangeFlag();
			VSMeshDate * pMeshDate = pGeome->GetMeshDate();
			if (!pMeshDate)
			{
				continue;
			}
			VSVertexBuffer * pVertexBuffer = pMeshDate->GetVertexBuffer();
			if (!pVertexBuffer)
			{
				continue;
			}

			//look for VRAM addr
			VSVector3 * pPosDest[VSMorphFunction::MAX_NUM_POS3] = { NULL };

			void * pNormalDest[VSMorphFunction::MAX_NUM_NORMAL3] = { NULL };
			void * pTangentDest = NULL;
			VSVector3 *	pBinormalDest = NULL;
			DWORD	*pColorDest[VSMorphFunction::MAX_NUM_COLOR] = { NULL };
			
			if (pVertexBuffer->Lock() == NULL)
			{
				continue;
			}

			for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_POS3; uiLevel++)
			{
				pPosDest[uiLevel] = (VSVector3 *)pVertexBuffer->GetLockPositionDate(uiLevel);
			}

			for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
			{
				pNormalDest[uiLevel] = pVertexBuffer->GetLockNormalDate(uiLevel);
			}
			pTangentDest = pVertexBuffer->GetLockTangentDate();
			pBinormalDest = (VSVector3 *)pVertexBuffer->GetLockBinormalDate();

			for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_COLOR; uiLevel++)
			{
				pColorDest[uiLevel] = (DWORD *)pVertexBuffer->GetLockColorDate(uiLevel);
			}
			






			pMorphFunction->UpdateGeometryDate(i);
			if (!pPosDest[0] && !pColorDest[0])
			{
				pVertexBuffer->UnLock();
				continue;			
			}

			for (unsigned int j = 0; j < uiVertexNum; j++)
			{

				pMorphFunction->UpdateVertexDate(j);


				for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_POS3; uiLevel++)
				{
					VSVector3 * pVerSrc = pMorphFunction->GetPos(uiLevel);
					if (pPosDest[uiLevel] && pVerSrc)
					{

						*pPosDest[uiLevel] = *pVerSrc;
						pPosDest[uiLevel] = (VSVector3 *)(((unsigned char *)pPosDest[uiLevel]) + pVertexBuffer->GetOneVertexSize());					
					}
					else
					{
						pPosDest[uiLevel] = NULL;
					}
				}


				for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
				{
					VSVector3 * pNormalSrc = pMorphFunction->GetNormal(uiLevel);
					if (pNormalDest[uiLevel] && pNormalSrc)
					{

						if (bCompressData)
						{
							VSVector3W Normal(*pNormalSrc);
							DWORD * pTempDest = (DWORD *)pNormalDest[uiLevel];
							*pTempDest = Normal.GetDWABGR();
						}
						else
						{
							VSVector3 * pTempDest = (VSVector3 *)pNormalDest[uiLevel];
							*pTempDest = *pNormalSrc;
						}
						pNormalDest[uiLevel] = (((unsigned char *)pNormalDest[uiLevel]) + pVertexBuffer->GetOneVertexSize());
						
					}
					else
					{
						pNormalDest[uiLevel] = NULL;
					}
				}


				VSVector3W * pTangentSrc = pMorphFunction->GetTangent();
				if (pTangentDest && pTangentSrc)
				{
					if (bCompressData)
					{
						DWORD * pTempDest = (DWORD *)pTangentDest;
						*pTempDest = (*pTangentSrc).GetDWABGR();
					}
					else
					{
						VSVector3 * pTempDest = (VSVector3 *)pTangentDest;
						*pTempDest = (*pTangentSrc).GetV3();
					}
					pTangentDest = (((unsigned char *)pTangentDest) + pVertexBuffer->GetOneVertexSize());
				}
				else
				{
					pTangentDest = NULL;
				}

				VSVector3 * pBinormalSrc = pMorphFunction->GetBinormal();
				if (pBinormalDest && pBinormalSrc)
				{

					*pBinormalDest = *pBinormalSrc;
					pBinormalDest = (VSVector3 *)(((unsigned char *)pBinormalDest) + pVertexBuffer->GetOneVertexSize());

				}
				else
				{
					pBinormalDest = NULL;
				}




				for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_COLOR; uiLevel++)
				{
					DWORD * pColorSrc = pMorphFunction->GetColor(uiLevel);
					if (pColorDest[uiLevel] && pColorSrc)
					{

						*pColorDest[uiLevel] = *pColorSrc;
						pColorDest[uiLevel] = (DWORD *)(((unsigned char *)pColorDest[uiLevel]) + pVertexBuffer->GetOneVertexSize());
					}
					else
					{
						pColorDest[uiLevel] = NULL;
					}
				}

			}
			pVertexBuffer->UnLock();
		}

	}
}
