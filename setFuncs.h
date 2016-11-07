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
	    GLfloat mat_ambient[] = { color[0], color[1], color[2], 1.0 };
	    GLfloat mat_diffuse[] = { color[0], color[1], color[2], 1.0 };
	    GLfloat mat_specular[] = { color[0], color[1], color[2], 1.0 };
	    GLfloat mat_shininess = { 100.0 };
	    
	    // Set material properties
	    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
	    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );	
}
