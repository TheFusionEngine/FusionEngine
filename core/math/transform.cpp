/*************************************************************************/
/*  transform.cpp                                                        */
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
#include "transform.h"
#include "math_funcs.h"
#include "os/copymem.h"
#include "print_string.h"
	

void Transform3D::affine_invert() {

	basis.invert();
	origin = basis.xform(-origin);
}

Transform3D Transform3D::affine_inverse() const {

	Transform3D ret=*this;
	ret.affine_invert();
	return ret;

}


void Transform3D::invert() {

	basis.transpose();
	origin = basis.xform(-origin);	
}

Transform3D Transform3D::inverse() const {

	Transform3D ret=*this;
	ret.invert();
	return ret;
}


void Transform3D::rotate(const Vector3& p_axis,real_t p_phi) {

	*this = *this * Transform3D( Basis( p_axis, p_phi ), Vector3()  );
}

Transform3D Transform3D::rotated(const Vector3& p_axis,real_t p_phi) const{

	return *this * Transform3D( Basis( p_axis, p_phi ), Vector3()  );
}

void Transform3D::rotate_basis(const Vector3& p_axis,real_t p_phi) {

	basis.rotate(p_axis,p_phi);
}

Transform3D Transform3D::looking_at( const Vector3& p_target, const Vector3& p_up ) const {

	Transform3D t = *this;
	t.set_look_at(origin,p_target,p_up);
	return t;
}

void Transform3D::set_look_at( const Vector3& p_eye, const Vector3& p_target, const Vector3& p_up ) {

	// RefCounted: MESA source code
	Vector3 v_x, v_y, v_z;
	
	/* Make rotation matrix */
	
	/* Z vector */
	v_z = p_eye - p_target;
	
	v_z.normalize();
	
	v_y = p_up;
	

	v_x=v_y.cross(v_z);
	
	/* Recompute Y = Z cross X */
	v_y=v_z.cross(v_x);
	
	v_x.normalize();
	v_y.normalize();
	
	basis.set_axis(0,v_x);
	basis.set_axis(1,v_y);
	basis.set_axis(2,v_z);
	origin=p_eye;
	
}

Transform3D Transform3D::interpolate_with(const Transform3D& p_transform, float p_c) const {

	/* not sure if very "efficient" but good enough? */

	Vector3 src_scale = basis.get_scale();
	Quat src_rot = basis;
	Vector3 src_loc = origin;

	Vector3 dst_scale = p_transform.basis.get_scale();
	Quat dst_rot = p_transform.basis;
	Vector3 dst_loc = p_transform.origin;

	Transform3D dst;
	dst.basis=src_rot.slerp(dst_rot,p_c);
	dst.basis.scale(src_scale.linear_interpolate(dst_scale,p_c));
	dst.origin=src_loc.linear_interpolate(dst_loc,p_c);

	return dst;
}

void Transform3D::scale(const Vector3& p_scale) {

	basis.scale(p_scale);
	origin*=p_scale;
}

Transform3D Transform3D::scaled(const Vector3& p_scale) const {

	Transform3D t = *this;
	t.scale(p_scale);
	return t;
}

void Transform3D::scale_basis(const Vector3& p_scale) {

	basis.scale(p_scale);
}

void Transform3D::translate( real_t p_tx, real_t p_ty, real_t p_tz)  {
	translate( Vector3(p_tx,p_ty,p_tz) );

}
void Transform3D::translate( const Vector3& p_translation ) {

	for( int i = 0; i < 3; i++ ) {
		origin[i] += basis[i].dot(p_translation);
	}
}

Transform3D Transform3D::translated( const Vector3& p_translation ) const {

	Transform3D t=*this;
	t.translate(p_translation);
	return t;
}

void Transform3D::orthonormalize() {

	basis.orthonormalize();
}

Transform3D Transform3D::orthonormalized() const {

	Transform3D _copy = *this;
	_copy.orthonormalize();
	return _copy;
}

bool Transform3D::operator==(const Transform3D& p_transform) const {

	return (basis==p_transform.basis && origin==p_transform.origin);
}
bool Transform3D::operator!=(const Transform3D& p_transform) const {

	return (basis!=p_transform.basis || origin!=p_transform.origin);
}

void Transform3D::operator*=(const Transform3D& p_transform) {

	origin=xform(p_transform.origin);
	basis*=p_transform.basis;	
}

Transform3D Transform3D::operator*(const Transform3D& p_transform) const {

	Transform3D t=*this;
	t*=p_transform;
	return t;
}

Transform3D::operator String() const {

	return basis.operator String() + " - " + origin.operator String();
}


Transform3D::Transform3D(const Basis& p_basis, const Vector3& p_origin) {

	basis=p_basis;
	origin=p_origin;
}


