#ifndef VSREFERENCE_H
#define VSREFERENCE_H
#include "VSGraphic.h"
#include "VSMemManager.h"
#include "VSPointer.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSReference
	{
	public:
		template <class T>
		friend class VSPointer;
		friend class VSVariant;
		VSReference();
		virtual ~VSReference() = 0;
		//SmartPointer
	public:
		int GetRef()const { return m_iReference;}
	protected:
		void IncreRef()
		{
			VSLockedIncrement((long *)&m_iReference);
			//m_iReference++;
		}
		void DecreRef()
		{
			//m_iReference--; 
			VSLockedDecrement((long *)&m_iReference);
			if(!m_iReference) 
				VS_DELETE this;
		}
		int m_iReference;
	
	
	};

}
#endif