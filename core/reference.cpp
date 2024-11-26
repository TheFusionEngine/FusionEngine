/*************************************************************************/
/*  reference.cpp                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "reference.h"



bool RefCounted::init_ref() {

	if (refcount.ref()) {

		// this may fail in the scenario of two threads assigning the pointer for the FIRST TIME
		// at the same time, which is never likely to happen (would be crazy to do)
		// so don't do it.

		if (refcount_init.get()>0) {
			refcount_init.unref();
			refcount.unref(); // first referencing is already 1, so compensate for the ref above
		}

		return true;
	} else {

		return false;
	}

}


void RefCounted::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("init_ref"),&RefCounted::init_ref);
	ObjectTypeDB::bind_method(_MD("reference"),&RefCounted::reference);
	ObjectTypeDB::bind_method(_MD("unreference"),&RefCounted::unreference);
}

void RefCounted::reference(){

	refcount.ref();

}
bool RefCounted::unreference(){

	return refcount.unref();
}

RefCounted::RefCounted() {

	refcount.init();
	refcount_init.init();
}


RefCounted::~RefCounted() {

}

Variant WeakRef::get_ref() const {

	if (ref==0)
		return Variant();

	Object *obj = ObjectDB::get_instance(ref);
	if (!obj)
		return Variant();
	RefCounted *r = obj->cast_to<RefCounted>();
	if (r) {

		return REF(r);
	}

	return obj;
}

void WeakRef::set_obj(Object *p_object) {
	ref=p_object ? p_object->get_instance_ID() : 0;
}

void WeakRef::set_ref(const REF& p_ref) {

	ref=p_ref.is_valid() ? p_ref->get_instance_ID() : 0;
}

WeakRef::WeakRef() {
	ref=0;
}

void WeakRef::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("get_ref:Object"),&WeakRef::get_ref);
}
#if 0

RefCounted * RefBase::get_reference_from_ref(const RefBase &p_base) {

	return p_base.get_reference();
}
void RefBase::ref_inc(RefCounted *p_reference) {

	p_reference->refcount.ref();
}
bool RefBase::ref_dec(RefCounted *p_reference) {

	bool ref = p_reference->refcount.unref();
	return ref;
}

RefCounted *RefBase::first_ref(RefCounted *p_reference) {

	if (p_reference->refcount.ref()) {

		// this may fail in the scenario of two threads assigning the pointer for the FIRST TIME
		// at the same time, which is never likely to happen (would be crazy to do)
		// so don't do it.

		if (p_reference->refcount_init.get()>0) {
			p_reference->refcount_init.unref();
			p_reference->refcount.unref(); // first referencing is already 1, so compensate for the ref above
		}

		return p_reference;
	} else {

		return 0;
	}

}
char * RefBase::get_refptr_data(const RefPtr &p_refptr) const {

	return p_refptr.data;
}
#endif
