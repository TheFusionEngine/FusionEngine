#include "pspmath.h"

// simple ease in/out function
// input must be 0 < t < 1

float vfpu_ease_in_out(float t) {
	float r;
	__asm__ volatile (
		"mtv     %1, S000\n"
		"vmov.s  S000[0:1], S000\n"			// clamp S000 to 0..1 range
		"vmul.s  S000, S000, S000[2]\n"		// raise S000 to 0..2 range
		"vsub.s  S000, S000, S000[1]\n"		// S000 now in -1 to 1 range
		"vsin.s  S000, S000\n"				// S000 = sin(S000)
		"vmul.s  S000, S000, S000[1/2]\n"	// S000 = sin(t)/2
		"vadd.s  S000, S000, S000[1/2]\n"	// S000 = 0.5 + sin(t)/2
		"mfv     %0, S000\n"
	: "=r"(r) : "r"(t));
	return r;
}
