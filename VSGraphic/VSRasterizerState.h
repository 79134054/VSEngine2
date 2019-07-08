#ifndef VSRASTERIZERSTATE_H
#define VSRASTERIZERSTATE_H
#include "VSBind.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSRasterizerDesc : public VSObject
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //Cull Type
		{
			CT_NONE,
			CT_CW,
			CT_CCW,
			CT_MAX

		};
		VSRasterizerDesc()
		{
			m_bWireEnable = false;
			m_uiCullType = CT_CCW;	
			m_bClipPlaneEnable = false;
			m_bScissorTestEnable = false;
			m_fDepthBias = 0.0f;

		}
		~VSRasterizerDesc()
		{

		}

		
		bool m_bWireEnable;
		unsigned char m_uiCullType;	
		bool m_bClipPlaneEnable;
		bool m_bScissorTestEnable;
		VSREAL	m_fDepthBias;
		void * GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(VSRasterizerDesc)-sizeof(VSObject);
			return (void *)&m_bWireEnable;
		}
	};
	VSTYPE_MARCO(VSRasterizerDesc);
	class VSGRAPHIC_API VSRasterizerState : public VSBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:	
		virtual ~VSRasterizerState();
	protected:
		VSRasterizerState();
		VSRasterizerDesc m_RasterizerDesc;
	protected:
		static VSPointer<VSRasterizerState> Default;
		static VSPointer<VSRasterizerState> ScissorEnable;
	public:
		static const VSRasterizerState *GetDefault()
		{
			return Default;
		}

		static const VSRasterizerState *GetScissorEnable()
		{
			return ScissorEnable;
		}
		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		FORCEINLINE const VSRasterizerDesc & GetRasterizerDesc()const
		{
			return m_RasterizerDesc;
		}
		friend class VSResourceManager;
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
	};
	DECLARE_Ptr(VSRasterizerState);
	VSTYPE_MARCO(VSRasterizerState);
}
#endif