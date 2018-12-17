#ifndef VSMAIN_H
#define VSMAIN_H
#include "VSSystem.h"
#include "VSPriority.h"
#include "VSArray.h"
namespace VSEngine2
{
	typedef bool (*Function)();
	class VSRtti ;
	typedef bool (*FunctionProperty)(VSRtti *);
	class VSGRAPHIC_API VSMain
	{
	public:
		
		static void AddInitialPropertyFuntion(FunctionProperty Func);
		static void AddTerminalPropertyFuntion(Function Func);
		static void AddInitialFuntion(Function Func);
		static void AddInitialFuntion(Function Func,VSPriority *pPriority);
		static bool Initialize();
		static void AddTerminalFuntion(Function Func);
		static void AddTerminalFuntion(Function Func,VSPriority *pPriority);
		static bool Terminate();
	private:
		VSMain();
		~VSMain();
		struct Element
		{
		public:
			Element()
			{
				Func = NULL;
				pPriority = NULL;
			}
			~Element()
			{
				Func = NULL;
				pPriority = NULL;
			}
			Function		Func;
			VSPriority *	pPriority;
			bool operator > (const Element& e)const
			{
				static VSPriority Pr1;
				static VSPriority Pr2;

				VSPriority *p1 = NULL;
				VSPriority *p2 = NULL;
				if(pPriority)
					p1 = pPriority;
				else
					p1 = &Pr1;
				if(e.pPriority)
					p2 = e.pPriority;
				else
					p2 = &Pr2;
				return (*p1) > (*p2);
			}
			bool operator < (const Element& e)const
			{
				static VSPriority Pr1;
				static VSPriority Pr2;

				VSPriority *p1 = NULL;
				VSPriority *p2 = NULL;
				if(pPriority)
					p1 = pPriority;
				else
					p1 = &Pr1;
				if(e.pPriority)
					p2 = e.pPriority;
				else
					p2 = &Pr2;
				return (*p1) > (*p2);
			}
			bool operator == (const Element& e)const
			{
				static VSPriority Pr1;
				static VSPriority Pr2;

				VSPriority *p1 = NULL;
				VSPriority *p2 = NULL;
				if(pPriority)
					p1 = pPriority;
				else
					p1 = &Pr1;
				if(e.pPriority)
					p2 = e.pPriority;
				else
					p2 = &Pr2;
				return (*p1) == (*p2);
			}
		};
		static VSArray<Element> * ms_pInitialArray;
		static VSArray<FunctionProperty> * ms_pInitialPropertyArray;
		static VSArray<Function> * ms_pTerminalPropertyArray;
		static VSArray<Element> * ms_pTerminalArray;
		static unsigned int ms_uiInitialObject;
		static unsigned int ms_uiTerminalObject;
		
		class PriorityCompare
		{
		public:
			FORCEINLINE bool operator()(Element & e1,Element& e2)
			{
				static VSPriority Pr1;
				static VSPriority Pr2;

				VSPriority *p1 = NULL;
				VSPriority *p2 = NULL;
				if(e1.pPriority)
					p1 = e1.pPriority;
				else
					p1 = &Pr1;
				if(e2.pPriority)
					p2 = e2.pPriority;
				else
					p2 = &Pr2;
				return (*p1) <= (*p2);
			
			}
		
		};
	};
}
#endif