#ifndef VSTWOPARAMANIMBLENDSEQUENCE_H
#define VSTWOPARAMANIMBLENDSEQUENCE_H
#include "VSTwoParamAnimBlend.h"
#include "VSAnimSequenceFunc.h"
namespace VSEngine2
{
	/*

		ParaMin[0]			width					ParaMax[0]

		j0				j1				j3				j4
	i0	-------------------------------------------------	ParaMin[1]
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
	i1	-------------------------------------------------	 height
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
	i2	-------------------------------------------------	ParaMax[1]


	*/
	class VSStream;
	class VSGRAPHIC_API VSTwoParamAnimBlendSequence : public VSTwoParamAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~VSTwoParamAnimBlendSequence();

		VSTwoParamAnimBlendSequence(const VSUsedName & ShowName, VSAnimTree * pAnimTree);

	public:

		VSTwoParamAnimBlendSequence();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		void CreateSlot(unsigned int uiWidth, unsigned int uiHeight);
		void SetAnim(unsigned int i,unsigned int j ,const VSUsedName &AnimName);


		void SetAnimFrequency(unsigned int i, unsigned int j, double Frequency);
		void SetAnimPhase(unsigned int i, unsigned int j, double Phase);
		virtual bool IsLeafNode(){ return true; }
	protected:
		VSArray<VSAnimSequenceFuncPtr> m_AnimSequenceFuncArray;
		
		
		VSAnimSequenceFunc * GetAnimSequenceFuntion(unsigned int i, unsigned int j);
	};
	DECLARE_Ptr(VSTwoParamAnimBlendSequence);
	VSTYPE_MARCO(VSTwoParamAnimBlendSequence);
}
#endif