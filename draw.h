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
void drawSun(planet* sun){
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


	glPopMatrix();
}

void drawPlanet(planet* body){
	// Get body color
	float color[3] = {};
	body->getColor(color);


	// Set material properties
	if (lightFlag){
	    setMaterials(color);
	}

	// Position body around sun at correct distance
	glRotatef( body->getOrbit(), 0.0, 1.0, 0.0 );
	glTranslatef( body->getDistance(), 0.0, 0.0 );

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

	// Move back to starting position
	glTranslatef( -1 * body->getDistance(), 0.0, 0.0 );
	glRotatef( -1 * body->getOrbit(), 0.0, 1.0, 0.0 );
}
void drawPlanets(){
	// Draw each planet
	for (auto& p: planetMap){
		if (p.second->getName() != "Sun"){
			drawPlanet(p.second);
		}
	}
	glFlush();
	// glutSwapBuffers();
	// glutPostRedisplay();
}


void drawSatellite(planet* body){
	// Get body color
	float color[3] = {};
	body->getColor(color);
	cout << color[0] << ", " << color[1] << ", " << color[2] << endl;
	string parent = body->getParent();
	cout << body->getName() << ":  " <<  body->getOrbit() << "   " << body->getDistance() << endl;
	cout << parent << ":  " <<  planetMap.at(parent)->getOrbit() << "   " << planetMap.at(parent)->getDistance() << endl;
	
	// Set material properties
	if (lightFlag){
	    setMaterials(color);
	}

	// Position parent around sun at correct distance
	glRotatef( planetMap.at(parent)->getOrbit(), 0.0, 1.0, 0.0 );
	glTranslatef( planetMap.at(parent)->getDistance(), 0.0, 0.0 );
	
	// Position satellite around parent at correct distance
	glRotatef( body->getOrbit(), 0.0, 1.0, 0.0 );
	glTranslatef( body->getDistance(), 0.0, 0.0 );
	
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

	// Move back to starting position
	glTranslatef( -1 * body->getDistance(), 0.0, 0.0 );
	glRotatef( -1 * body->getOrbit(), 0.0, 1.0, 0.0 );
	glTranslatef( -1 * planetMap.at(parent)->getDistance(), 0.0, 0.0 );
	glRotatef( -1 * planetMap.at(parent)->getOrbit(), 0.0, 1.0, 0.0 );
	glFlush();
}

void drawSatellites(){

	// Draw satelite bodies
	for (auto& m: moonMap){
		drawSatellite(m.second);
	}

	glFlush();
	// glutSwapBuffers();
	// glutPostRedisplay();
}

void drawBodyName(planet *body){
	const char* name = body->getName().c_str();
	float color[3] = {1.0, 1.0, 1.0};
	if (lightFlag){
	    setMaterials(color);
	}
	// Position body around sun at correct distance
	glRotatef( body->getOrbit(), 0.0, 1.0, 0.0 );
	glTranslatef( body->getDistance(), 0.0, 0.0 );

  	glRasterPos3f(0, body->getRadius()+0.5, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)name);
	// Move back to starting position
	glTranslatef( -1 * body->getDistance(), 0.0, 0.0 );
	glRotatef( -1 * body->getOrbit(), 0.0, 1.0, 0.0 );
}

void drawSatelliteName(planet *body){
	const char* name = body->getName().c_str();
	string parent = body->getParent();

	//set text color to white
	float color[3] = {1.0, 1.0, 1.0};
	if (lightFlag){
	    setMaterials(color);
	}
	
	// Position parent around sun at correct distance
	glRotatef( planetMap.at(parent)->getOrbit(), 0.0, 1.0, 0.0 );
	glTranslatef( planetMap.at(parent)->getDistance(), 0.0, 0.0 );
	
	// Position satellite around parent at correct distance
	glRotatef( body->getOrbit(), 0.0, 1.0, 0.0 );
	glTranslatef( body->getDistance(), 0.0, 0.0 );

  	glRasterPos3f(0, body->getRadius()+0.5, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)name);

	// Move back to starting position
	glTranslatef( -1 * body->getDistance(), 0.0, 0.0 );
	glRotatef( -1 * body->getOrbit(), 0.0, 1.0, 0.0 );
	glTranslatef( -1 * planetMap.at(parent)->getDistance(), 0.0, 0.0 );
	glRotatef( -1 * planetMap.at(parent)->getOrbit(), 0.0, 1.0, 0.0 );
}
void drawNames(){
	// Draw each planet
	for (auto& p: planetMap){

		drawBodyName(p.second);
	}

	// Draw satelite names
	for (auto& m: moonMap){
		drawSatelliteName(m.second);
	}
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();	
}