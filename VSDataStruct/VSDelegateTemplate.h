#if DELEGATE_PARAM_COUNT > 0
#define DELEGATE_SEPARATOR ,
#else
#define DELEGATE_SEPARATOR
#endif
#define DELEGATE_DO_JOIN(X,Y) X##Y
#define	DELEGATE_JOIN_MACRO(X,Y) DELEGATE_DO_JOIN(X,Y)


#ifdef DELEGATE_PREFERRED_SYNTAX
	#define DELEGATE_CLASS_NAME VSDelegate
	#define EVENT_CLASS_NAME	VSDelegateEvent
#else
	#define DELEGATE_CLASS_NAME DELEGATE_JOIN_MACRO(VSDelegate,DELEGATE_PARAM_COUNT)
	#define EVENT_CLASS_NAME DELEGATE_JOIN_MACRO(VSDelegateEvent,DELEGATE_PARAM_COUNT)
#endif

template<class RETUREN_TYPE DELEGATE_SEPARATOR DELEGATE_TEMPLATE_TYPE>
#ifdef DELEGATE_PREFERRED_SYNTAX
class DELEGATE_CLASS_NAME<RETUREN_TYPE(DELEGATE_TYPE)>
#else
class DELEGATE_CLASS_NAME
#endif
{
public:
	DELEGATE_CLASS_NAME()
	{
		m_p = NULL;
		m_F = NULL;
	}
	~DELEGATE_CLASS_NAME()
	{

	}
protected:
	typedef RETUREN_TYPE(* F)(void * p DELEGATE_SEPARATOR DELEGATE_TYPE);

	void * m_p;
	F m_F;



	template<class T,RETUREN_TYPE (T::*FunName)(DELEGATE_TYPE)>
	static RETUREN_TYPE MethodStub(void * p DELEGATE_SEPARATOR DELEGATE_TYPE_VALUE)
	{
		T * Ap = (T *)p;
		return (Ap->*FunName)(DELEGATE_VALUE);
	}

	template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TYPE)const>
	static RETUREN_TYPE ConstMethodStub(void * p DELEGATE_SEPARATOR DELEGATE_TYPE_VALUE)
	{
		T * Ap = (T *)p;
		return (Ap->*FunName)(DELEGATE_VALUE);
	}

	template<RETUREN_TYPE(*FunName)(DELEGATE_TYPE)>
	static RETUREN_TYPE FunStub(void * p DELEGATE_SEPARATOR DELEGATE_TYPE_VALUE)
	{
		return (FunName)(DELEGATE_VALUE);
	}

	static DELEGATE_CLASS_NAME Create(void * p , F f)
	{
		DELEGATE_CLASS_NAME Temp;
		Temp.m_F = f;
		Temp.m_p = p;
		return Temp;
	}
public:
	template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TYPE)const>
	static DELEGATE_CLASS_NAME FromMethod(T * p)
	{
		return Create((void*)p, &ConstMethodStub<T, FunName>);
	}

	template<class T,RETUREN_TYPE (T::*FunName)(DELEGATE_TYPE)>
	static DELEGATE_CLASS_NAME FromMethod( T * p)
	{
		return Create((void*)p, &MethodStub<T,FunName>);
	}

	template<RETUREN_TYPE(*FunName)(DELEGATE_TYPE)>
	static DELEGATE_CLASS_NAME FromFun()
	{
		return Create(NULL, &FunStub<FunName>);
	}



	RETUREN_TYPE Excute(DELEGATE_TYPE_VALUE)
	{
		return (*m_F)(m_p DELEGATE_SEPARATOR DELEGATE_VALUE);
	}
	RETUREN_TYPE operator()(DELEGATE_TYPE_VALUE) const
	{
		return (*m_F)(m_p DELEGATE_SEPARATOR DELEGATE_VALUE);
	}

	operator bool() const
	{
		return m_F != NULL;
	}

	bool operator!() const
	{
		return !(operator bool());
	}

	bool operator==(const DELEGATE_CLASS_NAME& rhs) const
	{
		return (m_p == rhs.m_p && m_F == rhs.m_F);
	}
};
template <class RETUREN_TYPE DELEGATE_SEPARATOR DELEGATE_TEMPLATE_TYPE>
#ifdef DELEGATE_PREFERRED_SYNTAX
class EVENT_CLASS_NAME<RETUREN_TYPE(DELEGATE_TYPE)>
#else
class EVENT_CLASS_NAME
#endif
{
public:
#ifdef DELEGATE_PREFERRED_SYNTAX
	typedef DELEGATE_CLASS_NAME<RETUREN_TYPE(DELEGATE_TEMPLATE_TYPE)> Handler;	
#else
	typedef DELEGATE_CLASS_NAME<RETUREN_TYPE DELEGATE_SEPARATOR DELEGATE_TEMPLATE_TYPE> Handler;
#endif
public:
	EVENT_CLASS_NAME() {}
	
	void operator+=(const Handler& handler)
	{
		this->Add(handler);
	}
	void Add(const Handler& handler)
	{
		assert(!this->Has(handler));

		m_handlers.AddElement(handler);
	}

	template<RETUREN_TYPE(*FunName)(DELEGATE_TYPE)>
	void AddFun()
	{
		Add(Handler::FromFun<FunName>());
	}

	template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TYPE)const>
	void AddMethod(T * p)
	{
		Add(Handler::FromMethod<T, FunName>(p));
	}

	template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TYPE)>
	void AddMethod(T * p)
	{
		Add(Handler::FromMethod<T, FunName>(p));
	}
	void operator-=(const Handler& handler)
	{
		this->Remove(handler);
	}
	void Remove(const Handler& handler)
	{
		unsigned int i = m_handlers.FindElement(handler);

		if (i < m_handlers.GetNum())
		{
			m_handlers.Erase(i);
		}
	}
	template<RETUREN_TYPE(*FunName)(DELEGATE_TYPE)>
	void RemoveFun()
	{
		Remove(Handler::FromFun<FunName>());
	}

	template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TYPE)const>
	void RemoveMethod(T * p)
	{
		Remove(Handler::FromMethod<T, FunName>(p));
	}

	template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TYPE)>
	void RemoveMethod(T * p)
	{
		Remove(Handler::FromMethod<T, FunName>(p));
	}
	bool Has(const Handler& handler)
	{
		unsigned int i = m_handlers.FindElement(handler);
		return i != m_handlers.GetNum();
	}
	/// ÊÇ·ñÓÐÐ§
	bool IsValid() const
	{
		return m_handlers.GetNum() > 0;
	}
	
	void Reset()
	{
		m_handlers.Clear();
	}

	
	void operator()(DELEGATE_TYPE_VALUE) const
	{
		this->Invoke(DELEGATE_VALUE);
	}
	void Invoke(DELEGATE_TYPE_VALUE) const
	{
		for (unsigned int i = 0; i < m_handlers.GetNum(); ++i)
		{
			m_handlers[i](DELEGATE_VALUE);
		}
	}
	void InvokeWithEmitter(DELEGATE_TYPE_VALUE
#if DELEGATE_PARAM_COUNT > 0
		,
#endif
		const Handler& emitter) const
	{
		for (unsigned i = 0; i < m_handlers.GetNum(); ++i)
		{
			if (m_handlers[i] != emitter)
			{
				m_handlers[i](DELEGATE_VALUE);
			}
		}
	}

private:
	typedef VSArray<Handler> DelegateList;
	DelegateList m_handlers;
};
#undef DELEGATE_SEPARATOR
#undef DELEGATE_DO_JOIN
#undef DELEGATE_CLASS_NAME
#undef EVENT_CLASS_NAME