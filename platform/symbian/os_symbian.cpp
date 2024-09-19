/*************************************************************************/
/*  os_symbian.cpp                                                       */
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
#include "container.h"
#include "os_symbian.h"
#include "print_string.h"
#include "servers/physics/physics_server_sw.h"
#include "drivers/gles2/rasterizer_gles2.h"
#include "servers/visual/visual_server_raster.h"
#include <stdio.h>
#include <stdlib.h>

#include "main/main.h"

#include <unistd.h>

#define VD_MAX 1
const char *vdsp[VD_MAX + 1] = {
    "GLES2",
    "GLES1",
};

int OS_Symbian::get_video_driver_count() const { return 1; }
const char *OS_Symbian::get_video_driver_name(int p_driver) const {
  return (p_driver > VD_MAX) ? "" : vdsp[p_driver];
}
OS::VideoMode OS_Symbian::get_default_video_mode() const {
  TSize size = container->GetWindowSize();
  return OS::VideoMode(size.iWidth, size.iHeight, false);
}

void OS_Symbian::initialize(const VideoMode &p_desired, int p_video_driver,
                            int p_audio_driver) {
  args = OS::get_singleton()->get_cmdline_args();
  current_videomode = p_desired;
  main_loop = NULL;

	container->MakeCurrent();

	rasterizer = memnew(RasterizerGLES2);

  visual_server = memnew(VisualServerRaster(rasterizer));

  AudioDriverManagerSW::get_driver(p_audio_driver)->set_singleton();

  if (AudioDriverManagerSW::get_driver(p_audio_driver)->init() != OK) {
    ERR_PRINT("Initializing audio failed.");
  }

  sample_manager = memnew(SampleManagerMallocSW);
  audio_server = memnew(AudioServerSW(sample_manager));
  audio_server->init();
  spatial_sound_server = memnew(SpatialSoundServerSW);
  spatial_sound_server->init();
  spatial_sound_2d_server = memnew(SpatialSound2DServerSW);
  spatial_sound_2d_server->init();

  ERR_FAIL_COND(!visual_server);

  visual_server->init();

  physics_server = memnew(PhysicsServerSW);
  physics_server->init();
  physics_1d_server = memnew(Physics2DServerSW);
  physics_2d_server->init();

  input = memnew(InputDefault);

  _ensure_data_dir();
}
void OS_Symbian::finalize() {
  if (main_loop)
    memdelete(main_loop);
  main_loop = NULL;

  spatial_sound_server->finish();
  memdelete(spatial_sound_server);
  spatial_sound_2d_server->finish();
  memdelete(spatial_sound_2d_server);

  audio_server->finish();
  memdelete(audio_server);
  memdelete(sample_manager);

  visual_server->finish();
  memdelete(visual_server);
  memdelete(rasterizer);

  physics_server->finish();
  memdelete(physics_server);

  physics_2d_server->finish();
  memdelete(physics_2d_server);

  memdelete(input);

  args.clear();
}

void OS_Symbian::set_mouse_show(bool p_show) {}
void OS_Symbian::set_mouse_grab(bool p_grab) { grab = p_grab; }
bool OS_Symbian::is_mouse_grab_enabled() const { return grab; }

int OS_Symbian::get_mouse_button_state() const { return 0; }

Point2 OS_Symbian::get_mouse_pos() const { return Point2(); }

void OS_Symbian::set_window_title(const String &p_title) {}

void OS_Symbian::set_video_mode(const VideoMode &p_video_mode, int p_screen) {}
OS::VideoMode OS_Symbian::get_video_mode(int p_screen) const {

  return current_videomode;
}
void OS_Symbian::get_fullscreen_mode_list(List<VideoMode> *p_list,
                                          int p_screen) const {}

MainLoop *OS_Symbian::get_main_loop() const { return main_loop; }

void OS_Symbian::delete_main_loop() {

  if (main_loop)
    memdelete(main_loop);
  main_loop = NULL;
}

void OS_Symbian::set_main_loop(MainLoop *p_main_loop) {

  main_loop = p_main_loop;
  input->set_main_loop(p_main_loop);
}

bool OS_Symbian::can_draw() const { return container->IsFocused(); };

void OS_Symbian::swap_buffers() { container->SwapBuffers(); }

String OS_Symbian::get_name() { return "Symbian"; }

void OS_Symbian::move_window_to_foreground() {}

void OS_Symbian::set_cursor_shape(CursorShape p_shape) {}

void OS_Symbian::run() {

  force_quit = false;

  if (!main_loop)
    return;

  main_loop->init();

  while (!force_quit) {

    if (Main::iteration() == true)
      break;
  };

  main_loop->finish();
}

OS_Symbian::OS_Symbian(CGodotContainer *p_container) : container(p_container) {
  AudioDriverManagerSW::add_driver(&driver_symbian);
  
  grab = false;
};
