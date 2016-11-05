#ifndef __GLOBALS_H_
#define __GLOBALS_H_

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
enum Mode {wire, flat, smooth, image};

bool infoFlag;
bool pauseFlag;
int currSpeed;
Mode drawMode; 

float sunColor[3] = {1.0, 1.0, 0.2};
float earthColor[3] = {0.2, 0.2, 1.0};
float marsColor[3] = {1.0, 0.2, 0.2};

#endif
