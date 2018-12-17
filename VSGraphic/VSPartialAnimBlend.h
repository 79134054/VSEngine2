#ifndef VSPARTIALANIMBLEND_H
#define VSPARTIALANIMBLEND_H
#include "VSAnimBlendFunction.h"
namespace VSEngine2
{
	/*
	CrossFading IMMEDIATE |______|______|  [0,1) 是动画0 [1,2) 是动画1 ，以此类推
	0		1		2


	*/
#define  FAST_BLEND_PARIAL_ANIMITION
	class VSStream;
	class VSGRAPHIC_API VSPartialAnimBlend : public VSAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		

	public:
		virtual ~VSPartialAnimBlend();

		VSPartialAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree);

		virtual void AddInputNode();

		virtual void DeleteInputNode();

		virtual void SetBoneWeight(unsigned int i, const VSUsedName & BoneName,VSREAL Weight);

		virtual bool ComputePara(double dAppTime);
		void AddControllBone(const VSUsedName & BoneName);
	protected:
#ifdef FAST_BLEND_PARIAL_ANIMITION
		VSArray<VSMatrix3X3W> m_FastTempMatrix;
		VSMatrix3X3W m_FastRootMatrix;
#endif
		VSArray<VSArray<VSREAL>> m_Weight;
		VSMap<VSUsedName, VSArray<VSREAL>> m_BoneWeight;
		void ComputeWeight();
	public:

		VSPartialAnimBlend();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		virtual bool SetObject(VSObject * pObject);

	};
	DECLARE_Ptr(VSPartialAnimBlend);
	VSTYPE_MARCO(VSPartialAnimBlend);
}
#endif