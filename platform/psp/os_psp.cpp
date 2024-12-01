/*************************************************************************/
/*  OS_PSP.cpp                                                        */
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
#include "rasterizer_psp.h"
#include "drivers/gles1/rasterizer_gles1.h"
#include "os_psp.h"
#include <stdio.h>
#include <stdlib.h>
#include "print_string.h"
#include "servers/physics/physics_server_sw.h"
#include "drivers/unix/memory_pool_static_malloc.h"
#include "drivers/unix/dir_access_unix.h"
#include "drivers/unix/file_access_unix.h"
#include "os/memory_pool_dynamic_static.h"
#include "core/os/thread_dummy.h"
#include "drivers/unix/thread_posix.h"
#include "main/main.h"
#include <sys/time.h>
#include <unistd.h>
#include <pspaudio.h>
#include <pspkernel.h>
#include "drivers/unix/tcp_server_posix.h"
#include "drivers/unix/stream_peer_tcp_posix.h"
#include "drivers/unix/ip_unix.h"

// #include <GL/glut.h>
int OS_PSP::get_video_driver_count() const {

	return 1;
}
const char * OS_PSP::get_video_driver_name(int p_driver) const {

	return "sceGu";
}
OS::VideoMode OS_PSP::get_default_video_mode() const {


	return OS::VideoMode(480,272,true);

}

static MemoryPoolStaticMalloc *mempool_static=NULL;
static MemoryPoolDynamicStatic *mempool_dynamic=NULL;
	
	
void OS_PSP::initialize_core() {

	ThreadPosix::make_default();
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

#ifdef PSP_NET
	TCPServerPosix::make_default();
	StreamPeerTCPPosix::make_default();
	IP_Unix::make_default();
#endif
}

void OS_PSP::finalize_core() {
	if (mempool_dynamic)
		memdelete( mempool_dynamic );
	delete mempool_static;
}

void OS_PSP::initialize(const VideoMode& p_desired,int p_video_driver,int p_audio_driver) {

	args=OS::get_singleton()->get_cmdline_args();
	current_videomode=p_desired;
	main_loop=NULL;

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	
	rasterizer = memnew( RasterizerPSP );

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

}
void OS_PSP::finalize() {

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

void OS_PSP::set_mouse_show(bool p_show) {


}

PspCtrlButtons buttons[16] = {
		PSP_CTRL_CROSS,
		PSP_CTRL_CIRCLE,
		PSP_CTRL_SQUARE,
		PSP_CTRL_TRIANGLE,
		(PspCtrlButtons)0,
		(PspCtrlButtons)0,
		PSP_CTRL_LTRIGGER,
		PSP_CTRL_RTRIGGER,
		(PspCtrlButtons)0,
		(PspCtrlButtons)0,
		PSP_CTRL_SELECT,
		PSP_CTRL_START,
		PSP_CTRL_UP,
		PSP_CTRL_DOWN,
		PSP_CTRL_LEFT,
		PSP_CTRL_RIGHT
};

void OS_PSP::process_keys() {
	sceCtrlReadBufferPositive(&pad, 1);

	last++;

	for(int i = 0; i < 16; i++) {
		if (pad.Buttons & buttons[i]) {
			InputEvent event;
			event.type = InputEvent::JOYSTICK_BUTTON;
			event.device = 0;
			event.joy_button.button_index = i;
			event.joy_button.pressed = true;
			event.ID = last;
			input->parse_input_event(event);
		} else {
			InputEvent event;
			event.type = InputEvent::JOYSTICK_BUTTON;
			event.device = 0;
			event.joy_button.button_index = i;
			event.joy_button.pressed = false;
			event.ID = last;
			input->parse_input_event(event);
		}
	}

	uint8_t lx = (pad.Lx - 128);
	uint8_t ly = (pad.Ly - 128);

	input->set_joy_axis(0, 0, lx);
	input->set_joy_axis(0, 1, ly);
}

void OS_PSP::set_mouse_grab(bool p_grab) {

	grab=p_grab;
}
bool OS_PSP::is_mouse_grab_enabled() const {

	return grab;
}

int OS_PSP::get_mouse_button_state() const {

	return 0;
}

Point2 OS_PSP::get_mouse_pos() const {

	return Point2();
}

void OS_PSP::set_window_title(const String& p_title) {


}

void OS_PSP::set_video_mode(const VideoMode& p_video_mode,int p_screen) {


}

OS::Date OS_PSP::get_date() const {
	Date ret;
	return ret;
}

OS::Time OS_PSP::get_time() const {
	Time ret;
	return ret;
}


void OS_PSP::delay_usec(uint32_t p_usec) const{
	usleep(p_usec);
}

uint64_t OS_PSP::get_ticks_usec() const{
	struct timeval tv_now;
	gettimeofday(&tv_now, NULL);

	uint64_t longtime = (uint64_t)tv_now.tv_usec + (uint64_t)tv_now.tv_sec * 1000000L;
	longtime -= ticks_start;

	return longtime;
}

OS::VideoMode OS_PSP::get_video_mode(int p_screen) const {

	//return current_videomode;
	return get_default_video_mode();
}
void OS_PSP::get_fullscreen_mode_list(List<VideoMode> *p_list,int p_screen) const {


}


MainLoop *OS_PSP::get_main_loop() const {

	return main_loop;
}

void OS_PSP::delete_main_loop() {

	if (main_loop)
		memdelete(main_loop);
	main_loop=NULL;
}

void OS_PSP::set_main_loop( MainLoop * p_main_loop ) {

	main_loop=p_main_loop;
	input->set_main_loop(p_main_loop);
}

bool OS_PSP::can_draw() const {

	return true;
};


String OS_PSP::get_name() {

	return "PSP";
}



void OS_PSP::move_window_to_foreground() {

}

void OS_PSP::set_cursor_shape(CursorShape p_shape) {


}

void OS_PSP::process_audio() {
	// sceAudioOutput
}

int OS_PSP::psp_callback_thread(unsigned sz, void *thiz) {
	sceKernelRegisterExitCallback(
			sceKernelCreateCallback("Confirm Exit Callback", [](int, int, void *up) {
				reinterpret_cast<OS_PSP *>(up)->force_quit = true;
				return 0;
			}, *reinterpret_cast<void **>(thiz)));
	sceKernelSleepThreadCB();
	sceKernelExitThread(0);

	return 0;
}

void OS_PSP::run() {

	force_quit = false;
	
	if (!main_loop)
		return;
		
	main_loop->init();

	auto thiz = this;
	sceKernelStartThread(
			sceKernelCreateThread("Exit Callback Thread", psp_callback_thread, 0x11, 0x200, 0, nullptr),
			sizeof(this),
			&thiz);

	while (!force_quit) {
		// process_audio();
		process_keys();
		
		if (Main::iteration()==true)
			break;
	};

	main_loop->finish();
}

void OS_PSP::swap_buffers() {
	// glutSwapBuffers();
}

OS_PSP::OS_PSP() {

	AudioDriverManagerSW::add_driver(&driver_psp);
	//adriver here
	grab=false;
	_verbose_stdout=true;

};
