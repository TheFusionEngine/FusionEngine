import os
import sys


def is_active():
    return True


def get_name():
    return "wii"


def can_build():

    if os.name != "posix":
        return False

    return True  # enabled


def get_opts():
    return []


def get_flags():
    return [
        ("builtin_zlib", "no"),
        ("tools", "no"),
        ("theora", "no"),
        ("module_pspmath_enabled", "no"),
    ]


def configure(env):
    env.disabled_modules = ["openssl", "webp"]

    env["bits"] = "32"
    devkitpro_path = os.environ["DEVKITPRO"]
    devkitarm_path = devkitpro_path + "/devkitPPC"
    ogc_path = devkitpro_path + "/libogc"

    env.Append(CPPPATH=["#platform/wii"])
    env["CC"] = devkitarm_path + "/bin/powerpc-eabi-gcc"
    env["CXX"] = devkitarm_path + "/bin/powerpc-eabi-g++"
    env["LD"] = devkitarm_path + "/bin/powerpc-eabi-g++"
    env["AR"] = devkitarm_path + "/bin/powerpc-eabi-ar"
    env["RANLIB"] = devkitarm_path + "/bin/powerpc-eabi-ranlib"
    env["AS"] = devkitarm_path + "/bin/powerpc-eabi-as"

    arch = ["-MMD", "-MP", "-mrvl", "-mcpu=750", "-meabi", "-mhard-float"]
    env.Append(
        CCFLAGS=[
            "-DGEKKO",
            "-DHW_RVL",
            "-D__WII__",
            "-DPOSIX_IP_ENABLED",
            "-DWII_ENABLED",
            "-DGAMECUBE_WII",
            "-DNEED_LONG_INT",
            "-DLIBC_FILEIO_ENABLED",
            "-DNO_SAFE_CAST",
            "-DPTHREAD_NO_RENAME",
            "-DNO_THREADS",
            "-DBIG_ENDIAN_ENABLED",
            "-DGLEW_ENABLED",
            "-DGLES_OVER_GL",
            "-fno-exceptions",
            "-DNO_SAFE_CAST",
            "-fno-rtti",
            "-DNO_FCNTL",
        ]
        + arch
    )

    env.Append(
        CPPPATH=[
            ogc_path + "/include",
            devkitpro_path + "/portlibs/wii/include/",
            devkitpro_path + "/portlibs/ppc/include/",
        ]
    )
    env.Append(
        LIBPATH=[
            ogc_path + "/lib/wii",
            devkitpro_path + "/portlibs/wii/lib/",
            devkitpro_path + "/portlibs/ppc/lib/",
        ]
    )

    env.Append(ENV={"DEVKITPRO": devkitpro_path})

    env.Append(
        LIBS=[
            "opengx",
            "SDLmain",
            "SDL",
            "aesnd",
            "fat",
            "ogc",
            "wiiuse",
            "wiikeyboard",
            "m",
            "bte",
        ]
    )
    env.Append(LIBS=["png", "z"])
    env.Append(LINKFLAGS=arch)

    if env["target"] == "release":
        env.Append(CCFLAGS=["-O2", "-ffast-math"])
    elif env["target"] == "release_debug":
        env.Append(CCFLAGS=["-g2", "-O2", "-ffast-math", "-DDEBUG_ENABLED"])
    elif env["target"] == "debug":
        env.Append(
            CCFLAGS=["-O0", "-g2", "-Wall", "-DDEBUG_ENABLED", "-DDEBUG_MEMORY_ENABLED"]
        )
