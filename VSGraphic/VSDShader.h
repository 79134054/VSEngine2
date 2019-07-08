#ifndef VSDSHADER_H
#define VSDSHADER_H
#include "VSShader.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSDShader : public VSShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSDShader();
		VSDShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSDShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		virtual ~VSDShader();

	public:


		static const VSDShader *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
		static VSPointer<VSDShader> Default;
	};
	DECLARE_Ptr(VSDShader);
	VSTYPE_MARCO(VSDShader);
}
#endif