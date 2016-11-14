extern bool solidFlag, pauseFlag, velocityFlag, moonLabelFlag, bodyLabelFlag, lightFlag, texFlag, smoothFlag;
extern map<string,planet*> planetMap, moonMap;
extern float hourSpeed, xVelocity, yVelocity, zVelocity, xTranslate, yTranslate, zTranslate, xRotate, yRotate;
extern string relative;

void setTexImage (){
    int numR, numC;
    unsigned char *image;
    for (auto& p: planetMap){
        if (p.second->getImage().img == NULL) {

            if(LoadBmpFile(p.second->getName(), numR, numC, image)) {
                // Image_s texImage = {numR, numC, image};
                p.second->setImage({numR, numC, image});
                cout << "YAY!" << endl;
            } else {
                // Image_s texImage = {0, 0, nullptr};
                p.second->setImage({0, 0, nullptr});
                cout << "BOO!" << endl;
            }
            // p.second->setImage({0, 0, nullptr});

        }
    }

}

void setDrawMode(Mode mode){
    switch (mode) {
        case wire:
            solidFlag = false;
            smoothFlag = false;
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        break;

        case flat:
            solidFlag = true;
            smoothFlag = false;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glShadeModel( GL_FLAT );
        break;

        case smooth:
            solidFlag = true;
            smoothFlag = true;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glShadeModel( GL_SMOOTH );
        break;

        case image:
            smoothFlag = true;
            texFlag = true;
            setTexImage();
        break;
    }
}


void setMaterials(GLUquadric* quad, float color[], float albedo, bool emiss){
        gluQuadricDrawStyle( quad, GLU_FILL );
        // set normals for object based on draw mode
        if (solidFlag) {
            if (smoothFlag){
                gluQuadricNormals(quad, GLU_SMOOTH);
            } else {
                gluQuadricNormals(quad, GLU_FLAT);
            }

        } else {
            gluQuadricNormals(quad, GLU_NONE);
        }

        // set texture coordinates based  texture flag
        if (texFlag){
            gluQuadricTexture(quad, GLU_TRUE);
        } else {
            gluQuadricTexture(quad, GLU_FALSE);
        }

	    // Set material property values
	    GLfloat ambient[] = { (float)(color[0] * 0.6), (float)(color[1] * 0.6), (float)(color[2] * 0.6), 1.0 };
	    GLfloat diffuse[] = { (float)(color[0]), (float)(color[1]), (float)(color[2]), 1.0 };
	    GLfloat specular[] = { (float)(color[0]), (float)(color[1]), (float)(color[2]), 1.0 };
	    GLfloat shininess = { float(100 * albedo) };
	    GLfloat emission[4];
	    // Set material properties
	    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambient );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular );
	    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shininess );
        // Set emissivity, if emiss flag is true, emissivity to body colors, else set to zeros
        if (emiss) {
            GLfloat emission[] = { (float)(color[0]), (float)(color[1]), (float)(color[2]), 1.0 }; 
            glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emission );
        } else {
            GLfloat emission[] = { 0.0, 0.0, 0.0, 1.0};
            glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emission );
        }
}

void setVelocity(int x, int y, int z){
}


void setNextFrame(){
    // Move bodies if not paused
    if (!pauseFlag){
        // Move planets ahead by hour speed
        for (auto& p: planetMap)
            p.second->step(hourSpeed);
        // Move satellites ahead by hour speed
        for (auto& m: moonMap)
            m.second->step(hourSpeed);
    }
    // increment x,y,z by corresponding velocity values
    if (velocityFlag){
        xTranslate += xVelocity;
        yTranslate += yVelocity;
        zTranslate += zVelocity;
    }
}

void resetView(){
    xRotate = 0.0;
    yRotate = 0.0;
    xTranslate = 0.0;
    zTranslate = -200.0;
}


void setRelativeBody(string name){
    relative = name;
    xTranslate = 0;
    yTranslate = 0;
    zTranslate = -200;
}

void setSpeedSelectSubmenuOption(int option) {
    switch (option) {
    case 0: // Minimum speed
        hourSpeed = 0.1;
    break;

    case 1: // 1 Hour/Step
        hourSpeed = 1;
    break;

    case 2: // 12 Hour/Step
        hourSpeed = 12;
    break;

    case 3: // 1 Day/Step
        hourSpeed = 24;
    break;

    case 4: // 2 Day/Step
        hourSpeed = 48;
    break;
    }

    glutPostRedisplay();
}

void setPlanetSelectSubmenuOption(int option) {
    switch (option) {
    // Planets in order
    // "Marys Virgin Explanation Made Joseph 
    // Suspect Upstairs Neighbour"
    // 
    // Source: xkcd.com/992
    case 0:
        setRelativeBody("Sun");
    break;

    case 1:
        setRelativeBody("Mercury");
    break;

    case 2:
        setRelativeBody("Venus");
    break;

    case 3:
        setRelativeBody("Earth");
    break;

    case 4:
        setRelativeBody("Mars");
    break;

    case 5:
        setRelativeBody("Jupiter");
    break;

    case 6:
        setRelativeBody("Saturn");
    break;

    case 7:
        setRelativeBody("Uranus");
    break;

    case 8:
        setRelativeBody("Neptune");
    break;
    }

    glutPostRedisplay();
}

void setMenuOption(int option) {
    switch (option) {
    case 0: // Toggle lighting
        lightFlag = !lightFlag;
        (lightFlag) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
    break;

    case 1: // Toggle planatary labels
        bodyLabelFlag = !bodyLabelFlag;
    break;

    case 2: // Toggle moon labels
        moonLabelFlag = !moonLabelFlag;
    break;

    case 3: // Reset position
        resetView();
    break;

    case 4: // Info Screen
    break;

    case 5: // Exit
        exit(0);
    break;
    }

    glutPostRedisplay();
}
