#ifndef VSONEPARAMSMOOTHANIMBLENDSEQUENCE_H
#define VSONEPARAMSMOOTHANIMBLENDSEQUENCE_H
#include "VSOneParamSmoothAnimBlend.h"
#include "VSAnimSequenceFunc.h"
namespace VSEngine2
{
	/*
	CrossFading IMMEDIATE|______|______|  [0,1) 是动画0 [1,2) 是动画1 ，以此类推
	0		1		2


	*/
	class VSStream;
	class VSGRAPHIC_API VSOneParamSmoothAnimBlendSequence : public VSOneParamSmoothAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~VSOneParamSmoothAnimBlendSequence();

		VSOneParamSmoothAnimBlendSequence(const VSUsedName & ShowName, VSAnimTree * pAnimTree);

	public:

		VSOneParamSmoothAnimBlendSequence();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		void CreateSlot(unsigned int uiWidth);
		void SetAnim(unsigned int i,const VSUsedName &AnimName);

		void SetAnimFrequency(unsigned int Index,double Frequency);
		void SetAnimPhase(unsigned int Index,double Phase);
		virtual bool SetObject(VSObject * pObject);
		virtual bool IsLeafNode(){ return true; }
	protected:
		VSArray<VSAnimSequenceFuncPtr> m_AnimSequenceFuncArray;
	};
	DECLARE_Ptr(VSOneParamSmoothAnimBlendSequence);
	VSTYPE_MARCO(VSOneParamSmoothAnimBlendSequence);
}
#endif