import os
import sys


def is_active():
    return True


def get_name():
    return "Symbian"


def can_build():
    if os.name != "posix":
        return False

    epocroot = os.getenv("EPOCROOT")
    if not epocroot or not os.path.isdir(epocroot):
        return False

    return True


def get_opts():
    return [
        ('signkey', 'Signing key', '../signing.key'),
        ('signcert', 'Signing certificate', '../signing.cer'),
    ]


def get_flags():
    return [
        ('theora', 'no'),
    ]


def configure(env):
    env.Append(CPPPATH=["#platform/symbian"])

    env["CC"] = "arm-none-symbianelf-gcc"
    env["CXX"] = "arm-none-symbianelf-g++"
    env["LD"] = "arm-none-symbianelf-g++"

    env.Append(CPPFLAGS=[
        '-D__SERIES60_3X__',
        '-DSYMBIAN_S60V3_ENABLED',
        '-DGLES2_ENABLED',
        '-DNO_THREADS',
    ])
    env['symbiandef'] = [
        '-DUNIX_SOCKET_UNAVAILABLE',
        '-DNO_NETWORK',
        '-DNO_SYSLOG',
        '-DSYMBIAN_ENABLED',
        '-DUNIX_ENABLED',
        '-DNO_STATVFS',
        # Symbian
        '-D_POSIX_C_SOURCE=200112',
        '-D__ARM_ARCH_5__',
        '-DNDEBUG',
        '-D_UNICODE',
        '-D__SYMBIAN32__',
        '-D__SERIES60_31__',
        '-D__GCCE__',
        '-D__EPOC32__',
        '-D__MARM__',
        '-D__EABI__',
        '-D__MARM_ARMV5__',
        '-D__MARM_THUMB__',
        '-D__MARM_INTERWORK__',
        '-D__EXE__',
        '-D__SUPPORT_CPP_EXCEPTIONS__',
        '-D__MARM_ARMV5__',
    ]
    env['symbianinc'] = [
        '-isystem', os.path.join(os.path.dirname(os.path.realpath(__file__)), 'sympathy'),
        '-isystem', os.path.join(os.getenv('EPOCROOT'), 'epoc32/include/platform/mw'),
        '-isystem', os.path.join(os.getenv('EPOCROOT'), 'epoc32/include/platform'),
        '-isystem', os.path.join(os.getenv('EPOCROOT'), 'epoc32/include/mw'),
        '-isystem', os.path.join(os.getenv('EPOCROOT'), 'epoc32/include/stdapis'),
        '-isystem', os.path.join(os.getenv('EPOCROOT'), 'epoc32/include/stdapis/stlportv5'),
        '-isystem', os.path.join(os.getenv('EPOCROOT'), 'epoc32/include'),
    ]
    env.Append(CPPFLAGS=[
        *env['symbiandef'],
        '-D__PRODUCT_INCLUDE__=\\"' + os.path.join(os.getenv('EPOCROOT'), 'epoc32/include/variant/symbian_os.hrh') + '\\"',
        *env['symbianinc'],
        '-include', os.path.join(os.getenv('EPOCROOT'), 'epoc32/include/gcce/gcce.h'),
        #'-include', os.path.join(os.getenv('EPOCROOT'), 'epoc32/include/symbian_os.hrh'),
        #'-fomit-frame-pointer',
        '-fvisibility=hidden',
        '-fvisibility-inlines-hidden',
        #'-fdata-sections',
        #'-ffunction-sections',
        '-fpermissive',
        '-Wno-error=narrowing',
        '-msoft-float',
        '-mthumb',
        '-mapcs',
        '-mthumb-interwork',
        '-march=armv5t',
        '-fno-optimize-sibling-calls',
        '-fno-unit-at-a-time',
        '-fno-threadsafe-statics',
    ])

    env['uroot'] = os.path.join(os.getenv('EPOCROOT'), 'epoc32/release/armv5/udeb')
    env['libroot'] = os.path.join(os.getenv('EPOCROOT'), 'epoc32/release/armv5/lib')

    env.Append(CCFLAGS=[
        '-std=gnu++14'
    ])
    env.Append(CFLAGS=[
        '-fno-common',
    ])
    env.Append(LINKFLAGS=[
        '-fwhole-program',
        '-Wl,--default-symver',
        '-Wl,--fatal-warnings',
        '-Wl,--no-relax',
        '-Wl,--no-undefined',
        '-Wl,--target1-abs',
        '-Wl,--demangle',
        '-Wl,--pic-veneer',
        #'-Wl,--gc-sections',
        '-Wl,-Map=bin/godot.exe.map',
        '-Ttext',
        '0x8000',
        '-Tdata',
        '0xe00000',
        '--entry',
        '_E32Startup',
        '-u',
        '_E32Startup',
        '-nostdlib',
        '-msoft-float',
        '-mthumb',
        '-mapcs',
        '-mthumb-interwork',
        '-fno-optimize-sibling-calls',
        '-march=armv5t',
        #'-shared',
        '-L' + os.path.join(os.path.dirname(os.path.dirname(env['CXX'])), 'lib/gcc/arm-none-symbianelf/12.1.0'),
        '-L' + os.path.join(os.path.dirname(os.path.dirname(env['CXX'])), 'arm-none-symbianelf/lib'),
        '-L' + env['libroot'],
        '-L' + env['uroot'],
    ])
    env.Append(LIBS=[
        ':eexe.lib',
        #'-Wl,--start-group',
        ':usrt2_2.lib',
        ':libcrt0.lib',
        #'-Wl,--end-group',
        ':stdnew.dso',
        ':libstdcppv5.dso',
        ':libc.dso',
        ':libm.dso',
        ':libpthread.dso',
        ':euser.dso',
        ':dfpaeabi.dso',
        ':dfprvct2_2.dso',
        ':drtaeabi.dso',
        ':scppnwdl.dso',
        ':drtrvct2_2.dso',
        ':librt.dso',
        ':libdl.dso',
        ':bafl.dso',
        ':estor.dso',
        ':eikcore.dso',
        ':apparc.dso',
        ':avkon.dso',
        ':cone.dso',
        ':hal.dso',
        ':libGLES_CM.dso',
        ':libGLESv2.dso',
        ':libegl.dso',
        ':ws32.dso',
        ':gdi.dso',
        ':mediaclientaudiostream.dso',
        ':inetprotutil.dso',
        ':etext.dso',
        ':efsrv.dso',
        'supc++',
        'gcc',
    ])

    if env["bits"] == "default":
        env["bits"] = "32"

    if env["target"] == "release":
        env.Append(CCFLAGS=["-O2", "-ffast-math", "-fomit-frame-pointer"])
    elif env["target"] == "release_debug":
        env.Append(CCFLAGS=["-O2", "-ffast-math", "-DDEBUG_ENABLED"])
    elif env["target"] == "debug":
        env.Append(CCFLAGS=["-g2", "-Wall", "-DDEBUG_ENABLED", "-DDEBUG_MEMORY_ENABLED"])

    env.Append(CPPFLAGS=["-DSYMBIAN_ENABLED", "-DUNIX_ENABLED"])
    env.Append(LIBS=["pthread", "z"])
	
    import methods
    env.Append( BUILDERS = { 'GLSL120' : env.Builder(action = methods.build_legacygl_headers, suffix = 'glsl.h',src_suffix = '.glsl') } )
    env.Append( BUILDERS = { 'GLSL' : env.Builder(action = methods.build_glsl_headers, suffix = 'glsl.h',src_suffix = '.glsl') } )
    env.Append( BUILDERS = { 'GLSL120GLES' : env.Builder(action = methods.build_gles2_headers, suffix = 'glsl.h',src_suffix = '.glsl') } )
