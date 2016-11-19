/******************************************************************************
 * File: drawInfo.h
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description:
 *		Contains routines to draw informative elements of the program.
 ******************************************************************************/
extern void initLighting();

/* drawText()
 *
 * Draw some text at a given location with a given font
 * This is used primarily in the drawInfoScreen()
 * function.
 *
 * Parameters:
 *		char* msg  - The message to be drawn
 *		float x    - The x location to draw at
 *		float y    - The y locatino to draw at
 * 		void* font - The font to use
 */
void drawText(const char *msg, float x, float y, void *font) {
	glRasterPos2f(x, y);
	glutBitmapString(font, (const unsigned char *)msg);
}

/* drawStatus()
 *
 * Draw the current motion status in the top left corner  of the screen.
 * This information includes the current time progression per frame,
 * as well as travel velocities.
 */
void drawStatus() {
	glPushMatrix();
	//Set matrix mode
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	// Change to ortho view for 2D drawing
	glOrtho(-1, 1, -1, 1, 0.0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1, 1, 1);

	// Create text variables
	char velocity [40];
	char speed[40];

	// Set base draw mode for status (we don't have to undo this as the end since we're not drawing more planets)
	if (drawMode != 1) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glDisable(GL_TEXTURE_2D);

	}
	// Set material property values to max
	GLfloat labelMat[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, labelMat );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, labelMat );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, labelMat );

	// Display current draw mode
	drawText("Draw Mode", -0.94, 0.95, GLUT_BITMAP_HELVETICA_10);
	drawText("--------------", -0.98, 0.93, GLUT_BITMAP_HELVETICA_10);
	switch (drawMode) {
	case 1:
		drawText("Wire Frame", -0.94, 0.90, GLUT_BITMAP_HELVETICA_10);
		break;
	case 2:
		drawText("Flat Shading", -0.95, 0.90, GLUT_BITMAP_HELVETICA_10);
		break;
	case 3:
		drawText("Smooth Shading", -0.97, 0.90, GLUT_BITMAP_HELVETICA_10);
		break;
	case 4:
		drawText("Texture Mapped", -0.96, 0.90, GLUT_BITMAP_HELVETICA_10);
		break;
	}

	// Display lighting toggle
	drawText("Lighting", -0.93, 0.85, GLUT_BITMAP_HELVETICA_10);
	drawText("--------------", -0.98, 0.83, GLUT_BITMAP_HELVETICA_10);
	(lightFlag) ? drawText("ON", -0.92, 0.80, GLUT_BITMAP_HELVETICA_10) : drawText("OFF", -0.92, 0.80, GLUT_BITMAP_HELVETICA_10);

	// Display speed and velocity
	drawText("Time Speed", -0.95, 0.75, GLUT_BITMAP_HELVETICA_10);
	drawText("--------------", -0.98, 0.73, GLUT_BITMAP_HELVETICA_10);
	// Set speed text
	(pauseFlag) ? sprintf(speed, "      PAUSED") : sprintf(speed, "%.1f  HRS / FRAME", hourSpeed);
	drawText(speed, -0.97, 0.70, GLUT_BITMAP_HELVETICA_10);

	drawText("Velocity", -0.93, 0.65, GLUT_BITMAP_HELVETICA_10);
	drawText("--------------", -0.98, 0.63, GLUT_BITMAP_HELVETICA_10);

	// Print velocities if velocity is engaged
	if (velocityFlag) {
		// Set and draw velocity string
		sprintf(velocity, "<%d, %d, %d>", (int)(-1 * xVelocity), (int)(-1 * yVelocity), (int)(zVelocity));
		drawText(velocity, -0.97, 0.60, GLUT_BITMAP_HELVETICA_10);

	} else {
		// Let user know that velocity is off
		drawText("OFF", -0.93, 0.60, GLUT_BITMAP_HELVETICA_10);
	}

	glPopMatrix();
}

/* drawInfoScreen()
 *
 * Draw the info screen to indicate program controls
 *
 */
void drawInfoScreen() {
	glPushMatrix();
	// Reset lighting
	setInfoLighting();

	// Variables for the start of text columns
	float leftCol1 = -0.70;
	float leftCol2 = -0.45;
	float rightCol1 = 0.20;
	float rightCol2 = 0.35;



	//Set ortho view
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	// change to ortho view for 2D drawing
	glOrtho(-1, 1, -1, 1, 0.0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0, 1.0, 1.0);

	// Increase line boldness
	glLineWidth(1.5);

	// Title
	drawText("Solar System Simulator", -0.25, 0.80, GLUT_BITMAP_HELVETICA_18);


	// Left side first header
	drawText("Mouse Controls", leftCol1 + 0.1, 0.65, GLUT_BITMAP_HELVETICA_18);

	// Mouse control options
	drawText("Right Click", leftCol1, 0.60, GLUT_BITMAP_HELVETICA_12);
	drawText("Left Click & Drag", leftCol1, 0.55, GLUT_BITMAP_HELVETICA_12);
	drawText("Wheel Up", leftCol1, 0.50, GLUT_BITMAP_HELVETICA_12);
	drawText("Wheel Down", leftCol1, 0.45, GLUT_BITMAP_HELVETICA_12);

	// Mouse control effects
	drawText("Display Menu", leftCol2, 0.60, GLUT_BITMAP_HELVETICA_12);
	drawText("Rotate Scene", leftCol2, 0.55, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Forward", leftCol2, 0.50, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Backward", leftCol2, 0.45, GLUT_BITMAP_HELVETICA_12);

	// Left side second header
	drawText("Motion Control Keys", leftCol1 + 0.1, 0.35, GLUT_BITMAP_HELVETICA_18);

	// Mostion control key options
	drawText("Arrow Keys", leftCol1, 0.30, GLUT_BITMAP_HELVETICA_12);
	drawText("+", leftCol1, 0.25, GLUT_BITMAP_HELVETICA_12);
	drawText("-", leftCol1, 0.20, GLUT_BITMAP_HELVETICA_12);
	drawText("F", leftCol1, 0.15, GLUT_BITMAP_HELVETICA_12);
	drawText("P", leftCol1, 0.10, GLUT_BITMAP_HELVETICA_12);
	drawText("W", leftCol1, 0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("S", leftCol1, 0.0, GLUT_BITMAP_HELVETICA_12);
	drawText("A", leftCol1, -0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("D", leftCol1, -0.10, GLUT_BITMAP_HELVETICA_12);
	drawText("Q", leftCol1, -0.15, GLUT_BITMAP_HELVETICA_12);
	drawText("Z", leftCol1, -0.20, GLUT_BITMAP_HELVETICA_12);

	// Motion control key effects
	drawText("Rotate Scene", leftCol2, 0.30, GLUT_BITMAP_HELVETICA_12);
	drawText("Increase Speed", leftCol2, 0.25, GLUT_BITMAP_HELVETICA_12);
	drawText("Decrease Speed", leftCol2, 0.20, GLUT_BITMAP_HELVETICA_12);
	drawText("Progress One Frame", leftCol2, 0.15, GLUT_BITMAP_HELVETICA_12);
	drawText("Pause Time", leftCol2, 0.10, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Forward", leftCol2, 0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Backward", leftCol2, 0.0, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Left", leftCol2, -0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Right", leftCol2, -0.10, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Up", leftCol2, -0.15, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Down", leftCol2, -0.20, GLUT_BITMAP_HELVETICA_12);

	// Right side first header
	drawText("Draw Mode Keys", rightCol1 + 0.1, 0.65, GLUT_BITMAP_HELVETICA_18);

	// Draw mode key options
	drawText("1", rightCol1, 0.60, GLUT_BITMAP_HELVETICA_12);
	drawText("2", rightCol1, 0.55, GLUT_BITMAP_HELVETICA_12);
	drawText("3", rightCol1, 0.50, GLUT_BITMAP_HELVETICA_12);
	drawText("4", rightCol1, 0.45, GLUT_BITMAP_HELVETICA_12);

	// Draw mode key effects
	drawText("Wire Frame", rightCol2, 0.60, GLUT_BITMAP_HELVETICA_12);
	drawText("Solid With Flat Shading", rightCol2, 0.55, GLUT_BITMAP_HELVETICA_12);
	drawText("Solid With Smooth Shading", rightCol2, 0.50, GLUT_BITMAP_HELVETICA_12);
	drawText("Texture Mapped", rightCol2, 0.45, GLUT_BITMAP_HELVETICA_12);

	// Right side second header
	drawText("Toggle Keys", rightCol1 + 0.1, 0.35, GLUT_BITMAP_HELVETICA_18);

	// Toggle key options
	drawText("B", rightCol1, 0.30, GLUT_BITMAP_HELVETICA_12);
	drawText("M", rightCol1, 0.25, GLUT_BITMAP_HELVETICA_12);
	drawText("O", rightCol1, 0.20, GLUT_BITMAP_HELVETICA_12);
	drawText("L", rightCol1, 0.15, GLUT_BITMAP_HELVETICA_12);
	drawText("V", rightCol1, 0.10, GLUT_BITMAP_HELVETICA_12);
	drawText("I", rightCol1, 0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("R", rightCol1, 0.0, GLUT_BITMAP_HELVETICA_12);
	drawText("ESC", rightCol1, -0.05, GLUT_BITMAP_HELVETICA_12);

	// Toggle key effects
	drawText("Display Planet Body Names", rightCol2, 0.30, GLUT_BITMAP_HELVETICA_12);
	drawText("Display Moon Names", rightCol2, 0.25, GLUT_BITMAP_HELVETICA_12);
	drawText("Display Orbital Paths", rightCol2, 0.20, GLUT_BITMAP_HELVETICA_12);
	drawText("Turn Lighting On/Off", rightCol2, 0.15, GLUT_BITMAP_HELVETICA_12);
	drawText("Turn Velocity Travel On/Off", rightCol2, 0.10, GLUT_BITMAP_HELVETICA_12);
	drawText("Display Info Screen", rightCol2, 0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("Reset View To Starting Position", rightCol2, 0.0, GLUT_BITMAP_HELVETICA_12);
	drawText("Exit program", rightCol2, -0.05, GLUT_BITMAP_HELVETICA_12);


	drawText("Press 'I' to close this screen", -0.14, -0.45, GLUT_BITMAP_HELVETICA_18);

	float y1 = -0.60;
	float y2 = -0.65;
	drawText("Draw Mode", -0.80, y1, GLUT_BITMAP_HELVETICA_18);
	drawText("Lighting", -0.30, y1, GLUT_BITMAP_HELVETICA_18);
	drawText("Labels", 0.10, y1, GLUT_BITMAP_HELVETICA_18);
	drawText("Orbit Paths", 0.40, y1, GLUT_BITMAP_HELVETICA_18);

	// Display current draw mode
	switch (drawMode) {
	case 1:
		drawText("Wire Frame", -0.80, y2, GLUT_BITMAP_HELVETICA_12);
		break;
	case 2:
		drawText("Solid with Flat Shading", -0.80, y2, GLUT_BITMAP_HELVETICA_12);
		break;
	case 3:
		drawText("Solid with Smooth Shading", -0.80, y2, GLUT_BITMAP_HELVETICA_12);
		break;
	case 4:
		drawText("Texture Mapped", -0.80, y2, GLUT_BITMAP_HELVETICA_12);
		break;
	}

	(lightFlag) ? drawText("ON", -0.30, y2, GLUT_BITMAP_HELVETICA_12) : drawText("OFF", -0.30, y2, GLUT_BITMAP_HELVETICA_12);

	if (bodyLabelFlag && moonLabelFlag) {
		drawText("Planets & Moons", 0.10, y2, GLUT_BITMAP_HELVETICA_12);
	} else if (bodyLabelFlag) {
		drawText("Planets", 0.10, y2, GLUT_BITMAP_HELVETICA_12);
	} else if (moonLabelFlag) {
		drawText("Moons", 0.10, y2, GLUT_BITMAP_HELVETICA_12);
	} else {
		drawText("None", 0.10, y2, GLUT_BITMAP_HELVETICA_12);
	}
	(orbitFlag) ? drawText("ON", 0.40, y2, GLUT_BITMAP_HELVETICA_12) : drawText("OFF", 0.40, y2, GLUT_BITMAP_HELVETICA_12);



	// Draw vertical separation line
	glBegin(GL_LINES);
	glVertex2f(0.0, 0.75);
	glVertex2f(0.0, -0.35);
	glEnd();

	// Draw horizontal sepatation line
	glBegin(GL_LINES);
	glVertex2f(-0.85, 0.75);
	glVertex2f(0.85, 0.75);
	glEnd();

	glPopMatrix();
}
