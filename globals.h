#ifndef __GLOBALS_H_
#define __GLOBALS_H_
#include <map>
#include <string>

using namespace std;

// Image structure
struct Image_s {
	int nRows;
	int nCols;
	unsigned char *img;
};

// Position
struct Position_s {
	float x;
	float y;
	float z;
};

// Current draw mode
enum Mode {wire, flat, smooth, image};

// Global things
bool infoFlag = false;
bool pauseFlag = false;
bool solidFlag = false;
bool lightFlag = true;
float hourSpeed = 1;
int width = 1200;
int height = 720;

float sunColor[3] = {1.0, 1.0, 0.3};
float mercuryColor[3] = {0.8, 0.5, 0.5};
float venusColor[3] = {0.8, 0.5, 0.3};
float earthColor[3] = {0.2, 0.2, 1.0};
float marsColor[3] = {1.0, 0.2, 0.2};
float jupiterColor[3] = {0.8, 0.6, 0.0};
float saturnColor[3] = {0.6, 0.7, 0.0};
float uranusColor[3] = {0.2, 1.0, 0.6};
float neptuneColor[3] = {0.2, 0.4, 0.8};
float xTranslate = 0.0;
float yTranslate = 0.0;
float zTranslate = -100;
float xRotate = 35.0;
float yRotate = 0.0;
float zRotate = 0.0;
float mouseX, mouseY;

#endif
