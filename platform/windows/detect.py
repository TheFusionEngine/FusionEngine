

import os

import sys	


def is_active():
	return True
        
def get_name():
	return "Windows"

def can_build():
	return True
		
def get_opts():

	gcc=""
	gcc64=""
	#if (os.name!="nt"):
	#	gcc = "i586-gcc32msvc-"
	#	gcc64 = "i686-w64-gcc32-"

	pdb_available = os.name == "nt"

	if (os.getenv("GCC32_PREFIX")):
		gcc=os.getenv("GCC32_PREFIX")
	if (os.getenv("GCC64_PREFIX")):
		gcc64=os.getenv("GCC64_PREFIX")

	return [
		('gcc_prefix','GCC Prefix',gcc),
		('gcc_prefix_64','GCC Prefix 64 bits',gcc64),
		('gcc64_for_32','Use GCC 64 for 32 Bits Build','no'),
		('compiler', 'Compiler to use (gcc, msvc)', 'gcc'),
		('pdb', 'Generate PDB', 'yes' if pdb_available else 'no'),
	]


def get_flags():

	return [
		('freetype','builtin'), #use builtin freetype
		('openssl','no'), #use builtin openssl
		('theora','no'),
	]


def configure(env):

	env.Append(CPPPATH=['#platform/windows'])

	nulstr=""
	if (os.name=="posix"):
		nulstr=">/dev/null"
	else:
		nulstr=">nul"

	if env['compiler'] == 'msvc':
		#build using visual studio
		env['ENV']['TMP'] = os.environ['TMP']
		env.Append(CPPPATH=['#platform/windows/include'])
		env.Append(LIBPATH=['#platform/windows/lib'])

		if (env["freetype"]!="no"):
			env.Append(CCFLAGS=['/DFREETYPE_ENABLED'])
			env.Append(CPPPATH=['#tools/freetype'])
			env.Append(CPPPATH=['#tools/freetype/freetype/include'])

		if (env["target"]=="release"):

			env.Append(CCFLAGS=['/O2'])
			env.Append(LINKFLAGS=['/SUBSYSTEM:WINDOWS'])
			env.Append(LINKFLAGS=['/ENTRY:mainCRTStartup'])

		elif (env["target"]=="release_debug"):

			env.Append(CCFLAGS=['/O2','/DDEBUG_ENABLED'])
			env.Append(LINKFLAGS=['/SUBSYSTEM:CONSOLE'])

		elif (env["target"]=="debug"):

			env.Append(CCFLAGS=['/Zi','/DDEBUG_ENABLED','/DDEBUG_MEMORY_ENABLED','/DD3D_DEBUG_INFO','/O1'])
			env.Append(LINKFLAGS=['/SUBSYSTEM:CONSOLE'])
			env.Append(LINKFLAGS=['/DEBUG'])


		env.Append(CCFLAGS=['/MT','/Gd','/GR','/nologo'])
		env.Append(CXXFLAGS=['/TP'])
		env.Append(CPPFLAGS=['/DMSVC', '/GR', ])
		env.Append(CCFLAGS=['/I'+os.getenv("WindowsSdkDir")+"/Include"])
		env.Append(CCFLAGS=['/DWINDOWS_ENABLED'])
		env.Append(CCFLAGS=['/DRTAUDIO_ENABLED'])
		env.Append(CCFLAGS=['/DWIN32'])
		env.Append(CCFLAGS=['/DTYPED_METHOD_BIND'])

		env.Append(CCFLAGS=['/DGLES2_ENABLED'])
		env.Append(CCFLAGS=['/DGLES1_ENABLED'])
		env.Append(CCFLAGS=['/DGLEW_ENABLED'])
		LIBS=['winmm','opengl32','dsound','kernel32','ole32','user32','gdi32', 'IPHLPAPI', 'wsock32', 'shell32','advapi32']
		env.Append(LINKFLAGS=[p+env["LIBSUFFIX"] for p in LIBS])
		
		env.Append(LIBPATH=[os.getenv("WindowsSdkDir")+"/Lib"])
		if (os.getenv("DXSDK_DIR")):
			DIRECTX_PATH=os.getenv("DXSDK_DIR")
		else:
			DIRECTX_PATH="C:/Program Files/Microsoft DirectX SDK (March 2009)"

		if (os.getenv("VCINSTALLDIR")):
			VC_PATH=os.getenv("VCINSTALLDIR")
		else:
			VC_PATH=""

		env.Append(CCFLAGS=["/I" + p for p in os.getenv("INCLUDE").split(";")])
		env.Append(LIBPATH=[p for p in os.getenv("LIB").split(";")])
		env.Append(CCFLAGS=["/I"+DIRECTX_PATH+"/Include"])
		env.Append(LIBPATH=[DIRECTX_PATH+"/Lib/x86"])
		env['ENV'] = os.environ;
	elif env['compiler'] == 'gcc':
		env.use_windows_spawn_fix()

		if (os.name=="nt"):
			env['ENV']['TMP'] = os.environ['TMP'] #way to go scons, you can be so stupid sometimes
		else:
			env["PROGSUFFIX"]=env["PROGSUFFIX"]+".exe"

		gcc_prefix=""

		if (env["bits"]=="default"):
			env["bits"]="32"

		use64=False
		if (env["bits"]=="32"):

			if (env["gcc64_for_32"]=="yes"):
				env.Append(CCFLAGS=['-m32'])
				env.Append(LINKFLAGS=['-m32'])
				env.Append(LINKFLAGS=['-static-libgcc'])
				env.Append(LINKFLAGS=['-static-libstdc++'])
				gcc_prefix=env["gcc_prefix_64"];
			else:
				gcc_prefix=env["gcc_prefix"];


		else:
			gcc_prefix=env["gcc_prefix_64"];
			env.Append(LINKFLAGS=['-static'])


		if os.system(gcc_prefix+"gcc --version"+nulstr)!=0:
			#not really super consistent but..
			print("Can't find Windows compiler: "+gcc_prefix)
			sys.exit(255)

		if (env["target"]=="release"):
			
			env.Append(CCFLAGS=['-O2','-ffast-math','-fomit-frame-pointer'])
			env.Append(LINKFLAGS=['-Wl,--subsystem,windows'])

		elif (env["target"]=="release_debug"):

			env.Append(CCFLAGS=['-O2','-ffast-math','-DDEBUG_ENABLED'])

		elif (env["target"]=="debug"):
					
			env.Append(CCFLAGS=['-g', '-Wall','-DDEBUG_ENABLED','-DDEBUG_MEMORY_ENABLED'])

		if (env["freetype"]!="no"):
			env.Append(CCFLAGS=['-DFREETYPE_ENABLED'])
			env.Append(CPPPATH=['#tools/freetype'])
			env.Append(CPPPATH=['#tools/freetype/freetype/include'])

		env["CC"] = gcc_prefix+"gcc.exe"
		env['AS'] = gcc_prefix+"as.exe"
		env['CXX'] = gcc_prefix+"g++.exe"
		env['AR'] = gcc_prefix+"ar.exe"
		env['RANLIB'] = gcc_prefix+"ranlib.exe"
		env['LD'] = gcc_prefix+"g++.exe"

		env.Append(CCFLAGS=['-DWINDOWS_ENABLED', '-DWIN98_ENABLED', '-D_UNICODE', '-DUNICODE', '-mwindows', '-D__MSVCRT_VERSION__=0x400', '-DWINDOWS_USE_MUTEX=1'])
		env.Append(CPPFLAGS=['-DRTAUDIO_ENABLED', '-DWIN98_ENABLED', '-D_UNICODE', '-DUNICODE', '-DMINGW_ENABLED'])
		env.Append(CCFLAGS=['-DGLES1_ENABLED', '-DOPENGL_ENABLED', '-DGLES_OVER_GL', '-DGLEW_ENABLED', '-DMINGW_ENABLED', '-DNO_SAFE_CAST', '-fno-rtti'])
		env.Append(LIBS=['unicows', 'mingw32', 'opengl32', 'dsound', 'ole32', 'winmm', 'gdi32', 'iphlpapi', 'wsock32', 'kernel32', 'comctl32'])

		env.Append(LINKFLAGS=['-Wl,--stack,'+str(16*1024*1024), '-static-libgcc'])

		env.Append(CCFLAGS=['-march=pentium','-mtune=generic'])
		env.Append(CPPFLAGS=['-march=pentium','-mtune=generic'])
		env.Append(LINKFLAGS=['-march=pentium','-mtune=generic'])
		
	# import methods
	# env.Append( BUILDERS = { 'GLSL120' : env.Builder(action = methods.build_legacygl_headers, suffix = 'glsl.h',src_suffix = '.glsl') } )
	# env.Append( BUILDERS = { 'GLSL' : env.Builder(action = methods.build_glsl_headers, suffix = 'glsl.h',src_suffix = '.glsl') } )
	# env.Append( BUILDERS = { 'HLSL9' : env.Builder(action = methods.build_hlsl_dx9_headers, suffix = 'hlsl.h',src_suffix = '.hlsl') } )
	# env.Append( BUILDERS = { 'GLSL120GLES' : env.Builder(action = methods.build_gles2_headers, suffix = 'glsl.h',src_suffix = '.glsl') } )