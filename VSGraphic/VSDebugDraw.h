#ifndef VSDEBUGDRAW_H
#define VSDEBUGDRAW_H
#include "VSReference.h"
#include "VSDynamicBufferGeometry.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSDebugDraw : public VSReference,public VSMemObject
	{
		//Graph Debug
	public:
		VSDebugDraw();
		~VSDebugDraw();
		void AddDebugLine(const VSVector3 & P1,const VSVector3 & P2,const DWORD &Color,bool bDepth);
		void AddDebugTriangle(const VSVector3 & P1,const VSVector3 & P2,const VSVector3 &P3,
			const DWORD &Color,bool bDepth);
		void AddDebugLineAABB(const VSAABB3 & AABB,const DWORD &Color,bool bDepth);
		void AddDebugLineSphere(const VSSphere3 & Sphere,const DWORD &Color,bool bDepth);
		void AddDebugLineOBB(const VSOBB3 & OBB,const DWORD &Color,bool bDepth);

		void AddDebugTriangleAABB(const VSAABB3 & AABB,const DWORD &Color,bool bDepth);
		void AddDebugTriangleSphere(const VSSphere3 & Sphere,const DWORD &Color,bool bDepth);
		void AddDebugTriangleOBB(const VSOBB3 & OBB,const DWORD &Color,bool bDepth);
		bool m_bEnable;
	protected:
		struct DebugVertexType
		{
			VSVector3 Pos;
			DWORD Color;

		};
		VSArray<DebugVertexType> DepthDebugLineArray;
		VSArray<DebugVertexType> NoDepthDebugLineArray;

		VSMaterialRPtr m_pOnlyVertexColor;
		VSMaterialRPtr m_pOnlyVertexColorDisableDepth;
		VSVertexFormatPtr m_pDrawVertexFormat;
	public:
		void DrawDebugInfo(VSCamera * pCamera);
	};
	DECLARE_Ptr(VSDebugDraw);
}	
#endif