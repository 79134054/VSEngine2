#ifndef VSTYPE_H
#define VSTYPE_H
#include "VSPointer.h"
#include "VSString.h"

//一般内部使用的从object类继承的，是要区分属性的，所以声明rtti来区分属性，VSTYPE_MARCO
//来区分是内部从object类继承的，支持以属性为单位来实现序列化

//自定义类型，如果想要支持自定义的序列化，要用CUSTOMTYPE_MARCO  声明，不区分属性，
//里面类的属性版本兼容自己要制定。
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