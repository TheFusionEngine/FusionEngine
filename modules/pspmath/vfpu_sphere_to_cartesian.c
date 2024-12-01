#include "pspmath.h"

void vfpu_sphere_to_cartesian(float az, float ze, float rad, ScePspFVector4 *result) {
	__asm__ volatile (
		"mtv      %1, S000\n"
		"mtv      %2, S001\n"
		"mtv      %3, S002\n"
		"vcst.s   S003, VFPU_2_PI\n" 							// C000 = [az, ze, rad, 2/pi]
		"vscl.p   C000, C000, S003\n"							// C000 = [az*2/pi, ze*2/pi, rad, 2/pi]
		"vrot.p   C010, S000, [s, c]\n"							// C010 = [sin(az), cos(az), ?, ?]
		"vrot.p   C012, S001, [s, c]\n"							// C010 = [sin(az), cos(az), sin(ze), cos(ze)]
		"vmul.q   C020, C010[0, y, 1, x], C010[0, z, w, z]\n"   // C020 = [0, cos(az)*sin(ez), cos(ze), sin(az)*sin(ze)]
		"vscl.t   C021, C021, S002\n"							// C020 = [0, r*cos(az)*sin(ez), r*cos(ze), r*sin(az)*sin(ze)]
		"sv.q     C020, 0 + %0\n"
	:"=m"(*result):"r"(az), "r"(ze), "r"(rad));
}
