#ifndef __GLOBALS_H_
#define __GLOBALS_H_

// Image structure
struct image {
	int nRows;
	int nCols;
	unsigned char *img;
}

// Position
struct position {
	int x;
	int y;
	int z;
}
enum Mode {wire, flat, smooth, image};

bool infoFlag;
bool pauseFlag;
int currSpeed;
Mode drawMode; 


#endif
