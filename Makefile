all:
	g++ src/*.cpp -lGL -lGLEW -lglfw -lfreeimage -lnoise -o theperfecterday --std=c++11 -ggdb
