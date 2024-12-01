/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * Copyright (c) 2007 Steve Galatis
 * Copyright (c) 2007 Christophe Avoinme
 *
 * This sample illustrates how to use several of the vfpu math functions.
 *
 * In particular:
 *		how to use vfpu_sphere_to_cartesian to build a static 3d starfield
 *		how to use quaternions for camera control
 *		how to use quaternion ln, exp and linear sampling to interpolate between 2 quaternions
 *		how to use the random number generator on the vfpu, and how to seed it
 */

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <psprtc.h>
#include <pspgu.h>
#include <psputils.h>
#include <pspmath.h>

PSP_MODULE_INFO("pspmath_demo", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU);

typedef struct {
	unsigned int c;
	float x, y, z;
} CVertex;

u32 __attribute__((aligned(16))) list[1024];
CVertex __attribute__((aligned(16))) stars[1000];
ScePspFMatrix4 projection3d, view, model;
ScePspQuatMatrix qcam, qa, qb;
int running = 1;

int exitCallback(int arg1, int arg2, void *common)
{
	running = 0;
	return 0;
}

int callbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int setupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("callbackThread", callbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
		sceKernelStartThread(thid, 0, 0);
	return thid;
}

void initGU() {
	sceGuInit();
	sceGuStart(GU_DIRECT,list);
	sceGuDrawBuffer(GU_PSM_8888,(void *)0,512);
	sceGuDispBuffer(480,272,(void*)0x88000,512);
	sceGuOffset(2048 - (480/2),2048 - (272/2));
	sceGuViewport(2048,2048,480,272);
	sceGuScissor(0,0,480,272);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuShadeModel(GU_SMOOTH);
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDisable(GU_CULL_FACE);
	sceGuDisable(GU_LIGHTING);
	sceGuDisable(GU_TEXTURE_2D);
	sceGuFinish();
	sceGuSync(0,0);
	sceDisplayWaitVblankStart();
	sceGuDisplay(1);
}

int main(int argc, char *argv[]) {
	u64 tick;
	int n;
	SceCtrlData pad;
	float t;

	// vfpu has a random number generator,
	// and it needs to be seeded

	sceRtcGetCurrentTick(&tick);
	vfpu_srand((u32)tick / 10000.0f);

	setupCallbacks();
	initGU();

	vfpu_perspective_matrix(&projection3d, 60.0f, 480.0f/272.0f, 0.5f, 1000.0f);
	vfpu_identity_matrix(&view);
	vfpu_identity_matrix(&model);

	// build a random starfield, using sphere to cartesian coordinate conversions
	// note how we use vfpu_randf here to generate random radian angles,
	// and how we use vfpu_rand_8888 to generate random colors for the stars

	for (n=0; n<1000; n++) {
		vfpu_sphere_to_cartesian(vfpu_randf(0.0f, 6.28319f), vfpu_randf(0.0f, 3.14159f), 1000.0f, (ScePspFVector4 *)&(stars[n]));
		stars[n].c = vfpu_rand_8888(160, 255);
	}

	n = 0;

	// start off with 2 random rotations for the interpolation
	vfpu_quaternion_from_euler(&qa, vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f));
	vfpu_quaternion_ln(&qa, &qa);
	vfpu_quaternion_from_euler(&qb, vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f));
	vfpu_quaternion_ln(&qb, &qb);

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	while (running) {

		n++;

		// when we reach the limit of our interpolation:
		//		copy qb to qa
		//		generate a new random quaternion in qb
		//		take the log of quaternion qb
		//		reset the time counter

		if (n >= 300) {
			n = 0;
			vfpu_quaternion_copy(&qa, &qb);
			vfpu_quaternion_from_euler(&qb, vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f));
			vfpu_quaternion_ln(&qb, &qb);
		}

		// to build the view matrix:
		//		get the interpolated quaternion in qcam
		//		take the exp of qcam
		//		build a rotation matrix from qcam, place into view matrix
		//
		// here we use the function .5 + (cos(t) * .5) for our 'time' variable
		// this gives us the smooth ease in/ease out motion you see as the demo
		// interpolates between qa and qb
		//
		// this is why we must do 300-n, as cos(0) = 1, cos(PI) = -1, we want
		// the reverse, -1 to 1

		t = n/300.0f;
		vfpu_quaternion_sample_linear(&qcam, &qa, &qb, vfpu_ease_in_out(t));
		vfpu_quaternion_exp(&qcam, &qcam);
		vfpu_quaternion_to_matrix(&qcam, &view);

		sceGuStart(GU_DIRECT, list);
		sceGuClearColor(0);
		sceGuClear(GU_COLOR_BUFFER_BIT|GU_FAST_CLEAR_BIT);

		// load our generated matrix data into the display list

		sceGuSetMatrix(GU_PROJECTION, &projection3d);
		sceGuSetMatrix(GU_VIEW, &view);
		sceGuSetMatrix(GU_MODEL, &model);

		sceGuDrawArray(GU_POINTS, GU_VERTEX_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, 1000, 0, stars);

		sceGuFinish();
		sceGuSync(0,0);
		sceGuSwapBuffers();

		sceCtrlPeekBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_RTRIGGER) {
			sceKernelExitGame();
		}
		sceDisplayWaitVblankStart();
	}

	sceGuTerm();
	sceKernelExitGame();
	return 0;
}
