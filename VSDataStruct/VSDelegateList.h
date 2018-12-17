#ifndef VSDELEGATELIST_H
#define VSDELEGATELIST_H
#include "VSSystem.h"
#include "VSArray.h"
namespace VSEngine2
{
	#define DELEGATE_PREFERRED_SYNTAX
	//ע	���ݲ���������ͬ����Ӧÿ��������Ҫ�������һ���࣬��������������������������������ͬ��
	//		ͬʱ�����ķ����ʹ�ͳ��ģ��������ͬ������������޷�����������ͬ����
	//		�����������꣬�����Ͳ�ͬ������벻�����������ʹ�ͳ����һ��
	//���� ������һ������int �ͷ���������int�ĺ�����˵��������꣬
	//		VSDelegate<int(int)> k; ���ַ������ڱ����������ԡ�
	//		���������
	//		VSDelegate1<int,int> ; ��������ֱ�ʾ������������
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