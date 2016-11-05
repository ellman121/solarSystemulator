#ifndef __PLANET_H_
#define __PLANET_H_

#include <string>
#include "globals.h"

using namespace std;

class planet
{
	// Constructors
	planet();
	planet(string name, float radius, float rotationSpeed, planet *planet, float distance, float orbitalSpeed, float color[3], image img);
	~planet();

	// Getters
	float getRadius();
	float getDistance();
	float getRotation();
	float getOrbit();
	void getColor(float returned[]);
	image getImage();

	// Setters
	// bool setRadius(int radius);
	// bool setColor(float newColor[3]);
	// bool setImage(image newImage);
	// bool setParent(planet newParent);
	// bool setDistance(int distance);
	// bool setSpeed(int speed);

	// Action functions
	void step();

private:
	string _name;			// Name of the planet

	float _radius;			// Radius of the planet
	float _rotation;		// How many degrees the planet spins/hour
	float _rotSpeed;		// How many degrees the planet rotates around its axis/hour

	planet* _parent;		// The object that this planet orbits
	float _distance;		// Distance from the parent
	float _orbit;			// Current degrees around parent
	float _orbitalSpeed;	// Number of degrees around parent we rotate/hour

	float _color[3];		// Default color of the planet's
	image _img;				// Image data
};

#endif
