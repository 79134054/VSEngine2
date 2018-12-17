#include "VSAnimSequenceFunc.h"
#include "VSBoneNode.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSAnimSequenceFunc,VSAnimFunction)
BEGIN_ADD_PROPERTY(VSAnimSequenceFunc,VSAnimFunction)
REGISTER_PROPERTY(m_AnimName,AnimName,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bOnlyAddtiveOutput, OnlyAddtiveOutput, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnimSequenceFunc)
IMPLEMENT_INITIAL_END
VSAnimSequenceFunc::VSAnimSequenceFunc(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
					:VSAnimFunction(ShowName,pAnimTree)
{

	

	VSString OutputName = _T("Output");
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::AVT_ANIM,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	m_pAnimR = NULL;
	m_LastKey.Clear();
	m_AdditiveLastKey.Clear();
	m_uiRepeatType = VSController::RT_WRAP;
	m_TriggerBeginStart += TriggerAnimEventType::Handler::FromMethod<VSAnimSequenceFunc, &VSAnimSequenceFunc::BeginStart>(this);
	m_bOnlyAddtiveOutput = false;
	
}
bool VSAnimSequenceFunc::SetObject(VSObject * pObject)
{
	if(VSAnimFunction::SetObject(pObject))
	{
		VSSkelecton * pSkelecton = GetSkelectonMeshNode()->GetSkelecton();
		VSMAC_ASSERT(pSkelecton);
		unsigned int BoneNum = pSkelecton->GetBoneNum();
		m_UsedBoneIndexInAnim.SetBufferNum(BoneNum);
		m_LastKey.SetBufferNum(BoneNum);
		VSMemset(&m_LastKey[0],0,sizeof(LAST_KEY_TYPE) * BoneNum);
		SetAnim(m_AnimName);
		return true;
	}

	return false;
}
VSAnimSequenceFunc::VSAnimSequenceFunc(VSSkelectonMeshNode * pSkelectonMeshNode)
{
	VSMAC_ASSERT(pSkelectonMeshNode);
	m_pObject = pSkelectonMeshNode;
	VSSkelecton * pSkelecton = GetSkelectonMeshNode()->GetSkelecton();
	VSMAC_ASSERT(pSkelecton);


	unsigned int BoneNum = pSkelecton->GetBoneNum();
	m_UsedBoneIndexInAnim.SetBufferNum(BoneNum);

	m_BoneOutPut.SetBufferNum(BoneNum);
	for (unsigned int i = 0; i < m_BoneOutPut.GetNum(); i++)
	{
		m_BoneOutPut[i].FromTransfrom(pSkelecton->GetBoneNode(i)->GetLocalTransform());

	}

	m_pAnimR = NULL;
	m_LastKey.Clear();
	m_LastKey.SetBufferNum(BoneNum);
	VSMemset(&m_LastKey[0],0,sizeof(LAST_KEY_TYPE) * BoneNum);
	m_uiRepeatType = VSController::RT_WRAP;
	m_TriggerBeginStart += TriggerAnimEventType::Handler::FromMethod<VSAnimSequenceFunc, &VSAnimSequenceFunc::BeginStart>(this);
	m_bOnlyAddtiveOutput = false;
}
VSAnimSequenceFunc::VSAnimSequenceFunc()
{
	m_pAnimR = NULL;
	m_uiRepeatType = VSController::RT_WRAP;
	m_TriggerBeginStart += TriggerAnimEventType::Handler::FromMethod<VSAnimSequenceFunc, &VSAnimSequenceFunc::BeginStart>(this);
	m_bOnlyAddtiveOutput = false;
}
VSAnimSequenceFunc::~VSAnimSequenceFunc()
{
	m_pAnimR = NULL;
}
void VSAnimSequenceFunc::SetAnim(const VSUsedName&  AnimName)
{
	m_AnimName = AnimName;
	const VSSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	if (!pMesh)
	{
		return ;
	}
	VSSkelecton * pSkelecton = pMesh->GetSkelecton();
	if (!pSkelecton)
	{
		return;
	}
	const VSAnimSet * pAnimSet = pMesh->GetAnimSet();
	if (!pAnimSet)
	{
		return;
	}
	VSAnimRPtr pAnimR = pAnimSet->GetAnim(AnimName);
	if (!pAnimR)
		return ;
	if (m_pAnimR == pAnimR)
	{
		return;
	}
	m_pAnimR = pAnimR;
	m_pAnimR->AddLoadEventObject(this);
	m_bStart = false;
	
}
void VSAnimSequenceFunc::LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data)
{
	const VSSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	VSMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return ;
	}
	VSSkelecton * pSkelecton = pMesh->GetSkelecton();
	VSMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return;
	}
	VSAnim * pAnim = m_pAnimR->GetResource();
	
	m_dMaxTime = pAnim->GetAnimLength();
	VSMemset(&m_LastKey[0],0,sizeof(LAST_KEY_TYPE) * m_LastKey.GetNum());

	for (unsigned int i = 0; i < m_UsedBoneIndexInAnim.GetNum(); i++)
	{
		VSBoneNode * pBone = pSkelecton->GetBoneNode(i);
		if (pBone)
		{
			bool bIsFound = false;
			for (unsigned int j = 0; j < pAnim->GetBoneKeyNum(); j++)
			{
				if (pBone->m_cName == pAnim->GetBoneKey(j)->m_cName)
				{
					m_UsedBoneIndexInAnim[i] = j;
					bIsFound = true;
					break;
				}
			}
			if (!bIsFound)
			{
				m_UsedBoneIndexInAnim[i] = VSMAX_INTEGER;
			}
		}

	}

	m_AdditiveLastKey.Destroy();
	m_AdditiveBoneOutPut.Destroy();
	if (pAnim->IsAdditive())
	{
		m_AdditiveLastKey.SetBufferNum(m_LastKey.GetNum());
		VSMemset(&m_AdditiveLastKey[0], 0, sizeof(LAST_KEY_TYPE)* m_AdditiveLastKey.GetNum());
		m_AdditiveBoneOutPut.SetBufferNum(m_UsedBoneIndexInAnim.GetNum());
		for (unsigned int i = 0; i < m_AdditiveBoneOutPut.GetNum();i++)
		{
			m_AdditiveBoneOutPut[i] = m_BoneOutPut[i];
		}
	}
	

}
void VSAnimSequenceFunc::ComputeAnim(VSAnim * pAnim, VSArray<LAST_KEY_TYPE> & LastKey, VSArray<VSAnimAtom> &BoneOutPut)
{
	for (unsigned int i = 0; i < m_UsedBoneIndexInAnim.GetNum(); i++)
	{
		if (m_UsedBoneIndexInAnim[i] != VSMAX_INTEGER)
		{
			VSBoneKey * pBoneKey = pAnim->GetBoneKey(m_UsedBoneIndexInAnim[i]);
			unsigned int ScaleNum = (unsigned int)pBoneKey->m_ScaleArray.GetNum();
			if (ScaleNum)
			{

				if (pBoneKey->m_ScaleArray[LastKey[i].uiLKScale].m_dKeyTime > m_dNowAnimTime)
				{
					LastKey[i].uiLKScale = 0;
				}
				unsigned int j;
				for (j = LastKey[i].uiLKScale; j < ScaleNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->m_ScaleArray[j].m_dKeyTime)
					{
						break;
					}
				}
				if (j == ScaleNum)
				{
					j = ScaleNum - 1;
				}
				unsigned int Key1;
				unsigned int Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				LastKey[i].uiLKScale = Key1;
				double dDiff = pBoneKey->m_ScaleArray[Key2].m_dKeyTime - pBoneKey->m_ScaleArray[Key1].m_dKeyTime;
				if (dDiff <= 0.0)
					dDiff = 1.0;
				VSREAL fFactor = (VSREAL)((m_dNowAnimTime - pBoneKey->m_ScaleArray[Key1].m_dKeyTime) / dDiff);

				if (fFactor < 0)
					fFactor = 0;
				if (fFactor > 1.0f)
					fFactor = 1.0f;

				VSVector3 Scale = pBoneKey->m_ScaleArray[Key1].m_Vector * (1 - fFactor) + pBoneKey->m_ScaleArray[Key2].m_Vector * fFactor;
				BoneOutPut[i].m_fScale = Scale;

			}

			unsigned int RotatorNum = (unsigned int)pBoneKey->m_RotatorArray.GetNum();
			if (RotatorNum)
			{
				if (pBoneKey->m_RotatorArray[LastKey[i].uiLKRotator].m_dKeyTime > m_dNowAnimTime)
				{
					LastKey[i].uiLKRotator = 0;
				}
				unsigned int j;
				for (j = LastKey[i].uiLKRotator; j < RotatorNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->m_RotatorArray[j].m_dKeyTime)
					{
						break;
					}
				}
				if (j == RotatorNum)
				{
					j = RotatorNum - 1;
				}
				unsigned int Key1;
				unsigned int Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				LastKey[i].uiLKRotator = Key1;
				double dDiff = pBoneKey->m_RotatorArray[Key2].m_dKeyTime - pBoneKey->m_RotatorArray[Key1].m_dKeyTime;
				if (dDiff <= 0.0)
					dDiff = 1.0;
				VSREAL fFactor = (VSREAL)((m_dNowAnimTime - pBoneKey->m_RotatorArray[Key1].m_dKeyTime) / dDiff);

				if (fFactor < 0)
					fFactor = 0;
				if (fFactor > 1.0f)
					fFactor = 1.0f;

				VSQuat Rotator;
#ifdef USE_ROTATOR_LINE_INTERPOLATION
				Rotator.Slerp(fFactor, pBoneKey->m_RotatorArray[Key1].m_Quat, pBoneKey->m_RotatorArray[Key2].m_Quat);
#else
				Rotator = LineInterpolation(pBoneKey->m_RotatorArray[Key1].m_Quat, pBoneKey->m_RotatorArray[Key2].m_Quat, fFactor);
#endif
				BoneOutPut[i].m_Rotator = Rotator;

			}

			unsigned int TranslationNum = (unsigned int)pBoneKey->m_TranslationArray.GetNum();
			if (TranslationNum)
			{
				if (pBoneKey->m_TranslationArray[LastKey[i].uiLKTranslation].m_dKeyTime > m_dNowAnimTime)
				{
					LastKey[i].uiLKTranslation = 0;
				}
				unsigned int j;
				for (j = LastKey[i].uiLKTranslation; j < TranslationNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->m_TranslationArray[j].m_dKeyTime)
					{
						break;
					}
				}
				if (j == TranslationNum)
				{
					j = TranslationNum - 1;
				}
				unsigned int Key1;
				unsigned int Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				LastKey[i].uiLKTranslation = Key1;
				double dDiff = pBoneKey->m_TranslationArray[Key2].m_dKeyTime - pBoneKey->m_TranslationArray[Key1].m_dKeyTime;
				if (dDiff <= 0.0)
					dDiff = 1.0;
				VSREAL fFactor = (VSREAL)((m_dNowAnimTime - pBoneKey->m_TranslationArray[Key1].m_dKeyTime) / dDiff);

				if (fFactor < 0)
					fFactor = 0;
				if (fFactor > 1.0f)
					fFactor = 1.0f;

				VSVector3 Translation = pBoneKey->m_TranslationArray[Key1].m_Vector * (1 - fFactor) + pBoneKey->m_TranslationArray[Key2].m_Vector * fFactor;

				BoneOutPut[i].m_Pos = Translation;

			}


		}
		else
		{
			// 			BoneOutPut[i].Identity();
			VSMAC_ASSERT(0);
		}

	}
}
bool VSAnimSequenceFunc::Update(double dAppTime)
{
	if(!VSAnimFunction::Update(dAppTime))
		return false;
	
	VSSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	VSMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return false;
	}
	VSSkelecton * pSkelecton = pMesh->GetSkelecton();
	VSMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return false;
	}
	if (m_pAnimR == NULL)
	{
		return false;	
	}

	if (!m_pAnimR->IsLoaded())
	{
		return false;
	}

	
	
	VSAnim * pAnim = m_pAnimR->GetResource();
	VSAnim * pBlendAnim = NULL;
	if (pAnim->IsAdditive())
	{
		VSAnimR * pBlendAnimR = pAnim->GetBlendAnim();
		if (!pBlendAnimR->IsLoaded())
		{
			return false;
		}

		pBlendAnim = pBlendAnimR->GetResource();
	}
	
	ComputeAnim(pAnim, m_LastKey, m_BoneOutPut);

	if (pBlendAnim && !m_bOnlyAddtiveOutput)
	{
		ComputeAnim(pBlendAnim, m_AdditiveLastKey, m_AdditiveBoneOutPut);

		for (unsigned int i = 0; i < m_BoneOutPut.GetNum();i++)
		{
			m_BoneOutPut[i].AddTwo(m_AdditiveBoneOutPut[i]);
		}
	}
	if (pAnim->IsRootAnim())
	{

		VSMatrix3X3W CurTansform;
		m_BoneOutPut[0].GetMatrix(CurTansform);
		VSTransform SkelectonT = pSkelecton->GetLocalTransform();
		VSMatrix3X3W RootMatrix = SkelectonT.GetCombineInverse() * m_SaveStartRootBoneTransform.GetCombineInverse() * CurTansform  * SkelectonT.GetCombine() * m_SaveStartMeshComponetTransform.GetCombine();
		m_BoneOutPut[0].FromTransfrom(m_SaveStartRootBoneTransform);
		m_RootAtom.FromMatrix(RootMatrix);

	}
	else
	{
		VSSpatial * pMeshComponent = pMesh->GetParent();
		if (pMeshComponent)
		{
			m_RootAtom.FromTransfrom(pMeshComponent->GetLocalTransform());
		}
	}
	return true;

}

void VSAnimSequenceFunc::UpDateBone()
{
	if (!m_bEnable)
	{
		return;
	}
	VSSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	VSMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return ;
	}
	VSSkelecton * pSkelecton = pMesh->GetSkelecton();
	VSMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return ;
	}
	if (!m_pAnimR || !m_pAnimR->IsLoaded())
	{
		return ;
	}
	VSAnim * pAnim = m_pAnimR->GetResource();
	VSSpatial * pMeshComponent = pMesh->GetParent();
	if (pMeshComponent)
	{

		VSMatrix3X3W RootMatrix;
		m_RootAtom.GetMatrix(RootMatrix);
		pMeshComponent->SetLocalMat(RootMatrix);

	}

	for(unsigned int i = 0 ; i < pSkelecton->GetBoneNum() ; i++)
	{
		VSBoneNode * pBone = pSkelecton->GetBoneNode(i);
		if(pBone)
		{
			//如果前面没有 skelectonMesh 没有AnimTree,如果播放addivtive动画，会出错。
			//也就是每次骨头矩阵都会叠加，如果有animtree，每次animtree更新完毕后，会重新设置骨头矩阵
			//然后additive 叠加上去，这样是正确的.
			VSMatrix3X3W BoneOutMat;	
			m_BoneOutPut[i].GetMatrix(BoneOutMat);

			pBone->SetLocalMat(BoneOutMat);

			
		}
	}
	
}
void VSAnimSequenceFunc::BeginStart()
{
	VSSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	VSMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return;
	}
	VSSkelecton * pSkelecton = pMesh->GetSkelecton();
	VSMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return;
	}
	VSBoneNode * pBone = pSkelecton->GetBoneNode(0);
	m_SaveStartRootBoneTransform = pBone->GetLocalTransform();
	VSSpatial * pMeshComponent = pMesh->GetParent();
	if (pMeshComponent)
	{

		m_SaveStartMeshComponetTransform = pMeshComponent->GetLocalTransform();
	}
}
VSREAL VSAnimSequenceFunc::GetAnimTime()
{
	if (!m_bEnable)
	{
		return 0.0f;
	}
	const VSSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	VSMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return 0.0f;
	}
	VSSkelecton * pSkelecton = pMesh->GetSkelecton();
	VSMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return 0.0f;
	}
	if (!m_pAnimR || !m_pAnimR->IsLoaded())
	{
		return 0.0f;
	}
	VSAnim * pAnim = m_pAnimR->GetResource();
	return pAnim->GetAnimLength() * (VSREAL)m_dFrequency;
}