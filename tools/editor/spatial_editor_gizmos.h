/*************************************************************************/
/*  spatial_editor_gizmos.h                                              */
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
#ifndef SPATIAL_EDITOR_GIZMOS_H
#define SPATIAL_EDITOR_GIZMOS_H


#include "tools/editor/plugins/spatial_editor_plugin.h"
#include "scene/3d/light.h"
#include "scene/3d/camera.h"
#include "scene/3d/position_3d.h"
#include "scene/3d/spatial_sample_player.h"
#include "scene/3d/spatial_stream_player.h"
#include "scene/3d/test_cube.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/body_shape.h"
#include "scene/3d/room_instance.h"
#include "scene/3d/visibility_notifier.h"
#include "scene/3d/portal.h"
#include "scene/3d/ray_cast.h"
#include "scene/3d/navigation_mesh.h"

#include "scene/3d/vehicle_body.h"
#include "scene/3d/collision_polygon.h"
#include "scene/3d/physics_joint.h"


class Camera3D;

class SpatialGizmoTool  : public SpatialEditorGizmo {

	OBJ_TYPE(SpatialGizmoTool,SpatialGizmo);

	struct Instance{

		RID instance;
		Ref<Mesh> mesh;
		RID skeleton;
		bool billboard;
		bool unscaled;
		bool can_intersect;
		bool extra_margin;
		Instance() {

			billboard=false;
			unscaled=false;
			can_intersect=false;
			extra_margin=false;
		}

		void create_instance(Node3D *p_base);

	};

	Vector<Vector3> collision_segments;
	Ref<TriangleMesh> collision_mesh;

	struct Handle {
		Vector3 pos;
		bool billboard;
	};

	Vector<Vector3> handles;
	Vector<Vector3> secondary_handles;
	bool billboard_handle;

	bool valid;
	Node3D *base;
	Vector<Instance> instances;
	Node3D *spatial_node;
protected:
	void add_lines(const Vector<Vector3> &p_lines,const Ref<Material>& p_material,bool p_billboard=false);
	void add_mesh(const Ref<Mesh>& p_mesh,bool p_billboard=false,const RID& p_skeleton=RID());
	void add_collision_segments(const Vector<Vector3> &p_lines);
	void add_collision_triangles(const Ref<TriangleMesh>& p_tmesh);
	void add_unscaled_billboard(const Ref<Material>& p_material,float p_scale=1);
	void add_handles(const Vector<Vector3> &p_handles,bool p_billboard=false,bool p_secondary=false);

	void set_spatial_node(Node3D *p_node);

public:

	virtual Vector3 get_handle_pos(int p_idx) const;
	virtual bool intersect_frustum(const Camera3D *p_camera,const Vector<Plane> &p_frustum);
	virtual bool intersect_ray(const Camera3D *p_camera,const Point2& p_point,  Vector3& r_pos, Vector3& r_normal,int *r_gizmo_handle=NULL,bool p_sec_first=false);

	void clear();
	void create();
	void transform();
	//void redraw();
	void free();

	SpatialGizmoTool();
	~SpatialGizmoTool();
};



class LightSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(LightSpatialGizmo,SpatialGizmoTool);

	Light* light;

public:


	virtual String get_handle_name(int p_idx) const;
	virtual Variant get_handle_value(int p_idx) const;
	virtual void set_handle(int p_idx,Camera3D *p_camera, const Point2& p_point);
	virtual void commit_handle(int p_idx,const Variant& p_restore,bool p_cancel=false);

	void redraw();
	LightSpatialGizmo(Light* p_light=NULL);

};

class CameraSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(CameraSpatialGizmo,SpatialGizmoTool);

	Camera3D* camera;

public:


	virtual String get_handle_name(int p_idx) const;
	virtual Variant get_handle_value(int p_idx) const;
	virtual void set_handle(int p_idx,Camera3D *p_camera, const Point2& p_point);
	virtual void commit_handle(int p_idx,const Variant& p_restore,bool p_cancel=false);

	void redraw();
	CameraSpatialGizmo(Camera3D* p_camera=NULL);

};



class MeshInstanceSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(MeshInstanceSpatialGizmo,SpatialGizmoTool);

	MeshInstance3D* mesh;

public:

	void redraw();
	MeshInstanceSpatialGizmo(MeshInstance3D* p_mesh=NULL);

};

class Position3DSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(Position3DSpatialGizmo,SpatialGizmoTool);

	Position3D* p3d;

public:

	void redraw();
	Position3DSpatialGizmo(Position3D* p_p3d=NULL);

};

class SkeletonSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(SkeletonSpatialGizmo,SpatialGizmoTool);

	Skeleton3D* skel;

public:

	void redraw();
	SkeletonSpatialGizmo(Skeleton3D* p_skel=NULL);

};


class SoundPlayer3DSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(SoundPlayer3DSpatialGizmo,SpatialGizmoTool);

	SoundPlayer3D* splayer;

public:

	void redraw();
	SoundPlayer3DSpatialGizmo(SoundPlayer3D* p_splayer=NULL);

};

class TestCubeSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(TestCubeSpatialGizmo,SpatialGizmoTool);

	TestCube* tc;

public:
	void redraw();
	TestCubeSpatialGizmo(TestCube* p_tc=NULL);

};


class RoomSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(RoomSpatialGizmo,SpatialGizmoTool);


	struct _EdgeKey {

		Vector3 from;
		Vector3 to;

		bool operator<(const _EdgeKey& p_with) const { return from==p_with.from ? to < p_with.to : from < p_with.from; }
	};



	Room* room;

public:

	void redraw();
	RoomSpatialGizmo(Room* p_room=NULL);

};


class PortalSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(PortalSpatialGizmo,SpatialGizmoTool);

	Portal* portal;

public:

	void redraw();
	PortalSpatialGizmo(Portal* p_portal=NULL);

};


class VisibilityNotifierGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(VisibilityNotifierGizmo ,SpatialGizmoTool);


	VisibilityNotifier3D* notifier;

public:

	virtual String get_handle_name(int p_idx) const;
	virtual Variant get_handle_value(int p_idx) const;
	virtual void set_handle(int p_idx,Camera3D *p_camera, const Point2& p_point);
	virtual void commit_handle(int p_idx,const Variant& p_restore,bool p_cancel=false);

	void redraw();
	VisibilityNotifierGizmo(VisibilityNotifier3D* p_notifier=NULL);

};



class CollisionShapeSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(CollisionShapeSpatialGizmo,SpatialGizmoTool);

	CollisionShape3D* cs;

public:
	virtual String get_handle_name(int p_idx) const;
	virtual Variant get_handle_value(int p_idx) const;
	virtual void set_handle(int p_idx,Camera3D *p_camera, const Point2& p_point);
	virtual void commit_handle(int p_idx,const Variant& p_restore,bool p_cancel=false);
	void redraw();
	CollisionShapeSpatialGizmo(CollisionShape3D* p_cs=NULL);

};


class CollisionPolygonSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(CollisionPolygonSpatialGizmo,SpatialGizmoTool);

	CollisionPolygon3D* polygon;

public:

	void redraw();
	CollisionPolygonSpatialGizmo(CollisionPolygon3D* p_polygon=NULL);

};


class RayCastSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(RayCastSpatialGizmo,SpatialGizmoTool);

	RayCast3D* raycast;

public:

	void redraw();
	RayCastSpatialGizmo(RayCast3D* p_raycast=NULL);

};



class VehicleWheelSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(VehicleWheelSpatialGizmo,SpatialGizmoTool);

	VehicleWheel3D* car_wheel;

public:

	void redraw();
	VehicleWheelSpatialGizmo(VehicleWheel3D* p_car_wheel=NULL);

};


class NavigationMeshSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(NavigationMeshSpatialGizmo,SpatialGizmoTool);


	struct _EdgeKey {

		Vector3 from;
		Vector3 to;

		bool operator<(const _EdgeKey& p_with) const { return from==p_with.from ? to < p_with.to : from < p_with.from; }
	};



	NavigationMeshInstance* navmesh;

public:

	void redraw();
	NavigationMeshSpatialGizmo(NavigationMeshInstance* p_navmesh=NULL);

};


class PinJointSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(PinJointSpatialGizmo,SpatialGizmoTool);

	PinJoint3D* p3d;

public:

	void redraw();
	PinJointSpatialGizmo(PinJoint3D* p_p3d=NULL);

};


class HingeJointSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(HingeJointSpatialGizmo,SpatialGizmoTool);

	HingeJoint3D* p3d;

public:

	void redraw();
	HingeJointSpatialGizmo(HingeJoint3D* p_p3d=NULL);

};

class SliderJointSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(SliderJointSpatialGizmo,SpatialGizmoTool);

	SliderJoint3D* p3d;

public:

	void redraw();
	SliderJointSpatialGizmo(SliderJoint3D* p_p3d=NULL);

};

class ConeTwistJointSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(ConeTwistJointSpatialGizmo,SpatialGizmoTool);

	ConeTwistJoint* p3d;

public:

	void redraw();
	ConeTwistJointSpatialGizmo(ConeTwistJoint* p_p3d=NULL);

};


class Generic6DOFJointSpatialGizmo  : public SpatialGizmoTool {

	OBJ_TYPE(Generic6DOFJointSpatialGizmo,SpatialGizmoTool);

	Generic6DOFJoint* p3d;

public:

	void redraw();
	Generic6DOFJointSpatialGizmo(Generic6DOFJoint* p_p3d=NULL);

};


class SpatialEditorGizmos  {
public:

	Ref<FixedMaterial> create_line_material(const Color& p_base_color);
	Ref<FixedMaterial> create_solid_material(const Color& p_base_color);
	Ref<FixedMaterial> handle2_material;
	Ref<FixedMaterial> handle_material;
	Ref<FixedMaterial> light_material;
	Ref<FixedMaterial> light_material_omni_icon;
	Ref<FixedMaterial> light_material_directional_icon;
	Ref<FixedMaterial> camera_material;
	Ref<FixedMaterial> skeleton_material;
	Ref<FixedMaterial> room_material;
	Ref<FixedMaterial> portal_material;
	Ref<FixedMaterial> raycast_material;
	Ref<FixedMaterial> visibility_notifier_material;
	Ref<FixedMaterial> car_wheel_material;
	Ref<FixedMaterial> joint_material;

	Ref<FixedMaterial> navmesh_edge_material;
	Ref<FixedMaterial> navmesh_solid_material;
	Ref<FixedMaterial> navmesh_edge_material_disabled;
	Ref<FixedMaterial> navmesh_solid_material_disabled;


	Ref<FixedMaterial> sample_player_icon;
	Ref<FixedMaterial> stream_player_icon;
	Ref<FixedMaterial> visibility_notifier_icon;

	Ref<FixedMaterial> shape_material;
	Ref<Texture> handle_t;

	Ref<Mesh> pos3d_mesh;
	static SpatialEditorGizmos *singleton;

	Ref<TriangleMesh> test_cube_tm;


	Ref<SpatialEditorGizmo> get_gizmo(Node3D *p_spatial);

	SpatialEditorGizmos();
};

#endif // SPATIAL_EDITOR_GIZMOS_H

