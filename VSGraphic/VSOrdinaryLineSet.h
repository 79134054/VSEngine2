#ifndef VSORDINARYLINESET_H
#define VSORDINARYLINESET_H
#include "VSLineSet.h"
#include "VSCurve3.h"
#include "VSCurve3Subdivision.h"
#include "VSLine3.h"
#include "VSRay3.h"
#include "VSSegment3.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSOrdinaryLineSet : public VSLineSet
	{
	public:
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL

	public:
		VSOrdinaryLineSet(VSControlCurve3 * pControlCurve,VSCurve3Subdivision * pSubidvision);
		VSOrdinaryLineSet(const VSLine3 & Line);
		VSOrdinaryLineSet(const VSRay3 & Ray);
		VSOrdinaryLineSet(const VSSegment3 & Segment);
		virtual ~VSOrdinaryLineSet();
	
	protected:
		VSOrdinaryLineSet();

	};
	DECLARE_Ptr(VSOrdinaryLineSet);
	VSTYPE_MARCO(VSOrdinaryLineSet);

};
#endif