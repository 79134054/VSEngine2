#ifndef VSCOMMAND_H
#define VSCOMMAND_H
#include "VSMath.h"
#include "VSString.h"
#include "VSMap.h"
#include "VSMemManager.h"
#include "VSSystem.h"
namespace VSEngine2
{
	class VSCommand : public VSMemObject
	{
	public:

		VSCommand (int numArguments, TCHAR** arguments);
		VSCommand (TCHAR* commandLine);
		~VSCommand ();

		bool GetName(const TCHAR* name); 
		bool GetInteger (const TCHAR* name, int& value);
		bool GetReal (const TCHAR* name, VSREAL& value);
		bool GetString (const TCHAR* name, VSString & value);


	protected:

		VSMap<VSString, VSString> m_CommandMap;
	};
}
#endif