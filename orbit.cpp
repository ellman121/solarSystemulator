/*
 * orbit.cpp
 *
 * Authors: Elliott Rarden && Katie MacMillan
 *
 * This code is based on the code provided by Dr. Weiss at
 * www.mds.sdsmt.edu/csc344/Assignments/PA3
 *
 * This program simulates the solar system allowing the user to explore the
 * various planets and moons.
 *
 * Lighting features may be toggled, as well as four different drawing
 * modes. The system may be drawn in wireframe, solid with flat shading,
 * solid with smooth shading, or texture mapped.
 *
 * The scene may be rotated about the x and y
 * axis, translated up, down, left and right, well as moving forward an
 * backward in the scene. In addition velocity feature may be enabled to
 * allow the user continuous movement through the system.
 *
 * The system may be viewed from the relative position of any planet
 * in the system, in effect causing it to be stationary while the
 * rest of the system moves around the relative body.
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
 *	Planets -
 *		All bodies are assumed to have circular orbits; orbital ecentricity
 *		has not been included, though axial tilts and orbital inclines have
 *		been, as well as heavenly body albedo coefficients.
 *
 *	Moons -
 *		Orbital paths of the moon are not displayed, as they cause too much
 *		clutter in the scene.
 *
 *		For Mars' moons we used only Phobos, as Deimos is too small to be
 *		seen in our sysem.
 *
 *		For Jupiters moons, we used only the Gallilean moons, as many
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

// Our headers
#include "globals.h"
#include "planet.h"
#include "rings.h"
#include "imageReader.h"
#include "initSystem.h"
#include "setFuncs.h"
#include "menu.h"
#include "drawInfo.h"
#include "drawObjects.h"
#include "callbacks.h"

// function prototypes
void OpenGLInit( void );
void ResizeWindow( int w, int h );
void initLighting();
void initMenus();


/* initMenus()
 *
 * Initialize the right-click menu for our program
 */
void initMenus() {
	int primaryMenu;
	int speedSelectSubmenu;
	int planetSelectSubmenu;

	// Create the speed selection submenu
	speedSelectSubmenu = glutCreateMenu(setSpeedSelectSubmenuOption);
	glutAddMenuEntry("Minimum Speed", 0);
	glutAddMenuEntry("1 Hour/Frame", 1);
	glutAddMenuEntry("12 Hours/Frame", 2);
	glutAddMenuEntry("1 Day/Frame", 3);
	glutAddMenuEntry("2 Days/Frame", 4);

	// Create planet selection submenu
	planetSelectSubmenu = glutCreateMenu(setPlanetSelectSubmenuOption);
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
	primaryMenu = glutCreateMenu(setMenuOption);
	glutAddSubMenu("Select Speed", speedSelectSubmenu);
	glutAddSubMenu("Planet Focus", planetSelectSubmenu);
	glutAddMenuEntry("Toggle Lights", 0);
	glutAddMenuEntry("Toggle Orbital Paths", 1);
	glutAddMenuEntry("Toggle Planet Labels", 2);
	glutAddMenuEntry("Toggle Moon Labels", 3);
	glutAddMenuEntry("Reset Position", 4);
	glutAddMenuEntry("Show Info Screen", 5);
	glutAddMenuEntry("Exit", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/* drawLightSource()
 *
 * Initalize OpenGL
 */
void OpenGLInit( void )
{
	glShadeModel( GL_FLAT );
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClearDepth( 1.0 );
	glEnable( GL_DEPTH_TEST );
}


/* ResizeWindow()
 *
 * Funciton that is called when the user resizes the window for GLUT
 *
 * Parameters:
 *		int w - The new width of the window
 *		int h - The new height of the window
 */
void ResizeWindow( int w, int h )
{
	float aspectRatio;
	height = ( h == 0 ) ? 1 : h;
	width = ( w == 0 ) ? 1 : w;
	// Set view port to use the whole window
	glViewport( 0, 0, w, h );
	aspectRatio = ( float ) width / ( float ) height;

	// Do a crappy job of setting up the projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	// Set the perspective
	gluPerspective( 60.0, aspectRatio, 1.0, 1000000.0 );

	// Select the Modelview matrix
	glMatrixMode( GL_MODELVIEW );
}

/* main()
 *
 * Main will initailze OpenGL, GLUT, the Solar System, and drop into GlutMainLoop();
 *
 * Parameters:
 *		int argc 	- Argument count
 *		char** argv - Argument values
 */
int main( int argc, char** argv )
{
	// Need to double buffer for animation
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	// Create and position the graphics window
	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( width, height );
	glutCreateWindow( "Solar System Demo" );

	// Initialize OpenGL.
	OpenGLInit();
	// Initialize popup menu
	initMenus();
	// Initialize solar system
	setPlanets();
	setMoons();
	setRings();
	setTexImage();
	// Set default positioning
	setView();

	// Set up the callback function for resizing windows
	glutReshapeFunc( ResizeWindow );

	// Set up lighting
	setInfoLighting();
	glEnable(GL_LIGHTING);

	// Set initial wireframe draw mode
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glShadeModel( GL_FLAT );

	// Callback for graphics image redrawing
	glutDisplayFunc( displayCallback );
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(NULL);
	glutSpecialFunc(specialKeyCallback);

	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop( );

	return 0;
}
