#ifndef VSORDINARYGEOMETRY_H
#define VSORDINARYGEOMETRY_H
#include "VSTriangleSet.h"
#include "VSAABB3.h"
#include "VSOBB3.h"
#include "VSSurface3.h"
#include "VSSurface3Subdivision.h"
#include "VSPolygon3.h"
#include "VSSphere3.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSOrdinaryTriangleSet : public VSTriangleSet
	{
	public:
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSOrdinaryTriangleSet(const VSAABB3 & AABB);
		VSOrdinaryTriangleSet(const VSOBB3 & OBB);
		VSOrdinaryTriangleSet(const VSSphere3 & Sphere);
		VSOrdinaryTriangleSet(VSSurface3 * pSurface ,VSSurface3Subdivision * pSubidvision);
		VSOrdinaryTriangleSet(const VSPlane3 &Plane);
		VSOrdinaryTriangleSet(const VSTriangle3 &Triangle);
		VSOrdinaryTriangleSet(const VSRectangle3 &Rectangle);
		VSOrdinaryTriangleSet(const VSPolygon3 &Polyon);
		virtual ~VSOrdinaryTriangleSet();

	protected:
		VSOrdinaryTriangleSet();

	};
	DECLARE_Ptr(VSOrdinaryTriangleSet);
	VSTYPE_MARCO(VSOrdinaryTriangleSet);
};
#endif