/*************************************************************************/
/*  OS_Ultra.cpp                                                        */
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
#include "servers/visual/visual_server_raster.h"
#include "servers/visual/rasterizer_dummy.h"
#include "os_ultra.h"
#include <stdio.h>
#include <stdlib.h>
#include "print_string.h"
#include "servers/physics_3d_server.h"
#include "drivers/unix/mutex_posix.h"
#include "drivers/unix/thread_posix.h"
// #include "drivers/unix/semaphore_posix.h"
#include "drivers/unix/file_access_unix.h"
#include "drivers/unix/dir_access_unix.h"
#include "core/os/thread_dummy.h"
#include "drivers/unix/memory_pool_static_malloc.h"
#include "os/memory_pool_dynamic_static.h"

#include "main/main.h"
#include <unistd.h>

// #include <GL/glkos.h>

#include <sys/time.h>

int OS_Ultra::get_video_driver_count() const {

	return 1;
}
const char * OS_Ultra::get_video_driver_name(int p_driver) const {

	return "GLES1";
}
OS::VideoMode OS_Ultra::get_default_video_mode() const {

	return OS::VideoMode(640, 480,false);
}

static MemoryPoolStaticMalloc *mempool_static=NULL;
static MemoryPoolDynamicStatic *mempool_dynamic=NULL;

void OS_Ultra::initialize_core() {

	printf("init core\n");
 	ThreadDummy::make_default();
 	SemaphoreDummy::make_default();
 	MutexDummy::make_default();
	printf("init fs\n");

	ticks_start = 0;
	ticks_start = get_ticks_usec();

	mempool_static = new MemoryPoolStaticMalloc;
	mempool_dynamic = memnew( MemoryPoolDynamicStatic );

	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_RESOURCES);
	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_USERDATA);
	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_FILESYSTEM);
	//FileAccessBufferedFA<FileAccessUnix>::make_default();
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_RESOURCES);
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_USERDATA);
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_FILESYSTEM);
}

void OS_Ultra::finalize_core() {
	if (mempool_dynamic)
		memdelete( mempool_dynamic );
	delete mempool_static;
}

void OS_Ultra::initialize(const VideoMode& p_desired,int p_video_driver,int p_audio_driver) {

	args=OS::get_singleton()->get_cmdline_args();
	current_videomode=p_desired;
	main_loop=NULL;


	rasterizer = memnew( RasterizerDummy );

	visual_server = memnew( VisualServerRaster(rasterizer) );

	AudioDriverManagerSW::get_driver(p_audio_driver)->set_singleton();

	if (AudioDriverManagerSW::get_driver(p_audio_driver)->init()!=OK) {

		ERR_PRINT("Initializing audio failed.");
	}

	sample_manager = memnew( SampleManagerMallocSW );
	audio_server = memnew( AudioServerSW(sample_manager) );
	audio_server->init();
	spatial_sound_server = memnew( SpatialSoundServerSW );
	spatial_sound_server->init();
	spatial_sound_2d_server = memnew( SpatialSound2DServerSW );
	spatial_sound_2d_server->init();


	ERR_FAIL_COND(!visual_server);

	visual_server->init();
	//
	physics_server = memnew( PhysicsServer3DSW );
	physics_server->init();
	physics_2d_server = memnew( PhysicsServer2DSW );
	physics_2d_server->init();

	input = memnew( InputDefault );

	_ensure_data_dir();

	printf("INIT\n");
}
void OS_Ultra::finalize() {

	if(main_loop)
		memdelete(main_loop);
	main_loop=NULL;

	spatial_sound_server->finish();
	memdelete(spatial_sound_server);
	spatial_sound_2d_server->finish();
	memdelete(spatial_sound_2d_server);

	//if (debugger_connection_console) {
//		memdelete(debugger_connection_console);
//}

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

void OS_Ultra::set_mouse_show(bool p_show) {


}

OS::Date OS_Ultra::get_date() const {
	Date ret;
	return ret;
}

OS::Time OS_Ultra::get_time() const {
	Time ret;
	return ret;
}

void OS_Ultra::delay_usec(uint32_t p_usec) const {
	// usleep(p_usec);

}

uint64_t OS_Ultra::get_ticks_usec() const{
	struct timeval tv_now;
	gettimeofday(&tv_now, NULL);

	uint64_t longtime = (uint64_t)tv_now.tv_usec + (uint64_t)tv_now.tv_sec * 1000000L;
	longtime -= ticks_start;

	return longtime;
}

void OS_Ultra::set_mouse_grab(bool p_grab) {

	grab=p_grab;
}
bool OS_Ultra::is_mouse_grab_enabled() const {

	return grab;
}

int OS_Ultra::get_mouse_button_state() const {

	return 0;
}

Point2 OS_Ultra::get_mouse_pos() const {

	return Point2();
}

void OS_Ultra::set_window_title(const String& p_title) {


}

void OS_Ultra::set_video_mode(const VideoMode& p_video_mode,int p_screen) {


}
OS::VideoMode OS_Ultra::get_video_mode(int p_screen) const {

	return current_videomode;
}
void OS_Ultra::get_fullscreen_mode_list(List<VideoMode> *p_list,int p_screen) const {


}

void OS_Ultra::vprint(const char* p_format, va_list p_list,bool p_stder)
{
	if (p_stder) {
		vfprintf(stderr,p_format,p_list);
		fflush(stderr);
	} else {
		vprintf(p_format,p_list);
		fflush(stdout);
	}
}

MainLoop *OS_Ultra::get_main_loop() const {

	return main_loop;
}

void OS_Ultra::delete_main_loop() {

	if (main_loop)
		memdelete(main_loop);
	main_loop=NULL;
}

void OS_Ultra::set_main_loop( MainLoop * p_main_loop ) {

	main_loop=p_main_loop;
	input->set_main_loop(p_main_loop);
}

bool OS_Ultra::can_draw() const {

	return true; //please draw
};


String OS_Ultra::get_name() {

	return "Dreamcast KOS";
}



void OS_Ultra::move_window_to_foreground() {

}

void OS_Ultra::set_cursor_shape(CursorShape p_shape) {


}

void OS_Ultra::run() {

	force_quit = false;

	if (!main_loop)
		return;

	main_loop->init();

	while (!force_quit) {
		if (Main::iteration()==true)
			break;
	};

	main_loop->finish();
}

void OS_Ultra::swap_buffers() {
	// printf("swap \n");
	// glKosSwapBuffers();
}

OS_Ultra::OS_Ultra() {

	AudioDriverManagerSW::add_driver(&driver_dummy);
	//adriver here
	grab=false;

};
