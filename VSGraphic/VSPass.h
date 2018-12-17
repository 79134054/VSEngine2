#ifndef VSPASS_H
#define VSPASS_H
#include "VSObject.h"
#include "VSShaderMap.h"
#include "VSShaderKey.h"
#include "VSPShader.h"
#include "VSVShader.h"
#include "VSGShader.h"
#include "VSDShader.h"
#include "VSHShader.h"
#include "VSMatrix3X3W.h"
namespace VSEngine2
{
	class VSSpatial;
	class VSCamera;
	class VSGRAPHIC_API VSPass : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
		enum //pass type
		{
			PT_MATERIAL,
			PT_NORMALDEPTH,
			PT_PREZ,
			PT_POINT_CUBE_SHADOW,
			PT_POINT_VOLUME_SHADOW,
			PT_DIRECT_VOLUME_SHADOW,
			PT_SHADOW,
			PT_DUAL_PARABOLOID_SHADOW,
			PT_LIGHT_FUNCTION,
			PT_INDIRECT,
			PT_POSTEFFECT,
			PT_MAX
		};
	public:
		virtual ~VSPass() = 0;
	protected:
		VSPass();
		VSSpatial *						m_pSpatial;
		VSCamera * 						m_pCamera;
		unsigned int					m_uiPassId;
		VSShaderSet * m_pVShaderSet;
		VSShaderKey m_VShaderkey;

		VSShaderSet * m_pPShaderSet;
		VSShaderKey m_PShaderkey;

		VSShaderSet * m_pGShaderSet;
		VSShaderKey m_GShaderkey;

		VSMaterialInstance * m_pMaterialInstance;
		MaterialShaderPara MSPara;
	public:
		FORCEINLINE void SetMaterialInstance(VSMaterialInstance * pMaterialInstance)
		{
			m_pMaterialInstance = pMaterialInstance;
		}
		FORCEINLINE void SetSpatial(VSSpatial * pSpatial)
		{
			//VSMAC_ASSERT(pSpatial);
			m_pSpatial = pSpatial;
		}
		FORCEINLINE void SetCamera(VSCamera * pCamera)
		{
			//VSMAC_ASSERT(pCamera);
			m_pCamera = pCamera;
		}
		FORCEINLINE void SetPassId(unsigned int uiPassId)
		{
			m_uiPassId = uiPassId;
		}
		virtual bool Draw(VSRenderer * pRenderer) = 0;
		virtual unsigned int GetPassType() = 0;

		bool GetPShader(MaterialShaderPara& MSPara,VSShaderMap & ShaderMap,const VSUsedName &Name);
		bool GetVShader(MaterialShaderPara& MSPara,VSShaderMap & ShaderMap,const VSUsedName &Name);
		bool GetGShader(MaterialShaderPara& MSPara, VSShaderMap & ShaderMap, const VSUsedName &Name);
	};
	DECLARE_Ptr(VSPass);
	VSTYPE_MARCO(VSPass);
}
#endif