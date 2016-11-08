


// displayCallback() handles the animation and the redrawing of the graphics window contents.

// Global things
extern bool infoFlag, pauseFlag, solidFlag, lightFlag, velocityFlag;
extern int width, height;
extern float  hourSpeed, xVelocity, yVelocity, zVelocity, xTranslate, yTranslate, zTranslate, xRotate , yRotate, mouseX, mouseY;
extern map<string,planet*> planetMap;
extern map<string,planet*> moonMap;

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
	if (!pauseFlag){
		for (auto& p: planetMap)
	    	p.second->step(hourSpeed);
		for (auto& m: moonMap)
	    	m.second->step(hourSpeed);
	}
	if (velocityFlag){
		xTranslate += xVelocity;
		yTranslate += yVelocity;
		zTranslate += zVelocity;
	}
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

		drawBodies();
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

void mouseCallback(int x, int y){
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
			glutMotionFunc(mouseCallback);
		} else {
			glutMotionFunc(NULL);
		}
    }
}

void keyboardCallback(unsigned char key, int x, int y){
	switch(key){
		// Move scene right
		case 'D':
		case 'd':
			if(velocityFlag){
				xVelocity--;
			} else {
				xTranslate -= 5;
			}
		break;

		// Move scene left
        case 'A':
		case 'a':
			if(velocityFlag){
				xVelocity++;
			} else {
				xTranslate += 5;
			}
		break;
		// Move up in scene
		case 'Q':
		case 'q':
			if(velocityFlag){
				yVelocity++;
			} else {
				yTranslate += 5;
			}
		break;
		//Move down in scene
		case 'Z':
		case 'z':
			if(velocityFlag){
				yVelocity--;
			} else {
				yTranslate -= 5;
			}
			break;
		// Move forward in scene
		case 'W':
		case 'w':
			if(velocityFlag){
				zVelocity++;
			} else {
				zTranslate += 5;
			}
		break;
		//Move backward in scene
		case 'S':
		case 's':
			if(velocityFlag){
				zVelocity--;
			} else {
				zTranslate -= 5;
			}
		break;

		// Increase hours per frame
		case '+':
			if (hourSpeed >= 1){
				hourSpeed++;
			} else {
				hourSpeed += 0.1;
			}
		break;

		// Decrease hours per frame
		case '-':
			// Decrement by 1 until speed is 1 hour per frame
			if (hourSpeed > 1){
				hourSpeed--;
			} else {
				// Don't go below 0.1 hour per frame
				if (hourSpeed > 0.1){
					// Decrement by 0.1 hours when going below 1 hour per frame
					hourSpeed -= 0.1;
				}
			}
		break;

		// Pause animation
        case 'P':
		case 'p':
			pauseFlag = !pauseFlag;
		break;

		// Reset scene to original position
		case 'R':
		case 'r':
			xRotate = 35.0;
			yRotate = 0.0;
			xTranslate = 0.0;
			zTranslate = -100.0;
		break;

		//Toggle lighting
		case 'L':
		case 'l':
			lightFlag = !lightFlag;
			if (lightFlag){
				glEnable(GL_LIGHTING);
			} else {
				glDisable(GL_LIGHTING);
			}
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

		// toggle velocity travel
		case 'V':
		case 'v':
			velocityFlag = !velocityFlag;
			if (velocityFlag){
				xVelocity = 0;
				yVelocity = 0;
				zVelocity = 0;
			}
		break;

        // Set drawing modes 1 -> wireframe, 2 -> flat, 3 -> smooth, 4 ->image
        case '0':
            xVelocity = 0;
            yVelocity = 0;
            zVelocity = 0;
        break;
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
			xRotate++;
		break;

		// Rotate clockwise about the x axis
        case GLUT_KEY_DOWN:
			xRotate--;
		break;

		// Rotate counter clockwise about the y axis
        case GLUT_KEY_RIGHT:
			yRotate++;
		break;

		// Rotate clockwise about the y axis
        case GLUT_KEY_LEFT:
			yRotate--;
		break;

	}
}

// void passiveMouseFunc(int x, int y){
// 	int X = x - ( width / 2.0 );
// 	int Y = height - y - ( height / 2.0 );
// 	for (auto& m : moonMap){
// 		float radius = m.second->getRadius();
// 		float theta = 
// 		float xDir = cos()
// sin(θ) = opposite / hypotenuse
// Cosine Function:
// ...cah...
// cos(θ) = adjacent / hypotenuse


// 		if (m.second->getName() != "Sun"){
// 			drawPlanet(p.second, false);
// 		}
// }