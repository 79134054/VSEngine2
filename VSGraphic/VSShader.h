#ifndef VSSHADER_H
#define VSSHADER_H
#include "VSObject.h"
#include "VSUserConstant.h"
#include "VSController.h"
#include "VSUserSampler.h"
#include "VSResourceManager.h"
#include "VSBind.h"
#include "VSResource.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSShader : public VSBind,public VSResource
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		VSShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		VSShader();
		virtual ~VSShader() = 0;
		virtual unsigned int GetResourceType()const
		{
			return RT_SHADER;
		}
	public:
		


		virtual bool SetParam(const VSUsedName &Name,void * pDate);
		virtual bool SetParam(const VSUsedName &Name,VSTexAllState * pTexture,unsigned int uiIndex = 0);
		virtual void ClearInfo();

		
		const VSString & GetBuffer()const{ return m_Buffer;} 
		FORCEINLINE const void *GetCacheBuffer()const
		{
			return m_pCacheBuffer;
		}
		FORCEINLINE unsigned int GetCacheBufferSize()const
		{
			return m_uiCacheBufferSize;
		}
		bool SetCacheBuffer(void * pBuffer,unsigned int uiSize);
		const VSString &GetMainFunName()const
		{
			return m_MainFunName;
		}
		unsigned int m_uiConstBufferSize;
	protected:
		VSString m_Buffer;
		unsigned char * m_pCacheBuffer;
		unsigned int m_uiCacheBufferSize;
		VSString	m_MainFunName;
		
	public:	
		void SetShaderString(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		void SetShaderString(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
	public:
		VSArray<VSUserConstantPtr>		m_pUserConstant;
		VSArray<VSUserSamplerPtr>		m_pUserSampler;
		VSShaderKey						m_ShaderKey;
		bool		m_bCreatePara;
		unsigned int m_uiArithmeticInstructionSlots;
		unsigned int m_uiTextureInstructionSlots;
	};
	DECLARE_Ptr(VSShader);
	VSTYPE_MARCO(VSShader);

}
#endif