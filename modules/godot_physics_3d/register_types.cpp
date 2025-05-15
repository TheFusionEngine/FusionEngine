#include "register_types.h"

#include "physics_server_sw.h"

void register_godot_physics_3d_types(){
#ifndef PHYSICS_3D_DISABLED
    PhysicsServer3D::set_singleton(memnew(PhysicsServer3DSW));
    PhysicsServer3D::get_singleton()->init();
#endif
}

void unregister_godot_physics_3d_types(){
    //Server is cleaned up by unregister_server_types
}
