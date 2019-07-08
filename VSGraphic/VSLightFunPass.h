#ifndef VSLIGHTFUNPASS_H
#define VSLIGHTFUNPASS_H
#include "VSPass.h"
namespace VSEngine2
{

	class VSGRAPHIC_API VSLightFunPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSLightFunPass();
		VSLightFunPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

	public:
		virtual bool Draw(VSRenderer * pRenderer);

		virtual unsigned int GetPassType()
		{
			return PT_LIGHT_FUNCTION;
		}
		VSLight * m_pLight;
		VSRenderState m_RenderState;
	};
	DECLARE_Ptr(VSLightFunPass);
	VSTYPE_MARCO(VSLightFunPass);
}
#endif