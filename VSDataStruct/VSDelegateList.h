#ifndef VSDELEGATELIST_H
#define VSDELEGATELIST_H
#include "VSSystem.h"
#include "VSArray.h"
namespace VSEngine2
{
	#define DELEGATE_PREFERRED_SYNTAX
	//注	根据参数个数不同，对应每个个数都要编译出来一个类，如果打开这个宏编译出来的所有类名可以相同，
	//		同时声明的方法和传统的模板声明不同，否则编译器无法区分类名相同的类
	//		如果不打开这个宏，类名就不同否则编译不过，但声明和传统声明一样
	//例如 ：对于一个参数int 和返回类型是int的函数来说，打开这个宏，
	//		VSDelegate<int(int)> k; 这种方法存在编译器兼容性。
	//		不打开这个宏
	//		VSDelegate1<int,int> ; 后面的数字表示函数参数个数
	#ifdef DELEGATE_PREFERRED_SYNTAX
		template <typename TSignature> class VSDelegate;
		template <typename TSignature> class VSDelegateEvent;
	#endif

	#define DELEGATE_PARAM_COUNT	0
	#define DELEGATE_TEMPLATE_TYPE
	#define DELEGATE_TYPE
	#define DELEGATE_TYPE_VALUE		
	#define DELEGATE_VALUE
	#include "VSDelegateTemplate.h"
	#undef DELEGATE_PARAM_COUNT
	#undef DELEGATE_TEMPLATE_TYPE
	#undef DELEGATE_TYPE
	#undef DELEGATE_TYPE_VALUE		
	#undef DELEGATE_VALUE

	#define DELEGATE_PARAM_COUNT	1	
	#define DELEGATE_TEMPLATE_TYPE	typename A1
	#define DELEGATE_TYPE			A1
	#define DELEGATE_TYPE_VALUE		A1 a1
	#define DELEGATE_VALUE			a1
	#include "VSDelegateTemplate.h"
	#undef DELEGATE_PARAM_COUNT
	#undef DELEGATE_TEMPLATE_TYPE
	#undef DELEGATE_TYPE
	#undef DELEGATE_TYPE_VALUE		
	#undef DELEGATE_VALUE

	#define DELEGATE_PARAM_COUNT	2	
	#define DELEGATE_TEMPLATE_TYPE	typename A1,typename A2
	#define DELEGATE_TYPE			A1,A2
	#define DELEGATE_TYPE_VALUE		A1 a1,A2 a2
	#define DELEGATE_VALUE			a1,a2
	#include "VSDelegateTemplate.h"
	#undef DELEGATE_PARAM_COUNT
	#undef DELEGATE_TEMPLATE_TYPE
	#undef DELEGATE_TYPE
	#undef DELEGATE_TYPE_VALUE		
	#undef DELEGATE_VALUE


	#define DELEGATE_PARAM_COUNT	3	
	#define DELEGATE_TEMPLATE_TYPE	typename A1,typename A2,typename A3
	#define DELEGATE_TYPE			A1,A2,A3
	#define DELEGATE_TYPE_VALUE		A1 a1,A2 a2,A3 a3
	#define DELEGATE_VALUE			a1,a2,a3
	#include "VSDelegateTemplate.h"
	#undef DELEGATE_PARAM_COUNT
	#undef DELEGATE_TEMPLATE_TYPE
	#undef DELEGATE_TYPE
	#undef DELEGATE_TYPE_VALUE		
	#undef DELEGATE_VALUE
}



#endif