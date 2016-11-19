/******************************************************************************
 * File: globals.h
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description:
 * 		This file contains variables used across multiple files to store
 * 		and track position, current settings, as well as definitions.
 ******************************************************************************/
#ifndef __GLOBALS_H_
#define __GLOBALS_H_

// C Libraries
#include <cstdlib>
#include <cmath>
#include <ctime>

// C++ Libraries
#include <iostream>
#include <algorithm>
#include <string>
#include <map>

// Steve Jobs made me
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

using namespace std;

// Definitions
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#define nullImage (-1)

/**Flags**/
bool infoFlag = true;

// Time Flags
bool pauseFlag = false;
bool stepFlag = false;

// Toggle Flags
bool orbitFlag = true;
bool lightFlag = true;
bool solidFlag = false;
bool smoothFlag = false;
bool texFlag = false;
bool bodyLabelFlag = true;
bool moonLabelFlag = false;
bool velocityFlag = false;

// Screen size
int width = 1200;
int height = 720;

// Time Progression Speed
float hourSpeed = 0.1;

// Positional Variables
float xTranslate = 0.0;
float yTranslate = 0.0;
float zTranslate = -250;
float xVelocity = 0;
float yVelocity = 0;
float zVelocity = 0;
float xRotate = 0.0;
float yRotate = 50.0;
float mouseX, mouseY;

// Body Viewing From
string relative = "Sun";

// Texture Map Storage
GLuint texNames[32];

// Current draw mode
enum Mode {wire, flat, smooth, image};
int drawMode = 1;

#endif