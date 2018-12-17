#ifndef VSALLOC_H
#define VSALLOC_H
#include "VSMemManager.h"
#ifdef VSDATESTRUCT_EXPORTS
#define VSDATESTRUCT_API __declspec(dllexport) 
#else 
#define VSDATESTRUCT_API __declspec(dllimport) 
#endif
namespace VSEngine2
{
	template<class KEY, class VALUE>
	class MapElement : public VSMemObject
	{
	public:
		KEY		Key;
		VALUE	Value;
		void operator= (const MapElement<KEY, VALUE> & Element)
		{
			Key = Element.Key;
			Value = Element.Value;
		}
		MapElement()
		{

		}
		MapElement(const MapElement<KEY, VALUE> & Element)
		{
			Key = Element.Key;
			Value = Element.Value;
		}
		MapElement(const KEY &_Key, const VALUE &_Value)
		{
			Key = _Key;
			Value = _Value;
		}
		~MapElement()
		{

		}

	};
	template <class KEY, class VALUE,class N>
	void Merge(MapElement<KEY, VALUE> *pBuffer, MapElement<KEY, VALUE> *TempBuffer, unsigned int uiIndex0, unsigned int uiIndexLength0, unsigned int uiIndexLength1,N Compare)
	{

		unsigned int uiIndex1 = uiIndex0 + uiIndexLength0;

		unsigned int uiEnd0 = uiIndexLength0 + uiIndex0;
		unsigned int uiEnd1 = uiIndex1 + uiIndexLength1;

		unsigned int uiTempIndex = uiIndex0;
		while (uiIndex0 < uiEnd0 && uiIndex1 < uiEnd1)
		{
			if (Compare(pBuffer[uiIndex0], pBuffer[uiIndex1]) == 0)
			{

				TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
				uiIndex1++;
			}
			else
			{
				TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
				uiIndex0++;
			}
			uiTempIndex++;
		}


		while (uiIndex0 < uiEnd0)
		{

			TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
			uiTempIndex++;
			uiIndex0++;
		}
		while (uiIndex1 < uiEnd1)
		{
			TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
			uiTempIndex++;
			uiIndex1++;
		}

	}
	template <class KEY, class VALUE,class N>
	void MergeSort(MapElement<KEY, VALUE> *pBuffer, MapElement<KEY, VALUE> *TempBuffer, unsigned int  uiBufferLength,N Compare)
	{
		unsigned int uiDetLength = 1;
		unsigned int uiNumSwap = 0;
		while (uiDetLength < uiBufferLength)
		{
			unsigned int uiLastDetLength = uiDetLength;
			uiDetLength = uiDetLength << 1;
			unsigned int i = 0;
			for (; i + uiDetLength < uiBufferLength; i = i + uiDetLength)
			{
				Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLastDetLength, Compare);
			}


			if (i < uiBufferLength)
			{
				unsigned int uiLeftLength = uiBufferLength - i;
				if (uiLeftLength > uiLastDetLength)
				{
					Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLeftLength - uiLastDetLength, Compare);
				}
				else
				{
					Merge(pBuffer, TempBuffer, i, uiLeftLength, 0, Compare);
				}
			}
			Swap(pBuffer, TempBuffer);
			uiNumSwap++;
		}
		if ((uiNumSwap % 2) != 0)
		{
			VSMemcpy(TempBuffer, pBuffer, sizeof(MapElement<KEY, VALUE>)* uiBufferLength);
		}
	}
	template <class KEY, class VALUE>
	void Merge(MapElement<KEY, VALUE> *pBuffer, MapElement<KEY, VALUE> *TempBuffer, unsigned int uiIndex0, unsigned int uiIndexLength0, unsigned int uiIndexLength1)
	{

		unsigned int uiIndex1 = uiIndex0 + uiIndexLength0;

		unsigned int uiEnd0 = uiIndexLength0 + uiIndex0;
		unsigned int uiEnd1 = uiIndex1 + uiIndexLength1;

		unsigned int uiTempIndex = uiIndex0;
		while (uiIndex0 < uiEnd0 && uiIndex1 < uiEnd1)
		{
			if (pBuffer[uiIndex0].Key > pBuffer[uiIndex1].Key)
			{

				TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
				uiIndex1++;
			}
			else
			{
				TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
				uiIndex0++;
			}
			uiTempIndex++;
		}


		while (uiIndex0 < uiEnd0)
		{

			TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
			uiTempIndex++;
			uiIndex0++;
		}
		while (uiIndex1 < uiEnd1)
		{
			TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
			uiTempIndex++;
			uiIndex1++;
		}

	}
	template <class KEY, class VALUE>
	void MergeSort(MapElement<KEY, VALUE> *pBuffer, MapElement<KEY, VALUE> *TempBuffer, unsigned int  uiBufferLength)
	{
		unsigned int uiDetLength = 1;
		unsigned int uiNumSwap = 0;
		while (uiDetLength < uiBufferLength)
		{
			unsigned int uiLastDetLength = uiDetLength;
			uiDetLength = uiDetLength << 1;
			unsigned int i = 0;
			for (; i + uiDetLength < uiBufferLength; i = i + uiDetLength)
			{
				Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLastDetLength);
			}


			if (i < uiBufferLength)
			{
				unsigned int uiLeftLength = uiBufferLength - i;
				if (uiLeftLength > uiLastDetLength)
				{
					Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLeftLength - uiLastDetLength);
				}
				else
				{
					Merge(pBuffer, TempBuffer, i, uiLeftLength, 0);
				}
			}
			Swap(pBuffer, TempBuffer);
			uiNumSwap++;
		}
		if ((uiNumSwap % 2) != 0)
		{
			VSMemcpy(TempBuffer, pBuffer, sizeof(MapElement<KEY, VALUE>)* uiBufferLength);
		}
	}

	template <class T>
	void Merge(T *pBuffer, T *TempBuffer, unsigned int uiIndex0, unsigned int uiIndexLength0, unsigned int uiIndexLength1)
	{

		unsigned int uiIndex1 = uiIndex0 + uiIndexLength0;

		unsigned int uiEnd0 = uiIndexLength0 + uiIndex0;
		unsigned int uiEnd1 = uiIndex1 + uiIndexLength1;

		unsigned int uiTempIndex = uiIndex0;
		while (uiIndex0 < uiEnd0 && uiIndex1 < uiEnd1)
		{
			if (pBuffer[uiIndex0] > pBuffer[uiIndex1])
			{

				TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
				uiIndex1++;
			}
			else
			{
				TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
				uiIndex0++;
			}
			uiTempIndex++;
		}


		while (uiIndex0 < uiEnd0)
		{

			TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
			uiTempIndex++;
			uiIndex0++;
		}
		while (uiIndex1 < uiEnd1)
		{
			TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
			uiTempIndex++;
			uiIndex1++;
		}

	}
	template <class T>
	void MergeSort(T *pBuffer, T *TempBuffer, unsigned int  uiBufferLength)
	{
		unsigned int uiDetLength = 1;
		unsigned int uiNumSwap = 0;
		while (uiDetLength < uiBufferLength)
		{
			unsigned int uiLastDetLength = uiDetLength;
			uiDetLength = uiDetLength << 1;
			unsigned int i = 0;
			for (; i + uiDetLength < uiBufferLength; i = i + uiDetLength)
			{
				Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLastDetLength);
			}


			if (i < uiBufferLength)
			{
				unsigned int uiLeftLength = uiBufferLength - i;
				if (uiLeftLength > uiLastDetLength)
				{
					Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLeftLength - uiLastDetLength);
				}
				else
				{
					Merge(pBuffer, TempBuffer, i, uiLeftLength, 0);
				}
			}
			Swap(pBuffer, TempBuffer);
			uiNumSwap++;
		}
		if ((uiNumSwap % 2) != 0)
		{
			VSMemcpy(TempBuffer, pBuffer, sizeof(T)* uiBufferLength);
		}
	}

	template <class T, class N>
	void Merge(T *pBuffer, T *TempBuffer, unsigned int uiIndex0, unsigned int uiIndexLength0, unsigned int uiIndexLength1, N Compare)
	{

		unsigned int uiIndex1 = uiIndex0 + uiIndexLength0;

		unsigned int uiEnd0 = uiIndexLength0 + uiIndex0;
		unsigned int uiEnd1 = uiIndex1 + uiIndexLength1;

		unsigned int uiTempIndex = uiIndex0;
		while (uiIndex0 < uiEnd0 && uiIndex1 < uiEnd1)
		{
			if (Compare(pBuffer[uiIndex0], pBuffer[uiIndex1]) == 0)
			{

				TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
				uiIndex1++;
			}
			else
			{
				TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
				uiIndex0++;
			}
			uiTempIndex++;
		}


		while (uiIndex0 < uiEnd0)
		{

			TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
			uiTempIndex++;
			uiIndex0++;
		}
		while (uiIndex1 < uiEnd1)
		{
			TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
			uiTempIndex++;
			uiIndex1++;
		}

	}
	template <class T, class N>
	void MergeSort(T *pBuffer, T *TempBuffer, unsigned int  uiBufferLength, N Compare)
	{
		unsigned int uiDetLength = 1;
		unsigned int uiNumSwap = 0;
		while (uiDetLength < uiBufferLength)
		{
			unsigned int uiLastDetLength = uiDetLength;
			uiDetLength = uiDetLength << 1;
			unsigned int i = 0;
			for (; i + uiDetLength < uiBufferLength; i = i + uiDetLength)
			{
				Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLastDetLength, Compare);
			}


			if (i < uiBufferLength)
			{
				unsigned int uiLeftLength = uiBufferLength - i;
				if (uiLeftLength > uiLastDetLength)
				{
					Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLeftLength - uiLastDetLength, Compare);
				}
				else
				{
					Merge(pBuffer, TempBuffer, i, uiLeftLength, 0, Compare);
				}
			}
			Swap(pBuffer, TempBuffer);
			uiNumSwap++;
		}
		if ((uiNumSwap % 2) != 0)
		{
			VSMemcpy(TempBuffer, pBuffer, sizeof(T)* uiBufferLength);
		}
	}
	template <class T,VSMemManagerFun MMFun = VSMemObject::GetMemManager>
	class VSContainer : public VSMemObject
	{
	protected:
		T * New(unsigned int uiNum)
		{
			if (!uiNum)
			{
				return NULL;
			}
#ifdef USE_CUSTOM_NEW
			T* pPtr = (T *)MMFun().Allocate(uiNum * sizeof(T),0,true);
			VSMAC_ASSERT(pPtr);
			if (!pPtr)
			{
				return NULL;
			}
// 			if (ValueBase<T>::NeedsConstructor)
// 			{
// 				for (unsigned int i = 0 ; i < uiNum ; i++)
// 				{
// 					VS_NEW(pPtr + i)T();
// 				}
// 			}
#else
			T* pPtr = (T*)malloc(sizeof(T) * uiNum)
			VSMAC_ASSERT(pPtr);
			if (!pPtr)
			{
				return NULL;
			}
#endif
			return pPtr;
		}

		void Delete(T * & pPtr,unsigned int uiNum)
		{
			if (!pPtr)
			{
				return ;
			}
#ifdef USE_CUSTOM_NEW
			if (uiNum > 0)
			{
				if (ValueBase<T>::NeedsDestructor)
				{
					for (unsigned int i = 0 ; i < uiNum ; i++)
					{
						(pPtr + i)->~T();
					}
				}
			}

			MMFun().Deallocate((char *)pPtr,0,true);
			pPtr = NULL;
#else
			free(pPtr);
			pPtr = NULL;
#endif
			
		}

	};
}

#endif