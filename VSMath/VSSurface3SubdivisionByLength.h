#ifndef VSSURFACE3SUBDIVISIONBYLENGTH_H
#define VSSURFACE3SUBDIVISIONBYLENGTH_H
#include "VSSurface3Subdivision.h"
/*
等分曲面方法


*/
namespace VSEngine2
{
	class VSMATH_API VSSurface3SubdivisionByLength : public VSSurface3Subdivision
	{
	public:
		VSSurface3SubdivisionByLength(unsigned int Row,unsigned int Column,VSREAL U1 = 0.0f,VSREAL U2 = 1.0f,VSREAL V1 = 0.0f,VSREAL V2 = 1.0f,unsigned int uiIterations = 32);
		virtual ~VSSurface3SubdivisionByLength();
		virtual bool Subdivision();
		virtual unsigned int GetSubdivisionMethod(){return SM_LENGTH;}
	protected:
		unsigned int m_uiIterations;
	};


}
#endif