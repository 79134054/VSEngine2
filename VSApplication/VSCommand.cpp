#include "VSCommand.h"
using namespace VSEngine2;
//----------------------------------------------------------------------------
VSCommand::VSCommand (int numArguments, TCHAR** arguments)
{
	for (int i = 0; i < numArguments; ++i)
	{
		VSString Command(arguments[i]);
		VSString Value;
		if (Value.GetString(Command, '=', 1, false))
		{
			VSString Key;
			Key.GetString(Command, '=', 1);
			m_CommandMap.AddElement(Key, Value);
		}
		else
		{
			m_CommandMap.AddElement(Command, Value);
		}
	}
}
//----------------------------------------------------------------------------
VSCommand::VSCommand (TCHAR* commandLine)
{

	if (!commandLine)
	{
		return;
	}

	TCHAR * pNextToken = NULL;
	TCHAR* token = VSStrtok(commandLine, " ",&pNextToken);
	while (token)
	{
		VSString Command(token);
		VSString Value;
		if (Command.GetString(Value, '=', 1, false))
		{
			VSString Key;
			Command.GetString(Key, '=', 1);
			m_CommandMap.AddElement(Key, Value);
		}
		else
		{
			m_CommandMap.AddElement(Command, Value);
		}

		token = VSStrtok(NULL, " ",&pNextToken);
	}


}
//----------------------------------------------------------------------------
VSCommand::~VSCommand ()
{

}
//----------------------------------------------------------------------------
bool VSCommand::GetName(const TCHAR* name)
{

	if (m_CommandMap.Find(name) != m_CommandMap.GetNum())
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool VSCommand::GetInteger (const TCHAR* name, int& value)
{
	unsigned int i = m_CommandMap.Find(name);
	if (i != m_CommandMap.GetNum())
	{
		value = StringToInt(m_CommandMap[i].Value);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool VSCommand::GetReal(const TCHAR* name, VSREAL& value)
{
	unsigned int i = m_CommandMap.Find(name);
	if (i != m_CommandMap.GetNum())
	{
		value = StringToReal(m_CommandMap[i].Value);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool VSCommand::GetString (const TCHAR* name, VSString & value)
{
	unsigned int i = m_CommandMap.Find(name);
	if (i != m_CommandMap.GetNum())
	{
		value = m_CommandMap[i].Value;
		return true;
	}
	
	return false;
}
