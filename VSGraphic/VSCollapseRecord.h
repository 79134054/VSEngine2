#ifndef VSCOLLAPSERECORD_H
#define VSCOLLAPSERECORD_H
#include "VSObject.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSCollapseRecord : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	protected:
		
		unsigned int m_uiDesireRecordID;
	public:
		unsigned int m_uiCurRecordID;
		VSCollapseRecord();
		virtual ~VSCollapseRecord() = 0;
		FORCEINLINE unsigned int DesireRecordID()const
		{
			return m_uiDesireRecordID;
		}
		FORCEINLINE void SetDesireRecordID(unsigned int DesireRecordID)
		{
			if (DesireRecordID <GetRecordNum())
			{
				m_uiDesireRecordID = DesireRecordID;
			}
		}
		virtual unsigned int GetRecordNum() = 0;
	public:

	};
	DECLARE_Ptr(VSCollapseRecord);
	VSTYPE_MARCO(VSCollapseRecord);

	class RecordTriInfo : public VSObject
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		RecordTriInfo()
		{
			m_uiKeep = 0 ;
			m_uiThrow = 0;
			m_uiLeftTraingleNum = 0;
			m_MapIndex.Clear();
		}
		~RecordTriInfo()
		{

		}
		unsigned int m_uiKeep;
		unsigned int m_uiThrow;
		unsigned int m_uiLeftTraingleNum;
		VSArray<unsigned int>	m_MapIndex;
	};
	DECLARE_Ptr(RecordTriInfo);
	VSTYPE_MARCO(RecordTriInfo);
	class VSCollapseRecordTri : public VSCollapseRecord
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSCollapseRecordTri();
		~VSCollapseRecordTri();
		
		friend class VSCreateClodTriFactory;
		
		virtual unsigned int GetRecordNum()
		{
			return m_RecordInfo.GetNum();
		}
		friend class VSTriangleSet;
	protected:
		VSArray<RecordTriInfo> m_RecordInfo;

	};
	DECLARE_Ptr(VSCollapseRecordTri);
	VSTYPE_MARCO(VSCollapseRecordTri);


	class RecordLineInfo
	{
	public:
		RecordLineInfo()
		{
			m_uiKeep = VSMAX_INTEGER ;
			m_uiThrow = VSMAX_INTEGER;
			m_uiLeftEdgeNum = 0;
			m_MapIndex = VSMAX_INTEGER;
		}
		~RecordLineInfo()
		{

		}
		unsigned int m_uiKeep;
		unsigned int m_uiThrow;
		unsigned int m_uiLeftEdgeNum;
		unsigned int	m_MapIndex;

	};
	class VSCollapseRecordLine : public VSCollapseRecord
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSCollapseRecordLine();
		~VSCollapseRecordLine();
		
		friend class VSCreateClodLineFactory;

		friend class VSLineSet;
		virtual unsigned int GetRecordNum()
		{
			return m_RecordInfo.GetNum();
		}
		friend class VSLineSet;

	protected:
		VSArray<RecordLineInfo> m_RecordInfo;

	};
	DECLARE_Ptr(VSCollapseRecordLine);
	VSTYPE_MARCO(VSCollapseRecordLine);

}
#endif