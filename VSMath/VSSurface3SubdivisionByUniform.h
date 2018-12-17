#ifndef VSSURFACE3SUBDIVISIONBYUNIFORM_H
#define VSSURFACE3SUBDIVISIONBYUNIFORM_H
#include "VSSurface3Subdivision.h"
/*
等分曲面方法


*/
namespace VSEngine2
{
	class VSMATH_API VSSurface3SubdivisionByUniform : public VSSurface3Subdivision
	{
	public:
		VSSurface3SubdivisionByUniform(unsigned int uiRow,unsigned int uiColumn,VSREAL U1 = 0.0f,VSREAL U2 = 1.0f,VSREAL V1 = 0.0f,VSREAL V2 = 1.0f);
		virtual ~VSSurface3SubdivisionByUniform();
		virtual bool Subdivision();
		virtual unsigned int GetSubdivisionMethod(){return SM_UNIFORM;}
	};


}
#endif