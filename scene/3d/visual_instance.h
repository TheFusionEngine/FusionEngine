/*************************************************************************/
/*  visual_instance.h                                                    */
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
#ifndef VISUAL_INSTANCE_H
#define VISUAL_INSTANCE_H

#include "scene/3d/spatial.h"
#include "face3.h"
#include "rid.h"
#include "scene/resources/material.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class VisualInstance3D : public Node3D {

	OBJ_TYPE( VisualInstance3D, Node3D );
	OBJ_CATEGORY("3D Visual Nodes");

	RID instance;
	uint32_t layers;


	RID _get_visual_instance_rid() const;


protected:


	void _notification(int p_what);
	static void _bind_methods();
public:

	enum GetFacesFlags {
		FACES_SOLID=1, // solid geometry
		FACES_ENCLOSING=2,
		FACES_DYNAMIC=4 // dynamic object geometry

	};

	RID get_instance() const;
	virtual AABB get_aabb() const=0;
	virtual DVector<Face3> get_faces(uint32_t p_usage_flags) const=0;
	
	virtual AABB get_transformed_aabb() const; // helper

	void set_base(const RID& p_base);

	void set_layer_mask(uint32_t p_mask);
	uint32_t get_layer_mask() const;


	VisualInstance3D();	
	~VisualInstance3D();

};

class BakedLightInstance;

class GeometryInstance3D : public VisualInstance3D {

	OBJ_TYPE( GeometryInstance3D, VisualInstance3D );
public:

	enum Flags {
		FLAG_VISIBLE=VS::INSTANCE_FLAG_VISIBLE,
		FLAG_CAST_SHADOW=VS::INSTANCE_FLAG_CAST_SHADOW,
		FLAG_RECEIVE_SHADOWS=VS::INSTANCE_FLAG_RECEIVE_SHADOWS,
		FLAG_BILLBOARD=VS::INSTANCE_FLAG_BILLBOARD,
		FLAG_BILLBOARD_FIX_Y=VS::INSTANCE_FLAG_BILLBOARD_FIX_Y,
		FLAG_DEPH_SCALE=VS::INSTANCE_FLAG_DEPH_SCALE,
		FLAG_VISIBLE_IN_ALL_ROOMS=VS::INSTANCE_FLAG_VISIBLE_IN_ALL_ROOMS,
		FLAG_USE_BAKED_LIGHT=VS::INSTANCE_FLAG_USE_BAKED_LIGHT,
		FLAG_MAX=VS::INSTANCE_FLAG_MAX,
	};


private:

	bool flags[FLAG_MAX];
	Ref<Material> material_override;
	float draw_begin;
	float draw_end;
	void _find_baked_light();
	BakedLightInstance *baked_light_instance;
	int baked_light_texture_id;

	void _baked_light_changed();
	void _update_visibility();
protected:

	void _notification(int p_what);
	static void _bind_methods();
public:

	void set_flag(Flags p_flag,bool p_value);
	bool get_flag(Flags p_flag) const;

	void set_draw_range_begin(float p_dist);
	float get_draw_range_begin() const;

	void set_draw_range_end(float p_dist);
	float get_draw_range_end() const;

	void set_material_override(const Ref<Material>& p_material);
	Ref<Material> get_material_override() const;

	void set_baked_light_texture_id(int p_id);
	int get_baked_light_texture_id() const;

	GeometryInstance3D();
};

VARIANT_ENUM_CAST( GeometryInstance3D::Flags );

#endif
