/******************************************************************************
 * File: planet.cpp
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description: This file contains class declerations for the planet class
 *
 ******************************************************************************/

#ifndef __PLANET_H_
#define __PLANET_H_

#include <string>
#include <vector>
#include <cmath>

using namespace std;

class planet
{
public:
	// Constructors
	planet();
	planet(string name, string parent, float radius, float distance, float daysPerYear, float hoursPerDay, float incline, float tilt, float albedo, float color[3], int img, vector<string> satellites, vector<string> rings);
	~planet();

	// Getters
	float getRadius();
	float getDistance();
	float getRotation();
	float getOrbit();

	float getIncline();
	float getTilt();
	float getAlbedo();

	void getColor(float returned[]);
	int getImage();
	string getName();
	string getParent();
	vector<string> getSatellites();
	vector<string> getRings();

	// Action functions
	void step(float speed);
	// Set texture image
	void setImage(int image);

private:
	string _name;				// Name of the planet
	string _parent;				// The object that this planet orbits
	vector<string> _satellites;	// Vector of the planet's satelitex (i.e. moons)
	vector<string> _rings;		// Vector of the rings this planet contains

	float _radius;				// Radius of the planet
	float _rotation;			// How many degrees the planet has spun around
	float _rotSpeed;			// How many degrees the planet rotates around its axis/hour

	float _distance;			// Distance from the parent
	float _orbit;				// Current degrees around parent
	float _orbitalSpeed;		// Number of degrees around parent we rotate/hour

	float _incline;				// The angle of the orbit in the Y plane
	float _axialTilt;			// The angle of tilt of the axis
	float _albedo;				// The reflectivity coefficient of the body

	float _color[3];			// Default color of the planet's
	int _img;					// Index into image array data
};

#endif
