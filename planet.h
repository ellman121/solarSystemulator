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

	// Setters

	// Other functions

private:
	int radius;		// Radius of the planet
	int spinDeg;	// How many degrees the planet spins/hour
	float color[];	// Default color of the planet's
	image img;		// Image data

	planet* parent;	// The object that this planet orbits
	int distance;	// Distance from the parent
}

#endif
