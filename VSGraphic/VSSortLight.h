#ifndef VSSORTLIGHT_H
#define VSSORTLIGHT_H
#include "VSPointLight.h"
#include "VSDirectionLight.h"
namespace VSEngine2
{
	class LightPriority
	{
	public:
		LightPriority(){}
		~LightPriority(){}
		bool operator()(VSLight *pLight1,VSLight * pLight2)
		{
			if (pLight1->GetLightType() == pLight2->GetLightType())
			{
				VSLocalLight * pLocalLight1 = DynamicCast<VSLocalLight>(pLight1);
				if (pLocalLight1)
				{
					VSLocalLight * pLocalLight2 = DynamicCast<VSLocalLight>(pLight2);

					if (pLocalLight1->GetCastShadow() == pLocalLight2->GetCastShadow())
					{
						if (pLocalLight1->GetCastShadow())
						{
							if (pLocalLight1->GetLightType() == VSLight::LT_POINT)
							{
								if (((VSPointLight *)pLocalLight1)->GetShadowType() > ((VSPointLight *)pLocalLight2)->GetShadowType())
								{
									return 0;
								}
								else
								{
									if (((VSPointLight *)pLocalLight1)->GetShadowType() == ((VSPointLight *)pLocalLight2)->GetShadowType())
									{
										if (pLocalLight1->HaveLightFun() > pLocalLight2->HaveLightFun())
										{
											return 0;
										}
										else
										{
											return 1;
										}
									}
									else
									{
										return 1;
									}
									
								}
							}
							else if (pLocalLight1->GetLightType() == VSLight::LT_DIRECTION)
							{
								if (((VSDirectionLight *)pLocalLight1)->GetShadowType() > ((VSDirectionLight *)pLocalLight2)->GetShadowType())
								{
									return 0;
								}
								else
								{
									if (((VSDirectionLight *)pLocalLight1)->GetShadowType() == ((VSDirectionLight *)pLocalLight2)->GetShadowType())
									{
										if (pLocalLight1->HaveLightFun() > pLocalLight2->HaveLightFun())
										{
											return 0;
										}
										else
										{
											return 1;
										}
									}
									else
									{
										return 1;
									}
									
								}
							}
							else
							{
								if (pLocalLight1->HaveLightFun() > pLocalLight2->HaveLightFun())
								{
									return 0;
								}
								else
								{
									return 1;
								}
							}
							
						}
						else
						{
							if (pLocalLight1->HaveLightFun() > pLocalLight2->HaveLightFun())
							{
								return 0;
							}
							else
							{
								return 1;
							}
						}
					}
					else
					{
						if(pLocalLight1->GetCastShadow() < pLocalLight2->GetCastShadow())
						{
							return 1;
						}
						else
						{
							return 0;
						}
					}
				}
				else
				{
					return 1;
				}
			}
			else
			{
				if(pLight1->GetLightType() < pLight2->GetLightType())
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}


	};
	

}

#endif