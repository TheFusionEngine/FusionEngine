/*************************************************************************/
/*  body_shape.cpp                                                       */
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
#include "body_shape.h"
#include "servers/visual_server.h"
#include "scene/resources/sphere_shape.h"
#include "scene/resources/ray_shape.h"
#include "scene/resources/box_shape.h"
#include "scene/resources/capsule_shape.h"
//#include "scene/resources/cylinder_shape.h"
#include "scene/resources/convex_polygon_shape.h"
#include "scene/resources/concave_polygon_shape.h"
#include "scene/resources/height_map_shape.h"
#include "scene/resources/plane_shape.h"
#include "mesh_instance.h"
#include "physics_body.h"
#include "quick_hull.h"

void CollisionShape3D::_update_body() {


	if (get_parent() && get_parent()->cast_to<CollisionObject3D>())
		get_parent()->cast_to<CollisionObject3D>()->_update_shapes_from_children();

}

void CollisionShape3D::make_convex_from_brothers() {

	Node *p = get_parent();
	if (!p)
		return;

	for(int i=0;i<p->get_child_count();i++) {

		Node *n = p->get_child(i);
		if (n->cast_to<MeshInstance3D>()) {

			MeshInstance3D *mi=n->cast_to<MeshInstance3D>();
			Ref<Mesh> m = mi->get_mesh();
			if (m.is_valid()) {

				Ref<Shape3D> s = m->create_convex_shape();
				set_shape(s);
			}
		}
	}

}
/*

void CollisionShape3D::_update_indicator() {

	while (VisualServer::get_singleton()->mesh_get_surface_count(indicator))
		VisualServer::get_singleton()->mesh_remove_surface(indicator,0);

	if (shape.is_null())
		return;

	DVector<Vector3> points;
	DVector<Vector3> normals;

	VS::PrimitiveType pt = VS::PRIMITIVE_TRIANGLES;

	if (shape->cast_to<RayShape3D>()) {

		RayShape3D *rs = shape->cast_to<RayShape3D>();
		points.push_back(Vector3());
		points.push_back(Vector3(0,0,rs->get_length()));
		pt = VS::PRIMITIVE_LINES;
	} else if (shape->cast_to<SphereShape3D>()) {

//		VisualServer *vs=VisualServer::get_singleton();
		SphereShape3D *shapeptr=shape->cast_to<SphereShape3D>();


		Color col(0.4,1.0,1.0,0.5);

		int lats=6;
		int lons=12;
		float size=shapeptr->get_radius();


		for(int i = 1; i <= lats; i++) {
			double lat0 = Math_PI * (-0.5 + (double) (i - 1) / lats);
			double z0  = Math::sin(lat0);
			double zr0 =  Math::cos(lat0);

			double lat1 = Math_PI * (-0.5 + (double) i / lats);
			double z1 = Math::sin(lat1);
			double zr1 = Math::cos(lat1);

			for(int j = lons; j >= 1; j--) {

				double lng0 = 2 * Math_PI * (double) (j - 1) / lons;
				double x0 = Math::cos(lng0);
				double y0 = Math::sin(lng0);

				double lng1 = 2 * Math_PI * (double) (j) / lons;
				double x1 = Math::cos(lng1);
				double y1 = Math::sin(lng1);

				Vector3 v4=Vector3(x0 * zr0, z0, y0 *zr0)*size;
				Vector3 v3=Vector3(x0 * zr1, z1, y0 *zr1)*size;
				Vector3 v2=Vector3(x1 * zr1, z1, y1 *zr1)*size;
				Vector3 v1=Vector3(x1 * zr0, z0, y1 *zr0)*size;

				Vector<Vector3> line;
				line.push_back(v1);
				line.push_back(v2);
				line.push_back(v3);
				line.push_back(v4);


				points.push_back(v1);
				points.push_back(v2);
				points.push_back(v3);

				points.push_back(v1);
				points.push_back(v3);
				points.push_back(v4);

				normals.push_back(v1.normalized());
				normals.push_back(v2.normalized());
				normals.push_back(v3.normalized());

				normals.push_back(v1.normalized());
				normals.push_back(v3.normalized());
				normals.push_back(v4.normalized());

			}
		}
	} else if (shape->cast_to<BoxShape3D>()) {

		BoxShape3D *shapeptr=shape->cast_to<BoxShape3D>();

		for (int i=0;i<6;i++) {


			Vector3 face_points[4];


			for (int j=0;j<4;j++) {

				float v[3];
				v[0]=1.0;
				v[1]=1-2*((j>>1)&1);
				v[2]=v[1]*(1-2*(j&1));

				for (int k=0;k<3;k++) {

					if (i<3)
						face_points[j][(i+k)%3]=v[k]*(i>=3?-1:1);
					else
						face_points[3-j][(i+k)%3]=v[k]*(i>=3?-1:1);
				}
			}
			Vector3 normal;
			normal[i%3]=(i>=3?-1:1);

			for(int j=0;j<4;j++)
				face_points[j]*=shapeptr->get_extents();

			points.push_back(face_points[0]);
			points.push_back(face_points[1]);
			points.push_back(face_points[2]);

			points.push_back(face_points[0]);
			points.push_back(face_points[2]);
			points.push_back(face_points[3]);

			for(int n=0;n<6;n++)
				normals.push_back(normal);

		}

	} else if (shape->cast_to<ConvexPolygonShape3D>()) {

		ConvexPolygonShape3D *shapeptr=shape->cast_to<ConvexPolygonShape3D>();

		Geometry::MeshData md;
		QuickHull::build(Variant(shapeptr->get_points()),md);

		for(int i=0;i<md.faces.size();i++) {

			for(int j=2;j<md.faces[i].indices.size();j++) {
				points.push_back(md.vertices[md.faces[i].indices[0]]);
				points.push_back(md.vertices[md.faces[i].indices[j-1]]);
				points.push_back(md.vertices[md.faces[i].indices[j]]);
				normals.push_back(md.faces[i].plane.normal);
				normals.push_back(md.faces[i].plane.normal);
				normals.push_back(md.faces[i].plane.normal);
			}
		}
	} else if (shape->cast_to<ConcavePolygonShape3D>()) {

		ConcavePolygonShape3D *shapeptr=shape->cast_to<ConcavePolygonShape3D>();

		points = shapeptr->get_faces();
		for(int i=0;i<points.size()/3;i++) {

			Vector3 n = Plane( points[i*3+0],points[i*3+1],points[i*3+2] ).normal;
			normals.push_back(n);
			normals.push_back(n);
			normals.push_back(n);
		}

	} else if (shape->cast_to<CapsuleShape3D>()) {

		CapsuleShape3D *shapeptr=shape->cast_to<CapsuleShape3D>();

		DVector<Plane> planes = Geometry::build_capsule_planes(shapeptr->get_radius(), shapeptr->get_height()/2.0, 12, Vector3::AXIS_Z);
		Geometry::MeshData md = Geometry::build_convex_mesh(planes);

		for(int i=0;i<md.faces.size();i++) {

			for(int j=2;j<md.faces[i].indices.size();j++) {
				points.push_back(md.vertices[md.faces[i].indices[0]]);
				points.push_back(md.vertices[md.faces[i].indices[j-1]]);
				points.push_back(md.vertices[md.faces[i].indices[j]]);
				normals.push_back(md.faces[i].plane.normal);
				normals.push_back(md.faces[i].plane.normal);
				normals.push_back(md.faces[i].plane.normal);

			}
		}

	} else if (shape->cast_to<PlaneShape3D>()) {

		PlaneShape3D *shapeptr=shape->cast_to<PlaneShape3D>();

		Plane p = shapeptr->get_plane();
		Vector3 n1 = p.get_any_perpendicular_normal();
		Vector3 n2 = p.normal.cross(n1).normalized();

		Vector3 pface[4]={
			p.normal*p.d+n1*100.0+n2*100.0,
			p.normal*p.d+n1*100.0+n2*-100.0,
			p.normal*p.d+n1*-100.0+n2*-100.0,
			p.normal*p.d+n1*-100.0+n2*100.0,
		};

		points.push_back(pface[0]);
		points.push_back(pface[1]);
		points.push_back(pface[2]);

		points.push_back(pface[0]);
		points.push_back(pface[2]);
		points.push_back(pface[3]);

		normals.push_back(p.normal);
		normals.push_back(p.normal);
		normals.push_back(p.normal);
		normals.push_back(p.normal);
		normals.push_back(p.normal);
		normals.push_back(p.normal);

	}

	if (!points.size())
		return;
	RID material = VisualServer::get_singleton()->fixed_material_create();
	VisualServer::get_singleton()->fixed_material_set_param(material,VS::FIXED_MATERIAL_PARAM_DIFFUSE,Color(0,0.6,0.7,0.3));
	VisualServer::get_singleton()->fixed_material_set_param(material,VS::FIXED_MATERIAL_PARAM_EMISSION,0.7);
	if (normals.size()==0)
		VisualServer::get_singleton()->material_set_flag(material,VS::MATERIAL_FLAG_UNSHADED,true);
	VisualServer::get_singleton()->material_set_flag(material,VS::MATERIAL_FLAG_DOUBLE_SIDED,true);
	Array d;
	d.resize(VS::ARRAY_MAX);
	d[VS::ARRAY_VERTEX]=points;
	if (normals.size())
		d[VS::ARRAY_NORMAL]=normals;
	VisualServer::get_singleton()->mesh_add_surface(indicator,pt,d);
	VisualServer::get_singleton()->mesh_surface_set_material(indicator,0,material,true);

}

*/
void CollisionShape3D::_add_to_collision_object(Object* p_cshape) {

	if (unparenting)
		return;

	CollisionObject3D *co=p_cshape->cast_to<CollisionObject3D>();
	ERR_FAIL_COND(!co);

	if (shape.is_valid()) {

		co->add_shape(shape,get_transform());
        if (trigger)
            co->set_shape_as_trigger( co->get_shape_count() -1, true );
    }
}

void CollisionShape3D::_notification(int p_what) {

	switch(p_what) {

		case NOTIFICATION_ENTER_TREE: {
			unparenting=false;

			//indicator_instance = VisualServer::get_singleton()->instance_create2(indicator,get_world()->get_scenario());
		} break;
		case NOTIFICATION_TRANSFORM_CHANGED: {
		//	VisualServer::get_singleton()->instance_set_transform(indicator_instance,get_global_transform());
			if (updating_body) {
				_update_body();
			}
		} break;
		case NOTIFICATION_EXIT_TREE: {
		/*	if (indicator_instance.is_valid()) {
				VisualServer::get_singleton()->free(indicator_instance);
				indicator_instance=RID();
			}*/
		} break;
		case NOTIFICATION_UNPARENTED: {
			unparenting=true;
			if (updating_body)
				_update_body();
		} break;
		case NOTIFICATION_PARENTED: {
			if (updating_body)
				_update_body();
		} break;
	}
}


void CollisionShape3D::resource_changed(RES res) {

	update_gizmo();


}

void CollisionShape3D::_bind_methods() {

	//not sure if this should do anything
	ObjectTypeDB::bind_method(_MD("resource_changed"),&CollisionShape3D::resource_changed);
	ObjectTypeDB::bind_method(_MD("set_shape","shape"),&CollisionShape3D::set_shape);
	ObjectTypeDB::bind_method(_MD("get_shape"),&CollisionShape3D::get_shape);
	ObjectTypeDB::bind_method(_MD("_add_to_collision_object"),&CollisionShape3D::_add_to_collision_object);
	ObjectTypeDB::bind_method(_MD("set_trigger","enable"),&CollisionShape3D::set_trigger);
	ObjectTypeDB::bind_method(_MD("is_trigger"),&CollisionShape3D::is_trigger);
	ObjectTypeDB::bind_method(_MD("make_convex_from_brothers"),&CollisionShape3D::make_convex_from_brothers);
	ObjectTypeDB::set_method_flags("CollisionShape3D","make_convex_from_brothers",METHOD_FLAGS_DEFAULT|METHOD_FLAG_EDITOR);


	ADD_PROPERTY( PropertyInfo( Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"), _SCS("set_shape"), _SCS("get_shape"));
    ADD_PROPERTY(PropertyInfo(Variant::BOOL,"trigger"),_SCS("set_trigger"),_SCS("is_trigger"));
}


void CollisionShape3D::set_shape(const Ref<Shape3D> &p_shape) {

	if (!shape.is_null())
		shape->unregister_owner(this);
	shape=p_shape;
	if (!shape.is_null())
		shape->register_owner(this);
	update_gizmo();
	if (updating_body)
		_update_body();
}

Ref<Shape3D> CollisionShape3D::get_shape() const {

	return shape;
}


void CollisionShape3D::set_updating_body(bool p_update) {
	updating_body=p_update;
}

bool CollisionShape3D::is_updating_body() const {

	return updating_body;
}

void CollisionShape3D::set_trigger(bool p_trigger) {

    trigger=p_trigger;
    if (updating_body)
        _update_body();
}

bool CollisionShape3D::is_trigger() const{

    return trigger;
}

CollisionShape3D::CollisionShape3D() {

	//indicator = VisualServer::get_singleton()->mesh_create();
	updating_body=true;
	unparenting=false;
    trigger=false;
}

CollisionShape3D::~CollisionShape3D() {
	if (!shape.is_null())
		shape->unregister_owner(this);
	//VisualServer::get_singleton()->free(indicator);
}

#if 0
#include "body_volume.h"

#include "scene/3d/physics_body.h"
#include "geometry.h"

#define ADD_TRIANGLE( m_a, m_b, m_c, m_color)\
{\
	Vector<Vector3> points;\
	points.resize(3);\
	points[0]=m_a;\
	points[1]=m_b;\
	points[2]=m_c;\
	Vector<Color> colors;\
	colors.resize(3);\
	colors[0]=m_color;\
	colors[1]=m_color;\
	colors[2]=m_color;\
	vs->poly_add_primitive(p_indicator,points,Vector<Vector3>(),colors,Vector<Vector3>());\
}


void CollisionShape3D::_notification(int p_what) {

	switch (p_what) {
		case NOTIFICATION_ENTER_SCENE: {


			if (get_root_node()->get_editor() && !indicator.is_valid()) {

				indicator=VisualServer::get_singleton()->poly_create();
				RID mat=VisualServer::get_singleton()->fixed_material_create();
				VisualServer::get_singleton()->material_set_flag( mat, VisualServer::MATERIAL_FLAG_UNSHADED, true );
				VisualServer::get_singleton()->material_set_flag( mat, VisualServer::MATERIAL_FLAG_WIREFRAME, true );
				VisualServer::get_singleton()->material_set_flag( mat, VisualServer::MATERIAL_FLAG_DOUBLE_SIDED, true );
				VisualServer::get_singleton()->material_set_line_width( mat, 3 );

				VisualServer::get_singleton()->poly_set_material(indicator,mat,true);

				update_indicator(indicator);
			}

			if (indicator.is_valid()) {

				indicator_instance=VisualServer::get_singleton()->instance_create2(indicator,get_world()->get_scenario());
				VisualServer::get_singleton()->instance_attach_object_instance_ID(indicator_instance,get_instance_ID());
			}
			volume_changed();
		} break;
		case NOTIFICATION_EXIT_SCENE: {

			if (indicator_instance.is_valid()) {

				VisualServer::get_singleton()->free(indicator_instance);
			}
			volume_changed();
		} break;
		case NOTIFICATION_TRANSFORM_CHANGED: {

			if (indicator_instance.is_valid()) {

				VisualServer::get_singleton()->instance_set_transform(indicator_instance,get_global_transform());
			}
			volume_changed();
		} break;
		default: {}
	}
}

void CollisionShape3D::volume_changed() {

	if (indicator.is_valid())
		update_indicator(indicator);

	Object *parent=get_parent();
	if (!parent)
		return;
	PhysicsBody3D *physics_body=parent->cast_to<PhysicsBody3D>();

	ERR_EXPLAIN("CollisionShape3D parent is not of type PhysicsBody3D");
	ERR_FAIL_COND(!physics_body);

	physics_body->recompute_child_volumes();

}

RID CollisionShape3D::_get_visual_instance_rid() const {

	return indicator_instance;

}

void CollisionShape3D::_bind_methods() {

	ObjectTypeDB::bind_method("_get_visual_instance_rid",&CollisionShape3D::_get_visual_instance_rid);
}

CollisionShape3D::CollisionShape3D() {

}

CollisionShape3D::~CollisionShape3D() {

	if (indicator.is_valid()) {

		VisualServer::get_singleton()->free(indicator);
	}

}

void CollisionShapeSphere::_set(const String& p_name, const Variant& p_value) {

	if (p_name=="radius") {
		radius=p_value;
		volume_changed();
	}

}

Variant CollisionShapeSphere::_get(const String& p_name) const {

	if (p_name=="radius") {
		return radius;
	}

	return Variant();
}

void CollisionShapeSphere::_get_property_list( List<PropertyInfo> *p_list) const {

	p_list->push_back( PropertyInfo(Variant::REAL,"radius",PROPERTY_HINT_RANGE,"0.01,16384,0.01") );
}

void CollisionShapeSphere::update_indicator(RID p_indicator) {

	VisualServer *vs=VisualServer::get_singleton();

	vs->poly_clear(p_indicator);
	Color col(0.4,1.0,1.0,0.5);

	int lats=6;
	int lons=12;
	float size=radius;

	for(int i = 1; i <= lats; i++) {
		double lat0 = Math_PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = Math::sin(lat0);
		double zr0 =  Math::cos(lat0);

		double lat1 = Math_PI * (-0.5 + (double) i / lats);
		double z1 = Math::sin(lat1);
		double zr1 = Math::cos(lat1);

		for(int j = lons; j >= 1; j--) {

			double lng0 = 2 * Math_PI * (double) (j - 1) / lons;
			double x0 = Math::cos(lng0);
			double y0 = Math::sin(lng0);

			double lng1 = 2 * Math_PI * (double) (j) / lons;
			double x1 = Math::cos(lng1);
			double y1 = Math::sin(lng1);

			Vector3 v4=Vector3(x0 * zr0, z0, y0 *zr0)*size;
			Vector3 v3=Vector3(x0 * zr1, z1, y0 *zr1)*size;
			Vector3 v2=Vector3(x1 * zr1, z1, y1 *zr1)*size;
			Vector3 v1=Vector3(x1 * zr0, z0, y1 *zr0)*size;

			Vector<Vector3> line;
			line.push_back(v1);
			line.push_back(v2);
			line.push_back(v3);
			line.push_back(v4);

			Vector<Color> cols;
			cols.push_back(col);
			cols.push_back(col);
			cols.push_back(col);
			cols.push_back(col);


			VisualServer::get_singleton()->poly_add_primitive(p_indicator,line,Vector<Vector3>(),cols,Vector<Vector3>());
		}
	}
}

void CollisionShapeSphere::append_to_volume(Ref<Shape3D> p_volume) {

	p_volume->add_sphere_shape(radius,get_transform());
}


CollisionShapeSphere::CollisionShapeSphere() {

	radius=1.0;
}

/* BOX */


void CollisionShapeBox::_set(const String& p_name, const Variant& p_value) {

	if (p_name=="half_extents") {
		half_extents=p_value;
		volume_changed();
	}

}

Variant CollisionShapeBox::_get(const String& p_name) const {

	if (p_name=="half_extents") {
		return half_extents;
	}

	return Variant();
}

void CollisionShapeBox::_get_property_list( List<PropertyInfo> *p_list) const {

	p_list->push_back( PropertyInfo(Variant::VECTOR3,"half_extents" ) );
}


void CollisionShapeBox::update_indicator(RID p_indicator) {

	VisualServer *vs=VisualServer::get_singleton();

	vs->poly_clear(p_indicator);
	Color col(0.4,1.0,1.0,0.5);


	for (int i=0;i<6;i++) {


		Vector3 face_points[4];

		for (int j=0;j<4;j++) {

			float v[3];
			v[0]=1.0;
			v[1]=1-2*((j>>1)&1);
			v[2]=v[1]*(1-2*(j&1));

			for (int k=0;k<3;k++) {

				if (i<3)
					face_points[j][(i+k)%3]=v[k]*(i>=3?-1:1);
				else
					face_points[3-j][(i+k)%3]=v[k]*(i>=3?-1:1);
			}
		}

		for(int j=0;j<4;j++)
			face_points[i]*=half_extents;

		ADD_TRIANGLE(face_points[0],face_points[1],face_points[2],col);
		ADD_TRIANGLE(face_points[2],face_points[3],face_points[0],col);

	}
}

void CollisionShapeBox::append_to_volume(Ref<Shape3D> p_volume) {

	p_volume->add_box_shape(half_extents,get_transform());
}


CollisionShapeBox::CollisionShapeBox() {

	half_extents=Vector3(1,1,1);
}

/* CYLINDER */


void CollisionShapeCylinder::_set(const String& p_name, const Variant& p_value) {

	if (p_name=="radius") {
		radius=p_value;
		volume_changed();
	}
	if (p_name=="height") {
		height=p_value;
		volume_changed();
	}

}

Variant CollisionShapeCylinder::_get(const String& p_name) const {

	if (p_name=="radius") {
		return radius;
	}
	if (p_name=="height") {
		return height;
	}
	return Variant();
}

void CollisionShapeCylinder::_get_property_list( List<PropertyInfo> *p_list) const {

	p_list->push_back( PropertyInfo(Variant::REAL,"radius",PROPERTY_HINT_RANGE,"0.01,16384,0.01") );
	p_list->push_back( PropertyInfo(Variant::REAL,"height",PROPERTY_HINT_RANGE,"0.01,16384,0.01") );
}


void CollisionShapeCylinder::update_indicator(RID p_indicator) {

	VisualServer *vs=VisualServer::get_singleton();

	vs->poly_clear(p_indicator);
	Color col(0.4,1.0,1.0,0.5);

	DVector<Plane> planes = Geometry::build_cylinder_planes(radius, height, 12, Vector3::AXIS_Z);
	Geometry::MeshData md = Geometry::build_convex_mesh(planes);

	for(int i=0;i<md.faces.size();i++) {

		for(int j=2;j<md.faces[i].indices.size();j++) {
			ADD_TRIANGLE(md.vertices[md.faces[i].indices[0]],md.vertices[md.faces[i].indices[j-1]],md.vertices[md.faces[i].indices[j]],col);
		}
	}

}

void CollisionShapeCylinder::append_to_volume(Ref<Shape3D> p_volume) {

	p_volume->add_cylinder_shape(radius,height*2.0,get_transform());
}


CollisionShapeCylinder::CollisionShapeCylinder() {

	height=1;
	radius=1;
}

/* CAPSULE */


void CollisionShapeCapsule::_set(const String& p_name, const Variant& p_value) {

	if (p_name=="radius") {
		radius=p_value;
		volume_changed();
	}

	if (p_name=="height") {
		height=p_value;
		volume_changed();
	}

}

Variant CollisionShapeCapsule::_get(const String& p_name) const {

	if (p_name=="radius") {
		return radius;
	}
	if (p_name=="height") {
		return height;
	}
	return Variant();
}

void CollisionShapeCapsule::_get_property_list( List<PropertyInfo> *p_list) const {

	p_list->push_back( PropertyInfo(Variant::REAL,"radius",PROPERTY_HINT_RANGE,"0.01,16384,0.01") );
	p_list->push_back( PropertyInfo(Variant::REAL,"height",PROPERTY_HINT_RANGE,"0.01,16384,0.01") );
}


void CollisionShapeCapsule::update_indicator(RID p_indicator) {

	VisualServer *vs=VisualServer::get_singleton();

	vs->poly_clear(p_indicator);
	Color col(0.4,1.0,1.0,0.5);

	DVector<Plane> planes = Geometry::build_capsule_planes(radius, height, 12, 3, Vector3::AXIS_Z);
	Geometry::MeshData md = Geometry::build_convex_mesh(planes);

	for(int i=0;i<md.faces.size();i++) {

		for(int j=2;j<md.faces[i].indices.size();j++) {
			ADD_TRIANGLE(md.vertices[md.faces[i].indices[0]],md.vertices[md.faces[i].indices[j-1]],md.vertices[md.faces[i].indices[j]],col);
		}
	}

}

void CollisionShapeCapsule::append_to_volume(Ref<Shape3D> p_volume) {


	p_volume->add_capsule_shape(radius,height,get_transform());
}


CollisionShapeCapsule::CollisionShapeCapsule() {

	height=1;
	radius=1;
}
#endif
