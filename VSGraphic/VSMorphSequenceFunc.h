#ifndef VSMORPHSEQUENCEFUNC_H
#define VSMORPHSEQUENCEFUNC_H
#include "VSMorphFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSMorphSequenceFunc : public VSMorphFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~VSMorphSequenceFunc();
		virtual void UpdateGeometryDate(unsigned int GeometryIndex);
		virtual void UpdateVertexDate(unsigned int uiVertexIndex);
		VSMorphSequenceFunc(const VSUsedName & ShowName,VSMorphTree * pMorphTree);
		void SetMorph(const VSUsedName & MorphName);
		virtual void ClearChangeFlag();
	protected:
		VSMorphSequenceFunc();
	protected:
		VSUsedName m_MorphName;
		VSVertexBuffer * m_pVertexBuffer;
		VSDataBuffer * pPosDate[MAX_NUM_POS3];
		VSDataBuffer * pNormalDate[MAX_NUM_NORMAL3];
		VSDataBuffer * pTangentDate;
		VSDataBuffer * pBinormalDate;
		VSDataBuffer * pColorDate[MAX_NUM_COLOR];
	};
	DECLARE_Ptr(VSMorphSequenceFunc);
	VSTYPE_MARCO(VSMorphSequenceFunc);
}
#endif