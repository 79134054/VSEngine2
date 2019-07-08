#include "VSDataBuffer.h"
#include "VSMath.h"
#include "VSVector3.h"
#include "VSVector2.h"
#include "VSVector3W.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSDataBuffer,VSObject)
BEGIN_ADD_PROPERTY(VSDataBuffer,VSObject)
REGISTER_PROPERTY(m_uiNum,Num,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiDT,DataType,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY_DATA(m_pDate,m_uiSize,Date)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDataBuffer)
IMPLEMENT_INITIAL_END
unsigned int VSDataBuffer::ms_uiDateTypeByte[DT_MAXNUM] =
{
	4,
	8,
	12,
	16,

	2,
	4,

	4,
	4,

	4,
	4,
	4,

	8,
	8,
	8,

	4,
	8,

	4
};
unsigned int VSDataBuffer::ms_uiDateTypeChannel[DT_MAXNUM] =
{
	1,
	2,
	3,
	4,

	1,
	1,

	4,
	4,
	
	2,
	2,
	2,

	4,
	4,
	4,

	2,
	4,

	4

};
VSDataBuffer::VSDataBuffer()
{
	m_uiDT = DT_MAXNUM;

	m_uiNum = 0;
	m_pDate = NULL;

}
VSDataBuffer::~VSDataBuffer()
{
	VSMAC_DELETEA(m_pDate);
}

bool VSDataBuffer::SetDate(const void *pDate,unsigned int uiNum,unsigned int uiDT)
{
	if(uiDT >= DT_MAXNUM || !pDate || !uiNum)
		return 0;

	m_uiDT = uiDT;
	m_uiNum = uiNum;

	VSMAC_DELETEA(m_pDate);

	m_pDate = VS_NEW unsigned char[GetSize()];

	if(!m_pDate)
		return 0;
	VSMemcpy(m_pDate,pDate,GetSize());
	m_uiSize = GetSize();
	return 1;
}
bool VSDataBuffer::AddDate(const void *pDate,unsigned int uiNum,unsigned int uiDT)
{
	if(uiDT >= DT_MAXNUM || !pDate || !uiNum)
		return 0;
	if(m_uiDT != VSMAX_INTEGER && m_uiDT != uiDT)
		return 0;


	unsigned char * Temp = NULL;
	Temp = VS_NEW unsigned char[ ( uiNum + m_uiNum )* GetStride()];
	if(!Temp)
		return 0;
	VSMemcpy(Temp,m_pDate,GetStride() * m_uiNum);
	VSMemcpy(Temp + GetStride() * m_uiNum,pDate,GetStride() * uiNum);

	VSMAC_DELETEA(m_pDate);
	m_uiNum += uiNum;
	m_pDate = Temp;
	m_uiSize = GetSize();
	return 1;
}
bool VSDataBuffer::CreateEmptyBuffer(unsigned int uiNum,unsigned int uiDT)
{
	if(uiDT >= DT_MAXNUM || !uiNum)
		return 0;

	m_uiDT = uiDT;
	m_uiNum = uiNum;
	VSMAC_DELETEA(m_pDate);

	m_pDate = VS_NEW unsigned char[GetSize()];

	if(!m_pDate)
		return 0;
	VSMemset(m_pDate,0,GetSize());
	m_uiSize = GetSize();
	return 1;
}



