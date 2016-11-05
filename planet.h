#ifndef __PLANET_H_
#define __PLANET_H_

#include "globals.h"

using namespace std;

class planataryObject
{
	// Constructors
	planataryObject();
	~planataryObject();

	// Getters
	int getRadius();
	void getColor(float returned[]);
	image getImage();
	int getDistance();

	// Setters
	bool setRadius(int rad);
	bool setSpin(int degrees);
	bool setColor(float new[3]);
	bool setImage(image newImage);
	bool setParent(planet newParent);
	bool setDistance(int distance);

	// Other functions
	void runHour();

private:
	int radius;		// Radius of the planet
	int spinDeg;	// How many degrees the planet spins/hour
	float color[];	// Default color of the planet's
	image img;		// Image data

	planet* parent;	// The object that this planet orbits
	int distance;	// Distance from the parent

	position pos;	// Current position of the planet in space
}

#endif
