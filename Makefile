all:
	g++ src/*.cpp -lGL -lGLEW -lglfw -lfreeimage -o theperfecterday --std=c++11 -ggdb
