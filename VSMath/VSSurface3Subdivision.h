#ifndef VSSURFACE3SUBDIVISION_H
#define VSSURFACE3SUBDIVISION_H
#include "VSControlSurface3.h"
#include "VSVector2.h"
/*
曲面划分方法类


*/
namespace VSEngine2
{
	class VSMATH_API VSSurface3Subdivision
	{
	public:
		enum  //Subdivision Method
		{
			SM_UNIFORM,
			SM_LENGTH,
			SM_MAX
		};
		VSSurface3Subdivision();
		virtual ~VSSurface3Subdivision() = 0;
		virtual bool Subdivision() = 0;
		virtual unsigned int GetSubdivisionMethod() = 0;
		FORCEINLINE bool SetSurface(VSSurface3 * pSurface);

		FORCEINLINE VSVector3 * GetPoints()const;
		FORCEINLINE unsigned int GetRow()const;
		FORCEINLINE unsigned int GetColumn()const;
		FORCEINLINE VSVector2 * GetUV()const;
	protected:
		VSSurface3 * m_pSurface;
		VSVector3 *m_pPoint;
		unsigned int m_uiRow;
		unsigned int m_uiColumn;
		VSREAL	m_fU1,m_fU2;
		VSREAL	m_fV1,m_fV2;
		VSVector2 * m_pUV;
	};
#include "VSSurface3Subdivision.inl"
}
#endif