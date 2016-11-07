#ifndef __PLANET_H_
#define __PLANET_H_

#include <string>
#include "globals.h"
#include <iostream>

using namespace std;

class planet
{
public:
	// Constructors
	planet();
	planet(string name, string parent, float radius, float distance, float daysPerYear, float hoursPerDay, float color[3], Image_s img);
	~planet();

	// Getters
	float getRadius();
	float getDistance();
	float getRotation();
	float getOrbit();
	void getColor(float returned[]);
	Image_s getImage();
	string getName();
	string getParent();

	// Setters
	// bool setRadius(int radius);
	// bool setColor(float newColor[3]);
	// bool setImage(image newImage);
	// bool setParent(planet newParent);
	// bool setDistance(int distance);
	// bool setSpeed(int speed);

	// Action functions
	void step(float speed);

private:
	string _name;			// Name of the planet

	float _radius;			// Radius of the planet
	float _rotation;		// How many degrees the planet spins/hour
	float _rotSpeed;		// How many degrees the planet rotates around its axis/hour

	string _parent;		// The object that this planet orbits
	float _distance;		// Distance from the parent
	float _orbit;			// Current degrees around parent
	float _orbitalSpeed;	// Number of degrees around parent we rotate/hour

	float _color[3];		// Default color of the planet's
	Image_s _img;				// Image data
};

#endif
