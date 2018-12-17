#include "VSAnimSet.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSBoneKey,VSObject)
BEGIN_ADD_PROPERTY(VSBoneKey,VSObject)
REGISTER_PROPERTY(m_TranslationArray,TranslationArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ScaleArray,ScaleArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotatorArray,RotatorArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_cName,BoneName,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSBoneKey)
IMPLEMENT_INITIAL_END
VSBoneKey::VSBoneKey()
{
}
VSBoneKey:: ~VSBoneKey()
{
}
void VSBoneKey::CompressSameFrame()
{
	// process translation
	if (m_TranslationArray.GetNum() >= 2)
	{
		VSArray<VSKeyTimeVector> NewTranslationArray;
		VSKeyTimeVector Fisrt = m_TranslationArray[0];
		NewTranslationArray.AddElement(Fisrt);
		unsigned int Index = 0;
		for (unsigned int i = 1 ; i < m_TranslationArray.GetNum() ; i++)
		{
			if (i != m_TranslationArray.GetNum() - 1)
			{			
				if (Fisrt.m_Vector == m_TranslationArray[i].m_Vector)
				{
					continue;
				}
			}
			
			if (Index != i - 1)
			{
				NewTranslationArray.AddElement(m_TranslationArray[i - 1]);
			}
			NewTranslationArray.AddElement(m_TranslationArray[i]);
			Fisrt = m_TranslationArray[i];
			Index = i;
		}

		m_TranslationArray = NewTranslationArray;
	}

	// process scale
	if (m_ScaleArray.GetNum() >= 2)
	{
		VSArray<VSKeyTimeVector> NewScaleArray;
		VSKeyTimeVector Fisrt = m_ScaleArray[0];
		NewScaleArray.AddElement(Fisrt);
		unsigned int Index = 0;
		for (unsigned int i = 1 ; i < m_ScaleArray.GetNum(); i++)
		{
			if (i != m_ScaleArray.GetNum() - 1)
			{			
				if (Fisrt.m_Vector == m_ScaleArray[i].m_Vector)
				{
					continue;
				}
			}

			if (Index != i - 1)
			{
				NewScaleArray.AddElement(m_ScaleArray[i - 1]);
			}
			NewScaleArray.AddElement(m_ScaleArray[i]);
			Fisrt = m_ScaleArray[i];
			Index = i;
		}

		m_ScaleArray = NewScaleArray;
	}
	// process Rotator
	if (m_RotatorArray.GetNum() >= 2)
	{
		VSArray<VSKeyTimeQuaternion> NewRotatorArray;
		VSKeyTimeQuaternion Fisrt = m_RotatorArray[0];
		NewRotatorArray.AddElement(Fisrt);
		unsigned int Index = 0;
		for (unsigned int i = 1 ; i < m_RotatorArray.GetNum(); i++)
		{
			if (i != m_RotatorArray.GetNum() - 1)
			{			
				if (Fisrt.m_Quat == m_RotatorArray[i].m_Quat)
				{
					continue;
				}
			}

			if (Index != i - 1)
			{
				NewRotatorArray.AddElement(m_RotatorArray[i - 1]);
			}
			NewRotatorArray.AddElement(m_RotatorArray[i]);
			Fisrt = m_RotatorArray[i];
			Index = i;
		}

		m_RotatorArray = NewRotatorArray;
	}
	
}
void VSBoneKey::Get(VSBoneKeyCompress * pBoneKeyCompress,
		 const VSVector3 & MaxTranslation , const VSVector3 & MinTranslation ,
		 const VSVector3 MaxScale,const VSVector3 MinScale)
{
	pBoneKeyCompress->m_cName = m_cName;
	for (unsigned int i = 0 ; i < m_ScaleArray.GetNum() ; i++)
	{
		VSKeyTimeVectorCompress Compress;
		Compress.m_dKeyTime = m_ScaleArray[i].m_dKeyTime;
		Compress.m_X = CompressFloat(m_ScaleArray[i].m_Vector.x,MaxScale.x,MinScale.x);
		Compress.m_Y = CompressFloat(m_ScaleArray[i].m_Vector.y,MaxScale.y,MinScale.y);
		Compress.m_Z = CompressFloat(m_ScaleArray[i].m_Vector.z,MaxScale.z,MinScale.z);
		pBoneKeyCompress->m_ScaleArray.AddElement(Compress);
	}

	for (unsigned int i = 0 ; i < m_TranslationArray.GetNum() ; i++)
	{
		VSKeyTimeVectorCompress Compress;
		Compress.m_dKeyTime = m_TranslationArray[i].m_dKeyTime;
		Compress.m_X = CompressFloat(m_TranslationArray[i].m_Vector.x,MaxTranslation.x,MinTranslation.x);
		Compress.m_Y = CompressFloat(m_TranslationArray[i].m_Vector.y,MaxTranslation.y,MinTranslation.y);
		Compress.m_Z = CompressFloat(m_TranslationArray[i].m_Vector.z,MaxTranslation.z,MinTranslation.z);
		pBoneKeyCompress->m_TranslationArray.AddElement(Compress);
	}

	for (unsigned int i = 0 ; i < m_RotatorArray.GetNum() ; i++)
	{
		VSKeyTimeQuaternionCompress Compress;
		Compress.m_dKeyTime = m_RotatorArray[i].m_dKeyTime;
		Compress.m_X = CompressFloat(m_RotatorArray[i].m_Quat.x,1.0f,-1.0f);
		Compress.m_Y = CompressFloat(m_RotatorArray[i].m_Quat.y,1.0f,-1.0f);
		Compress.m_Z = CompressFloat(m_RotatorArray[i].m_Quat.z,1.0f,-1.0f);
		Compress.m_W = CompressFloat(m_RotatorArray[i].m_Quat.w,1.0f,-1.0f);
		pBoneKeyCompress->m_RotatorArray.AddElement(Compress);
	}
}
IMPLEMENT_RTTI(VSBoneKeyCompress,VSObject)
BEGIN_ADD_PROPERTY(VSBoneKeyCompress,VSObject)
REGISTER_PROPERTY(m_TranslationArray,TranslationArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ScaleArray,ScaleArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotatorArray,RotatorArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_cName,BoneName,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSBoneKeyCompress)
IMPLEMENT_INITIAL_END
VSBoneKeyCompress::VSBoneKeyCompress()
{
}
VSBoneKeyCompress:: ~VSBoneKeyCompress()
{
}
void VSBoneKeyCompress::Get(VSBoneKey * pBoneKey,
					const VSVector3 & MaxTranslation , const VSVector3 & MinTranslation ,
					const VSVector3 MaxScale,const VSVector3 MinScale)
{
	pBoneKey->m_cName = m_cName;
	for (unsigned int i = 0 ; i < m_ScaleArray.GetNum() ; i++)
	{
		VSKeyTimeVector Decompress;
		Decompress.m_dKeyTime = m_ScaleArray[i].m_dKeyTime;
		Decompress.m_Vector.x = DecompressFloat(m_ScaleArray[i].m_X,MaxScale.x,MinScale.x);
		Decompress.m_Vector.y = DecompressFloat(m_ScaleArray[i].m_Y,MaxScale.y,MinScale.y);
		Decompress.m_Vector.z = DecompressFloat(m_ScaleArray[i].m_Z,MaxScale.z,MinScale.z);
		pBoneKey->m_ScaleArray.AddElement(Decompress);
	}

	for (unsigned int i = 0 ; i < m_TranslationArray.GetNum() ; i++)
	{
		VSKeyTimeVector Decompress;
		Decompress.m_dKeyTime = m_TranslationArray[i].m_dKeyTime;
		Decompress.m_Vector.x = DecompressFloat(m_TranslationArray[i].m_X,MaxTranslation.x,MinTranslation.x);
		Decompress.m_Vector.y = DecompressFloat(m_TranslationArray[i].m_Y,MaxTranslation.y,MinTranslation.y);
		Decompress.m_Vector.z = DecompressFloat(m_TranslationArray[i].m_Z,MaxTranslation.z,MinTranslation.z);
		pBoneKey->m_TranslationArray.AddElement(Decompress);
	}

	for (unsigned int i = 0 ; i < m_RotatorArray.GetNum() ; i++)
	{
		VSKeyTimeQuaternion Decompress;
		Decompress.m_dKeyTime = m_RotatorArray[i].m_dKeyTime;
		Decompress.m_Quat.x = DecompressFloat(m_RotatorArray[i].m_X,1.0f,-1.0f);
		Decompress.m_Quat.y = DecompressFloat(m_RotatorArray[i].m_Y,1.0f,-1.0f);
		Decompress.m_Quat.z = DecompressFloat(m_RotatorArray[i].m_Z,1.0f,-1.0f);
		Decompress.m_Quat.w = DecompressFloat(m_RotatorArray[i].m_W,1.0f,-1.0f);
		pBoneKey->m_RotatorArray.AddElement(Decompress);
	}
}
bool VSAnim::ms_bIsEnableASYNLoader = true;
bool VSAnim::ms_bIsEnableGC = true;
VSAnimPtr VSAnim::Default = NULL;
IMPLEMENT_RTTI(VSAnim,VSObject)
BEGIN_ADD_PROPERTY(VSAnim,VSObject)
REGISTER_PROPERTY(m_fLength,Length,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pBoneKeyArray,BoneKeyArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pBoneKeyCompressArray, BoneKeyCompressArray, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bCompress,Compress,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MaxCompressTranslation,MaxCompressTranslation,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MinCompressTranslation,MinCompressTranslation,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MinCompressScale,MinCompressScale,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MaxCompressScale,MaxCompressScale,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pBlendAnim, BlendAnim, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bRootAnimPlay, bRootAnimPlay, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnim)
IMPLEMENT_INITIAL_END
VSAnim::VSAnim()
{
	m_pBoneKeyArray.Clear();
	m_fLength = 0.0f;
	m_bCompress = false;
	m_MaxCompressTranslation = VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	m_MinCompressTranslation = VSVector3(-VSMAX_REAL,-VSMAX_REAL,-VSMAX_REAL);
	m_MaxCompressScale = VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	m_MinCompressScale = VSVector3(-VSMAX_REAL,-VSMAX_REAL,-VSMAX_REAL);
	m_pBlendAnim = NULL;
	m_bRootAnimPlay = false;
}
VSAnim:: ~VSAnim()
{
	m_pBoneKeyArray.Clear();
}
void VSAnim::ComputeAnimLength()
{
	if (m_pBlendAnim)
	{
		while (!m_pBlendAnim->IsLoaded())
		{

		}
		VSAnim * pBlend = m_pBlendAnim->GetResource();
		m_fLength = pBlend->GetAnimLength();
	}
	else
	{
		m_fLength = 0.0f;
		for (unsigned int i = 0; i < m_pBoneKeyArray.GetNum(); i++)
		{
			VSBoneKey * pBoneKey = m_pBoneKeyArray[i];
			for (unsigned int j = 0; j < pBoneKey->m_ScaleArray.GetNum(); j++)
			{
				if (m_fLength < pBoneKey->m_ScaleArray[j].m_dKeyTime)
					m_fLength = (VSREAL)pBoneKey->m_ScaleArray[j].m_dKeyTime;
			}
			for (unsigned int j = 0; j < pBoneKey->m_TranslationArray.GetNum(); j++)
			{
				if (m_fLength < pBoneKey->m_TranslationArray[j].m_dKeyTime)
					m_fLength = (VSREAL)pBoneKey->m_TranslationArray[j].m_dKeyTime;
			}
			for (unsigned int j = 0; j < pBoneKey->m_RotatorArray.GetNum(); j++)
			{
				if (m_fLength < pBoneKey->m_RotatorArray[j].m_dKeyTime)
					m_fLength = (VSREAL)pBoneKey->m_RotatorArray[j].m_dKeyTime;
			}
		}
		if (m_fLength < 0.0f)
		{
			m_fLength = 0.0f;
		}
	}
}
void VSAnim::AddBoneKey(VSBoneKey * pBoneKey)
{
	if(pBoneKey)
	{
		m_pBoneKeyArray.AddElement(pBoneKey);
	}
}
void VSAnim::Compress()
{
	if (!m_pBoneKeyArray.GetNum())
	{
		return ;
	}
	m_bCompress = true;
	m_pBoneKeyCompressArray.Clear();

	m_MaxCompressScale = VSVector3(-VSMAX_REAL,-VSMAX_REAL,-VSMAX_REAL);
	m_MaxCompressTranslation = VSVector3(-VSMAX_REAL,-VSMAX_REAL,-VSMAX_REAL);

	m_MinCompressScale = VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	m_MinCompressTranslation = VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);

	for (unsigned int i = 0 ; i < m_pBoneKeyArray.GetNum() ;i++)
	{
		for (unsigned int j = 0 ; j < m_pBoneKeyArray[i]->m_ScaleArray.GetNum() ; j++)
		{
			for (unsigned int k = 0 ; k < 3 ; k++)
			{
				if (m_MaxCompressScale.m[k] < m_pBoneKeyArray[i]->m_ScaleArray[j].m_Vector.m[k])
				{
					m_MaxCompressScale.m[k] = m_pBoneKeyArray[i]->m_ScaleArray[j].m_Vector.m[k];
				}

				if (m_MinCompressScale.m[k] > m_pBoneKeyArray[i]->m_ScaleArray[j].m_Vector.m[k])
				{
					m_MinCompressScale.m[k] = m_pBoneKeyArray[i]->m_ScaleArray[j].m_Vector.m[k];
				}
			}
			
		}
		
		for (unsigned int j = 0 ; j < m_pBoneKeyArray[i]->m_TranslationArray.GetNum() ; j++)
		{
			for (unsigned int k = 0 ; k < 3 ; k++)
			{
				if (m_MaxCompressTranslation.m[k] < m_pBoneKeyArray[i]->m_TranslationArray[j].m_Vector.m[k])
				{
					m_MaxCompressTranslation.m[k] = m_pBoneKeyArray[i]->m_TranslationArray[j].m_Vector.m[k];
				}

				if (m_MinCompressTranslation.m[k] > m_pBoneKeyArray[i]->m_TranslationArray[j].m_Vector.m[k])
				{
					m_MinCompressTranslation.m[k] = m_pBoneKeyArray[i]->m_TranslationArray[j].m_Vector.m[k];
				}
			}

		}
	}
	for (unsigned int i = 0 ; i < m_pBoneKeyArray.GetNum() ; i++)
	{
		m_pBoneKeyArray[i]->CompressSameFrame();
	}
	for (unsigned int i = 0 ; i < m_pBoneKeyArray.GetNum() ; i++)
	{
		VSBoneKeyCompress * pBoneKeyCompress = VS_NEW VSBoneKeyCompress();
		m_pBoneKeyArray[i]->Get(pBoneKeyCompress,m_MaxCompressTranslation,m_MinCompressTranslation,m_MaxCompressScale,m_MinCompressScale);
		m_pBoneKeyCompressArray.AddElement(pBoneKeyCompress);
	}
	m_pBoneKeyArray.Destroy();

}
bool VSAnim::PostLoad(void * pDate)
{
	VSObject::PostLoad(pDate);
	if (m_bCompress)
	{
		if (m_pBoneKeyCompressArray.GetNum() ==0 || m_pBoneKeyArray.GetNum() > 0)
		{
			VSMAC_ASSERT(0);
			return false;
		}
		for (unsigned int i = 0 ; i < m_pBoneKeyCompressArray.GetNum() ; i++)
		{
			VSBoneKey * pBoneKey = VS_NEW VSBoneKey();
			m_pBoneKeyCompressArray[i]->Get(pBoneKey,m_MaxCompressTranslation,m_MinCompressTranslation,m_MaxCompressScale,m_MinCompressScale);
			m_pBoneKeyArray.AddElement(pBoneKey);
		}
		m_pBoneKeyCompressArray.Destroy();
	}
	return true;
}
VSBoneKey * VSAnim::GetBoneKey(const VSUsedName & AnimName)const
{
	for(unsigned int i = 0 ; i < m_pBoneKeyArray.GetNum() ; i++)
		if(m_pBoneKeyArray[i])
		{
			if(m_pBoneKeyArray[i]->m_cName == AnimName)
				return m_pBoneKeyArray[i];
		}

		return NULL;

}
VSBoneKey * VSAnim::GetBoneKey(unsigned int i)const
{
	if(i >=  m_pBoneKeyArray.GetNum())
		return NULL;
	return m_pBoneKeyArray[i];
}
VSVector3 VSAnim::GetTranslation(const VSUsedName & UseName,VSREAL fTime,unsigned int uiRepeatType)const
{
	VSBoneKey * pBoneKey = GetBoneKey(UseName);
	unsigned int uiKeyNum = pBoneKey->m_TranslationArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return VSVector3(0,0,0);
	}
	
	if (uiRepeatType == VSController::RT_NONE || uiRepeatType == VSController::RT_MAX)
	{
		uiRepeatType = VSController::RT_CLAMP;
	}
	VSREAL fNewTime = (VSREAL)VSController::GetTime(fTime,pBoneKey->m_TranslationArray[0].m_dKeyTime,
						pBoneKey->m_TranslationArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_TranslationArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}
	
	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_TranslationArray[uiIndex2].m_dKeyTime - pBoneKey->m_TranslationArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	VSREAL fFactor = (VSREAL)((fNewTime - pBoneKey->m_TranslationArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	return pBoneKey->m_TranslationArray[uiIndex1].m_Vector * (1.0f - fFactor) + pBoneKey->m_TranslationArray[uiIndex2].m_Vector * fFactor ;
}
VSVector3 VSAnim::GetScale(const VSUsedName & UseName,VSREAL fTime,unsigned int uiRepeatType)const
{
	VSBoneKey * pBoneKey = GetBoneKey(UseName);
	unsigned int uiKeyNum = pBoneKey->m_ScaleArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return VSVector3(1.0f,1.0f,1.0f);
	}

	if (uiRepeatType == VSController::RT_NONE || uiRepeatType == VSController::RT_MAX)
	{
		uiRepeatType = VSController::RT_CLAMP;
	}
	VSREAL fNewTime = (VSREAL)VSController::GetTime(fTime,pBoneKey->m_ScaleArray[0].m_dKeyTime,
		pBoneKey->m_ScaleArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_ScaleArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}

	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_ScaleArray[uiIndex2].m_dKeyTime - pBoneKey->m_ScaleArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	VSREAL fFactor = (VSREAL)((fNewTime - pBoneKey->m_ScaleArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	return pBoneKey->m_ScaleArray[uiIndex1].m_Vector * (1.0f - fFactor) + pBoneKey->m_ScaleArray[uiIndex2].m_Vector * fFactor ;
}
VSQuat	  VSAnim::GetQuat(const VSUsedName & UseName,VSREAL fTime,unsigned int uiRepeatType)const
{
	VSBoneKey * pBoneKey = GetBoneKey(UseName);
	unsigned int uiKeyNum = pBoneKey->m_RotatorArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return VSQuat();
	}

	if (uiRepeatType == VSController::RT_NONE || uiRepeatType == VSController::RT_MAX)
	{
		uiRepeatType = VSController::RT_CLAMP;
	}
	VSREAL fNewTime = (VSREAL)VSController::GetTime(fTime,pBoneKey->m_RotatorArray[0].m_dKeyTime,
		pBoneKey->m_RotatorArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_RotatorArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}

	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_RotatorArray[uiIndex2].m_dKeyTime - pBoneKey->m_RotatorArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	VSREAL fFactor = (VSREAL)((fNewTime - pBoneKey->m_RotatorArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	VSQuat Rotator;
#ifdef USE_ROTATOR_LINE_INTERPOLATION
	Rotator.Slerp(fFactor,pBoneKey->m_RotatorArray[uiIndex1].m_Quat,pBoneKey->m_RotatorArray[uiIndex2].m_Quat);
#else
	Rotator = LineInterpolation(pBoneKey->m_RotatorArray[uiIndex1].m_Quat,pBoneKey->m_RotatorArray[uiIndex2].m_Quat,fFactor);
#endif
	return Rotator;
}
VSMatrix3X3W VSAnim::GetMat(const VSUsedName & UseName,VSREAL fTime,unsigned int uiRepeatType)const
{
	VSVector3 Scale = GetScale(UseName,fTime,uiRepeatType);
	VSQuat Rotator = GetQuat(UseName,fTime,uiRepeatType);
	VSVector3 Translate = GetTranslation(UseName,fTime,uiRepeatType);

	VSMatrix3X3W OutPut;

	VSMatrix3X3 mRotate;
	Rotator.GetMatrix(mRotate);
	VSMatrix3X3 Mat;
	Mat = VSMatrix3X3(	mRotate._00 * Scale.x,mRotate._01 * Scale.x,mRotate._02 * Scale.x,
						mRotate._10 * Scale.y,mRotate._11 * Scale.y,mRotate._12 * Scale.y,
						mRotate._20 * Scale.z,mRotate._21 * Scale.z,mRotate._22 * Scale.z);

	OutPut.AddTranslate(Translate);
	OutPut.Add3X3(Mat);
	return OutPut;	

}

VSVector3 VSAnim::GetTranslation(unsigned int uiIndex,VSREAL fTime,unsigned int uiRepeatType)const
{
	VSBoneKey * pBoneKey = GetBoneKey(uiIndex);
	unsigned int uiKeyNum = pBoneKey->m_TranslationArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return VSVector3(0,0,0);
	}

	if (uiRepeatType == VSController::RT_NONE || uiRepeatType == VSController::RT_MAX)
	{
		uiRepeatType = VSController::RT_CLAMP;
	}
	VSREAL fNewTime = (VSREAL)VSController::GetTime(fTime,pBoneKey->m_TranslationArray[0].m_dKeyTime,
		pBoneKey->m_TranslationArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_TranslationArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}

	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_TranslationArray[uiIndex2].m_dKeyTime - pBoneKey->m_TranslationArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	VSREAL fFactor = (VSREAL)((fNewTime - pBoneKey->m_TranslationArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	return pBoneKey->m_TranslationArray[uiIndex1].m_Vector * (1 - fFactor) + pBoneKey->m_TranslationArray[uiIndex2].m_Vector * fFactor ;
}
VSVector3 VSAnim::GetScale(unsigned int uiIndex,VSREAL fTime,unsigned int uiRepeatType)const
{
	VSBoneKey * pBoneKey = GetBoneKey(uiIndex);
	unsigned int uiKeyNum = pBoneKey->m_ScaleArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return VSVector3(1.0f,1.0f,1.0f);
	}

	if (uiRepeatType == VSController::RT_NONE || uiRepeatType == VSController::RT_MAX)
	{
		uiRepeatType = VSController::RT_CLAMP;
	}
	VSREAL fNewTime = (VSREAL)VSController::GetTime(fTime,pBoneKey->m_ScaleArray[0].m_dKeyTime,
		pBoneKey->m_ScaleArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_ScaleArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}

	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_ScaleArray[uiIndex2].m_dKeyTime - pBoneKey->m_ScaleArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	VSREAL fFactor = (VSREAL)((fNewTime - pBoneKey->m_ScaleArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	return pBoneKey->m_ScaleArray[uiIndex1].m_Vector * (1 - fFactor) + pBoneKey->m_ScaleArray[uiIndex2].m_Vector * fFactor ;
}
VSQuat	  VSAnim::GetQuat(unsigned int uiIndex,VSREAL fTime,unsigned int uiRepeatType)const
{
	VSBoneKey * pBoneKey = GetBoneKey(uiIndex);
	unsigned int uiKeyNum = pBoneKey->m_RotatorArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return VSQuat();
	}

	if (uiRepeatType == VSController::RT_NONE || uiRepeatType == VSController::RT_MAX)
	{
		uiRepeatType = VSController::RT_CLAMP;
	}
	VSREAL fNewTime = (VSREAL)VSController::GetTime(fTime,pBoneKey->m_RotatorArray[0].m_dKeyTime,
		pBoneKey->m_RotatorArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_RotatorArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}

	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_RotatorArray[uiIndex2].m_dKeyTime - pBoneKey->m_RotatorArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	VSREAL fFactor = (VSREAL)((fNewTime - pBoneKey->m_RotatorArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	VSQuat Rotator;
#ifdef USE_ROTATOR_LINE_INTERPOLATION
	Rotator.Slerp(fFactor,pBoneKey->m_RotatorArray[uiIndex1].m_Quat,pBoneKey->m_RotatorArray[uiIndex2].m_Quat);
#else	
	Rotator = LineInterpolation(pBoneKey->m_RotatorArray[uiIndex1].m_Quat,pBoneKey->m_RotatorArray[uiIndex2].m_Quat,fFactor);
#endif	
	return Rotator;
}
VSMatrix3X3W VSAnim::GetMat(unsigned int uiIndex,VSREAL fTime,unsigned int uiRepeatType)const
{
	VSVector3 Scale = GetScale(uiIndex,fTime,uiRepeatType);
	VSQuat Rotator = GetQuat(uiIndex,fTime,uiRepeatType);
	VSVector3 Translate = GetTranslation(uiIndex,fTime,uiRepeatType);

	VSMatrix3X3W OutPut;
	VSMatrix3X3 mRotate;
	Rotator.GetMatrix(mRotate);
	VSMatrix3X3 Mat;
	Mat = VSMatrix3X3(	mRotate._00 * Scale.x,mRotate._01 * Scale.x,mRotate._02 * Scale.x,
						mRotate._10 * Scale.y,mRotate._11 * Scale.y,mRotate._12 * Scale.y,
						mRotate._20 * Scale.z,mRotate._21 * Scale.z,mRotate._22 * Scale.z);

	OutPut.AddTranslate(Translate);
	OutPut.Add3X3(Mat);
	return OutPut;
	
}
/*******************************VSAnimSet*****************************************/
IMPLEMENT_RTTI(VSAnimSet,VSObject)
BEGIN_ADD_PROPERTY(VSAnimSet,VSObject)
REGISTER_PROPERTY(m_pAnimArray,AnimArray,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnimSet)
IMPLEMENT_INITIAL_END
VSAnimSet::VSAnimSet()
{
	m_pAnimArray.Clear();
}
VSAnimSet:: ~VSAnimSet()
{
	m_pAnimArray.Clear();
	
}
void VSAnimSet::AddAnim(VSUsedName AnimName,VSAnimR * pAnim)
{
	if(!pAnim)
		return;
	m_pAnimArray.AddElement(AnimName,pAnim);
	m_AddAnimEvent();
}
VSAnimR * VSAnimSet::GetAnim(const VSUsedName & AnimName)const
{
	unsigned int Index = m_pAnimArray.Find(AnimName);
	if (Index < m_pAnimArray.GetNum())
	{
		return m_pAnimArray[Index].Value;
	}
	return NULL;
}
VSAnimR * VSAnimSet::GetAnim(unsigned int i)const
{
	if(i >= m_pAnimArray.GetNum())
		return NULL;
	return m_pAnimArray[i].Value;
}

