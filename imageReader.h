/******************************************************************************
 * File: imageReader.h
 *
 * Author: John M. Weiss, Ph.D.
 * Written March 23, 2006.
 * Modified by: Elliot Rarden
 * Modified November 2016
 *
 * Ref: based on code from RgbImage.cc (Buss, 3-D Computer Graphics, 2003).
 *
 * Description:
 * 		Header file to to provide access to functinos to read uncmopressed
 *		bithmap images
 ******************************************************************************/
#ifndef __IMAGE_READER_H__
#define __IMAGE_READER_H__

// Function declerations
bool LoadBmpFile( string nameOfPlanet, int &NumRows, int &NumCols, unsigned char* &ImagePtr );

#endif
