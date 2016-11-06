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
*/

// C++ STL Libraries
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
#include "planet.h"
#include "imageReader.h"

using namespace std;

// function prototypes
void OpenGLInit( void );
void Animate( void );
void setDrawMode( Mode mode );
void ResizeWindow( int w, int h );
void initLighting();
void KeyPressFunc( unsigned char Key, int x, int y );
void SpecialKeyFunc( int Key, int x, int y );
void InitSolarSystem();
void timeProgress (int value);
// Global things
bool infoFlag = false;
bool pauseFlag = false;
bool solidFlag = false;
bool lightFlag = true;
float hourSpeed = 1;
int width = 1200;
int height = 720;

float sunColor[3] = {1.0, 1.0, 0.3};
float mercuryColor[3] = {0.8, 0.5, 0.3};
float venusColor[3] = {0.5, 0.6, 0.6};
float earthColor[3] = {0.2, 0.2, 1.0};
float marsColor[3] = {1.0, 0.2, 0.2};
float jupiterColor[3] = {0.8, 0.6, 0.0};
float saturnColor[3] = {0.6, 0.7, 0.0};
float uranusColor[3] = {0.2, 1.0, 0.6};
float neptuneColor[3] = {0.2, 0.4, 0.8};
float xTranslate = 0.0;
float yTranslate = 0.0;
float zTranslate = -100;
float xRotate = 35.0;
float yRotate = 0.0;
float zRotate = 0.0;
float mouseX, mouseY;

map<string,planet*> planetMap;

void passiveMouseCallback(int x, int y){
	// if (mouseFlag) {
		float deltaX = x - ( width / 2.0 );
		float deltaY = height - y - ( height / 2.0 );
		xRotate += (deltaY-mouseY);
		yRotate += (deltaX-mouseX);
		mouseX = x - ( width / 2.0 );
		mouseY = height - y - ( height / 2.0 );
	// }
	glutPostRedisplay();
}

void mouseCallback (int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			mouseX = x - ( width / 2.0 );
			mouseY = height - y - ( height / 2.0 );
			glutMotionFunc(passiveMouseCallback);
		} else {
			glutMotionFunc(NULL);
		}
    }
}

void keyboardCallback(unsigned char key, int x, int y){
	switch(key){
		case 'D':
		case 'd':
			xTranslate = (xTranslate + 5 > 600) ? 600 : xTranslate - 5;
		break;

        case 'A':
		case 'a':
			xTranslate = (xTranslate - 5 < -600) ? -600 : xTranslate + 5;
		break;

		case 'W':
		case 'w':
			zTranslate = (zTranslate + 5 > 600) ? 600 : zTranslate + 5;
		break;
		case 'S':
		case 's':
			zTranslate = (zTranslate - 5 < -600) ? -600 : zTranslate - 5;
		break;
		// case 'h':
		// 	yTranslate = (yTranslate + 5 > 600) ? 600 : yTranslate + 5;
		// break;
		// case 'n':
		// 	yTranslate = (yTranslate - 5 < -600) ? -600 : yTranslate - 5;
		// break;

		case '+':
			hourSpeed++;
		break;

		case '-':
			hourSpeed--;
		break;

        case 'P':
		case 'p':
			pauseFlag = !pauseFlag;
		break;
		case 'R':
		case 'r':
			xRotate = 35.0;
			yRotate = 0.0;
			yTranslate = 0.0;
			zTranslate = -100.0;
		break;

		case 'L':
		case 'l':
			lightFlag = !lightFlag;
			if (lightFlag){
				glEnable(GL_LIGHTING);
			} else {
				glDisable(GL_LIGHTING);
			}
		break;
        // 1 -> wireframe, 2 -> flat, 3 -> smooth, 4 ->image
        case '1':
            setDrawMode(wire);
        break;
        case '2':
            setDrawMode(flat);
        break;
        case '3':
            setDrawMode(smooth);
        break;
        case '4':
            setDrawMode(image);
        break;

        // Escape key to quit
        case 27:
            exit(0);
        break;
	}
	glutPostRedisplay();
}

void specialKeyCallback(int key, int x, int y){
	switch(key){
        case GLUT_KEY_UP:
			xRotate +=5;
		break;

        case GLUT_KEY_DOWN:
			xRotate -=5;
		break;

        case GLUT_KEY_RIGHT:
			yRotate +=5;
		break;

        case GLUT_KEY_LEFT:
			yRotate -=5;
		break;

		// case 'f':
		// 	zRotate += 5;
		// break;
		// case 'v':
		// 	zRotate -= 10;
		// break;
	}
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
            solidFlag = false;
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

void drawBody(planet* body){
	string name = body->getName();
	if(name!="Sun"){
		// First position it around the sun. Use DayOfYear to determine its position.
		glRotatef( body->getOrbit(), 0.0, 1.0, 0.0 );
		glTranslatef( body->getDistance(), 0.0, 0.0 );
	}
	glPushMatrix();						// Save matrix state
	// Second, rotate the earth on its axis. Use HourOfDay to determine its rotation.
	glRotatef( body->getRotation(), 0.0, 1.0, 0.0 );
	// Third, draw the earth as a wireframe sphere.

	float color[3] = {};
	body->getColor(color);
	glColor3fv(color);

    if (solidFlag) {
        glutSolidSphere( body->getRadius(), 10, 10 );
    } else {
        glutWireSphere( body->getRadius(), 10, 10 );
    }

	glPopMatrix();						// Restore matrix state
	if(name != "Sun"){
		// move back to starting position
		glTranslatef( -1 * body->getDistance(), 0.0, 0.0 );
		glRotatef( -1 * body->getOrbit(), 0.0, 1.0, 0.0 );
	}

}
// Animate() handles the animation and the redrawing of the graphics window contents.
void Animate( void )
{
	if (!pauseFlag){
		for (auto& p: planetMap)
	    	p.second->step(hourSpeed);
	}
	// Clear the redering window
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	if(!infoFlag){

		// Clear the current matrix (Modelview)
		glLoadIdentity();

		// Back off eight units to be able to view from the origin.
		glTranslatef ( xTranslate, 0.0, 0.0 );
		glTranslatef ( 0.0, yTranslate, 0.0 );
		glTranslatef ( 0.0, 0.0, zTranslate );

		// Rotate the plane of the elliptic
		// (rotate the model's plane about the x axis by fifteen degrees)
		glRotatef( xRotate, 1.0, 0.0, 0.0 );
		glRotatef( yRotate, 0.0, 1.0, 0.0 );
		glRotatef( zRotate, 0.0, 0.0, 1.0 );

		//draw each primary body 
		for (auto& p: planetMap)
			drawBody(p.second);
		// Draw satelite bodies
		// for (auto& m: moonMap)
		// 	drawSatelite(m.second);

	}else {
		cout << "draw info screen" << endl;
	}

	// Flush the pipeline, and swap the buffers
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();		// Request a re-draw for animation purposes

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
	glutDisplayFunc( Animate );
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(NULL);
	// glutPassiveMotionFunc(passiveMouseCallback);
	glutSpecialFunc(specialKeyCallback);
	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop( );

	return 0;
}
