extern bool solidFlag, pauseFlag, velocityFlag, moonLabelFlag, bodyLabelFlag, lightFlag, texFlag, smoothFlag;

extern float hourSpeed, xVelocity, yVelocity, zVelocity, xTranslate, yTranslate, zTranslate, xRotate, yRotate;
extern string relative;

float sunColor[3] = {1.0, 1.0, 0.3};
float mercuryColor[3] = {0.8, 0.5, 0.5};
float venusColor[3] = {0.6, 0.8, 0.3};
float earthColor[3] = {0.1, 0.4, .85};
float marsColor[3] = {1.0, 0.2, 0.2};
float jupiterColor[3] = {0.54, 0.165, 0.165};
float saturnColor[3] = {0.5, 0.7, 0.0};
float uranusColor[3] = {0.2, 1.0, 0.6};
float neptuneColor[3] = {0.2, 0.4, 0.8};
float moonColor[3] = {0.4, 0.4, 0.5};

map<string,planet*> planetMap;
map<string,planet*> moonMap;
map<int,planet*> asteroidBelt;
map<string,ring*> ringMap;
// map<string,planet*> textureRings;
// map<string,Image_s> texImgs;


void setTexImage (){
    int numR, numC;
    unsigned char *image;
    for (auto& p: planetMap){
        if (p.second->getImage().img == NULL) {

            if(LoadBmpFile(p.second->getName(), numR, numC, image)) {
                p.second->setImage({numR, numC, image});
            } else {
                p.second->setImage({0, 0, nullptr});
            }

        }
    }

    Image_s texImage = {0, 0, nullptr};
    if(LoadBmpFile("Moon", numR, numC, image)) {
        texImage = {numR, numC, image};
    }

    for (auto& m: moonMap){
        m.second->setImage(texImage);
    }

    if(LoadBmpFile("SaturnRings", numR, numC, image)) {
        texImage = {numR, numC, image};
        ringMap.at("Saturn")->setImage(texImage);
    }
}

void setNextFrame(){
    // Move bodies if not paused
    if (!pauseFlag){
        // Move planets ahead by hour speed
        for (auto& p: planetMap)
            p.second->step(hourSpeed);
        // Move satellites ahead by hour speed
        for (auto& m: moonMap)
            m.second->step(hourSpeed);
    }
    // increment x,y,z by corresponding velocity values
    if (velocityFlag){
        xTranslate += xVelocity;
        yTranslate += yVelocity;
        zTranslate += zVelocity;
    }
}

void setPlanets(){
    Image_s nullImage = {0, 0, NULL};
    vector<string> satellites = {};
    vector<string> rings = {};

    planetMap.emplace("Sun", new planet ("Sun", "", 696000/10, 0, 0, 25, 0, 7.25, 0, sunColor, nullImage, satellites, rings));
    planetMap.emplace("Mercury", new planet ("Mercury", "Sun", 2439, 58000000, 88, 1416, 3.38, 2.04, 0.68, mercuryColor, nullImage, satellites, rings));
    planetMap.emplace("Venus", new planet ("Venus", "Sun", 6052, 108000000, 225, 5832, 3.86, 177.36, 0.9, venusColor, nullImage, satellites, rings));

    satellites = {"Luna"};
    planetMap.emplace("Earth", new planet ("Earth", "Sun", 6378, 150000000, 365, 24, 7.155, 23.4392811, 0.306, earthColor, nullImage, satellites, rings));

    satellites = {"Deimos", "Phobos"};
    planetMap.emplace("Mars", new planet ("Mars", "Sun", 3394, 228000000, 687, 24.6, 5.65, 25.19, 0.25, marsColor, nullImage, satellites, rings));

    satellites = {"Adrastea", "Amalthea", "Callisto", "Europa", "Ganymede", "Io", "Metis", "Thebe"};
    planetMap.emplace("Jupiter", new planet ("Jupiter", "Sun", 71398, 779000000, 4332, 9.8, 6.09, 3.13, 0.343, jupiterColor, nullImage, satellites, rings));

    satellites = {"Dione", "Enceladus", "Iapetus", "Mimas", "Rhea", "Tethys", "Titan"};
    rings = {"Saturn"};
    planetMap.emplace("Saturn", new planet ("Saturn", "Sun", 60270, 1424000000, 10585, 10.7, 5.51, 26.73, 0.342, saturnColor, nullImage, satellites, rings));

    satellites = {"Ariel", "Miranda", "Oberon", "Titania", "Umbriel"};
    planetMap.emplace("Uranus", new planet ("Uranus", "Sun", 25550, 2867000000, 30682, 15.5, 6.48, 97.77, 0.3, uranusColor, nullImage, satellites, rings));

    satellites = {"Triton"};
    planetMap.emplace("Neptune", new planet ("Neptune", "Sun", 24750, 4492000000, 60195, 15.8, 6.43, 28.32, 0.290, neptuneColor, nullImage, satellites, rings));
}

void setMoons(){
    Image_s nullImage = {0, 0, NULL};
    vector<string> satellites = {};
    vector<string> rings = {};

    moonMap.emplace("Luna", new planet("Luna", "Earth",1738, 384400, 27.322, 27.322, 5.145, 6.687, 0.136, moonColor, nullImage, satellites, rings));
    
    moonMap.emplace("Deimos", new planet("Deimos", "Mars",4, 23460, 1.263, 30.312, 0.93, 0, 0.068, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Phobos", new planet("Phobos", "Mars",280, 9270, 0.319, 7.656, 1.093, 0, 0.071, moonColor, nullImage, satellites, rings));
    
    moonMap.emplace("Adrastea", new planet("Adrastea", "Jupiter", 208, 128980, 0.298, 7.152, 0.03, 0, 0.1, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Amalthea", new planet("Amalthea", "Jupiter", 83.5, 181300, 0.498, 11.952, 0.374, 0, 0.09, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Callisto", new planet("Callisto", "Jupiter", 2400, 1883000, 16.689, 400.536, 0.205, 0, 0.22, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Europa", new planet("Europa", "Jupiter", 1563, 670900, 3.551, 85, 0.471, 0.1, 0.67, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Ganymede", new planet("Ganymede", "Jupiter", 2638, 1070000, 7.155, 171.72, 0.204, 0.33, 0.43, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Io", new planet("Io", "Jupiter", 1814.5, 421600, 1.769, 42.456, 0.05, 0, 0.63, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Metis", new planet("Metis", "Jupiter", 20, 127960, 0.295, 7.08, 0.06, 0, 0.061, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Thebe", new planet("Thebe", "Jupiter", 50, 221900, 0.675, 16.2, 1.076, 0, 0.047, moonColor, nullImage, satellites, rings));
    
    moonMap.emplace("Dione", new planet("Dione", "Saturn",561.4, 377400, 2.737, 65.68596, 0.019, 0, 0.998, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Enceladus", new planet("Enceladus", "Saturn",249, 238020, 1.37, 32.885232, 0.019, 0, 0.99, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Iapetus", new planet("Iapetus", "Saturn",718, 3561300, 79.3215, 1903.716, 15.47, 0, 0.225, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Mimas", new planet("Mimas", "Saturn",199, 185520, 0.942, 22.608, 1.574, 0, 0.962,moonColor, nullImage, satellites, rings));
    moonMap.emplace("Rhea", new planet("Rhea", "Saturn",764, 527040, 4.518, 108.437088, 0.345, 0, 0.949, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Tethys", new planet("Tethys", "Saturn",530, 294660, 1.888, 45.307248, 1.12, 0, 0.8, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Titan", new planet("Titan", "Saturn",2575, 1221850, 15.945, 382.68, 0.34854, 0, 0.22, moonColor, nullImage, satellites, rings));

    moonMap.emplace("Ariel", new planet("Ariel", "Uranus",580, 191240, 2.52, 60.48, 0.260, 0, 0.23, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Miranda", new planet("Miranda", "Uranus",236, 129780, 1.414, 33.936, 4.232, 0, 0.32, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Oberon", new planet("Oberon", "Uranus",763, 582600, 13.463, 323.112, 0.058, 0, 0.14, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Titania", new planet("Titania", "Uranus",789, 435840, 8.706, 208.944, 0.340 , 0, 0.17, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Umbriel", new planet("Umbriel", "Uranus",595, 265970, 4.144, 99.456, 0.128, 0, 0.10, moonColor, nullImage, satellites, rings));

    moonMap.emplace("Triton", new planet("Triton", "Neptune",1352.5, 354800, 5.877, 141.048, 156.885, 0, 0.76, moonColor, nullImage, satellites, rings));
}

void setAstroidBelt() {
    // We're going to generate around 200 asteroids becasue those are the larger (i.e. over 100km radius)
    // ones and there is no consensus on exactly how many there are
    srand(0);
    int numAsteroids = 190 + (rand() % 20);
    float days = 0;
    Image_s nullImage = {0, 0, NULL};
    vector<string> satellites = {};
    vector<string> rings = {};
    float incline = 0;
    float tilt = 0;
    float albedo = 0;

    for (int i = 0; i < numAsteroids; ++i)
    {
        // Random radius near 100km
        float radius = 90 + (rand() % 20);

        // Random distance near 1/2 way between jupiter and mars
        float dist = 228000000;
        // cout << dist << endl;

        // Random number of days to orbit
        days += (rand() % 300);

        // Random number of hours per rotation
        float hours = (rand() % 25);

        // No names, they all orbit the sun, colours & images are the same as Earth's moon, no satellites and no rings
        asteroidBelt.emplace(i, new planet("", "Sun", radius, dist, days, hours, incline, tilt, albedo, moonColor, nullImage, satellites, rings));
    }
}

void setRings(){
    Image_s nullImage = {0, 0, NULL};

    float ringColor[] = {1, 1, 1};
    ringMap.emplace("Saturn", new ring ("SaturnRings", "Saturn", 67270, 140270, 0, 10.2, 0.342, ringColor, nullImage));
}

/**Set Functions For View Controls**/
void setDrawMode(Mode mode){
    switch (mode) {
        case wire:
            solidFlag = false;
            smoothFlag = false;
            texFlag = false;
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glDisable(GL_TEXTURE_2D);
        break;

        case flat:
            solidFlag = true;
            smoothFlag = false;
            texFlag = false;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glShadeModel( GL_FLAT );
            glDisable(GL_TEXTURE_2D);
        break;

        case smooth:
            solidFlag = true;
            smoothFlag = true;
            texFlag = false;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glShadeModel( GL_SMOOTH );
            glDisable(GL_TEXTURE_2D);
        break;

        case image:
            solidFlag = true;
            smoothFlag = true;
            texFlag = true;
            glShadeModel( GL_SMOOTH );
            glEnable(GL_TEXTURE_2D);
            setTexImage();
        break;
    }
}

void resetView(){
    xRotate = 0.0;
    yRotate = 0.0;
    xTranslate = 0.0;
    zTranslate = -200.0;
}

void setVelocity(int x, int y, int z){
}

void setRelativeBody(string name){
    relative = name;
    xTranslate = 0;
    yTranslate = 0;
    zTranslate = -200;
}


/**Set Functions for Drawing Bodies**/
void setMaterials(GLUquadric* quad, float color[], float albedo, bool emiss){
    gluQuadricDrawStyle( quad, GLU_FILL );
    // set normals for object based on draw mode
    if (solidFlag) {
        if (smoothFlag){
            gluQuadricNormals(quad, GLU_SMOOTH);
        } else {
            gluQuadricNormals(quad, GLU_FLAT);
        }
    } else {
        gluQuadricNormals(quad, GLU_NONE);
    }

    GLfloat ambient[4] = {0, 0, 0, 1.0};
    GLfloat diffuse[4] = {0, 0, 0, 1.0};
    GLfloat specular[4] = {0, 0, 0, 1.0};

    // set texture coordinates based  texture flag
    if (texFlag){
        gluQuadricTexture(quad, GLU_TRUE);
        // Set material property values
        for (int i = 0; i < 3; i++){
            ambient[i] = 0.4;
            diffuse[i] = albedo;
            specular[i] = 0.8;
        }

        } else {
            gluQuadricTexture(quad, GLU_FALSE);
            for (int i = 0; i < 3; i++){
                ambient[i] = color[i] * 0.6;
                diffuse[i] = color[i] * albedo;
                specular[i] = color[i];
            }
        }

	    GLfloat shininess = 3.5;
	    GLfloat emission[4];

	    // Set material properties
	    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambient );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular );
	    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shininess );
        // Set emissivity, if emiss flag is true, emissivity to body colors, else set to zeros
        if (emiss) {
            GLfloat emission[] = { (float)(color[0]), (float)(color[1]), (float)(color[2]), 1.0 }; 
            glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emission );
        } else {
            GLfloat emission[] = { 0.0, 0.0, 0.0, 1.0};
            glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emission );
        }
}
void setTexture(planet* body){
    Image_s texImg = body->getImage();
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texImg.nRows, texImg.nCols, GL_RGB, GL_UNSIGNED_BYTE, texImg.img);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );     // or GL_CLAMP
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );     // or GL_CLAMP
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glEnable(GL_TEXTURE_2D);
}
void setTexture(ring* body){
    Image_s texImg = body->getImage();
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texImg.nRows, texImg.nCols, GL_RGB, GL_UNSIGNED_BYTE, texImg.img);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );     // or GL_CLAMP
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT );     // or GL_CLAMP
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glEnable(GL_TEXTURE_2D);
}

/**Set Functions For Dropdown Menu**/
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

void setPlanetSelectSubmenuOption(int option) {
    switch (option) {
    // Planets in order
    // "Marys Virgin Explanation Made Joseph 
    // Suspect Upstairs Neighbour"
    // 
    // Source: xkcd.com/992
    case 0:
        setRelativeBody("Sun");
    break;

    case 1:
        setRelativeBody("Mercury");
    break;

    case 2:
        setRelativeBody("Venus");
    break;

    case 3:
        setRelativeBody("Earth");
    break;

    case 4:
        setRelativeBody("Mars");
    break;

    case 5:
        setRelativeBody("Jupiter");
    break;

    case 6:
        setRelativeBody("Saturn");
    break;

    case 7:
        setRelativeBody("Uranus");
    break;

    case 8:
        setRelativeBody("Neptune");
    break;
    }

    glutPostRedisplay();
}

void setMenuOption(int option) {
    switch (option) {
    case 0: // Toggle lighting
        lightFlag = !lightFlag;
        (lightFlag) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
    break;

    case 1: // Toggle planatary labels
        bodyLabelFlag = !bodyLabelFlag;
    break;

    case 2: // Toggle moon labels
        moonLabelFlag = !moonLabelFlag;
    break;

    case 3: // Reset position
        resetView();
    break;

    case 4: // Info Screen
    break;

    case 5: // Exit
        exit(0);
    break;
    }

    glutPostRedisplay();
}
