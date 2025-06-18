/*************************************************************************/
/*  register_server_types.cpp                                            */
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
#include "register_server_types.h"
#include "globals.h"

#include "visual_server.h"
#include "audio_server.h"
#include "physics_3d_server.h"
#include "physics_2d_server.h"
#ifdef DUMMY_3D_PHYSICS_ENABLED
#include "dummy_physics/dummy_physics_3d.h"
#endif
#ifdef DUMMY_2D_PHYSICS_ENABLED
#include "dummy_physics/dummy_physics_2d.h"
#endif
#include "spatial_sound_server.h"
#include "spatial_sound_2d_server.h"

void register_server_types() {

	Globals::get_singleton()->add_singleton( Globals::Singleton("VisualServer",VisualServer::get_singleton()) );
	Globals::get_singleton()->add_singleton( Globals::Singleton("VS",VisualServer::get_singleton()) );

	Globals::get_singleton()->add_singleton( Globals::Singleton("AudioServer",AudioServer::get_singleton()) );
	Globals::get_singleton()->add_singleton( Globals::Singleton("AS",AudioServer::get_singleton()) );

#ifndef PHYSICS_3D_DISABLED
	Globals::get_singleton()->add_singleton( Globals::Singleton("Physics3DServer",PhysicsServer3D::get_singleton()) );
	Globals::get_singleton()->add_singleton( Globals::Singleton("PS3D",PhysicsServer3D::get_singleton()) );
	REGISTER_OBJECT(Physics3DShapeQueryParameters);
	REGISTER_VIRTUAL_OBJECT(Physics3DDirectBodyState);
	REGISTER_VIRTUAL_OBJECT(Physics3DDirectSpaceState);
	REGISTER_VIRTUAL_OBJECT(Physics3DShapeQueryResult);
#ifdef DUMMY_3D_PHYSICS_ENABLED
	PhysicsServer3D::set_singleton(memnew(PhysicsServer3DDummy));
#endif
#endif

#ifndef PHYSICS_2D_DISABLED
	Globals::get_singleton()->add_singleton( Globals::Singleton("PhysicsServer2D",PhysicsServer2D::get_singleton()) );
	Globals::get_singleton()->add_singleton( Globals::Singleton("PS2D",PhysicsServer2D::get_singleton()) );
	REGISTER_OBJECT(Physics2DShapeQueryParameters);
	REGISTER_VIRTUAL_OBJECT(Physics2DDirectBodyState);
	REGISTER_VIRTUAL_OBJECT(Physics2DDirectSpaceState);
	REGISTER_VIRTUAL_OBJECT(Physics2DShapeQueryResult);
#ifdef DUMMY_2D_PHYSICS_ENABLED
	PhysicsServer2DDummy
	//Set dummy server as singleton
#endif
#endif

	Globals::get_singleton()->add_singleton( Globals::Singleton("SpatialSoundServer",SpatialSound2DServer::get_singleton()) );
	Globals::get_singleton()->add_singleton( Globals::Singleton("SS",SpatialSound2DServer::get_singleton()) );

	Globals::get_singleton()->add_singleton( Globals::Singleton("SpatialSound2DServer",SpatialSound2DServer::get_singleton()) );
	Globals::get_singleton()->add_singleton( Globals::Singleton("SS2D",SpatialSound2DServer::get_singleton()) );
}

void unregister_server_types(){
#ifndef PHYSICS_3D_DISABLED
	if (PhysicsServer3D::get_singleton()){
		memdelete(PhysicsServer3D::get_singleton());
	}
#endif
#ifndef PHYSICS_2D_DISABLED
	if (PhysicsServer2D::get_singleton()){
		memdelete(PhysicsServer2D::get_singleton());
	}
#endif
}
