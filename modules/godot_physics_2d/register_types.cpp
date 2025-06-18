#include "register_types.h"

#include "physics_2d_server_sw.h"

void register_godot_physics_2d_types(){
#ifndef PHYSICS_2D_DISABLED
    PhysicsServer2D::set_singleton(memnew(PhysicsServer2DSW));
    PhysicsServer2D::get_singleton()->init();
#endif
}

void unregister_godot_physics_2d_types(){
    //Server is cleaned up by unregister_server_types
}
