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
#include "os_psp.h"

#include <psptypes.h>
#include <pspiofilemgr.h>
#include <pspmodulemgr.h>
#include <psppower.h>
#include <pspkernel.h>
#include <pspthreadman.h>
#include <psputils.h>
#include <psputility.h>
#include <pspdisplay.h>
#include <pspsdk.h>
#include <pspctrl.h>
#include <pspwlan.h>
#include <psputility.h>
#include <psputility_netparam.h>
#include <pspgu.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>

PSP_MODULE_INFO("FUSION", PSP_MODULE_USER, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-1);
PSP_HEAP_THRESHOLD_SIZE_KB(1024);

static char disp_list[0x10000] __attribute__((aligned(64)));

void conf(pspUtilityDialogCommon *dialog, size_t dialog_size)
{
    memset(dialog, 0, sizeof(pspUtilityDialogCommon));

    dialog->size = dialog_size;
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &dialog->language); // Prompt language
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &dialog->buttonSwap); // X/O button swap
    dialog->graphicsThread = 0x11;
    dialog->accessThread = 0x13;
    dialog->fontThread = 0x12;
    dialog->soundThread = 0x10;
}



int netDialog()
{
    int ret = 0, done = 0;
    pspUtilityNetconfData data;
    struct pspUtilityNetconfAdhoc adhocparam;

    memset(&adhocparam, 0, sizeof(adhocparam));
    memset(&data, 0, sizeof(pspUtilityNetconfData));

    conf(&data.base, sizeof(pspUtilityNetconfData));
    data.action = PSP_NETCONF_ACTION_CONNECTAP;
    data.hotspot = 0;
    data.adhocparam = &adhocparam;

    if ((ret = sceUtilityNetconfInitStart(&data)) < 0) {
        printf("sceUtilityNetconfInitStart() failed: 0x%08x", ret);
        return ret;
    }
	
    do
    {
		sceGuStart(GU_DIRECT, disp_list);
		sceGuClear(GU_COLOR_BUFFER_BIT);
		sceGuFinish();
		sceGuSync(0, 0);

        done = sceUtilityNetconfGetStatus();
        switch(done) {
            case PSP_UTILITY_DIALOG_VISIBLE:
                if ((ret = sceUtilityNetconfUpdate(1)) < 0) {
                    printf("sceUtilityNetconfUpdate() failed: 0x%08x", ret);
                }
                break;

            case PSP_UTILITY_DIALOG_QUIT:
                if ((ret = sceUtilityNetconfShutdownStart()) < 0) {
                    printf("sceUtilityNetconfShutdownStart() failed: 0x%08x", ret);
                }
                break;
        }

        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
    } while(done != PSP_UTILITY_DIALOG_NONE);

    done = PSP_NET_APCTL_STATE_DISCONNECTED;
    if ((ret = sceNetApctlGetState(&done)) < 0) {
        printf("sceNetApctlGetState() failed: 0x%08x", ret);
        return 0;
    }
	sceKernelDelayThread(100000);
    return (done == PSP_NET_APCTL_STATE_GOT_IP);
}

int main(int argc, char* argv[]) {

	// SetupCallbacks();
    pspSdkDisableFPUExceptions();
    scePowerSetClockFrequency(333, 333, 166);
#ifdef PSP_NET
	//TODO: Make NET stuff optional
	sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
	sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
	
	sceNetInit(128*1024, 42, 4*1024, 42, 4*1024);
	sceNetInetInit();
	sceNetApctlInit(0x8000, 48);
#endif	


	OS_PSP os;
	
	char* args[] = {"-path", "."};

	Error err = Main::setup("psp", 2, args, true);
	if (err!=OK)
		return 255;
#ifdef PSP_NET
	netDialog();
#endif	
	
	if (Main::start()) {

		printf("game running\n");
		os.run(); // it is actually the OS that decides how to run
	}
	Main::cleanup();

	sceKernelExitGame();
	return 0;
}
