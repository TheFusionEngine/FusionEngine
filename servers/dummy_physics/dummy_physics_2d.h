#include "servers/physics_2d_server.h"

class Physics2DDirectBodyStateDummy : public Physics2DDirectBodyState {
	OBJ_TYPE( Physics2DDirectBodyStateDummy, Physics2DDirectBodyState );
protected:
	static void _bind_methods();
public:

	virtual Vector2 get_total_gravity() const{return Vector2();} // get gravity vector working on this body space/area
	virtual float get_total_density() const{return 0.0f;} // get density of this body space/area

	virtual float get_inverse_mass() const{return 0.0f;} // get the mass
	virtual real_t get_inverse_inertia() const{return 0.0f;} // get density of this body space

	virtual void set_linear_velocity(const Vector2& p_velocity){}
	virtual Vector2 get_linear_velocity() const{return Vector2();}

	virtual void set_angular_velocity(real_t p_velocity){}
	virtual real_t get_angular_velocity() const{return 0.0f;}

	virtual void set_transform(const Transform2D& p_transform){}
	virtual Transform2D get_transform() const{return Transform2D();}

	virtual void set_sleep_state(bool p_enable){}
	virtual bool is_sleeping() const{return true;}

	virtual int get_contact_count() const{return 0;}

	virtual Vector2 get_contact_local_pos(int p_contact_idx) const{return Vector2();}
	virtual Vector2 get_contact_local_normal(int p_contact_idx) const{return Vector2();}
	virtual int get_contact_local_shape(int p_contact_idx) const{return 0;}

	virtual RID get_contact_collider(int p_contact_idx) const{return RID();}
	virtual Vector2 get_contact_collider_pos(int p_contact_idx) const{return Vector2();}
	virtual ObjectID get_contact_collider_id(int p_contact_idx) const{return ObjectID();}
	virtual Object* get_contact_collider_object(int p_contact_idx) const;
	virtual int get_contact_collider_shape(int p_contact_idx) const{return 0;}
	virtual Variant get_contact_collider_shape_metadata(int p_contact_idx) const{return 0;}
	virtual Vector2 get_contact_collider_velocity_at_pos(int p_contact_idx) const{return Vector2();}

	virtual real_t get_step() const{return 0.0f;}
	virtual void integrate_forces();

	virtual Physics2DDirectSpaceState* get_space_state(){return NULL;}
};

class Physics2DDirectSpaceStateDummy : public Physics2DDirectSpaceState {
	OBJ_TYPE( Physics2DDirectSpaceStateDummy, Physics2DDirectSpaceState );

	Dictionary _intersect_ray(const Vector2& p_from, const Vector2& p_to,const Vector<RID>& p_exclude=Vector<RID>(),uint32_t p_layers=0,uint32_t p_object_type_mask=TYPE_MASK_COLLISION);

	Array _intersect_shape(const Ref<Physics2DShapeQueryParameters> &p_shape_query,int p_max_results=32);
	Array _cast_motion(const Ref<Physics2DShapeQueryParameters> &p_shape_query);
	Array _collide_shape(const Ref<Physics2DShapeQueryParameters> &p_shape_query,int p_max_results=32);
	Dictionary _get_rest_info(const Ref<Physics2DShapeQueryParameters> &p_shape_query);

protected:
	static void _bind_methods();

public:
	virtual bool intersect_ray(const Vector2& p_from, const Vector2& p_to,RayResult &r_result,const Set<RID>& p_exclude=Set<RID>(),uint32_t p_layer_mask=0xFFFFFFFF,uint32_t p_object_type_mask=TYPE_MASK_COLLISION){return false;}


	virtual int intersect_shape(const RID& p_shape, const Transform2D& p_xform,const Vector2& p_motion,float p_margin,ShapeResult *r_results,int p_result_max,const Set<RID>& p_exclude=Set<RID>(),uint32_t p_layer_mask=0xFFFFFFFF,uint32_t p_object_type_mask=TYPE_MASK_COLLISION){return 0;}

	virtual bool cast_motion(const RID& p_shape, const Transform2D& p_xform,const Vector2& p_motion,float p_margin,float &p_closest_safe,float &p_closest_unsafe, const Set<RID>& p_exclude=Set<RID>(),uint32_t p_layer_mask=0xFFFFFFFF,uint32_t p_object_type_mask=TYPE_MASK_COLLISION){return false;}

	virtual bool collide_shape(RID p_shape, const Transform2D& p_shape_xform,const Vector2& p_motion,float p_margin,Vector2 *r_results,int p_result_max,int &r_result_count, const Set<RID>& p_exclude=Set<RID>(),uint32_t p_layer_mask=0xFFFFFFFF,uint32_t p_object_type_mask=TYPE_MASK_COLLISION){return false;}

	virtual bool rest_info(RID p_shape, const Transform2D& p_shape_xform,const Vector2& p_motion,float p_margin,ShapeRestInfo *r_info, const Set<RID>& p_exclude=Set<RID>(),uint32_t p_layer_mask=0xFFFFFFFF,uint32_t p_object_type_mask=TYPE_MASK_COLLISION){return false;}

};

class PhysicsServer2DDummy : public PhysicsServer2D {
	OBJ_TYPE( PhysicsServer2DDummy, PhysicsServer2D);

public:
	virtual void set_active(){singleton = this;}

	virtual RID shape_create(ShapeType p_shape){return RID();}
	virtual void shape_set_data(RID p_shape, const Variant& p_data){}
	virtual void shape_set_custom_solver_bias(RID p_shape, real_t p_bias){}

	virtual ShapeType shape_get_type(RID p_shape) const{return SHAPE_CUSTOM;}
	virtual Variant shape_get_data(RID p_shape) const{return 0;}
	virtual real_t shape_get_custom_solver_bias(RID p_shape) const{return 0.0f;}

	//these work well, but should be used from the main thread only
	virtual bool shape_collide(RID p_shape_A, const Transform2D& p_xform_A,const Vector2& p_motion_A,RID p_shape_B, const Transform2D& p_xform_B, const Vector2& p_motion_B,Vector2 *r_results,int p_result_max,int &r_result_count){return false;}

	/* SPACE API */

	virtual RID space_create(){return RID();}
	virtual void space_set_active(RID p_space,bool p_active){}
	virtual bool space_is_active(RID p_space) const{return false;}

	virtual void space_set_param(RID p_space,SpaceParameter p_param, real_t p_value){}
	virtual real_t space_get_param(RID p_space,SpaceParameter p_param) const{return 0.0f;}

	// this function only works on fixed process, errors and returns null otherwise
	virtual Physics2DDirectSpaceState* space_get_direct_state(RID p_space){return NULL;}

	//missing space parameters

	/* AREA API */

	//missing attenuation? missing better override?

	virtual RID area_create(){return RID();}

	virtual void area_set_space(RID p_area, RID p_space){}
	virtual RID area_get_space(RID p_area) const{return RID();}

	virtual void area_set_space_override_mode(RID p_area, AreaSpaceOverrideMode p_mode){}
	virtual AreaSpaceOverrideMode area_get_space_override_mode(RID p_area) const{return AREA_SPACE_OVERRIDE_DISABLED;}

	virtual void area_add_shape(RID p_area, RID p_shape, const Transform2D& p_transform=Transform2D()){}
	virtual void area_set_shape(RID p_area, int p_shape_idx,RID p_shape){}
	virtual void area_set_shape_transform(RID p_area, int p_shape_idx, const Transform2D& p_transform){}

	virtual int area_get_shape_count(RID p_area) const{return 0;}
	virtual RID area_get_shape(RID p_area, int p_shape_idx) const{return RID();}
	virtual Transform2D area_get_shape_transform(RID p_area, int p_shape_idx) const{return Transform2D();}

	virtual void area_remove_shape(RID p_area, int p_shape_idx){}
	virtual void area_clear_shapes(RID p_area){}

	virtual void area_attach_object_instance_ID(RID p_area,ObjectID p_ID){}
	virtual ObjectID area_get_object_instance_ID(RID p_area) const{return ObjectID();}

	virtual void area_set_param(RID p_area,AreaParameter p_param,const Variant& p_value){}
	virtual void area_set_transform(RID p_area, const Transform2D& p_transform){}

	virtual Variant area_get_param(RID p_parea,AreaParameter p_param) const{return 0;}
	virtual Transform2D area_get_transform(RID p_area) const{return Transform2D();}

	virtual void area_set_monitor_callback(RID p_area,Object *p_receiver,const StringName& p_method){}

	/* BODY API */

	virtual RID body_create(BodyMode p_mode=BODY_MODE_RIGID,bool p_init_sleeping=false){return RID();}

	virtual void body_set_space(RID p_body, RID p_space){}
	virtual RID body_get_space(RID p_body) const{return RID();}

	virtual void body_set_mode(RID p_body, BodyMode p_mode){}
	virtual BodyMode body_get_mode(RID p_body) const{return BODY_MODE_STATIC;}

	virtual void body_add_shape(RID p_body, RID p_shape, const Transform2D& p_transform=Transform2D()){}
	virtual void body_set_shape(RID p_body, int p_shape_idx,RID p_shape){}
	virtual void body_set_shape_transform(RID p_body, int p_shape_idx, const Transform2D& p_transform){}
	virtual void body_set_shape_metadata(RID p_body, int p_shape_idx, const Variant& p_metadata){}

	virtual int body_get_shape_count(RID p_body) const{return 0;}
	virtual RID body_get_shape(RID p_body, int p_shape_idx) const{return RID();}
	virtual Transform2D body_get_shape_transform(RID p_body, int p_shape_idx) const{return Transform2D();}
	virtual Variant body_get_shape_metadata(RID p_body, int p_shape_idx) const{return 0;}

	virtual void body_set_shape_as_trigger(RID p_body, int p_shape_idx,bool p_enable){}
	virtual bool body_is_shape_set_as_trigger(RID p_body, int p_shape_idx) const{return false;}

	virtual void body_remove_shape(RID p_body, int p_shape_idx){}
	virtual void body_clear_shapes(RID p_body){}

	virtual void body_attach_object_instance_ID(RID p_body,uint32_t p_ID){}
	virtual uint32_t body_get_object_instance_ID(RID p_body) const{return 0;}

	virtual void body_set_continuous_collision_detection_mode(RID p_body,CCDMode p_mode){}
	virtual CCDMode body_get_continuous_collision_detection_mode(RID p_body) const{return CCD_MODE_DISABLED;}

	virtual void body_set_layer_mask(RID p_body, uint32_t p_mask){}
	virtual uint32_t body_get_layer_mask(RID p_body, uint32_t p_mask) const{return 0;}

	virtual void body_set_user_mask(RID p_body, uint32_t p_mask){}
	virtual uint32_t body_get_user_mask(RID p_body, uint32_t p_mask) const{return 0;}

	virtual void body_set_param(RID p_body, BodyParameter p_param, float p_value){}
	virtual float body_get_param(RID p_body, BodyParameter p_param) const{return 0.0f;}

	virtual void body_set_state(RID p_body, BodyState p_state, const Variant& p_variant){}
	virtual Variant body_get_state(RID p_body, BodyState p_state) const{return 0;}

	//do something about it
	virtual void body_set_applied_force(RID p_body, const Vector2& p_force){}
	virtual Vector2 body_get_applied_force(RID p_body) const{return Vector2();}

	virtual void body_set_applied_torque(RID p_body, float p_torque){}
	virtual float body_get_applied_torque(RID p_body) const{return 0.0f;}

	virtual void body_apply_impulse(RID p_body, const Vector2& p_pos, const Vector2& p_impulse){}
	virtual void body_set_axis_velocity(RID p_body, const Vector2& p_axis_velocity){}

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

	virtual bool body_collide_shape(RID p_body, int p_body_shape,RID p_shape, const Transform2D& p_shape_xform,const Vector2& p_motion,Vector2 *r_results,int p_result_max,int &r_result_count){return false;}

	/* JOINT API */

	virtual void joint_set_param(RID p_joint, JointParam p_param, real_t p_value){}
	virtual real_t joint_get_param(RID p_joint,JointParam p_param) const{return 0.0f;}

	virtual RID pin_joint_create(const Vector2& p_anchor,RID p_body_a,RID p_body_b=RID()){return RID();}
	virtual RID groove_joint_create(const Vector2& p_a_groove1,const Vector2& p_a_groove2, const Vector2& p_b_anchor, RID p_body_a,RID p_body_b){return RID();}
	virtual RID damped_spring_joint_create(const Vector2& p_anchor_a,const Vector2& p_anchor_b,RID p_body_a,RID p_body_b=RID()){return RID();}

    virtual void damped_string_joint_set_param(RID p_joint, DampedStringParam p_param, real_t p_value){}
	virtual real_t damped_string_joint_get_param(RID p_joint, DampedStringParam p_param) const{return 0.0f;}

	virtual JointType joint_get_type(RID p_joint) const{return JOINT_PIN;}

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
