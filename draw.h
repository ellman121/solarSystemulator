extern bool lightFlag, solidFlag;
extern map<string,planet*> planetMap, moonMap;

void drawLighSource (){
	    GLfloat light_position[] = { 0.0, 1.0, 0.0, 1.0 };
	    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };       // ambient light
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
	    setMaterials(color);
	}

  	glRasterPos3f(0, body->getRadius()+0.5, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)name);
}

void drawSun(planet* sun, bool sat){
	// Get body color
	float color[3] = {};
	sun->getColor(color);

	// Set material properties
	if (lightFlag){
	    setMaterials(color);
	}

	glPushMatrix();
	glColor3fv(color);

	// Draw body
    if (solidFlag) {
        glutSolidSphere( sun->getRadius(), 10, (int) (sun->getRadius()*50) );
    } else {
        glutWireSphere( sun->getRadius(), 10, 10 );
    }
    drawBodyName(sun);

	glPopMatrix();
}

// void transform(float yRotate, float xTranslate, float xRotate){

// }
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
        glutSolidSphere( body->getRadius(), 10, (int) (body->getRadius()*50) );
    } else {
        glutWireSphere( body->getRadius(), 10, 10 );
    }
    glPopMatrix();
    if (!sat){
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
