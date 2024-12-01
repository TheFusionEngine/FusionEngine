#ifndef _pspmath_h_
#define _pspmath_h_

#include <psptypes.h>

typedef struct quat {
	float x, y, z, w;
} __attribute__((aligned(16))) ScePspQuatMatrix;

void printMatrixFloat(int matid);
void vfpu_srand(unsigned int x);
float vfpu_randf(float min, float max);
unsigned int vfpu_rand_8888(int min, int max);
void vfpu_identity_matrix(ScePspFMatrix4 *m);
void vfpu_translate_matrix(ScePspFMatrix4 *m, float x, float y, float z);
void vfpu_perspective_matrix(ScePspFMatrix4 *m, float fovy, float aspect, float near, float far);
void vfpu_ortho_matrix(ScePspFMatrix4 *m, float left, float right, float bottom, float top, float near, float far);
float vfpu_sinf(float rad);
float vfpu_cosf(float rad);
float vfpu_tanf(float x);
float vfpu_asinf(float x);
float vfpu_acosf(float x);
float vfpu_atanf(float x);
float vfpu_atan2f(float x, float y);
float vfpu_sinhf(float x);
float vfpu_coshf(float x);
float vfpu_tanhf(float x);
void vfpu_sincos(float r, float *s, float *c);
float vfpu_expf(float x);
float vfpu_logf(float x);
float vfpu_fabsf(float x);
float vfpu_sqrtf(float x);
float vfpu_powf(float x, float y);
float vfpu_fmodf(float x, float y);
float vfpu_fminf(float x, float y);
float vfpu_fmaxf(float x, float y);
float vfpu_ease_in_out(float t);
void vfpu_normalize_vector(ScePspFVector4 *v);
void vfpu_sphere_to_cartesian(float az, float ze, float rad, ScePspFVector4 *result);
void vfpu_quaternion_identity(ScePspQuatMatrix *q);
void vfpu_quaternion_copy(ScePspQuatMatrix *dst, ScePspQuatMatrix *src);
void vfpu_quaternion_normalize(ScePspQuatMatrix *res);
void vfpu_quaternion_multiply(ScePspQuatMatrix *qout, ScePspQuatMatrix *a, ScePspQuatMatrix *b);
void vfpu_quaternion_from_euler(ScePspQuatMatrix *qout, float rx, float ry, float rz);
void vfpu_quaternion_exp(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);
void vfpu_quaternion_ln(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);
void vfpu_quaternion_sample_linear(ScePspQuatMatrix *qout, ScePspQuatMatrix *a, ScePspQuatMatrix *b, float t);
void vfpu_quaternion_to_matrix(ScePspQuatMatrix *q, ScePspFMatrix4 *m);

#endif
