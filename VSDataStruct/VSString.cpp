#include "VSString.h"
using namespace VSEngine2;
VSString VSString::ms_StringNULL;
TCHAR	VSString::TempBuffer[BUFFER_SIZE] = {0};
VSString::VSString()
{
	m_pBuffer = NULL;
	m_pBuffer= VS_NEW TCHAR[1];
	VSMAC_ASSERT(m_pBuffer);
	m_pBuffer[0] =_T('\0');
}
VSString::VSString(const TCHAR * String)
{
	VSMAC_ASSERT(String);
	m_pBuffer = NULL;
	if(String)
	{
		unsigned int  uiLength = (unsigned int)VSStrLen(String);
		m_pBuffer= VS_NEW TCHAR[uiLength + 1];
		VSMAC_ASSERT(m_pBuffer);

		VSStrCopy(m_pBuffer,uiLength + 1,String);
	}
	else
	{
		m_pBuffer= VS_NEW TCHAR[1];
		VSMAC_ASSERT(m_pBuffer);
		m_pBuffer[0] =_T('\0');
	}
}
VSString::VSString(const VSString &String)
{
	m_pBuffer = NULL;
	unsigned int  uiLength = String.GetLength();
	m_pBuffer= VS_NEW TCHAR[uiLength + 1];
	VSMAC_ASSERT(m_pBuffer);

	VSStrCopy(m_pBuffer,uiLength + 1,String.GetBuffer());
}
VSString::~VSString()
{

	VSMAC_DELETEA(m_pBuffer);

}
void VSString::SetTCHARBufferNum(unsigned int uiLength)
{
	VSMAC_DELETEA(m_pBuffer);
	m_pBuffer= VS_NEW TCHAR[uiLength + 1];
	m_pBuffer[uiLength] =_T('\0');
}
void VSString::Clear()
{

	VSMAC_DELETEA(m_pBuffer);
	m_pBuffer= VS_NEW TCHAR[1];
	VSMAC_ASSERT(m_pBuffer);
	m_pBuffer[0] =_T('\0');
}
VSString & VSString::operator =(const VSString &String)
{


	VSMAC_DELETEA(m_pBuffer);
	unsigned int  uiLength = String.GetLength();
	m_pBuffer= VS_NEW TCHAR[uiLength + 1];
	VSMAC_ASSERT(m_pBuffer);

	VSStrCopy(m_pBuffer,uiLength + 1,String.GetBuffer());
	return *this;
}
VSString & VSString::operator =(const TCHAR *String)
{
	VSMAC_ASSERT(String);
	VSMAC_DELETEA(m_pBuffer);
	if(String)
	{
		unsigned int  uiLength = (unsigned int)VSStrLen(String);
		m_pBuffer= VS_NEW TCHAR[uiLength + 1];
		VSMAC_ASSERT(m_pBuffer);

		VSStrCopy(m_pBuffer,uiLength + 1,String);
	}
	else
	{
		VSString();
	}
	return *this;
}

TCHAR &VSString::operator[](unsigned int i)
{
	return m_pBuffer[i];
}

const VSString & VSString::operator +=(const VSString &String)
{
	unsigned int uiLength1 = GetLength();
	unsigned int uiLength2 = String.GetLength();
	if(!uiLength1)
	{
		*this = String;
		return *this;
	}
	else if(!uiLength2)
		return *this;
	else
	{
		TCHAR *pBuffer = NULL;
		unsigned int uiLength = uiLength2 + uiLength1 + 1;
		pBuffer = VS_NEW TCHAR[uiLength];
		VSMAC_ASSERT(pBuffer);

		VSStrCopy(pBuffer,uiLength,GetBuffer());
		VSStrcat(pBuffer,uiLength,String.GetBuffer());

		VSMAC_DELETEA(m_pBuffer);
		m_pBuffer = pBuffer;
		return *this;

	}
}
const VSString & VSString::operator +=(const TCHAR *String)
{
	unsigned int uiLength1 = GetLength();
	unsigned int uiLength2 = 0;
	if (String)
	{
		uiLength2 = (unsigned int)VSStrLen(String);
	}
	if(!uiLength1)
	{
		*this = String;
		return *this;
	}
	else if(!uiLength2)
		return *this;
	else
	{
		TCHAR *pBuffer = NULL;
		unsigned int uiLength = uiLength2 + uiLength1 + 1;
		pBuffer = VS_NEW TCHAR[uiLength];
		VSMAC_ASSERT(pBuffer);

		VSStrCopy(pBuffer,uiLength,GetBuffer());
		VSStrcat(pBuffer,uiLength,String);

		VSMAC_DELETEA(m_pBuffer);
		m_pBuffer = pBuffer;
		return *this;

	}
}
bool VSString::GetString(const VSString & String,unsigned int uiFind,bool bIsFront, bool bIsHaveFind)
{
	if (uiFind >= String.GetLength())
	{
		return false;
	}
	
	unsigned int uiLength = 0;
	unsigned int uiIndex = 0;
	if ( bIsFront )
	{
		uiLength = uiFind;
		uiIndex = 0;
	}
	else
	{
		uiLength = String.GetLength() - uiFind - 1;
		uiIndex = uiFind + 1;
	}
	if (bIsHaveFind)
	{
		uiLength++;
		if ( !bIsFront )
		{
			uiIndex--;
		}
	}
	if (uiIndex >= String.GetLength())
	{
		return false;
	}
	VSMAC_DELETEA(m_pBuffer);
	m_pBuffer = VS_NEW TCHAR[uiLength + 1];
	VSMemcpy(m_pBuffer,&String.m_pBuffer[uiIndex],uiLength,uiLength + 1);
	m_pBuffer[uiLength] = _T('\0');
	return true;

}
bool VSString::GetString(const VSString & String,TCHAR cFind,int iFIndNum,bool bIsFront, bool bIsHaveFind)
{
	if (iFIndNum == 0)
	{
		return false;
	}
	unsigned int uiFIndNumTemp = 0;
	int uiIndex = -1;
	for (unsigned int i = 0 ; i < String.GetLength() ; i++)
	{
		if (cFind == String.m_pBuffer[i])
		{
			uiFIndNumTemp++;
			uiIndex = i;
		}
		if (uiFIndNumTemp == iFIndNum)
		{		
			break;
		}
	}
	if (uiIndex == -1)
	{
		return false;
	}
	return GetString(String,uiIndex,bIsFront,bIsHaveFind);

}
int VSString::GetSubStringIndex(const VSString &String, int iFindNum)const
{
	if (GetLength() < String.GetLength())
	{
		return -1;
	}
	unsigned int uiFindNumTemp = 0;
	int iFindIndex = -1;
	for (unsigned int Index = 0; Index < GetLength() ; Index++)
	{
		unsigned int j = 0;
		for (unsigned int i = Index ; i < String.GetLength() + Index ; i++)
		{
			if (m_pBuffer[i] == String.m_pBuffer[j])
			{
				j++;
				continue;
			}
			else
			{
				break;
			}
		}
		if (j == String.GetLength())
		{
			iFindIndex = Index;
			uiFindNumTemp++;
			if (uiFindNumTemp == iFindNum)
			{
				
				return iFindIndex;
			}
		}
	}

	return iFindIndex;

}
bool VSString::StripChars(const VSString &String,TCHAR * pStripChars)
{
	if (String.GetLength() > BUFFER_SIZE || !pStripChars)
	{
		return false;
	}
	
	unsigned int strip_chars_len = VSStrLen(pStripChars);
	TCHAR *_string_in = String.GetBuffer() , *_string_out = TempBuffer;
	while(*_string_in != _T('\0'))
	{
		int flag=0;
		for(unsigned int i = 0 ; i < strip_chars_len ; i++)
		{
			if(*_string_in == pStripChars[i])
			{
				flag=1;
				break;
			}
		}
		if(flag == 0)
		{
			*_string_out=*_string_in;
			_string_out++;


		}
		_string_in++;

	}
	*_string_out=_T('\0');
	*this = TempBuffer;
	return true;

}

bool VSString::ReplaceChars(const VSString &String ,TCHAR * pReplaceChars,TCHAR UseChar)
{
	if (String.GetLength() > BUFFER_SIZE || !pReplaceChars)
	{
		return false;
	}
	unsigned int replace_chars_len=VSStrLen(pReplaceChars);
	TCHAR *_string_in = String.GetBuffer(),*_string_out = TempBuffer;
	while(*_string_in != _T('\0'))
	{
		int flag=0;
		for(unsigned int i = 0 ; i < replace_chars_len ; i++)
		{
			if(*_string_in==pReplaceChars[i])
			{
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			*_string_out = *_string_in;

		}
		else
		{
			*_string_out = UseChar;

		}
		_string_out++;
		_string_in++;

	}
	*_string_out=_T('\0');

	*this = TempBuffer;
	return true;
}

void VSString::StringLtrim(const VSString &String)
{
	int index=0;
	TCHAR * pString = String.GetBuffer();
	while(_istspace(pString[index]))
		index++;
	if(index==0)
		return;
	else
	{
		unsigned int uiLength=String.GetLength();
		VSMemcpy((void *)TempBuffer, (void *)(pString + index),  ((uiLength - index) + 1) * sizeof(TCHAR) );
		*this = TempBuffer;
	}
}

void VSString::StringRtrim(const VSString &String)
{
	unsigned int uiLength=String.GetLength();
	if (!uiLength)
	{
		return ;
	}
	unsigned int index = uiLength - 1;
	TCHAR * pString = String.GetBuffer();
	while(_istspace(pString[index]))
		index--;

	VSMemcpy((void *)TempBuffer, (void *)(pString),  (index + 1) * sizeof(TCHAR));
	index++;
	TempBuffer[index]=_T('\0');
	*this = TempBuffer;
}
void VSString::Format(const TCHAR * pcString, ...)
{
	static TCHAR LogBuffer[LOG_BUFFER_SIZE];
	char *pArgs;
	pArgs = (char*) &pcString + sizeof(pcString);
	VSSprintf(LogBuffer, LOG_BUFFER_SIZE,pcString, pArgs);
	*this = LogBuffer;
}
bool  VSFileName::GetExtension(VSString & Extension)
{
	return Extension.GetString(*this,_T('.'),-1,false,false);
}
bool VSFileName::GetPathAndName(VSString & PathAndName)
{
	return PathAndName.GetString(*this,_T('.'),-1);
}
bool VSFileName::GetBaseName(VSString & BaseName)
{
	bool bResult = false;
	VSString BaseNameAndExtension;
	bResult = GetBaseNameAndExtension(BaseNameAndExtension);
	if (!bResult)
	{
		return false;
	}
	bResult = BaseName.GetString(BaseNameAndExtension,_T('.'),-1);
	return bResult;
}
bool VSFileName::GetBaseNameAndExtension(VSString & BaseNameAndExtension)
{
	bool bResult = false;
	bResult = BaseNameAndExtension.GetString(*this,_T('\\'),-1,false,false);
	if (!bResult)
	{
		bResult = BaseNameAndExtension.GetString(*this,_T('/'),-1,false,false);
	}
	return bResult;
}
bool VSFileName::GetPath(VSString & Path)
{
	bool bResult = false;
	bResult = Path.GetString(*this,_T('\\'),-1,true,false);
	if (!bResult)
	{
		bResult = Path.GetString(*this,_T('/'),-1,true,false);
	}
	return bResult;
}
namespace VSEngine2
{
VSString operator +(const VSString & String1, const VSString &String2)
{
	unsigned int uiLength1 = String1.GetLength();
	unsigned int uiLength2 = String2.GetLength();
	if (!uiLength2)
		return String1;
	else if(!uiLength1)
		return String2; 
	else
	{
		TCHAR *pBuffer = NULL;
		unsigned int uiLength = uiLength2 + uiLength1 + 1;
		pBuffer = VS_NEW TCHAR[uiLength];
		VSMAC_ASSERT(pBuffer);

		VSStrCopy(pBuffer,uiLength,String1.GetBuffer());
		VSStrcat(pBuffer,uiLength,String2.GetBuffer());
		VSString ReturnValue(pBuffer);
		VSMAC_DELETEA(pBuffer);
		return ReturnValue;
	}
}
VSString operator +(const VSString &String1, const TCHAR *String2)
{
	VSMAC_ASSERT(String2);
	unsigned int uiLength1 = String1.GetLength();
	unsigned int uiLength2 = 0;
	if (String2)
	{
		uiLength2 = (unsigned int)VSStrLen(String2);
	}
	
	if (!uiLength2)
		return String1;
	else if(!uiLength1)
		return VSString(String2); 
	else
	{
		TCHAR *pBuffer = NULL;
		unsigned int uiLength = uiLength2 + uiLength1 + 1;
		pBuffer = VS_NEW TCHAR[uiLength];
		VSMAC_ASSERT(pBuffer);

		VSStrCopy(pBuffer,uiLength,String1.GetBuffer());
		VSStrcat(pBuffer,uiLength,String2);
		VSString ReturnValue(pBuffer);
		VSMAC_DELETEA(pBuffer);
		return ReturnValue;
	}
}
VSString operator +(const TCHAR *String1, const VSString &String2)
{
	VSMAC_ASSERT(String1);
	unsigned int uiLength2 = String2.GetLength();
	unsigned int uiLength1 = 0;
	if (String1)
	{
		uiLength1 = (unsigned int)VSStrLen(String1);
	}

	if (!uiLength1)
		return String2;
	else if(!uiLength2)
		return VSString(String1); 
	else
	{
		TCHAR *pBuffer = NULL;
		unsigned int uiLength = uiLength2 + uiLength1 + 1;
		pBuffer = VS_NEW TCHAR[uiLength];
		VSMAC_ASSERT(pBuffer);

		VSStrCopy(pBuffer,uiLength,String1);
		VSStrcat(pBuffer,uiLength,String2.GetBuffer());
		VSString ReturnValue(pBuffer);
		VSMAC_DELETEA(pBuffer);
		return ReturnValue;
	}
}
int CompareString(const VSString &String1,const VSString &String2)
{
	return VSStrCmp(String1.GetBuffer(),String2.GetBuffer());

}
int CompareString(const VSString &String1,const TCHAR *String2)
{
	return VSStrCmp(String1.GetBuffer(),String2);
}
int CompareString(const TCHAR *String1,const VSString &String2)
{
	return VSStrCmp(String1,String2.GetBuffer());
}



bool operator ==(const VSString &String1,const VSString &String2)
{
	return (VSStrCmp(String1.GetBuffer(),String2.GetBuffer()) == 0);
}
bool operator ==(const VSString &String1,const TCHAR *String2)
{
	return (VSStrCmp(String1.GetBuffer(),String2) == 0);
}
bool operator ==(const TCHAR *String1,VSString &String2)
{
	return (VSStrCmp(String1,String2.GetBuffer()) == 0);
}

bool operator !=(const VSString &String1,const VSString &String2)
{
	return (VSStrCmp(String1.GetBuffer(),String2.GetBuffer()) != 0);
}
bool operator !=(const VSString &String1,const TCHAR *String2)
{
	return (VSStrCmp(String1.GetBuffer(),String2) != 0);	
}
bool operator !=(const TCHAR *String1,const VSString &String2)
{
	return (VSStrCmp(String1,String2.GetBuffer()) != 0);
}
int StringToInt(const VSString & String)
{
	int i = 0 ;
	VSScanf(String.GetBuffer(), "%d", (TCHAR *)&i);
	return i;
}
VSString IntToString(int Value)
{
	VSSprintf(VSString::TempBuffer,2048,_T("%d"),Value);
	return VSString(VSString::TempBuffer);
}
unsigned int StringToUInt(const VSString & String)
{
	unsigned int i = 0;
	VSScanf(String.GetBuffer(), "%d", (TCHAR *)&i);
	return i;
}
VSString UIntToString(unsigned int Value)
{
	VSSprintf(VSString::TempBuffer, 2048, _T("%d"), Value);
	return VSString(VSString::TempBuffer);
}
VSREAL StringToReal(const VSString & String)
{
	VSREAL f = 0.0f;
	_stscanf_s(String.GetBuffer(),"%f",&f);
	//VSScanf(String.GetBuffer(), "%d", (TCHAR *)&f);
	return f;
}
VSString RealToString(VSREAL Value)
{
	VSSprintf(VSString::TempBuffer,2048,_T("%f"),Value);
	return VSString(VSString::TempBuffer);

}
bool StringToBool(const VSString & String)
{
	if (String == _T("True") || String == _T("true") || String == _T("TRUE"))
	{
		return true;
	}
	else if (String == _T("False") || String == _T("false") || String == _T("FALSE"))
	{
		return false;
	}

	int d = 0;
	_stscanf_s(String.GetBuffer(),"%d",&d);
	if (d)
	{
		return true;
	}
	else
		return false;
}
VSString BoolToString(bool i)
{
	if (i)
	{
		return _T("True");
	}
	else
	{
		return _T("False");
	}
}
VSString BoolToString(unsigned int i)
{
	if (i)
	{
		return _T("True");
	}
	else
	{
		return _T("False");
	}
}
}