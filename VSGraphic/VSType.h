#ifndef VSTYPE_H
#define VSTYPE_H
#include "VSPointer.h"
#include "VSString.h"

//һ���ڲ�ʹ�õĴ�object��̳еģ���Ҫ�������Եģ���������rtti���������ԣ�VSTYPE_MARCO
//���������ڲ���object��̳еģ�֧��������Ϊ��λ��ʵ�����л�

//�Զ������ͣ������Ҫ֧���Զ�������л���Ҫ��CUSTOMTYPE_MARCO  ���������������ԣ�
//����������԰汾�����Լ�Ҫ�ƶ���
namespace VSEngine2
{
	//custom type
	template<typename T> struct TIsCustomType									{ enum { Value = false }; };
	template<typename T> struct TIsCustomPointerType							{ enum { Value = false }; };

#define CUSTOMTYPE_MARCO(ClassName) \
	template<>	struct TIsCustomType<ClassName>	{ enum { Value = true }; }; \
	template<>	struct TIsCustomPointerType<ClassName *>	{ enum { Value = true }; };

	// vs type
	template<typename T> struct TIsVSType										{ enum { Value = false }; };

	//native pointer
	template<typename T> struct TIsNativePointerType								{ enum { Value = false }; };
	template<typename T> struct TIsNativePointerType<T*>							{ enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<const T*>						{ enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<const T* const>				{ enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<T* volatile>					{ enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<T* const volatile>				{ enum { Value = true }; };


	//vs pointer
	template<typename T> struct TIsVSPointerType								{ enum { Value = false }; };
	template<typename T> struct TIsVSSmartPointerType							{ enum { Value = false }; };
#define VSTYPE_MARCO(ClassName) \
	template<>	struct TIsVSType<ClassName>										{ enum { Value = true }; }; \
	template<>	struct TIsVSPointerType<ClassName *>							{ enum { Value = true }; }; \
	template<> struct TIsVSSmartPointerType<VSPointer<ClassName>>				{ enum { Value = true }; }; \
	template<> struct TIsVSSmartPointerType<const VSPointer<ClassName>>			{ enum { Value = true }; };

	template<typename T> struct TIsVSEnumType									{ enum { Value = false }; };
#define VSENUMTYPE_MARCO(ClassName) \
	template<>	struct TIsVSEnumType<ClassName>									{ enum { Value = true }; }; 
	
	//resource
	class VSFSM;
	class VSAnimSet;
	class VSMaterial;
	class VSVShader;
	class VSPShader;
	class VSTexAllState;
	class VSSkelectonMeshNode;
	class VSStaticMeshNode;
	class VSResource;
	template<class T>
	class VSResourceProxy;
	template<typename T> struct TIsVSResourceType								{ enum { Value = false}; };
	template<typename T> struct TIsVSResourcePointerType						{ enum { Value = false}; };
	template<typename T> struct TIsVSResourceProxyType							{ enum { Value = false}; };
	template<typename T> struct TIsVSResourceProxyPointType						{ enum { Value = false}; };

	template<typename T> struct TIsVSResourceProxyType<VSResourceProxy<T>>		{ enum { Value = true }; };
	template<typename T> struct TIsVSResourceProxyPointType<VSPointer<VSResourceProxy<T>>>		{ enum { Value = true }; };

// 	template<>	struct TIsVSResourceType<VSResource *>							{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<VSPointer<VSResource>>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<VSFSM *>								{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<VSPointer<VSFSM>>						{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<VSMaterial *>							{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<VSPointer<VSMaterial>>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<VSAnimSet *>							{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<VSPointer<VSAnimSet>>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<VSVShader *>							{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<VSPointer<VSVShader>>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<VSPShader *>							{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<VSPointer<VSPShader>>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<VSTexAllState *>						{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<VSPointer<VSTexAllState>>				{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<VSSkelectonMeshNode *>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<VSPointer<VSSkelectonMeshNode>>		{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<VSStaticMeshNode *>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<VSPointer<VSStaticMeshNode>>			{ enum { Value = true }; };

	// string
	template<typename T> struct TIsVSStringType									{ enum { Value = false}; };
	template<>	struct TIsVSStringType<VSString>								{ enum { Value = true }; };
	

}
#endif