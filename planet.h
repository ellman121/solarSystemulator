#ifndef __PLANET_H_
#define __PLANET_H_

#include <string>
#include "globals.h"

using namespace std;

class planet
{
	// Constructors
	planet();
	planet(char *name, int radius, int rotation, float color[3], image img, planet *parent, int distance, int orbitalSpeed, position pos);
	~planet();

	// Getters
	int getRadius();
	void getColor(float returned[]);
	image getImage();
	int getDistance();

	// Setters
	bool setRadius(int radius);
	bool setRotation(int rotation);
	bool setColor(float new[3]);
	bool setImage(image newImage);
	bool setParent(planet newParent);
	bool setDistance(int distance);
	bool setSpeed(int speed);

	// Other functions
	void step();

private:
	string _name;		// Name of the planet
	int _radius;		// Radius of the planet
	int _rotation;		// How many degrees the planet spins/hour
	float _color[3];	// Default color of the planet's
	image _img;			// Image data

	planet* _parent;	// The object that this planet orbits
	int _distance;		// Distance from the parent
	int _orbitalSpeed;	// Angular velocity around parent

	position _position;	// Current position of the planet in space
}

#endif
