#include "servers/physics_3d_server.h"

class Physics3DDirectBodyStateDummy : public Physics3DDirectBodyState {
	OBJ_TYPE( Physics3DDirectBodyStateDummy, Physics3DDirectBodyState );

public:

	// get gravity vector working on this body space/area
	virtual Vector3 get_total_gravity() const{return Vector3();}
	// get density of this body space/area
	virtual float get_total_density() const{return 0.0f;}

	virtual float get_inverse_mass() const{return 0.0f;} // get the mass
	virtual Vector3 get_inverse_inertia() const{return Vector3();} // get density of this body space
	virtual Basis get_inverse_inertia_tensor() const{return Basis();} // get density of this body space

	virtual void set_linear_velocity(const Vector3& p_velocity){};
	virtual Vector3 get_linear_velocity() const{return Vector3();}

	virtual void set_angular_velocity(const Vector3& p_velocity){}
	virtual Vector3 get_angular_velocity() const{return Vector3();}

	virtual void set_transform(const Transform3D& p_transform){}
	virtual Transform3D get_transform() const{return Transform3D();}

	virtual void add_force(const Vector3& p_force, const Vector3& p_pos){}
	virtual void apply_impulse(const Vector3& p_pos, const Vector3& p_j){}

	virtual void set_sleep_state(bool p_enable){}
	virtual bool is_sleeping() const{return true;}

	virtual int get_contact_count() const{return 0;}

	virtual Vector3 get_contact_local_pos(int p_contact_idx) const{return Vector3();}
	virtual Vector3 get_contact_local_normal(int p_contact_idx) const{return Vector3();}
	virtual int get_contact_local_shape(int p_contact_idx) const{return 0;}

	virtual RID get_contact_collider(int p_contact_idx) const {return RID();}
	virtual Vector3 get_contact_collider_pos(int p_contact_idx) const{return Vector3();}
	virtual ObjectID get_contact_collider_id(int p_contact_idx) const{return ObjectID();}
	virtual Object* get_contact_collider_object(int p_contact_idx) const{return NULL;};
	virtual int get_contact_collider_shape(int p_contact_idx) const{return 0;}
	virtual Vector3 get_contact_collider_velocity_at_pos(int p_contact_idx) const{return Vector3();}

	virtual real_t get_step() const{return 0.0f;}
	virtual void integrate_forces(){}

	virtual Physics3DDirectSpaceState* get_space_state(){return NULL;}

};

class Physics3DDirectSpaceStateDummy : public Physics3DDirectSpaceState {
	OBJ_TYPE( Physics3DDirectSpaceStateDummy, Physics3DDirectSpaceState );

public:
	virtual bool intersect_ray(const Vector3& p_from, const Vector3& p_to,RayResult &r_result,const Set<RID>& p_exclude=Set<RID>(),uint32_t p_layer_mask=0xFFFFFFFF,uint32_t p_object_type_mask=TYPE_MASK_COLLISION){return false;}

	virtual int intersect_shape(const RID& p_shape, const Transform3D& p_xform,float p_margin,ShapeResult *r_results,int p_result_max,const Set<RID>& p_exclude=Set<RID>(),uint32_t p_layer_mask=0xFFFFFFFF,uint32_t p_object_type_mask=TYPE_MASK_COLLISION){return 0;}

	virtual bool cast_motion(const RID& p_shape, const Transform3D& p_xform,const Vector3& p_motion,float p_margin,float &p_closest_safe,float &p_closest_unsafe, const Set<RID>& p_exclude=Set<RID>(),uint32_t p_layer_mask=0xFFFFFFFF,uint32_t p_object_type_mask=TYPE_MASK_COLLISION,ShapeRestInfo *r_info=NULL){return false;}

	virtual bool collide_shape(RID p_shape, const Transform3D& p_shape_xform,float p_margin,Vector3 *r_results,int p_result_max,int &r_result_count, const Set<RID>& p_exclude=Set<RID>(),uint32_t p_layer_mask=0xFFFFFFFF,uint32_t p_object_type_mask=TYPE_MASK_COLLISION){return false;}

	virtual bool rest_info(RID p_shape, const Transform3D& p_shape_xform,float p_margin,ShapeRestInfo *r_info, const Set<RID>& p_exclude=Set<RID>(),uint32_t p_layer_mask=0xFFFFFFFF,uint32_t p_object_type_mask=TYPE_MASK_COLLISION){return false;}
};


class PhysicsServer3DDummy : public PhysicsServer3D {

	OBJ_TYPE( PhysicsServer3DDummy, PhysicsServer3D);

	virtual RID shape_create(ShapeType p_shape){return RID();}
	virtual void shape_set_data(RID p_shape, const Variant& p_data){}
	virtual void shape_set_custom_solver_bias(RID p_shape, real_t p_bias){}

	virtual ShapeType shape_get_type(RID p_shape) const{return SHAPE_CUSTOM;}
	virtual Variant shape_get_data(RID p_shape) const{return 0;}
	virtual real_t shape_get_custom_solver_bias(RID p_shape) const{return 0.0f;}

	/* SPACE API */

	virtual RID space_create(){return RID();}
	virtual void space_set_active(RID p_space,bool p_active){}
	virtual bool space_is_active(RID p_space) const{return false;}

	virtual void space_set_param(RID p_space,SpaceParameter p_param, real_t p_value){}
	virtual real_t space_get_param(RID p_space,SpaceParameter p_param) const{return 0.0f;}

	// this function only works on fixed process, errors and returns null otherwise
	virtual Physics3DDirectSpaceState* space_get_direct_state(RID p_space){return NULL;}

	//missing space parameters

	/* AREA API */

	//missing attenuation? missing better override?

	virtual RID area_create(){return RID();}

	virtual void area_set_space(RID p_area, RID p_space){}
	virtual RID area_get_space(RID p_area) const{return RID();}

    virtual void area_set_space_override_mode(RID p_area, AreaSpaceOverrideMode p_mode){}
	virtual AreaSpaceOverrideMode area_get_space_override_mode(RID p_area) const{return AREA_SPACE_OVERRIDE_DISABLED;}

	virtual void area_add_shape(RID p_area, RID p_shape, const Transform3D& p_transform=Transform3D()){}
	virtual void area_set_shape(RID p_area, int p_shape_idx,RID p_shape){}
	virtual void area_set_shape_transform(RID p_area, int p_shape_idx, const Transform3D& p_transform){}

	virtual int area_get_shape_count(RID p_area) const{return 0;}
	virtual RID area_get_shape(RID p_area, int p_shape_idx) const{return RID();}
	virtual Transform3D area_get_shape_transform(RID p_area, int p_shape_idx) const{return Transform3D();}

	virtual void area_remove_shape(RID p_area, int p_shape_idx){}
	virtual void area_clear_shapes(RID p_area){}

	virtual void area_attach_object_instance_ID(RID p_area,ObjectID p_ID){}
	virtual ObjectID area_get_object_instance_ID(RID p_area) const{return ObjectID();}

	virtual void area_set_param(RID p_area,AreaParameter p_param,const Variant& p_value){}
	virtual void area_set_transform(RID p_area, const Transform3D& p_transform){}

	virtual Variant area_get_param(RID p_parea,AreaParameter p_param) const{return 0;}
	virtual Transform3D area_get_transform(RID p_area) const{return Transform3D();}

	virtual void area_set_monitor_callback(RID p_area,Object *p_receiver,const StringName& p_method){}
	virtual void area_set_area_monitor_callback(RID p_area,Object *p_receiver,const StringName& p_method){}
	virtual void area_set_monitorable(RID p_area,bool p_monitorable){}

	virtual void area_set_ray_pickable(RID p_area,bool p_enable){}
	virtual bool area_is_ray_pickable(RID p_area) const{return false;}

	/* BODY API */

	virtual RID body_create(BodyMode p_mode,bool p_init_sleeping){return RID();}

	virtual void body_set_space(RID p_body, RID p_space){}
	virtual RID body_get_space(RID p_body) const{return RID();}

	virtual void body_set_mode(RID p_body, BodyMode p_mode){}
	virtual BodyMode body_get_mode(RID p_body, BodyMode p_mode) const{return BODY_MODE_STATIC;}

	virtual void body_add_shape(RID p_body, RID p_shape, const Transform3D& p_transform=Transform3D()){}
	virtual void body_set_shape(RID p_body, int p_shape_idx,RID p_shape){}
	virtual void body_set_shape_transform(RID p_body, int p_shape_idx, const Transform3D& p_transform){}

	virtual int body_get_shape_count(RID p_body) const{return 0;}
	virtual RID body_get_shape(RID p_body, int p_shape_idx) const{return RID();}
	virtual Transform3D body_get_shape_transform(RID p_body, int p_shape_idx) const{return Transform3D();}

	virtual void body_set_shape_as_trigger(RID p_body, int p_shape_idx,bool p_enable){}
	virtual bool body_is_shape_set_as_trigger(RID p_body, int p_shape_idx) const{return false;}

	virtual void body_remove_shape(RID p_body, int p_shape_idx){}
	virtual void body_clear_shapes(RID p_body){}

	virtual void body_attach_object_instance_ID(RID p_body,uint32_t p_ID){}
	virtual uint32_t body_get_object_instance_ID(RID p_body) const{return 0;}

	virtual void body_set_enable_continuous_collision_detection(RID p_body,bool p_enable){}
	virtual bool body_is_continuous_collision_detection_enabled(RID p_body) const{return false;}

	virtual void body_set_layer_mask(RID p_body, uint32_t p_mask){}
	virtual uint32_t body_get_layer_mask(RID p_body, uint32_t p_mask) const{return 0;}

	virtual void body_set_user_flags(RID p_body, uint32_t p_flags){}
	virtual uint32_t body_get_user_flags(RID p_body, uint32_t p_flags) const{return 0;}

	virtual void body_set_param(RID p_body, BodyParameter p_param, float p_value){}
	virtual float body_get_param(RID p_body, BodyParameter p_param) const{return 0.0f;}

	virtual void body_set_state(RID p_body, BodyState p_state, const Variant& p_variant){}
	virtual Variant body_get_state(RID p_body, BodyState p_state) const{return 0;}

	//do something about it
	virtual void body_set_applied_force(RID p_body, const Vector3& p_force){}
	virtual Vector3 body_get_applied_force(RID p_body) const{return Vector3();}

	virtual void body_set_applied_torque(RID p_body, const Vector3& p_torque){}
	virtual Vector3 body_get_applied_torque(RID p_body) const{return Vector3();}

	virtual void body_apply_impulse(RID p_body, const Vector3& p_pos, const Vector3& p_impulse){}
	virtual void body_set_axis_velocity(RID p_body, const Vector3& p_axis_velocity){}

	virtual void body_set_axis_lock(RID p_body,BodyAxisLock p_lock){}
	virtual BodyAxisLock body_get_axis_lock(RID p_body) const{return BODY_AXIS_LOCK_DISABLED;}

	//fix
	virtual void body_add_collision_exception(RID p_body, RID p_body_b){}
	virtual void body_remove_collision_exception(RID p_body, RID p_body_b){}
	virtual void body_get_collision_exceptions(RID p_body, List<RID> *p_exceptions){}

	virtual void body_set_max_contacts_reported(RID p_body, int p_contacts){}
	virtual int body_get_max_contacts_reported(RID p_body) const{return 0;}

	//missing remove
	virtual void body_set_contacts_reported_depth_treshold(RID p_body, float p_treshold){}
	virtual float body_get_contacts_reported_depth_treshold(RID p_body) const{return 0.0f;}

	virtual void body_set_omit_force_integration(RID p_body,bool p_omit){}
	virtual bool body_is_omitting_force_integration(RID p_body) const{return false;}

	virtual void body_set_force_integration_callback(RID p_body,Object *p_receiver,const StringName& p_method,const Variant& p_udata=Variant()){}

	virtual void body_set_ray_pickable(RID p_body,bool p_enable){}
	virtual bool body_is_ray_pickable(RID p_body) const{return false;}

	/* JOINT API */

	virtual JointType joint_get_type(RID p_joint) const{return JOINT_PIN;}

	virtual void joint_set_solver_priority(RID p_joint,int p_priority){}
	virtual int joint_get_solver_priority(RID p_joint) const{return 0;}


	virtual RID joint_create_pin(RID p_body_A,const Vector3& p_local_A,RID p_body_B,const Vector3& p_local_B){return RID();}

	virtual void pin_joint_set_param(RID p_joint,PinJointParam p_param, float p_value){}
	virtual float pin_joint_get_param(RID p_joint,PinJointParam p_param) const{return 0.0f;}

	virtual void pin_joint_set_local_A(RID p_joint, const Vector3& p_A){}
	virtual Vector3 pin_joint_get_local_A(RID p_joint) const{return Vector3();}

	virtual void pin_joint_set_local_B(RID p_joint, const Vector3& p_B){}
	virtual Vector3 pin_joint_get_local_B(RID p_joint) const{return Vector3();}

	virtual RID joint_create_hinge(RID p_body_A,const Transform3D& p_hinge_A,RID p_body_B,const Transform3D& p_hinge_B){return RID();}
	virtual RID joint_create_hinge_simple(RID p_body_A,const Vector3& p_pivot_A,const Vector3& p_axis_A,RID p_body_B,const Vector3& p_pivot_B,const Vector3& p_axis_B){return RID();}


	virtual void hinge_joint_set_param(RID p_joint,HingeJointParam p_param, float p_value){}
	virtual float hinge_joint_get_param(RID p_joint,HingeJointParam p_param) const{return 0.0f;}

	virtual void hinge_joint_set_flag(RID p_joint,HingeJointFlag p_flag, bool p_value){}
	virtual bool hinge_joint_get_flag(RID p_joint,HingeJointFlag p_flag) const{return false;}

	virtual RID joint_create_slider(RID p_body_A,const Transform3D& p_local_frame_A,RID p_body_B,const Transform3D& p_local_frame_B){return RID();} //reference frame is A

	virtual void slider_joint_set_param(RID p_joint,SliderJointParam p_param, float p_value){}
	virtual float slider_joint_get_param(RID p_joint,SliderJointParam p_param) const{return 0.0f;}

	virtual RID joint_create_cone_twist(RID p_body_A,const Transform3D& p_local_frame_A,RID p_body_B,const Transform3D& p_local_frame_B){return RID();} //reference frame is A

	virtual void cone_twist_joint_set_param(RID p_joint,ConeTwistJointParam p_param, float p_value){}
	virtual float cone_twist_joint_get_param(RID p_joint,ConeTwistJointParam p_param) const{return 0.0f;}

	virtual RID joint_create_generic_6dof(RID p_body_A,const Transform3D& p_local_frame_A,RID p_body_B,const Transform3D& p_local_frame_B){return RID();} //reference frame is A

	virtual void generic_6dof_joint_set_param(RID p_joint,Vector3::Axis,G6DOFJointAxisParam p_param, float p_value){}
	virtual float generic_6dof_joint_get_param(RID p_joint,Vector3::Axis,G6DOFJointAxisParam p_param){return 0.0f;}

	virtual void generic_6dof_joint_set_flag(RID p_joint,Vector3::Axis,G6DOFJointAxisFlag p_flag, bool p_enable){}
	virtual bool generic_6dof_joint_get_flag(RID p_joint,Vector3::Axis,G6DOFJointAxisFlag p_flag){return false;}

    /* QUERY API */

	/* MISC */

	virtual void free(RID p_rid){}

	virtual void set_active(bool p_active){}
	virtual void init(){}
	virtual void step(float p_step){}
	virtual void sync(){}
	virtual void flush_queries(){}
	virtual void finish(){}

	virtual int get_process_info(ProcessInfo p_info){return 0;}

};
