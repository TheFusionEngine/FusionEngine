
#ifndef TYPED_METHOD_BIND
template<   class P1, class P2, class P3, class P4, class P5, class P6 >
class MethodBind6 : public MethodBind {

public:

	StringName type_name;
	 void (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6) ;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6));
		
		return Variant();
	}

	MethodBind6 () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(6);
#else
		set_argument_count(6);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6)  ) {

	MethodBind6 <    P1, P2, P3, P4, P5, P6 > * a = memnew( (MethodBind6 <   P1, P2, P3, P4, P5, P6 >) );
	union {

		 void (T::*sm)(P1, P2, P3, P4, P5, P6) ;
		 void (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6) ;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T  , class P1, class P2, class P3, class P4, class P5, class P6>
class MethodBind6 : public MethodBind {
public:
	
	 void (T::*method)(P1, P2, P3, P4, P5, P6) ;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6));
		
		return Variant();
	}
	

	MethodBind6 () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(6);
#else
		set_argument_count(6);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6)  ) {

	MethodBind6<T  , P1, P2, P3, P4, P5, P6> * a = memnew( (MethodBind6<T  , P1, P2, P3, P4, P5, P6>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template< class R , class P1, class P2, class P3, class P4, class P5, class P6 >
class MethodBind6R : public MethodBind {

public:

	StringName type_name;
	R  (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6) ;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6));
		return Variant(ret);
		
	}

	MethodBind6R () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(6);
#else
		set_argument_count(6);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6)  ) {

	MethodBind6R <  R , P1, P2, P3, P4, P5, P6 > * a = memnew( (MethodBind6R < R , P1, P2, P3, P4, P5, P6 >) );
	union {

		R  (T::*sm)(P1, P2, P3, P4, P5, P6) ;
		R  (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6) ;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6>
class MethodBind6R : public MethodBind {
public:
	
	R  (T::*method)(P1, P2, P3, P4, P5, P6) ;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6));
		return Variant(ret);
		
	}
	

	MethodBind6R () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(6);
#else
		set_argument_count(6);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6)  ) {

	MethodBind6R<T ,R , P1, P2, P3, P4, P5, P6> * a = memnew( (MethodBind6R<T ,R , P1, P2, P3, P4, P5, P6>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template<   class P1, class P2, class P3, class P4, class P5, class P6 >
class MethodBind6C : public MethodBind {

public:

	StringName type_name;
	 void (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6) const;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6));
		
		return Variant();
	}

	MethodBind6C () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(6);
#else
		set_argument_count(6);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6) const ) {

	MethodBind6C <    P1, P2, P3, P4, P5, P6 > * a = memnew( (MethodBind6C <   P1, P2, P3, P4, P5, P6 >) );
	union {

		 void (T::*sm)(P1, P2, P3, P4, P5, P6) const;
		 void (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6) const;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T  , class P1, class P2, class P3, class P4, class P5, class P6>
class MethodBind6C : public MethodBind {
public:
	
	 void (T::*method)(P1, P2, P3, P4, P5, P6) const;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6));
		
		return Variant();
	}
	

	MethodBind6C () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(6);
#else
		set_argument_count(6);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6) const ) {

	MethodBind6C<T  , P1, P2, P3, P4, P5, P6> * a = memnew( (MethodBind6C<T  , P1, P2, P3, P4, P5, P6>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template< class R , class P1, class P2, class P3, class P4, class P5, class P6 >
class MethodBind6RC : public MethodBind {

public:

	StringName type_name;
	R  (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6) const;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6));
		return Variant(ret);
		
	}

	MethodBind6RC () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(6);
#else
		set_argument_count(6);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6) const ) {

	MethodBind6RC <  R , P1, P2, P3, P4, P5, P6 > * a = memnew( (MethodBind6RC < R , P1, P2, P3, P4, P5, P6 >) );
	union {

		R  (T::*sm)(P1, P2, P3, P4, P5, P6) const;
		R  (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6) const;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6>
class MethodBind6RC : public MethodBind {
public:
	
	R  (T::*method)(P1, P2, P3, P4, P5, P6) const;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6));
		return Variant(ret);
		
	}
	

	MethodBind6RC () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(6);
#else
		set_argument_count(6);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6) const ) {

	MethodBind6RC<T ,R , P1, P2, P3, P4, P5, P6> * a = memnew( (MethodBind6RC<T ,R , P1, P2, P3, P4, P5, P6>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template<   class P1, class P2, class P3, class P4, class P5, class P6, class P7 >
class MethodBind7 : public MethodBind {

public:

	StringName type_name;
	 void (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7) ;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7));
		
		return Variant();
	}

	MethodBind7 () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(7);
#else
		set_argument_count(7);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7)  ) {

	MethodBind7 <    P1, P2, P3, P4, P5, P6, P7 > * a = memnew( (MethodBind7 <   P1, P2, P3, P4, P5, P6, P7 >) );
	union {

		 void (T::*sm)(P1, P2, P3, P4, P5, P6, P7) ;
		 void (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7) ;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
class MethodBind7 : public MethodBind {
public:
	
	 void (T::*method)(P1, P2, P3, P4, P5, P6, P7) ;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7));
		
		return Variant();
	}
	

	MethodBind7 () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(7);
#else
		set_argument_count(7);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7)  ) {

	MethodBind7<T  , P1, P2, P3, P4, P5, P6, P7> * a = memnew( (MethodBind7<T  , P1, P2, P3, P4, P5, P6, P7>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template< class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7 >
class MethodBind7R : public MethodBind {

public:

	StringName type_name;
	R  (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7) ;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7));
		return Variant(ret);
		
	}

	MethodBind7R () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(7);
#else
		set_argument_count(7);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7)  ) {

	MethodBind7R <  R , P1, P2, P3, P4, P5, P6, P7 > * a = memnew( (MethodBind7R < R , P1, P2, P3, P4, P5, P6, P7 >) );
	union {

		R  (T::*sm)(P1, P2, P3, P4, P5, P6, P7) ;
		R  (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7) ;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
class MethodBind7R : public MethodBind {
public:
	
	R  (T::*method)(P1, P2, P3, P4, P5, P6, P7) ;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7));
		return Variant(ret);
		
	}
	

	MethodBind7R () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(7);
#else
		set_argument_count(7);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7)  ) {

	MethodBind7R<T ,R , P1, P2, P3, P4, P5, P6, P7> * a = memnew( (MethodBind7R<T ,R , P1, P2, P3, P4, P5, P6, P7>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template<   class P1, class P2, class P3, class P4, class P5, class P6, class P7 >
class MethodBind7C : public MethodBind {

public:

	StringName type_name;
	 void (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7) const;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7));
		
		return Variant();
	}

	MethodBind7C () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(7);
#else
		set_argument_count(7);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7) const ) {

	MethodBind7C <    P1, P2, P3, P4, P5, P6, P7 > * a = memnew( (MethodBind7C <   P1, P2, P3, P4, P5, P6, P7 >) );
	union {

		 void (T::*sm)(P1, P2, P3, P4, P5, P6, P7) const;
		 void (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7) const;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
class MethodBind7C : public MethodBind {
public:
	
	 void (T::*method)(P1, P2, P3, P4, P5, P6, P7) const;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7));
		
		return Variant();
	}
	

	MethodBind7C () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(7);
#else
		set_argument_count(7);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7) const ) {

	MethodBind7C<T  , P1, P2, P3, P4, P5, P6, P7> * a = memnew( (MethodBind7C<T  , P1, P2, P3, P4, P5, P6, P7>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template< class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7 >
class MethodBind7RC : public MethodBind {

public:

	StringName type_name;
	R  (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7) const;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7));
		return Variant(ret);
		
	}

	MethodBind7RC () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(7);
#else
		set_argument_count(7);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7) const ) {

	MethodBind7RC <  R , P1, P2, P3, P4, P5, P6, P7 > * a = memnew( (MethodBind7RC < R , P1, P2, P3, P4, P5, P6, P7 >) );
	union {

		R  (T::*sm)(P1, P2, P3, P4, P5, P6, P7) const;
		R  (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7) const;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
class MethodBind7RC : public MethodBind {
public:
	
	R  (T::*method)(P1, P2, P3, P4, P5, P6, P7) const;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7));
		return Variant(ret);
		
	}
	

	MethodBind7RC () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(7);
#else
		set_argument_count(7);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7) const ) {

	MethodBind7RC<T ,R , P1, P2, P3, P4, P5, P6, P7> * a = memnew( (MethodBind7RC<T ,R , P1, P2, P3, P4, P5, P6, P7>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template<   class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8 >
class MethodBind8 : public MethodBind {

public:

	StringName type_name;
	 void (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8) ;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8));
		
		return Variant();
	}

	MethodBind8 () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(8);
#else
		set_argument_count(8);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8)  ) {

	MethodBind8 <    P1, P2, P3, P4, P5, P6, P7, P8 > * a = memnew( (MethodBind8 <   P1, P2, P3, P4, P5, P6, P7, P8 >) );
	union {

		 void (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8) ;
		 void (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8) ;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
class MethodBind8 : public MethodBind {
public:
	
	 void (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8) ;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8));
		
		return Variant();
	}
	

	MethodBind8 () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(8);
#else
		set_argument_count(8);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8)  ) {

	MethodBind8<T  , P1, P2, P3, P4, P5, P6, P7, P8> * a = memnew( (MethodBind8<T  , P1, P2, P3, P4, P5, P6, P7, P8>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template< class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8 >
class MethodBind8R : public MethodBind {

public:

	StringName type_name;
	R  (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8) ;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8));
		return Variant(ret);
		
	}

	MethodBind8R () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(8);
#else
		set_argument_count(8);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8)  ) {

	MethodBind8R <  R , P1, P2, P3, P4, P5, P6, P7, P8 > * a = memnew( (MethodBind8R < R , P1, P2, P3, P4, P5, P6, P7, P8 >) );
	union {

		R  (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8) ;
		R  (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8) ;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
class MethodBind8R : public MethodBind {
public:
	
	R  (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8) ;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8));
		return Variant(ret);
		
	}
	

	MethodBind8R () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(8);
#else
		set_argument_count(8);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8)  ) {

	MethodBind8R<T ,R , P1, P2, P3, P4, P5, P6, P7, P8> * a = memnew( (MethodBind8R<T ,R , P1, P2, P3, P4, P5, P6, P7, P8>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template<   class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8 >
class MethodBind8C : public MethodBind {

public:

	StringName type_name;
	 void (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8) const;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8));
		
		return Variant();
	}

	MethodBind8C () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(8);
#else
		set_argument_count(8);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8) const ) {

	MethodBind8C <    P1, P2, P3, P4, P5, P6, P7, P8 > * a = memnew( (MethodBind8C <   P1, P2, P3, P4, P5, P6, P7, P8 >) );
	union {

		 void (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8) const;
		 void (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8) const;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
class MethodBind8C : public MethodBind {
public:
	
	 void (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8) const;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8));
		
		return Variant();
	}
	

	MethodBind8C () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(8);
#else
		set_argument_count(8);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8) const ) {

	MethodBind8C<T  , P1, P2, P3, P4, P5, P6, P7, P8> * a = memnew( (MethodBind8C<T  , P1, P2, P3, P4, P5, P6, P7, P8>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template< class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8 >
class MethodBind8RC : public MethodBind {

public:

	StringName type_name;
	R  (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8) const;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8));
		return Variant(ret);
		
	}

	MethodBind8RC () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(8);
#else
		set_argument_count(8);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8) const ) {

	MethodBind8RC <  R , P1, P2, P3, P4, P5, P6, P7, P8 > * a = memnew( (MethodBind8RC < R , P1, P2, P3, P4, P5, P6, P7, P8 >) );
	union {

		R  (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8) const;
		R  (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8) const;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
class MethodBind8RC : public MethodBind {
public:
	
	R  (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8) const;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8));
		return Variant(ret);
		
	}
	

	MethodBind8RC () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(8);
#else
		set_argument_count(8);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8) const ) {

	MethodBind8RC<T ,R , P1, P2, P3, P4, P5, P6, P7, P8> * a = memnew( (MethodBind8RC<T ,R , P1, P2, P3, P4, P5, P6, P7, P8>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template<   class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9 >
class MethodBind9 : public MethodBind {

public:

	StringName type_name;
	 void (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) ;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9));
		
		return Variant();
	}

	MethodBind9 () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(9);
#else
		set_argument_count(9);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9)  ) {

	MethodBind9 <    P1, P2, P3, P4, P5, P6, P7, P8, P9 > * a = memnew( (MethodBind9 <   P1, P2, P3, P4, P5, P6, P7, P8, P9 >) );
	union {

		 void (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8, P9) ;
		 void (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8, P9) ;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
class MethodBind9 : public MethodBind {
public:
	
	 void (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) ;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9));
		
		return Variant();
	}
	

	MethodBind9 () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(9);
#else
		set_argument_count(9);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9)  ) {

	MethodBind9<T  , P1, P2, P3, P4, P5, P6, P7, P8, P9> * a = memnew( (MethodBind9<T  , P1, P2, P3, P4, P5, P6, P7, P8, P9>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template< class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9 >
class MethodBind9R : public MethodBind {

public:

	StringName type_name;
	R  (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) ;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9));
		return Variant(ret);
		
	}

	MethodBind9R () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(9);
#else
		set_argument_count(9);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9)  ) {

	MethodBind9R <  R , P1, P2, P3, P4, P5, P6, P7, P8, P9 > * a = memnew( (MethodBind9R < R , P1, P2, P3, P4, P5, P6, P7, P8, P9 >) );
	union {

		R  (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8, P9) ;
		R  (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8, P9) ;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
class MethodBind9R : public MethodBind {
public:
	
	R  (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) ;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9));
		return Variant(ret);
		
	}
	

	MethodBind9R () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(9);
#else
		set_argument_count(9);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9)  ) {

	MethodBind9R<T ,R , P1, P2, P3, P4, P5, P6, P7, P8, P9> * a = memnew( (MethodBind9R<T ,R , P1, P2, P3, P4, P5, P6, P7, P8, P9>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template<   class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9 >
class MethodBind9C : public MethodBind {

public:

	StringName type_name;
	 void (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9));
		
		return Variant();
	}

	MethodBind9C () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(9);
#else
		set_argument_count(9);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const ) {

	MethodBind9C <    P1, P2, P3, P4, P5, P6, P7, P8, P9 > * a = memnew( (MethodBind9C <   P1, P2, P3, P4, P5, P6, P7, P8, P9 >) );
	union {

		 void (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const;
		 void (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
class MethodBind9C : public MethodBind {
public:
	
	 void (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9));
		
		return Variant();
	}
	

	MethodBind9C () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(9);
#else
		set_argument_count(9);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const ) {

	MethodBind9C<T  , P1, P2, P3, P4, P5, P6, P7, P8, P9> * a = memnew( (MethodBind9C<T  , P1, P2, P3, P4, P5, P6, P7, P8, P9>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template< class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9 >
class MethodBind9RC : public MethodBind {

public:

	StringName type_name;
	R  (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9));
		return Variant(ret);
		
	}

	MethodBind9RC () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(9);
#else
		set_argument_count(9);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const ) {

	MethodBind9RC <  R , P1, P2, P3, P4, P5, P6, P7, P8, P9 > * a = memnew( (MethodBind9RC < R , P1, P2, P3, P4, P5, P6, P7, P8, P9 >) );
	union {

		R  (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const;
		R  (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
class MethodBind9RC : public MethodBind {
public:
	
	R  (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9));
		return Variant(ret);
		
	}
	

	MethodBind9RC () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(9);
#else
		set_argument_count(9);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const ) {

	MethodBind9RC<T ,R , P1, P2, P3, P4, P5, P6, P7, P8, P9> * a = memnew( (MethodBind9RC<T ,R , P1, P2, P3, P4, P5, P6, P7, P8, P9>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template<   class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10 >
class MethodBind10 : public MethodBind {

public:

	StringName type_name;
	 void (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) ;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		if (p_argument==(10-1)) return Variant::get_type_for<P10>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		CHECK_ARG(10);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9), _VC(10));
		
		return Variant();
	}

	MethodBind10 () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(10);
#else
		set_argument_count(10);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)  ) {

	MethodBind10 <    P1, P2, P3, P4, P5, P6, P7, P8, P9, P10 > * a = memnew( (MethodBind10 <   P1, P2, P3, P4, P5, P6, P7, P8, P9, P10 >) );
	union {

		 void (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) ;
		 void (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) ;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
class MethodBind10 : public MethodBind {
public:
	
	 void (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) ;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		if (p_argument==(10-1)) return Variant::get_type_for<P10>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		CHECK_ARG(10);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9), _VC(10));
		
		return Variant();
	}
	

	MethodBind10 () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(10);
#else
		set_argument_count(10);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)  ) {

	MethodBind10<T  , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10> * a = memnew( (MethodBind10<T  , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template< class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10 >
class MethodBind10R : public MethodBind {

public:

	StringName type_name;
	R  (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) ;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		if (p_argument==(10-1)) return Variant::get_type_for<P10>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		CHECK_ARG(10);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9), _VC(10));
		return Variant(ret);
		
	}

	MethodBind10R () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(10);
#else
		set_argument_count(10);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)  ) {

	MethodBind10R <  R , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10 > * a = memnew( (MethodBind10R < R , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10 >) );
	union {

		R  (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) ;
		R  (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) ;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
class MethodBind10R : public MethodBind {
public:
	
	R  (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) ;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		if (p_argument==(10-1)) return Variant::get_type_for<P10>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		CHECK_ARG(10);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9), _VC(10));
		return Variant(ret);
		
	}
	

	MethodBind10R () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(false);
		_generate_argument_types(10);
#else
		set_argument_count(10);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)  ) {

	MethodBind10R<T ,R , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10> * a = memnew( (MethodBind10R<T ,R , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template<   class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10 >
class MethodBind10C : public MethodBind {

public:

	StringName type_name;
	 void (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		if (p_argument==(10-1)) return Variant::get_type_for<P10>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		CHECK_ARG(10);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9), _VC(10));
		
		return Variant();
	}

	MethodBind10C () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(10);
#else
		set_argument_count(10);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const ) {

	MethodBind10C <    P1, P2, P3, P4, P5, P6, P7, P8, P9, P10 > * a = memnew( (MethodBind10C <   P1, P2, P3, P4, P5, P6, P7, P8, P9, P10 >) );
	union {

		 void (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const;
		 void (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
class MethodBind10C : public MethodBind {
public:
	
	 void (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		if (p_argument==(10-1)) return Variant::get_type_for<P10>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		CHECK_ARG(10);
		
#endif
		(instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9), _VC(10));
		
		return Variant();
	}
	

	MethodBind10C () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(10);
#else
		set_argument_count(10);
#endif		
	};
};

template<class T  , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
MethodBind* create_method_bind( void (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const ) {

	MethodBind10C<T  , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10> * a = memnew( (MethodBind10C<T  , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10>) );	
	a->method=p_method;
	return a;
}
#endif

#ifndef TYPED_METHOD_BIND
template< class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10 >
class MethodBind10RC : public MethodBind {

public:

	StringName type_name;
	R  (__UnexistingClass::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const;

#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }

	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		if (p_argument==(10-1)) return Variant::get_type_for<P10>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return type_name;
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {

		__UnexistingClass *instance = (__UnexistingClass*)p_object;

		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();
		} 
		
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		CHECK_ARG(10);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9), _VC(10));
		return Variant(ret);
		
	}

	MethodBind10RC () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(10);
#else
		set_argument_count(10);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const ) {

	MethodBind10RC <  R , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10 > * a = memnew( (MethodBind10RC < R , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10 >) );
	union {

		R  (T::*sm)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const;
		R  (__UnexistingClass::*dm)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const;
	} u;
	u.sm=p_method;
	a->method=u.dm;
	a->type_name=T::get_type_static();
	return a;
}
#endif

#ifdef TYPED_METHOD_BIND
template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
class MethodBind10RC : public MethodBind {
public:
	
	R  (T::*method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const;
#ifdef DEBUG_METHODS_ENABLED
	virtual Variant::Type _gen_argument_type(int p_arg) const { return _get_argument_type(p_arg); }
	Variant::Type _get_argument_type(int p_argument) const {
		if (p_argument==-1) return Variant::get_type_for<R>();
		if (p_argument==(1-1)) return Variant::get_type_for<P1>();
		if (p_argument==(2-1)) return Variant::get_type_for<P2>();
		if (p_argument==(3-1)) return Variant::get_type_for<P3>();
		if (p_argument==(4-1)) return Variant::get_type_for<P4>();
		if (p_argument==(5-1)) return Variant::get_type_for<P5>();
		if (p_argument==(6-1)) return Variant::get_type_for<P6>();
		if (p_argument==(7-1)) return Variant::get_type_for<P7>();
		if (p_argument==(8-1)) return Variant::get_type_for<P8>();
		if (p_argument==(9-1)) return Variant::get_type_for<P9>();
		if (p_argument==(10-1)) return Variant::get_type_for<P10>();
		
		return Variant::NIL;
	}
#endif	
	virtual String get_instance_type() const {
		return T::get_type_static();
	}

	virtual Variant call(Object* p_object,const Variant** p_args,int p_arg_count, Variant::CallError& r_error) {
	
		T *instance=p_object->cast_to<T>();
		r_error.error=Variant::CallError::CALL_OK;
#ifdef DEBUG_METHODS_ENABLED

		ERR_FAIL_COND_V(!instance,Variant());
		if (p_arg_count>get_argument_count()) {
			r_error.error=Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument=get_argument_count();
			return Variant();

		}
		if (p_arg_count<(get_argument_count()-get_default_argument_count())) {

			r_error.error=Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument=get_argument_count()-get_default_argument_count();
			return Variant();
		}
		CHECK_ARG(1);
		CHECK_ARG(2);
		CHECK_ARG(3);
		CHECK_ARG(4);
		CHECK_ARG(5);
		CHECK_ARG(6);
		CHECK_ARG(7);
		CHECK_ARG(8);
		CHECK_ARG(9);
		CHECK_ARG(10);
		
#endif
		Variant ret = (instance->*method)(_VC(1), _VC(2), _VC(3), _VC(4), _VC(5), _VC(6), _VC(7), _VC(8), _VC(9), _VC(10));
		return Variant(ret);
		
	}
	

	MethodBind10RC () {
#ifdef DEBUG_METHODS_ENABLED
		_set_const(true);
		_generate_argument_types(10);
#else
		set_argument_count(10);
#endif		
	};
};

template<class T ,class R , class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
MethodBind* create_method_bind(R  (T::*p_method)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const ) {

	MethodBind10RC<T ,R , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10> * a = memnew( (MethodBind10RC<T ,R , P1, P2, P3, P4, P5, P6, P7, P8, P9, P10>) );	
	a->method=p_method;
	return a;
}
#endif
