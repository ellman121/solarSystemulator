# Makefile for TankGame

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = gcc
CXX = g++
LINK = g++

# Turn on optimization and warnings (add -g for debugging with gdb):
CFLAGS = -O -Wall
CXXFLAGS = -O -Wall

# OpenGL libraries for Linux:
GL_LIBS = -lglut -lGLU -lGL -lm

#-----------------------------------------------------------------------

default: orbit.cpp planet.cpp globals.h
	g++ -o solar orbit.cpp planet.cpp -lglut -lGLU -lGL -lm -std=c++11

osx: orbit.cpp planet.cpp globals.h
	g++ -o solar orbit.cpp planet.cpp -framework OpenGL -framework GLUT -framework Cocoa -std=c++11

# utility targets
clean:
	@rm -f *.o *~ core
	@rm solar
