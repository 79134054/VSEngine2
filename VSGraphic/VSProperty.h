#ifndef VSPROPERTY_H
#define VSPROPERTY_H
#include "VSRtti.h"
#include "VSSystem.h"
#include "VSArray.h"
#include "VSMap.h"
#include "VSProperty.marc"
#include "VSName.h"
#include "VSType.h"
#include "VSStream.h"
#include "VSEditorProperty.h"
#include "VSEnumTrans.h"
namespace VSEngine2
{
	


	template < class T > FORCEINLINE bool operator < (const T& v1, const T& v2)
	{
		return false;
	}
	template < class T > FORCEINLINE bool operator > (const T& v1, const T& v2)
	{
		return false;
	}
	class VSFunction;
	// Value Point Resource Struct Enum
	class VSProperty
	{
	public:
		enum	//Property Type
		{
			PT_VALUE,
			PT_ENUM,
			PT_DATA,
			PT_ARRAY,
			PT_MAP,
			PT_MAX
		};
		enum	// Flag 
		{
			F_NONE = 0X00,
			F_SAVE_LOAD = 0X01,
			F_CLONE = 0X02,
			F_COPY = 0X04,
			F_SAVE_LOAD_CLONE = 0X03,
			F_SAVE_LOAD_COPY = 0X05,
			F_REFLECT_NAME = 0X08,
			F_NO_USE_GC = 0X10,
			F_MAX
		};
		VSProperty()
		{
			m_pRttiOwner = NULL;
			m_pFuntionOwner = NULL;
			m_uiFlag = 0;
			m_uiElementOffset = 0;
		}
		VSProperty(VSRtti & Owner,const VSUsedName & Name,unsigned int uiElementOffset,unsigned int uiFlag)
			:m_pRttiOwner(&Owner)
		{
			m_Name = Name;
			m_uiElementOffset = uiElementOffset;
			m_uiFlag = uiFlag;
			m_pFuntionOwner = NULL;
		}
		VSProperty(VSFunction & Owner, const VSUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag)
			:m_pFuntionOwner(&Owner)
		{
			m_Name = Name;
			m_uiElementOffset = uiElementOffset;
			m_uiFlag = uiFlag;
			m_pRttiOwner = NULL;
		}
		virtual ~VSProperty()
		{

		}
		virtual unsigned int GetPropertyType()const = 0;
		FORCEINLINE VSRtti * GetRtti()const
		{
			return m_pRttiOwner;
		}
		FORCEINLINE VSFunction * GetFunction()const
		{
			return m_pFuntionOwner;
		}
		FORCEINLINE const VSUsedName & GetName()const
		{
			return m_Name;
		}
		virtual void* GetValueAddress(void* pObj)const
		{
			return (void*)(((unsigned char*)pObj) + m_uiElementOffset);
		}
		virtual bool Archive(VSStream &Stream,void* pObj) = 0;
		virtual VSProperty * GetInstance() = 0;
		virtual void CloneData(void* pSrcObj,void* pDestObj,VSMap<VSObject *,VSObject*>& CloneMap) = 0;
		virtual void CopyData(void* pSrcObj,void* pDestObj) = 0;
		virtual void AddEditorElement(void * pSrcObj,VSECollection * pParent,VSString &Name) = 0;
		virtual bool Clone(VSProperty * p)
		{
			if ((p->GetRtti() == GetRtti() && GetRtti()) || (p->GetFunction() == GetFunction() && GetFunction()))
			{
				m_Name = p->m_Name;
				m_uiFlag = p->m_uiFlag;
				m_uiElementOffset = p->m_uiElementOffset;
				return true;		
			}
			
			return false;
		}
		void SetOwner(VSRtti & Owner)
		{
			m_pRttiOwner = &Owner;
		}
		void SetOwner(VSFunction & Function)
		{
			m_pFuntionOwner = &Function;
		}
		unsigned int GetFlag()const
		{
			return m_uiFlag;
		}
		void SetFlag(unsigned int uiFlag)
		{
			m_uiFlag = uiFlag;
		}
	protected:
		VSRtti * m_pRttiOwner;
		VSFunction * m_pFuntionOwner;
		VSUsedName m_Name;
		unsigned int m_uiFlag;
		unsigned int m_uiElementOffset;
	};
	template<typename T>
	class VSEnumProperty : public VSProperty
	{
	public:
		VSEnumProperty()
		{
			VSMAC_ASSERT(sizeof(T) == 4);
		}
		VSEnumProperty(VSRtti & Owner,const VSUsedName & Name,const VSUsedName & EnumName,unsigned int uiElementOffset,unsigned int uiFlag)
			:VSProperty(Owner,Name,uiElementOffset,uiFlag)
		{
			VSMAC_ASSERT(sizeof(T) == 4);
			m_EnumName = EnumName;
		}
		VSEnumProperty(VSFunction & Owner, const VSUsedName & Name, const VSUsedName & EnumName, unsigned int uiElementOffset, unsigned int uiFlag)
			:VSProperty(Owner, Name, uiElementOffset, uiFlag)
		{
			VSMAC_ASSERT(sizeof(T) == 4);
			m_EnumName = EnumName;
		}
		virtual ~VSEnumProperty()
		{

		}
		virtual bool Clone(VSProperty * p)
		{
			VSEnumProperty * Temp = (VSEnumProperty *)p;
			if (!VSProperty::Clone(Temp))
				return false;
			m_EnumName = Temp->m_EnumName;
			return true;

		}
		virtual bool SetValue(void* pObj, unsigned int& pDataSrc) const
		{

			*(unsigned int*)(((unsigned char*)pObj) + m_uiElementOffset) =  pDataSrc;
			return true;

		}
		virtual bool GetValue(void* pObj, unsigned int& pDataDest) const
		{
			pDataDest = *(unsigned int*)(((unsigned char*)pObj) + m_uiElementOffset);
			return true;
		}
		virtual bool GetValue(const void* pObj, unsigned int& pDataDest) const
		{
			pDataDest = *(const unsigned int*)(((const char*)pObj) + m_uiElementOffset);
			return true;
		}
		virtual unsigned int& Value(void* pObj)const
		{
			return *(unsigned int*)(((const char*)pObj) + m_uiElementOffset);
		}
		virtual bool Archive(VSStream &Stream,void* pObj)
		{
			Stream.Archive(Value(pObj));
			return true;
		}
		virtual void CloneData(void* pSrcObj,void* pDestObj,VSMap<VSObject *,VSObject*>& CloneMap)
		{
			Value(pDestObj) = Value(pSrcObj);
		}
		virtual void CopyData(void* pSrcObj,void* pDestObj)
		{
			Value(pDestObj) = Value(pSrcObj);
		}
		virtual void AddEditorElement(void * pSrcObj,VSECollection * pParent,VSString &Name)
		{
			VSEEnumProperty * pEp = VS_NEW VSEEnumProperty((unsigned int*)(((const char*)pSrcObj) + m_uiElementOffset),Name,(VSObject*)pSrcObj);
			pParent->AddElement(pEp);
			VSArray<VSString> AS;
			VSString EnumName = m_EnumName.GetString();
			GET_ENUMARRAY(EnumName,AS);
			pEp->AddEnumString(AS);
		}
		virtual VSProperty * GetInstance()
		{
			return VS_NEW VSEnumProperty();
		}
		VSUsedName & GetEnumName()
		{
			return m_EnumName;
		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_ENUM;
		}
	protected:
		VSUsedName m_EnumName;
	};
	template<typename T,typename NumType>
	class VSDataProperty : public VSProperty
	{
	public:
		VSDataProperty(VSRtti & Owner,const VSUsedName & Name,unsigned int uiElementOffset,unsigned int uiDataNum,bool bDynamicCreate)
			:VSProperty(Owner,Name,uiElementOffset,F_SAVE_LOAD_CLONE | F_NO_USE_GC)
		{
			m_uiDataNum = uiDataNum;
			m_bDynamicCreate = bDynamicCreate;
		}
		VSDataProperty(VSFunction & Owner, const VSUsedName & Name, unsigned int uiElementOffset, unsigned int uiDataNum, bool bDynamicCreate)
			:VSProperty(Owner, Name, uiElementOffset, F_SAVE_LOAD_CLONE | F_NO_USE_GC)
		{
			m_uiDataNum = uiDataNum;
			m_bDynamicCreate = bDynamicCreate;
		}
		VSDataProperty()
		{

		}
		VSDataProperty(VSRtti & Owner,const VSUsedName & Name,unsigned int uiElementOffset,unsigned int uiNumElementOffset)
			:VSProperty(Owner, Name, uiElementOffset, F_SAVE_LOAD_CLONE | F_NO_USE_GC)
		{
			m_uiDataNum = 0;
			m_bDynamicCreate = true;
			m_uiNumElementOffset = uiNumElementOffset;
		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_DATA;
		}
		virtual void AddEditorElement(void * pSrcObj,VSECollection * pParent,VSString &Name)
		{
			return ;
		}
		virtual void CopyData(void* pSrcObj,void* pDestObj)
		{
			VSMAC_ASSERT(0);
		}
		virtual void CloneData(void* pSrcObj,void* pDestObj,VSMap<VSObject *,VSObject*>& CloneMap)
		{
			T * SrcValueAddres = *(T**)GetValueAddress(pSrcObj);

			if (m_uiDataNum > 0)
			{
				T** Temp = (T**)GetValueAddress(pDestObj);
				if (m_bDynamicCreate)
				{
					*Temp = VS_NEW T[m_uiDataNum];	
					VSMemcpy((void *)(*Temp),(void *)SrcValueAddres,m_uiDataNum * sizeof(T));
				}
				else
				{
					VSMemcpy((void *)(*Temp),(void *)SrcValueAddres,m_uiDataNum * sizeof(T));
				}
			}
			else
			{
				T** Temp = (T**)GetValueAddress(pDestObj);
				
				
				void * SrcNumOffSet = (void*)(((unsigned char*)pSrcObj) + m_uiNumElementOffset);
				void * DestNumOffSet = (void*)(((unsigned char*)pDestObj) + m_uiNumElementOffset);
				*(NumType*)DestNumOffSet = *(NumType*)SrcNumOffSet;
				NumType uiNum = *(NumType*)SrcNumOffSet;

				*Temp = VS_NEW T[uiNum];
				VSMemcpy((void *)(*Temp),(void *)SrcValueAddres,uiNum * sizeof(T));
			}
		}
		virtual bool Archive(VSStream &Stream,void* pObj)
		{
			unsigned int uiStreamFlag = Stream.GetStreamFlag();
			if (uiStreamFlag == VSStream::AT_SAVE)
			{
				T * ValueAddres = *(T**)GetValueAddress(pObj);

				if (m_uiDataNum > 0)
				{
					Stream.Write(ValueAddres,m_uiDataNum * sizeof(T));
				}
				else
				{
					void * NumOffSet = (void*)(((unsigned char*)pObj) + m_uiNumElementOffset);
					NumType uiNum = *(NumType*)NumOffSet;
					Stream.Write(NumOffSet,sizeof(NumType));
					Stream.Write(ValueAddres,uiNum * sizeof(T));
				}
			}
			else if (uiStreamFlag == VSStream::AT_LOAD)
			{
				T** Temp = (T**)GetValueAddress(pObj);
				if (m_uiDataNum > 0)
				{
					
					if (m_bDynamicCreate)
					{
						*Temp = VS_NEW T[m_uiDataNum];									
						Stream.Read((void *)(*Temp),m_uiDataNum * sizeof(T));
						
					}
					else
					{
						Stream.Read((void *)(*Temp),m_uiDataNum * sizeof(T));
					}
					
				}
				else
				{
					void * NumOffSet = (void*)(((unsigned char*)pObj) + m_uiNumElementOffset);
					Stream.Read(NumOffSet,sizeof(NumType));
					NumType uiNum = *(NumType*)NumOffSet;
					*Temp = VS_NEW T[uiNum];									
					Stream.Read((void *)(*Temp),uiNum * sizeof(T));
				}
			}
			else if (uiStreamFlag == VSStream::AT_SIZE)
			{
				if (m_uiDataNum > 0)
				{
					Stream.AddBufferSize(m_uiDataNum * sizeof(T));
				}
				else
				{
					void * NumOffSet = (void*)(((unsigned char*)pObj) + m_uiNumElementOffset);
					NumType uiNum = *(NumType*)NumOffSet;
					Stream.AddBufferSize(sizeof(NumType));
					Stream.AddBufferSize(uiNum * sizeof(T));
				}
			}
			return true;
		}
		virtual VSProperty * GetInstance()
		{
			return VS_NEW VSDataProperty<T,NumType>();
		}
		virtual bool Clone(VSProperty * p)
		{
			VSDataProperty<T,NumType> * Temp = (VSDataProperty<T,NumType> *)p;
			if (!VSProperty::Clone(Temp))
				return false;
			m_bDynamicCreate = Temp->m_bDynamicCreate;
			m_uiDataNum = Temp->m_uiDataNum;
			m_uiNumElementOffset = Temp->m_uiNumElementOffset;
			return true;
		}
	protected:
		bool m_bDynamicCreate;
		unsigned int m_uiDataNum;
		unsigned int m_uiNumElementOffset;
	};
	
	template<typename T>
	class VSValueBaseProperty : public VSProperty
	{
	public:
		VSValueBaseProperty()
		{

		}
		VSValueBaseProperty(VSRtti & Owner, const VSUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:VSProperty(Owner,Name,uiElementOffset,uiFlag)
		{
			m_LowValue = LowValue;
			m_HightValue = HighValue;
			m_fStep = fStep;
			m_bRange = Range;
		}
		VSValueBaseProperty(VSFunction & Owner, const VSUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:VSProperty(Owner, Name, uiElementOffset, uiFlag)
		{
			m_LowValue = LowValue;
			m_HightValue = HighValue;
			m_fStep = fStep;
			m_bRange = Range;
		}
		virtual ~VSValueBaseProperty()
		{
		
		}
		virtual bool Clone(VSProperty * p)
		{
			VSValueBaseProperty<T> * Temp = (VSValueBaseProperty<T> *)p;
			if (!VSProperty::Clone(Temp))
				return false;
			m_LowValue = Temp->m_LowValue;
			m_HightValue = Temp->m_HightValue;
			return true;
		}
	protected:
		T m_LowValue;
		T m_HightValue;
		T m_fStep;
		bool m_bRange;
	};
	template<typename T>
	class VSValueProperty : public VSValueBaseProperty<T>
	{
	public:
		VSValueProperty()
		{

		}
		VSValueProperty(VSRtti & Owner, const VSUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false ,T HighValue = T(), T LowValue = T(), T fStep = T())
			:VSValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range,HighValue, LowValue, fStep)
		{

		}
		VSValueProperty(VSFunction & Owner, const VSUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:VSValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		virtual ~VSValueProperty()
		{

		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_VALUE;
		}
		virtual bool SetValue(void* pObj, T& pDataSrc) const
		{

			if (pDataSrc > m_HightValue || pDataSrc < m_LowValue)
			{
				return false;
			}

			*(T*)(((unsigned char*)pObj) + m_uiElementOffset) =  pDataSrc;
			return true;
				
		}
		virtual bool GetValue(void* pObj, T& pDataDest) const
		{
			pDataDest = *(T*)(((unsigned char*)pObj) + m_uiElementOffset);
			return true;
		}
		virtual bool GetValue(const void* pObj, T& pDataDest) const
		{
			pDataDest = *(const T*)(((const char*)pObj) + m_uiElementOffset);
			return true;
		}
		virtual T& Value(void* pObj)const
		{
			return *(T*)(((const char*)pObj) + m_uiElementOffset);
		}
		virtual bool Archive(VSStream &Stream,void* pObj)
		{
			Stream.Archive(Value(pObj));
			return true;
		}
		virtual void CloneData(void* pSrcObj,void* pDestObj,VSMap<VSObject *,VSObject*>& CloneMap)
		{
			Copy(Value(pDestObj),Value(pSrcObj),CloneMap);
		}
		virtual void CopyData(void* pSrcObj,void* pDestObj)
		{
			Value(pDestObj) = Value(pSrcObj);
		}
		virtual void AddEditorElement(void * pSrcObj,VSECollection * pParent,VSString &Name)
		{
			CreateEditorElement(Value(pSrcObj), (VSObject *)pSrcObj, pParent, Name, m_bRange,m_HightValue, m_LowValue, m_fStep);
		}
		virtual VSProperty * GetInstance()
		{
			return VS_NEW VSValueProperty<T>();
		}
	};

	template<typename ArrayType,typename T>
	class VSArrayProperty : public VSValueBaseProperty<T>
	{
	public:
		VSArrayProperty()
		{

		}
		VSArrayProperty(VSRtti & Owner, const VSUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:VSValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		VSArrayProperty(VSFunction & Owner, const VSUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:VSValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		virtual ~VSArrayProperty()
		{

		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_ARRAY;
		}
		FORCEINLINE ArrayType & GetContaine(void* pObj)const
		{
			return (*(ArrayType*)(((unsigned char*)pObj) + m_uiElementOffset));
		}
		FORCEINLINE bool AddElement(void* pObj,unsigned int uiIndex,T& pDataSrc)
		{
			GetContaine(pObj).AddElement(pDataSrc);
			return true;
		}
		FORCEINLINE bool Erase(void* pObj,unsigned int i)
		{
			GetContaine(pObj).Erase(i);
		}
		virtual bool SetValue(void* pObj,unsigned int uiIndex,T& pDataSrc) const
		{

			if (pDataSrc > m_HightValue || pDataSrc < m_LowValue)
			{
				return false;
			}
			(GetContaine(pObj)[uiIndex]) =  pDataSrc;
			return true;
				
		}
		virtual bool GetValue(void* pObj, unsigned int uiIndex, T& pDataDest) const
		{
			pDataDest = (GetContaine(pObj)[uiIndex]);
			return true;
		}
		virtual bool Archive(VSStream &Stream,void* pObj)
		{
			Stream.Archive(GetContaine(pObj));
			return true;
		}
		virtual void CloneData(void* pSrcObj,void* pDestObj,VSMap<VSObject *,VSObject*>& CloneMap)
		{
			Copy(GetContaine(pDestObj),GetContaine(pSrcObj),CloneMap);
		}
		virtual void CopyData(void* pSrcObj,void* pDestObj)
		{
			GetContaine(pDestObj) = GetContaine(pSrcObj);
		}
		virtual void AddEditorElement(void * pSrcObj,VSECollection * pParent,VSString &Name)
		{
			CreateEditorElement(GetContaine(pSrcObj), (VSObject *)pSrcObj, pParent, Name, m_bRange, m_HightValue, m_LowValue, m_fStep);
		}
		virtual VSProperty * GetInstance()
		{
			return VS_NEW VSArrayProperty<ArrayType,T>();
		}
	};


	template<typename MapType,typename KEY,typename VALUE>
	class VSMapProperty : public VSValueBaseProperty<VALUE>
	{
	public:
		VSMapProperty(VSRtti & Owner, const VSUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false,VALUE HighValue = VALUE(), VALUE LowValue = VALUE(), VALUE fStep = VALUE())
			:VSValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		VSMapProperty(VSFunction & Owner, const VSUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, VALUE HighValue = VALUE(), VALUE LowValue = VALUE(), VALUE fStep = VALUE())
			:VSValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		VSMapProperty()
		{

		}
		virtual ~VSMapProperty()
		{

		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_MAP;
		}
		FORCEINLINE MapType & GetContaine(void* pObj)const
		{
			return (*(MapType*)(((unsigned char*)pObj) + m_uiElementOffset));
		}
		FORCEINLINE bool AddElement(void* pObj,unsigned int uiIndex,MapElement<KEY,VALUE>& pDataSrc)
		{
			GetContaine(pObj).AddElement(pDataSrc);
			return true;
		}
		FORCEINLINE bool Erase(void* pObj,unsigned int i)
		{
			GetContaine(pObj).Erase(i);
		}
		virtual bool SetValue(void* pObj,unsigned int uiIndex,KEY & Key,VALUE& Value) const
		{
			
			if (Value > m_HightValue || Value < m_LowValue)
			{
				return false;
			}

			MapElement<KEY,VALUE> Temp(Key,Value);
			(GetContaine(pObj)[uiIndex]) =  Temp;
			return true;

		}
		virtual bool GetValue(void* pObj, unsigned int uiIndex, MapElement<KEY,VALUE>& pDataDest) const
		{
			pDataDest = (GetContaine(pObj)[uiIndex]);
			return true;
		}
		virtual bool Archive(VSStream &Stream,void* pObj)
		{
			Stream.Archive(GetContaine(pObj));
			return true;
		}
		virtual void CloneData(void* pSrcObj,void* pDestObj,VSMap<VSObject *,VSObject*>& CloneMap)
		{
			Copy(GetContaine(pDestObj),GetContaine(pSrcObj),CloneMap);
		}
		virtual void CopyData(void* pSrcObj,void* pDestObj)
		{
			GetContaine(pDestObj) = GetContaine(pSrcObj);
		}
		virtual void AddEditorElement(void * pSrcObj,VSECollection * pParent,VSString &Name)
		{
			CreateEditorElement(GetContaine(pSrcObj), (VSObject *)pSrcObj, pParent, Name, m_bRange, m_HightValue, m_LowValue, m_fStep);
		}
		virtual VSProperty * GetInstance()
		{
			return VS_NEW VSMapProperty<MapType,KEY,VALUE>();
		}
	};


	template<class T,class NumType>
	struct DataPropertyCreator
	{
		VSProperty* CreateProperty(const VSUsedName & Name, VSRtti & Owner, unsigned int Offset,unsigned int NumOffset)
		{

			return VS_NEW VSDataProperty<T,NumType>(Owner, Name,Offset,NumOffset);
		}
		VSProperty* CreateFunctionProperty(const VSUsedName & Name, VSFunction & Owner, unsigned int Offset, unsigned int NumOffset)
		{

			return VS_NEW VSDataProperty<T, NumType>(Owner, Name, Offset, NumOffset);
		}
		VSProperty* CreateProperty(const VSUsedName & Name, VSRtti & Owner, unsigned int Offset,unsigned int uiDataNum,bool bDynamicCreate)
		{

			return VS_NEW VSDataProperty<T,NumType>(Owner, Name,Offset,uiDataNum,bDynamicCreate);
		}
	};
	template<class T>
	struct AutoPropertyCreator
	{
		VSProperty* CreateProperty(const VSUsedName & Name, VSRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value))
			{
				uiFlag |= VSProperty::F_NO_USE_GC;
			}
			return VS_NEW VSValueProperty<T>(Owner, Name,Offset,uiFlag);
	
			
		}
		VSProperty* CreateFunctionProperty(const VSUsedName & Name, VSFunction & Owner, unsigned int Offset, unsigned int uiFlag)
		{

			return VS_NEW VSValueProperty<T>(Owner, Name, Offset, uiFlag);
		}
		VSProperty* CreateProperty(const VSUsedName & Name, VSRtti & Owner, unsigned int Offset,T HighValue ,T LowValue,VSREAL fStep,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value))
			{
				uiFlag |= VSProperty::F_NO_USE_GC;
			}
			return VS_NEW VSValueProperty<T>(Owner, Name, Offset, uiFlag,true,HighValue, LowValue, fStep);
		}
	};

	template<class T,VSMemManagerFun MMFun>
	struct AutoPropertyCreator<VSArray<T,MMFun>>
	{
		VSProperty* CreateProperty(const VSUsedName & Name, VSRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value || ValueBase<T>::NeedsConstructor))
			{
				uiFlag |= VSProperty::F_NO_USE_GC;
			}
			return VS_NEW VSArrayProperty<VSArray<T,MMFun>,T>(Owner, Name,Offset,uiFlag);

		}
		VSProperty* CreateFunctionProperty(const VSUsedName & Name, VSFunction & Owner, unsigned int Offset, unsigned int uiFlag)
		{

			return VS_NEW VSArrayProperty<VSArray<T, MMFun>, T>(Owner, Name, Offset, uiFlag);

		}
		VSProperty* CreateProperty(const VSUsedName & Name, VSRtti & Owner, unsigned int Offset,T HighValue ,T LowValue,T fStep,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value || ValueBase<T>::NeedsConstructor))
			{
				uiFlag |= VSProperty::F_NO_USE_GC;
			}
			return VS_NEW VSArrayProperty<VSArray<T, MMFun>, T>(Owner, Name, Offset, uiFlag, HighValue, LowValue, fStep);

		}
	};


	template<class T,VSMemManagerFun MMFun>
	struct AutoPropertyCreator<VSArrayOrder<T,MMFun>>
	{
		VSProperty* CreateProperty(const VSUsedName & Name, VSRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value || ValueBase<T>::NeedsConstructor))
			{
				uiFlag |= VSProperty::F_NO_USE_GC;
			}
			return VS_NEW VSArrayProperty<VSArrayOrder<T,MMFun>,T>(Owner, Name,Offset,uiFlag);
		}
		VSProperty* CreateFunctionProperty(const VSUsedName & Name, VSFunction & Owner, unsigned int Offset, unsigned int uiFlag)
		{

			return VS_NEW VSArrayProperty<VSArrayOrder<T, MMFun>, T>(Owner, Name, Offset, uiFlag);
		}
		VSProperty* CreateProperty(const VSUsedName & Name, VSRtti & Owner, unsigned int Offset,T HighValue ,T LowValue,VSREAL fStep,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value || ValueBase<T>::NeedsConstructor))
			{
				uiFlag |= VSProperty::F_NO_USE_GC;
			}
			return VS_NEW VSArrayProperty<VSArrayOrder<T, MMFun>, T>(Owner, Name, Offset, uiFlag, HighValue, LowValue, fStep);

		}
	};

	template<class KEY,class VALUE,VSMemManagerFun MMFun>
	struct AutoPropertyCreator<VSMap<KEY,VALUE,MMFun>>
	{
		VSProperty* CreateProperty(const VSUsedName & Name, VSRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{

			return VS_NEW VSMapProperty<VSMap<KEY,VALUE,MMFun>,KEY,VALUE>(Owner, Name,Offset,uiFlag);


		}
		VSProperty* CreateFunctionProperty(const VSUsedName & Name, VSFunction & Owner, unsigned int Offset, unsigned int uiFlag)
		{

			return VS_NEW VSMapProperty<VSMap<KEY, VALUE, MMFun>, KEY, VALUE>(Owner, Name, Offset, uiFlag);


		}
		VSProperty* CreateProperty(const VSUsedName & Name, VSRtti & Owner, unsigned int Offset,VALUE HighValue ,VALUE LowValue,VSREAL fStep,unsigned int uiFlag)
		{

			return VS_NEW VSMapProperty<VSMap<KEY, VALUE, MMFun>, KEY, VALUE>(Owner, Name, Offset, uiFlag, HighValue, LowValue, fStep);


		}
	};
	
	template<class KEY,class VALUE,VSMemManagerFun MMFun>
	struct AutoPropertyCreator<VSMapOrder<KEY,VALUE,MMFun>>
	{
		VSProperty* CreateProperty(const VSUsedName & Name,VSRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{

			return VS_NEW VSMapProperty<VSMapOrder<KEY,VALUE,MMFun>,KEY,VALUE>(Owner, Name,Offset,uiFlag);

		}
		VSProperty* CreateFunctionProperty(const VSUsedName & Name, VSFunction & Owner, unsigned int Offset, unsigned int uiFlag)
		{

			return VS_NEW VSMapProperty<VSMapOrder<KEY, VALUE, MMFun>, KEY, VALUE>(Owner, Name, Offset, uiFlag);

		}
		VSProperty* CreateProperty(const VSUsedName & Name,VSRtti & Owner, unsigned int Offset,VALUE HighValue ,VALUE LowValue,VSREAL fStep,unsigned int uiFlag)
		{

			return VS_NEW VSMapProperty<VSMapOrder<KEY, VALUE, MMFun>, KEY, VALUE>(Owner, Name, Offset, uiFlag, HighValue, LowValue, fStep);
	
		}
		
	};

	
	class PropertyCreator
	{
	public:
		template<class ValueType>
		static AutoPropertyCreator<ValueType>& GetAutoPropertyCreator(ValueType& valueTypeDummyRef)
		{
			static AutoPropertyCreator<ValueType> apc;
			return apc;
		}
		template<class ValueType,class NumType>
		static DataPropertyCreator<ValueType,NumType>& GetAutoPropertyCreator(ValueType *& valueTypeDummyRef,NumType& valueNumTypeDummyRef)
		{
			static DataPropertyCreator<ValueType,NumType> apc;
			return apc;
		}
		template<class ValueType>
		static VSProperty* CreateEnumProperty(ValueType& valueTypeDummyRef,const VSUsedName & Name, const VSUsedName & EnumName,VSRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{
			return VS_NEW VSEnumProperty<ValueType>(Owner, Name, EnumName, Offset, uiFlag | VSProperty::F_NO_USE_GC);
		}
	};
	typedef void(*FunctionTemplatePtr)(VSObject * p, VSFunction * pFun, void * para, void *ret);
	class VSFunction
	{
	public:
		enum	// Flag 
		{
			F_DEFAULT	= 0X00,
			F_CLIENT	= 0X01,
			F_SERVER	= 0X02,
			F_REFLECT_UI =0X04,
			F_MAX
		};
		VSFunction()
		{
			m_pReturnProperty = NULL;
			ObjectFun = NULL;
		}
		~VSFunction()
		{
			for (unsigned int i = 0; i < m_PropertyArray.GetNum(); i++)
			{
				if (m_PropertyArray[i])
				{
					VSMAC_DELETE(m_PropertyArray[i])
				}
			}
			VSMAC_DELETE(m_pReturnProperty);
		}
		VSFunction(VSRtti & Owner, const VSUsedName & Name, unsigned int uiFlag)
		:m_pOwner(&Owner)
		{
			m_Name = Name;
			m_uiFlag = uiFlag;
			m_pReturnProperty = NULL;
			ObjectFun = NULL;
		}
		FORCEINLINE VSRtti * GetRtti()const
		{
			return m_pOwner;
		}
		FORCEINLINE const VSUsedName & GetName()const
		{
			return m_Name;
		}
		virtual bool Clone(VSFunction * f)
		{
			if ((f->GetRtti() == GetRtti() && GetRtti()))
			{
				m_Name = f->m_Name;
				m_uiFlag = f->m_uiFlag;
				for (unsigned int i = 0; i < m_PropertyArray.GetNum(); i++)
				{
					VSProperty * p = f->m_PropertyArray[i]->GetInstance();
					m_PropertyArray.AddElement(p);
					p->Clone(f->m_PropertyArray[i]);
				}
				return true;
			}
			return false;

		}
		virtual VSFunction * GetInstance()
		{
			return VS_NEW VSFunction();
		}
		void SetOwner(VSRtti & Owner)
		{
			m_pOwner = &Owner;
		}
		unsigned int GetFlag()const
		{
			return m_uiFlag;
		}
		void AddProperty(VSProperty * P)
		{
			m_PropertyArray.AddElement(P);
		}
		VSProperty * GetProperty(unsigned int i)
		{
			return m_PropertyArray[i];
		}
		unsigned int GetPropertyNum()const
		{
			return m_PropertyArray.GetNum();
		}
		void SetReturnType(VSProperty *p)
		{
			m_pReturnProperty = p;
		}
		bool IsReturnVoid()
		{
			return !m_pReturnProperty;
		}
		FunctionTemplatePtr ObjectFun;
		void SetTotleSize(unsigned int uiTotleSize)
		{
			m_uiTotleSize = uiTotleSize;
		}
		bool IsSame(VSFunction * p)
		{
			if (m_pOwner != p->m_pOwner)
			{
				return false;
			}
			else if (m_Name != p->m_Name)
			{
				return false;
			}
			else if (m_uiTotleSize != p->m_uiTotleSize)
			{
				return false;
			}
			else if (m_PropertyArray.GetNum() != p->m_PropertyArray.GetNum())
			{
				return false;
			}
			else 
			{
				return true;
			}
		}
	protected:
		VSRtti * m_pOwner;
		VSUsedName m_Name;
		unsigned int m_uiFlag;
		VSArray<VSProperty *> m_PropertyArray;
		VSProperty * m_pReturnProperty;
		unsigned int m_uiTotleSize;
	};

	

}


#endif