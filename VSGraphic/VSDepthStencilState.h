#ifndef VSDEPTHSENTCILSTATE_H
#define VSDEPTHSENTCILSTATE_H
#include "VSBind.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSDepthStencilDesc : public VSObject
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //Compare Nethod
		{
			CM_NEVER,
			CM_LESS,
			CM_EQUAL,
			CM_LESSEQUAL,
			CM_GREATER,
			CM_NOTEQUAL,
			CM_GREATEREQUAL,
			CM_ALWAYS,
			CM_MAX

		};
		enum //OperationType
		{
			OT_KEEP,
			OT_ZERO,
			OT_REPLACE,
			OT_INCREMENT,
			OT_DECREMENT,
			OT_INVERT,
			OT_INCREMENT_STA,
			OT_DECREMENT_STA,
			OT_MAX
		};
		VSDepthStencilDesc()
		{
			m_bDepthEnable = true;
			m_bDepthWritable = true;
			m_uiDepthCompareMethod = CM_LESSEQUAL;


			m_bStencilEnable = false;             
			m_uiStencilCompareMethod = CM_ALWAYS;		
			m_uiReference = 0x0;   
			m_uiMask = 0xFF;       
			m_uiWriteMask = 0xFF;   
			m_uiSPassZPassOP = OT_KEEP;     
			m_uiSPassZFailOP = OT_KEEP;    
			m_uiSFailZPassOP = OT_KEEP;

			m_uiCCW_StencilCompareMethod = CM_ALWAYS;	
			m_uiCCW_SPassZPassOP = OT_KEEP;   
			m_uiCCW_SPassZFailOP = OT_KEEP;   
			m_uiCCW_SFailZPassOP = OT_KEEP;
			m_bTwoSideStencilMode = false;
		}
		~VSDepthStencilDesc()
		{

		}


		bool m_bDepthEnable;
		bool m_bDepthWritable;
		unsigned char m_uiDepthCompareMethod;
		

		bool m_bStencilEnable;             
		unsigned char m_uiStencilCompareMethod;		
		unsigned char m_uiReference;   
		unsigned char m_uiMask;       
		unsigned char m_uiWriteMask;   
		unsigned char m_uiSPassZPassOP;     
		unsigned char m_uiSPassZFailOP;    
		unsigned char m_uiSFailZPassOP;

		unsigned char m_uiCCW_StencilCompareMethod;	
		unsigned char m_uiCCW_SPassZPassOP;     
		unsigned char m_uiCCW_SPassZFailOP;    
		unsigned char m_uiCCW_SFailZPassOP;
		bool m_bTwoSideStencilMode;

		void * GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(VSDepthStencilDesc) - sizeof(VSObject);
			return (void *)&m_bDepthEnable;
		}
	};
	VSTYPE_MARCO(VSDepthStencilDesc);
	class VSGRAPHIC_API VSDepthStencilState : public VSBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:	
		virtual ~VSDepthStencilState();
	protected:
		VSDepthStencilState();
		VSDepthStencilDesc m_DepthStencilDesc;
	protected:
		static VSPointer<VSDepthStencilState> Default;
	public:
		static const VSDepthStencilState *GetDefault()
		{
			return Default;
		}

		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		FORCEINLINE const VSDepthStencilDesc& GetDepthStencilDesc()const
		{
			return m_DepthStencilDesc;
		}
		friend class VSResourceManager;
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
	};
	DECLARE_Ptr(VSDepthStencilState);
	VSTYPE_MARCO(VSDepthStencilState);
}
#endif