/*************************************************************************/
/*  physics_server.cpp                                                   */
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
#include "physics_3d_server.h"
#include "print_string.h"

PhysicsServer3D * PhysicsServer3D::singleton=NULL;


void Physics3DDirectBodyState::integrate_forces() {

	real_t step = get_step();
	Vector3 lv = get_linear_velocity();
	lv+=get_total_gravity() * step;

	Vector3 av = get_angular_velocity();

	float damp = 1.0 - step * get_total_density();

	if (damp<0) // reached zero in the given time
		damp=0;

	lv*=damp;
	av*=damp;

	set_linear_velocity(lv);
	set_angular_velocity(av);




}

Object* Physics3DDirectBodyState::get_contact_collider_object(int p_contact_idx) const {

	ObjectID objid = get_contact_collider_id(p_contact_idx);
	Object *obj = ObjectDB::get_instance( objid );
	return obj;
}

PhysicsServer3D * PhysicsServer3D::get_singleton() {
	return singleton;
}

void PhysicsServer3D::set_singleton(PhysicsServer3D *server){
	if (!singleton){
		singleton = server;
	}
}

void Physics3DDirectBodyState::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("get_total_gravity"),&Physics3DDirectBodyState::get_total_gravity);
	ObjectTypeDB::bind_method(_MD("get_total_density"),&Physics3DDirectBodyState::get_total_density);

	ObjectTypeDB::bind_method(_MD("get_inverse_mass"),&Physics3DDirectBodyState::get_inverse_mass);
	ObjectTypeDB::bind_method(_MD("get_inverse_inertia"),&Physics3DDirectBodyState::get_inverse_inertia);

	ObjectTypeDB::bind_method(_MD("set_linear_velocity","velocity"),&Physics3DDirectBodyState::set_linear_velocity);
	ObjectTypeDB::bind_method(_MD("get_linear_velocity"),&Physics3DDirectBodyState::get_linear_velocity);

	ObjectTypeDB::bind_method(_MD("set_angular_velocity","velocity"),&Physics3DDirectBodyState::set_angular_velocity);
	ObjectTypeDB::bind_method(_MD("get_angular_velocity"),&Physics3DDirectBodyState::get_angular_velocity);

	ObjectTypeDB::bind_method(_MD("set_transform","transform"),&Physics3DDirectBodyState::set_transform);
	ObjectTypeDB::bind_method(_MD("get_transform"),&Physics3DDirectBodyState::get_transform);

	ObjectTypeDB::bind_method(_MD("add_force","force","pos"),&Physics3DDirectBodyState::add_force);
	ObjectTypeDB::bind_method(_MD("apply_impulse","pos","j"),&Physics3DDirectBodyState::apply_impulse);

	ObjectTypeDB::bind_method(_MD("set_sleep_state","enabled"),&Physics3DDirectBodyState::set_sleep_state);
	ObjectTypeDB::bind_method(_MD("is_sleeping"),&Physics3DDirectBodyState::is_sleeping);

	ObjectTypeDB::bind_method(_MD("get_contact_count"),&Physics3DDirectBodyState::get_contact_count);

	ObjectTypeDB::bind_method(_MD("get_contact_local_pos","contact_idx"),&Physics3DDirectBodyState::get_contact_local_pos);
	ObjectTypeDB::bind_method(_MD("get_contact_local_normal","contact_idx"),&Physics3DDirectBodyState::get_contact_local_normal);
	ObjectTypeDB::bind_method(_MD("get_contact_local_shape","contact_idx"),&Physics3DDirectBodyState::get_contact_local_shape);
	ObjectTypeDB::bind_method(_MD("get_contact_collider","contact_idx"),&Physics3DDirectBodyState::get_contact_collider);
	ObjectTypeDB::bind_method(_MD("get_contact_collider_pos","contact_idx"),&Physics3DDirectBodyState::get_contact_collider_pos);
	ObjectTypeDB::bind_method(_MD("get_contact_collider_id","contact_idx"),&Physics3DDirectBodyState::get_contact_collider_id);
	ObjectTypeDB::bind_method(_MD("get_contact_collider_object","contact_idx"),&Physics3DDirectBodyState::get_contact_collider_object);
	ObjectTypeDB::bind_method(_MD("get_contact_collider_shape","contact_idx"),&Physics3DDirectBodyState::get_contact_collider_shape);
	ObjectTypeDB::bind_method(_MD("get_contact_collider_velocity_at_pos","contact_idx"),&Physics3DDirectBodyState::get_contact_collider_velocity_at_pos);
	ObjectTypeDB::bind_method(_MD("get_step"),&Physics3DDirectBodyState::get_step);
	ObjectTypeDB::bind_method(_MD("integrate_forces"),&Physics3DDirectBodyState::integrate_forces);
	ObjectTypeDB::bind_method(_MD("get_space_state:Physics3DDirectSpaceState"),&Physics3DDirectBodyState::get_space_state);

}

Physics3DDirectBodyState::Physics3DDirectBodyState() {}

///////////////////////////////////////////////////////



void Physics3DShapeQueryParameters::set_shape(const RES &p_shape) {

	ERR_FAIL_COND(p_shape.is_null());
	shape=p_shape->get_rid();
}

void Physics3DShapeQueryParameters::set_shape_rid(const RID& p_shape) {

	shape=p_shape;
}

RID Physics3DShapeQueryParameters::get_shape_rid() const {

	return shape;
}

void Physics3DShapeQueryParameters::set_transform(const Transform3D& p_transform){

	transform=p_transform;
}
Transform3D Physics3DShapeQueryParameters::get_transform() const{

	return transform;
}

void Physics3DShapeQueryParameters::set_margin(float p_margin){

	margin=p_margin;
}

float Physics3DShapeQueryParameters::get_margin() const{

	return margin;
}

void Physics3DShapeQueryParameters::set_layer_mask(int p_layer_mask){

	layer_mask=p_layer_mask;
}
int Physics3DShapeQueryParameters::get_layer_mask() const{

	return layer_mask;
}


void Physics3DShapeQueryParameters::set_object_type_mask(int p_object_type_mask){

	object_type_mask=p_object_type_mask;
}
int Physics3DShapeQueryParameters::get_object_type_mask() const{

	return object_type_mask;
}
void Physics3DShapeQueryParameters::set_exclude(const Vector<RID>& p_exclude) {

	exclude.clear();;
	for(int i=0;i<p_exclude.size();i++)
		exclude.insert(p_exclude[i]);

}

Vector<RID> Physics3DShapeQueryParameters::get_exclude() const{

	Vector<RID> ret;
	ret.resize(exclude.size());
	int idx=0;
	for(Set<RID>::Element *E=exclude.front();E;E=E->next()) {
		ret[idx]=E->get();
	}
	return ret;
}

void Physics3DShapeQueryParameters::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("set_shape","shape:Shape3D"),&Physics3DShapeQueryParameters::set_shape);
	ObjectTypeDB::bind_method(_MD("set_shape_rid","shape"),&Physics3DShapeQueryParameters::set_shape_rid);
	ObjectTypeDB::bind_method(_MD("get_shape_rid"),&Physics3DShapeQueryParameters::get_shape_rid);

	ObjectTypeDB::bind_method(_MD("set_transform","transform"),&Physics3DShapeQueryParameters::set_transform);
	ObjectTypeDB::bind_method(_MD("get_transform"),&Physics3DShapeQueryParameters::get_transform);

	ObjectTypeDB::bind_method(_MD("set_margin","margin"),&Physics3DShapeQueryParameters::set_margin);
	ObjectTypeDB::bind_method(_MD("get_margin"),&Physics3DShapeQueryParameters::get_margin);

	ObjectTypeDB::bind_method(_MD("set_layer_mask","layer_mask"),&Physics3DShapeQueryParameters::set_layer_mask);
	ObjectTypeDB::bind_method(_MD("get_layer_mask"),&Physics3DShapeQueryParameters::get_layer_mask);

	ObjectTypeDB::bind_method(_MD("set_object_type_mask","object_type_mask"),&Physics3DShapeQueryParameters::set_object_type_mask);
	ObjectTypeDB::bind_method(_MD("get_object_type_mask"),&Physics3DShapeQueryParameters::get_object_type_mask);

	ObjectTypeDB::bind_method(_MD("set_exclude","exclude"),&Physics3DShapeQueryParameters::set_exclude);
	ObjectTypeDB::bind_method(_MD("get_exclude"),&Physics3DShapeQueryParameters::get_exclude);


}

Physics3DShapeQueryParameters::Physics3DShapeQueryParameters() {

	margin=0;
	layer_mask=0x7FFFFFFF;
	object_type_mask=Physics3DDirectSpaceState::TYPE_MASK_COLLISION;
}



/////////////////////////////////////

/*
Variant Physics3DDirectSpaceState::_intersect_shape(const RID& p_shape, const Transform3D& p_xform,int p_result_max,const Vector<RID>& p_exclude,uint32_t p_user_mask) {



	ERR_FAIL_INDEX_V(p_result_max,4096,Variant());
	if (p_result_max<=0)
		return Variant();

	Set<RID> exclude;
	for(int i=0;i<p_exclude.size();i++)
		exclude.insert(p_exclude[i]);

	ShapeResult *res=(ShapeResult*)alloca(p_result_max*sizeof(ShapeResult));

	int rc = intersect_shape(p_shape,p_xform,0,res,p_result_max,exclude);

	if (rc==0)
		return Variant();

	Ref<Physics3DShapeQueryResult>  result = memnew( Physics3DShapeQueryResult );
	result->result.resize(rc);
	for(int i=0;i<rc;i++)
		result->result[i]=res[i];

	return result;

}
*/


Dictionary Physics3DDirectSpaceState::_intersect_ray(const Vector3& p_from, const Vector3& p_to,const Vector<RID>& p_exclude,uint32_t p_layers,uint32_t p_object_type_mask) {

	RayResult inters;
	Set<RID> exclude;
	for(int i=0;i<p_exclude.size();i++)
		exclude.insert(p_exclude[i]);

	bool res = intersect_ray(p_from,p_to,inters,exclude,p_layers,p_object_type_mask);

	if (!res)
		return Dictionary(true);

	Dictionary d(true);
	d["position"]=inters.position;
	d["normal"]=inters.normal;
	d["collider_id"]=inters.collider_id;
	d["collider"]=inters.collider;
	d["shape"]=inters.shape;
	d["rid"]=inters.rid;

	return d;
}

Array Physics3DDirectSpaceState::_intersect_shape(const Ref<Physics3DShapeQueryParameters> &psq, int p_max_results) {

	Vector<ShapeResult> sr;
	sr.resize(p_max_results);
	int rc = intersect_shape(psq->shape,psq->transform,psq->margin,sr.ptr(),sr.size(),psq->exclude,psq->layer_mask,psq->object_type_mask);
	Array ret;
	ret.resize(rc);
	for(int i=0;i<rc;i++) {

		Dictionary d;
		d["rid"]=sr[i].rid;
		d["collider_id"]=sr[i].collider_id;
		d["collider"]=sr[i].collider;
		d["shape"]=sr[i].shape;
		ret[i]=d;
	}

	return ret;
}

Array Physics3DDirectSpaceState::_cast_motion(const Ref<Physics3DShapeQueryParameters> &psq,const Vector3& p_motion){

	float closest_safe,closest_unsafe;
	bool res = cast_motion(psq->shape,psq->transform,p_motion,psq->margin,closest_safe,closest_unsafe,psq->exclude,psq->layer_mask,psq->object_type_mask);
	if (!res)
		return Array();
	Array ret(true);
	ret.resize(2);
	ret[0]=closest_safe;
	ret[0]=closest_unsafe;
	return ret;

}
Array Physics3DDirectSpaceState::_collide_shape(const Ref<Physics3DShapeQueryParameters> &psq, int p_max_results){

	Vector<Vector3> ret;
	ret.resize(p_max_results*2);
	int rc=0;
	bool res = collide_shape(psq->shape,psq->transform,psq->margin,ret.ptr(),p_max_results,rc,psq->exclude,psq->layer_mask,psq->object_type_mask);
	if (!res)
		return Array();
	Array r;
	r.resize(rc*2);
	for(int i=0;i<rc*2;i++)
		r[i]=ret[i];
	return r;

}
Dictionary Physics3DDirectSpaceState::_get_rest_info(const Ref<Physics3DShapeQueryParameters> &psq){

	ShapeRestInfo sri;

	bool res = rest_info(psq->shape,psq->transform,psq->margin,&sri,psq->exclude,psq->layer_mask,psq->object_type_mask);
	Dictionary r(true);
	if (!res)
		return r;

	r["point"]=sri.point;
	r["normal"]=sri.normal;
	r["rid"]=sri.rid;
	r["collider_id"]=sri.collider_id;
	r["shape"]=sri.shape;
	r["linear_velocity"]=sri.linear_velocity;

	return r;
}



Physics3DDirectSpaceState::Physics3DDirectSpaceState() {



}


void Physics3DDirectSpaceState::_bind_methods() {


//	ObjectTypeDB::bind_method(_MD("intersect_ray","from","to","exclude","umask"),&Physics3DDirectSpaceState::_intersect_ray,DEFVAL(Array()),DEFVAL(0));
//	ObjectTypeDB::bind_method(_MD("intersect_shape:Physics3DShapeQueryResult","shape","xform","result_max","exclude","umask"),&Physics3DDirectSpaceState::_intersect_shape,DEFVAL(Array()),DEFVAL(0));

	ObjectTypeDB::bind_method(_MD("intersect_ray:Dictionary","from","to","exclude","layer_mask","type_mask"),&Physics3DDirectSpaceState::_intersect_ray,DEFVAL(Array()),DEFVAL(0x7FFFFFFF),DEFVAL(TYPE_MASK_COLLISION));
	ObjectTypeDB::bind_method(_MD("intersect_shape","shape:Physics3DShapeQueryParameters","max_results"),&Physics3DDirectSpaceState::_intersect_shape,DEFVAL(32));
	ObjectTypeDB::bind_method(_MD("cast_motion","shape:Physics3DShapeQueryParameters","motion"),&Physics3DDirectSpaceState::_cast_motion);
	ObjectTypeDB::bind_method(_MD("collide_shape","shape:Physics3DShapeQueryParameters","max_results"),&Physics3DDirectSpaceState::_collide_shape,DEFVAL(32));
	ObjectTypeDB::bind_method(_MD("get_rest_info","shape:Physics3DShapeQueryParameters"),&Physics3DDirectSpaceState::_get_rest_info);


	BIND_CONSTANT( TYPE_MASK_STATIC_BODY );
	BIND_CONSTANT( TYPE_MASK_KINEMATIC_BODY );
	BIND_CONSTANT( TYPE_MASK_RIGID_BODY );
	BIND_CONSTANT( TYPE_MASK_CHARACTER_BODY );
	BIND_CONSTANT( TYPE_MASK_AREA );
	BIND_CONSTANT( TYPE_MASK_COLLISION );

}


int Physics3DShapeQueryResult::get_result_count() const {

	return result.size();
}
RID Physics3DShapeQueryResult::get_result_rid(int p_idx) const {

	return result[p_idx].rid;
}
ObjectID Physics3DShapeQueryResult::get_result_object_id(int p_idx) const {

	return result[p_idx].collider_id;
}
Object* Physics3DShapeQueryResult::get_result_object(int p_idx) const {

	return result[p_idx].collider;
}
int Physics3DShapeQueryResult::get_result_object_shape(int p_idx) const {

	return result[p_idx].shape;
}

Physics3DShapeQueryResult::Physics3DShapeQueryResult() {


}

void Physics3DShapeQueryResult::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("get_result_count"),&Physics3DShapeQueryResult::get_result_count);
	ObjectTypeDB::bind_method(_MD("get_result_rid","idx"),&Physics3DShapeQueryResult::get_result_rid);
	ObjectTypeDB::bind_method(_MD("get_result_object_id","idx"),&Physics3DShapeQueryResult::get_result_object_id);
	ObjectTypeDB::bind_method(_MD("get_result_object","idx"),&Physics3DShapeQueryResult::get_result_object);
	ObjectTypeDB::bind_method(_MD("get_result_object_shape","idx"),&Physics3DShapeQueryResult::get_result_object_shape);


}





///////////////////////////////////////

void PhysicsServer3D::_bind_methods() {


	ObjectTypeDB::bind_method(_MD("shape_create","type"),&PhysicsServer3D::shape_create);
	ObjectTypeDB::bind_method(_MD("shape_set_data","shape","data"),&PhysicsServer3D::shape_set_data);

	ObjectTypeDB::bind_method(_MD("shape_get_type","shape"),&PhysicsServer3D::shape_get_type);
	ObjectTypeDB::bind_method(_MD("shape_get_data","shape"),&PhysicsServer3D::shape_get_data);


	ObjectTypeDB::bind_method(_MD("space_create"),&PhysicsServer3D::space_create);
	ObjectTypeDB::bind_method(_MD("space_set_active","space","active"),&PhysicsServer3D::space_set_active);
	ObjectTypeDB::bind_method(_MD("space_is_active","space"),&PhysicsServer3D::space_is_active);
	ObjectTypeDB::bind_method(_MD("space_set_param","space","param","value"),&PhysicsServer3D::space_set_param);
	ObjectTypeDB::bind_method(_MD("space_get_param","space","param"),&PhysicsServer3D::space_get_param);
	ObjectTypeDB::bind_method(_MD("space_get_direct_state:Physics3DDirectSpaceState","space"),&PhysicsServer3D::space_get_direct_state);

	ObjectTypeDB::bind_method(_MD("area_create"),&PhysicsServer3D::area_create);
	ObjectTypeDB::bind_method(_MD("area_set_space","area","space"),&PhysicsServer3D::area_set_space);
	ObjectTypeDB::bind_method(_MD("area_get_space","area"),&PhysicsServer3D::area_get_space);

	ObjectTypeDB::bind_method(_MD("area_set_space_override_mode","area","mode"),&PhysicsServer3D::area_set_space_override_mode);
	ObjectTypeDB::bind_method(_MD("area_get_space_override_mode","area"),&PhysicsServer3D::area_get_space_override_mode);

	ObjectTypeDB::bind_method(_MD("area_add_shape","area","shape","transform"),&PhysicsServer3D::area_add_shape,DEFVAL(Transform3D()));
	ObjectTypeDB::bind_method(_MD("area_set_shape","area","shape_idx","shape"),&PhysicsServer3D::area_set_shape);
	ObjectTypeDB::bind_method(_MD("area_set_shape_transform","area","shape_idx","transform"),&PhysicsServer3D::area_set_shape_transform);

	ObjectTypeDB::bind_method(_MD("area_get_shape_count","area"),&PhysicsServer3D::area_get_shape_count);
	ObjectTypeDB::bind_method(_MD("area_get_shape","area","shape_idx"),&PhysicsServer3D::area_get_shape);
	ObjectTypeDB::bind_method(_MD("area_get_shape_transform","area","shape_idx"),&PhysicsServer3D::area_get_shape_transform);

	ObjectTypeDB::bind_method(_MD("area_remove_shape","area","shape_idx"),&PhysicsServer3D::area_remove_shape);
	ObjectTypeDB::bind_method(_MD("area_clear_shapes","area"),&PhysicsServer3D::area_clear_shapes);


	ObjectTypeDB::bind_method(_MD("area_set_param","area","param","value"),&PhysicsServer3D::area_set_param);
	ObjectTypeDB::bind_method(_MD("area_set_transform","area","transform"),&PhysicsServer3D::area_set_transform);

	ObjectTypeDB::bind_method(_MD("area_get_param","area","param"),&PhysicsServer3D::area_get_param);
	ObjectTypeDB::bind_method(_MD("area_get_transform","area"),&PhysicsServer3D::area_get_transform);

	ObjectTypeDB::bind_method(_MD("area_attach_object_instance_ID","area","id"),&PhysicsServer3D::area_attach_object_instance_ID);
	ObjectTypeDB::bind_method(_MD("area_get_object_instance_ID","area"),&PhysicsServer3D::area_get_object_instance_ID);

	ObjectTypeDB::bind_method(_MD("area_set_monitor_callback","receiver","method"),&PhysicsServer3D::area_set_monitor_callback);

	ObjectTypeDB::bind_method(_MD("area_set_ray_pickable","area","enable"),&PhysicsServer3D::area_set_ray_pickable);
	ObjectTypeDB::bind_method(_MD("area_is_ray_pickable","area"),&PhysicsServer3D::area_is_ray_pickable);

	ObjectTypeDB::bind_method(_MD("body_create","mode","init_sleeping"),&PhysicsServer3D::body_create,DEFVAL(BODY_MODE_RIGID),DEFVAL(false));

	ObjectTypeDB::bind_method(_MD("body_set_space","body","space"),&PhysicsServer3D::body_set_space);
	ObjectTypeDB::bind_method(_MD("body_get_space","body"),&PhysicsServer3D::body_get_space);

	ObjectTypeDB::bind_method(_MD("body_set_mode","body","mode"),&PhysicsServer3D::body_set_mode);
	ObjectTypeDB::bind_method(_MD("body_get_mode","body"),&PhysicsServer3D::body_get_mode);

	ObjectTypeDB::bind_method(_MD("body_add_shape","body","shape","transform"),&PhysicsServer3D::body_add_shape,DEFVAL(Transform3D()));
	ObjectTypeDB::bind_method(_MD("body_set_shape","body","shape_idx","shape"),&PhysicsServer3D::body_set_shape);
	ObjectTypeDB::bind_method(_MD("body_set_shape_transform","body","shape_idx","transform"),&PhysicsServer3D::body_set_shape_transform);

	ObjectTypeDB::bind_method(_MD("body_get_shape_count","body"),&PhysicsServer3D::body_get_shape_count);
	ObjectTypeDB::bind_method(_MD("body_get_shape","body","shape_idx"),&PhysicsServer3D::body_get_shape);
	ObjectTypeDB::bind_method(_MD("body_get_shape_transform","body","shape_idx"),&PhysicsServer3D::body_get_shape_transform);

	ObjectTypeDB::bind_method(_MD("body_remove_shape","body","shape_idx"),&PhysicsServer3D::body_remove_shape);
	ObjectTypeDB::bind_method(_MD("body_clear_shapes","body"),&PhysicsServer3D::body_clear_shapes);

	ObjectTypeDB::bind_method(_MD("body_attach_object_instance_ID","body","id"),&PhysicsServer3D::body_attach_object_instance_ID);
	ObjectTypeDB::bind_method(_MD("body_get_object_instance_ID","body"),&PhysicsServer3D::body_get_object_instance_ID);


	ObjectTypeDB::bind_method(_MD("body_set_enable_continuous_collision_detection","body","enable"),&PhysicsServer3D::body_set_enable_continuous_collision_detection);
	ObjectTypeDB::bind_method(_MD("body_is_continuous_collision_detection_enabled","body"),&PhysicsServer3D::body_is_continuous_collision_detection_enabled);


	//ObjectTypeDB::bind_method(_MD("body_set_user_flags","flags""),&PhysicsServer3D::body_set_shape,DEFVAL(Transform3D));
	//ObjectTypeDB::bind_method(_MD("body_get_user_flags","body","shape_idx","shape"),&PhysicsServer3D::body_get_shape);

	ObjectTypeDB::bind_method(_MD("body_set_param","body","param","value"),&PhysicsServer3D::body_set_param);
	ObjectTypeDB::bind_method(_MD("body_get_param","body","param"),&PhysicsServer3D::body_get_param);

	ObjectTypeDB::bind_method(_MD("body_set_state","body","state","value"),&PhysicsServer3D::body_set_state);
	ObjectTypeDB::bind_method(_MD("body_get_state","body","state"),&PhysicsServer3D::body_get_state);

	ObjectTypeDB::bind_method(_MD("body_apply_impulse","body","pos","impulse"),&PhysicsServer3D::body_apply_impulse);
	ObjectTypeDB::bind_method(_MD("body_set_axis_velocity","body","axis_velocity"),&PhysicsServer3D::body_set_axis_velocity);

	ObjectTypeDB::bind_method(_MD("body_set_axis_lock","body","axis"),&PhysicsServer3D::body_set_axis_lock);
	ObjectTypeDB::bind_method(_MD("body_get_axis_lock","body"),&PhysicsServer3D::body_get_axis_lock);

	ObjectTypeDB::bind_method(_MD("body_add_collision_exception","body","excepted_body"),&PhysicsServer3D::body_add_collision_exception);
	ObjectTypeDB::bind_method(_MD("body_remove_collision_exception","body","excepted_body"),&PhysicsServer3D::body_remove_collision_exception);
//	virtual void body_get_collision_exceptions(RID p_body, List<RID> *p_exceptions)=0;

	ObjectTypeDB::bind_method(_MD("body_set_max_contacts_reported","body","amount"),&PhysicsServer3D::body_set_max_contacts_reported);
	ObjectTypeDB::bind_method(_MD("body_get_max_contacts_reported","body"),&PhysicsServer3D::body_get_max_contacts_reported);

	ObjectTypeDB::bind_method(_MD("body_set_omit_force_integration","body","enable"),&PhysicsServer3D::body_set_omit_force_integration);
	ObjectTypeDB::bind_method(_MD("body_is_omitting_force_integration","body"),&PhysicsServer3D::body_is_omitting_force_integration);

	ObjectTypeDB::bind_method(_MD("body_set_force_integration_callback","body","receiver","method","userdata"),&PhysicsServer3D::body_set_force_integration_callback,DEFVAL(Variant()));

	ObjectTypeDB::bind_method(_MD("body_set_ray_pickable","body","enable"),&PhysicsServer3D::body_set_ray_pickable);
	ObjectTypeDB::bind_method(_MD("body_is_ray_pickable","body"),&PhysicsServer3D::body_is_ray_pickable);

	/* JOINT API */

	BIND_CONSTANT( JOINT_PIN );
	BIND_CONSTANT( JOINT_HINGE );
	BIND_CONSTANT( JOINT_SLIDER );
	BIND_CONSTANT( JOINT_CONE_TWIST );
	BIND_CONSTANT( JOINT_6DOF );

	ObjectTypeDB::bind_method(_MD("joint_create_pin","body_A","local_A","body_B","local_B"),&PhysicsServer3D::joint_create_pin);
	ObjectTypeDB::bind_method(_MD("pin_joint_set_param","joint","param","value"),&PhysicsServer3D::pin_joint_set_param);
	ObjectTypeDB::bind_method(_MD("pin_joint_get_param","joint","param"),&PhysicsServer3D::pin_joint_get_param);

	ObjectTypeDB::bind_method(_MD("pin_joint_set_local_A","joint","local_A"),&PhysicsServer3D::pin_joint_set_local_A);
	ObjectTypeDB::bind_method(_MD("pin_joint_get_local_A","joint"),&PhysicsServer3D::pin_joint_get_local_A);

	ObjectTypeDB::bind_method(_MD("pin_joint_set_local_B","joint","local_B"),&PhysicsServer3D::pin_joint_set_local_B);
	ObjectTypeDB::bind_method(_MD("pin_joint_get_local_B","joint"),&PhysicsServer3D::pin_joint_get_local_B);

	BIND_CONSTANT(PIN_JOINT_BIAS );
	BIND_CONSTANT(PIN_JOINT_DAMPING );
	BIND_CONSTANT(PIN_JOINT_IMPULSE_CLAMP );


	BIND_CONSTANT(HINGE_JOINT_BIAS);
	BIND_CONSTANT(HINGE_JOINT_LIMIT_UPPER);
	BIND_CONSTANT(HINGE_JOINT_LIMIT_LOWER);
	BIND_CONSTANT(HINGE_JOINT_LIMIT_BIAS);
	BIND_CONSTANT(HINGE_JOINT_LIMIT_SOFTNESS);
	BIND_CONSTANT(HINGE_JOINT_LIMIT_RELAXATION);
	BIND_CONSTANT(HINGE_JOINT_MOTOR_TARGET_VELOCITY);
	BIND_CONSTANT(HINGE_JOINT_MOTOR_MAX_IMPULSE);
	BIND_CONSTANT(HINGE_JOINT_FLAG_USE_LIMIT);
	BIND_CONSTANT(HINGE_JOINT_FLAG_ENABLE_MOTOR);

	ObjectTypeDB::bind_method(_MD("joint_create_hinge","body_A","hinge_A","body_B","hinge_B"),&PhysicsServer3D::joint_create_hinge);

	ObjectTypeDB::bind_method(_MD("hinge_joint_set_param","joint","param","value"),&PhysicsServer3D::hinge_joint_set_param);
	ObjectTypeDB::bind_method(_MD("hinge_joint_get_param","joint","param"),&PhysicsServer3D::hinge_joint_get_param);

	ObjectTypeDB::bind_method(_MD("hinge_joint_set_flag","joint","flag","enabled"),&PhysicsServer3D::hinge_joint_set_flag);
	ObjectTypeDB::bind_method(_MD("hinge_joint_get_flag","joint","flag"),&PhysicsServer3D::hinge_joint_get_flag);

	ObjectTypeDB::bind_method(_MD("joint_create_slider","body_A","local_ref_A","body_B","local_ref_B"),&PhysicsServer3D::joint_create_slider);

	ObjectTypeDB::bind_method(_MD("slider_joint_set_param","joint","param","value"),&PhysicsServer3D::slider_joint_set_param);
	ObjectTypeDB::bind_method(_MD("slider_joint_get_param","joint","param"),&PhysicsServer3D::slider_joint_get_param);

	BIND_CONSTANT( SLIDER_JOINT_LINEAR_LIMIT_UPPER );
	BIND_CONSTANT( SLIDER_JOINT_LINEAR_LIMIT_LOWER );
	BIND_CONSTANT( SLIDER_JOINT_LINEAR_LIMIT_SOFTNESS );
	BIND_CONSTANT( SLIDER_JOINT_LINEAR_LIMIT_RESTITUTION );
	BIND_CONSTANT( SLIDER_JOINT_LINEAR_LIMIT_DAMPING );
	BIND_CONSTANT( SLIDER_JOINT_LINEAR_MOTION_SOFTNESS );
	BIND_CONSTANT( SLIDER_JOINT_LINEAR_MOTION_RESTITUTION );
	BIND_CONSTANT( SLIDER_JOINT_LINEAR_MOTION_DAMPING );
	BIND_CONSTANT( SLIDER_JOINT_LINEAR_ORTHOGONAL_SOFTNESS );
	BIND_CONSTANT( SLIDER_JOINT_LINEAR_ORTHOGONAL_RESTITUTION );
	BIND_CONSTANT( SLIDER_JOINT_LINEAR_ORTHOGONAL_DAMPING );

	BIND_CONSTANT( SLIDER_JOINT_ANGULAR_LIMIT_UPPER );
	BIND_CONSTANT( SLIDER_JOINT_ANGULAR_LIMIT_LOWER );
	BIND_CONSTANT( SLIDER_JOINT_ANGULAR_LIMIT_SOFTNESS );
	BIND_CONSTANT( SLIDER_JOINT_ANGULAR_LIMIT_RESTITUTION );
	BIND_CONSTANT( SLIDER_JOINT_ANGULAR_LIMIT_DAMPING );
	BIND_CONSTANT( SLIDER_JOINT_ANGULAR_MOTION_SOFTNESS );
	BIND_CONSTANT( SLIDER_JOINT_ANGULAR_MOTION_RESTITUTION );
	BIND_CONSTANT( SLIDER_JOINT_ANGULAR_MOTION_DAMPING );
	BIND_CONSTANT( SLIDER_JOINT_ANGULAR_ORTHOGONAL_SOFTNESS );
	BIND_CONSTANT( SLIDER_JOINT_ANGULAR_ORTHOGONAL_RESTITUTION );
	BIND_CONSTANT( SLIDER_JOINT_ANGULAR_ORTHOGONAL_DAMPING );
	BIND_CONSTANT( SLIDER_JOINT_MAX );


	ObjectTypeDB::bind_method(_MD("joint_create_cone_twist","body_A","local_ref_A","body_B","local_ref_B"),&PhysicsServer3D::joint_create_cone_twist);

	ObjectTypeDB::bind_method(_MD("cone_twist_joint_set_param","joint","param","value"),&PhysicsServer3D::cone_twist_joint_set_param);
	ObjectTypeDB::bind_method(_MD("cone_twist_joint_get_param","joint","param"),&PhysicsServer3D::cone_twist_joint_get_param);

	BIND_CONSTANT( CONE_TWIST_JOINT_SWING_SPAN );
	BIND_CONSTANT( CONE_TWIST_JOINT_TWIST_SPAN );
	BIND_CONSTANT( CONE_TWIST_JOINT_BIAS );
	BIND_CONSTANT( CONE_TWIST_JOINT_SOFTNESS );
	BIND_CONSTANT( CONE_TWIST_JOINT_RELAXATION );


	BIND_CONSTANT( G6DOF_JOINT_LINEAR_LOWER_LIMIT );
	BIND_CONSTANT( G6DOF_JOINT_LINEAR_UPPER_LIMIT );
	BIND_CONSTANT( G6DOF_JOINT_LINEAR_LIMIT_SOFTNESS );
	BIND_CONSTANT( G6DOF_JOINT_LINEAR_RESTITUTION );
	BIND_CONSTANT( G6DOF_JOINT_LINEAR_DAMPING );
	BIND_CONSTANT( G6DOF_JOINT_ANGULAR_LOWER_LIMIT );
	BIND_CONSTANT( G6DOF_JOINT_ANGULAR_UPPER_LIMIT );
	BIND_CONSTANT( G6DOF_JOINT_ANGULAR_LIMIT_SOFTNESS );
	BIND_CONSTANT( G6DOF_JOINT_ANGULAR_DAMPING );
	BIND_CONSTANT( G6DOF_JOINT_ANGULAR_RESTITUTION );
	BIND_CONSTANT( G6DOF_JOINT_ANGULAR_FORCE_LIMIT );
	BIND_CONSTANT( G6DOF_JOINT_ANGULAR_ERP );
	BIND_CONSTANT( G6DOF_JOINT_ANGULAR_MOTOR_TARGET_VELOCITY );
	BIND_CONSTANT( G6DOF_JOINT_ANGULAR_MOTOR_FORCE_LIMIT );


	BIND_CONSTANT( G6DOF_JOINT_FLAG_ENABLE_LINEAR_LIMIT );
	BIND_CONSTANT( G6DOF_JOINT_FLAG_ENABLE_ANGULAR_LIMIT );
	BIND_CONSTANT( G6DOF_JOINT_FLAG_ENABLE_MOTOR );

	ObjectTypeDB::bind_method(_MD("joint_get_type","joint"),&PhysicsServer3D::joint_get_type);

	ObjectTypeDB::bind_method(_MD("joint_set_solver_priority","joint","priority"),&PhysicsServer3D::joint_set_solver_priority);
	ObjectTypeDB::bind_method(_MD("joint_get_solver_priority","joint"),&PhysicsServer3D::joint_get_solver_priority);

	ObjectTypeDB::bind_method(_MD("joint_create_generic_6dof","body_A","local_ref_A","body_B","local_ref_B"),&PhysicsServer3D::joint_create_generic_6dof);

	ObjectTypeDB::bind_method(_MD("generic_6dof_joint_set_param","joint","axis","param","value"),&PhysicsServer3D::generic_6dof_joint_set_param);
	ObjectTypeDB::bind_method(_MD("generic_6dof_joint_get_param","joint","axis","param"),&PhysicsServer3D::generic_6dof_joint_get_param);

	ObjectTypeDB::bind_method(_MD("generic_6dof_joint_set_flag","joint","axis","flag","enable"),&PhysicsServer3D::generic_6dof_joint_set_flag);
	ObjectTypeDB::bind_method(_MD("generic_6dof_joint_get_flag","joint","axis","flag"),&PhysicsServer3D::generic_6dof_joint_get_flag);


/*
	ObjectTypeDB::bind_method(_MD("joint_set_param","joint","param","value"),&PhysicsServer3D::joint_set_param);
	ObjectTypeDB::bind_method(_MD("joint_get_param","joint","param"),&PhysicsServer3D::joint_get_param);

	ObjectTypeDB::bind_method(_MD("pin_joint_create","anchor","body_a","body_b"),&PhysicsServer3D::pin_joint_create,DEFVAL(RID()));
	ObjectTypeDB::bind_method(_MD("groove_joint_create","groove1_a","groove2_a","anchor_b","body_a","body_b"),&PhysicsServer3D::groove_joint_create,DEFVAL(RID()),DEFVAL(RID()));
	ObjectTypeDB::bind_method(_MD("damped_spring_joint_create","anchor_a","anchor_b","body_a","body_b"),&PhysicsServer3D::damped_spring_joint_create,DEFVAL(RID()));

	ObjectTypeDB::bind_method(_MD("damped_string_joint_set_param","joint","param","value"),&PhysicsServer3D::damped_string_joint_set_param,DEFVAL(RID()));
	ObjectTypeDB::bind_method(_MD("damped_string_joint_get_param","joint","param"),&PhysicsServer3D::damped_string_joint_get_param);

	ObjectTypeDB::bind_method(_MD("joint_get_type","joint"),&PhysicsServer3D::joint_get_type);
*/
	ObjectTypeDB::bind_method(_MD("free_rid","rid"),&PhysicsServer3D::free);

	ObjectTypeDB::bind_method(_MD("set_active","active"),&PhysicsServer3D::set_active);

//	ObjectTypeDB::bind_method(_MD("init"),&PhysicsServer3D::init);
//	ObjectTypeDB::bind_method(_MD("step"),&PhysicsServer3D::step);
//	ObjectTypeDB::bind_method(_MD("sync"),&PhysicsServer3D::sync);
	//ObjectTypeDB::bind_method(_MD("flush_queries"),&PhysicsServer3D::flush_queries);


	ObjectTypeDB::bind_method(_MD("get_process_info"),&PhysicsServer3D::get_process_info);

	BIND_CONSTANT( SHAPE_PLANE );
	BIND_CONSTANT( SHAPE_RAY );
	BIND_CONSTANT( SHAPE_SPHERE );
	BIND_CONSTANT( SHAPE_BOX );
	BIND_CONSTANT( SHAPE_CAPSULE );
	BIND_CONSTANT( SHAPE_CONVEX_POLYGON );
	BIND_CONSTANT( SHAPE_CONCAVE_POLYGON );
	BIND_CONSTANT( SHAPE_HEIGHTMAP );
	BIND_CONSTANT( SHAPE_CUSTOM );


	BIND_CONSTANT( AREA_PARAM_GRAVITY );
	BIND_CONSTANT( AREA_PARAM_GRAVITY_VECTOR );
	BIND_CONSTANT( AREA_PARAM_GRAVITY_IS_POINT );
	BIND_CONSTANT( AREA_PARAM_GRAVITY_POINT_ATTENUATION );
	BIND_CONSTANT( AREA_PARAM_DENSITY );
	BIND_CONSTANT( AREA_PARAM_PRIORITY );

	BIND_CONSTANT( AREA_SPACE_OVERRIDE_COMBINE );
	BIND_CONSTANT( AREA_SPACE_OVERRIDE_DISABLED );
	BIND_CONSTANT( AREA_SPACE_OVERRIDE_REPLACE );

	BIND_CONSTANT( BODY_MODE_STATIC );
	BIND_CONSTANT( BODY_MODE_KINEMATIC );
	BIND_CONSTANT( BODY_MODE_RIGID );
	BIND_CONSTANT( BODY_MODE_CHARACTER );

	BIND_CONSTANT( BODY_PARAM_BOUNCE );
	BIND_CONSTANT( BODY_PARAM_FRICTION );
	BIND_CONSTANT( BODY_PARAM_MASS );
	BIND_CONSTANT( BODY_PARAM_MAX );

	BIND_CONSTANT( BODY_STATE_TRANSFORM );
	BIND_CONSTANT( BODY_STATE_LINEAR_VELOCITY );
	BIND_CONSTANT( BODY_STATE_ANGULAR_VELOCITY );
	BIND_CONSTANT( BODY_STATE_SLEEPING );
	BIND_CONSTANT( BODY_STATE_CAN_SLEEP );
/*
	BIND_CONSTANT( JOINT_PIN );
	BIND_CONSTANT( JOINT_GROOVE );
	BIND_CONSTANT( JOINT_DAMPED_SPRING );

	BIND_CONSTANT( DAMPED_STRING_REST_LENGTH );
	BIND_CONSTANT( DAMPED_STRING_STIFFNESS );
	BIND_CONSTANT( DAMPED_STRING_DAMPING );
*/
//	BIND_CONSTANT( TYPE_BODY );
//	BIND_CONSTANT( TYPE_AREA );

	BIND_CONSTANT( AREA_BODY_ADDED );
	BIND_CONSTANT( AREA_BODY_REMOVED );

	BIND_CONSTANT( INFO_ACTIVE_OBJECTS );
	BIND_CONSTANT( INFO_COLLISION_PAIRS );
	BIND_CONSTANT( INFO_ISLAND_COUNT );

}
