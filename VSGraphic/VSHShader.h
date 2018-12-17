#ifndef VSHSHADER_H
#define VSHSHADER_H
#include "VSShader.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSHShader : public VSShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSHShader();
		VSHShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSHShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		virtual ~VSHShader();

	public:


		static const VSHShader *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
		static VSPointer<VSHShader> Default;
	};
	DECLARE_Ptr(VSHShader);
	VSTYPE_MARCO(VSHShader);
}
#endif