/*
* orbit.cpp
*
* Program to demonstrate how to use a local
* coordinate method to position parts of a
* model in relation to other model parts.
*
* Draws a simple solar system, with a sun, planet and moon.
* Based on sample code from the OpenGL programming guide
* by Woo, Neider, Davis.  Addison-Wesley.
*
* Author: Samuel R. Buss
*
* Software accompanying the book
* 3D Computer Graphics: A Mathematical Introduction with OpenGL,
* by S. Buss, Cambridge University Press, 2003.
*
* Software is "as-is" and carries no warranty.  It may be used without
* restriction, but if you modify it, please change the filenames to
* prevent confusion between different versions.
*
* Bug reports: Sam Buss, sbuss@ucsd.edu.
* Web page: http://math.ucsd.edu/~sbuss/MathCG
*
* USAGE:
*    Press "r" key to toggle (off and on) running the animation
*    Press "s" key to single-step animation
*    The up and down array keys control the time step used in the animation rate.
*    Each key press multiplies or divides the times by a factor of two.
*    Press ESCAPE to exit.
*/

// JMW: Minor modifications for CSC433/533 Computer Graphics, Fall 2016.

#include <cstdlib>
#include <iostream>
#include <GL/freeglut.h>
#include "planet.h"
#include <string>
#include <map>

using namespace std;

// function prototypes
void OpenGLInit( void );
void Animate( void );
void ResizeWindow( int w, int h );
void KeyPressFunc( unsigned char Key, int x, int y );
void SpecialKeyFunc( int Key, int x, int y );
void InitSolarSystem();
void timeProgress (int value);
// Global things
bool infoFlag = false;
bool pauseFlag = false;
bool solidPlanets = false;
float hourSpeed = 1;

float sunColor[3] = {1.0, 1.0, 0.3};
float mercuryColor[3] = {0.8, 0.5, 0.3};
float venusColor[3] = {0.5, 0.6, 0.6};
float earthColor[3] = {0.2, 0.2, 1.0};
float marsColor[3] = {1.0, 0.2, 0.2};
float jupiterColor[3] = {0.8, 0.6, 0.0};
float saturnColor[3] = {0.6, 0.7, 0.0};
float uranusColor[3] = {0.2, 1.0, 0.6};
float neptuneColor[3] = {0.2, 0.4, 0.8};
float zZoom = -100;
float xRotate = 35.0;
float yRotate = 0.0;

map<string,planet*> planetMap;

void keyboardCallback(unsigned char key, int x, int y){
	switch(key){
        case 'Z':
		case 'z':
			zZoom += 10;
		break;

        case 'A':
		case 'a':
			zZoom -= 10;
		break;

        case 'X':
		case 'x':
			xRotate +=10;
		break;

        case 'S':
		case 's':
			xRotate -=10;
		break;

        case 'C':
		case 'c':
			xRotate +=10;
		break;

        case 'D':
		case 'd':
			xRotate -=10;
		break;		case 'q':
			hourSpeed++;
			cout << hourSpeed << endl;
		break;

        case 'W':
		case 'w':
			hourSpeed--;
			cout << hourSpeed << endl;
		break;

        case 'P':
		case 'p':
			cout << "pause" << endl;
			pauseFlag = !pauseFlag;
			cout << pauseFlag << endl;
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

void setDrawMode(Mode mode)
{
    switch  mode{
        case wire:
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            gluQuadricDrawStyle( object, GLU_LINE );
        break;

        case flat:
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glShadeModel( GL_FLAT );
        break;

        case smooth:
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glShadeModel( GL_SMOOTH );
        break;

        case image:
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
	glutWireSphere( body->getRadius(), 10, 10 );

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
		glTranslatef ( 0.0, yRotate, zZoom );

		// Rotate the plane of the elliptic
		// (rotate the model's plane about the x axis by fifteen degrees)
		glRotatef( xRotate, 1.0, 0.0, 0.0 );
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
	h = ( h == 0 ) ? 1 : h;
	w = ( w == 0 ) ? 1 : w;
	glViewport( 0, 0, w, h );	// View port uses whole window
	aspectRatio = ( float ) w / ( float ) h;

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
	glutInitWindowSize( 1200, 720 );
	glutCreateWindow( "Solar System Demo" );

	// Initialize OpenGL.
	OpenGLInit();

	// Set up the callback function for resizing windows
	glutReshapeFunc( ResizeWindow );

	// Callback for graphics image redrawing
	glutDisplayFunc( Animate );
	glutKeyboardFunc(keyboardCallback);

	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop( );

	return 0;
}
