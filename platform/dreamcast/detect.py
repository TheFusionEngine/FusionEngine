
import os
import sys	


def is_active():
	return True
        
def get_name():
	return "Dreamcast"


def can_build():

	if (os.name!="posix"):
		return False

	return True # enabled
  
def get_opts():

	return [
	('use_llvm','Use llvm compiler','no'),
	('force_32_bits','Force 32 bits binary','yes')
	]
  
def get_flags():

	return [
	('builtin_zlib', 'no'),
	('theora','no'), #use builtin openssl
	('tools', 'no'),
	('disable_3d', 'no'),
	('disable_advanced_gui', 'yes'),
	('webp', 'no'),
	('vorbis', 'no'),
	('speex', 'no'),
	('dds', 'no'),
	('pvr', 'no'),
	('jpg', 'no'),
	('squish', 'no'),
	('minizip', 'no'),
	('openssl', 'no'),
	('musepack', 'no'),
	
	]
			


def configure(env):

	env.Append(CPPPATH=['#platform/dreamcast'])

	env["CC"]="kos-cc"
	env["CXX"]="kos-c++"
	env["LD"]="kos-c++"
	env["AR"]="sh-elf-gcc-ar"
	env["RANLIB"]="sh-elf-gcc-ranlib"



	env["bits"]="32"


	#if (env["tools"]=="no"):
	#	#no tools suffix
	#	env['OBJSUFFIX'] = ".nt"+env['OBJSUFFIX']
	#	env['LIBSUFFIX'] = ".nt"+env['LIBSUFFIX']
	
	# env.Append(CCFLAGS=['-fno-builtin'])
	
	kos_path = os.environ["KOS_BASE"]
	kos_arch = os.environ["KOS_ARCH"]
	
	env.Append(CPPPATH=[kos_path+"/../sh-elf/sh-elf/include/"])
	env.Append(LIBPATH=[kos_path+"/../sh-elf/sh-elf/lib", kos_path+"/lib/"+kos_arch])


	if (env["target"]=="release"):

		env.Append(CCFLAGS=['-Os','-ffast-math','-fomit-frame-pointer'])

	elif (env["target"]=="release_debug"):

		env.Append(CCFLAGS=['-O2','-ffast-math','-DDEBUG_ENABLED'])

	elif (env["target"]=="debug"):

		env.Append(CCFLAGS=['-g2', '-Wall','-DDEBUG_ENABLED','-DDEBUG_MEMORY_ENABLED'])

	env.Append(CPPFLAGS=['-DSERVER_ENABLED', '-DBACKEND_KOSPVR', '-DDREAMCAST', '-DNDEBUG', '-D__DREAMCAST__', '-D__arch_dreamcast', '-D_arch_dreamcast', '-D_arch_sub_pristine', '-DKOS', '-DNEED_LONG_INT', '-DLIBC_FILEIO_ENABLED', '-DNO_STATVFS', '-fno-operator-names'])
	#env.Append(LDFLAGS=['-Wl,--start-group', '-Wl,--end-group'])
	env.Append(LIBS=['m','z','GLdc', 'kallisti']) #TODO detect linux/BSD!


