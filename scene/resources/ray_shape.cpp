/*************************************************************************/
/*  ray_shape.cpp                                                        */
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
#include "ray_shape.h"

#include "servers/physics_server.h"



void RayShape3D::_update_shape() {

	PhysicsServer::get_singleton()->shape_set_data(get_shape(),length);
	emit_changed();
}

void RayShape3D::set_length(float p_length) {

	length=p_length;
	_update_shape();
	notify_change_to_owners();
}

float RayShape3D::get_length() const {

	return length;
}


void RayShape3D::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("set_length","length"),&RayShape3D::set_length);
	ObjectTypeDB::bind_method(_MD("get_length"),&RayShape3D::get_length);

	ADD_PROPERTY( PropertyInfo(Variant::REAL,"length",PROPERTY_HINT_RANGE,"0,4096,0.01"), _SCS("set_length"), _SCS("get_length") );

}

RayShape3D::RayShape3D() : Shape3D( PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_RAY)) {

	set_length(1.0);
}
