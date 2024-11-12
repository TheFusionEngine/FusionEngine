/*************************************************************************/
/*  OS_WII.cpp                                                        */
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
#include "drivers/gles1/rasterizer_gles1.h"
#include "drivers/unix/tcp_server_posix.h"
#include "drivers/unix/stream_peer_tcp_posix.h"
#include "drivers/unix/ip_unix.h"
#include "os_wii.h"
#include <stdio.h>
#include <stdlib.h>
#include "print_string.h"
#include "servers/physics/physics_server_sw.h"
#include "drivers/unix/memory_pool_static_malloc.h"
#include "drivers/unix/dir_access_unix.h"
#include "drivers/unix/file_access_unix.h"
#include "os/memory_pool_dynamic_static.h"
#include "core/os/thread_dummy.h"
#include "network2.h"

#include "main/main.h"

#include <glad/glad.h>

#include <unistd.h>
#include <sys/time.h>
#include <sys/errno.h>

int OS_WII::get_video_driver_count() const {

	return 1;
}
const char * OS_WII::get_video_driver_name(int p_driver) const {

	return "opengx";
}

OS::VideoMode OS_WII::get_default_video_mode() const {

	return OS::VideoMode(640,480,false);
}

static MemoryPoolStaticMalloc *mempool_static=NULL;
static MemoryPoolDynamicStatic *mempool_dynamic=NULL;

extern "C" {
	void *ogx_get_proc_address(const char *proc);
	void ogx_initialize();
}

static void *getprocaddr(const char *proc) {
	void *addr = ogx_get_proc_address(proc);
	printf("%s: %s = %p\n", __FUNCTION__, proc, addr);
	return addr;
}

void OS_WII::initialize_core() {
	SYS_STDIO_Report(true);

	net_deinit();
	int rv;
	while ((rv = net_init()) == -EAGAIN) ;
	if (rv < 0) {
		printf("Error while initializing the network: %s\n", strerror(-rv));
	} else {
		printf("Net result: %d\n", rv);
	}

	ThreadDummy::make_default();
	SemaphoreDummy::make_default();
	MutexDummy::make_default();

	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_RESOURCES);
	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_USERDATA);
	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_FILESYSTEM);
	//FileAccessBufferedFA<FileAccessUnix>::make_default();
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_RESOURCES);
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_USERDATA);
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_FILESYSTEM);


	mempool_static = new MemoryPoolStaticMalloc;
	mempool_dynamic = memnew( MemoryPoolDynamicStatic );
	
	ticks_start = 0;
	ticks_start = get_ticks_usec();

	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_RESOURCES);
	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_USERDATA);
	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_FILESYSTEM);
	//FileAccessBufferedFA<FileAccessUnix>::make_default();
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_RESOURCES);
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_USERDATA);
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_FILESYSTEM);

	TCPServerPosix::make_default();
	StreamPeerTCPPosix::make_default();
	IP_Unix::make_default();
	
	SDL_Init(SDL_INIT_VIDEO);
	videoInfo = SDL_GetVideoInfo();
	
	videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
	videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
	videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
	videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */
	videoFlags |= SDL_FULLSCREEN;

	if ( videoInfo->hw_available )
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;

	if ( videoInfo->blit_hw )
		videoFlags |= SDL_HWACCEL;

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	surface = SDL_SetVideoMode(640, 480, 32, videoFlags);
	ogx_initialize();

	gladLoadGLLoader(getprocaddr);
	printf("glEnableClientState => %p\n", glad_glEnableClientState);
}

void OS_WII::finalize_core() {
	if (mempool_dynamic)
		memdelete( mempool_dynamic );
	delete mempool_static;
}


void OS_WII::initialize(const VideoMode& p_desired,int p_video_driver,int p_audio_driver) {

	args=OS::get_singleton()->get_cmdline_args();
	current_videomode=p_desired;
	main_loop=NULL;

	
	rasterizer = memnew( RasterizerGLES1 );

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
	physics_server = memnew( PhysicsServerSW );
	physics_server->init();
	physics_2d_server = memnew( Physics2DServerSW );
	physics_2d_server->init();

	input = memnew( InputDefault );

	// _ensure_data_dir();

		
}
void OS_WII::finalize() {

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

void OS_WII::set_mouse_show(bool p_show) {


}
void OS_WII::set_mouse_grab(bool p_grab) {

	grab=p_grab;
}
bool OS_WII::is_mouse_grab_enabled() const {

	return grab;
}

int OS_WII::get_mouse_button_state() const {

	return 0;
}

Point2 OS_WII::get_mouse_pos() const {

	return Point2();
}

void OS_WII::set_window_title(const String& p_title) {


}

void OS_WII::set_video_mode(const VideoMode& p_video_mode,int p_screen) {


}
OS::VideoMode OS_WII::get_video_mode(int p_screen) const {

	return current_videomode;
}
void OS_WII::get_fullscreen_mode_list(List<VideoMode> *p_list,int p_screen) const {


}


OS::Date OS_WII::get_date() const {
	Date ret;
	return ret;
}

OS::Time OS_WII::get_time() const {
	Time ret;
	return ret;
}


void OS_WII::delay_usec(uint32_t p_usec) const{
	usleep(p_usec);
}

uint64_t OS_WII::get_ticks_usec() const{
	struct timeval tv_now;
	gettimeofday(&tv_now, NULL);

	uint64_t longtime = (uint64_t)tv_now.tv_usec + (uint64_t)tv_now.tv_sec * 1000000L;
	longtime -= ticks_start;

	return longtime;
}

MainLoop *OS_WII::get_main_loop() const {

	return main_loop;
}

void OS_WII::delete_main_loop() {

	if (main_loop)
		memdelete(main_loop);
	main_loop=NULL;
}

void OS_WII::set_main_loop( MainLoop * p_main_loop ) {

	main_loop=p_main_loop;
	input->set_main_loop(p_main_loop);
}

bool OS_WII::can_draw() const {

	return true; //can never draw
};


String OS_WII::get_name() {

	return "Wii";
}



void OS_WII::move_window_to_foreground() {

}

void OS_WII::set_cursor_shape(CursorShape p_shape) {


}

void OS_WII::swap_buffers() {
	SDL_GL_SwapBuffers();
}

void OS_WII::process_input() {
	
	while ( SDL_PollEvent( &event ) )
		{
		switch( event.type )
		{

		case SDL_QUIT:
			/* handle quit requests */
			force_quit = true;
			break;
		default:
			break;
		}
	}
}

void OS_WII::run() {

	force_quit = false;
	
	if (!main_loop)
		return;
		
	main_loop->init();
		
	while (!force_quit) {
		process_input();
		
		if (Main::iteration()==true)
			break;
	};
	
	main_loop->finish();
}

OS_WII::OS_WII() {

	AudioDriverManagerSW::add_driver(&driver_dummy);
	//adriver here
	grab=false;

};
