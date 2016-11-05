#include "planet.h"

/*
 * Constructors
 ***************/

// No-Arg constructor
// Don't use this one
planet::planet() {
	_name = "";
	_radius = 0;
	_rotation = =0;

}

// Use this constructor
planet::planet(string name, float radius, float rotationSpeed, float color[3], image img, planet *planet, float distance, float orbitalSpeed)
{
	_name = name;
	_radius = radius;
	_rotation = 0;
	_rotSpeed = rotationSpeed;
	_color = color;
	_img = img;
	_parent = parent;
	_distance = distance;
	_orbit = 0;
	_orbitalSpeed = orbitalSpeed;
}

/*
 * Destructors
 ***************/
planet::~planet();

/*
 * Getters
 ***************/

// Get the radius of a planet
float planet::getRadius()
{
	return _radius;
}

// The distance from parent planet
float planet::getDistance()
{
	// If we don't have a parent, return 0
	return (_parent == null) ? 0 : _distance;
}

// The number of degrees around self axis we've rotated
float getRotation()
{
	return _rotation;
}

// The number of degrees around parent we've rotated
float getOrbit()
{
	return _orbit;
}

// Get the color from the planet
void planet::getColor(float *returned)
{
	returned[0] = _color[0];
	returned[1] = _color[1];
	returned[2] = _color[2];
}

// Return a pionter to the bitmap image for the planet
image planet::getImage()
{
	return _img;
}

/*
 * Activities
 ***************/

// Simulate the planet's movements in 1 hour
void planet::step()
{
	_rotation += (_rotation + _rotSpeed > 360) : _rotSpeed ? _rotSpeed - 360;
	_orbit += (_orbit + _orbitalSpeed > 360) : _orbitalSpeed ? _orbitalSpeed - 360;

}
