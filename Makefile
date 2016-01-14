all:
	g++ src/*.cpp src/*.c -Iinclude -lGL -lGLEW -lglfw -lfreeimage -lnoise -o theperfecterday --std=c++11 -ggdb
