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
*	Moons - 
*		If a value could not be found to define a moon's day length, the default
*		was set to that of Luna, the Earth's moon.
*
*		For Jupiters moons, we used only the Inner and Gallilean moons, as many
* 		of the other moons had very small diameters < 10 km and were fairly 
*		inconcequential.
*
*		Saturn has a total of 62 satellite bodies, however, 7 of them comprise 
*		99.96% of the mass which orbits Saturn. For this reason we used only the 
*		primary 7 moons in this model.
*
*		For Uranus we used only the five major moons. These moons are quite dark
*		with very low reflectivity coefficients.
*
*		Neptune has 14 moons. Triton is the largest making up 99.59% of the mass
*		orbiting Neptune. The next two largest moons are __ and __ which together
*		comprise 0.36% of the orbiting mass. The remaining 11 moons  make up the
*		final 0.05% of the mass. For this reason we modeled only Triton.
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
// #include "globals.h"
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
void initMenus();

// Global things
bool infoFlag = false;
bool pauseFlag = false;
bool solidFlag = false;
bool lightFlag = true;
bool bodyLabelFlag = true;
bool moonLabelFlag = false;
bool velocityFlag = false;
float hourSpeed = 1;
int width = 1200;
int height = 720;

float sunColor[3] = {1.0, 1.0, 0.3};
float mercuryColor[3] = {0.8, 0.5, 0.5};
float venusColor[3] = {0.6, 0.8, 0.3};
float earthColor[3] = {0.0, 0.4, 1.0};
float marsColor[3] = {1.0, 0.2, 0.2};
float jupiterColor[3] = {0.64, 0.165, 0.165};
float saturnColor[3] = {0.5, 0.7, 0.0};
float uranusColor[3] = {0.2, 1.0, 0.6};
float neptuneColor[3] = {0.2, 0.4, 0.8};
float moonColor[3] = {0.4, 0.4, 0.5};


float xTranslate = 0.0;
float yTranslate = 0.0;
float zTranslate = -200;
float xVelocity = 0;
float yVelocity = 0;
float zVelocity = 0;
float xRotate = 0.0;
float yRotate = 0.0;
float mouseX, mouseY;
map<string,planet*> planetMap;
map<string,planet*> moonMap;

void initMenus() {
	int primaryMenu;
	int speedSelectSubmenu;
	int planetSelectSubmenu;

	// Create the speed selection submenu
	speedSelectSubmenu = glutCreateMenu(processSpeedSelectSubmenuOption);
	glutAddMenuEntry("Minimum Speed", 0);
	glutAddMenuEntry("1 Hour/Frame", 1);
	glutAddMenuEntry("12 Hours/Frame", 2);
	glutAddMenuEntry("1 Day/Frame", 3);
	glutAddMenuEntry("2 Days/Frame", 4);

	// Create planet selection submenu
	planetSelectSubmenu = glutCreateMenu(processPlanetSelectSubmenuOption);
	glutAddMenuEntry("Sun", 0);
	glutAddMenuEntry("Mercury", 1);
	glutAddMenuEntry("Venus", 2);
	glutAddMenuEntry("Earth", 3);
	glutAddMenuEntry("Mars", 4);
	glutAddMenuEntry("Jupiter", 5);
	glutAddMenuEntry("Saturn", 6);
	glutAddMenuEntry("Uranus", 7);
	glutAddMenuEntry("Neptune", 8);

	// Create our primary menu
	primaryMenu = glutCreateMenu(processMenuOption);
	glutAddSubMenu("Select Speed", speedSelectSubmenu);
	glutAddSubMenu("Planet Focus", planetSelectSubmenu);
	glutAddMenuEntry("Toggle Lights", 0);
	glutAddMenuEntry("Toggle Planet Labels", 1);
	glutAddMenuEntry("Toggle Moon Labels", 2);
	glutAddMenuEntry("Reset Position", 3);
	glutAddMenuEntry("Show Info Screen", 4);
	glutAddMenuEntry("Exit", 5);
}

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
	gluPerspective( 60.0, aspectRatio, 1.0, 1000000.0 );

	// Select the Modelview matrix
	glMatrixMode( GL_MODELVIEW );
}

// 200 Lines to emplace each sun/planet/moon in the solar system
void InitSolarSystem(){
	Image_s nullImage = {0, 0, NULL};


	vector<string> satellites = {};
	planetMap.emplace("Sun", new planet ("Sun", "", 696000/10, 0, 0, 25, 0, 7.25, 0, sunColor, nullImage, satellites));
	planetMap.emplace("Mercury", new planet ("Mercury", "Sun", 2439, 58000000, 88, 1416, 3.38, 2.04, 0.68, mercuryColor, nullImage, satellites));
	planetMap.emplace("Venus", new planet ("Venus", "Sun", 6052, 108000000, 225, 5832, 3.86, 177.36, 0.9, venusColor, nullImage, satellites));

	satellites = {"Luna"};
	planetMap.emplace("Earth", new planet ("Earth", "Sun", 6378, 150000000, 365, 24, 7.155, 23.4392811, 0.306, earthColor, nullImage, satellites));

	satellites = {"Deimos", "Phobos"};
	planetMap.emplace("Mars", new planet ("Mars", "Sun", 3394, 228000000, 687, 24.6, 5.65, 25.19, 0.25, marsColor, nullImage, satellites));

	satellites = {"Adrastea", "Amalthea", "Callisto", "Europa", "Ganymede", "Io", "Metis", "Thebe"};
	planetMap.emplace("Jupiter", new planet ("Jupiter", "Sun", 71398, 779000000, 4332, 9.8, 6.09, 3.13, 0.343, jupiterColor, nullImage, satellites));

	satellites = {"Dione", "Enceladus", "Iapetus", "Mimas", "Rhea", "Tethys", "Titan"};
	planetMap.emplace("Saturn", new planet ("Saturn", "Sun", 60270, 1424000000, 10761, 10.2, 5.51, 26.73, 0.342, saturnColor, nullImage,satellites));

	satellites = {"Ariel", "Miranda", "Oberon", "Titania", "Umbriel"};
	planetMap.emplace("Uranus", new planet ("Uranus", "Sun", 25550, 2867000000, 30682, 15.5, 6.48, 97.77, 0.3, uranusColor, nullImage, satellites));

	satellites = {"Triton"};
	planetMap.emplace("Neptune", new planet ("Neptune", "Sun", 24750, 4492000000, 60195, 15.8, 6.43, 28.32, 0.290, neptuneColor, nullImage, satellites));









	satellites.clear();
//  									name, parent, radius, distance, daysPerYear, hoursPerDay, incline, tilt, reflectance, color[3], img, satellites)
	moonMap.emplace("Luna", new planet("Luna", "Earth",1738, 384400, 27.322, 27.322, 5.145, 6.687, 0.136, moonColor, nullImage, satellites));
	
	moonMap.emplace("Deimos", new planet("Deimos", "Mars",4, 23460, 1.263, 30.312, 0.93, 0, 0.068, moonColor, nullImage, satellites));
	moonMap.emplace("Phobos", new planet("Phobos", "Mars",280, 9270, 0.319, 7.656, 1.093, 0, 0.071, moonColor, nullImage, satellites));

	
	
	moonMap.emplace("Adrastea", new planet("Adrastea", "Jupiter", 208, 128980, 0.298, 7.152, 0.03, 0, 0.1, moonColor, nullImage, satellites));
	moonMap.emplace("Amalthea", new planet("Amalthea", "Jupiter", 83.5, 181300, 0.498, 11.952, 0.374, 0, 0.09, moonColor, nullImage, satellites));
	moonMap.emplace("Callisto", new planet("Callisto", "Jupiter", 2400, 1883000, 16.689, 400.536, 0.205, 0, 0.22, moonColor, nullImage, satellites));
	moonMap.emplace("Europa", new planet("Europa", "Jupiter", 1563, 670900, 3.551, 85, 0.471, 0.1, 0.67, moonColor, nullImage, satellites));
	moonMap.emplace("Ganymede", new planet("Ganymede", "Jupiter", 2638, 1070000, 7.155, 171.72, 0.204, 0.33, 0.43, moonColor, nullImage, satellites));
	moonMap.emplace("Io", new planet("Io", "Jupiter", 1814.5, 421600, 1.769, 42.456, 0.05, 0, 0.63, moonColor, nullImage, satellites));
	moonMap.emplace("Metis", new planet("Metis", "Jupiter", 20, 127960, 0.295, 7.08, 0.06, 0, 0.061, moonColor, nullImage, satellites));
	moonMap.emplace("Thebe", new planet("Thebe", "Jupiter", 50, 221900, 0.675, 16.2, 1.076, 0, 0.047, moonColor, nullImage, satellites));
	
	moonMap.emplace("Dione", new planet("Dione", "Saturn",561.4, 377400, 2.737, 65.68596, 0.019, 0, 0.998, moonColor, nullImage, satellites));
	moonMap.emplace("Enceladus", new planet("Enceladus", "Saturn",249, 238020, 1.37, 32.885232, 0.019, 0, 0.99, moonColor, nullImage, satellites));
	moonMap.emplace("Iapetus", new planet("Iapetus", "Saturn",718, 3561300, 79.3215, 1903.716, 15.47, 0, 0.225, moonColor, nullImage, satellites));
	moonMap.emplace("Mimas", new planet("Mimas", "Saturn",199, 185520, 0.942, 22.608, 1.574, 0, 0.962,moonColor, nullImage, satellites));
	moonMap.emplace("Rhea", new planet("Rhea", "Saturn",764, 527040, 4.518, 108.437088, 0.345, 0, 0.949, moonColor, nullImage, satellites));
	moonMap.emplace("Tethys", new planet("Tethys", "Saturn",530, 294660, 1.888, 45.307248, 1.12, 0, 0.8, moonColor, nullImage, satellites));
	moonMap.emplace("Titan", new planet("Titan", "Saturn",2575, 1221850, 15.945, 382.68, 0.34854, 0, 0.22, moonColor, nullImage, satellites));


	moonMap.emplace("Ariel", new planet("Ariel", "Uranus",580, 191240, 2.52, 60.48, 0.260, 0, 0.23, moonColor, nullImage, satellites));
	moonMap.emplace("Miranda", new planet("Miranda", "Uranus",236, 129780, 1.414, 33.936, 4.232, 0, 0.32, moonColor, nullImage, satellites));
	moonMap.emplace("Oberon", new planet("Oberon", "Uranus",763, 582600, 13.463, 323.112, 0.058, 0, 0.14, moonColor, nullImage, satellites));
	moonMap.emplace("Titania", new planet("Titania", "Uranus",789, 435840, 8.706, 208.944, 0.340 , 0, 0.17, moonColor, nullImage, satellites));
	moonMap.emplace("Umbriel", new planet("Umbriel", "Uranus",595, 265970, 4.144, 99.456, 0.128, 0, 0.10, moonColor, nullImage, satellites));
	

	moonMap.emplace("Triton", new planet("Triton", "Neptune",1352.5, 354800, 5.877, 141.048, 156.885, 0, 0.76, moonColor, nullImage, satellites));
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
