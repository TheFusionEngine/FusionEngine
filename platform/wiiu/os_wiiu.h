/*************************************************************************/
/*  os_server.h                                                          */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
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

#ifndef OS_WIIU_H
#define OS_WIIU_H



#include "servers/audio/audio_driver_dummy.h"
#include "os/input.h"
#include "drivers/unix/os_unix.h"
#include "servers/audio/audio_server_sw.h"
// #include "servers/physics_2d/physics_2d_server_wrap_mt.h"
#include "rasterizer_wiiu.h"
#include "servers/audio/sample_manager_sw.h"
#include "servers/physics_2d/physics_2d_server_sw.h"
#include "servers/physics_server.h"
#include "servers/spatial_sound/spatial_sound_server_sw.h"
#include "servers/spatial_sound_2d/spatial_sound_2d_server_sw.h"
#include "servers/visual/rasterizer.h"
#include "servers/visual_server.h"
#include <SDL2/SDL.h>
#include <coreinit/debug.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/osmesa.h>

class OS_WIIU : public OS {

	Rasterizer *rasterizer;
	VisualServer *visual_server;
	VideoMode current_videomode;
	List<String> args;
	MainLoop *main_loop;

    SDL_Window* window;
	SDL_Surface* screen;
	SDL_Renderer* renderer;
	void* frameBuffer;
	template <typename T>
	struct RGBA {
		T r;
		T g;
		T b;
		T a;
	};
	OSMesaContext ctx;
	bool grab;
	bool mesa = false;
	uint64_t ticks_start;

	PhysicsServer *physics_server;
	Physics2DServer *physics_2d_server;

	virtual void delete_main_loop();
// 	IP_Unix *ip_unix;

	AudioServerSW *audio_server;
	SampleManagerMallocSW *sample_manager;
	SpatialSoundServerSW *spatial_sound_server;
	SpatialSound2DServerSW *spatial_sound_2d_server;

	AudioDriverDummy driver_dummy;

	bool force_quit;

// 	JoyPadPSP *joy;
	// SceCtrlData pad;
	int last;

	InputDefault *input;

protected:
	virtual int get_video_driver_count() const;
	virtual const char *get_video_driver_name(int p_driver) const;
	virtual VideoMode get_default_video_mode() const;

	virtual void initialize(const VideoMode &p_desired, int p_video_driver, int p_audio_driver);
	virtual void finalize();

	virtual void finalize_core();
	virtual void initialize_core();

	virtual void set_main_loop(MainLoop *p_main_loop);

	virtual void init_keys();
	// virtual void process_keys();

public:
	virtual String get_name();
// 	virtual void print(const char *p_format, ...);
// 	virtual void vprint(const char* p_format, va_list p_list, bool p_stderr=false);
	virtual void set_cursor_shape(CursorShape p_shape);
	virtual void set_custom_mouse_cursor(const RES &p_cursor, CursorShape p_shape, const Vector2 &p_hotspot);

	virtual void set_mouse_show(bool p_show);
	virtual void set_mouse_grab(bool p_grab);
	virtual void vprint(const char *p_format, va_list p_list, bool p_stderr = false);
	virtual void alert(const String &p_alert, const String &p_title = "ALERT!");
	virtual String get_stdin_string(bool p_block = true);
	virtual Error execute(const String& p_path, const List<String>& p_arguments,bool p_blocking,ProcessID *r_child_id=NULL,String* r_pipe=NULL,int *r_exitcode=NULL);
	virtual Error kill(const ProcessID &p_pid);
	virtual bool has_environment(const String &p_var) const;
	virtual String get_environment(const String &p_var) const;
	virtual Date get_date() const;
	virtual Time get_time() const;

	virtual void delay_usec(uint32_t p_usec) const;
	virtual uint64_t get_ticks_usec() const;
	virtual bool is_mouse_grab_enabled() const;
	virtual Point2 get_mouse_pos() const;
	virtual int get_mouse_button_state() const;
	virtual void set_window_title(const String &p_title);
	virtual int get_audio_driver_count() const;
	virtual const char *get_audio_driver_name(int p_driver) const;
	virtual void swap_buffers();

	virtual MainLoop *get_main_loop() const;

	virtual bool can_draw() const;

	virtual void set_video_mode(const VideoMode &p_video_mode, int p_screen = 0);
	virtual VideoMode get_video_mode(int p_screen = 0) const;
	virtual void get_fullscreen_mode_list(List<VideoMode> *p_list, int p_screen = 0) const;

	virtual Size2 get_window_size() const;

	virtual void move_window_to_foreground();

	void run();

	OS_WIIU();
	~OS_WIIU() {
		OSMesaDestroyContext( ctx );
		free(frameBuffer);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
	}
};

#endif
