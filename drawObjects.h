extern int height, width;

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

void drawOrbit (planet* body){
	glPushMatrix();
	// Get body color and parent name
	float orbitColor[3] = {0.0, 0.0, 1.0};

	string parent = body->getParent();
	float radius = body->getRadius();
	// get distance between body surface and parent surface
	float distance = planetMap.at(parent)->getRadius()+ radius +body->getDistance();

	GLUquadricObj* orbit = gluNewQuadric();

	// Set material properties
	if (lightFlag){
	    setMaterials(orbit, orbitColor, 1, false);
	}
    glDisable(GL_CULL_FACE);

	// Draw a texture mapped top ring
	if (smoothFlag){
		gluQuadricNormals(orbit, GLU_SMOOTH);
	} else {
		gluQuadricNormals(orbit, GLU_FLAT);
	}
	glDisable(GL_TEXTURE_2D);
	gluQuadricTexture(orbit, GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, 5);
	
	if (body->getName() != relative){
		glRotatef( body->getIncline(), 0.0, 0.0, 1.0 );
	}

	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder( orbit, distance, distance+0.1, 0, 100, 1);

	gluDeleteQuadric(orbit);
	glEnable(GL_CULL_FACE);
	glPopMatrix();
}

void drawRings (ring* body){
	glPushMatrix();

	float ringColor[3];
	body->getColor(ringColor);

	// get distance between body surface and start of rings
	float iRadius = body->getInnerRadius();
	float oRadius = body->getOuterRadius();

	GLUquadricObj* ring = gluNewQuadric();

	// Set material properties
	if (lightFlag){
	    setMaterials(ring, ringColor, 1, false);
	}
	if (texFlag){
		setTexture(body);
	}
    glDisable(GL_CULL_FACE);

	// Draw a texture mapped top ring
	if (smoothFlag){
		gluQuadricNormals(ring, GLU_SMOOTH);
	} else {
		gluQuadricNormals(ring, GLU_FLAT);
	}
	if (texFlag){
		gluQuadricTexture(ring, GL_TRUE);
	} else {
		gluQuadricTexture(ring, GL_FALSE);
	}

	gluCylinder( ring, iRadius, oRadius, 0, 100, 1);

	gluDeleteQuadric(ring);
	glEnable(GL_CULL_FACE);
	glPopMatrix();
}

void drawSun(planet* sun){
	glPushMatrix();
	// Get sun color
	float sunColor[3] = {};
	sun->getColor(sunColor);
	float radius = sun->getRadius();
	GLUquadricObj* sphere = gluNewQuadric();

	// Set material properties
	if (lightFlag){
	    setMaterials(sphere, sunColor, sun->getAlbedo(), true);
	}
	if (texFlag){
		setTexture(sun);
	}

	// Rotate about y axis for body rotation
	glRotatef( sun->getRotation(), 0.0, 1.0, 0.0 );

	// rotate about x axis to adjust latitude and longinal lines in wireframe
	glRotatef( 90, 1.0, 0.0, 0.0 );

	// Draw body
    glColor3fv(sunColor);
	(solidFlag) ? gluSphere( sphere, radius, (int) (radius*10), (int) (radius*10) ) : glutWireSphere( radius, 10, 10 );

	// Draw Rings
	drawRings(ringMap.at("Sun"));		

	glPopMatrix();

 	if (bodyLabelFlag){
		glDisable(GL_TEXTURE_2D);
	    drawBodyName("Sun", radius);
 	}

}
void drawBody(planet* body, bool sat){
	glPushMatrix();

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



	// reverse axial tilt before drawing label
	glRotatef( -1 * (body->getTilt()+270), 1.0, 0.0, 0.0 );
	
	// Draw satellite for body
	vector<string> satellites = body->getSatellites();
	for (int s = 0; s < satellites.size(); s++){
		drawBody(moonMap.at(satellites.at(s)), true);
	}
    
    if ((!sat && bodyLabelFlag) || (sat && moonLabelFlag)){
		glDisable(GL_TEXTURE_2D);
		drawBodyName(name, radius);
    }
	glPopMatrix();
    
}
