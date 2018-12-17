#ifndef VSSTRING_H
#define VSSTRING_H
#include "VSContainer.h"
#include "VSMath.h"
namespace VSEngine2
{
class VSDATESTRUCT_API VSString : public VSContainer<TCHAR>
{
public:
	enum
	{
		BUFFER_SIZE = 2048
	};
	VSString();
	VSString(const TCHAR * String);
	VSString(const VSString &String);
	~VSString();
	static VSString ms_StringNULL;
	static TCHAR TempBuffer[BUFFER_SIZE];
	FORCEINLINE TCHAR * GetBuffer()const
	{
		return m_pBuffer;
	}
	FORCEINLINE unsigned int GetLength()const
	{
		return (unsigned int)VSStrLen(m_pBuffer);
	}
	void SetTCHARBufferNum(unsigned int uiLength);
	void Clear();
	//重载=操作符
	VSString & operator =(const VSString &String);
	VSString & operator =(const TCHAR *String);

	//重载[]操作符
	TCHAR &operator[](unsigned int i);

	//重载+=操作符
	const VSString & operator +=(const VSString &String);
	const VSString & operator +=(const TCHAR *String);

	//取得指定下标的前段字符串或者后段字符串
	bool GetString(const VSString & String,unsigned int uiFind,bool bIsFront = true, bool bIsHaveFind = false);
	//查找第iFindNum次出现的指定字符，然后取前段字符串或者后段字符串。iFIndNum = -1表示最后一次出现的。
	bool GetString(const VSString & String,TCHAR cFind,int iFIndNum,bool bIsFront = true, bool bIsHaveFind = false);
	//查找当前字符串中第iFindNum次出现的String的下标，返回-1则表示查找失败
	int GetSubStringIndex(const VSString &String, int iFindNum)const;
	//去掉指定的字符
	bool StripChars(const VSString &String,TCHAR * pStripChars);
	//替换指定的字符
	bool ReplaceChars(const VSString &String ,TCHAR * pReplaceChars,TCHAR UseChar);
	//去掉字符串的左边所有空格
	void StringLtrim(const VSString &String);
	//去掉字符串的右边所有空格
	void StringRtrim(const VSString &String);

	void Format(const TCHAR * pcString, ...);
protected:
	TCHAR * m_pBuffer;

};
	//重载+操作符
	VSDATESTRUCT_API VSString operator +(const VSString & String1, const VSString &String2);
	VSDATESTRUCT_API VSString operator +(const VSString &String1, const TCHAR *String2);
	VSDATESTRUCT_API VSString operator +(const TCHAR *String1, const VSString &String2);

	//大于返回大于0，小于返回小于0，等于返回0 
	VSDATESTRUCT_API int CompareString(const VSString &String1,const VSString &String2);
	VSDATESTRUCT_API int CompareString(const VSString &String1,const TCHAR *String2);
	VSDATESTRUCT_API int CompareString(const TCHAR *String1,const VSString &String2);

	//重载==操作符
	VSDATESTRUCT_API bool operator ==(const VSString &String1,const VSString &String2);
	VSDATESTRUCT_API bool operator ==(const VSString &String1,const TCHAR *String2);
	VSDATESTRUCT_API bool operator ==(const TCHAR *String1,VSString &String2);
	//重载!=操作符
	VSDATESTRUCT_API bool operator !=(const VSString &String1,const VSString &String2);
	VSDATESTRUCT_API bool operator !=(const VSString &String1,const TCHAR *String2);
	VSDATESTRUCT_API bool operator !=(const TCHAR *String1,const VSString &String2);

	VSDATESTRUCT_API int StringToInt(const VSString & String);
	VSDATESTRUCT_API VSString IntToString(int i);

	VSDATESTRUCT_API unsigned int StringToUInt(const VSString & String);
	VSDATESTRUCT_API VSString UIntToString(unsigned int i);

	VSDATESTRUCT_API bool StringToBool(const VSString & String);
	VSDATESTRUCT_API VSString BoolToString(bool i);
	VSDATESTRUCT_API VSString BoolToString(unsigned int i);

	VSDATESTRUCT_API VSREAL StringToReal(const VSString & String);
	VSDATESTRUCT_API VSString RealToString(VSREAL Value);


	class VSDATESTRUCT_API VSFileName : public VSString
	{
	public:
		VSFileName()
		{

		}
		VSFileName(const TCHAR * String):VSString(String)
		{
		}
		VSFileName(const VSString &String):VSString(String)
		{

		}
		~VSFileName()
		{

		}
		bool GetExtension(VSString & Extension);
		bool GetBaseName(VSString & BaseName);
		bool GetBaseNameAndExtension(VSString & BaseNameAndExtension);
		bool GetPath(VSString & Path);
		bool GetPathAndName(VSString & PathAndName);
	};
}
#endif