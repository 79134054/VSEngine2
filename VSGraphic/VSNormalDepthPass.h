#ifndef VSNORMALDEPTHPASS_H
#define VSNORMALDEPTHPASS_H
#include "VSPass.h"
namespace VSEngine2
{

	class VSGRAPHIC_API VSNormalDepthPass : public VSPass
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSNormalDepthPass();
		VSNormalDepthPass();
	public:

		
	protected:
		
		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSRenderState m_RenderState;
	public:
		virtual bool Draw(VSRenderer * pRenderer);

		virtual unsigned int GetPassType()
		{
			return PT_NORMALDEPTH;
		}

	};
	DECLARE_Ptr(VSNormalDepthPass);
	VSTYPE_MARCO(VSNormalDepthPass);
}
#endif