/******************************************************************************
 * File: planet.cpp
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description: This file is a collection of functions that will set
 * and initialize different elements of the program.
 *
 ******************************************************************************/

/* setNextFrame()
 *
 * This function progresses all system
 * bodies through time.
 */
void setNextFrame() {
    // Move bodies if not paused
    if (!pauseFlag || stepFlag) {
        // Move planets ahead by hour speed
        for (auto& p : planetMap)
            p.second->step(hourSpeed);
        // Move satellites ahead by hour speed
        for (auto& m : moonMap)
            m.second->step(hourSpeed);

        stepFlag = false;
    }
    // increment x,y,z by corresponding velocity values
    if (velocityFlag) {
        xTranslate += xVelocity;
        yTranslate += yVelocity;
        zTranslate += zVelocity;
    }
}

/* setDrawMode()
 *
 * Change the draw mode to the new mode
 *
 * Parameters:
 *      Mode mode - The new mode we are drawing in
 */
void setDrawMode(Mode mode) {
    switch (mode) {
    // Set variables and settings for wireframe drawing
    case wire:
        drawMode = 1;
        solidFlag = false;
        smoothFlag = false;
        texFlag = false;
        break;
    // Set variables and settings for solid, flat shading drawing
    case flat:
        drawMode = 2;
        solidFlag = true;
        smoothFlag = false;
        texFlag = false;
        break;
    // Set variables and settings for solid, smooth shading drawing
    case smooth:
        drawMode = 3;
        solidFlag = true;
        smoothFlag = true;
        texFlag = false;
        break;
    // Set variables and settings for texture mapped drawing
    case image:
        drawMode = 4;
        solidFlag = true;
        smoothFlag = true;
        texFlag = true;
        break;
    }
}

/* setDrawParameters()
 *
 * This function sets draw poperties
 * based on the current draw mode.
 */
void setDrawParameters() {
    switch (drawMode) {
    // Set variables and settings for wireframe drawing
    case 1:
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glDisable(GL_TEXTURE_2D);
        break;
    // Set variables and settings for solid, flat shading drawing
    case 2:
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glShadeModel( GL_FLAT );
        glDisable(GL_TEXTURE_2D);
        break;
    // Set variables and settings for solid, smooth shading drawing
    case 3:
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glShadeModel( GL_SMOOTH );
        glDisable(GL_TEXTURE_2D);
        break;
    // Set variables and settings for texture mapped drawing
    case 4:
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glShadeModel( GL_SMOOTH );
        glEnable(GL_TEXTURE_2D);
        break;
    }
    // Hide future object backfaces
    glEnable(GL_CULL_FACE);

}

/* setView()
 *
 * Set the translate and rotate variables
 * to the default positions.
 */
void setView() {
    // When the sun is relative the view is slightly changed
    if (relative == "Sun") {
        xRotate = 20.0;
        yRotate = 50.0;
        xTranslate = 0.0;
        yTranslate = 0.0;
        zTranslate = -250.0;
    } else {
        xRotate = 1.0;
        yRotate = 0.0;
        xTranslate = 0.0;
        yTranslate = 0.0;
        zTranslate = -205.0;

    }
}
/* setInfoLighting()
 *
 * Set the lighting for the info screen
 */
void setInfoLighting() {
    glEnable(GL_LIGHTING);

    // Set material properties
    GLfloat labelMat[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, labelMat );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, labelMat );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, labelMat );


    // Specify light properties
    GLfloat position[] = { 0.0, 400.0, 0.0, 1.0 };
    GLfloat light[] = { 1.0, 1.0, 1.0, 1.0 };

    // Enable a light source
    glEnable( GL_LIGHT0 );
    glLightfv( GL_LIGHT0, GL_POSITION, light );
    glLightfv( GL_LIGHT0, GL_AMBIENT, light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light );

    // Set shading model
    glShadeModel( GL_FLAT );
    // Enable depth buffer to get rid of hidden surfaces
    glEnable( GL_DEPTH_TEST );
    // Automatic normalize normals
    glEnable( GL_NORMALIZE );
    // Hide back of objects
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
}


/* setMaterials()
 *
 * Set the material properties for a given quadric
 *
 * Parameters:
 *      GLUquadric* quad - The quadric to modify
 *      float color[]    - Array of colors to set the quadric to
 *      float albedo     - Diffuse light value
 *      float emiss      - The emissivity of the quadric
 */
void setMaterials(GLUquadric* quad, float color[], float albedo, bool emiss) {
    gluQuadricDrawStyle( quad, GLU_FILL );
    // set normals for object based on draw mode
    if (solidFlag) {
        if (smoothFlag) {
            gluQuadricNormals(quad, GLU_SMOOTH);
        } else {
            gluQuadricNormals(quad, GLU_FLAT);
        }
    } else {
        gluQuadricNormals(quad, GLU_NONE);
    }

    GLfloat ambient[4] = {0, 0, 0, 1.0};
    GLfloat diffuse[4] = {0, 0, 0, 1.0};
    GLfloat specular[4] = {0, 0, 0, 1.0};

    // set texture coordinates based  texture flag
    if (texFlag) {
        gluQuadricTexture(quad, GLU_TRUE);
        // Set material property values
        for (int i = 0; i < 3; i++) {
            ambient[i] = 0.4;
            diffuse[i] = albedo;
            specular[i] = 0.8;
        }

    } else {
        gluQuadricTexture(quad, GLU_FALSE);
        for (int i = 0; i < 3; i++) {
            ambient[i] = color[i] * 0.6;
            diffuse[i] = color[i] * albedo;
            specular[i] = color[i];
        }
    }

    GLfloat shininess = 3.5;
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

/* setTexture()
 *
 * Give a planet the texture it desrerves
 *
 * Parameters:
 *      int tex - The texture "name" (OpenGL names for textures are ints).
 */
void setTexture(int tex) {
    // Enable texure mipmap
    glBindTexture(GL_TEXTURE_2D, tex);
    // Specify wrapping
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    // Specify fitting
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // Specify lighting
    (lightFlag) ? glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ) : glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
    glEnable(GL_TEXTURE_2D);
}
/* setOrbitProperties()
 *
 * Set draw properties for orbital paths
 */
void setOrbitProperties() {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Show the under side of the disk
    glDisable(GL_CULL_FACE);

    // Set material properties
    GLfloat labelMat[] = { 0.2, 0.2, 0.4, 1.0 };
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, labelMat );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, labelMat );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, labelMat );


}