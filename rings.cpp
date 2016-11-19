/******************************************************************************
 * File: planet.cpp
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description: This file contains implementation for the ring class. Most
 * functions are getter functions, with only one setter function, setImage(),
 * and one action function, step(), which progresses the associated body
 * through time by a specified amount.
 *
 * This class was neccessary because rather than having a radius and a distance
 * element, a ring object has an inner and and outer radius. Since the planet
 * class uses a different scale value to scale down distance from radius, we
 * were unable to use the planet class to create ring objects as well. In
 * nearly every other respect the ring class is the same as the planet class.
 * The only other difference is that there is no satellite or ring vector
 * associated with the ring class, since rings do not have orbiting bodies.
 *
 ******************************************************************************/

#include "rings.h"

/*
 * Constructors
 ***************/

// Use this constructor
ring::ring(string name, string parent, float iRadius, float oRadius, float incline, float hoursPerDay, float albedo, float color[3], int img) {
	_name = name;
	_parent = parent;
	_innerRadius = iRadius / 1000;			// scale down radius
	_outerRadius = oRadius / 1000;			// scale down radius

	_rotation = 0;							// start at 0 degree rotation position
	if (hoursPerDay == 0) {
		_rotSpeed = 0;
	} else {
		_rotSpeed = 360.0 / hoursPerDay;	// rotational degree change per hour
	}

	_incline = incline;						// the angle in y at which the body orbits
	_albedo = albedo;						// the reflectivity coefficient of the body


	_color[0] = color[0];
	_color[1] = color[1];
	_color[2] = color[2];
	_img = img;
}

/*
 * Destructors
 ***************/
ring::~ring() {}

/*
 * Getters
 ***************/

// Get the inner radius of a ring
float ring::getInnerRadius() {
	return _innerRadius;
}

// Get the outer radius of a ring
float ring::getOuterRadius() {
	return _outerRadius;
}


// The number of degrees around self axis we've rotated
float ring::getRotation() {
	return _rotation;
}

// Get the orbit incline angle
float ring::getIncline() {
	return _incline;
}

// Get the orbit incline angle
float ring::getAlbedo() {
	return _albedo;
}

// Get the color from the ring
void ring::getColor(float *returned) {
	returned[0] = _color[0];
	returned[1] = _color[1];
	returned[2] = _color[2];
}

// Return a pionter to the bitmap image for the ring
int ring::getImage() {
	return _img;
}

// Get the name of the ring group
string ring::getName() {
	return _name;
}

// Get the name of the parent ring
string ring::getParent() {
	return _parent;
}


/*
 * Setters
 ***************/
// sets the image for the ring
void ring::setImage(int image) {
	_img = image;
}

/*
 * Activities
 ***************/

// Simulate the ring's movements in 1 hour
void ring::step(float speed)
{
	_rotation = fmod( _rotation + (_rotSpeed * speed), 360);
}
