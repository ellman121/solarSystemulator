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

// Current draw mode
enum Mode {wire, flat, smooth, image};

extern bool infoFlag;
extern bool pauseFlag;
extern int currSpeed;
extern Mode drawMode; 

#endif
