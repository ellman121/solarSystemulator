


// displayCallback() handles the animation and the redrawing of the graphics window contents.

// Global things
extern bool infoFlag, pauseFlag, solidFlag, lightFlag;
extern int width, height;
extern float xTranslate, hourSpeed, yTranslate, zTranslate, xRotate , yRotate, zRotate, mouseX, mouseY;
extern map<string,planet*> planetMap;

// displayCallback() handles the animation and the redrawing of the graphics window contents.
void displayCallback( void )
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

		// Move to current view point
		glTranslatef ( xTranslate, yTranslate, zTranslate );

		// Rotate the scene according to user specifications
		glRotatef( xRotate, 1.0, 0.0, 0.0 );
		glRotatef( yRotate, 0.0, 1.0, 0.0 );
		glRotatef( zRotate, 0.0, 0.0, 1.0 );

		// Draw each planet
		for (auto& p: planetMap){
			if (p.second->getName() != "Sun"){
				drawBody(p.second);
			}
		}
		drawSun(planetMap.at("Sun"));
		// Draw satelite bodies
		// for (auto& m: moonMap)
		// 	drawSatelite(m.second);
			
		drawLighSource();
	
		// Draw each planet
		for (auto& p: planetMap){

			drawBodyName(p.second);
		}
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
			xTranslate = (xTranslate + 1 > 600) ? 600 : xTranslate - 1;
		break;

		// Move scene left
        case 'A':
		case 'a':
			xTranslate = (xTranslate - 1 < -600) ? -600 : xTranslate + 1;
		break;

		// Move forward in scene
		case 'W':
		case 'w':
			zTranslate = (zTranslate + 1 > 600) ? 600 : zTranslate + 1;
		break;
		//Move backward in scene
		case 'S':
		case 's':
			zTranslate = (zTranslate - 1 < -600) ? -600 : zTranslate - 1;
		break;

		// Increase hours per frame
		case '+':
			hourSpeed++;
		break;

		// Decrease hours per frame
		case '-':
			hourSpeed--;
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