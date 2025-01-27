/*************************************************************************/
/*  ray_cast.cpp                                                         */
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
#include "ray_cast.h"

#include "servers/physics_server.h"
#include "collision_object.h"
void RayCast3D::set_cast_to(const Vector3& p_point) {

	cast_to=p_point;
	if (is_inside_tree() && get_tree()->is_editor_hint())
		update_gizmo();

}

Vector3 RayCast3D::get_cast_to() const{

	return cast_to;
}

bool RayCast3D::is_colliding() const{

	return collided;
}
Object *RayCast3D::get_collider() const{

	if (against==0)
		return NULL;

	return ObjectDB::get_instance(against);
}

int RayCast3D::get_collider_shape() const {

	return against_shape;
}
Vector3 RayCast3D::get_collision_point() const{

	return collision_point;
}
Vector3 RayCast3D::get_collision_normal() const{

	return collision_normal;
}


void RayCast3D::set_enabled(bool p_enabled) {

	enabled=p_enabled;
	if (is_inside_tree() && !get_tree()->is_editor_hint())
		set_fixed_process(p_enabled);
	if (!p_enabled)
		collided=false;

}


bool RayCast3D::is_enabled() const {


	return enabled;
}


void RayCast3D::_notification(int p_what) {

	switch(p_what) {

		case NOTIFICATION_ENTER_TREE: {

			if (enabled && !get_tree()->is_editor_hint()) {
				set_fixed_process(true);
				Node *p = get_parent();
				while( p && p->cast_to<Node3D>() ) {

					CollisionObject3D *co = p->cast_to<CollisionObject3D>();
					if (co) {

						exception=co->get_rid();
						exceptions.insert(exception);
					}

					p=p->get_parent();
				}
			} else
				set_fixed_process(false);



		} break;
		case NOTIFICATION_EXIT_TREE: {

			if (enabled) {
				set_fixed_process(false);
			}

			exceptions.erase(exception);

		} break;
		case NOTIFICATION_FIXED_PROCESS: {

			if (!enabled)
				break;



			Ref<World3D> w3d = get_world();
			ERR_BREAK( w3d.is_null() );

			PhysicsDirectSpaceState *dss = PhysicsServer::get_singleton()->space_get_direct_state(w3d->get_space());
			ERR_BREAK( !dss );

			Transform3D gt = get_global_transform();

			Vector3 to = cast_to;
			if (to==Vector3())
				to=Vector3(0,0.01,0);

			PhysicsDirectSpaceState::RayResult rr;

			if (dss->intersect_ray(gt.get_origin(),gt.xform(to),rr,exceptions)) {

				collided=true;
				against=rr.collider_id;
				collision_point=rr.position;
				collision_normal=rr.normal;
				against_shape=rr.shape;
			} else {
				collided=false;
			}



		} break;
	}
}

void RayCast3D::_bind_methods() {


	ObjectTypeDB::bind_method(_MD("set_enabled","enabled"),&RayCast3D::set_enabled);
	ObjectTypeDB::bind_method(_MD("is_enabled"),&RayCast3D::is_enabled);

	ObjectTypeDB::bind_method(_MD("set_cast_to","local_point"),&RayCast3D::set_cast_to);
	ObjectTypeDB::bind_method(_MD("get_cast_to"),&RayCast3D::get_cast_to);

	ObjectTypeDB::bind_method(_MD("is_colliding"),&RayCast3D::is_colliding);

	ObjectTypeDB::bind_method(_MD("get_collider"),&RayCast3D::get_collider);
	ObjectTypeDB::bind_method(_MD("get_collider_shape"),&RayCast3D::get_collider_shape);
	ObjectTypeDB::bind_method(_MD("get_collision_point"),&RayCast3D::get_collision_point);
	ObjectTypeDB::bind_method(_MD("get_collision_normal"),&RayCast3D::get_collision_normal);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL,"enabled"),_SCS("set_enabled"),_SCS("is_enabled"));
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3,"cast_to"),_SCS("set_cast_to"),_SCS("get_cast_to"));
}

RayCast3D::RayCast3D() {

	enabled=false;
	against=0;
	collided=false;
	against_shape=0;
	cast_to=Vector3(0,-1,0);
}
