#ifndef VSVSHADER_H
#define VSVSHADER_H
#include "VSShader.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSVShader : public VSShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSVShader();
		VSVShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSVShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		virtual ~VSVShader();

	public:


		static const VSVShader *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
		static VSPointer<VSVShader> Default;
	};
	DECLARE_Ptr(VSVShader);
	VSTYPE_MARCO(VSVShader);
}
#endif