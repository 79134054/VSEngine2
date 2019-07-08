#ifndef VSLIGHTACTOR_H
#define VSLIGHTACTOR_H
#include "VSActor.h"
#include "VSLight.h"
namespace VSEngine2
{

	class VSGRAPHIC_API VSLightActor : public VSActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		VSLightActor();
		virtual ~VSLightActor() = 0;

		virtual unsigned int GetLightType() = 0;

		GET_TYPE_NODE(VSLight)
		virtual void Update(double dAppTime);
		DECLARE_INITIAL_NO_CLASS_FACTORY
		VSArray<VSUsedName> m_ShadowSceneNameArray;
		virtual void AddToSceneMap(VSSceneMap * pSceneMap);
	};
	DECLARE_Ptr(VSLightActor);
	VSTYPE_MARCO(VSLightActor);

	class VSGRAPHIC_API VSDirectionLightActor : public VSLightActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		VSDirectionLightActor();
		virtual ~VSDirectionLightActor();
		DECLARE_INITIAL
		virtual unsigned int GetLightType()
		{
			return VSLight::LT_DIRECTION;
		}
		GET_TYPE_NODE(VSDirectionLight)
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(VSDirectionLightActor);
	VSTYPE_MARCO(VSDirectionLightActor);


	class VSGRAPHIC_API VSPointLightActor : public VSLightActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		VSPointLightActor();
		virtual ~VSPointLightActor();
		DECLARE_INITIAL
		virtual unsigned int GetLightType()
		{
			return VSLight::LT_POINT;
		}
		GET_TYPE_NODE(VSPointLight)
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(VSPointLightActor);
	VSTYPE_MARCO(VSPointLightActor);


	class VSGRAPHIC_API VSSpotLightActor : public VSLightActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		VSSpotLightActor();
		virtual ~VSSpotLightActor();
		DECLARE_INITIAL
		virtual unsigned int GetLightType()
		{
			return VSLight::LT_SPOT;
		}
		GET_TYPE_NODE(VSSpotLight)
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(VSSpotLightActor);
	VSTYPE_MARCO(VSSpotLightActor);

	class VSGRAPHIC_API VSSkyLightActor : public VSLightActor
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VSSkyLightActor();
		virtual ~VSSkyLightActor();
		DECLARE_INITIAL
		virtual unsigned int GetLightType()
		{
			return VSLight::LT_SKY;
		}
		GET_TYPE_NODE(VSSkyLight)
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(VSSkyLightActor);
	VSTYPE_MARCO(VSSkyLightActor);
}
#endif