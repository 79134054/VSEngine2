#ifndef VSENUMTRANS_H
#define VSENUMTRANS_H
#include "VSString.h"
#include "VSMap.h"
#include "VSArray.h"
#include "VSGraphic.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSEnumTrans
	{
	public:
		VSEnumTrans();
		~VSEnumTrans();
		static VSEnumTrans& GetInstance();
		void Add(const TCHAR * EnumName,const TCHAR * Value);
		void Get(const TCHAR * EnumName, VSArray<VSString>& AS);
	protected:
		VSMap<VSString,VSArray<VSString>> m_EnumSaver;
	};
}
#define ADD_ENUM(EnumName,Value) \
	VSEnumTrans::GetInstance().Add(_T(#EnumName),_T(#Value));
#define GET_ENUMARRAY(EnumName,Array) \
	VSEnumTrans::GetInstance().Get(_T(#EnumName),Array);
#endif
