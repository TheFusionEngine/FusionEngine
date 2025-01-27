/*************************************************************************/
/*  context_gl_x11.h                                                     */
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
#ifndef CONTEXT_GL_X11_H
#define CONTEXT_GL_X11_H

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
#ifdef X11_ENABLED

#if defined(OPENGL_ENABLED) || defined(LEGACYGL_ENABLED)
#include <OSMesa/gl.h>
#define GLAPI extern
// #include <OSMesa/glu.h>
#include <OSMesa/osmesa.h>
#include <SDL/SDL.h>

#include "os/os.h"
#include "drivers/gl_context/context_gl.h"
// #include <X11/Xlib.h>


struct ContextGL_X11_Private;

class ContextGL_X11 : public ContextGL {

	// ContextGL_X11_Private *p;
	OS::VideoMode default_video_mode;
//	::Colormap x11_colormap;

	SDL_Surface* screen;
	OSMesaContext ctx;
	
	// namespace {
		
	template <typename T>
	struct RGBA {
		T r;
		T g;
		T b;
		T a;
	};
	// }

	
	void *frameBuffer;
	bool double_buffer;
	bool direct_render;
	int glx_minor,glx_major;
	bool opengl_3_context;
	int winSizeX = 800;
	int winSizeY = 600;
public:

	virtual void release_current();	
	virtual void make_current();	
	virtual void swap_buffers();
	virtual int get_window_width();
	virtual int get_window_height();

	virtual Error initialize();

	ContextGL_X11(const OS::VideoMode& p_default_video_mode,bool p_opengl_3_context);	
	~ContextGL_X11();

};

#endif

#endif
#endif
