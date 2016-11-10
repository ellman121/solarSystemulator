extern bool solidFlag, pauseFlag, velocityFlag;
extern map<string,planet*> planetMap, moonMap;
extern float hourSpeed, xVelocity, yVelocity, zVelocity, xTranslate, yTranslate, zTranslate;
extern string relative;

void setDrawMode(Mode mode)
{
    switch (mode) {
        case wire:
            solidFlag = false;
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        break;

        case flat:
            solidFlag = true;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glShadeModel( GL_FLAT );
        break;

        case smooth:
            solidFlag = true;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glShadeModel( GL_SMOOTH );
        break;

        case image:
            int numR, numC;
            unsigned char *image;
            if(LoadBmpFile("Sun", numR, numC, image))
                cout << "YAY" << endl;
            delete image;
            cin >> numR;
        break;
    }
}


void setMaterials( float color[], float albedo, bool emiss){
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

void setRelativeBody(string name){
    relative = name;
    xTranslate = 0;
    yTranslate = 0;
    zTranslate = -200;

}