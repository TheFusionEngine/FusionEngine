/*************************************************************************/
/*  context_gl_x11.cpp                                                   */
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
#include "context_gl_x11.h"

#ifdef X11_ENABLED
#if defined(OPENGL_ENABLED) || defined(LEGACYGL_ENABLED)
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <algorithm>
// #include <GL/glx.h>

// typedef GLXContext (*GLXCREATECONTEXTATTRIBSARBPROC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);


void ContextGL_X11::release_current() {

	// glXMakeCurrent(x11_display, None, NULL);
}

void ContextGL_X11::make_current() {

	// glXMakeCurrent(x11_display, x11_window, p->glx_context);
}
void ContextGL_X11::swap_buffers() {

		if (SDL_MUSTLOCK(screen) && (SDL_LockSurface(screen)<0)) {
	    fprintf(stderr, "SDL ERROR: Can't lock screen: %s\n", SDL_GetError());
	    // return 1;
	}

	// SDL pixel buffer expects BGRA data, not RGBA data - read accordingly
	// glReadPixels(0, 0, winSizeX, winSizeY, GL_BGRA, GL_UNSIGNED_BYTE, screen->pixels);

	// Image will be upside-down from SDL's perspective, flip manually using an RGBA
	// variation of the math from:
	// https://github.com/vallentin/GLCollection/blob/master/screenshot.cpp
	//
	// Apparently this is necessary, since OpenGL doesn't provide a built-in way
	// to handle this:
	// https://www.opengl.org/archives/resources/features/KilgardTechniques/oglpitfall/
	/*
	for (int y = 0; y < winSizeY / 2; ++y) {
	    for (int x = 0; x < winSizeX; ++x) {
		int top = (x + y * winSizeX) * 4;
		int bottom = (x + (winSizeY - y - 1) * winSizeX) * 4;
		char rgba[4];
		memcpy(rgba, frameBuffer + top, sizeof(rgba));
		memcpy(screen->pixels + top, frameBuffer + bottom, sizeof(rgba));
		memcpy(screen->pixels + bottom, rgba, sizeof(rgba));
	    }
	}*/
	// memcpy(screen->pixels, &frameBuffer, sizeof(frameBuffer));
	// memcpy(screen->pixels, frameBuffer, winSizeX * winSizeY * 4);
	// std::transform((GLfloat *)frameBuffer, (GLfloat *)frameBuffer + (winSizeX * winSizeY * 4), (uint8_t *)screen->pixels, [](GLfloat v) { return v * 255; });
	std::transform((RGBA<GLfloat> *)frameBuffer,
               (RGBA<GLfloat> *)frameBuffer + (winSizeX * winSizeY),
               (RGBA<uint8_t> *)screen->pixels,
               [](RGBA<GLfloat> v) { return RGBA<uint8_t>{ v.b * 255, v.g * 255, v.r * 255,v.a * 255}; });

	if (SDL_MUSTLOCK(screen)) {
	    SDL_UnlockSurface(screen);
	}
	SDL_Flip(screen);
}
/*
static GLWrapperFuncPtr wrapper_get_proc_address(const char* p_function) {

	//print_line(String()+"getting proc of: "+p_function);
	GLWrapperFuncPtr func=(GLWrapperFuncPtr)glXGetProcAddress( (const GLubyte*) p_function);
	if (!func) {
		print_line("Couldn't find function: "+String(p_function));
	}

	return func;

}*/

Error ContextGL_X11::initialize() {

	
	SDL_Init(SDL_INIT_VIDEO);
	
    // SDL_Event evt;
    const SDL_VideoInfo* info;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	fprintf(stderr,"ERROR: cannot initialize SDL video.\n");
	// return 1;
    }

    info = SDL_GetVideoInfo();

    screen = NULL;
    if ((screen=SDL_SetVideoMode(winSizeX, winSizeY, info->vfmt->BitsPerPixel, SDL_SWSURFACE)) == 0) {
	fprintf(stderr,"ERROR: Video mode set failed.\n");
	// return 1;
    }
 
    ctx = OSMesaCreateContextExt( OSMESA_RGBA, 16, 0, 0, NULL );
    if (!ctx) {
	printf("OSMesaCreateContext failed!\n");
	exit(1);
    }

   
    
    frameBuffer = calloc(winSizeX * winSizeY * 4, sizeof(GLfloat));
    if (!OSMesaMakeCurrent(ctx, frameBuffer, GL_FLOAT, winSizeX, winSizeY)) {
	printf("OSMesaMakeCurrent failed!\n");
	exit(1);
    }
    
    OSMesaPixelStore(OSMESA_Y_UP, 0);
	return OK;
}

int ContextGL_X11::get_window_width() {
	
	return winSizeX;
}
int ContextGL_X11::get_window_height() {

	
	return winSizeY;

}


ContextGL_X11::ContextGL_X11(const OS::VideoMode& p_default_video_mode,bool p_opengl_3_context) {

	default_video_mode=p_default_video_mode;
	// x11_display=p_x11_display;
	
	opengl_3_context=p_opengl_3_context;
	
	double_buffer=false;
	direct_render=false;
	glx_minor=glx_major=0;
	// p = memnew( ContextGL_X11_Private );
	// p->glx_context=0;
}


ContextGL_X11::~ContextGL_X11() {

	// memdelete( p );
}


#endif
#endif
