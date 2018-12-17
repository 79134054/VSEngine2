#ifndef VSANIMSEQUENCEFUNC_H
#define VSANIMSEQUENCEFUNC_H
#include "VSAnimFunction.h"
namespace VSEngine2
{
	class VSStream;


	class VSGRAPHIC_API VSAnimSequenceFunc : public VSAnimFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~VSAnimSequenceFunc();
		virtual bool Update(double dAppTime);
		friend class VSSkelectonMeshNode;
		friend class VSOneParamSmoothAnimBlendSequence;
		friend class VSTwoParamAnimBlendSequence;
		VSAnimSequenceFunc(const VSUsedName & ShowName,VSAnimTree * pAnimTree);
		
		void SetAnim(const VSUsedName& AnimName);
		virtual bool SetObject(VSObject * pObject);
		FORCEINLINE const VSAnimR * GetAnim()const
		{
			return m_pAnimR;
		}
		FORCEINLINE const VSUsedName & GetAnimName()const
		{
			return m_AnimName;
		}
		VSREAL GetAnimTime();
		friend class VSPlayRootAnimEvent;
		void BeginStart();
		bool m_bOnlyAddtiveOutput;
		virtual bool IsLeafNode(){ return true; }
	protected:
		VSAnimSequenceFunc();
	protected:
		VSAnimRPtr m_pAnimR;
		VSArray<unsigned int> m_UsedBoneIndexInAnim;
		VSUsedName m_AnimName;
		struct LAST_KEY_TYPE 
		{
		public:
			LAST_KEY_TYPE()
			{
				uiLKTranslation = 0;
				uiLKScale = 0;
				uiLKRotator = 0;
			}
			~LAST_KEY_TYPE()
			{

			}
			unsigned int uiLKTranslation;
			unsigned int uiLKScale;
			unsigned int uiLKRotator;
		};
		VSArray<LAST_KEY_TYPE> m_LastKey;
		VSArray<LAST_KEY_TYPE> m_AdditiveLastKey;
		// used only in SkelectonMeshNode
		void UpDateBone();
		VSAnimSequenceFunc(VSSkelectonMeshNode * pSkelectonMeshNode);
		void LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data = 0);

		VSTransform m_SaveStartMeshComponetTransform;
		VSTransform m_SaveStartRootBoneTransform;
		
		void ComputeAnim(VSAnim * pAnim, VSArray<LAST_KEY_TYPE> & LastKey, VSArray<VSAnimAtom> &BoneOutPut);
		VSArray<VSAnimAtom> m_AdditiveBoneOutPut;
	};
	DECLARE_Ptr(VSAnimSequenceFunc);
	VSTYPE_MARCO(VSAnimSequenceFunc);

}
#endif