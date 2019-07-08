#include "VSCollapseRecord.h"
#include "VSStream.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSCollapseRecord,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSCollapseRecord)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSCollapseRecord,VSObject)
END_ADD_PROPERTY
VSCollapseRecord::VSCollapseRecord()
{
	m_uiCurRecordID = 0;
	m_uiDesireRecordID = 0;
}
VSCollapseRecord::~VSCollapseRecord()
{
	m_uiCurRecordID = 0;
	m_uiDesireRecordID = 0;
}
IMPLEMENT_RTTI(RecordTriInfo,VSObject)
BEGIN_ADD_PROPERTY(RecordTriInfo,VSObject)
REGISTER_PROPERTY(m_uiKeep,Keep,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiThrow,Throw,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiLeftTraingleNum,LeftTraingleNum,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MapIndex,MapIndex,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(RecordTriInfo)
IMPLEMENT_INITIAL_END

IMPLEMENT_RTTI(VSCollapseRecordTri,VSCollapseRecord)
BEGIN_ADD_PROPERTY(VSCollapseRecordTri,VSCollapseRecord)
REGISTER_PROPERTY(m_RecordInfo,RecordInfo,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCollapseRecordTri)
IMPLEMENT_INITIAL_END
VSCollapseRecordTri::VSCollapseRecordTri()
{

	m_RecordInfo.Clear();
}
VSCollapseRecordTri::~VSCollapseRecordTri()
{
	m_RecordInfo.Clear();
}




IMPLEMENT_RTTI(VSCollapseRecordLine,VSCollapseRecord)
BEGIN_ADD_PROPERTY(VSCollapseRecordLine,VSCollapseRecord)
REGISTER_PROPERTY(m_RecordInfo,RecordInfo,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCollapseRecordLine)
IMPLEMENT_INITIAL_END
VSCollapseRecordLine::VSCollapseRecordLine()
{

	m_RecordInfo.Clear();
}
VSCollapseRecordLine::~VSCollapseRecordLine()
{
	m_RecordInfo.Clear();
}

