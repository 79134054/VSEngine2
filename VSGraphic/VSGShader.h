#ifndef VSGSHADER_H
#define VSGSHADER_H
#include "VSShader.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSGShader : public VSShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSGShader();
		VSGShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSGShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		virtual ~VSGShader();

	public:


		static const VSGShader *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
		static VSPointer<VSGShader> Default;
	};
	DECLARE_Ptr(VSGShader);
	VSTYPE_MARCO(VSGShader);
}
#endif