@echo off

set WIN_LIB=-lgdi32 -lwinmm -lshell32
set OPENGL_LIB=-lopengl32
set RAY_LIB=-lraylib

set LIB_ARGS=%RAY_LIB% %WIN_LIB% %OPENGL_LIB%

g++ -c ./src/*.cpp -I ./include
g++ -o ./bin/snake *.o -L ./lib %LIB_ARGS%

del *.o