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


// Global things
bool infoFlag = false;
bool pauseFlag = false;
bool solidFlag = false;
bool lightFlag = true;
float hourSpeed = 1;
int width = 1200;
int height = 720;

float sunColor[3] = {1.0, 1.0, 0.3};
float mercuryColor[3] = {0.8, 0.5, 0.5};
float venusColor[3] = {0.8, 0.5, 0.3};
float earthColor[3] = {0.2, 0.2, 1.0};
float marsColor[3] = {1.0, 0.2, 0.2};
float jupiterColor[3] = {0.8, 0.6, 0.0};
float saturnColor[3] = {0.6, 0.7, 0.0};
float uranusColor[3] = {0.2, 1.0, 0.6};
float neptuneColor[3] = {0.2, 0.4, 0.8};
float moonColor[3] = {1, 1, 1};
float xTranslate = 0.0;
float zTranslate = -200;
float xRotate = 35.0;
float yRotate = 0.0;
float mouseX, mouseY;
map<string,planet*> planetMap;
map<string,planet*> moonMap;


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

void InitSolarSystem(){
	Image_s nullImage = {0, 0, NULL};


	vector<string> satellites = {};
	planetMap.emplace("Sun", new planet ("Sun", "", 696000/10, 0, 0, 25, 0, sunColor, nullImage, satellites));
	planetMap.emplace("Mercury", new planet ("Mercury", "Sun", 2439, 58000000, 88, 1416, 0, mercuryColor, nullImage, satellites));
	planetMap.emplace("Venus", new planet ("Venus", "Sun", 6052, 108000000, 225, 5832, 0, venusColor, nullImage, satellites));
	satellites = {"Luna"};
	planetMap.emplace("Earth", new planet ("Earth", "Sun", 6378, 150000000, 365, 24, 0, earthColor, nullImage, satellites));
	satellites = {"Deimos", "Phobos"};
	planetMap.emplace("Mars", new planet ("Mars", "Sun", 3394, 228000000, 687, 24.6, 0, marsColor, nullImage, satellites));
	satellites = {"Adrastea","Aitne","Amalthea","Ananke","Aoede","Arche","Autonoe","Callisto","Carme","Callirrhoe","Carpo","Chaldene","Cyllene","Elara","Erinome","Euanthe","Eukelade","Euporie","Europa","Eurydome","Ganymede","Harpalyke","Hegemone","Helike","Hermippe","Himalia","Io","Iocaste","Isonone","Kale","Kallichore","Kalyke","Kore","Leda","Lysithea","Magaclite","Metis","Mneme","Orthosie","Pasiphae","Pasithee","Praxidike","Sinope","Sponde","S/2000 J11","S/2003 J2","S/2003 J3","S/2003 J4","S/2003 J5","S/2003 J9","S/2003 J10","S/2003 J12","S/2003 J15","S/2003 J16","S/2003 J17","S/2003 J18","S/2003 J19","S/2003 J 23","Taygete","Thebe","Thelxinoe","Themisto","Thyone"};
	planetMap.emplace("Jupiter", new planet ("Jupiter", "Sun", 71398, 779000000, 4332, 9.8, 0, jupiterColor, nullImage, satellites));
	satellites = {"Aegir", "Albiorix", "Anthe", "Atlas", "Bebhionn", "Bergelmir", "Bestla", "Calypso", "Daphnis", "Dione", "Enceladus", "Epimetheus", "Erriapo", "Farbauti", "Fenrir", "Fornjot", "Greip", "Hati", "Helene", "Hyperion", "Hyrokkin", "Iapetus", "Ijiraq", "Janus", "Jarnsaxa", "Kari", "Kiviuq", "Loge", "Methone", "Mimas", "Mundilfari", "Narvi", "Paaliaq", "Pallene", "Pan", "Pandora", "Phoebe", "Polydeuces", "Prometheus", "Rhea", "Siarnaq", "Skathi", "Skoll", "Surtur", "Suttungr", "S/2004 S07", "S/2004 S12", "S/2004 S13", "S/2004 S17", "S/2006 S1", "S/2006 S3", "S/2007 S2", "S/2007 S3", "Tarqeq", "Tarvos", "Telesto", "Tethys", "Thrymr", "Titan", "Ymir"};
	planetMap.emplace("Saturn", new planet ("Saturn", "Sun", 60270, 1424000000, 10761, 10.2, 0, saturnColor, nullImage,satellites));
	satellites = {"Ariel", "Belinda", "Bianca", "Caliban", "Cordelia", "Cressida", "Cupid", "Desdemona", "Ferdinand", "Francisco", "Juliet", "Mab", "Margaret", "Miranda", "Oberon", "Ophelia", "Perdita", "Portia", "Prospero", "Puck", "Rosalind", "Setebos", "Stephano", "Sycorax", "Titania", "Trinculo", "Umbriel"};

	planetMap.emplace("Uranus", new planet ("Uranus", "Sun", 25550, 2867000000, 30682, 15.5, 0, uranusColor, nullImage, satellites));
	satellites = {"Despina", "Galatea", "Halimede", "Larissa", "Laomedeia", "Naiad", "Nereid", "Neso", "Proteus", "Psamathe", "Sao", "Thalassa", "Triton"};

	planetMap.emplace("Neptune", new planet ("Neptune", "Sun", 24750, 4492000000, 60195, 15.8, 0, neptuneColor, nullImage, satellites));

	satellites.clear();
	moonMap.emplace("Luna", new planet("Luna", "Earth",1738, 384400, 27.322, 0, 30.0, moonColor, nullImage, satellites));
	moonMap.emplace("Deimos", new planet("Deimos", "Mars",4, 23460, 1.263, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Phobos", new planet("Phobos", "Mars",280, 9270, 0.319, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Adrastea", new planet("Adrastea", "Jupiter",208, 128980, 0.298, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Aitne", new planet("Aitne", "Jupiter",1.5, 23547000, 736, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Amalthea", new planet("Amalthea", "Jupiter",17554, 181300, 0.498, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Ananke", new planet("Ananke", "Jupiter",10, 21200000, 631, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Aoede", new planet("Aoede", "Jupiter",2, 23807655, 748.8, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Arche", new planet("Arche", "Jupiter",1.5, 23064000, 715.6, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Autonoe", new planet("Autonoe", "Jupiter",2, 24122000, 753, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Callisto", new planet("Callisto", "Jupiter",2400, 1883000, 16.689, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Carme", new planet("Carme", "Jupiter",15, 22600000, 692, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Callirrhoe", new planet("Callirrhoe", "Jupiter",5, 24200000, 774, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Carpo", new planet("Carpo", "Jupiter",1.5, 17100000, 456.5, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Chaldene", new planet("Chaldene", "Jupiter",1.9, 23179000, 741, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Cyllene", new planet("Cyllene", "Jupiter",1, 24000000, 737.8, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Elara", new planet("Elara", "Jupiter",40, 11737000, 259.65, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Erinome", new planet("Erinome", "Jupiter",1.6, 23279000, 672, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Euanthe", new planet("Euanthe", "Jupiter",1.5, 21017000, 622, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Eukelade", new planet("Eukelade", "Jupiter",2, 24557295, 746.4, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Euporie", new planet("Euporie", "Jupiter",1, 19394000, 534, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Europa", new planet("Europa", "Jupiter",1563, 670900, 3.551, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Eurydome", new planet("Eurydome", "Jupiter",1.5, 23219000, 713, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Ganymede", new planet("Ganymede", "Jupiter",2638, 1070000, 7.155, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Harpalyke", new planet("Harpalyke", "Jupiter",2.15, 21105000, 595, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Hegemone", new planet("Hegemone", "Jupiter",1.5, 24514095, 781.6, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Helike", new planet("Helike", "Jupiter",2, 10972830, 233.8, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Hermippe", new planet("Hermippe", "Jupiter",2, 21252000, 630, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Himalia", new planet("Himalia", "Jupiter",85, 11480000, 250.57, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Io", new planet("Io", "Jupiter",1814.5, 421600, 1.769, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Iocaste", new planet("Iocaste", "Jupiter",2.6, 21269000, 657, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Isonone", new planet("Isonone", "Jupiter",1.9, 23217000, 712, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Kale", new planet("Kale", "Jupiter",1, 23124000, 609, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Kallichore", new planet("Kallichore", "Jupiter",1, 22395390, 683, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Kalyke", new planet("Kalyke", "Jupiter",2.6, 23583000, 760, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Kore", new planet("Kore", "Jupiter",1, 24543000, 779.2, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Leda", new planet("Leda", "Jupiter",5, 11094000, 238.72, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Lysithea", new planet("Lysithea", "Jupiter",12, 11720000, 259.22, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Magaclite", new planet("Magaclite", "Jupiter",2.7, 23806000, 771, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Metis", new planet("Metis", "Jupiter",20, 127960, 0.295, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Mneme", new planet("Mneme", "Jupiter",1, 21069000, 620.04, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Orthosie", new planet("Orthosie", "Jupiter",1, 21168000, 617, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Pasiphae", new planet("Pasiphae", "Jupiter",18, 23500000, 735, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Pasithee", new planet("Pasithee", "Jupiter",1, 23029000, 715, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Praxidike", new planet("Praxidike", "Jupiter",3.4, 21147000, 632, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Sinope", new planet("Sinope", "Jupiter",14, 23700700, 758, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Sponde", new planet("Sponde", "Jupiter",1, 23808000, 732, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2000 J11", new planet("S/2000 J11", "Jupiter",2, 12555000, 284.3, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J2", new planet("S/2003 J2", "Jupiter",1, 28570410, 982.5, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J3", new planet("S/2003 J3", "Jupiter",1, 18339885, 504, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J4", new planet("S/2003 J4", "Jupiter",1, 23257920, 723.2, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J5", new planet("S/2003 J5", "Jupiter",2, 24084180, 759.7, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J9", new planet("S/2003 J9", "Jupiter",0.5, 22441680, 683, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J10", new planet("S/2003 J10", "Jupiter",1, 24249600, 767, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J12", new planet("S/2003 J12", "Jupiter",0.5, 19002480, 533.3, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J15", new planet("S/2003 J15", "Jupiter",1, 22000000, 668.4, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J16", new planet("S/2003 J16", "Jupiter",1, 21000000, 595.4, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J17", new planet("S/2003 J17", "Jupiter",1, 22000000, 690.3, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J18", new planet("S/2003 J18", "Jupiter",1, 20700000, 606.3, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J19", new planet("S/2003 J19", "Jupiter",1, 22800000, 701.3, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2003 J 23", new planet("S/2003 J 23", "Jupiter",1, 23563000, 732.44, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Taygete", new planet("Taygete", "Jupiter",2.5, 23360000, 687, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Thebe", new planet("Thebe", "Jupiter",50, 221900, 0.675, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Thelxinoe", new planet("Thelxinoe", "Jupiter",1, 21162000, 628.09, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Themisto", new planet("Themisto", "Jupiter",4, 7507000, 130.07, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Thyone", new planet("Thyone", "Jupiter",2, 21312000, 615, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Aegir", new planet("Aegir", "Saturn",3, 20735000, 1116.5, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Albiorix", new planet("Albiorix", "Saturn",15, 16392000, 783, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Anthe", new planet("Anthe", "Saturn",0.5, 197700, 1.04, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Atlas", new planet("Atlas", "Saturn",499.5, 137640, 0.602, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Bebhionn", new planet("Bebhionn", "Saturn",3, 17119000, 834.8, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Bergelmir", new planet("Bergelmir", "Saturn",3, 19338000, 1005.9, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Bestla", new planet("Bestla", "Saturn",3.5, 20129000, 1083.6, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Calypso", new planet("Calypso", "Saturn",240, 294660, 1.888, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Daphnis", new planet("Daphnis", "Saturn",3.5, 136500, 0.594, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Dione", new planet("Dione", "Saturn",560, 377400, 2.737, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Enceladus", new planet("Enceladus", "Saturn",249, 238020, 1.37, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Epimetheus", new planet("Epimetheus", "Saturn",7590, 151422, 0.694, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Erriapo", new planet("Erriapo", "Saturn",5, 17611000, 871.17, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Farbauti", new planet("Farbauti", "Saturn",2.5, 20390000, 1086.1, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Fenrir", new planet("Fenrir", "Saturn",2, 22453000, 1260.3, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Fornjot", new planet("Fornjot", "Saturn",3, 25108000, 1490.9, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Greip", new planet("Greip", "Saturn",3, 18206000, 921.2, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Hati", new planet("Hati", "Saturn",3, 19856000, 1038.7, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Helene", new planet("Helene", "Saturn",504, 377400, 2.737, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Hyperion", new planet("Hyperion", "Saturn",40680, 1481000, 21.277, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Hyrokkin", new planet("Hyrokkin", "Saturn",4, 18437000, 931.8, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Iapetus", new planet("Iapetus", "Saturn",718, 3561300, 79.3215, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Ijiraq", new planet("Ijiraq", "Saturn",7, 11440000, 451.48, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Janus", new planet("Janus", "Saturn",14630, 151472, 0.695, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Jarnsaxa", new planet("Jarnsaxa", "Saturn",3, 18811000, 964.7, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Kari", new planet("Kari", "Saturn",3.5, 22118000, 1233.6, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Kiviuq", new planet("Kiviuq", "Saturn",8.5, 11365000, 449.22, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Loge", new planet("Loge", "Saturn",3, 23065000, 1312, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Methone", new planet("Methone", "Saturn",1.5, 194000, 1.01, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Mimas", new planet("Mimas", "Saturn",199, 185520, 0.942, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Mundilfari", new planet("Mundilfari", "Saturn",3.5, 18709000, 951.38, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Narvi", new planet("Narvi", "Saturn",4, 18719000, 956.2, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Paaliaq", new planet("Paaliaq", "Saturn",12.5, 15199000, 686.92, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Pallene", new planet("Pallene", "Saturn",2, 211000, 1.14, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Pan", new planet("Pan", "Saturn",9.66, 133630, 0.575, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Pandora", new planet("Pandora", "Saturn",3410, 141700, 0.629, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Phoebe", new planet("Phoebe", "Saturn",110, 12952000, 550.48, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Polydeuces", new planet("Polydeuces", "Saturn",2, 377400, 2.74, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Prometheus", new planet("Prometheus", "Saturn",5032, 139350, 0.613, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Rhea", new planet("Rhea", "Saturn",764, 527040, 4.518, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Siarnaq", new planet("Siarnaq", "Saturn",22.5, 18160000, 893.07, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Skathi", new planet("Skathi", "Saturn",4, 15645000, 728.93, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Skoll", new planet("Skoll", "Saturn",3, 17665000, 878.3, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Surtur", new planet("Surtur", "Saturn",3, 22707000, 1297.7, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Suttungr", new planet("Suttungr", "Saturn",3.5, 19470000, 1016.8, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2004 S07", new planet("S/2004 S07", "Saturn",3, 19800000, 1103, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2004 S12", new planet("S/2004 S12", "Saturn",2.5, 19650000, 1048, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2004 S13", new planet("S/2004 S13", "Saturn",3, 18450000, 906, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2004 S17", new planet("S/2004 S17", "Saturn",2, 18600000, 986, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2006 S1", new planet("S/2006 S1", "Saturn",3, 18981135, 970, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2006 S3", new planet("S/2006 S3", "Saturn",3, 21132000, 1142, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2007 S2", new planet("S/2007 S2", "Saturn",3, 16560000, 800, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("S/2007 S3", new planet("S/2007 S3", "Saturn",2.5, 20518500, 1100, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Tarqeq", new planet("Tarqeq", "Saturn",3.5, 18009000, 887.5, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Tarvos", new planet("Tarvos", "Saturn",8, 18239000, 925.7, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Telesto", new planet("Telesto", "Saturn",240, 294660, 1.888, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Tethys", new planet("Tethys", "Saturn",530, 294660, 1.888, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Thrymr", new planet("Thrymr", "Saturn",3.5, 20470000, 1088.89, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Titan", new planet("Titan", "Saturn",2575, 1221850, 15.945, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Ymir", new planet("Ymir", "Saturn",10, 23096000, 1312.37, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Ariel", new planet("Ariel", "Uranus",580, 191240, 2.52, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Belinda", new planet("Belinda", "Uranus",33, 75260, 0.624, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Bianca", new planet("Bianca", "Uranus",21, 75260, 0.433, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Caliban", new planet("Caliban", "Uranus",40, 7200000, 579.5, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Cordelia", new planet("Cordelia", "Uranus",13, 49750, 0.335, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Cressida", new planet("Cressida", "Uranus",31, 61770, 0.464, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Cupid", new planet("Cupid", "Uranus",6, 74800, 0.618, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Desdemona", new planet("Desdemona", "Uranus",27, 62660, 0.474, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Ferdinand", new planet("Ferdinand", "Uranus",10.5, 20901000, 2823.4, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Francisco", new planet("Francisco", "Uranus",11, 4276000, 266.6, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Juliet", new planet("Juliet", "Uranus",42, 64360, 0.493, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Mab", new planet("Mab", "Uranus",8, 97734, 0.923, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Margaret", new planet("Margaret", "Uranus",5.5, 14688700, 1694.8, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Miranda", new planet("Miranda", "Uranus",236, 129780, 1.414, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Oberon", new planet("Oberon", "Uranus",763, 582600, 13.463, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Ophelia", new planet("Ophelia", "Uranus",15.2, 53440, 0.3764, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Perdita", new planet("Perdita", "Uranus",10, 76420, 0.638, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Portia", new planet("Portia", "Uranus",54, 66085, 0.513, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Prospero", new planet("Prospero", "Uranus",15, 16256000, 5.346, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Puck", new planet("Puck", "Uranus",77, 86010, 0.762, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Rosalind", new planet("Rosalind", "Uranus",27, 69941, 0.558, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Setebos", new planet("Setebos", "Uranus",23.5, 17418000, 2234.8, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Stephano", new planet("Stephano", "Uranus",16, 8004000, 677.4, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Sycorax", new planet("Sycorax", "Uranus",80, 12200000, 1283.39, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Titania", new planet("Titania", "Uranus",789, 435840, 8.706, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Trinculo", new planet("Trinculo", "Uranus",5, 8578000, 759, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Umbriel", new planet("Umbriel", "Uranus",595, 265970, 4.144, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Despina", new planet("Despina", "Neptune",80, 62000, 0.4, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Galatea", new planet("Galatea", "Neptune",70, 52500, 0.33, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Halimede", new planet("Halimede", "Neptune",30, 15686000, 1874.83, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Larissa", new planet("Larissa", "Neptune",100, 73600, 0.56, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Laomedeia", new planet("Laomedeia", "Neptune",19, 22613200, 2980.4, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Naiad", new planet("Naiad", "Neptune",25, 48200, 0.3, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Nereid", new planet("Nereid", "Neptune",170, 5513400, 360.16, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Neso", new planet("Neso", "Neptune",30, 47279670, 9007.1, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Proteus", new planet("Proteus", "Neptune",210, 117600, 1.12, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Psamathe", new planet("Psamathe", "Neptune",19, 46738000, 9136.11, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Sao", new planet("Sao", "Neptune",19, 22337190, 2925.6, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Thalassa", new planet("Thalassa", "Neptune",45, 50000, 0.31, 0, 0, moonColor, nullImage, satellites));
	moonMap.emplace("Triton", new planet("Triton", "Neptune",1352.5, 354800, 5.877, 0, 0, moonColor, nullImage, satellites));



















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
