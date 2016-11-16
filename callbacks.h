


// displayCallback() handles the animation and the redrawing of the graphics window contents.

// Global things
extern bool infoFlag, pauseFlag, solidFlag, lightFlag, velocityFlag;
extern int width, height;
extern float  hourSpeed, xVelocity, yVelocity, zVelocity, xTranslate, yTranslate, zTranslate, xRotate , yRotate, mouseX, mouseY;
extern map<string,planet*> planetMap;
extern map<string,planet*> moonMap;
extern map<int,planet*> asteroidBelt;
string relative = "Sun";


// displayCallback() handles the animation and the redrawing of the graphics window contents.
void displayCallback( void )
{
	float aspectRatio = ( float ) width / ( float ) height;
	// Set up the projection view matrix (not very well!)
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0, aspectRatio, 1.0, 1000000.0 );

	// Select the Modelview matrix
	glMatrixMode( GL_MODELVIEW );

	// Progress frame objects and variables to the next hour
	setNextFrame();
	// Clear the redering window
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	if(!infoFlag){

		// Clear the current matrix (Modelview)
		glLoadIdentity();

		// Move to current view point
		glTranslatef ( xTranslate, yTranslate, zTranslate );

		// Rotate the scene according to user specifications
		glRotatef( xRotate, 1.0, 0.0, 0.0 );
		glRotatef( yRotate, 0.0, 1.0, 0.0 );

		// If viewing system relative to a planet, move scene negated distance and orbit of planet
		if (relative != "Sun"){
			float sRadius = planetMap.at("Sun")->getRadius();
			float rRadius = planetMap.at(relative)->getRadius();
			// how much bigger the sun is than relative body
			float percent = 1 - (rRadius/sRadius);

			// translate to position of planet center in x position
			// translate in z direction forward or back depending on how big the planet is
			glTranslatef (-1 * (sRadius + rRadius + planetMap.at(relative)->getDistance()), 0, 200*percent);
			glRotatef( -1 * planetMap.at(relative)->getOrbit(), 0.0, 1.0, 0.0 );
		}

		// Draw each planet
		for (auto& p: planetMap){
			(p.second->getName() != "Sun") ? drawBody(p.second, false) : drawSun(p.second, false);
		}

		// Draw the asteroid belt
		for (auto& a: asteroidBelt) {
			// cout << "Drawing asteroid " << a.first << endl;
			drawBody(a.second, false);
		}
		
		drawLighSource();
		drawStatus();
		
	glFlush();

	}else {
		cout << "draw info screen" << endl;
	}
	// Flush pipeline, swap buffers, and redraw
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();

}

void passiveMouseCallback(int x, int y){
	// get  current mouse position on screen
	float deltaX = x - ( width / 2.0 );
	float deltaY = height - y - ( height / 2.0 );

	// subtract stored coordinates from current and add to rotate
	xRotate += (deltaY-mouseY);
	yRotate += (deltaX-mouseX);

	// Store current position
	mouseX = deltaX;
	mouseY = deltaY;

	glutPostRedisplay();
}

void mouseCallback (int button, int state, int x, int y){

	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			// Store coordinates where button was pressed
			mouseX = x - ( width / 2.0 );
			mouseY = height - y - ( height / 2.0 );
			// Register mouse callback
			glutMotionFunc(passiveMouseCallback);
		} else {
			// Unregister mouse callback
			glutMotionFunc(NULL);
		}
    }
}

void keyboardCallback(unsigned char key, int x, int y){
	switch(key){
		// Increase x direction velocity or move left in scene
        case 'A':
		case 'a':
			(velocityFlag)? xVelocity++ : xTranslate += 5;
		break;
		// Decrease x direction velocity or move right in scene
		case 'D':
		case 'd':
			(velocityFlag) ? xVelocity-- : xTranslate -= 5;
		break;
		// Increase y direction velocity or move downward in scene
		case 'Z':
		case 'z':
			(velocityFlag) ? yVelocity++ : yTranslate += 5;
		break;
		// Decrease y direction velocity or move upward in scene
		case 'Q':
		case 'q':
			(velocityFlag) ? yVelocity-- : yTranslate -= 5;
		break;
		// Increase z direction velocity or move forward in scene
		case 'W':
		case 'w':
			(velocityFlag) ? zVelocity++ : zTranslate += 5;
		break;
		// Decrease z direction velocity or move backward in scene
		case 'S':
		case 's':
			(velocityFlag) ? zVelocity-- : zTranslate -= 5;
		break;

		// Increase hours per frame
		case '+':
			(hourSpeed >= 1) ? hourSpeed++ : hourSpeed += 0.1;
		break;

		// Decrement by 1 until speed is 1 hour per frame then by 0.1
		case '-':
			// Don't go below 0.1 hour per frame
			(hourSpeed > 1) ? hourSpeed-- : (hourSpeed > 0.1) ? hourSpeed -= 0.1 : hourSpeed += 0;
		break;

		// Pause animation
        case 'P':
		case 'p':
			pauseFlag = !pauseFlag;
		break;

		// Reset scene to original position
		case 'R':
		case 'r':
			resetView();
		break;

		//Toggle lighting
		case 'L':
		case 'l':
			lightFlag = !lightFlag;
			(lightFlag) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
		break;

		// toggle labels for planetary bodies
		case 'B':
		case 'b':
			bodyLabelFlag = !bodyLabelFlag;
		break;

		// toggle labels for moons
		case 'M':
		case 'm':
			moonLabelFlag = !moonLabelFlag;
		break;

		// toggle velocity travel, set velocity values to 0
		case 'V':
		case 'v':
			velocityFlag = !velocityFlag;
			if (velocityFlag){
				xVelocity = yVelocity = zVelocity = 0;
			}
		break;

		/*****Temporary for testing relative viewpoint*/
        case '0':
        	setRelativeBody("Sun");
        break;
        case '9':
        	setRelativeBody("Earth");
        break;
        case '8':
	        setRelativeBody("Jupiter");
        break;
        case '7':
	        setRelativeBody("Saturn");
        break;

        // Set drawing modes 1 -> wireframe, 2 -> flat, 3 -> smooth, 4 ->image
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
		// Rotate counter clockwise about the x axis
        case GLUT_KEY_UP:
			xRotate+= 0.5;
		break;

		// Rotate clockwise about the x axis
        case GLUT_KEY_DOWN:
			xRotate-= 0.5;
		break;

		// Rotate counter clockwise about the y axis
        case GLUT_KEY_RIGHT:
			yRotate+= 0.5;
		break;

		// Rotate clockwise about the y axis
        case GLUT_KEY_LEFT:
			yRotate-= 0.5;
		break;

	}

}
