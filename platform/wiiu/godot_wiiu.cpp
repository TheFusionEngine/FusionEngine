/*************************************************************************/
/*  godot_server.cpp                                                     */
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

#include <whb/proc.h>
#include <whb/gfx.h>

#include "main/main.h"

#include <whb/gfx.h>
#include <whb/log_console.h>

#include <whb/sdcard.h>
#include <sndcore2/core.h>
#include "os_wiiu.h"
// #include "CafeGLSLCompiler.h"
#include <sysapp/launch.h>
int main(int argc, char *argv[]) {
	WHBProcInit();
	// WHBGfxInit();
	WHBLogConsoleInit();
	WHBMountSdCard();
	AXInit();
	// GLSL_Init();
    // ramfsInit();
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		OSReport("ERROR: cannot initialize SDL video.%s\n", SDL_GetError());
	OSReport("Fusion wiiu\n");
	OS_WIIU os;

	OSReport("Fusion wiiu OS init\n");
	char* args[] = {"-path", WHBGetSdCardMountPath()};
	OSReport("setup\n");

	Error err = Main::setup("wiiu", 2, args, true);
    OSReport("setup\n");


	if (err!=OK)
		return 0;
	
	if (Main::start()) {
		OSReport("game running\n");
		os.run(); // it is actually the OS that decides how to run
	}
	Main::cleanup();
	//GLSL_Shutdown();
	
    AXQuit();
	WHBLogConsoleFree();
    WHBProcShutdown();
	SYSLaunchMenu();
	return 0;
}
