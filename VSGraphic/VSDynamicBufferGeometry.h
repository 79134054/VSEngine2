#ifndef VSDYNAMICBUFFERGEOMETRY_H
#define VSDYNAMICBUFFERGEOMETRY_H
#include "VSGeometry.h"
#include "VSUseBuffer.h"
#include "VSVertexFormat.h"
namespace VSEngine2
{
	class VSVertexBuffer;
	class VSIndexBuffer;
	class VSGRAPHIC_API VSDynamicBufferGeometry : public VSGeometry
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VSDynamicBufferGeometry();
		virtual ~VSDynamicBufferGeometry() = 0;
		virtual void ClearInfo() = 0 ;
		virtual bool HaveDate() = 0;
		virtual unsigned int UpdateGeometry() = 0;
		virtual void Draw(VSCamera * pCamera);
	};
	DECLARE_Ptr(VSDynamicBufferGeometry);

	class VSGRAPHIC_API VSDVGeometry : public VSDynamicBufferGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		VSDVGeometry();
		void Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
								unsigned int uiMeshDateType,
								unsigned int uiVertexNum);
		virtual ~VSDVGeometry();
		bool Add(const void * pVeretexDate,unsigned int uiVertexSize);
		void * NewGetV(unsigned int uiVertexSize);
		//更新和视点无关的Mesh 信息
		virtual unsigned int UpdateGeometry();
		virtual void ClearInfo();
		virtual bool HaveDate()
		{
			if (!m_pVertexUseBuffer->GetElementNum())
			{
				return false;
			}
			return true;
		}
		unsigned int GetMaxRenderVertexNum()const;
	protected:
		
		VSUseBufferPtr m_pVertexUseBuffer;

		unsigned int m_uiCurVUseBufferElementIndex;
		VSUseBufferPtr m_pVertexUseBufferRender;


	};
	DECLARE_Ptr(VSDVGeometry);



	class VSGRAPHIC_API VSDVDIGeometry : public VSDynamicBufferGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		VSDVDIGeometry();
		void Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
			unsigned int uiMeshDateType,
			unsigned int uiVertexNum,
			unsigned int uiIndexNum);
		virtual ~VSDVDIGeometry();
		bool Add(const void * pVeretexDate,unsigned int uiVertexSize,
			const void * pIndexDate,unsigned int uiIndexSize);

		void * NewGetV(unsigned int uiVertexSize);
		void * NewGetI(unsigned int uiIndexSize);
		//更新和视点无关的Mesh 信息
		virtual unsigned int UpdateGeometry();
		virtual void ClearInfo();
		virtual bool HaveDate()
		{
			if (!m_pVertexUseBuffer->GetElementNum())
			{
				return false;
			}
			return true;
		}
	protected:
		VSUseBufferPtr m_pVertexUseBuffer;
		VSUseBufferPtr m_pIndexUseBuffer;
		unsigned int m_uiCurVUseBufferElementIndex;
		unsigned int m_uiCurIUseBufferElementIndex;

		VSUseBufferPtr m_pVertexUseBufferRender;
		VSUseBufferPtr m_pIndexUseBufferRender;


	};
	DECLARE_Ptr(VSDVDIGeometry);



	class VSGRAPHIC_API VSSVDIGeometry : public VSDynamicBufferGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		VSSVDIGeometry();
		void Create(unsigned int uiMeshDateType,VSVertexBuffer * pVertexBuffer,unsigned int uiIndexNum);
		virtual ~VSSVDIGeometry();
		bool Add(const void * pIndexDate,unsigned int uiIndexSize);
		void * NewGetI(unsigned int uiIndexSize);
		//更新和视点无关的Mesh 信息
		virtual unsigned int UpdateGeometry();
		virtual void ClearInfo();
		virtual bool HaveDate()
		{
			if (!m_pIndexUseBuffer->GetElementNum())
			{
				return false;
			}
			return true;
		}
	protected:
		
		VSUseBufferPtr m_pIndexUseBuffer;
		unsigned int m_uiCurIUseBufferElementIndex;
		VSUseBufferPtr m_pIndexUseBufferRender;

	};
	DECLARE_Ptr(VSSVDIGeometry);



	class VSGRAPHIC_API VSDVSIGeometry : public VSDynamicBufferGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		VSDVSIGeometry();
		void Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
					unsigned int uiMeshDateType,
					unsigned int uiVertexNum,
					VSIndexBuffer * pIndexBuffer);
		virtual ~VSDVSIGeometry();
		bool Add(const void * pVeretexDate,unsigned int uiVertexSize);
		void * NewGetV(unsigned int uiVertexSize);
		//更新和视点无关的Mesh 信息
		virtual unsigned int UpdateGeometry();
		virtual void ClearInfo();
		virtual bool HaveDate()
		{
			if (!m_pVertexUseBuffer->GetElementNum())
			{
				return false;
			}
			return true;
		}
	protected:

		VSUseBufferPtr m_pVertexUseBuffer;

		unsigned int m_uiCurVUseBufferElementIndex;
		VSUseBufferPtr m_pVertexUseBufferRender;


	};
	DECLARE_Ptr(VSDVSIGeometry);
}
#endif