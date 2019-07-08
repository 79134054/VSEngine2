#ifndef VSCUSTOMARCHIVEOBJECT_H
#define VSCUSTOMARCHIVEOBJECT_H
#include "VSRtti.h"
#include "VSType.h"
#include "VSMap.h"
namespace VSEngine2
{
	class VSStream;
	class VSEditorElement;
	//��֧��ָ�����л�����֧��postload �� postclone
	//���Ա�����VSType ��֧�ֵ�
	class VSGRAPHIC_API VSCustomArchiveObject:public VSMemObject
	{
	public:
		VSCustomArchiveObject();
		virtual ~VSCustomArchiveObject();
		virtual void Archive(VSStream & Stream) = 0;
		virtual void CopyFrom(VSCustomArchiveObject *,VSMap<VSObject *,VSObject*>& CloneMap) = 0;
		virtual VSEditorElement *  CreateEElement(VSString& Name,VSObject * pOwner);
	};
	CUSTOMTYPE_MARCO(VSCustomArchiveObject)
}
#endif