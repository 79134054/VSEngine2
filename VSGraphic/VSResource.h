#ifndef VSRESOURCE_H
#define VSRESOURCE_H
#include "VSName.h"
#include "VSObject.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSResource
	{
	public:
		enum	//Resource Type
		{
			RT_TEXTURE,
			RT_SKELECTON_MODEL,
			RT_STATIC_MODEL,
			RT_ACTION,
			RT_MATERIAL,
			RT_POSTEFFECT,
			RT_SHADER,
			RT_ANIMTREE,
			RT_MORPHTREE,
			RT_TERRAIN,	
			RT_FSM,
			RT_ACTOR,
			RT_MAP,
			RT_FONTTYPE,
			RT_MAX
		};
		VSResource();
		virtual ~VSResource() = 0;
		FORCEINLINE const VSUsedName & GetResourceName()const
		{
			return m_ResourceName;
		}

		FORCEINLINE void SetResourceName(const VSUsedName & ResourceName)
		{
			m_ResourceName = ResourceName;
		}
		virtual unsigned int GetResourceType()const = 0;
		static const VSString &GetFileSuffix(unsigned int uiFileSuffix); 
	protected:
		VSUsedName	m_ResourceName;
		static VSString ms_FileSuffix[]; 
	};
	class VSGRAPHIC_API VSResourceProxyBase : public VSReference , public VSMemObject
	{
	public:
		VSResourceProxyBase();
		virtual ~VSResourceProxyBase() = 0;
		virtual bool IsEndableASYNLoad() = 0;
		virtual bool IsEndableGC() = 0;
		virtual bool IsLoaded() = 0;
		virtual const VSUsedName & GetResourceName() = 0;
		virtual unsigned int GetResourceType() = 0;
		void LoadEvent()
		{
			for (unsigned int i = 0 ; i < m_LoadedEventObject.GetNum() ; i++)
			{
				m_LoadedEventObject[i].Key->LoadedEvent(this, m_LoadedEventObject[i].Value);
			}
			m_LoadedEventObject.Destroy();
		}
		void AddLoadEventObject(VSObject * pObject,int Data = 0)
		{
			if (!pObject)
			{
				return ;
			}
			if (m_bIsLoaded == false)
			{
				m_LoadedEventObject.AddElement(pObject,Data);	
			}
			else
			{
				pObject->LoadedEvent(this);
			}
		}
	protected:
		VSMap<VSObjectPtr,int> m_LoadedEventObject;
		bool	m_bIsLoaded;
		VSUsedName	m_ResourceName;
		void Loaded()
		{
			m_bIsLoaded = true;
			LoadEvent();
		}
		void SetResourceName(const VSUsedName & ResourceName)
		{
			m_ResourceName = ResourceName;
		}
	};
	DECLARE_Ptr(VSResourceProxyBase);
	template<class T>
	class VSResourceProxy : public VSResourceProxyBase
	{
	public:
		VSResourceProxy();
		virtual ~VSResourceProxy();
		T * GetResource();
		virtual const VSUsedName & GetResourceName();
		friend class VSTextureRJob;
		friend class VSMaterialRJob;
		friend class VSAnimRJob;
		friend class VSRefPosRJob;
		friend class VSStaticMeshRJob;
		friend class VSSkelectonMeshRJob;
		friend class VSResourceManager;
		friend class VSVShaderRJob;
		friend class VSPShaderRJob;
		friend class VSAnimTreeRJob;
		friend class VSMorphTreeRJob;
		friend class VSActorRJob;
		friend class VSSceneMapRJob;
		friend class VSFontRJob;
		virtual bool IsLoaded();
		virtual bool IsEndableASYNLoad();
		virtual bool IsEndableGC();
		virtual unsigned int GetResourceType();
		void SetNewResource(VSPointer<T> pResource);
		static VSResourceProxy<T> * Create( T * pResource);
	protected:
		VSPointer<T> m_pResource;
		VSPointer<T> m_pPreResource;
		
		
	};
	template<class T>
	VSResourceProxy<T> * VSResourceProxy<T>::Create( T * pResource)
	{
		VSResourceProxy<T> * pProxy = VS_NEW VSResourceProxy<T>();
		pProxy->SetResourceName(pResource->GetResourceName());
		pProxy->SetNewResource(pResource);
		pProxy->Loaded();
		return pProxy;
	}
	template<class T>
	VSResourceProxy<T>::VSResourceProxy()
	{
		m_bIsLoaded = false;
		m_pPreResource = (T *)T::GetDefalut();
		m_pResource = NULL;
	}
	template<class T>
	VSResourceProxy<T>::~VSResourceProxy()
	{
		m_pPreResource = NULL;
		m_pResource = NULL;
	}
	template<class T>
	void VSResourceProxy<T>::SetNewResource(VSPointer<T> pResource)
	{
		m_pResource = pResource;
		m_pResource->SetResourceName(m_ResourceName);
	}
	template<class T>
	bool VSResourceProxy<T>::IsEndableASYNLoad()
	{
		return T::ms_bIsEnableASYNLoader;
	}
	template<class T>
	bool VSResourceProxy<T>::IsEndableGC()
	{
		return T::ms_bIsEnableGC;
	}
	template<class T>
	bool VSResourceProxy<T>::IsLoaded()
	{
		return m_bIsLoaded;
	}
	template<class T>
	T * VSResourceProxy<T>::GetResource()
	{
		if (m_bIsLoaded )
		{
			return m_pResource;
		}
		else
		{
			return m_pPreResource;
		}
	}
	template<class T>
	unsigned int VSResourceProxy<T>::GetResourceType()
	{
		return GetResource()->GetResourceType();
	}

	template<class T>
	const VSUsedName & VSResourceProxy<T>::GetResourceName()
	{
		return m_ResourceName;
	}
#define DECLARE_Proxy(ClassName)	\
	typedef VSResourceProxy<ClassName> ##ClassName##R;\
	typedef VSPointer<VSResourceProxy<ClassName>> ##ClassName##RPtr;

	
}
#endif