#ifndef VSLOGIC_H
#define VSLOGIC_H
#include "VSMemManager.h"
#include "VSName.h"
namespace VSEngine2
{
	struct VSLOGIC_INPUT_EVENT 
	{
		unsigned int uiInputType;
		unsigned int uiEvent;
		unsigned int uiKey;
		int x;
		int y;
		int z;
	};
	struct VSMessage
	{
		VSMessage()
		{

		}
		~VSMessage()
		{

		}
		VSUsedName Sender;
		VSUsedName Receiver;
		unsigned int MessageType;
		double DispatchTime;
		void * pData;
		unsigned int uiDataSize;
	};
}

#endif