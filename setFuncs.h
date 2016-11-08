extern bool solidFlag;
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


void setMaterials( float color[]){
	    // Set material property values
	    GLfloat mat_ambient[] = { (float)(color[0] * 0.4), (float)(color[1] * 0.4), (float)(color[2] * 0.4), 1.0 };
	    GLfloat mat_diffuse[] = { (float)(color[0] * 0.7), (float)(color[1] * 0.7), (float)(color[2] * 0.7), 1.0 };
	    GLfloat mat_specular[] = { (float)(color[0] * 0.8), (float)(color[1] * 0.8), (float)(color[2] * 0.8), 1.0 };
	    GLfloat mat_shininess = { 75.0 };
	    
	    // Set material properties
	    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
	    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );	
}

void setVelocity(int x, int y, int z){

}