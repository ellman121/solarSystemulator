#include "planet.h"
#include <cmath>
#include <iostream>
using namespace std;
/*
 * Constructors
 ***************/

// No-Arg constructor
// Don't use this one
planet::planet() {
	_name = "";
	_radius = 0;
	_rotation =0;

}

// Use this constructor
planet::planet(string name, string parent, float radius, float distance, float daysPerYear, float hoursPerDay, float color[3], Image_s img)
{
	_name = name;
	_parent = parent;
	_radius = radius/1000;								//scale down radius
	_distance = distance/1000000;							//scale down distance
	_rotation = 0;										//start at 0 degree rotation position
	_rotSpeed = 360.0/hoursPerDay;						//rotational degree change per hour
	_orbit = 0;											//start at 0 degree orbit position
	_orbitalSpeed = 360.0/(24.0*daysPerYear);				//orbital degree change per hour
	_color[0] = color[0];
	_color[1] = color[1];
	_color[2] = color[2];
	_img = img;
	// cout << name << " Radius: " << _radius << " Orbit: " << _orbitalSpeed << " Distance: " << _orbitalSpeed << endl;


}

/*
 * Destructors
 ***************/
planet::~planet(){}

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
	return (_parent == "") ? 0 : _distance;
}

// The number of degrees around self axis we've rotated
float planet::getRotation()
{
	return _rotation;
}

// The number of degrees around parent we've rotated
float planet::getOrbit()
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
Image_s planet::getImage()
{
	return _img;
}

// Get the name of the planet
string planet::getName()
{
	return _name;
}

// Get the name of the parent planet
string planet::getParent()
{
	return _parent;
}
/*
 * Activities
 ***************/

// Simulate the planet's movements in 1 hour
void planet::step(float speed)
{
	_rotation = fmod( _rotation + (_rotSpeed * speed), 360);
	_orbit = fmod( _orbit + (_orbitalSpeed * speed), 360);
}
