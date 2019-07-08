#ifndef VSSHADOWPASS_H
#define VSSHADOWPASS_H
#include "VSPass.h"
namespace VSEngine2
{
	class VSPointLight;
	class VSGRAPHIC_API VSCubeShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSCubeShadowPass();
		VSCubeShadowPass();
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
			return PT_POINT_CUBE_SHADOW;
		}
		VSPointLight * m_pPointLight;

	};

	class VSGRAPHIC_API VSVolumeShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSVolumeShadowPass();
		VSVolumeShadowPass();

	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSRenderState m_RenderState;
		void SetCustomConstant(MaterialShaderPara &MSPara, VSGShader * pGShader);
	public:
		virtual bool Draw(VSRenderer * pRenderer);
		virtual unsigned int GetPassType()
		{
			if (m_pLocalLight->GetLightType() == VSLight::LT_POINT)
			{
				return PT_POINT_VOLUME_SHADOW;
			} 
			else
			{
				return PT_DIRECT_VOLUME_SHADOW;
			}
			
		}
		VSLocalLight * m_pLocalLight;

	};
	class VSGRAPHIC_API VSShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSShadowPass();
		VSShadowPass();
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
			return PT_SHADOW;
		}
		VSLocalLight * m_pLocalLight;
	}; 

	class VSGRAPHIC_API VSDualParaboloidShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSDualParaboloidShadowPass();
		VSDualParaboloidShadowPass();
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
			return PT_DUAL_PARABOLOID_SHADOW;
		}
		VSPointLight * m_pPointLight;
	}; 

}
#endif