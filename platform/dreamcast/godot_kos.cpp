/*************************************************************************/
/*  godot_server.cpp                                                     */
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
#include "main/main.h"
#include "os_kos.h"
#include <GL/glkos.h>
#include <kos.h>

unsigned char romdisk_data = 0;

int main(int argc, char* argv[]) {
	glKosInit();
	OS_KOS os;
#if 0
	file_t fd;
    file_t d;
    dirent_t *de;
    int amt;

    printf("Reading directory from CD-Rom:\r\n");

    /* Read and print the root directory */
    d = fs_open("/cd", O_RDONLY | O_DIR);

    if(d < 0) {
        printf("Can't open root!\r\n");

    }

    while((de = fs_readdir(d))) {
        printf("%s  /  ", de->name);

        if(de->size >= 0) {
            printf("%d\r\n", de->size);
        }
        else {
            printf("DIR\r\n");
        }
    }

    fs_close(d);
#endif
	char* args[] = {"-path", "/cd/"};
	
	Error err  = Main::setup("kos",2, args);
	if (err!=OK)
		return 255;
		
	if (Main::start()) {
        printf("Running\n");
		os.run(); // it is actually the OS that decides how to run
    }
	Main::cleanup();
	
	return 0;
}
