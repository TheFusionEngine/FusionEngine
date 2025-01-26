/*************************************************************************/
/*  physics_body.cpp                                                     */
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
#include "physics_body.h"
#include "scene/scene_string_names.h"

void PhysicsBody3D::_notification(int p_what) {

/*
	switch(p_what) {

		case NOTIFICATION_TRANSFORM_CHANGED: {

			PhysicsServer::get_singleton()->body_set_state(get_rid(),PhysicsServer::BODY_STATE_TRANSFORM,get_global_transform());

		} break;
	}
	*/
}

Vector3 PhysicsBody3D::get_linear_velocity() const {

	return Vector3();
}
Vector3 PhysicsBody3D::get_angular_velocity() const {

	return Vector3();
}

float PhysicsBody3D::get_inverse_mass() const {

	return 0;
}


void PhysicsBody3D::set_layer_mask(uint32_t p_mask) {

	layer_mask=p_mask;
	PhysicsServer::get_singleton()->body_set_layer_mask(get_rid(),p_mask);
}

uint32_t PhysicsBody3D::get_layer_mask() const {

	return layer_mask;
}

void PhysicsBody3D::add_collision_exception_with(Node* p_node) {

	ERR_FAIL_NULL(p_node);
	PhysicsBody3D *physics_body = p_node->cast_to<PhysicsBody3D>();
	if (!physics_body) {
		ERR_EXPLAIN("Collision exception only works between two objects of PhysicsBody3D type");
	}
	ERR_FAIL_COND(!physics_body);
	PhysicsServer::get_singleton()->body_add_collision_exception(get_rid(),physics_body->get_rid());

}

void PhysicsBody3D::remove_collision_exception_with(Node* p_node) {

	ERR_FAIL_NULL(p_node);
	PhysicsBody3D *physics_body = p_node->cast_to<PhysicsBody3D>();
	if (!physics_body) {
		ERR_EXPLAIN("Collision exception only works between two objects of PhysicsBody3D type");
	}
	ERR_FAIL_COND(!physics_body);
	PhysicsServer::get_singleton()->body_remove_collision_exception(get_rid(),physics_body->get_rid());
}

void PhysicsBody3D::_bind_methods() {
	ObjectTypeDB::bind_method(_MD("set_layer_mask","mask"),&PhysicsBody3D::set_layer_mask);
	ObjectTypeDB::bind_method(_MD("get_layer_mask"),&PhysicsBody3D::get_layer_mask);
	ADD_PROPERTY(PropertyInfo(Variant::INT,"layers",PROPERTY_HINT_ALL_FLAGS),_SCS("set_layer_mask"),_SCS("get_layer_mask"));
}


PhysicsBody3D::PhysicsBody3D(PhysicsServer::BodyMode p_mode) : CollisionObject3D( PhysicsServer::get_singleton()->body_create(p_mode), false) {

	layer_mask=1;

}


void StaticBody3D::set_friction(real_t p_friction){

	ERR_FAIL_COND(p_friction<0 || p_friction>1);

	friction=p_friction;
	PhysicsServer::get_singleton()->body_set_param(get_rid(),PhysicsServer::BODY_PARAM_FRICTION,friction);

}
real_t StaticBody3D::get_friction() const{

	return friction;
}

void StaticBody3D::set_bounce(real_t p_bounce){

	ERR_FAIL_COND(p_bounce<0 || p_bounce>1);

	bounce=p_bounce;
	PhysicsServer::get_singleton()->body_set_param(get_rid(),PhysicsServer::BODY_PARAM_BOUNCE,bounce);

}
real_t StaticBody3D::get_bounce() const{

	return bounce;
}

void StaticBody3D::set_constant_linear_velocity(const Vector3& p_vel) {

	constant_linear_velocity=p_vel;
	PhysicsServer::get_singleton()->body_set_state(get_rid(),PhysicsServer::BODY_STATE_LINEAR_VELOCITY,constant_linear_velocity);

}

void StaticBody3D::set_constant_angular_velocity(const Vector3& p_vel) {

	constant_angular_velocity=p_vel;
	PhysicsServer::get_singleton()->body_set_state(get_rid(),PhysicsServer::BODY_STATE_ANGULAR_VELOCITY,constant_angular_velocity);
}

Vector3 StaticBody3D::get_constant_linear_velocity() const {

	return constant_linear_velocity;
}
Vector3 StaticBody3D::get_constant_angular_velocity() const {

	return constant_angular_velocity;
}



void StaticBody3D::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("set_constant_linear_velocity","vel"),&StaticBody3D::set_constant_linear_velocity);
	ObjectTypeDB::bind_method(_MD("set_constant_angular_velocity","vel"),&StaticBody3D::set_constant_angular_velocity);
	ObjectTypeDB::bind_method(_MD("get_constant_linear_velocity"),&StaticBody3D::get_constant_linear_velocity);
	ObjectTypeDB::bind_method(_MD("get_constant_angular_velocity"),&StaticBody3D::get_constant_angular_velocity);

	ObjectTypeDB::bind_method(_MD("set_friction","friction"),&StaticBody3D::set_friction);
	ObjectTypeDB::bind_method(_MD("get_friction"),&StaticBody3D::get_friction);

	ObjectTypeDB::bind_method(_MD("set_bounce","bounce"),&StaticBody3D::set_bounce);
	ObjectTypeDB::bind_method(_MD("get_bounce"),&StaticBody3D::get_bounce);

	ObjectTypeDB::bind_method(_MD("add_collision_exception_with","body:PhysicsBody3D"),&PhysicsBody3D::add_collision_exception_with);
	ObjectTypeDB::bind_method(_MD("remove_collision_exception_with","body:PhysicsBody3D"),&PhysicsBody3D::remove_collision_exception_with);

	ADD_PROPERTY( PropertyInfo(Variant::REAL,"friction",PROPERTY_HINT_RANGE,"0,1,0.01"),_SCS("set_friction"),_SCS("get_friction"));
	ADD_PROPERTY( PropertyInfo(Variant::REAL,"bounce",PROPERTY_HINT_RANGE,"0,1,0.01"),_SCS("set_bounce"),_SCS("get_bounce"));

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3,"constant_linear_velocity"),_SCS("set_constant_linear_velocity"),_SCS("get_constant_linear_velocity"));
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3,"constant_angular_velocity"),_SCS("set_constant_angular_velocity"),_SCS("get_constant_angular_velocity"));
}

StaticBody3D::StaticBody3D() : PhysicsBody3D(PhysicsServer::BODY_MODE_STATIC) {

	bounce=0;
	friction=1;
}

StaticBody3D::~StaticBody3D() {


}




void RigidBody3D::_body_enter_tree(ObjectID p_id) {

	Object *obj = ObjectDB::get_instance(p_id);
	Node *node = obj ? obj->cast_to<Node>() : NULL;
	ERR_FAIL_COND(!node);

	Map<ObjectID,BodyState>::Element *E=contact_monitor->body_map.find(p_id);
	ERR_FAIL_COND(!E);
	ERR_FAIL_COND(E->get().in_tree);

	E->get().in_tree=true;
	emit_signal(SceneStringNames::get_singleton()->body_enter,node);

	for(int i=0;i<E->get().shapes.size();i++) {

		emit_signal(SceneStringNames::get_singleton()->body_enter_shape,p_id,node,E->get().shapes[i].body_shape,E->get().shapes[i].local_shape);
	}

}

void RigidBody3D::_body_exit_tree(ObjectID p_id) {

	Object *obj = ObjectDB::get_instance(p_id);
	Node *node = obj ? obj->cast_to<Node>() : NULL;
	ERR_FAIL_COND(!node);
	Map<ObjectID,BodyState>::Element *E=contact_monitor->body_map.find(p_id);
	ERR_FAIL_COND(!E);
	ERR_FAIL_COND(!E->get().in_tree);
	E->get().in_tree=false;
	emit_signal(SceneStringNames::get_singleton()->body_exit,node);
	for(int i=0;i<E->get().shapes.size();i++) {

		emit_signal(SceneStringNames::get_singleton()->body_exit_shape,p_id,node,E->get().shapes[i].body_shape,E->get().shapes[i].local_shape);
	}
}

void RigidBody3D::_body_inout(int p_status, ObjectID p_instance, int p_body_shape,int p_local_shape) {

	bool body_in = p_status==1;
	ObjectID objid=p_instance;

	Object *obj = ObjectDB::get_instance(objid);
	Node *node = obj ? obj->cast_to<Node>() : NULL;

	Map<ObjectID,BodyState>::Element *E=contact_monitor->body_map.find(objid);

	ERR_FAIL_COND(!body_in && !E);

	if (body_in) {
		if (!E) {

			E = contact_monitor->body_map.insert(objid,BodyState());
			//E->get().rc=0;
			E->get().in_tree=node && node->is_inside_tree();
			if (node) {
				node->connect(SceneStringNames::get_singleton()->enter_tree,this,SceneStringNames::get_singleton()->_body_enter_tree,make_binds(objid));
				node->connect(SceneStringNames::get_singleton()->exit_tree,this,SceneStringNames::get_singleton()->_body_exit_tree,make_binds(objid));
				if (E->get().in_tree) {
					emit_signal(SceneStringNames::get_singleton()->body_enter,node);
				}
			}

		}
		//E->get().rc++;
		if (node)
			E->get().shapes.insert(ShapePair(p_body_shape,p_local_shape));


		if (E->get().in_tree) {
			emit_signal(SceneStringNames::get_singleton()->body_enter_shape,objid,node,p_body_shape,p_local_shape);
		}

	} else {

		//E->get().rc--;

		if (node)
			E->get().shapes.erase(ShapePair(p_body_shape,p_local_shape));

		bool in_tree = E->get().in_tree;

		if (E->get().shapes.empty()) {

			if (node) {
				node->disconnect(SceneStringNames::get_singleton()->enter_tree,this,SceneStringNames::get_singleton()->_body_enter_tree);
				node->disconnect(SceneStringNames::get_singleton()->exit_tree,this,SceneStringNames::get_singleton()->_body_exit_tree);
				if (in_tree)
					emit_signal(SceneStringNames::get_singleton()->body_exit,obj);

			}

			contact_monitor->body_map.erase(E);
		}
		if (node && in_tree) {
			emit_signal(SceneStringNames::get_singleton()->body_exit_shape,objid,obj,p_body_shape,p_local_shape);
		}

	}

}


struct _RigidBodyInOut {

	ObjectID id;
	int shape;
	int local_shape;
};

void RigidBody3D::_direct_state_changed(Object *p_state) {

	//eh.. fuck
#ifdef DEBUG_ENABLED

	state=p_state->cast_to<PhysicsDirectBodyState>();
#else
	state=(PhysicsDirectBodyState*)p_state; //trust it
#endif

	if (contact_monitor) {

		//untag all
		int rc=0;
		for( Map<ObjectID,BodyState>::Element *E=contact_monitor->body_map.front();E;E=E->next()) {

			for(int i=0;i<E->get().shapes.size();i++) {

				E->get().shapes[i].tagged=false;
				rc++;
			}
		}

		_RigidBodyInOut *toadd=(_RigidBodyInOut*)alloca(state->get_contact_count()*sizeof(_RigidBodyInOut));
		int toadd_count=0;//state->get_contact_count();
		RigidBody_RemoveAction *toremove=(RigidBody_RemoveAction*)alloca(rc*sizeof(RigidBody_RemoveAction));
		int toremove_count=0;

		//put the ones to add

		for(int i=0;i<state->get_contact_count();i++) {

			ObjectID obj = state->get_contact_collider_id(i);
			int local_shape = state->get_contact_local_shape(i);
			int shape = state->get_contact_collider_shape(i);
			toadd[i].local_shape=local_shape;
			toadd[i].id=obj;
			toadd[i].shape=shape;

			bool found=false;

			Map<ObjectID,BodyState>::Element *E=contact_monitor->body_map.find(obj);
			if (!E) {
				toadd_count++;
				continue;
			}

			ShapePair sp( shape,local_shape );
			int idx = E->get().shapes.find(sp);
			if (idx==-1) {

				toadd_count++;
				continue;
			}

			E->get().shapes[idx].tagged=true;
		}

		//put the ones to remove

		for( Map<ObjectID,BodyState>::Element *E=contact_monitor->body_map.front();E;E=E->next()) {

			for(int i=0;i<E->get().shapes.size();i++) {

				if (!E->get().shapes[i].tagged) {

					toremove[toremove_count].body_id=E->key();
					toremove[toremove_count].pair=E->get().shapes[i];
					toremove_count++;
				}
			}
		}


		//process remotions

		for(int i=0;i<toremove_count;i++) {

			_body_inout(0,toremove[i].body_id,toremove[i].pair.body_shape,toremove[i].pair.local_shape);
		}

		//process aditions

		for(int i=0;i<toadd_count;i++) {

			_body_inout(1,toadd[i].id,toadd[i].shape,toadd[i].local_shape);
		}

	}

	set_ignore_transform_notification(true);
	set_global_transform(state->get_transform());
	linear_velocity=state->get_linear_velocity();
	angular_velocity=state->get_angular_velocity();
	sleeping=state->is_sleeping();
	if (get_script_instance())
		get_script_instance()->call("_integrate_forces",state);
	set_ignore_transform_notification(false);

	state=NULL;
}

void RigidBody3D::_notification(int p_what) {


}

void RigidBody3D::set_mode(Mode p_mode) {

	mode=p_mode;
	switch(p_mode) {

		case MODE_RIGID: {

			PhysicsServer::get_singleton()->body_set_mode(get_rid(),PhysicsServer::BODY_MODE_RIGID);
		} break;
		case MODE_STATIC: {

			PhysicsServer::get_singleton()->body_set_mode(get_rid(),PhysicsServer::BODY_MODE_STATIC);

		} break;
		case MODE_CHARACTER: {
			PhysicsServer::get_singleton()->body_set_mode(get_rid(),PhysicsServer::BODY_MODE_CHARACTER);

		} break;
		case MODE_KINEMATIC: {

			PhysicsServer::get_singleton()->body_set_mode(get_rid(),PhysicsServer::BODY_MODE_KINEMATIC);
		} break;

	}
}

RigidBody3D::Mode RigidBody3D::get_mode() const{

	return mode;
}

void RigidBody3D::set_mass(real_t p_mass){

	ERR_FAIL_COND(p_mass<=0);
	mass=p_mass;
	_change_notify("mass");
	_change_notify("weight");
	PhysicsServer::get_singleton()->body_set_param(get_rid(),PhysicsServer::BODY_PARAM_MASS,mass);

}
real_t RigidBody3D::get_mass() const{

	return mass;
}

void RigidBody3D::set_weight(real_t p_weight){

	set_mass(p_weight/9.8);
}
real_t RigidBody3D::get_weight() const{

	return mass*9.8;
}


void RigidBody3D::set_friction(real_t p_friction){

	ERR_FAIL_COND(p_friction<0 || p_friction>1);

	friction=p_friction;
	PhysicsServer::get_singleton()->body_set_param(get_rid(),PhysicsServer::BODY_PARAM_FRICTION,friction);

}
real_t RigidBody3D::get_friction() const{

	return friction;
}

void RigidBody3D::set_bounce(real_t p_bounce){

	ERR_FAIL_COND(p_bounce<0 || p_bounce>1);

	bounce=p_bounce;
	PhysicsServer::get_singleton()->body_set_param(get_rid(),PhysicsServer::BODY_PARAM_BOUNCE,bounce);

}
real_t RigidBody3D::get_bounce() const{

	return bounce;
}

void RigidBody3D::set_axis_velocity(const Vector3& p_axis) {

	Vector3 v = state? state->get_linear_velocity() : linear_velocity;
	Vector3 axis = p_axis.normalized();
	v-=axis*axis.dot(v);
	v+=p_axis;
	if (state) {
		set_linear_velocity(v);
	} else {
		PhysicsServer::get_singleton()->body_set_axis_velocity(get_rid(),p_axis);
		linear_velocity=v;
	}
}

void RigidBody3D::set_linear_velocity(const Vector3& p_velocity){

	linear_velocity=p_velocity;
	if (state)
		state->set_linear_velocity(linear_velocity);
	else
		PhysicsServer::get_singleton()->body_set_state(get_rid(),PhysicsServer::BODY_STATE_LINEAR_VELOCITY,linear_velocity);

}

Vector3 RigidBody3D::get_linear_velocity() const{

	return linear_velocity;
}

void RigidBody3D::set_angular_velocity(const Vector3& p_velocity){

	angular_velocity=p_velocity;
	if (state)
		state->set_angular_velocity(angular_velocity);
	else
		PhysicsServer::get_singleton()->body_set_state(get_rid(),PhysicsServer::BODY_STATE_ANGULAR_VELOCITY,angular_velocity);
}
Vector3 RigidBody3D::get_angular_velocity() const{

	return angular_velocity;
}

void RigidBody3D::set_use_custom_integrator(bool p_enable){

	if (custom_integrator==p_enable)
		return;

	custom_integrator=p_enable;
	PhysicsServer::get_singleton()->body_set_omit_force_integration(get_rid(),p_enable);


}
bool RigidBody3D::is_using_custom_integrator(){

	return custom_integrator;
}

void RigidBody3D::set_sleeping(bool p_sleeping) {

	sleeping=p_sleeping;
	PhysicsServer::get_singleton()->body_set_state(get_rid(),PhysicsServer::BODY_STATE_SLEEPING,sleeping);

}

void RigidBody3D::set_can_sleep(bool p_active) {

	can_sleep=p_active;
	PhysicsServer::get_singleton()->body_set_state(get_rid(),PhysicsServer::BODY_STATE_CAN_SLEEP,p_active);
}

bool RigidBody3D::is_able_to_sleep() const {

	return can_sleep;
}

bool RigidBody3D::is_sleeping() const {

	return sleeping;
}

void RigidBody3D::set_max_contacts_reported(int p_amount) {

	max_contacts_reported=p_amount;
	PhysicsServer::get_singleton()->body_set_max_contacts_reported(get_rid(),p_amount);
}

int RigidBody3D::get_max_contacts_reported() const{

	return max_contacts_reported;
}

void RigidBody3D::apply_impulse(const Vector3& p_pos, const Vector3& p_impulse) {

	PhysicsServer::get_singleton()->body_apply_impulse(get_rid(),p_pos,p_impulse);
}

void RigidBody3D::set_use_continuous_collision_detection(bool p_enable) {

	ccd=p_enable;
	PhysicsServer::get_singleton()->body_set_enable_continuous_collision_detection(get_rid(),p_enable);
}

bool RigidBody3D::is_using_continuous_collision_detection() const {


	return ccd;
}


void RigidBody3D::set_contact_monitor(bool p_enabled) {

	if (p_enabled==is_contact_monitor_enabled())
		return;

	if (!p_enabled) {

		for(Map<ObjectID,BodyState>::Element *E=contact_monitor->body_map.front();E;E=E->next()) {

			//clean up mess
		}

		memdelete( contact_monitor );
		contact_monitor=NULL;
	} else {

		contact_monitor = memnew( ContactMonitor );
	}

}

bool RigidBody3D::is_contact_monitor_enabled() const {

	return contact_monitor!=NULL;
}

void RigidBody3D::set_axis_lock(AxisLock p_lock) {

	axis_lock=p_lock;
	PhysicsServer::get_singleton()->body_set_axis_lock(get_rid(),PhysicsServer::BodyAxisLock(axis_lock));
}

RigidBody3D::AxisLock RigidBody3D::get_axis_lock() const {

	return axis_lock;
}


Array RigidBody3D::get_colliding_bodies() const {

	ERR_FAIL_COND_V(!contact_monitor,Array());

	Array ret;
	ret.resize(contact_monitor->body_map.size());
	int idx=0;
	for (const Map<ObjectID,BodyState>::Element *E=contact_monitor->body_map.front();E;E=E->next()) {
		Object *obj = ObjectDB::get_instance(E->key());
		if (!obj) {
			ret.resize( ret.size() -1 ); //ops
		} else {
			ret[idx++]=obj;
		}

	}

	return ret;
}


void RigidBody3D::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("set_mode","mode"),&RigidBody3D::set_mode);
	ObjectTypeDB::bind_method(_MD("get_mode"),&RigidBody3D::get_mode);

	ObjectTypeDB::bind_method(_MD("set_mass","mass"),&RigidBody3D::set_mass);
	ObjectTypeDB::bind_method(_MD("get_mass"),&RigidBody3D::get_mass);

	ObjectTypeDB::bind_method(_MD("set_weight","weight"),&RigidBody3D::set_weight);
	ObjectTypeDB::bind_method(_MD("get_weight"),&RigidBody3D::get_weight);

	ObjectTypeDB::bind_method(_MD("set_friction","friction"),&RigidBody3D::set_friction);
	ObjectTypeDB::bind_method(_MD("get_friction"),&RigidBody3D::get_friction);

	ObjectTypeDB::bind_method(_MD("set_bounce","bounce"),&RigidBody3D::set_bounce);
	ObjectTypeDB::bind_method(_MD("get_bounce"),&RigidBody3D::get_bounce);

	ObjectTypeDB::bind_method(_MD("set_linear_velocity","linear_velocity"),&RigidBody3D::set_linear_velocity);
	ObjectTypeDB::bind_method(_MD("get_linear_velocity"),&RigidBody3D::get_linear_velocity);

	ObjectTypeDB::bind_method(_MD("set_angular_velocity","angular_velocity"),&RigidBody3D::set_angular_velocity);
	ObjectTypeDB::bind_method(_MD("get_angular_velocity"),&RigidBody3D::get_angular_velocity);

	ObjectTypeDB::bind_method(_MD("set_max_contacts_reported","amount"),&RigidBody3D::set_max_contacts_reported);
	ObjectTypeDB::bind_method(_MD("get_max_contacts_reported"),&RigidBody3D::get_max_contacts_reported);

	ObjectTypeDB::bind_method(_MD("set_use_custom_integrator","enable"),&RigidBody3D::set_use_custom_integrator);
	ObjectTypeDB::bind_method(_MD("is_using_custom_integrator"),&RigidBody3D::is_using_custom_integrator);

	ObjectTypeDB::bind_method(_MD("set_contact_monitor","enabled"),&RigidBody3D::set_contact_monitor);
	ObjectTypeDB::bind_method(_MD("is_contact_monitor_enabled"),&RigidBody3D::is_contact_monitor_enabled);

	ObjectTypeDB::bind_method(_MD("set_use_continuous_collision_detection","enable"),&RigidBody3D::set_use_continuous_collision_detection);
	ObjectTypeDB::bind_method(_MD("is_using_continuous_collision_detection"),&RigidBody3D::is_using_continuous_collision_detection);

	ObjectTypeDB::bind_method(_MD("set_axis_velocity","axis_velocity"),&RigidBody3D::set_axis_velocity);
	ObjectTypeDB::bind_method(_MD("apply_impulse","pos","impulse"),&RigidBody3D::apply_impulse);

	ObjectTypeDB::bind_method(_MD("set_sleeping","sleeping"),&RigidBody3D::set_sleeping);
	ObjectTypeDB::bind_method(_MD("is_sleeping"),&RigidBody3D::is_sleeping);

	ObjectTypeDB::bind_method(_MD("set_can_sleep","able_to_sleep"),&RigidBody3D::set_can_sleep);
	ObjectTypeDB::bind_method(_MD("is_able_to_sleep"),&RigidBody3D::is_able_to_sleep);

	ObjectTypeDB::bind_method(_MD("_direct_state_changed"),&RigidBody3D::_direct_state_changed);
	ObjectTypeDB::bind_method(_MD("_body_enter_tree"),&RigidBody3D::_body_enter_tree);
	ObjectTypeDB::bind_method(_MD("_body_exit_tree"),&RigidBody3D::_body_exit_tree);

	ObjectTypeDB::bind_method(_MD("set_axis_lock","axis_lock"),&RigidBody3D::set_axis_lock);
	ObjectTypeDB::bind_method(_MD("get_axis_lock"),&RigidBody3D::get_axis_lock);

	ObjectTypeDB::bind_method(_MD("get_colliding_bodies"),&RigidBody3D::get_colliding_bodies);

	BIND_VMETHOD(MethodInfo("_integrate_forces",PropertyInfo(Variant::OBJECT,"state:PhysicsDirectBodyState")));

	ADD_PROPERTY( PropertyInfo(Variant::INT,"mode",PROPERTY_HINT_ENUM,"Rigid,Static,Character,Kinematic"),_SCS("set_mode"),_SCS("get_mode"));
	ADD_PROPERTY( PropertyInfo(Variant::REAL,"mass",PROPERTY_HINT_EXP_RANGE,"0.01,65535,0.01"),_SCS("set_mass"),_SCS("get_mass"));
	ADD_PROPERTY( PropertyInfo(Variant::REAL,"weight",PROPERTY_HINT_EXP_RANGE,"0.01,65535,0.01",PROPERTY_USAGE_EDITOR),_SCS("set_weight"),_SCS("get_weight"));
	ADD_PROPERTY( PropertyInfo(Variant::REAL,"friction",PROPERTY_HINT_RANGE,"0,1,0.01"),_SCS("set_friction"),_SCS("get_friction"));
	ADD_PROPERTY( PropertyInfo(Variant::REAL,"bounce",PROPERTY_HINT_RANGE,"0,1,0.01"),_SCS("set_bounce"),_SCS("get_bounce"));
	ADD_PROPERTY( PropertyInfo(Variant::BOOL,"custom_integrator"),_SCS("set_use_custom_integrator"),_SCS("is_using_custom_integrator"));
	ADD_PROPERTY( PropertyInfo(Variant::BOOL,"continuous_cd"),_SCS("set_use_continuous_collision_detection"),_SCS("is_using_continuous_collision_detection"));
	ADD_PROPERTY( PropertyInfo(Variant::INT,"contacts_reported"),_SCS("set_max_contacts_reported"),_SCS("get_max_contacts_reported"));
	ADD_PROPERTY( PropertyInfo(Variant::BOOL,"contact_monitor"),_SCS("set_contact_monitor"),_SCS("is_contact_monitor_enabled"));
	ADD_PROPERTY( PropertyInfo(Variant::BOOL,"sleeping"),_SCS("set_sleeping"),_SCS("is_sleeping"));
	ADD_PROPERTY( PropertyInfo(Variant::BOOL,"can_sleep"),_SCS("set_can_sleep"),_SCS("is_able_to_sleep"));
	ADD_PROPERTY( PropertyInfo(Variant::INT,"axis_lock",PROPERTY_HINT_ENUM,"Disabled,Lock X,Lock Y,Lock Z"),_SCS("set_axis_lock"),_SCS("get_axis_lock"));
	ADD_PROPERTY( PropertyInfo(Variant::VECTOR3,"velocity/linear"),_SCS("set_linear_velocity"),_SCS("get_linear_velocity"));
	ADD_PROPERTY( PropertyInfo(Variant::VECTOR3,"velocity/angular"),_SCS("set_angular_velocity"),_SCS("get_angular_velocity"));

	ADD_SIGNAL( MethodInfo("body_enter_shape",PropertyInfo(Variant::INT,"body_id"),PropertyInfo(Variant::OBJECT,"body"),PropertyInfo(Variant::INT,"body_shape"),PropertyInfo(Variant::INT,"local_shape")));
	ADD_SIGNAL( MethodInfo("body_exit_shape",PropertyInfo(Variant::INT,"body_id"),PropertyInfo(Variant::OBJECT,"body"),PropertyInfo(Variant::INT,"body_shape"),PropertyInfo(Variant::INT,"local_shape")));
	ADD_SIGNAL( MethodInfo("body_enter",PropertyInfo(Variant::OBJECT,"body")));
	ADD_SIGNAL( MethodInfo("body_exit",PropertyInfo(Variant::OBJECT,"body")));

	BIND_CONSTANT( MODE_STATIC );
	BIND_CONSTANT( MODE_KINEMATIC );
	BIND_CONSTANT( MODE_RIGID );
	BIND_CONSTANT( MODE_CHARACTER );
}

RigidBody3D::RigidBody3D() : PhysicsBody3D(PhysicsServer::BODY_MODE_RIGID) {

	mode=MODE_RIGID;

	bounce=0;
	mass=1;
	friction=1;
	max_contacts_reported=0;
	state=NULL;

	//angular_velocity=0;
	sleeping=false;
	ccd=false;

	custom_integrator=false;
	contact_monitor=NULL;
	can_sleep=true;

	axis_lock = AXIS_LOCK_DISABLED;

	PhysicsServer::get_singleton()->body_set_force_integration_callback(get_rid(),this,"_direct_state_changed");
}

RigidBody3D::~RigidBody3D() {

	if (contact_monitor)
		memdelete( contact_monitor );



}
//////////////////////////////////////////////////////
//////////////////////////


Variant CharacterBody3D::_get_collider() const {

	ObjectID oid=get_collider();
	if (oid==0)
		return Variant();
	Object *obj = ObjectDB::get_instance(oid);
	if (!obj)
		return Variant();

	RefCounted *ref = obj->cast_to<RefCounted>();
	if (ref) {
		return Ref<RefCounted>(ref);
	}

	return obj;
}


bool CharacterBody3D::_ignores_mode(PhysicsServer::BodyMode p_mode) const {

	switch(p_mode) {
		case PhysicsServer::BODY_MODE_STATIC: return !collide_static;
		case PhysicsServer::BODY_MODE_KINEMATIC: return !collide_kinematic;
		case PhysicsServer::BODY_MODE_RIGID: return !collide_rigid;
		case PhysicsServer::BODY_MODE_CHARACTER: return !collide_character;
	}

	return true;
}

Vector3 CharacterBody3D::move(const Vector3& p_motion) {

	//give me back regular physics engine logic
	//this is madness
	//and most people using this function will think
	//what it does is simpler than using physics
	//this took about a week to get right..
	//but is it right? who knows at this point..


	colliding=false;
	ERR_FAIL_COND_V(!is_inside_tree(),Vector3());
	PhysicsDirectSpaceState *dss = PhysicsServer::get_singleton()->space_get_direct_state(get_world()->get_space());
	ERR_FAIL_COND_V(!dss,Vector3());
	const int max_shapes=32;
	Vector3 sr[max_shapes*2];
	int res_shapes;

	Set<RID> exclude;
	exclude.insert(get_rid());


	//recover first
	int recover_attempts=4;

	bool collided=false;
	uint32_t mask=0;
	if (collide_static)
		mask|=PhysicsDirectSpaceState::TYPE_MASK_STATIC_BODY;
	if (collide_kinematic)
		mask|=PhysicsDirectSpaceState::TYPE_MASK_KINEMATIC_BODY;
	if (collide_rigid)
		mask|=PhysicsDirectSpaceState::TYPE_MASK_RIGID_BODY;
	if (collide_character)
		mask|=PhysicsDirectSpaceState::TYPE_MASK_CHARACTER_BODY;

//	print_line("motion: "+p_motion+" margin: "+rtos(margin));

	//print_line("margin: "+rtos(margin));

	float m = margin;
	//m=0.001;

	do {

		//motion recover
		for(int i=0;i<get_shape_count();i++) {


			if (dss->collide_shape(get_shape(i)->get_rid(), get_global_transform() * get_shape_transform(i),m,sr,max_shapes,res_shapes,exclude,get_layer_mask(),mask)) {
				collided=true;
			}

		}



		if (!collided)
			break;

		//print_line("have to recover");
		Vector3 recover_motion;
		bool all_outside=true;
		for(int j=0;j<8;j++) {
			for(int i=0;i<res_shapes;i++) {

				Vector3 a = sr[i*2+0];
				Vector3 b = sr[i*2+1];
				//print_line(String()+a+" -> "+b);
#if 0
				float d = a.distance_to(b);

				//if (d<margin)
				///	continue;
	   ///
	   ///
				recover_motion+=(b-a)*0.2;
#else
				float dist = a.distance_to(b);
				if (dist>CMP_EPSILON) {
					Vector3 norm = (b-a).normalized();
					if (dist>margin*0.5)
						all_outside=false;
					float adv = norm.dot(recover_motion);
					//print_line(itos(i)+" dist: "+rtos(dist)+" adv: "+rtos(adv));
					recover_motion+=norm*MAX(dist-adv,0)*0.4;
				}
#endif

			}
		}


		if (recover_motion==Vector3()) {
			collided=false;
			break;
		}

		//print_line("**** RECOVER: "+recover_motion);

		Transform3D gt = get_global_transform();
		gt.origin+=recover_motion;
		set_global_transform(gt);

		recover_attempts--;

		if (all_outside)
			break;

	} while (recover_attempts);


	//move second
	float safe = 1.0;
	float unsafe = 1.0;
	int best_shape=-1;

	PhysicsDirectSpaceState::ShapeRestInfo rest;

	//print_line("pos: "+get_global_transform().origin);
	//print_line("motion: "+p_motion);


	for(int i=0;i<get_shape_count();i++) {



		float lsafe,lunsafe;
		PhysicsDirectSpaceState::ShapeRestInfo lrest;
		bool valid = dss->cast_motion(get_shape(i)->get_rid(), get_global_transform() * get_shape_transform(i), p_motion,0, lsafe,lunsafe,exclude,get_layer_mask(),mask,&lrest);
		//print_line("shape: "+itos(i)+" travel:"+rtos(ltravel));
		if (!valid) {
			safe=0;
			unsafe=0;
			best_shape=i; //sadly it's the best
			//print_line("initial stuck");

			break;
		}
		if (lsafe==1.0) {
			//print_line("initial free");
			continue;
		}
		if (lsafe < safe) {

			//print_line("initial at "+rtos(lsafe));
			safe=lsafe;
			safe=MAX(0,lsafe-0.01);
			unsafe=lunsafe;
			best_shape=i;
			rest=lrest;
		}
	}


	//print_line("best shape: "+itos(best_shape)+" motion "+p_motion);

	if (safe>=1) {
		//not collided
		colliding=false;
	} else {

		colliding=true;

		if (true || (safe==0 && unsafe==0)) { //use it always because it's more precise than GJK
			//no advance, use rest info from collision
			Transform3D ugt = get_global_transform();
			ugt.origin+=p_motion*unsafe;

			PhysicsDirectSpaceState::ShapeRestInfo rest_info;
			bool c2 = dss->rest_info(get_shape(best_shape)->get_rid(), ugt*get_shape_transform(best_shape), m,&rest,exclude,get_layer_mask(),mask);
			if (!c2) {
				//should not happen, but floating point precision is so weird..
				colliding=false;
			}

		//	print_line("Rest Travel: "+rest.normal);

		}

		if (colliding) {

			collision=rest.point;
			normal=rest.normal;
			collider=rest.collider_id;
			collider_vel=rest.linear_velocity;
			collider_shape=rest.shape;
		}
	}

	Vector3 motion=p_motion*safe;
	//if (colliding)
	//	motion+=normal*0.001;
	Transform3D gt = get_global_transform();
	gt.origin+=motion;
	set_global_transform(gt);

	return p_motion-motion;

}

Vector3 CharacterBody3D::move_to(const Vector3& p_position) {

	return move(p_position-get_global_transform().origin);
}

bool CharacterBody3D::can_move_to(const Vector3& p_position, bool p_discrete) {

	ERR_FAIL_COND_V(!is_inside_tree(),false);
	PhysicsDirectSpaceState *dss = PhysicsServer::get_singleton()->space_get_direct_state(get_world()->get_space());
	ERR_FAIL_COND_V(!dss,false);

	uint32_t mask=0;
	if (collide_static)
		mask|=PhysicsDirectSpaceState::TYPE_MASK_STATIC_BODY;
	if (collide_kinematic)
		mask|=PhysicsDirectSpaceState::TYPE_MASK_KINEMATIC_BODY;
	if (collide_rigid)
		mask|=PhysicsDirectSpaceState::TYPE_MASK_RIGID_BODY;
	if (collide_character)
		mask|=PhysicsDirectSpaceState::TYPE_MASK_CHARACTER_BODY;

	Vector3 motion = p_position-get_global_transform().origin;
	Transform3D xform=get_global_transform();

	if (true || p_discrete) {

		xform.origin+=motion;
		motion=Vector3();
	}

	Set<RID> exclude;
	exclude.insert(get_rid());

	//fill exclude list..
	for(int i=0;i<get_shape_count();i++) {


		bool col = dss->intersect_shape(get_shape(i)->get_rid(), xform * get_shape_transform(i),0,NULL,0,exclude,get_layer_mask(),mask);
		if (col)
			return false;
	}

	return true;
}

bool CharacterBody3D::is_colliding() const {

	ERR_FAIL_COND_V(!is_inside_tree(),false);

	return colliding;
}
Vector3 CharacterBody3D::get_collision_pos() const {

	ERR_FAIL_COND_V(!colliding,Vector3());
	return collision;

}
Vector3 CharacterBody3D::get_collision_normal() const {

	ERR_FAIL_COND_V(!colliding,Vector3());
	return normal;

}

Vector3 CharacterBody3D::get_collider_velocity() const {

	return collider_vel;
}

ObjectID CharacterBody3D::get_collider() const {

	ERR_FAIL_COND_V(!colliding,0);
	return collider;
}
int CharacterBody3D::get_collider_shape() const {

	ERR_FAIL_COND_V(!colliding,-1);
	return collider_shape;

}
void CharacterBody3D::set_collide_with_static_bodies(bool p_enable) {

	collide_static=p_enable;
}
bool CharacterBody3D::can_collide_with_static_bodies() const {

	return collide_static;
}

void CharacterBody3D::set_collide_with_rigid_bodies(bool p_enable) {

	collide_rigid=p_enable;

}
bool CharacterBody3D::can_collide_with_rigid_bodies() const {


	return collide_rigid;
}

void CharacterBody3D::set_collide_with_kinematic_bodies(bool p_enable) {

	collide_kinematic=p_enable;

}
bool CharacterBody3D::can_collide_with_kinematic_bodies() const {

	return collide_kinematic;
}

void CharacterBody3D::set_collide_with_character_bodies(bool p_enable) {

	collide_character=p_enable;
}
bool CharacterBody3D::can_collide_with_character_bodies() const {

	return collide_character;
}

void CharacterBody3D::set_collision_margin(float p_margin) {

	margin=p_margin;
}

float CharacterBody3D::get_collision_margin() const{

	return margin;
}

void CharacterBody3D::_bind_methods() {


	ObjectTypeDB::bind_method(_MD("move","rel_vec"),&CharacterBody3D::move);
	ObjectTypeDB::bind_method(_MD("move_to","position"),&CharacterBody3D::move_to);

	ObjectTypeDB::bind_method(_MD("can_move_to","position"),&CharacterBody3D::can_move_to);

	ObjectTypeDB::bind_method(_MD("is_colliding"),&CharacterBody3D::is_colliding);

	ObjectTypeDB::bind_method(_MD("get_collision_pos"),&CharacterBody3D::get_collision_pos);
	ObjectTypeDB::bind_method(_MD("get_collision_normal"),&CharacterBody3D::get_collision_normal);
	ObjectTypeDB::bind_method(_MD("get_collider_velocity"),&CharacterBody3D::get_collider_velocity);
	ObjectTypeDB::bind_method(_MD("get_collider:Object"),&CharacterBody3D::_get_collider);
	ObjectTypeDB::bind_method(_MD("get_collider_shape"),&CharacterBody3D::get_collider_shape);


	ObjectTypeDB::bind_method(_MD("set_collide_with_static_bodies","enable"),&CharacterBody3D::set_collide_with_static_bodies);
	ObjectTypeDB::bind_method(_MD("can_collide_with_static_bodies"),&CharacterBody3D::can_collide_with_static_bodies);

	ObjectTypeDB::bind_method(_MD("set_collide_with_kinematic_bodies","enable"),&CharacterBody3D::set_collide_with_kinematic_bodies);
	ObjectTypeDB::bind_method(_MD("can_collide_with_kinematic_bodies"),&CharacterBody3D::can_collide_with_kinematic_bodies);

	ObjectTypeDB::bind_method(_MD("set_collide_with_rigid_bodies","enable"),&CharacterBody3D::set_collide_with_rigid_bodies);
	ObjectTypeDB::bind_method(_MD("can_collide_with_rigid_bodies"),&CharacterBody3D::can_collide_with_rigid_bodies);

	ObjectTypeDB::bind_method(_MD("set_collide_with_character_bodies","enable"),&CharacterBody3D::set_collide_with_character_bodies);
	ObjectTypeDB::bind_method(_MD("can_collide_with_character_bodies"),&CharacterBody3D::can_collide_with_character_bodies);

	ObjectTypeDB::bind_method(_MD("set_collision_margin","pixels"),&CharacterBody3D::set_collision_margin);
	ObjectTypeDB::bind_method(_MD("get_collision_margin","pixels"),&CharacterBody3D::get_collision_margin);

	ADD_PROPERTY( PropertyInfo(Variant::BOOL,"collide_with/static"),_SCS("set_collide_with_static_bodies"),_SCS("can_collide_with_static_bodies"));
	ADD_PROPERTY( PropertyInfo(Variant::BOOL,"collide_with/kinematic"),_SCS("set_collide_with_kinematic_bodies"),_SCS("can_collide_with_kinematic_bodies"));
	ADD_PROPERTY( PropertyInfo(Variant::BOOL,"collide_with/rigid"),_SCS("set_collide_with_rigid_bodies"),_SCS("can_collide_with_rigid_bodies"));
	ADD_PROPERTY( PropertyInfo(Variant::BOOL,"collide_with/character"),_SCS("set_collide_with_character_bodies"),_SCS("can_collide_with_character_bodies"));
	ADD_PROPERTY( PropertyInfo(Variant::REAL,"collision/margin",PROPERTY_HINT_RANGE,"0.001,256,0.001"),_SCS("set_collision_margin"),_SCS("get_collision_margin"));


}

CharacterBody3D::CharacterBody3D() : PhysicsBody3D(PhysicsServer::BODY_MODE_KINEMATIC){

	collide_static=true;
	collide_rigid=true;
	collide_kinematic=true;
	collide_character=true;

	colliding=false;
	collider=0;
	margin=0.001;
	collider_shape=0;
}
CharacterBody3D::~CharacterBody3D()  {


}


