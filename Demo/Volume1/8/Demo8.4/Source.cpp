//展示RTTI用来判断继承关系
#include <VSObject.h>
#include <iostream>
using namespace VSEngine2;
class A
{
public:
	//声明类A的RTTI
	virtual VSRtti & GetType()const{ return ms_Type; }
	static VSRtti ms_Type;

	bool IsSameType(const A *pObject)const;
	bool IsDerived(const A *pObject)const;
	bool IsSameType(const VSRtti &Type)const;
	bool IsDerived(const VSRtti &Type)const;
};
//定义类A的RTTI
VSRtti A::ms_Type(_T("A"), NULL, NULL);
bool A::IsSameType(const A *pObject)const
{
	return pObject && GetType().IsSameType(pObject->GetType());
}
bool A::IsDerived(const A *pObject)const
{
	return pObject && GetType().IsDerived(pObject->GetType());
}
bool A::IsSameType(const VSRtti &Type)const
{
	return GetType().IsSameType(Type);
}
bool A::IsDerived(const VSRtti &Type)const
{
	return GetType().IsDerived(Type);
}
class B : public A
{
public:
	//声明类B的RTTI
	virtual VSRtti & GetType()const{ return ms_Type; }
	static VSRtti ms_Type;
};
//定义类B的RTTI
VSRtti B::ms_Type(_T("A"), &A::ms_Type, NULL);
class C : public A
{
public:
	//声明类C的RTTI
	virtual VSRtti & GetType()const{ return ms_Type; }
	static VSRtti ms_Type;
};
//定义类C的RTTI
VSRtti C::ms_Type(_T("C"), &A::ms_Type, NULL);
template <class T>
T* StaticCast(A* pkObj)
{
	return (T*)pkObj;
}
template <class T>
const T* StaticCast(const A* pkObj)
{
	return (const T*)pkObj;
}
template<class T>
T * DynamicCast(A * pObj)
{
	return pObj && pObj->IsDerived(T::ms_Type) ? (T*)pObj : 0;
}
template<class T>
const T * DynamicCast(const A * pObj)
{
	return pObj && pObj->IsDerived(T::ms_Type) ? (const T*)pObj : 0;
}

void main()
{
 	VSInitSystem();
 	VSInitMath();
	VSMain::Initialize();
	A* p1 = VS_NEW A();
	A* p2 = VS_NEW B();
	A* p3 = VS_NEW C();
	p2->IsDerived(p1);
	p3->IsDerived(p2);
	B* b1 = DynamicCast<B>(p2);
	if (b1 != NULL)
	{
		// do something
		std::cout << "b1 is not Null" << std::endl;
	}
	else
	{
		std::cout << "b1 is Null" << std::endl;
	}
	C* b2 = DynamicCast<C>(p2);
	if (b2 != NULL)
	{
		std::cout << "b2 is not Null" << std::endl;
		// do something
	}
	else
	{
		std::cout << "b1 is Null" << std::endl;
	}
	VS_DELETE p1;
	VS_DELETE p2;
	VS_DELETE p3;
 	VSMain::Terminate();
}
