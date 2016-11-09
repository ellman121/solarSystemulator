extern bool solidFlag, pauseFlag, velocityFlag;
extern map<string,planet*> planetMap, moonMap;
extern float hourSpeed, xVelocity, yVelocity, zVelocity, xTranslate, yTranslate, zTranslate;

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


void setMaterials( float color[], float albedo){
	    // Set material property values
	    GLfloat mat_ambient[] = { (float)(color[0] * 0.4), (float)(color[1] * 0.4), (float)(color[2] * 0.4), 1.0 };
	    GLfloat mat_diffuse[] = { (float)(color[0] * 0.7), (float)(color[1] * 0.7), (float)(color[2] * 0.7), 1.0 };
	    GLfloat mat_specular[] = { (float)(color[0] * 0.8), (float)(color[1] * 0.8), (float)(color[2] * 0.8), 1.0 };
	    GLfloat mat_shininess = { float(100 * albedo) };
	    
	    // Set material properties
	    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
	    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );	
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