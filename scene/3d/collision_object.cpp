/*************************************************************************/
/*  collision_object.cpp                                                 */
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
#include "collision_object.h"
#include "servers/physics_server.h"
#include "scene/scene_string_names.h"
void CollisionObject3D::_update_shapes_from_children() {

	shapes.resize(0);
	for(int i=0;i<get_child_count();i++) {

		Node* n = get_child(i);
		n->call("_add_to_collision_object",this);
	}

//	_update_shapes();
}

void CollisionObject3D::_notification(int p_what) {

	switch(p_what) {

		case NOTIFICATION_ENTER_WORLD: {

			RID space = get_world()->get_space();
			if (area) {
				PhysicsServer::get_singleton()->area_set_space(rid,space);
			} else
				PhysicsServer::get_singleton()->body_set_space(rid,space);

			_update_pickable();
		//get space
		};

		case NOTIFICATION_TRANSFORM_CHANGED: {

			if (area)
				PhysicsServer::get_singleton()->area_set_transform(rid,get_global_transform());
			else
				PhysicsServer::get_singleton()->body_set_state(rid,PhysicsServer::BODY_STATE_TRANSFORM,get_global_transform());

		} break;
		case NOTIFICATION_VISIBILITY_CHANGED: {

			_update_pickable();

		} break;
		case NOTIFICATION_EXIT_WORLD: {

			if (area) {
				PhysicsServer::get_singleton()->area_set_space(rid,RID());
			} else
				PhysicsServer::get_singleton()->body_set_space(rid,RID());

		} break;
	}
}

void CollisionObject3D::_update_shapes() {

	if (!rid.is_valid())
		return;

	if (area)
		PhysicsServer::get_singleton()->area_clear_shapes(rid);
	else
		PhysicsServer::get_singleton()->body_clear_shapes(rid);

	for(int i=0;i<shapes.size();i++) {

		if (shapes[i].shape.is_null())
			continue;
		if (area)
			PhysicsServer::get_singleton()->area_add_shape(rid,shapes[i].shape->get_rid(),shapes[i].xform);
		else {
			PhysicsServer::get_singleton()->body_add_shape(rid,shapes[i].shape->get_rid(),shapes[i].xform);
			if (shapes[i].trigger)
				PhysicsServer::get_singleton()->body_set_shape_as_trigger(rid,i,shapes[i].trigger);
		}
	}
}


bool CollisionObject3D::_set(const StringName& p_name, const Variant& p_value) {
	String name=p_name;

	if (name=="shape_count") {

		shapes.resize(p_value);
		_update_shapes();
		_change_notify();

	} else if (name.begins_with("shapes/")) {

		int idx=name.get_slice("/",1).to_int();
		String what=name.get_slice("/",2);
		if (what=="shape")
			set_shape(idx,RefPtr(p_value));
		else if (what=="transform")
			set_shape_transform(idx,p_value);
        else if (what=="trigger")
            set_shape_as_trigger(idx,p_value);


	} else
		return false;

	return true;


}

bool CollisionObject3D::_get(const StringName& p_name,Variant &r_ret) const {

	String name=p_name;

	if (name=="shape_count") {
		r_ret= shapes.size();
	} else if (name.begins_with("shapes/")) {

		int idx=name.get_slice("/",1).to_int();
		String what=name.get_slice("/",2);
		if (what=="shape")
			r_ret= get_shape(idx);
		else if (what=="transform")
			r_ret= get_shape_transform(idx);
        else if (what=="trigger")
            r_ret= is_shape_set_as_trigger(idx);

	} else
		return false;

	return true;
}

void CollisionObject3D::_get_property_list( List<PropertyInfo> *p_list) const {

	p_list->push_back( PropertyInfo(Variant::INT,"shape_count",PROPERTY_HINT_RANGE,"0,256,1",PROPERTY_USAGE_NOEDITOR|PROPERTY_USAGE_NO_INSTANCE_STATE) );

	for(int i=0;i<shapes.size();i++) {
		String path="shapes/"+itos(i)+"/";
		p_list->push_back( PropertyInfo(Variant::OBJECT,path+"shape",PROPERTY_HINT_RESOURCE_TYPE,"Shape",PROPERTY_USAGE_NOEDITOR|PROPERTY_USAGE_NO_INSTANCE_STATE) );
		p_list->push_back( PropertyInfo(Variant::TRANSFORM,path+"transform",PROPERTY_HINT_NONE,"",PROPERTY_USAGE_NOEDITOR|PROPERTY_USAGE_NO_INSTANCE_STATE) );
		p_list->push_back( PropertyInfo(Variant::BOOL,path+"trigger",PROPERTY_HINT_NONE,"",PROPERTY_USAGE_NOEDITOR|PROPERTY_USAGE_NO_INSTANCE_STATE) );

	}
}


void CollisionObject3D::_input_event(Node *p_camera, const InputEvent& p_input_event, const Vector3& p_pos, const Vector3& p_normal, int p_shape) {

	if (get_script_instance()) {
		get_script_instance()->call(SceneStringNames::get_singleton()->_input_event,p_camera,p_input_event,p_pos,p_normal,p_shape);
	}
	emit_signal(SceneStringNames::get_singleton()->input_event,p_camera,p_input_event,p_pos,p_normal,p_shape);
}

void CollisionObject3D::_mouse_enter() {

	if (get_script_instance()) {
		get_script_instance()->call(SceneStringNames::get_singleton()->_mouse_enter);
	}
	emit_signal(SceneStringNames::get_singleton()->mouse_enter);
}


void CollisionObject3D::_mouse_exit() {

	if (get_script_instance()) {
		get_script_instance()->call(SceneStringNames::get_singleton()->_mouse_exit);
	}
	emit_signal(SceneStringNames::get_singleton()->mouse_exit);

}

void CollisionObject3D::_update_pickable() {
	if (!is_inside_tree())
		return;
	bool pickable = ray_pickable && is_inside_tree() && is_visible();
	if (area)
		PhysicsServer::get_singleton()->area_set_ray_pickable(rid,pickable);
	else
		PhysicsServer::get_singleton()->body_set_ray_pickable(rid,pickable);
}

void CollisionObject3D::set_ray_pickable(bool p_ray_pickable) {

	ray_pickable=p_ray_pickable;
	_update_pickable();

}

bool CollisionObject3D::is_ray_pickable() const {

	return ray_pickable;
}


void CollisionObject3D::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("add_shape","shape:Shape","transform"),&CollisionObject3D::add_shape,DEFVAL(Transform3D()));
	ObjectTypeDB::bind_method(_MD("get_shape_count"),&CollisionObject3D::get_shape_count);
	ObjectTypeDB::bind_method(_MD("set_shape","shape_idx","shape:Shape"),&CollisionObject3D::set_shape);
	ObjectTypeDB::bind_method(_MD("set_shape_transform","shape_idx","transform"),&CollisionObject3D::set_shape_transform);
//    ObjectTypeDB::bind_method(_MD("set_shape_transform","shape_idx","transform"),&CollisionObject3D::set_shape_transform);
	ObjectTypeDB::bind_method(_MD("set_shape_as_trigger","shape_idx","enable"),&CollisionObject3D::set_shape_as_trigger);
	ObjectTypeDB::bind_method(_MD("is_shape_set_as_trigger","shape_idx"),&CollisionObject3D::is_shape_set_as_trigger);
	ObjectTypeDB::bind_method(_MD("get_shape:Shape","shape_idx"),&CollisionObject3D::get_shape);
	ObjectTypeDB::bind_method(_MD("get_shape_transform","shape_idx"),&CollisionObject3D::get_shape_transform);
	ObjectTypeDB::bind_method(_MD("remove_shape","shape_idx"),&CollisionObject3D::remove_shape);
	ObjectTypeDB::bind_method(_MD("clear_shapes"),&CollisionObject3D::clear_shapes);
	ObjectTypeDB::bind_method(_MD("set_ray_pickable","ray_pickable"),&CollisionObject3D::set_ray_pickable);
	ObjectTypeDB::bind_method(_MD("is_ray_pickable"),&CollisionObject3D::is_ray_pickable);
	ObjectTypeDB::bind_method(_MD("set_capture_input_on_drag","enable"),&CollisionObject3D::set_capture_input_on_drag);
	ObjectTypeDB::bind_method(_MD("get_capture_input_on_drag"),&CollisionObject3D::get_capture_input_on_drag);
	ObjectTypeDB::bind_method(_MD("get_rid"),&CollisionObject3D::get_rid);
	BIND_VMETHOD( MethodInfo("_input_event",PropertyInfo(Variant::OBJECT,"camera"),PropertyInfo(Variant::INPUT_EVENT,"event"),PropertyInfo(Variant::VECTOR3,"click_pos"),PropertyInfo(Variant::VECTOR3,"click_normal"),PropertyInfo(Variant::INT,"shape_idx")));

	ADD_SIGNAL( MethodInfo("input_event",PropertyInfo(Variant::OBJECT,"camera"),PropertyInfo(Variant::INPUT_EVENT,"event"),PropertyInfo(Variant::VECTOR3,"click_pos"),PropertyInfo(Variant::VECTOR3,"click_normal"),PropertyInfo(Variant::INT,"shape_idx")));
	ADD_SIGNAL( MethodInfo("mouse_enter"));
	ADD_SIGNAL( MethodInfo("mouse_exit"));

	ADD_PROPERTY( PropertyInfo(Variant::BOOL,"input/ray_pickable"),_SCS("set_ray_pickable"),_SCS("is_ray_pickable"));
	ADD_PROPERTY(PropertyInfo(Variant::BOOL,"input/capture_on_drag"),_SCS("set_capture_input_on_drag"),_SCS("get_capture_input_on_drag"));
}


void CollisionObject3D::add_shape(const Ref<Shape>& p_shape, const Transform3D& p_transform) {

	ShapeData sdata;
	sdata.shape=p_shape;
	sdata.xform=p_transform;
	shapes.push_back(sdata);
	_update_shapes();

}
int CollisionObject3D::get_shape_count() const {

	return shapes.size();

}
void CollisionObject3D::set_shape(int p_shape_idx, const Ref<Shape>& p_shape) {

	ERR_FAIL_INDEX(p_shape_idx,shapes.size());
	shapes[p_shape_idx].shape=p_shape;
	_update_shapes();
}

void CollisionObject3D::set_shape_transform(int p_shape_idx, const Transform3D& p_transform) {

	ERR_FAIL_INDEX(p_shape_idx,shapes.size());
	shapes[p_shape_idx].xform=p_transform;

	_update_shapes();
}

Ref<Shape> CollisionObject3D::get_shape(int p_shape_idx) const {

	ERR_FAIL_INDEX_V(p_shape_idx,shapes.size(),Ref<Shape>());
	return shapes[p_shape_idx].shape;

}
Transform3D CollisionObject3D::get_shape_transform(int p_shape_idx) const {

	ERR_FAIL_INDEX_V(p_shape_idx,shapes.size(),Transform3D());
	return shapes[p_shape_idx].xform;

}
void CollisionObject3D::remove_shape(int p_shape_idx) {

	ERR_FAIL_INDEX(p_shape_idx,shapes.size());
	shapes.remove(p_shape_idx);

	_update_shapes();
}

void CollisionObject3D::clear_shapes() {

	shapes.clear();

	_update_shapes();
}

void CollisionObject3D::set_shape_as_trigger(int p_shape_idx, bool p_trigger) {

    ERR_FAIL_INDEX(p_shape_idx,shapes.size());
    shapes[p_shape_idx].trigger=p_trigger;
    if (!area && rid.is_valid()) {

        PhysicsServer::get_singleton()->body_set_shape_as_trigger(rid,p_shape_idx,p_trigger);

    }
}

bool CollisionObject3D::is_shape_set_as_trigger(int p_shape_idx) const {

    ERR_FAIL_INDEX_V(p_shape_idx,shapes.size(),false);
    return shapes[p_shape_idx].trigger;
}

CollisionObject3D::CollisionObject3D(RID p_rid, bool p_area) {

	rid=p_rid;
	area=p_area;
	capture_input_on_drag=false;
	ray_pickable=true;
	if (p_area) {
		PhysicsServer::get_singleton()->area_attach_object_instance_ID(rid,get_instance_ID());
	} else {
		PhysicsServer::get_singleton()->body_attach_object_instance_ID(rid,get_instance_ID());
	}
//	set_transform_notify(true);

}

void CollisionObject3D::set_capture_input_on_drag(bool p_capture) {

	capture_input_on_drag=p_capture;

}

bool CollisionObject3D::get_capture_input_on_drag() const {

	return capture_input_on_drag;
}


CollisionObject3D::CollisionObject3D() {


	capture_input_on_drag=false;
	ray_pickable=true;

	//owner=

	//set_transform_notify(true);
}

CollisionObject3D::~CollisionObject3D() {

	PhysicsServer::get_singleton()->free(rid);
}
