/*************************************************************************/
/*  register_scene_types.cpp                                             */
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
#include "register_scene_types.h"
#include "os/os.h"
#include "globals.h"
#include "scene/io/resource_format_image.h"
#include "scene/io/resource_format_wav.h"

//#include "scene/io/scene_format_script.h"
#include "resources/default_theme/default_theme.h"
#include "object_type_db.h"
#include "scene/main/canvas_layer.h"
#include "scene/main/viewport.h"
#include "scene/gui/control.h"

#include "scene/gui/texture_progress.h"
#include "scene/gui/empty_control.h"
#include "scene/gui/button.h"
#include "scene/gui/button_array.h"
#include "scene/gui/button_group.h"
// #endif
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/scroll_bar.h"
#include "scene/gui/progress_bar.h"
#include "scene/gui/slider.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/spin_box.h"
#include "scene/gui/option_button.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/texture_frame.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/check_button.h"
#include "scene/gui/tab_container.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/tabs.h"
#include "scene/gui/center_container.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/panel.h"
#include "scene/gui/spin_box.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"
#include "scene/gui/text_edit.h"
#include "scene/gui/texture_button.h"
#include "scene/gui/separator.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/box_container.h"
#include "scene/gui/grid_container.h"
#include "scene/gui/split_container.h"
#include "scene/gui/video_player.h"
#include "scene/gui/reference_frame.h"
#include "scene/resources/video_stream.h"


#include "scene/2d/particles_2d.h"
#include "scene/2d/path_2d.h"

#include "scene/2d/canvas_item.h"
#include "scene/2d/sprite.h"
#include "scene/2d/animated_sprite.h"
#include "scene/2d/polygon_2d.h"

#include "scene/2d/visibility_notifier_2d.h"

#include "scene/2d/physics_body_2d.h"
#include "scene/2d/ray_cast_2d.h"
#include "scene/2d/joints_2d.h"
#include "scene/2d/area_2d.h"
#include "scene/2d/camera_2d.h"
#include "scene/2d/collision_shape_2d.h"
#include "scene/2d/collision_polygon_2d.h"
#include "scene/2d/parallax_background.h"
#include "scene/2d/parallax_layer.h"
#include "scene/2d/sound_player_2d.h"
#include "scene/2d/sample_player_2d.h"
#include "scene/2d/screen_button.h"
#include "scene/2d/remote_transform_2d.h"
#include "scene/2d/y_sort.h"

#include "scene/2d/position_2d.h"
#include "scene/2d/tile_map.h"
#include "scene/resources/tile_set.h"

#include "scene/animation/animation_player.h"
#include "scene/animation/animation_tree_player.h"
#include "scene/animation/tween.h"
#include "scene/main/scene_main_loop.h"
#include "scene/main/resource_preloader.h"
#include "scene/resources/packed_scene.h"


#include "scene/resources/surface_tool.h"
#include "scene/resources/mesh_data_tool.h"
#include "scene/resources/scene_preloader.h"

#include "scene/main/timer.h"

#include "scene/audio/stream_player.h"
#include "scene/audio/event_player.h"
#include "scene/audio/sound_room_params.h"
#include "scene/resources/sphere_shape.h"
#include "scene/resources/ray_shape.h"
#include "scene/resources/box_shape.h"
#include "scene/resources/capsule_shape.h"
#include "scene/resources/plane_shape.h"
#include "scene/resources/convex_polygon_shape.h"
#include "scene/resources/concave_polygon_shape.h"

#include "scene/resources/shape_line_2d.h"
#include "scene/resources/circle_shape_2d.h"
#include "scene/resources/segment_shape_2d.h"
#include "scene/resources/rectangle_shape_2d.h"
#include "scene/resources/capsule_shape_2d.h"
#include "scene/resources/convex_polygon_shape_2d.h"
#include "scene/resources/concave_polygon_shape_2d.h"
#include "scene/resources/mesh_library.h"

#include "scene/resources/polygon_path_finder.h"

#include "scene/resources/sample.h"
#include "scene/audio/sample_player.h"
#include "scene/resources/texture.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh.h"
#include "scene/resources/room.h"

#include "scene/resources/world.h"
#include "scene/resources/world_2d.h"
#include "scene/resources/volume.h"

#include "scene/resources/sample_library.h"
#include "scene/resources/audio_stream.h"
#include "scene/resources/gibberish_stream.h"
#include "scene/resources/bit_mask.h"
#include "scene/scene_string_names.h"


#include "scene/3d/spatial.h"
#include "scene/3d/skeleton.h"
#include "scene/3d/bone_attachment.h"
#include "scene/3d/room_instance.h"
#include "scene/3d/path.h"
#include "scene/3d/scenario_fx.h"

#include "scene/3d/visibility_notifier.h"

#ifndef NODE_3D_DISABLED
#include "scene/3d/camera.h"
#include "scene/3d/interpolated_camera.h"
#include "scene/3d/position_3d.h"
#include "scene/3d/test_cube.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/quad.h"
#include "scene/3d/light.h"
#include "scene/3d/particles.h"
#include "scene/3d/portal.h"
#include "scene/resources/environment.h"
#include "scene/3d/physics_body.h"
#include "scene/3d/vehicle_body.h"
#include "scene/3d/body_shape.h"
#include "scene/3d/area.h"
#include "scene/3d/physics_joint.h"
#include "scene/3d/multimesh_instance.h"
#include "scene/3d/baked_light_instance.h"
#include "scene/3d/ray_cast.h"
#include "scene/3d/immediate_geometry.h"
#include "scene/3d/sprite_3d.h"
#include "scene/3d/spatial_sample_player.h"
#include "scene/3d/spatial_stream_player.h"
#include "scene/3d/proximity_group.h"
#include "scene/3d/navigation_mesh.h"
#include "scene/3d/navigation.h"
#include "scene/3d/collision_polygon.h"
#endif



static ResourceFormatLoaderImage *resource_loader_image=NULL;
static ResourceFormatLoaderWAV *resource_loader_wav=NULL;
static ResourceFormatLoaderBitMap *resource_loader_bitmap=NULL;
static ResourceFormatLoaderTheme *resource_loader_theme=NULL;
static ResourceFormatLoaderShader *resource_loader_shader=NULL;

//static SceneStringNames *string_names;

void register_scene_types() {

	SceneStringNames::create();

	OS::get_singleton()->yield(); //may take time to init

	Node::init_node_hrcr();

	resource_loader_image = memnew( ResourceFormatLoaderImage );
	ResourceLoader::add_resource_format_loader( resource_loader_image );

	resource_loader_wav = memnew( ResourceFormatLoaderWAV );
	ResourceLoader::add_resource_format_loader( resource_loader_wav );

	resource_loader_bitmap = memnew( ResourceFormatLoaderBitMap );
	ResourceLoader::add_resource_format_loader( resource_loader_bitmap );

#ifdef TOOLS_ENABLED

	//scene first!

#endif

	resource_loader_theme = memnew( ResourceFormatLoaderTheme );
	ResourceLoader::add_resource_format_loader( resource_loader_theme );

	resource_loader_shader = memnew( ResourceFormatLoaderShader );
	ResourceLoader::add_resource_format_loader( resource_loader_shader );

	make_default_theme();

	OS::get_singleton()->yield(); //may take time to init

	REGISTER_OBJECT(Object);

	REGISTER_OBJECT(Node);

	REGISTER_OBJECT(Viewport);
	REGISTER_VIRTUAL_OBJECT(RenderTargetTexture);
	REGISTER_OBJECT(Timer);
	REGISTER_OBJECT(CanvasLayer);
	REGISTER_OBJECT(ResourcePreloader);

	/* REGISTER GUI */
	REGISTER_OBJECT(ButtonGroup);
	REGISTER_VIRTUAL_OBJECT(BaseButton);

	OS::get_singleton()->yield(); //may take time to init

	REGISTER_OBJECT(Control);
	REGISTER_OBJECT(EmptyControl);
	REGISTER_OBJECT(Button);
	REGISTER_OBJECT(Label);
	REGISTER_OBJECT(HScrollBar);
	REGISTER_OBJECT(VScrollBar);
	REGISTER_OBJECT(ProgressBar);
	REGISTER_OBJECT(HSlider);
	REGISTER_OBJECT(VSlider);
	REGISTER_OBJECT(Popup);
	REGISTER_OBJECT(PopupPanel);
	REGISTER_OBJECT(MenuButton);
	REGISTER_OBJECT(CheckButton);
	REGISTER_OBJECT(Panel);
	REGISTER_OBJECT(Range);

	OS::get_singleton()->yield(); //may take time to init

	REGISTER_OBJECT(TextureFrame);
	REGISTER_OBJECT(TabContainer);
	REGISTER_OBJECT(Tabs);
	REGISTER_VIRTUAL_OBJECT(Separator);
	REGISTER_OBJECT(HSeparator);
	REGISTER_OBJECT(VSeparator);
	REGISTER_OBJECT(TextureButton);
	REGISTER_OBJECT(Container);
	REGISTER_VIRTUAL_OBJECT(BoxContainer);
	REGISTER_OBJECT(HBoxContainer);
	REGISTER_OBJECT(VBoxContainer);
	REGISTER_OBJECT(GridContainer);
	REGISTER_OBJECT(CenterContainer);
	REGISTER_OBJECT(ScrollContainer);
	REGISTER_OBJECT(PanelContainer);
	REGISTER_VIRTUAL_OBJECT(SplitContainer);
	REGISTER_OBJECT(HSplitContainer);
	REGISTER_OBJECT(VSplitContainer);
	OS::get_singleton()->yield(); //may take time to init


	REGISTER_VIRTUAL_OBJECT(ButtonArray);
	REGISTER_OBJECT(HButtonArray);
	REGISTER_OBJECT(VButtonArray);
	REGISTER_OBJECT(TextureProgress);


#ifndef	ADVANCED_GUI_DISABLED

	REGISTER_OBJECT(FileDialog);
	REGISTER_OBJECT(LineEdit);
	REGISTER_OBJECT(PopupMenu);
	REGISTER_OBJECT(Tree);
	REGISTER_OBJECT(TextEdit);

	REGISTER_VIRTUAL_OBJECT(TreeItem);
	REGISTER_OBJECT(OptionButton);
	REGISTER_OBJECT(SpinBox);
	REGISTER_OBJECT(ReferenceFrame);
	REGISTER_OBJECT(ColorPicker);
	REGISTER_OBJECT(ColorPickerButton);
	REGISTER_OBJECT(RichTextLabel);
	REGISTER_OBJECT(PopupDialog);
	REGISTER_OBJECT(WindowDialog);
	REGISTER_OBJECT(AcceptDialog);
	REGISTER_OBJECT(ConfirmationDialog);
	REGISTER_OBJECT(VideoPlayer);
	REGISTER_OBJECT(MarginContainer);

	OS::get_singleton()->yield(); //may take time to init

#endif
	/* REGISTER 3D */

	REGISTER_OBJECT(Node3D);
	REGISTER_OBJECT(Skeleton3D);
	REGISTER_OBJECT(AnimationPlayer);
	REGISTER_OBJECT(Tween);

	OS::get_singleton()->yield(); //may take time to init

#ifndef NODE_3D_DISABLED
	REGISTER_OBJECT(BoneAttachment3D);
	REGISTER_VIRTUAL_OBJECT(VisualInstance3D);
	REGISTER_OBJECT(Camera3D);
	REGISTER_OBJECT(InterpolatedCamera);
	REGISTER_OBJECT(TestCube);
	REGISTER_OBJECT(MeshInstance3D);
	REGISTER_OBJECT(ImmediateGeometry3D);
	REGISTER_OBJECT(Sprite3D);
	REGISTER_OBJECT(AnimatedSprite3D);
	REGISTER_VIRTUAL_OBJECT(Light3D);
	REGISTER_OBJECT(DirectionalLight3D);
	REGISTER_OBJECT(OmniLight3D);
	REGISTER_OBJECT(SpotLight3D);
	REGISTER_OBJECT(AnimationTreePlayer);
	REGISTER_OBJECT(Portal);
	REGISTER_OBJECT(Particles3D);
	REGISTER_OBJECT(Position3D);
	REGISTER_OBJECT(Quad);
	REGISTER_OBJECT(NavigationMeshInstance);
	REGISTER_OBJECT(NavigationMesh);
	REGISTER_OBJECT(Navigation);
	OS::get_singleton()->yield(); //may take time to init

	REGISTER_VIRTUAL_OBJECT(CollisionObject3D);
	REGISTER_OBJECT(StaticBody3D);
	REGISTER_OBJECT(RigidBody3D);
	REGISTER_OBJECT(CharacterBody3D);

	REGISTER_OBJECT(VehicleBody3D);
	REGISTER_OBJECT(VehicleWheel3D);
	REGISTER_OBJECT(Area3D);
	REGISTER_OBJECT(ProximityGroup);
	REGISTER_OBJECT(CollisionShape3D);
	REGISTER_OBJECT(CollisionPolygon3D);
	REGISTER_OBJECT(RayCast3D);
	REGISTER_OBJECT(MultiMeshInstance3D);
	REGISTER_OBJECT(Room);
	REGISTER_OBJECT(Curve3D);
	REGISTER_OBJECT(Path3D);
	REGISTER_OBJECT(PathFollow3D);
	REGISTER_OBJECT(VisibilityNotifier3D);
	REGISTER_OBJECT(VisibilityEnabler3D);
	REGISTER_OBJECT(BakedLightInstance);
	REGISTER_OBJECT(BakedLightSampler);
	REGISTER_OBJECT(WorldEnvironment);

	REGISTER_VIRTUAL_OBJECT(Joint3D);
	REGISTER_OBJECT(PinJoint3D);
	REGISTER_OBJECT(HingeJoint3D);
	REGISTER_OBJECT(SliderJoint3D);
	REGISTER_OBJECT(ConeTwistJoint);
	REGISTER_OBJECT(Generic6DOFJoint);

	//scenariofx	

	OS::get_singleton()->yield(); //may take time to init

	REGISTER_OBJECT(SamplePlayer3D);
	REGISTER_OBJECT(StreamPlayer3D);
	REGISTER_OBJECT(SoundRoomParams);

#endif
	REGISTER_OBJECT(MeshLibrary);
#ifndef ULTRA
	AcceptDialog::set_swap_ok_cancel( GLOBAL_DEF("display/swap_ok_cancel",bool(OS::get_singleton()->get_swap_ok_cancel())) );
#endif
	REGISTER_OBJECT(SamplePlayer);


//	REGISTER_OBJECT(StaticBody3D);
//	REGISTER_OBJECT(RigidBody3D);
//	REGISTER_OBJECT(CharacterBody);
//	REGISTER_OBJECT(BodyVolumeSphere);
	//REGISTER_OBJECT(BodyVolumeBox);
	//REGISTER_OBJECT(BodyVolumeCylinder);
	//REGISTER_OBJECT(BodyVolumeCapsule);
	//REGISTER_OBJECT(PhysicsJointPin);




	REGISTER_OBJECT(StreamPlayer);
	REGISTER_OBJECT(EventPlayer);


	/* disable types by default, only editors should enable them */
	ObjectTypeDB::set_type_enabled("CollisionShape3D",false);
	//ObjectTypeDB::set_type_enabled("BodyVolumeSphere",false);
	//ObjectTypeDB::set_type_enabled("BodyVolumeBox",false);
	//ObjectTypeDB::set_type_enabled("BodyVolumeCapsule",false);
	//ObjectTypeDB::set_type_enabled("BodyVolumeCylinder",false);
	//ObjectTypeDB::set_type_enabled("BodyVolumeConvexPolygon",false);
	REGISTER_VIRTUAL_OBJECT(CanvasItem);
	REGISTER_OBJECT(Node2D);
	REGISTER_OBJECT(Particles2D);
	REGISTER_OBJECT(ParticleAttractor2D);
	REGISTER_OBJECT(Sprite2D);
	REGISTER_OBJECT(ViewportSprite);
	REGISTER_OBJECT(SpriteFrames);
	REGISTER_OBJECT(AnimatedSprite2D);
	REGISTER_OBJECT(Position2D);
	REGISTER_VIRTUAL_OBJECT(CollisionObject2D);
	REGISTER_VIRTUAL_OBJECT(PhysicsBody2D);
	REGISTER_OBJECT(StaticBody2D);
	REGISTER_OBJECT(RigidBody2D);
	REGISTER_OBJECT(CharacterBody2D);
	REGISTER_OBJECT(Area2D);
	REGISTER_OBJECT(CollisionShape2D);
	REGISTER_OBJECT(CollisionPolygon2D);
	REGISTER_OBJECT(RayCast2D);
	REGISTER_OBJECT(VisibilityNotifier2D);
	REGISTER_OBJECT(VisibilityEnabler2D);
	REGISTER_OBJECT(Polygon2D);
	REGISTER_OBJECT(YSort);

	ObjectTypeDB::set_type_enabled("CollisionShape2D",false);
	ObjectTypeDB::set_type_enabled("CollisionPolygon2D",false);

	OS::get_singleton()->yield(); //may take time to init

	REGISTER_OBJECT(Camera2D);
	REGISTER_VIRTUAL_OBJECT(Joint2D);
	REGISTER_OBJECT(PinJoint2D);
	REGISTER_OBJECT(GrooveJoint2D);
	REGISTER_OBJECT(DampedSpringJoint2D);
	REGISTER_OBJECT(TileSet);
	REGISTER_OBJECT(TileMap);
	REGISTER_OBJECT(ParallaxBackground);
	REGISTER_OBJECT(ParallaxLayer);
	REGISTER_VIRTUAL_OBJECT(SoundPlayer2D);
	REGISTER_OBJECT(SamplePlayer2D);
	REGISTER_OBJECT(TouchScreenButton);
	REGISTER_OBJECT(RemoteTransform2D);

	OS::get_singleton()->yield(); //may take time to init

	/* REGISTER RESOURCES */
#ifndef NODE_3D_DISABLED
	REGISTER_OBJECT(Mesh);
	REGISTER_VIRTUAL_OBJECT(Material);
	REGISTER_OBJECT(FixedMaterial);
	REGISTER_OBJECT(ParticleSystemMaterial);
	REGISTER_OBJECT(UnshadedMaterial);
	REGISTER_OBJECT(ShaderMaterial);
	REGISTER_OBJECT(RoomBounds);
	REGISTER_OBJECT(Shader);
	REGISTER_OBJECT(MultiMesh);
	REGISTER_OBJECT(MeshLibrary);

	OS::get_singleton()->yield(); //may take time to init

	REGISTER_OBJECT(RayShape3D);
	REGISTER_OBJECT(SphereShape3D);
	REGISTER_OBJECT(BoxShape3D);
	REGISTER_OBJECT(CapsuleShape3D);
	REGISTER_OBJECT(PlaneShape3D);
	REGISTER_OBJECT(ConvexPolygonShape3D);
	REGISTER_OBJECT(ConcavePolygonShape3D);

	REGISTER_OBJECT(SurfaceTool);
	REGISTER_OBJECT(MeshDataTool);
	REGISTER_OBJECT(BakedLight);
	OS::get_singleton()->yield(); //may take time to init

#endif

	REGISTER_OBJECT(World3D);
	REGISTER_OBJECT(Environment);
	REGISTER_OBJECT(World2D);
	REGISTER_VIRTUAL_OBJECT(Texture);
	REGISTER_OBJECT(ImageTexture);
	REGISTER_OBJECT(AtlasTexture);
	REGISTER_OBJECT(LargeTexture);
	REGISTER_OBJECT(CubeMap);
	REGISTER_OBJECT(Animation);
	REGISTER_OBJECT(Font);
	REGISTER_OBJECT(StyleBoxEmpty);
	REGISTER_OBJECT(StyleBoxTexture);
	REGISTER_OBJECT(StyleBoxFlat);
	REGISTER_OBJECT(StyleBoxImageMask);
	REGISTER_OBJECT(Theme);

	REGISTER_OBJECT(PolygonPathFinder);
	REGISTER_OBJECT(BitMap);

	OS::get_singleton()->yield(); //may take time to init

	//REGISTER_OBJECT(Volume);
	REGISTER_OBJECT(Sample);
	REGISTER_OBJECT(SampleLibrary);
	REGISTER_VIRTUAL_OBJECT(AudioStream);
	REGISTER_OBJECT(AudioStreamGibberish);
	REGISTER_VIRTUAL_OBJECT(VideoStream);

	OS::get_singleton()->yield(); //may take time to init
	REGISTER_VIRTUAL_OBJECT(Shape2D);
	REGISTER_OBJECT(LineShape2D);
	REGISTER_OBJECT(SegmentShape2D);
	REGISTER_OBJECT(RayShape2D);
	REGISTER_OBJECT(CircleShape2D);
	REGISTER_OBJECT(RectangleShape2D);
	REGISTER_OBJECT(CapsuleShape2D);
	REGISTER_OBJECT(ConvexPolygonShape2D);
	REGISTER_OBJECT(ConcavePolygonShape2D);
	REGISTER_OBJECT(Curve2D);
	REGISTER_OBJECT(Path2D);
	REGISTER_OBJECT(PathFollow2D);

	OS::get_singleton()->yield(); //may take time to init
	REGISTER_OBJECT(PackedScene);

	REGISTER_OBJECT(SceneTree);

	OS::get_singleton()->yield(); //may take time to init

}

void unregister_scene_types() {
	
	clear_default_theme();
	
	memdelete( resource_loader_image );
	memdelete( resource_loader_wav );
	memdelete( resource_loader_bitmap );

	memdelete( resource_loader_theme );
	memdelete( resource_loader_shader );
}
