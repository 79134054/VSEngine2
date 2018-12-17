#ifndef VSFILE_H
#define VSFILE_H
#include "VSSystem.h"
namespace VSEngine2
{
	class VSSYSTEM_API VSFile
	{
	public:
		enum //Open Mode
		{
			OM_RB,
			OM_WB,
			OM_RT,
			OM_WT,
			OM_MAX
		};
		enum 
		{
			VSMAX_PATH = 256
		};
		enum	//Seek Flag
		{
			SF_CUR,
			SF_END,
			SF_SET,
			SF_MAX

		};
		VSFile();
		~VSFile();
		bool Flush();

		bool Seek(unsigned int uiOffSet,unsigned int uiOrigin);
		bool Open(const TCHAR * pFileName,unsigned int uiOpenMode);
		bool Write(const void *pBuffer,unsigned int uiSize,unsigned int uiCount);
		bool Read(void *pBuffer,unsigned int uiSize,unsigned int uiCount);
		//这个函数返回pbuffer中，如果buffer大小大于取得的字符，则最后一个字符是回车，
		//倒数第2个是 LF    (Line Feed)
		bool GetLine(void * pBuffer,unsigned int uiSize);
		FORCEINLINE unsigned int GetFileSize()const
		{
			return m_uiFileSize;
		}
	protected:
		static TCHAR ms_cOpenMode[OM_MAX][5];
		static unsigned int m_uiSeekFlag[];
		FILE  *m_pFileHandle;
		unsigned int m_uiOpenMode;
		unsigned int m_uiFileSize;
		TCHAR m_tcFileName[VSMAX_PATH];
		
	};
}
#endif