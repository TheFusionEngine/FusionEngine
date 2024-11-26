#include "func_ref.h"

Variant Callable::call_func(const Variant** p_args, int p_argcount, Variant::CallError& r_error) {

	if (id==0) {
		r_error.error=Variant::CallError::CALL_ERROR_INSTANCE_IS_NULL;
		return Variant();
	}
	Object* obj = ObjectDB::get_instance(id);

	if (!obj) {
		r_error.error=Variant::CallError::CALL_ERROR_INSTANCE_IS_NULL;
		return Variant();
	}

	return obj->call(function,p_args,p_argcount,r_error);

}

void Callable::set_instance(Object *p_obj){

	ERR_FAIL_NULL(p_obj);
	id=p_obj->get_instance_ID();
}
void Callable::set_function(const StringName& p_func){

	function=p_func;
}

void Callable::_bind_methods() {

	{
		MethodInfo mi;
		mi.name="call";
		mi.arguments.push_back( PropertyInfo( Variant::STRING, "method"));
		Vector<Variant> defargs;
		for(int i=0;i<10;i++) {
			mi.arguments.push_back( PropertyInfo( Variant::NIL, "arg"+itos(i)));
			defargs.push_back(Variant());
		}
		ObjectTypeDB::bind_native_method(METHOD_FLAGS_DEFAULT,"call_func",&Callable::call_func,mi,defargs);

	}

	ObjectTypeDB::bind_method(_MD("set_instance","instance"),&Callable::set_instance);
	ObjectTypeDB::bind_method(_MD("set_function","name"),&Callable::set_function);

}


Callable::Callable(){

	id=0;
}

