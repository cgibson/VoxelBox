import platform

lib_target = "VoxelBox"
test_target = "testLib"

env = Environment(CC = 'g++')
env.Append(CPPPATH=['include'])
conf = Configure(env)

env.Append(CCFLAGS=['-g'])
#env.Append(CCFLAGS=['-g','-O3'])

Export('env')

##############################################

sources = Glob('src/*.cpp')


env.SharedLibrary(target = lib_target, source = sources, LIBPATH='.')

##############################################

objects = []

objects = objects + SConscript('test/SConscript')

env.Program(test_target, objects, LIBS=['VoxelBox'], LIBPATH='.')
