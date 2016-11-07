/*
* orbit.cpp
*
* Program to simulate the solar system and allow eht user to fly around, viewing
* individual planets.  
*
* Authors: Elliott Rarden && Katie MacMillan
*
* This code is based on the code provided by Dr. Weiss at 
* www.mds.sdsmt.edu/csc344/Assignments/PA3
*
* Compile and run using 
*
*   $ make
*   $ ./solar
*
* If on an Apple Mac Computer, use
*
*   $ make osx
*   $ ./solar
*
* Notes:
*	  	If program is closed from comandline there will be a mem leak from 
*		dynamically allocated bmp images
*
*/

// C++ STL Libraries*/
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>

// Steve Jobs made me
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// Our headers
#include "globals.h"
#include "planet.h"
#include "imageReader.h"
#include "setFuncs.h"
#include "draw.h"
#include "callbacks.h"

using namespace std;

// function prototypes
void OpenGLInit( void );
void ResizeWindow( int w, int h );
void initLighting();
void InitSolarSystem();

// Global things
extern bool infoFlag, pauseFlag, solidFlag, lightFlag;
extern int width, height;
extern float hourSpeed, sunColor[3], mercuryColor[3], venusColor[3], earthColor[3], marsColor[3], jupiterColor[3], saturnColor[3], uranusColor[3], neptuneColor[3], xTranslate, yTranslate, zTranslate, xRotate , yRotate, zRotate, mouseX, mouseY;
map<string,planet*> planetMap;

void initLighting() {
    // specify material reflectivity
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 0.10 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess = { 100.0 };
    
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );
    
    // specify light source properties
    GLfloat light_position[] = { 0.0, 400.0, 0.0, 1.0 };
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };       // ambient light
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };       // diffuse light
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };      // highlights

    glEnable( GL_LIGHT0 );      // enable one light source
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );

    glShadeModel( GL_FLAT );    // start with flat shading (smooth is default)

    glEnable( GL_DEPTH_TEST );  // enable depth buffer for hidden-surface elimination
    glEnable( GL_NORMALIZE );   // automatic normalization of normals
    glEnable( GL_CULL_FACE );   // eliminate backfacing polygons
    glCullFace( GL_BACK );
    // glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );   // render back faces

    glClearColor( 0.0, 0.0, 0.0, 1.0 );     // black background
    glColor3f ( 0.8, 0.8, 0.0 );            // draw in yellow
}


// Initialize OpenGL's rendering modes
void OpenGLInit( void )
{
	glShadeModel( GL_FLAT );
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClearDepth( 1.0 );
	glEnable( GL_DEPTH_TEST );
}

// ResizeWindow is called when the window is resized
void ResizeWindow( int w, int h )
{
	float aspectRatio;
	height = ( h == 0 ) ? 1 : h;
	width = ( w == 0 ) ? 1 : w;
	glViewport( 0, 0, w, h );	// View port uses whole window
	aspectRatio = ( float ) width / ( float ) height;

	// Set up the projection view matrix (not very well!)
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0, aspectRatio, 1.0, 1000.0 );

	// Select the Modelview matrix
	glMatrixMode( GL_MODELVIEW );
}

void InitSolarSystem(){
	Image_s nullImage = {0, 0, NULL};


	planetMap.emplace("Sun", new planet ("Sun", nullptr, 696000/20, 0, 0, 25, sunColor, nullImage));
	planetMap.emplace("Mercury", new planet ("Mercury", planetMap.at("Sun"), 2439, 58, 88, 1416, mercuryColor, nullImage));
	planetMap.emplace("Venus", new planet ("Venus", planetMap.at("Sun"), 6052, 108, 225, 5832, venusColor, nullImage));
	planetMap.emplace("Earth", new planet ("Earth", planetMap.at("Sun"), 6378, 150, 365, 24, earthColor, nullImage));
	planetMap.emplace("Mars", new planet ("Mars", planetMap.at("Sun"), 3394, 228, 687, 24.6, marsColor, nullImage));
	planetMap.emplace("Jupiter", new planet ("Jupiter", planetMap.at("Sun"), 71398, 779, 4332, 9.8, jupiterColor, nullImage));
	planetMap.emplace("Saturn", new planet ("Saturn", planetMap.at("Sun"), 60270, 1424, 10761, 10.2, saturnColor, nullImage));
	planetMap.emplace("Uranus", new planet ("Uranus", planetMap.at("Sun"), 25550, 2867, 30682, 15.5, uranusColor, nullImage));
	planetMap.emplace("Neptune", new planet ("Neptune", planetMap.at("Sun"), 24750, 4492, 60195, 15.8, neptuneColor, nullImage));
}
// Main routine
// Set up OpenGL, hook up callbacks, and start the main loop
int main( int argc, char** argv )
{
	// Need to double buffer for animation
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	InitSolarSystem();
	// Create and position the graphics window
	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( width, height );
	glutCreateWindow( "Solar System Demo" );

	// Initialize OpenGL.
	OpenGLInit();

	// Set up the callback function for resizing windows
	glutReshapeFunc( ResizeWindow );

    // Set up lighting
    initLighting();
    glEnable(GL_LIGHTING);

	// Callback for graphics image redrawing
	glutDisplayFunc( displayCallback );
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(NULL);
	// glutPassiveMotionFunc(passiveMouseCallback);
	glutSpecialFunc(specialKeyCallback);
	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop( );

	return 0;
}
