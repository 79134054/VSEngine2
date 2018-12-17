#include "VSDx11Renderer.h"
#include "VS2DTexSampler.h"
#include "VSGeometry.h"
#include "VSString.h"
#include "VSResourceManager.h"
#include "VSShaderStringFactory.h"
#include "VSDirectionLight.h"
#include "VSPointLight.h"
#include "VSSpotLight.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
void VSDX11Renderer::SetUserConstantWorldViewProjectMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSMatrix3X3W WorldViewProjectMat;
	VSTransform World = MSPara.pGeometry->GetWorldTransform();
	WorldViewProjectMat = World.GetCombine()* MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix();
	VSMatrix3X3W  * TempMatrix  = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = WorldViewProjectMat;
	ID++;
}
void VSDX11Renderer::SetUserLightShadowMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(MSPara.pShadowLight);
	if (pLocalLight)
	{
		VSTransform World = MSPara.pGeometry->GetWorldTransform();
		VSMatrix3X3W  * TempMatrix  = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
		*TempMatrix = World.GetCombine() * pLocalLight->m_LightShadowMatrix;
		ID++;
	}
	else
	{
		VSMAC_ASSERT(0);
	}
}
void VSDX11Renderer::SetUserConstantWorldViewMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSMatrix3X3W WorldViewMat;

	VSTransform World = MSPara.pGeometry->GetWorldTransform();

	WorldViewMat = World.GetCombine()* MSPara.pCamera->GetViewMatrix();

	VSMatrix3X3W  * TempMatrix  = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = WorldViewMat;
	ID++;
}
void VSDX11Renderer::SetUserConstantSkin(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	if(MSPara.pGeometry->GetAffectBoneNum() && pVBuffer->GetBlendWeightDate() && pVBuffer->GetBlendIndicesDate())
	{
		VSVector3W  * TempVector  = (VSVector3W  *)pShader->m_pUserConstant[ID]->GetDate();
		VSArray<VSVector3W> & Buffer = MSPara.pGeometry->GetSkinWeightBuffer();
		VSMemcpy(TempVector, Buffer.GetBuffer(),Buffer.GetNum() * sizeof(VSVector3W));		
		ID++;
	}
}
void VSDX11Renderer::SetUserConstantLightWorldPos(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSVector3 * pLightWorldPos = (VSVector3 *) pShader->m_pUserConstant[ID]->GetDate();
	*pLightWorldPos= MSPara.pShadowLight->GetWorldTranslate();
	ID++;
}
void VSDX11Renderer::SetUserConstantLightWorldDirection(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSVector3 * pLightWorldDirection = (VSVector3 *) pShader->m_pUserConstant[ID]->GetDate();

	const VSMatrix3X3 &Rotator = MSPara.pShadowLight->GetWorldRotate();
	VSVector3 U,V,N;
	Rotator.GetUVN(U,V,N);

	*pLightWorldDirection= N;
	ID++;
}
void VSDX11Renderer::SetMaterialVShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,VSVShader *pVShader)
{
	unsigned int ID = 0;

	if (uiPassType == VSPass::PT_MATERIAL 
		|| uiPassType == VSPass::PT_PREZ 
		|| uiPassType == VSPass::PT_NORMALDEPTH 
		|| uiPassType == VSPass::PT_INDIRECT)
	{	
		SetUserConstantWorldViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
	}
	else if (uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{

		SetUserConstantWorldViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
	}
	else if (uiPassType == VSPass::PT_POINT_VOLUME_SHADOW)
	{
		SetUserConstantWorldMatrix(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);

	}
	else if (uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		SetUserConstantWorldMatrix(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);

	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		SetUserLightShadowMatrix(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		SetUserConstantWorldViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantPointLightRange(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
	}
	if (uiPassType == VSPass::PT_NORMALDEPTH)
	{
		SetUserConstantWorldViewMatrix(MSPara,pVShader,ID);
		
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION)
	{
		//SetUserConstantInvRTWidth(MSPara,pVShader,ID);
	}

}
void VSDX11Renderer::SetUserConstantWorldMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSTransform World = MSPara.pGeometry->GetWorldTransform();
	VSMatrix3X3W  *TempMatrix  = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = World.GetCombine();
	ID++;
}
void VSDX11Renderer::SetUserConstantCameraPos(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSVector3 * pCameraPos = (VSVector3 *) pShader->m_pUserConstant[ID]->GetDate();
	*pCameraPos= MSPara.pCamera->GetWorldTranslate();
	ID++;
}
void VSDX11Renderer::SetUserConstantSkyUpColor(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSColorRGBA * pColor = (VSColorRGBA *) pShader->m_pUserConstant[ID]->GetDate();
	*pColor= MSPara.m_SkyLightUpColor;
	ID++;
}
void VSDX11Renderer::SetUserConstantSkyDownColor(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSColorRGBA * pColor = (VSColorRGBA *) pShader->m_pUserConstant[ID]->GetDate();
	*pColor= MSPara.m_SkyLightDownColor;
	ID++;	
}
void VSDX11Renderer::SetUserConstantLight(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSArray<VSLight*> & LightArray = MSPara.LightArray;

	int iLightNum[VSLight::LT_MAX] = { 0 };	
	VSVector3W * pLightBuffer[VSLight::LT_MAX] = { NULL };
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{

					iLightNum[j]++;
				}
			}
		}
	}

	for(unsigned int i = 0 ; i < VSLight::LT_MAX ; i++)
	{
		if (!iLightNum[i])
		{
			continue;
		}
		pLightBuffer[i] = (VSVector3W *) pShader->m_pUserConstant[ID]->GetDate();
		ID++;
	}
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			VSLight* pLight = LightArray[i];
			if (LightArray[i]->GetLightType() == VSLight::LT_DIRECTION)
			{

				*pLightBuffer[VSLight::LT_DIRECTION] = ((VSDirectionLight *)pLight)->m_Diffuse;
				pLightBuffer[VSLight::LT_DIRECTION]++;
				*pLightBuffer[VSLight::LT_DIRECTION] = ((VSDirectionLight *)pLight)->m_Specular;
				pLightBuffer[VSLight::LT_DIRECTION]++;
				const VSMatrix3X3 &Rotator = ((VSDirectionLight *)pLight)->GetWorldRotate();
				VSVector3 U,V,N;
				Rotator.GetUVN(U,V,N);

				pLightBuffer[VSLight::LT_DIRECTION]->x = N.x;
				pLightBuffer[VSLight::LT_DIRECTION]->y = N.y;
				pLightBuffer[VSLight::LT_DIRECTION]->z = N.z;
				pLightBuffer[VSLight::LT_DIRECTION]->w = ((VSDirectionLight *)pLight)->GetShadowResolution() * 1.0f;
				pLightBuffer[VSLight::LT_DIRECTION]++;

				pLightBuffer[VSLight::LT_DIRECTION]->x = ((VSDirectionLight *)pLight)->m_ZBias;

				pLightBuffer[VSLight::LT_DIRECTION]++;

				pLightBuffer[VSLight::LT_DIRECTION]->x = ((VSDirectionLight *)pLight)->m_LightFunScale.x;
				pLightBuffer[VSLight::LT_DIRECTION]->y = ((VSDirectionLight *)pLight)->m_LightFunScale.y;
				pLightBuffer[VSLight::LT_DIRECTION]->z = ((VSDirectionLight *)pLight)->m_LightFunOffset.x;
				pLightBuffer[VSLight::LT_DIRECTION]->w = ((VSDirectionLight *)pLight)->m_LightFunOffset.y;
				pLightBuffer[VSLight::LT_DIRECTION]++;

				VSMatrix3X3W MVP = ((VSLocalLight *)pLight)->GetMVP();
				VSVector3W MVPRow[4];
				MVP.GetRowVector(MVPRow[0],MVPRow[1],MVPRow[2],MVPRow[3]);
				*pLightBuffer[VSLight::LT_DIRECTION] = MVPRow[0];
				pLightBuffer[VSLight::LT_DIRECTION]++;
				*pLightBuffer[VSLight::LT_DIRECTION] = MVPRow[1];
				pLightBuffer[VSLight::LT_DIRECTION]++;
				*pLightBuffer[VSLight::LT_DIRECTION] = MVPRow[2];
				pLightBuffer[VSLight::LT_DIRECTION]++;
				*pLightBuffer[VSLight::LT_DIRECTION] = MVPRow[3];
				pLightBuffer[VSLight::LT_DIRECTION]++;
				if (((VSDirectionLight *)pLight)->GetShadowType() == VSDirectionLight::ST_VOLUME)
				{
					unsigned int uiRTWidth = m_uiCurRTWidth;
					unsigned int uiRTHeight = m_uiCurRTHeight;
					VSREAL fOffsetX = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSREAL fOffsetY = 0.5f + (0.5f / (VSREAL)(uiRTHeight));
					VSMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
						0.0f,    -0.5f,     0.0f,       0.0f,
						0.0f,     0.0f,     1.0f,		0.0f,
						fOffsetX, fOffsetY, 0.0f,       1.0f );
					VSMatrix3X3W Mat = MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix() * texScaleBiasMat;

					VSVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[0];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[1];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[2];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[3];
					pLightBuffer[VSLight::LT_DIRECTION]++;

					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;

					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
				}
				else if (((VSDirectionLight *)pLight)->GetShadowType() == VSDirectionLight::ST_CSM)
				{
					//shadow
					unsigned int uiRTWidth = ((VSDirectionLight *)pLight)->GetShadowResolution();

					VSREAL fOffsetX = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSREAL fOffsetY = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
						0.0f,    -0.5f,     0.0f,       0.0f,
						0.0f,     0.0f,     1.0f,		0.0f,
						fOffsetX, fOffsetY, 0.0f,       1.0f );
					VSMatrix3X3W Mat = ((VSDirectionLight *)pLight)->m_CSMLightShadowMatrix[0] * texScaleBiasMat;

					VSVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[0];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[1];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[2];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[3];
					pLightBuffer[VSLight::LT_DIRECTION]++;

					Mat = ((VSDirectionLight *)pLight)->m_CSMLightShadowMatrix[1] * texScaleBiasMat;

					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[0];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[1];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[2];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[3];
					pLightBuffer[VSLight::LT_DIRECTION]++;

					Mat = ((VSDirectionLight *)pLight)->m_CSMLightShadowMatrix[2] * texScaleBiasMat;

					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[0];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[1];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[2];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[3];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					
				}
				else
				{
					//shadow
					unsigned int uiRTWidth = ((VSDirectionLight *)pLight)->GetShadowResolution();

					VSREAL fOffsetX = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSREAL fOffsetY = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
						0.0f,    -0.5f,     0.0f,       0.0f,
						0.0f,     0.0f,     1.0f,		0.0f,
						fOffsetX, fOffsetY, 0.0f,       1.0f );
					VSMatrix3X3W Mat = ((VSDirectionLight *)pLight)->m_LightShadowMatrix * texScaleBiasMat;

					VSVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[0];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[1];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[2];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[3];
					pLightBuffer[VSLight::LT_DIRECTION]++;

					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;

					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;	
				}
			}
			else if (LightArray[i]->GetLightType() == VSLight::LT_POINT)
			{
				*pLightBuffer[VSLight::LT_POINT] = ((VSPointLight *)pLight)->m_Diffuse;
				pLightBuffer[VSLight::LT_POINT]++;
				*pLightBuffer[VSLight::LT_POINT] = ((VSPointLight *)pLight)->m_Specular;
				pLightBuffer[VSLight::LT_POINT]++;
				pLightBuffer[VSLight::LT_POINT]->x = ((VSPointLight *)pLight)->GetWorldTranslate().x;
				pLightBuffer[VSLight::LT_POINT]->y = ((VSPointLight *)pLight)->GetWorldTranslate().y;
				pLightBuffer[VSLight::LT_POINT]->z = ((VSPointLight *)pLight)->GetWorldTranslate().z;
				pLightBuffer[VSLight::LT_POINT]->w = ((VSPointLight *)pLight)->GetRange();
				pLightBuffer[VSLight::LT_POINT]++;
				pLightBuffer[VSLight::LT_POINT]->x = ((VSPointLight *)pLight)->GetShadowResolution() * 1.0f;
				pLightBuffer[VSLight::LT_POINT]->y = ((VSPointLight *)pLight)->m_ZBias;
				pLightBuffer[VSLight::LT_POINT]++;

				pLightBuffer[VSLight::LT_POINT]->x = ((VSPointLight *)pLight)->m_LightFunScale.x;
				pLightBuffer[VSLight::LT_POINT]->y = ((VSPointLight *)pLight)->m_LightFunScale.y;
				pLightBuffer[VSLight::LT_POINT]->z = ((VSPointLight *)pLight)->m_LightFunOffset.x;
				pLightBuffer[VSLight::LT_POINT]->w = ((VSPointLight *)pLight)->m_LightFunOffset.y;
				pLightBuffer[VSLight::LT_POINT]++;

				VSMatrix3X3W MVP = ((VSLocalLight *)pLight)->GetMVP();
				VSVector3W MVPRow[4];
				MVP.GetRowVector(MVPRow[0],MVPRow[1],MVPRow[2],MVPRow[3]);
				*pLightBuffer[VSLight::LT_POINT] = MVPRow[0];
				pLightBuffer[VSLight::LT_POINT]++;
				*pLightBuffer[VSLight::LT_POINT] = MVPRow[1];
				pLightBuffer[VSLight::LT_POINT]++;
				*pLightBuffer[VSLight::LT_POINT] = MVPRow[2];
				pLightBuffer[VSLight::LT_POINT]++;
				*pLightBuffer[VSLight::LT_POINT] = MVPRow[3];
				pLightBuffer[VSLight::LT_POINT]++;

				if (((VSPointLight *)pLight)->GetShadowType() == VSPointLight::ST_VOLUME)
				{

					unsigned int uiRTWidth = m_uiCurRTWidth;
					unsigned int uiRTHeight = m_uiCurRTHeight;
					VSREAL fOffsetX = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSREAL fOffsetY = 0.5f + (0.5f / (VSREAL)(uiRTHeight));
					VSMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
													0.0f,    -0.5f,     0.0f,       0.0f,
													0.0f,     0.0f,     1.0f,		0.0f,
													fOffsetX, fOffsetY, 0.0f,       1.0f );
					VSMatrix3X3W Mat = MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix() * texScaleBiasMat;

					VSVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_POINT] = Row[0];
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = Row[1];
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = Row[2];
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = Row[3];
					pLightBuffer[VSLight::LT_POINT]++;


				}
				else
				{
					//shadow
					*pLightBuffer[VSLight::LT_POINT] = VSVector3W();
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = VSVector3W();
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = VSVector3W();
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = VSVector3W();
					pLightBuffer[VSLight::LT_POINT]++;
				}
				
			}
			else if (LightArray[i]->GetLightType() == VSLight::LT_SPOT)
			{
				*pLightBuffer[VSLight::LT_SPOT] = ((VSSpotLight *)pLight)->m_Diffuse;
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = ((VSSpotLight *)pLight)->m_Specular;
				pLightBuffer[VSLight::LT_SPOT]++;
				pLightBuffer[VSLight::LT_SPOT]->x = ((VSSpotLight *)pLight)->GetWorldTranslate().x;
				pLightBuffer[VSLight::LT_SPOT]->y = ((VSSpotLight *)pLight)->GetWorldTranslate().y;
				pLightBuffer[VSLight::LT_SPOT]->z = ((VSSpotLight *)pLight)->GetWorldTranslate().z;
				pLightBuffer[VSLight::LT_SPOT]->w = ((VSSpotLight *)pLight)->GetRange();
				pLightBuffer[VSLight::LT_SPOT]++;
				const VSMatrix3X3 &Rotator = ((VSSpotLight *)pLight)->GetWorldRotate();
				VSVector3 U,V,N;
				Rotator.GetUVN(U,V,N);
				pLightBuffer[VSLight::LT_SPOT]->x = N.x;
				pLightBuffer[VSLight::LT_SPOT]->y = N.y;
				pLightBuffer[VSLight::LT_SPOT]->z = N.z;
				pLightBuffer[VSLight::LT_SPOT]->w = ((VSDirectionLight *)pLight)->GetShadowResolution() * 1.0f;
				pLightBuffer[VSLight::LT_SPOT]++;

				pLightBuffer[VSLight::LT_SPOT]->x = ((VSSpotLight *)pLight)->GetFalloff();
				pLightBuffer[VSLight::LT_SPOT]->y = ((VSSpotLight *)pLight)->GetTheta();
				pLightBuffer[VSLight::LT_SPOT]->z = ((VSSpotLight *)pLight)->GetPhi();
				pLightBuffer[VSLight::LT_SPOT]->w = ((VSSpotLight *)pLight)->m_ZBias;
				pLightBuffer[VSLight::LT_SPOT]++;

				pLightBuffer[VSLight::LT_SPOT]->x = ((VSSpotLight *)pLight)->m_LightFunScale.x;
				pLightBuffer[VSLight::LT_SPOT]->y = ((VSSpotLight *)pLight)->m_LightFunScale.y;
				pLightBuffer[VSLight::LT_SPOT]->z = ((VSSpotLight *)pLight)->m_LightFunOffset.x;
				pLightBuffer[VSLight::LT_SPOT]->w = ((VSSpotLight *)pLight)->m_LightFunOffset.y;
				pLightBuffer[VSLight::LT_SPOT]++;

				VSMatrix3X3W MVP = ((VSLocalLight *)pLight)->GetMVP();
				VSVector3W MVPRow[4];
				MVP.GetRowVector(MVPRow[0],MVPRow[1],MVPRow[2],MVPRow[3]);
				*pLightBuffer[VSLight::LT_SPOT] = MVPRow[0];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = MVPRow[1];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = MVPRow[2];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = MVPRow[3];
				pLightBuffer[VSLight::LT_SPOT]++;


				//shadow
				unsigned int uiRTWidth = ((VSSpotLight *)pLight)->GetShadowResolution();

				VSREAL fOffsetX = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
				VSREAL fOffsetY = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
				VSMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
					0.0f,    -0.5f,     0.0f,       0.0f,
					0.0f,     0.0f,     1.0f,		0.0f,
					fOffsetX, fOffsetY, 0.0f,       1.0f );
				VSMatrix3X3W Mat = ((VSSpotLight *)pLight)->m_LightShadowMatrix * texScaleBiasMat;

				VSVector3W Row[4];
				Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
				*pLightBuffer[VSLight::LT_SPOT] = Row[0];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = Row[1];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = Row[2];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = Row[3];
				pLightBuffer[VSLight::LT_SPOT]++;

			}
		}
	}
}
void VSDX11Renderer::SetUserConstantLightFuntionSampler(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSArray<VSLight*> & LightArray = MSPara.LightArray;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->HaveLightFun())
		{

			pShader->m_pUserSampler[ID]->SetTex(pLocalLight->GetLightFunDiffuseTexture(),0);
			ID++;

		}
	}
}
void VSDX11Renderer::SetUserConstantShadowSampler(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSArray<VSLight*> & LightArray = MSPara.LightArray;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->GetCastShadow())
		{
			for (unsigned int j = 0 ; j < pLocalLight->GetShadowTextureNum() ; j++)
			{
				pShader->m_pUserSampler[ID]->SetTex(pLocalLight->GetShadowTexture(j),j);
				
			}
			ID++;
			
		}
	}
}
void VSDX11Renderer::SetUserConstantFarZ(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSREAL  * Temp  = (VSREAL *)pShader->m_pUserConstant[ID]->GetDate();
	*Temp = MSPara.pCamera->GetZFar();	
	ID++;
}
void VSDX11Renderer::SetUserConstantPointLightRange(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSREAL  * Temp  = (VSREAL *)pShader->m_pUserConstant[ID]->GetDate();
	VSPointLight * pPointLight = DynamicCast<VSPointLight>(MSPara.pShadowLight);
	VSMAC_ASSERT(pPointLight);
	*Temp = pPointLight->GetRange();
	ID++;
}
void VSDX11Renderer::SetUserConstantInvRTWidth(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSREAL  * Temp  = (VSREAL *)pShader->m_pUserConstant[ID]->GetDate();
	VSLocalLight * pLight = DynamicCast<VSLocalLight>(MSPara.pShadowLight);
	VSMAC_ASSERT(pLight);
	*Temp = 1.0f / pLight->GetLightMaterialRtWidth();	
	ID++;
}
void VSDX11Renderer::SetUserConstantProjectMatrix(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID)
{
	VSMatrix3X3W  *TempMatrix = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = MSPara.pCamera->GetProjMatrix();
	ID++;
}
void VSDX11Renderer::SetUserConstantViewMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSMatrix3X3W  *TempMatrix  = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = MSPara.pCamera->GetViewMatrix();
	ID++;
}
void VSDX11Renderer::SetUserConstantViewProjectMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSMatrix3X3W  *TempMatrix  = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix();
	ID++;
}
void VSDX11Renderer::SetMaterialPShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,VSPShader *pPShader)
{
	unsigned int ID = 0;
	if (uiPassType == VSPass::PT_MATERIAL)
	{

		SetUserConstantWorldMatrix(MSPara,pPShader,ID);
		SetUserConstantViewMatrix(MSPara,pPShader,ID);
		SetUserConstantProjectMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara,pPShader,ID);
		SetUserConstantFarZ(MSPara,pPShader,ID);
		if (MSPara.LightArray.GetNum() > 0)
		{
			SetUserConstantLight(MSPara,pPShader,ID);	
		}
		
		unsigned int uiTexSamplerID = 0;
		SetUserConstantShadowSampler(MSPara,pPShader,uiTexSamplerID);	
		SetUserConstantLightFuntionSampler(MSPara,pPShader,uiTexSamplerID);
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);

	}
	else if (uiPassType == VSPass::PT_INDIRECT)
	{
		SetUserConstantWorldMatrix(MSPara,pPShader,ID);
		SetUserConstantViewMatrix(MSPara,pPShader,ID);
		SetUserConstantProjectMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara,pPShader,ID);
		SetUserConstantSkyUpColor(MSPara,pPShader,ID);
		SetUserConstantSkyDownColor(MSPara,pPShader,ID);

		unsigned int uiTexSamplerID = 0;
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);
	}
	else if (uiPassType == VSPass::PT_NORMALDEPTH)
	{
		SetUserConstantWorldMatrix(MSPara, pPShader, ID);
		SetUserConstantViewMatrix(MSPara, pPShader, ID);
		SetUserConstantProjectMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara, pPShader, ID);
		SetUserConstantFarZ(MSPara,pPShader,ID);
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);
	}
	else if (uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{
		SetUserConstantWorldMatrix(MSPara,pPShader,ID);
		SetUserConstantCameraPos(MSPara,pPShader,ID);

		SetUserConstantPointLightRange(MSPara,pPShader,ID);
		
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);
	}
	else if (uiPassType == VSPass::PT_PREZ 
		|| uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW
		|| uiPassType == VSPass::PT_POINT_VOLUME_SHADOW)
	{
		
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		SetUserLightShadowMatrix(MSPara, pPShader, ID);
		SetUserConstantWorldMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara, pPShader, ID);
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION
		|| uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW 
		|| uiPassType == VSPass::PT_POSTEFFECT)
	{
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);
	}
	
}
