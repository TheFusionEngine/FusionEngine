#ifndef FUNC_REF_H
#define FUNC_REF_H

#include "reference.h"

class Callable : public RefCounted{

	OBJ_TYPE(Callable,RefCounted);
	ObjectID id;
	StringName function;

protected:

	static void _bind_methods();
public:

	Variant call_func(const Variant** p_args, int p_argcount, Variant::CallError& r_error);
	void set_instance(Object *p_obj);
	void set_function(const StringName& p_func);
	Callable();
};

#endif // FUNC_REF_H
