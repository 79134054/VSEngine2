/********************************************************************************/
#define DECLARE_PRIORITY \
public:\
	static  VSPriority ms_Priority;
	
/********************************************************************************/

#define IMPLEMENT_PRIORITY(classname)\
	VSPriority classname::ms_Priority;

/********************************************************************************/
#define ADD_PRIORITY(classname)\
	if(!ms_Priority.AddPriorityThan(&classname::ms_Priority))\
		return 0;