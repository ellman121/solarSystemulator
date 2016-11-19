/******************************************************************************
 * File: menu.h
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description: This file contains the code for creating and handling the
 * right click menu.
 *
 ******************************************************************************/

/* setSpeedSelectSubmenuOption()
 *
 * This submenu allows the user to set the speed of simulation
 *
 * Parameters:
 *      int option - The option chosen by the user
 */
void setSpeedSelectSubmenuOption(int option) {
    switch (option) {
    case 0: // Minimum speed
        hourSpeed = 0.1;
        break;

    case 1: // 1 Hour/Step
        hourSpeed = 1;
        break;

    case 2: // 12 Hour/Step
        hourSpeed = 12;
        break;

    case 3: // 1 Day/Step
        hourSpeed = 24;
        break;

    case 4: // 2 Day/Step
        hourSpeed = 48;
        break;
    }

    glutPostRedisplay();
}

/* setSpeedSelectSubmenuOption()
 *
 * This submenu allows the user to set the
 * planet to be the relative position the system
 * is viewed from. When a relative planet is
 * selected, all view positional variables are
 * reset.
 *
 * Parameters:
 *      int option - The option chosen by the user
 */
void setPlanetSelectSubmenuOption(int option) {
    switch (option) {
    // Planets in order
    // "Marys Virgin Explanation Made Joseph
    // Suspect Upstairs Neighbour"
    //
    // Source: xkcd.com/992
    case 0:
        relative =
            relative = "Sun";
        setView();
        break;

    case 1:
        relative = "Mercury";
        setView();
        break;

    case 2:
        relative = "Venus";
        setView();
        break;

    case 3:
        relative = "Earth";
        setView();
        break;

    case 4:
        relative = "Mars";
        setView();
        break;

    case 5:
        relative = "Jupiter";
        setView();
        break;

    case 6:
        relative = "Saturn";
        setView();
        break;

    case 7:
        relative = "Uranus";
        setView();
        break;

    case 8:
        relative = "Neptune";
        setView();
        break;
    }

    glutPostRedisplay();
}

/* setMenuOption()
 *
 * This menu allows the user to perform certain actions with
 * the mouse instead of keybaord shortcuts
 *
 * Parameters:
 *      int option - The option chosen by the user
 */
void setMenuOption(int option) {
    switch (option) {
    case 0: // Toggle lighting
        lightFlag = !lightFlag;
        (lightFlag) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
        break;

    case 1: // Toggle orbital paths
        orbitFlag = !orbitFlag;
        break;

    case 2: // Toggle planatary labels
        bodyLabelFlag = !bodyLabelFlag;
        break;

    case 3: // Toggle moon labels
        moonLabelFlag = !moonLabelFlag;
        break;

    case 4: // Reset position
        setView();
        break;

    case 5: // Info Screen
        infoFlag = !infoFlag;
        break;

    case 6: // Exit
        exit(0);
        break;
    }

    glutPostRedisplay();
}
