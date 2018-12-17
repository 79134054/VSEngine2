#ifndef VSPSHADER_H
#define VSPSHADER_H
#include "VSShader.h"
namespace VSEngine2
{
	class VSStream;
	class VSPShader : public VSShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSPShader();
		VSPShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSPShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		virtual ~VSPShader();

	public:


		

		static const VSPShader *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
		static VSPointer<VSPShader> Default;
	};
	DECLARE_Ptr(VSPShader);
	VSTYPE_MARCO(VSPShader);

}
#endif