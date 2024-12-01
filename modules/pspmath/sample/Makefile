TARGET = sample
OBJS = main.o
	
	
PSP_DRIVE = /cygdrive/j
USE_PSPSDK_LIBC = 1

INCDIR =
CFLAGS = -G0 -Wall -O2 -fno-strict-aliasing
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti 
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS= -lpspgu -lpsprtc -lpspctrl -lpspmath

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = pspmath demo
PSP_EBOOT_ICON = icon.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
	