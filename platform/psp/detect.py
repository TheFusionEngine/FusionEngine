
import os
import sys	


def is_active():
	return True


def get_name():
	return "psp"


def can_build():
	if (os.name!="posix"):
		return False

	return True # enabled


def get_opts():
		return [
		('compile_prx', 'Compile a prx executable for use with PSPLink', 'no')
	]


def get_flags():
	return [
		('builtin_zlib', 'no'),
		('theora','no'),
		('module_pspmath_enabled', 'yes'),
	]


def configure(env):

	env.Append(CPPPATH=['#platform/psp'])

	env["CC"]="psp-gcc"
	env["CXX"]="psp-g++"
	env["LD"]="psp-g++"

	env["bits"]="32"

	psp_path = os.environ["PSPDEV"]

	env.Append(CPPPATH=[psp_path+"/psp/sdk/include/"])
	env.Append(LIBPATH=[psp_path+"/psp/sdk/lib"])

	if env["target"]=="release":
		env.Append(CCFLAGS=['-Os','-ffast-math','-fomit-frame-pointer'])
	elif env["target"]=="release_debug":
		env.Append(CCFLAGS=['-Os','-ffast-math','-DDEBUG_ENABLED'])
	elif env["target"]=="debug":
		env.Append(CCFLAGS=['-g2', '-Wall','-DDEBUG_ENABLED','-DDEBUG_MEMORY_ENABLED'])


	env.Append(CPPFLAGS=['-DNEED_LONG_INT', '-DPSP_ENABLED', '-fno-exceptions', '-DNO_SAFE_CAST', '-fno-rtti'])
	if env['compile_prx'] == 'yes':
		env.Append(LDFLAGS=['-specs='+psp_path+'/psp/sdk/lib/prxspecs', '-Wl', '-q', '-T'+psp_path+'psp/sdk/lib/linkfile.prx', psp_path +"/psp/sdk/lib/prxexports.o" , '-nostartfiles', "-zmax-page-size=128"])
	env.Append(LIBS=['pthread', 'z', 'pspdisplay', 'pspge', 'pspgu', 'pspgum_vfpu', 'pspvfpu', 'pspctrl', 'psppower', 'pspaudio', 'pspnet', 'pspnet_apctl', 'pspdebug'])


	if (env["CXX"]=="clang++"):
		env.Append(CPPFLAGS=['-DTYPED_METHOD_BIND'])
		env["CC"]="clang"
		env["LD"]="clang++"

