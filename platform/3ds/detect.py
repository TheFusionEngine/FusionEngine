
import os
import sys	


def is_active():
	return True


def get_name():
	return "Server"


def can_build():
	if (os.name!="posix"):
		return False

	return True # enabled


def get_opts():
	return []


def get_flags():
	return [
		('builtin_zlib', 'no'),
		('theora','no'), #use builtin openssl
	]

def build_shader_gen(target, source, env, for_signature):
    return "picasso -o {} {}".format(target[0], source[0])


def build_shader_header(target, source, env):
    import os
    data = source[0].get_contents()
    data_str = ",".join([str(x) for x in data])
    name = os.path.basename(str(target[0]))[:-2]
    target[0].prepare()
    with open(str(target[0]), 'w') as f:
        f.write("/* Auto-generated from {} */\n".format(str(source[0])))
        f.write("static uint8_t shader_builtin_{}[] =\n{{{}}};".format(
            name, data_str))


def configure(env):

	env.Append(CPPPATH=['#platform/3ds'])

	devkit_path = os.environ["DEVKITPRO"]
	dev_path = os.environ["DEVKITARM"]

	
	env.Append(BUILDERS={'PICA': env.Builder(generator=build_shader_gen, suffix='.shbin', src_suffix='.pica')})
	env.Append(BUILDERS={'PICA_HEADER': env.Builder(action=build_shader_header, suffix='.h', src_suffix='.shbin')})


	env["CC"]=dev_path+"/bin/arm-none-eabi-gcc"
	env["CXX"]=dev_path+"/bin/arm-none-eabi-g++"
	env["LD"]=dev_path+"/bin/arm-none-eabi-g++"
	env["AR"]=dev_path+"/bin/arm-none-eabi-ar"
	env["AS"]=dev_path+"/bin/arm-none-eabi-as"
	env["RANLIB"]=dev_path+"/bin/arm-none-eabi-ranlib"

	env["bits"]="32"


	
	arch = ['-march=armv6k', '-mtune=mpcore','-mfloat-abi=hard','-mtp=soft' ]
	
	env.Append(CPPPATH=[dev_path+"/arm-none-eabi/include/", devkit_path+"/portlibs/3ds/include/", devkit_path+"/libctru/include/"])
	env.Append(LIBPATH=[dev_path+"/arm-none-eabi/lib/armv6k/fpu/", devkit_path+"/portlibs/3ds/lib/", devkit_path+"/libctru/lib/"])
	


	if env["target"]=="release":
		env.Append(CCFLAGS=['-mword-relocations','-ffunction-sections', '-fno-rtti', '-fno-exceptions']+arch)
	elif env["target"]=="release_debug":
		env.Append(CCFLAGS=['-O2','-ffast-math','-DDEBUG_ENABLED'])
	elif env["target"]=="debug":
		env.Append(CCFLAGS=['-g2', '-Wall','-DDEBUG_ENABLED','-DDEBUG_MEMORY_ENABLED'])

	env.Append(CCFLAGS=['-D__3DS__','-DNEED_LONG_INT', '-D_3DS_ENABLED', '-DNO_THREADS','-DNO_SAFE_CAST'])
	env.Append(LIBS=['pthread', 'z', 'citro3d', 'ctru'])
	env.Append(LINKFLAGS=['-specs=3dsx.specs', '-g'] + arch)
	
	if (env["CXX"]=="clang++"):
		env.Append(CPPFLAGS=['-DTYPED_METHOD_BIND'])
		env["CC"]="clang"
		env["LD"]="clang++"

