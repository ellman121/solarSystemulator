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

#endif
