/******************************************************************************
 * File: drawObjects.h
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description:
 * 		This file is a collection of routines to draw celestial bodies. Planets
 * 		are drawn in drawBody() which is called recursively to also draw any
 * 		moons orbiting the planet.
 ******************************************************************************/

/* drawLighSource()
 *
 * Initialize light source at the center of the solar system
 * (i.e. the sun)
 */
void drawLighSource () {
	// Set lighting parameters
	GLfloat position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.05, 0.05, 0.05, 1.0 };
	GLfloat diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat	attenuation[] = {0.0, 1.0, 1.0};
	// Set up solar system light source
	glEnable( GL_LIGHT0 );
	glLightModelfv(  GL_LIGHT_MODEL_AMBIENT, ambient );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	glLightfv( GL_LIGHT0, GL_POSITION, position );
	glLightfv( GL_LIGHT0,  GL_QUADRATIC_ATTENUATION, attenuation );
}
/* drawName()
 *
 * Draw the name of a celestial body slightly above the outer
 * edge of the object.
 *
 * Parameters:
 *		string sName - The name of the body in question
 *		float radius - The radius of the body in question
 */
void drawName(string sName, float radius) {
	glPushMatrix();
	// Store the name as a char* data type
	const char* name = sName.c_str();
	// Set material property values to max
	GLfloat labelMat[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, labelMat );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, labelMat );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, labelMat );

	// Set color to white
	glColor3f(1.0, 1.0, 1.0);

	// Position label above body and draw
	glRasterPos3f(0, radius + (radius * 0.1), 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)name);

	glPopMatrix();
}

/* drawOrbit()
 *
 * Draws the orbital path for the given celesital body. These
 * do not rener well when drawn as solid objects
 *
 * Input:
 *		planet* body - the body who's orbital path is to be drawn
 */
void drawOrbit (planet* body) {
	glPushMatrix();

	// Get body color and parent name
	float orbitColor[3] = {0.5, 0.4, 1};

	string parent = body->getParent();
	float radius = body->getRadius();
	// get distance between body surface and parent surface
	float distance = planetMap.at(parent)->getRadius() + radius + body->getDistance();

	GLUquadricObj* orbit = gluNewQuadric();

	// Draw a texture mapped top ring
	(smoothFlag) ? gluQuadricNormals(orbit, GLU_SMOOTH) : gluQuadricNormals(orbit, GLU_FLAT);
	(texFlag) ? gluQuadricTexture(orbit, GL_TRUE) : gluQuadricTexture(orbit, GL_FALSE);

	if (body->getName() != relative) {
		glRotatef( body->getIncline(), 0.0, 0.0, 1.0 );
	}

	// Rotate the orbital ring to  lie on x plane
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glColor3fv(orbitColor);
	gluCylinder( orbit, distance - 0.01, distance + 0.01, 0, 100, 4);

	// Cleanup
	gluDeleteQuadric(orbit);
	glPopMatrix();
}

/* drawRings()
 *
 * Draws the rings around a celestial body (e.g. Saturn)
 *
 * Input:
 *		ring* body - the rings to be drawn
 */
void drawRings (ring* body) {
	glPushMatrix();

	float ringColor[3];
	body->getColor(ringColor);

	// Get distance between start and end of rings
	float iRadius = body->getInnerRadius();
	float oRadius = body->getOuterRadius();

	GLUquadricObj* ring = gluNewQuadric();

	// Set material properties
	setMaterials(ring, ringColor, 1, false);

	// Set texture mipmap for object
	if (texFlag) {
		setTexture(body->getImage());
	}

	// Show backface of ring
	glDisable(GL_CULL_FACE);

	(smoothFlag) ? gluQuadricNormals(ring, GLU_SMOOTH) : gluQuadricNormals(ring, GLU_FLAT);
	// Draw a texture mapped to ring
	(texFlag) ? gluQuadricTexture(ring, GL_TRUE) : gluQuadricTexture(ring, GL_FALSE);
	// Rotate about y axis for body rotation
	glRotatef( body->getRotation(), 0.0, 1.0, 0.0 );
	// Draw ring
	glColor3fv(ringColor);
	gluCylinder( ring, iRadius, oRadius, 0, 100, 1);

	// Clean up
	gluDeleteQuadric(ring);
	glPopMatrix();
	glEnable(GL_CULL_FACE);
}

/* drawSun()
 *
 * Draws the sun at (0, 0, 0), as well as the
 * asteroid belt "ring"
 *
 * Input:
 *		planet* sun - pointer to the sun in memory
 */
void drawSun(planet* sun) {
	glPushMatrix();
	// Get sun color
	float sunColor[3] = {};
	sun->getColor(sunColor);
	float radius = sun->getRadius();
	GLUquadricObj* sphere = gluNewQuadric();

	// Set material properties
	setMaterials(sphere, sunColor, sun->getAlbedo(), true);
	// Draw a texture mapped top ring
	(smoothFlag) ? gluQuadricNormals(sphere, GLU_SMOOTH) : gluQuadricNormals(sphere, GLU_FLAT);
	(texFlag) ? gluQuadricTexture(sphere, GL_TRUE) : gluQuadricTexture(sphere, GL_FALSE);

	if (texFlag) {
		setTexture(sun->getImage());
	}

	// Rotate about y axis for body rotation
	glRotatef( sun->getRotation(), 0.0, 1.0, 0.0 );

	// rotate about x axis to adjust latitude and longinal lines in wireframe
	glRotatef( 90, 1.0, 0.0, 0.0 );

	// Draw body
	glColor3fv(sunColor);
	(solidFlag) ? gluSphere( sphere, radius, (int) (radius * 100), (int) (radius * 100) ) : glutWireSphere( radius, 10, 10 );

	// Draw Rings
	drawRings(ringMap.at("AsteroidBelt"));

	glPopMatrix();

	if (bodyLabelFlag) {
		glDisable(GL_TEXTURE_2D);
		drawName("Sun", radius);
	}
}

/* drawBody()
 *
 * Draws the given celesital body
 *
 * Input:
 *		planet* body - the body to be drawn
 */
void drawBody(planet* body, bool sat) {
	glPushMatrix();

	// Get body color and parent name
	float color[3] = {};
	body->getColor(color);
	string name = body->getName();
	string parent = body->getParent();
	float radius = body->getRadius();

	GLUquadricObj* sphere = gluNewQuadric();
	// get distance between body surface and parent surface
	float distance = planetMap.at(parent)->getRadius() + radius + body->getDistance();
	// Set material properties
	setMaterials(sphere, color, body->getAlbedo(), false);
	if (texFlag) {
		setTexture(body->getImage());
	}

	// Draw a texture mapped top ring
	(smoothFlag) ? gluQuadricNormals(sphere, GLU_SMOOTH) : gluQuadricNormals(sphere, GLU_FLAT);
	(texFlag) ? gluQuadricTexture(sphere, GL_TRUE) : gluQuadricTexture(sphere, GL_FALSE);

	// make sure system is not being viewed relative to current body
	// roate about the z axis for orbital incline
	if (name != relative) {
		glRotatef( body->getIncline(), 0.0, 0.0, 1.0 );
	}

	// rotate around y axis to get orbital position
	glRotatef( body->getOrbit(), 0.0, 1.0, 0.0 );
	// translate distance in x plane  to place body
	glTranslatef( distance, 0.0, 0.0 );

	// Rotate about y axis for body rotation
	glRotatef( body->getRotation(), 0.0, 1.0, 0.0 );

	// rotate about x axis for body axial tilt
	glRotatef( body->getTilt() + 270, 1.0, 0.0, 0.0 );

	// Draw body
	glColor3fv(color);
	(solidFlag) ? gluSphere( sphere, radius, (int) (radius * 100), (int) (radius * 100) ) : glutWireSphere( radius, 10, 10 );

	// Draw any rings around the body
	vector<string> rings = body->getRings();
	for (int r = 0; r < rings.size(); r++) {
		drawRings(ringMap.at(rings.at(r)));
	}

	// reverse axial tilt before drawing label and satellites
	glRotatef( -1 * (body->getTilt() + 270), 1.0, 0.0, 0.0 );

	// Draw satellites for body
	vector<string> satellites = body->getSatellites();
	for (int s = 0; s < satellites.size(); s++) {
		drawBody(moonMap.at(satellites.at(s)), true);
	}

	// Draw labels
	if ((!sat && bodyLabelFlag) || (sat && moonLabelFlag)) {
		glDisable(GL_TEXTURE_2D);
		drawName(name, radius);
	}
	glPopMatrix();
}
