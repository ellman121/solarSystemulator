/******************************************************************************
 * File: planet.cpp
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description: This file contains class declerations for the ring class
 *
 ******************************************************************************/

#ifndef __RING_H_
#define __RING_H_

#include <string>
#include <vector>
#include "globals.h"
#include <iostream>

using namespace std;

class ring
{
public:
	// Constructors
	ring(string group, string planet, float iRadius, float oRadius, float incline, float hoursPerDay, float albedo, float color[3], int img);
	~ring();

	// Getters
	float getInnerRadius();
	float getOuterRadius();
	float getIncline();
	float getAlbedo();
	float getRotation();

	void getColor(float returned[]);
	int getImage();
	string getGroup();
	string getPlanet();
	// Setter functions
	void setImage (int image);
	// Action functions
	void step(float speed);

private:
	string _group;			// Name of the ring
	string _planet;			// The object that this ring orbits

	float _innerRadius;		// Radius of the ring
	float _outerRadius;		// Radius of the ring
	float _rotation;		// How many degrees the ring spins/hour
	float _rotSpeed;		// How many degrees the ring rotates around its axis/hour

	float _incline;			// The angle of the orbit in the Y plane
	float _albedo;			// The reflectivity coefficient of the body

	float _color[3];		// Default color of the ring's
	int _img;			// Image data
};

#endif
