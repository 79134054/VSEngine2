#include "VSLog.h"
using namespace VSEngine2;
/********************************************************************************/
VSLog::VSLog()
{


}
/********************************************************************************/
VSLog::~VSLog()
{

}
/********************************************************************************/
bool VSLog::Open(const TCHAR * pFileName)
{
	return VSFile::Open(pFileName,OM_WT);
}
/********************************************************************************/
bool VSLog::WriteInfo(const TCHAR *pcString, ...)const
{

	char *pArgs;

	pArgs = (char*) &pcString + sizeof(pcString);
	_vstprintf_s(VSSystem::ms_sLogBuffer, LOG_BUFFER_SIZE,pcString, pArgs) ;
	_ftprintf(m_pFileHandle, VSSystem::ms_sLogBuffer);
	return 1;
}
/********************************************************************************/