#ifndef VSANIMSEQUENCE_H
#define VSANIMSEQUENCE_H
#include "VSObject.h"
#include "VSVector3.h"
#include "VSQuat.h"
#include "VSMatrix3X3W.h"
#include "VSName.h"
#include "VSController.h"
#include "VSResource.h"
#include <VSDelegateList.h>
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSKeyTimeInfo
	{
	public:
		VSKeyTimeInfo(){};
		~VSKeyTimeInfo(){};
		VSREAL m_dKeyTime;
	};
	class VSGRAPHIC_API VSKeyTimeVector : public VSKeyTimeInfo
	{
	public:
		VSKeyTimeVector(){};
		~VSKeyTimeVector(){};
		VSVector3 m_Vector;
	};
	class VSGRAPHIC_API VSKeyTimeReal	:public VSKeyTimeInfo
	{
	public:
		VSKeyTimeReal(){}
		~VSKeyTimeReal(){}
		VSREAL	m_Real;
	};
	class VSGRAPHIC_API VSKeyTimeQuaternion : public VSKeyTimeInfo
	{
	public:
		VSKeyTimeQuaternion(){};
		~VSKeyTimeQuaternion(){};	
		VSQuat m_Quat;

	};

	class VSGRAPHIC_API VSKeyTimeVectorCompress : public VSKeyTimeInfo
	{
	public:
		VSKeyTimeVectorCompress(){};
		~VSKeyTimeVectorCompress(){};
		unsigned short m_X;
		unsigned short m_Y;
		unsigned short m_Z;
	};
	class VSGRAPHIC_API VSKeyTimeRealCompress	:public VSKeyTimeInfo
	{
	public:
		VSKeyTimeRealCompress(){}
		~VSKeyTimeRealCompress(){}
		unsigned short m_Real;
	};
	class VSGRAPHIC_API VSKeyTimeQuaternionCompress : public VSKeyTimeInfo
	{
	public:
		VSKeyTimeQuaternionCompress(){};
		~VSKeyTimeQuaternionCompress(){};	
		unsigned short m_X;
		unsigned short m_Y;
		unsigned short m_Z;
		unsigned short m_W;
	};



	DECLARE_Ptr(VSBoneKeyCompress);
	class VSGRAPHIC_API VSBoneKey : public VSObject
	{
		//RTTI
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
		
	public:
		virtual ~VSBoneKey();

	public:
		VSBoneKey();
		VSArray<VSKeyTimeVector>			m_TranslationArray;
		VSArray<VSKeyTimeVector>			m_ScaleArray;
		VSArray<VSKeyTimeQuaternion>		m_RotatorArray;
		VSUsedName							m_cName;
		void CompressSameFrame();
		void Get(VSBoneKeyCompress * pBoneKeyCompress,
			const VSVector3 & MaxTranslation , const VSVector3 & MinTranslation ,
			const VSVector3 MaxScale,const VSVector3 MinScale);
	};
	DECLARE_Ptr(VSBoneKey);
	VSTYPE_MARCO(VSBoneKey);
	
	class VSGRAPHIC_API VSBoneKeyCompress : public VSObject
	{
		//RTTI
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL

	public:
		virtual ~VSBoneKeyCompress();

	public:
		VSBoneKeyCompress();
		VSArray<VSKeyTimeVectorCompress>			m_TranslationArray;
		VSArray<VSKeyTimeVectorCompress>			m_ScaleArray;
		VSArray<VSKeyTimeQuaternionCompress>		m_RotatorArray;
		VSUsedName									m_cName;
		void Get(VSBoneKey * pBoneKey,
			const VSVector3 & MaxTranslation , const VSVector3 & MinTranslation ,
			const VSVector3 MaxScale,const VSVector3 MinScale);
	};
	DECLARE_Ptr(VSBoneKeyCompress);
	VSTYPE_MARCO(VSBoneKeyCompress);

	
	class VSAnim;
	DECLARE_Proxy(VSAnim);
	class VSGRAPHIC_API VSAnim : public VSObject,public VSResource
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
		
	public:
		virtual ~VSAnim();

	public:
		friend class VSResourceManager;
		VSAnim();
		virtual unsigned int GetResourceType()const
		{

			return RT_ACTION;
		}
		FORCEINLINE bool IsAdditive()const
		{
			return m_pBlendAnim != NULL;
		}
		FORCEINLINE bool IsRootAnim()const
		{
			return m_bRootAnimPlay;
		}

		FORCEINLINE void SetRootMotion(bool bRootAnimPlay)
		{
			m_bRootAnimPlay = bRootAnimPlay;
		}

		VSAnimR * GetBlendAnim()const
		{
			return m_pBlendAnim;
		}
	protected:
		VSArray<VSBoneKeyPtr> m_pBoneKeyArray;
		VSArray<VSBoneKeyCompressPtr> m_pBoneKeyCompressArray;
		VSREAL m_fLength;
		bool m_bCompress;
		VSVector3 m_MaxCompressTranslation;
		VSVector3 m_MinCompressTranslation;
		VSVector3 m_MaxCompressScale;
		VSVector3 m_MinCompressScale;
		bool m_bRootAnimPlay;
		VSAnimRPtr m_pBlendAnim;
	public:
		// only use fbx
		void Compress();
		void ComputeAnimLength();
		FORCEINLINE VSREAL GetAnimLength()const
		{
			return m_fLength;
		}
		void AddBoneKey(VSBoneKey * pBoneKey);
		VSBoneKey * GetBoneKey(const VSUsedName & AnimName)const;
		VSBoneKey * GetBoneKey(unsigned int uiIndex)const;
		FORCEINLINE unsigned int GetBoneKeyNum()const
		{
			return m_pBoneKeyArray.GetNum();
		}
		virtual bool PostLoad(void * pDate = NULL);
		VSUsedName m_cName;

		VSVector3 GetTranslation(const VSUsedName & UseName,VSREAL fTime,unsigned int uiRepeatType = VSController::RT_CLAMP)const;
		VSVector3 GetScale(const VSUsedName & UseName,VSREAL fTime,unsigned int uiRepeatType = VSController::RT_CLAMP)const;
		VSQuat	  GetQuat(const VSUsedName & UseName,VSREAL fTime,unsigned int uiRepeatType = VSController::RT_CLAMP)const;
		VSMatrix3X3W GetMat(const VSUsedName & UseName,VSREAL fTime,unsigned int uiRepeatType = VSController::RT_CLAMP)const;


		VSVector3 GetTranslation(unsigned int i,VSREAL fTime,unsigned int uiRepeatType = VSController::RT_CLAMP)const;
		VSVector3 GetScale(unsigned int i,VSREAL fTime,unsigned int uiRepeatType = VSController::RT_CLAMP)const;
		VSQuat	  GetQuat(unsigned int i,VSREAL fTime,unsigned int uiRepeatType = VSController::RT_CLAMP)const;
		VSMatrix3X3W GetMat(unsigned int i,VSREAL fTime,unsigned int uiRepeatType = VSController::RT_CLAMP)const;
	public:
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		static const VSAnim *GetDefalut()
		{
			return Default;
		}
		static VSPointer<VSAnim> Default;
	};
	DECLARE_Ptr(VSAnim);
	VSTYPE_MARCO(VSAnim);
	DECLARE_Proxy(VSAnim);
#ifdef DELEGATE_PREFERRED_SYNTAX
	typedef VSDelegateEvent<void(void)> AddAnimEventType;
#else
	typedef VSDelegateEvent0<void> AddAnimEventType;
#endif
	
	class VSGRAPHIC_API VSAnimSet : public VSObject
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	public:
		virtual ~VSAnimSet();
	public:
		VSAnimSet();
	protected:
		VSMapOrder<VSUsedName,VSAnimRPtr> m_pAnimArray;

 	public:
		
		void AddAnim(VSUsedName AnimName,VSAnimR * pAnim);
		VSAnimR * GetAnim(const VSUsedName & AnimName)const;
		VSAnimR * GetAnim(unsigned int i)const;
		FORCEINLINE unsigned int GetAnimNum()const
		{
			return m_pAnimArray.GetNum();
		}



		AddAnimEventType m_AddAnimEvent;
		
	
		
		
	};
	DECLARE_Ptr(VSAnimSet);
	VSTYPE_MARCO(VSAnimSet);
	
}
#endif