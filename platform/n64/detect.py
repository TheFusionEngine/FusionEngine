
import os
import sys	


def is_active():
	return True
        
def get_name():
	return "Ultra"


def can_build():

	if (os.name!="posix"):
		return False

	return True # enabled
  
def get_opts():

	return [
	('use_llvm','Use llvm compiler','no'),
	('force_32_bits','Force 32 bits binary','no')
	]
  
def get_flags():

	return [
	('builtin_zlib', 'yes'),
	('theora','no'), #use builtin openssl
	('webp', 'no')
	]
			


def configure(env):

	env.Append(CPPPATH=['#platform/n64'])
	#if (env["use_llvm"]=="yes"):
	gcc_path = os.environ["N64_INST"] + "/bin/"
	env["CC"]=gcc_path+"mips64-elf-gcc"
	env["CXX"]=gcc_path+"mips64-elf-g++"
	env["LD"]=gcc_path+"mips64-elf-g++"


	env["bits"]="32"


	#if (env["tools"]=="no"):
	#	#no tools suffix
	#	env['OBJSUFFIX'] = ".nt"+env['OBJSUFFIX']
	#	env['LIBSUFFIX'] = ".nt"+env['LIBSUFFIX']


	if (env["target"]=="release"):

		env.Append(CCFLAGS=['-O2','-Os','-ffast-math','-fomit-frame-pointer'])

	elif (env["target"]=="release_debug"):

		env.Append(CCFLAGS=['-O2','-ffast-math','-DDEBUG_ENABLED'])

	elif (env["target"]=="debug"):

		env.Append(CCFLAGS=['-g2', '-Wall','-DDEBUG_ENABLED','-DDEBUG_MEMORY_ENABLED'])

	env.Append(CPPFLAGS=['-DNEED_LONG_INT', '-fno-exceptions', '-DNO_SAFE_CAST', '-fno-rtti','-DULTRA', '-DNO_THREADS'])
	env.Append(LIBS=['c', 'stdc++', 'dragon','m', 'dragonsys']) #TODO detect linux/BSD!
	env.Append(LINKFLAGS=['-Tn64.ld', ''])
	if (env["CXX"]=="clang++"):
		env.Append(CPPFLAGS=['-DTYPED_METHOD_BIND'])
		env["CC"]="clang"
		env["LD"]="clang++"

