extern bool lightFlag, solidFlag, bodyLabelFlag, moonLabelFlag, velocityFlag, pauseFlag;
extern map<string,planet*> planetMap, moonMap;
extern float zTranslate, hourSpeed, xVelocity, yVelocity, zVelocity, xTranslate, yTranslate;
extern int height, width;

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
	// Set material property values
	    GLfloat labelMat[] = { 1.0, 1.0, 1.0, 1.0 };
	    // GLfloat mat_shininess = { 75.0 };
	    
	    // Set material properties
	    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, labelMat );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, labelMat );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, labelMat );
	    // glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );
	}
	glColor3fv(color);
  	glRasterPos3f(0, body->getRadius()+(body->getRadius() * 0.1), 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)name);
}

void drawSun(planet* sun, bool sat){
	// Get body color
	float color[3] = {};
	sun->getColor(color);

	// Set material properties
	if (lightFlag){
	    setMaterials(color);
	    GLfloat mat_emission[] = { (float)(color[0]*0.6), (float)(color[1]*0.6), (float)(color[2]*0.6), 1.0 };
	    
	    // Set material properties
	    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, mat_emission );

	}

	glPushMatrix();
	glColor3fv(color);

	// Draw body
    if (solidFlag) {
        glutSolidSphere( sun->getRadius(), (int) (sun->getRadius()*10), (int) (sun->getRadius()*10) );
    } else {
        glutWireSphere( sun->getRadius(), 10, 10 );
    }
 	if (bodyLabelFlag){
	    drawBodyName(sun); 		
 	}

	glPopMatrix();
}

void drawBody(planet* body, bool sat){
	// Get body color
	float color[3] = {};
	body->getColor(color);
	string parent = body->getParent();
	float distance;
	// float distance2 = planetMap.at("Sun")->getRadius()+ planetMap.at("Earth")->getRadius()+planetMap.at("Earth")->getDistance();
	float distance2 = 0;

	if (body->getName() != "Sun"){
		distance = planetMap.at(parent)->getRadius()+ body->getRadius()+body->getDistance() - distance2;
	} else {
		distance = body->getDistance()-1* distance2;
	}

	// float distance = body->getDistance();

	// Set material properties
	if (lightFlag){
	    setMaterials(color);
	}

	// Position body around sun at correct distance
	glRotatef( body->getIncline(), 0.0, 0.0, 1.0 );
	glRotatef( body->getOrbit(), 0.0, 1.0, 0.0 );

	glTranslatef( distance, 0.0, 0.0 );

	glPushMatrix();
	// Rotate the body on it's axis
	glRotatef( body->getRotation(), 0.0, 1.0, 0.0 );
	glColor3fv(color);
	// Draw body
    if (solidFlag) {
        glutSolidSphere( body->getRadius(), (int) (body->getRadius()*10), (int) (body->getRadius()*10) );
    } else {
        glutWireSphere( body->getRadius(), 10, 10 );
    }
    glPopMatrix();
    if ((!sat && bodyLabelFlag) || (sat && moonLabelFlag)){
		drawBodyName(body);
    }

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
	glMatrixMode (GL_PROJECTION); // Tell opengl that we are doing project matrix work
	glLoadIdentity(); // Clear the matrix
	glOrtho(-1, 1, -1, 1, 0.0, 1); // Setup an Ortho view
	glMatrixMode(GL_MODELVIEW); // Tell opengl that we are doing model matrix work. (drawing)
	glLoadIdentity(); // Clear the model matrix

	char xV [40];
	char yV [40];
	char zV [40];
	char speed[40];
	float color[3] = {0, 1.0, 0};

	if (!pauseFlag){
		sprintf(speed, "Speed:  %f  hours / frame", hourSpeed);

	} else {
		sprintf(speed, "Speed:  PAUSED");

	}
 	glColor3fv(color);

  	glRasterPos3f(-0.97, 0.9, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)speed);
	
  	glRasterPos3f(-0.95, 0.85, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)"Velocity");
  	glRasterPos3f(-0.98, 0.83, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)"<--------->");
	if (velocityFlag){

		sprintf(xV, "X:  %d", (int)(-1 * xVelocity));
		sprintf(yV, "Y:  %d", (int)(-1 * yVelocity));
		sprintf(zV, "Z:  %d", (int)(zVelocity));
	

	  	glRasterPos3f(-0.95, 0.8, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)xV);

	  	glRasterPos3f(-0.95, 0.75, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)yV);

	  	glRasterPos3f(-0.95, 0.7, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)zV);

	} else {
	  	glRasterPos3f(-0.95, 0.8, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)"OFF");	
	}
}