import os
import sys


def is_active():
    return True


def get_name():
    return "wii"


def can_build():

    if (os.name != "posix"):
        return False

    return True  # enabled


def get_opts():
    return []



def get_flags():

    return [
		('builtin_zlib', 'no'),
		('tools', 'no'),
		('theora','no'),
		('module_pspmath_enabled', 'no'),
	]

def configure(env):
    env.disabled_modules = ['openssl','webp']

    env["bits"] = "32"
    devkitpro_path = os.environ["DEVKITPRO"]
    devkitarm_path = devkitpro_path + "/devkitPPC"
    ogc_path = devkitpro_path + "/libogc"
    
    env.Append(CPPPATH=['#platform/wii'])
    env["CC"] = devkitarm_path + "/bin/powerpc-eabi-gcc"
    env["CXX"] = devkitarm_path + "/bin/powerpc-eabi-g++"
    env["LD"] = devkitarm_path + "/bin/powerpc-eabi-g++"
    env["AR"] = devkitarm_path + "/bin/powerpc-eabi-ar"
    env["RANLIB"] = devkitarm_path + "/bin/powerpc-eabi-ranlib"
    env["AS"] = devkitarm_path + "/bin/powerpc-eabi-as"

    arch = ['-MMD','-MP','-mrvl', '-mcpu=750', '-meabi', '-mhard-float']
    # env.Append(CCFLAGS=['-g','-Wall', '-std=gnu++11'] + arch)
    env.Append(CCFLAGS=[ '-DGEKKO','-DHW_RVL', '-D__WII__', '-DGAMECUBE_WII','-DNEED_LONG_INT', '-DLIBC_FILEIO_ENABLED','-DNO_SAFE_CAST', '-DPTHREAD_NO_RENAME', '-DNO_THREADS'] + arch)
    
    env.Append(CPPPATH=[ogc_path+"/include", devkitpro_path +"/portlibs/wii/include/", devkitpro_path +"/portlibs/ppc/include/"])
    env.Append(LIBPATH=[ogc_path+"/lib/wii", devkitpro_path+"/portlibs/wii/lib/", devkitpro_path+"/portlibs/ppc/lib/"])
    # env.Append(DEVKITPRO=devkitpro_path)
    env.Append(ENV={"DEVKITPRO": devkitpro_path})
    # env.Append(LINKFLAGS=["-specs="+ogc_path+"/share/wut.specs"])
    # env.Append(LIBPATH=[devkitpro_path+"/portlibs/armv6k/lib", devkitpro_path +
               # "/portlibs/3ds/lib", ctrulib_path + "/lib", devkitarm_path + "/arm-none-eabi/lib/armv6k/fpu"])

    # env.Append(LINKFLAGS=['-specs=3dsx.specs', '-g'] + arch)
    env.Append(LIBS=["SDLmain","SDL","opengx","aesnd","fat","ogc","wiiuse","wiikeyboard","m","bte"])
    env.Append(LIBS=["png", "z"])
    env.Append(LINKFLAGS=arch)


    if (env["target"] == "release"):
        #if (env["debug_release"] == "yes"):
        #   env.Append(CCFLAGS=['-g2'])
        #else:
        # env.Append(CCFLAGS=['-Ofast'])
        env.Append(CCFLAGS=['-g'])
    elif (env["target"] == "release_debug"):
        env.Append(CCFLAGS=['-O2', '-ffast-math', '-DDEBUG_ENABLED'])
        #if (env["debug_release"] == "yes"):
        env.Append(CCFLAGS=['-g2'])
    elif (env["target"] == "debug"):
        env.Append(CCFLAGS=['-O3','-g2','-Wall',
                   '-DDEBUG_ENABLED', '-DDEBUG_MEMORY_ENABLED'])
