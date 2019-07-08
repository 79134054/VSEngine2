#ifndef VSMESHDATE_H
#define VSMESHDATE_H
#include "VSVertexBuffer.h"
#include "VSCollapseRecord.h"
#include "VSIndexBuffer.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSMeshDate : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum //MeshDate Type
		{
			MDT_POINT,
			MDT_LINE,
			MDT_TRIANGLE,
			MDT_MAX
		};
		virtual ~VSMeshDate() = 0;
		VSMeshDate();
		bool SetIndexBuffer(VSIndexBuffer * pIndexBuffer);
		FORCEINLINE VSIndexBuffer * GetIndexBuffer()const;

		bool SetVertexBuffer(VSVertexBuffer *pVertexBuffer);
		FORCEINLINE VSVertexBuffer *GetVertexBuffer()const;




		virtual unsigned int GetTotleNum()const = 0;

		virtual unsigned int GetMeshDateType() = 0;
		virtual bool CreateCollapseRecord(){return 1;}
		virtual void UpDateClodMesh(){}
		virtual unsigned int GetActiveNum()const{ return GetTotleNum();}
		virtual void SetLodDesirePercent(VSREAL fDesirePercent);

		bool IsClodMesh()const {return m_pCollapseRecord != NULL;}
		FORCEINLINE void RemoveClodMesh()
		{
			m_pCollapseRecord = NULL;
		}
		virtual unsigned int GetGirdNum(unsigned int uiInputNum)const = 0;
	protected:
		
		friend class VSGeometry;
		enum
		{
			DRAW_MATH_ELEMENT_LENGTH = 10000
		};
		VSVertexBufferPtr	m_pVertexBuffer;
		VSIndexBufferPtr	m_pIndexBuffer;
		

	protected:
		VSCollapseRecordPtr m_pCollapseRecord;
	};
	DECLARE_Ptr(VSMeshDate);
	VSTYPE_MARCO(VSMeshDate);
#include "VSMeshDate.inl"
}
#endif