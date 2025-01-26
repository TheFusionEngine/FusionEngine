
import os
import sys	


def is_active():
	return True


def get_name():
	return "vita"


def can_build():
	if (os.name!="posix"):
		return False

	return True # enabled


def get_opts():
	return []


def get_flags():
	return [
		('builtin_zlib', 'no'),
		('theora','no'),
		('module_pspmath_enabled', 'yes'),
	]


def configure(env):

	env.Append(CPPPATH=['#platform/vita'])

	env["CC"]="arm-vita-eabi-gcc"
	env["CXX"]="arm-vita-eabi-g++"
	env["LD"]="arm-vita-eabi-g++"


	env["bits"]="32"


	vita_path = os.environ["VITASDK"]

	env.Append(CPPPATH=[vita_path+"/arm-vita-eabi/include/"])
	env.Append(LIBPATH=[vita_path+"/arm-vita-eabi/lib"])
	
	env.Append(CCFLAGS=["-Wl,-q"])
	
	
	if env["target"]=="release":
		env.Append(CCFLAGS=['-Os','-ffast-math','-fomit-frame-pointer'])
	elif env["target"]=="release_debug":
		env.Append(CCFLAGS=['-Os','-ffast-math','-DDEBUG_ENABLED'])
	elif env["target"]=="debug":
		env.Append(CCFLAGS=['-g2', '-Wall','-DDEBUG_ENABLED','-DDEBUG_MEMORY_ENABLED'])

	env.Append(CPPFLAGS=['-DNEED_LONG_INT', '-D__psp2__', '-DVITA_ENABLED', '-fno-exceptions', '-DNO_SAFE_CAST', '-fno-rtti']) #'-DGLES2_ENABLED'
	env.Append(LIBS=['z', "SceCtrl_stub", "SceAudio_stub","SceMotion_stub","SceCamera_stub","liblibIMGEGL_stub", "liblibgpu_es4_ext_stub","liblibGLESv1_CM_stub"])
	# env.Append(LIBS=['z', "SceCtrl_stub", "SceAudio_stub","liblibGLESv2_stub","liblibIMGEGL_stub", "liblibgpu_es4_ext_stub"])
	env.Append(LINKFLAGS=["-Wl,-q,-whole-archive", "-lpthread", "-Wl,-q,-no-whole-archive"])

