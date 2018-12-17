#ifndef VSPOSTEFFECTPASS_H
#define VSPOSTEFFECTPASS_H
#include "VSPass.h"
namespace VSEngine2
{

	class VSGRAPHIC_API VSPostEffectPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSPostEffectPass();
		VSPostEffectPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		void SetCustomConstant(MaterialShaderPara &MSPara, VSVShader * pVShader, VSPShader * pPShader);
	public:
		virtual bool Draw(VSRenderer * pRenderer);

		virtual unsigned int GetPassType()	
		{
			return PT_POSTEFFECT;
		}
		unsigned int m_uiRTWidth;
		unsigned int m_uiRTHeight;
		VSTexAllState * m_PColorBuffer;
		VSRenderState m_RenderState;
	};
	DECLARE_Ptr(VSPostEffectPass);
	VSTYPE_MARCO(VSPostEffectPass);
}
#endif