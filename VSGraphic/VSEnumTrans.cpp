#include "VSEnumTrans.h"
using namespace VSEngine2;
VSEnumTrans& VSEnumTrans::GetInstance()
{
	static VSEnumTrans EnumTrans;
	return EnumTrans;
}
VSEnumTrans::VSEnumTrans()
{

}
VSEnumTrans::~VSEnumTrans()
{

}
void VSEnumTrans::Add(const TCHAR * EnumName,const TCHAR * Value)
{
	unsigned int i = m_EnumSaver.Find(EnumName);
	if (i == m_EnumSaver.GetNum())
	{
		MapElement<VSString,VSArray<VSString>> Element;
		Element.Key = EnumName;
		Element.Value.AddElement(Value);
		m_EnumSaver.AddElement(Element);
		return;
	}
	MapElement<VSString,VSArray<VSString>> &Element = m_EnumSaver[i];
	Element.Value.AddElement(Value);
}
void VSEnumTrans::Get(const TCHAR * EnumName, VSArray<VSString>& AS)
{
	unsigned int i = m_EnumSaver.Find(EnumName);
	if (i == m_EnumSaver.GetNum())
	{
		return ;
	}
	AS = m_EnumSaver[i].Value;
}