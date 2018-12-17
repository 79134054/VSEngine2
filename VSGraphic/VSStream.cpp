#include "VSStream.h"
#include "VSObject.h"
#include "VSGraphicInclude.h"
#include "VSResourceManager.h"
using namespace VSEngine2;
unsigned int VSStream::ms_uiCurVerson = 0;
/********************************************************************************/
int VSStream::GetStrDistUse(const VSString &Str)
{
	return sizeof(int) + (unsigned int)Str.GetLength() * sizeof(TCHAR);

}
/********************************************************************************/
int VSStream::GetStrDistUse(const TCHAR * pCh)
{
	return sizeof(int) + (unsigned int)VSStrLen(pCh) * sizeof(TCHAR);
}
/********************************************************************************/
VSStream::VSStream()
{
	m_pVObjectArray.Clear();
	m_pmLoadMap.Clear();
	m_pmVSTypeLoadMap.Clear();
	m_pcCurBufPtr	= NULL;
	m_pcBuffer		= NULL;
	m_uiBufferSize	= 0;
	m_dwFlag = 0;
	m_bLoadUseGC = false;
}
/********************************************************************************/
VSStream::~VSStream()
{

	m_pVObjectArray.Clear();
	m_pcCurBufPtr	= NULL;

}
/********************************************************************************/
bool VSStream::Read(void * pvBuffer,unsigned int uiSize)
{

	if(!pvBuffer || !m_pcBuffer)
		return 0;

	if(unsigned int(m_pcCurBufPtr + uiSize - m_pcBuffer) > m_uiBufferSize)
		return 0;

	
	VSMemcpy(pvBuffer,m_pcCurBufPtr,uiSize);

	m_pcCurBufPtr +=uiSize;
	return 1;

}
/********************************************************************************/
bool VSStream::Write(const void * pvBuffer,unsigned int uiSize)
{
	if(!pvBuffer || !m_pcBuffer)
		return 0;

	if(unsigned int(m_pcCurBufPtr + uiSize - m_pcBuffer) > m_uiBufferSize)
		return 0;


	VSMemcpy(m_pcCurBufPtr,pvBuffer,uiSize);

	m_pcCurBufPtr +=uiSize;
	return 1;
}
/********************************************************************************/
bool VSStream::ReadString(VSString & String)
{
	unsigned int uiBufferSize = 0;
	unsigned int uiStrLen = 0;
	if(!Read(&uiBufferSize,sizeof(unsigned int)))
		return 0;
	if (!uiBufferSize)
	{
		return 1;
	}
	char * pCh = 0;
	pCh= VS_NEW char[uiBufferSize];
	if(!Read(pCh,uiBufferSize))
	{
		VSMAC_DELETEA(pCh);
		return 0;
	}
	if (HasAnyFlag(UNICODE_FLAG))
	{
		uiStrLen = uiBufferSize >> 1;
		String.SetTCHARBufferNum(uiStrLen);
#ifdef	_UNICODE		
		VSMemcpy(String.GetBuffer(),pCh,uiBufferSize);
#else
		VSWcsToMbs(String.GetBuffer(),uiStrLen,(wchar_t *)pCh,uiStrLen);
#endif
	}
	else
	{
		uiStrLen = uiBufferSize;
		String.SetTCHARBufferNum(uiStrLen);
#ifdef	_UNICODE
		VSMbsToWcs((wchar_t *)String.GetBuffer(),uiStrLen,pCh,uiStrLen);
#else
		VSMemcpy(String.GetBuffer(),pCh,uiBufferSize);
#endif
	}
	VSMAC_DELETEA(pCh);

	return 1;

}
/********************************************************************************/
bool VSStream::WriteString(const VSString & String)
{
	unsigned int uiBufferLen = String.GetLength() * sizeof(TCHAR);
	if(!Write(&uiBufferLen,sizeof(unsigned int)))
		return 0;
	if(!Write(String.GetBuffer(),uiBufferLen))
		return 0;
	return 1;
}
bool VSStream::RegisterReachableObject(VSObject * pObject)
{
	if (pObject->IsHasFlag(VSObject::OF_REACH))
	{
		return false;
	}
	pObject->SetFlag(VSObject::OF_REACH);
	pObject->ClearFlag(VSObject::OF_UNREACH);

	return 1;
}
bool VSStream::RegisterPostLoadObject(VSObject * pObject)
{
	for(unsigned int i = 0 ; i < (unsigned int)m_pPostLoadObject.GetNum(); i++)
	{
		if(m_pPostLoadObject[i] == pObject)
		{
			return 0;
		}

	}
	m_pPostLoadObject.AddElement((VSObject *)pObject);	
	return true;
}

/********************************************************************************/
bool VSStream::RegisterObject(VSObject * pObject)
{
	VSMAC_ASSERT(pObject);
	if(!pObject)
		return 0;

	for(unsigned int i = 0 ; i < (unsigned int)m_pVObjectArray.GetNum(); i++)
	{
		if(m_pVObjectArray[i] == pObject)
		{
			return 0;
		}

	}
	m_pVObjectArray.AddElement((VSObject *)pObject);

	return 1;
}
/********************************************************************************/
VSObject *VSStream::GetVSTypeMapValue(VSObject *pKey)const
{
	unsigned int i = m_pmVSTypeLoadMap.Find(pKey);
	if(i == m_pmVSTypeLoadMap.GetNum())
		return NULL;

	return m_pmVSTypeLoadMap[i].Value;
}
/********************************************************************************/
const VSObject *VSStream::GetMapValue(VSObject* pKey)const 
{
	unsigned int i = m_pmLoadMap.Find(pKey);
	if(i == m_pmLoadMap.GetNum())
		return NULL;
	
	return m_pmLoadMap[i].Value;
}
/********************************************************************************/
const VSObject *VSStream::GetObjectByRtti(const VSRtti &Rtti)
{
	VSObject * pObject = NULL;
	for(unsigned int i = 0 ; i < m_pVObjectArray.GetNum() ; i++)
	{
		if((m_pVObjectArray[i]->GetType()).IsSameType(Rtti))
		{
			pObject = m_pVObjectArray[i];
			break;
		}
	}
	if (pObject == NULL)
	{
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			if ((m_pVObjectArray[i]->GetType()).IsDerived(Rtti))
			{

				pObject = m_pVObjectArray[i];
				break;
			}
		}
	}
	
	if (pObject)
	{
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			VSObject * p = m_pVObjectArray[i];
			VSMAC_ASSERT(p != NULL);
			if (p)
			{
				p->ClearFlag(VSObject::OF_REACH);
				p->SetFlag(VSObject::OF_UNREACH);
			}
		}

		VSStream GCCollectStream;
		GCCollectStream.SetStreamFlag(VSStream::AT_LOAD_OBJECT_COLLECT_GC);
		GCCollectStream.ArchiveAll(pObject);


		VSArray<VSObject *> CanGCObject;
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum();)
		{
			VSObject * p = m_pVObjectArray[i];
			if (p->IsHasFlag(VSObject::OF_UNREACH))
			{
				CanGCObject.AddElement(p);
				m_pVObjectArray.Erase(i);
			}
			else
			{
				i++;
			}
		}

		VSResourceManager::AddCanGCObject(CanGCObject);

	}
	if (pObject)
	{
		m_uiStreamFlag = AT_POSTLOAD;
		ArchiveAll(pObject);
	}
	return pObject;
}
bool VSStream::GetAllResourceObject(VSArray<VSObject *> &ObjectArray)
{
	ObjectArray.Clear();
	for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
	{
		if (VSResourceManager::IsResourceType(m_pVObjectArray[i]->GetType()))
		{

			ObjectArray.AddElement(m_pVObjectArray[i]);
		}
	}
	if (ObjectArray.GetNum() > 0)
	{
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			VSObject * p = m_pVObjectArray[i];
			VSMAC_ASSERT(p != NULL);
			if (p)
			{
				p->ClearFlag(VSObject::OF_REACH);
				p->SetFlag(VSObject::OF_UNREACH);
			}
		}

		VSStream GCCollectStream;
		GCCollectStream.SetStreamFlag(VSStream::AT_LOAD_OBJECT_COLLECT_GC);
		for (unsigned int i = 0; i < ObjectArray.GetNum(); i++)
		{
			GCCollectStream.ArchiveAll(ObjectArray[i]);
		}

		VSArray<VSObject *> CanGCObject;
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum();)
		{
			VSObject * p = m_pVObjectArray[i];
			if (p->IsHasFlag(VSObject::OF_UNREACH))
			{
				CanGCObject.AddElement(p);
				m_pVObjectArray.Erase(i);
			}
			else
			{
				i++;
			}
		}

		VSResourceManager::AddCanGCObject(CanGCObject);
		m_uiStreamFlag = AT_POSTLOAD;
		for (unsigned int i = 0; i < ObjectArray.GetNum(); i++)
		{
			ArchiveAll(ObjectArray[i]);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}
/********************************************************************************/
bool VSStream::GetObjectArrayByRtti(const VSRtti &Rtti, VSArray<VSObject *> &ObjectArray, bool IsDerivedFrom)
{
	ObjectArray.Clear();
	for(unsigned int i = 0 ; i < m_pVObjectArray.GetNum() ; i++)
	{
		if ((m_pVObjectArray[i]->GetType()).IsSameType(Rtti) || ((m_pVObjectArray[i]->GetType()).IsDerived(Rtti) && IsDerivedFrom))
		{

			ObjectArray.AddElement(m_pVObjectArray[i]);
		}
	}
	if (ObjectArray.GetNum() > 0)
	{
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			VSObject * p = m_pVObjectArray[i];
			VSMAC_ASSERT(p != NULL);
			if (p)
			{
				p->ClearFlag(VSObject::OF_REACH);
				p->SetFlag(VSObject::OF_UNREACH);
			}
		}

		VSStream GCCollectStream;
		GCCollectStream.SetStreamFlag(VSStream::AT_LOAD_OBJECT_COLLECT_GC);
		for (unsigned int i = 0; i < ObjectArray.GetNum(); i++)
		{
			GCCollectStream.ArchiveAll(ObjectArray[i]);
		}
		
		VSArray<VSObject *> CanGCObject;
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum();)
		{
			VSObject * p = m_pVObjectArray[i];
			if (p->IsHasFlag(VSObject::OF_UNREACH))
			{
				CanGCObject.AddElement(p);
				m_pVObjectArray.Erase(i);
			}
			else
			{
				i++;
			}
		}

		VSResourceManager::AddCanGCObject(CanGCObject);

		m_uiStreamFlag = AT_POSTLOAD;
		for (unsigned int i = 0; i < ObjectArray.GetNum(); i++)
		{
			ArchiveAll(ObjectArray[i]);
		}

		return 1;
	}
	else
	{
		return 0;
	}
}
/*******************************************************************************


									new version

*******************************************************************************/

bool VSStream::WriteResource(VSResourceProxyBasePtr& Resource)
{
	bool IsNone = true;
	if (Resource)
	{
		IsNone = false;	
	}
	Write(&IsNone,sizeof(bool));
	if (Resource)
	{
		
		VSString FileName = Resource->GetResourceName().GetString();
		WriteString(FileName);
	}
	
	return true;
}

bool VSStream::ReadResource(VSResourceProxyBasePtr& Resource)
{
	bool IsNone = true;
	Read(&IsNone,sizeof(bool));
	if (IsNone == false)
	{
		VSString Name;
		ReadString(Name);
		Resource = VSResourceManager::LoadResource(Name.GetBuffer(),true);
		if (!Resource)
		{
			VSMAC_ASSERT(0);
			return false;
		}
	}
	
	return true;
}
int VSStream::GetResourceDistUse(VSResourceProxyBasePtr& Resource)
{
	if (Resource)
	{
		VSString FileName = Resource->GetResourceName().GetString();
		return GetStrDistUse(FileName);
	}
	return 0;
}
bool VSStream::ArchiveAll(VSObject * pObject)
{
	if (!pObject)
	{
		return false;
	}
	
	if (m_uiStreamFlag == AT_REGISTER)
	{
		if(RegisterObject(pObject))
		{
			VSRtti &Rtti = pObject->GetType();
			for (unsigned int j = 0 ; j < Rtti.GetPropertyNum() ; j++)
			{
				VSProperty * pProperty = Rtti.GetProperty(j);
				if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
				{
					pProperty->Archive(*this,pObject);
				}
			}
		}
	}
	else if (m_uiStreamFlag == AT_POSTLOAD)
	{
		if (RegisterPostLoadObject(pObject))
		{

			VSRtti &Rtti = pObject->GetType();
			for (unsigned int j = 0 ; j < Rtti.GetPropertyNum() ; j++)
			{
				VSProperty * pProperty = Rtti.GetProperty(j);
				if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
				{
					pProperty->Archive(*this,pObject);
				}
			}
			pObject->PostLoad();
		}
	}
	else if (m_uiStreamFlag == AT_OBJECT_COLLECT_GC)
	{
		if (RegisterReachableObject(pObject))
		{
			VSRtti &Rtti = pObject->GetType();
			for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
			{
				VSProperty * pProperty = Rtti.GetProperty(j);
				if (!(pProperty->GetFlag() & VSProperty::F_NO_USE_GC))
				{
					pProperty->Archive(*this, pObject);
				}
			}		
		}
	}
	else if (m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC)
	{
		if (RegisterReachableObject(pObject))
		{
			VSRtti &Rtti = pObject->GetType();
			for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
			{
				VSProperty * pProperty = Rtti.GetProperty(j);			
				pProperty->Archive(*this, pObject);
			}
		}
	}
	else if (m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
	{
		VSRtti &Rtti = pObject->GetType();
		for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);
			if (!(pProperty->GetFlag() & VSProperty::F_NO_USE_GC))
			{
				pProperty->Archive(*this, pObject);
			}
		}
	}
	return true;
}
bool VSStream::NewLoadFromBuffer(unsigned char * pBuffer,unsigned int uiSize)
{

	if (!pBuffer || !uiSize)
	{
		return false;
	}
	m_pVObjectArray.Clear();
	m_pmLoadMap.Clear();
	m_pcCurBufPtr	= NULL;
	m_uiBufferSize	= 0;

	//都加载到内存中
	m_uiBufferSize = uiSize;
	m_pcBuffer = pBuffer;

	m_pcCurBufPtr = m_pcBuffer;

	VSArray<ObjectTabel> ObjectTable;
	//加载版本号
	Read(&m_uiVerson,sizeof(unsigned int));
	//加载物体个数
	unsigned int iObjectNum = 0;
	Read(&iObjectNum,sizeof(unsigned int));

	ObjectTable.SetBufferNum(iObjectNum);
	//Object Table
	for(unsigned int i = 0 ;i < iObjectNum ; i++)
	{
		VSObject * pObject = 0;
		//读取指针
		if(!Read(&ObjectTable[i].m_pAddr,sizeof(VSObject *)))
		{
			return 0;
		}
		//读取RTTI
		if(!ReadString(ObjectTable[i].m_RttiName))
		{
			return 0;
		}

		if(!Read(&ObjectTable[i].m_uiObjectPropertySize,sizeof(unsigned int)))
		{
			return 0;
		}

		if(!Read(&ObjectTable[i].m_uiObjectPropertyNum,sizeof(unsigned int)))
		{
			return 0;
		}


	}

	VSArray<unsigned int> TableID;
	//create object
	for(unsigned int i = 0 ;i < iObjectNum ; i++)
	{
		VSObject * pObject = NULL;
		if (m_bLoadUseGC)
		{
			pObject = VSObject::GetInstance(ObjectTable[i].m_RttiName);
		}
		else
		{
			pObject = VSObject::GetNoGCInstance(ObjectTable[i].m_RttiName);
		}
		//创建空对象
		
		if(!pObject)
		{		
			continue;
		}
		//创建加载映射表
		m_pmLoadMap.AddElement(ObjectTable[i].m_pAddr,pObject);


		if (RegisterObject(pObject))
		{
			TableID.AddElement(i);
		}
	}

	//load object property table
	for (unsigned int i = 0; i < iObjectNum; i++)
	{
		ObjectTable[i].m_ObjectPropertyTable.SetBufferNum(ObjectTable[i].m_uiObjectPropertyNum);

		for (unsigned int j = 0 ; j < ObjectTable[i].m_uiObjectPropertyNum ; j++)
		{


			Read(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID,sizeof(unsigned int));	
			Read(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiOffSet,sizeof(unsigned int));

		}
		m_pcCurBufPtr += ObjectTable[i].m_uiObjectPropertySize;
	}

	m_uiStreamFlag = AT_LOAD;
	//load object property
	
	for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
	{
		VSRtti &Rtti = m_pVObjectArray[i]->GetType();
		unsigned int uiTableID = TableID[i];
		for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);
			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{
				for (unsigned int k = 0; k < ObjectTable[uiTableID].m_ObjectPropertyTable.GetNum(); k++)
				{
					if (pProperty->GetName().GetNameCode() == ObjectTable[uiTableID].m_ObjectPropertyTable[k].m_uiNameID)
					{
						m_pcCurBufPtr = m_pcBuffer + ObjectTable[uiTableID].m_ObjectPropertyTable[k].m_uiOffSet;
						pProperty->Archive(*this, m_pVObjectArray[i]);
						break;
					}
				}
			}
		}
	}
	
	//处理连接
	m_uiStreamFlag = AT_LINK;
	//必须从后往前便利，因为register的过程是递归的深度注册，所以从后往前link保证子节点先完成，然后父亲节点完成。 
	for(int i = m_pVObjectArray.GetNum() - 1; i >= 0 ; i--)
	{

		VSRtti &Rtti = m_pVObjectArray[i]->GetType();
		for (unsigned int j = 0 ; j < Rtti.GetPropertyNum() ; j++)
		{	
			VSProperty * pProperty = Rtti.GetProperty(j);
			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{
				pProperty->Archive(*this,m_pVObjectArray[i]);
			}
		}
	}

// 	m_uiStreamFlag = AT_POSTLOAD;
// 	for(unsigned int i = 0 ; i < m_pVObjectArray.GetNum();i++)
// 	{
// 		ArchiveAll(m_pVObjectArray[i]);
// 	}

	m_pcBuffer = NULL;
	return 1;
}
bool VSStream::NewLoad(const TCHAR * const pcFileName)
{
	VSFile *pFile = VS_NEW VSFile();
	if (!pFile)
	{
		return 0;
	}

	//打开文件
	if(!pFile->Open(pcFileName,VSFile::OM_RB))
	{
		VSMAC_DELETE(pFile);
		return 0;
	}

	//都加载到内存中
	unsigned int uiBufferSize = pFile->GetFileSize();
	unsigned char * pBuffer = VS_NEW unsigned char[uiBufferSize];
	if(!pBuffer)
	{
		VSMAC_DELETE(pFile);
		return 0;
	}
	if(!pFile->Read(pBuffer,uiBufferSize,1))
	{
		VSMAC_DELETE(pFile);
		return 0;
	}
	VSMAC_DELETE(pFile);

	if(!NewLoadFromBuffer(pBuffer,uiBufferSize))
	{
		VSMAC_DELETEA(pBuffer);
		return false;
	}
	VSMAC_DELETEA(pBuffer);

	return 1;
}
bool VSStream::NewSave(const TCHAR * const pcFileName)
{
	

	unsigned int iObjectNum = m_pVObjectArray.GetNum();
	m_uiBufferSize = 0;

	//版本号
	m_uiBufferSize += sizeof(unsigned int );
	//物体个数空间
	m_uiBufferSize += sizeof(unsigned int);

	VSArray<ObjectTabel> ObjectTable;
	ObjectTable.SetBufferNum(iObjectNum);
	// obejct table
	for(unsigned int i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{
		//addr
		m_uiBufferSize += sizeof(VSObject*);
		
		ObjectTable[i].m_pAddr = m_pVObjectArray[i];

		//rtti name
		m_uiBufferSize += GetStrDistUse(m_pVObjectArray[i]->GetType().GetName());

		ObjectTable[i].m_RttiName = m_pVObjectArray[i]->GetType().GetName();

		// object property size
		m_uiBufferSize += sizeof(unsigned int);

		// object property num
		m_uiBufferSize += sizeof(unsigned int);

	}
	
	unsigned int uiObjectContentAddr = m_uiBufferSize;

	m_uiStreamFlag = AT_SIZE;
	//object property size 
	for(unsigned int i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{
		ObjectTable[i].m_uiOffSet = m_uiBufferSize;

		VSRtti &Rtti = m_pVObjectArray[i]->GetType();
		ObjectTable[i].m_ObjectPropertyTable.SetBufferNum(Rtti.GetPropertyNum());

		ObjectTable[i].m_uiObjectPropertyNum = 0;

		for (unsigned int j = 0 ; j < Rtti.GetPropertyNum() ; j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);
			
			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{
				//name
				m_uiBufferSize += sizeof(unsigned int);
				ObjectTable[i].m_ObjectPropertyTable[j].m_PropertyName = pProperty->GetName().GetString();
				ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID = pProperty->GetName().GetNameCode();
				//offset
				m_uiBufferSize += sizeof(unsigned int);

				ObjectTable[i].m_uiObjectPropertyNum++;
			}
			

		}

		ObjectTable[i].m_uiObjectPropertyTableSize = m_uiBufferSize - ObjectTable[i].m_uiOffSet;

		for (unsigned int j = 0 ; j < Rtti.GetPropertyNum() ; j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);

			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{
				ObjectTable[i].m_ObjectPropertyTable[j].m_uiOffSet = m_uiBufferSize;

				m_uiArchivePropertySize = 0;

				pProperty->Archive(*this,m_pVObjectArray[i]);

				ObjectTable[i].m_ObjectPropertyTable[j].m_uiSize = m_uiArchivePropertySize;

				m_uiBufferSize += m_uiArchivePropertySize;
			}
		}

		ObjectTable[i].m_uiObjectPropertySize = m_uiBufferSize - ObjectTable[i].m_uiObjectPropertyTableSize - ObjectTable[i].m_uiOffSet;
	}

	for(unsigned int i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{
		m_pVObjectArray[i]->BeforeSave();
	}
	//申请空间
	VSMAC_DELETEA(m_pcBuffer);
	m_pcBuffer = VS_NEW unsigned char[m_uiBufferSize];	
	if(!m_pcBuffer)
	{
		return 0;
	}
	m_pcCurBufPtr = m_pcBuffer;

	m_uiVerson = ms_uiCurVerson;
	//存储版本号
	Write(&m_uiVerson,sizeof(unsigned int));

	//存储物体个数
	Write(&iObjectNum,sizeof(unsigned int));


	// OBJECT TABLE
	for(unsigned int i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{
		//addr
		if(!Write(&ObjectTable[i].m_pAddr,sizeof(VSObject *)))
		{
			VSMAC_DELETEA(m_pcBuffer);
			return 0;
		}

		//rtti name
		if(!WriteString(ObjectTable[i].m_RttiName))
		{
			VSMAC_DELETEA(m_pcBuffer);
			return 0;
		}

		Write(&ObjectTable[i].m_uiObjectPropertySize,sizeof(unsigned int));

		Write(&ObjectTable[i].m_uiObjectPropertyNum,sizeof(unsigned int));

	}

	//OBJECT PROPERTY 
	m_uiStreamFlag = AT_SAVE;
	for(unsigned int i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{

		VSRtti &Rtti = m_pVObjectArray[i]->GetType();
		for (unsigned int j = 0 ; j < Rtti.GetPropertyNum() ; j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);

			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{

				Write(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID,sizeof(unsigned int));
				Write(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiOffSet,sizeof(unsigned int));
			}
			
		}

		for (unsigned int j = 0 ; j < Rtti.GetPropertyNum() ; j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);
			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{
				pProperty->Archive(*this,m_pVObjectArray[i]);
			}
		}
	}

	for(unsigned int i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{
		m_pVObjectArray[i]->PostSave();
	}

	VSFile *pFile = VS_NEW VSFile();
	if (!pFile)
	{
		VSMAC_DELETEA(m_pcBuffer);
		return 0;
	}
	//打开文件
	if(!pFile->Open(pcFileName,VSFile::OM_WB))
	{
		VSMAC_DELETE(pFile);
		VSMAC_DELETEA(m_pcBuffer);
		return 0;
	}

	if(!pFile->Write(m_pcBuffer,m_uiBufferSize,1))
	{
		VSMAC_DELETE(pFile);
		VSMAC_DELETEA(m_pcBuffer);
		return 0;
	}
	VSMAC_DELETE(pFile);
	VSMAC_DELETEA(m_pcBuffer);
	return 1;
}