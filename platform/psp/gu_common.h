#pragma once

#include <psptypes.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspge.h>

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)
#define GU_CMDLIST_SIZE (16*1024)

#define MK_RGBA(r,g,b,a) GU_RGBA((int)(r),(int)(g),(int)(b),(int)(a))
#define MK_RGBA_F(r,g,b,a) GU_RGBA((int)((r)*255),(int)((g)*255),(int)((b)*255),(int)((a)*255))
#define MK_RGBA_C(c) GU_RGBA((int)((c.r)*255),(int)((c.g)*255),(int)((c.b)*255),(int)((c.a)*255))