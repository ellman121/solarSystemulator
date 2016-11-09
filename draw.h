extern bool lightFlag, solidFlag, bodyLabelFlag, moonLabelFlag, velocityFlag, pauseFlag;
extern map<string,planet*> planetMap, moonMap;
extern float zTranslate, hourSpeed, xVelocity, yVelocity, zVelocity, xTranslate, yTranslate;
extern int height, width;
extern string relative;

void drawLighSource (){
	    GLfloat light_position[] = { 0.0, 1.0, 0.0, 1.0 };
	    GLfloat light_ambient[] = { 0.6, 0.6, 1.0, 1.0 };       // ambient light
	    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };       // diffuse light
	    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };      // highlights

	    // Set up solar system light source
	    glEnable( GL_LIGHT0 );
	    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
	    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
	    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
	    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );

	    // Eliminate hidden surfaces
	    glEnable( GL_DEPTH_TEST );
	    glEnable( GL_NORMALIZE );
	    glEnable( GL_CULL_FACE );
	    glCullFace( GL_BACK );
}


void drawBodyName(planet *body){
	const char* name = body->getName().c_str();
	float color[3] = {1.0, 1.0, 1.0};

	if (lightFlag){
		// Set material property values to max
	    GLfloat labelMat[] = { 1.0, 1.0, 1.0, 1.0 };
	    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, labelMat );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, labelMat );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, labelMat );
	}

	glColor3fv(color);
	// Position label above body and draw
  	glRasterPos3f(0, body->getRadius()+(body->getRadius() * 0.1), 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)name);
}

void drawSun(planet* sun, bool sat){
	// Get sun color
	float color[3] = {};
	sun->getColor(color);

	// Set material properties
	if (lightFlag){
	    setMaterials(color, sun->getAlbedo());

	    // Set sun emissivity
	    GLfloat mat_emission[] = { (float)(color[0]*0.6), (float)(color[1]*0.6), (float)(color[2]*0.6), 1.0 };	    
	    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, mat_emission );

	}

	glPushMatrix();

	// rotate about x axis to adjust latitude and longinal lines in wireframe
	glRotatef( 90, 1.0, 0.0, 0.0 );

	glColor3fv(color);
	// Draw body
    if (solidFlag) {
        glutSolidSphere( sun->getRadius(), (int) (sun->getRadius()*10), (int) (sun->getRadius()*10) );
    } else {
        glutWireSphere( sun->getRadius(), 10, 10 );
    }

    // draw label for sun
 	if (bodyLabelFlag){
	    drawBodyName(sun); 		
 	}

	glPopMatrix();
}

void drawBody(planet* body, bool sat){
	// Get body color and parent name
	float color[3] = {};
	body->getColor(color);
	string parent = body->getParent();

	float distance;
	// offset for current planet focus
	float distance2 = 0;

	// get distance between body surface and parent surface
	distance = planetMap.at(parent)->getRadius()+ body->getRadius()+body->getDistance() - distance2;

	// Set material properties
	if (lightFlag){
	    setMaterials(color, body->getAlbedo());
	}

	// roate about the z axis for orbital incline
	if (body->getName() != relative){
		glRotatef( body->getIncline(), 0.0, 0.0, 1.0 );
	}
	// rotate around y axis to get orbital position
	glRotatef( body->getOrbit(), 0.0, 1.0, 0.0 );
	// translate distance in x plane  to place body
	glTranslatef( distance, 0.0, 0.0 );

	glPushMatrix();

	// Rotate about y axis for body rotation
	glRotatef( body->getRotation(), 0.0, 1.0, 0.0 );
	// rotate about x axis for body axial tilt
	glRotatef( body->getTilt()+90, 1.0, 0.0, 0.0 );

	glColor3fv(color);
	// Draw body
    if (solidFlag) {
        glutSolidSphere( body->getRadius(), (int) (body->getRadius()*10), (int) (body->getRadius()*10) );
    } else {
        glutWireSphere( body->getRadius(), 10, 10 );
    }

    glPopMatrix();
    // Draw body labels
    if ((!sat && bodyLabelFlag) || (sat && moonLabelFlag)){
		drawBodyName(body);
    }

    // Draw satellite for body
	vector<string> satellites = body->getSatellites();
	for (int s = 0; s < satellites.size(); s++)
	{	
		drawBody(moonMap.at(satellites.at(s)), true);
	}
	
	// Move back to starting position
	glTranslatef( -1 * distance, 0.0, 0.0 );
	glRotatef( -1 * body->getOrbit(), 0.0, 1.0, 0.0 );
	glRotatef( -1 * body->getIncline(), 0.0, 0.0, 1.0 );
}

void drawBodies(){
	// Draw each planet
	for (auto& p: planetMap){
		if (p.second->getName() != "Sun"){
			drawBody(p.second, false);
		} else {
			drawSun(p.second, false);
		}
	}
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
	float color[3] = {0, 1.0, 0};

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