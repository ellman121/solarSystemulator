extern bool lightFlag, solidFlag, bodyLabelFlag, moonLabelFlag, velocityFlag, pauseFlag;
extern map<string,planet*> planetMap, moonMap;
extern map<string, ring*> ringMap;
extern float zTranslate, hourSpeed, xVelocity, yVelocity, zVelocity, xTranslate, yTranslate;
extern int height, width;
extern string relative;

void drawLighSource (){
	    GLfloat position[] = { 0.0, 0.0, 0.0, 1.0 };
	    GLfloat ambient[] = { 0.05, 0.05, 0.05, 1.0 };       // ambient light
	    GLfloat diffuse[] = { 0.7, 0.7, 0.7, 1.0 };       // diffuse light
	    GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };      // highlights
	    GLfloat	attenuation[] = {0.0, 1.0, 1.0};
	    // Set up solar system light source
	    glEnable( GL_LIGHT0 );
	    glLightModelfv(  GL_LIGHT_MODEL_AMBIENT, ambient );
	    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	    glLightfv( GL_LIGHT0, GL_POSITION, position );
	    glLightfv( GL_LIGHT0,  GL_QUADRATIC_ATTENUATION, attenuation );
	    // Eliminate hidden surfaces
	    glEnable(GL_BLEND);
	    glEnable( GL_DEPTH_TEST );
	    glEnable( GL_NORMALIZE );
	    glEnable( GL_CULL_FACE );
	    glCullFace( GL_BACK );
}


void drawBodyName(string sName, float radius){
	const char* name = sName.c_str();
	if (lightFlag){
		// Set material property values to max
	    GLfloat labelMat[] = { 1.0, 1.0, 1.0, 1.0 };
	    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, labelMat );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, labelMat );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, labelMat );
	}

	glColor3f(1.0, 1.0, 1.0);

	// Position label above body and draw
  	glRasterPos3f(0, radius+(radius * 0.1), 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)name);
}

void drawSun(planet* sun, bool sat){
	// Get sun color
	float color[3] = {};
	sun->getColor(color);
	float radius = sun->getRadius();
	GLUquadricObj* sphere = gluNewQuadric();

	// Set material properties
	if (lightFlag){
	    setMaterials(sphere, color, sun->getAlbedo(), true);
	}
	if (texFlag){
		setTexture(sun);
	}
	glPushMatrix();

	// Rotate about y axis for body rotation
	glRotatef( sun->getRotation(), 0.0, 1.0, 0.0 );

	// rotate about x axis to adjust latitude and longinal lines in wireframe
	glRotatef( 90, 1.0, 0.0, 0.0 );

	// Draw body
    glColor3fv(color);
	(solidFlag) ? gluSphere( sphere, radius, (int) (radius*10), (int) (radius*10) ) : glutWireSphere( radius, 10, 10 );

	// reverse axial tilt before drawing label
	glRotatef( -90, 1.0, 0.0, 0.0 );
 	if (bodyLabelFlag){
	    drawBodyName("Sun", radius);
 	}

 	// Reverse body rotation
 	glRotatef( -1 * sun->getOrbit(), 0.0, 1.0, 0.0 );

	glPopMatrix();
}
void drawRings (ring* body){

	float color[3];
	body->getColor(color);

	// get distance between body surface and start of rings
	float iRadius = body->getInnerRadius();
	float oRadius = body->getOuterRadius();

	GLUquadricObj* ring = gluNewQuadric();

	// Set material properties
	if (lightFlag){
	    setMaterials(ring, color, 1, false);
	}

	Image_s texImg = body->getImage();
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texImg.nCols, texImg.nRows, GL_RGB, GL_UNSIGNED_BYTE, texImg.img);
    glDisable(GL_CULL_FACE);

	// Draw a texture mapped top ring
	gluQuadricNormals(ring, GLU_SMOOTH);
	gluQuadricTexture(ring, GL_TRUE);

	gluCylinder( ring, iRadius, oRadius, 0, 100, 1);

	gluDeleteQuadric(ring);
}

void drawBody(planet* body, bool sat){
	// Get body color and parent name
	float color[3] = {};
	body->getColor(color);
	string name = body->getName();
	string parent = body->getParent();
	float radius = body->getRadius();

	GLUquadricObj* sphere = gluNewQuadric();
	// get distance between body surface and parent surface
	float distance = planetMap.at(parent)->getRadius()+ radius +body->getDistance();
	// Set material properties
	if (lightFlag){
	    setMaterials(sphere, color, body->getAlbedo(), false);
	}

	// make sure system is not being viewed relative to current body
	// roate about the z axis for orbital incline
	if (name != relative){
		glRotatef( body->getIncline(), 0.0, 0.0, 1.0 );
	}

	// rotate around y axis to get orbital position
	glRotatef( body->getOrbit(), 0.0, 1.0, 0.0 );
	// translate distance in x plane  to place body
	glTranslatef( distance, 0.0, 0.0 );

	glPushMatrix();

	// Rotate about y axis for body rotation
	glRotatef( body->getRotation(), 0.0, 1.0, 0.0 );

	if (texFlag){
		setTexture(body);
	}

	glColor3fv(color);
	// rotate about x axis for body axial tilt
	glRotatef( body->getTilt()+270, 1.0, 0.0, 0.0 );

	// Draw body
	(solidFlag) ? gluSphere( sphere, radius, (int) (radius*10), (int) (radius*10) ) : glutWireSphere( radius, 10, 10 );

	// Draw any rings around the body
	vector<string> rings = body->getRings();
	for (int r = 0; r < rings.size(); r++){
		drawRings(ringMap.at(rings.at(r)));
	}

	glDisable(GL_TEXTURE_2D);

	// reverse axial tilt before drawing label
	glRotatef( -1 * (body->getTilt()+270), 1.0, 0.0, 0.0 );
    if ((!sat && bodyLabelFlag) || (sat && moonLabelFlag)){
		drawBodyName(name, radius);
    }

    glPopMatrix();

    // Draw satellite for body
	vector<string> satellites = body->getSatellites();
	for (int s = 0; s < satellites.size(); s++){
		drawBody(moonMap.at(satellites.at(s)), true);
	}

	// Move back to starting position
	glTranslatef( -1 * distance, 0.0, 0.0 );
	glRotatef( -1 * body->getOrbit(), 0.0, 1.0, 0.0 );
	glRotatef( -1 * body->getIncline(), 0.0, 0.0, 1.0 );

}

void drawStatus(){

	//Set ortho view
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	// change to ortho view for 2D drawing
	glOrtho(-1, 1, -1, 1, 0.0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// create variables
	char xV [40];
	char yV [40];
	char zV [40];
	char speed[40];
	float color[3] = {1.0, 1.0, 1.0};

	// set speed text
	(pauseFlag) ? sprintf(speed, "Speed:  PAUSED") : sprintf(speed, "Speed:  %f  hours / frame", hourSpeed);

 	glColor3fv(color);

  	glRasterPos3f(-0.97, 0.9, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)speed);

  	glRasterPos3f(-0.95, 0.85, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)"Velocity");

  	glRasterPos3f(-0.98, 0.83, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)"<--------->");

	// Print velocities if velocity is engaged
	if (velocityFlag){
		// set velocity strings
		sprintf(xV, "X:  %d", (int)(-1 * xVelocity));
		sprintf(yV, "Y:  %d", (int)(-1 * yVelocity));
		sprintf(zV, "Z:  %d", (int)(zVelocity));

		// draw strings
	  	glRasterPos3f(-0.95, 0.8, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)xV);

	  	glRasterPos3f(-0.95, 0.75, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)yV);

	  	glRasterPos3f(-0.95, 0.7, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)zV);

	} else {
		// Let user know that velocity is off
	  	glRasterPos3f(-0.95, 0.8, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)"OFF");
	}
}