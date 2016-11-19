/******************************************************************************
 * File: initSystem.h
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description: This file contains functions with hard-coded initialization values
 * for the solar system's bodies and their associated texture map images.
 *
 ******************************************************************************/

// These std::maps tie a string for the planets name to
// a planet data structure
map<string, planet*> planetMap;
map<string, planet*> moonMap;
map<string, ring*> ringMap;
// Empty string vector for bodies with no moons or rings
vector<string> nullVector = {};


// Set colors for bodies
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

/* setPlanets()
 *
 * This function creates a new planet object
 * for each planet in the solar system and
 * places them into the planetMap vector.
 *
 */
void setPlanets() {
    // Earth's moon will be the first to be assigned
    vector<string> satellites = {"Luna"};
    // The astroid belt will be considered the Sun's ring
    vector<string> rings = {"AsteroidBelt"};

    planetMap.emplace("Sun", new planet ("Sun", "", 696000 / 10, 0, 0, 25, 0, 7.25, 0, sunColor, nullImage, nullVector, rings));
    planetMap.emplace("Mercury", new planet ("Mercury", "Sun", 2439, 58000000, 88, 1416, 3.38, 2.04, 0.68, mercuryColor, nullImage, nullVector, nullVector));
    planetMap.emplace("Venus", new planet ("Venus", "Sun", 6052, 108000000, 225, 5832, 3.86, 177.36, 0.9, venusColor, nullImage, nullVector, nullVector));

    planetMap.emplace("Earth", new planet ("Earth", "Sun", 6378, 150000000, 365, 24, 7.155, 23.4392811, 0.306, earthColor, nullImage, satellites, nullVector));

    // Update satellites to Mars' moons
    satellites = {"Phobos"};
    planetMap.emplace("Mars", new planet ("Mars", "Sun", 3394, 228000000, 687, 24.6, 5.65, 25.19, 0.25, marsColor, nullImage, satellites, nullVector));

    // Update satellites to Jupiter's moons
    satellites = {"Callisto", "Europa", "Ganymede", "Io"};
    planetMap.emplace("Jupiter", new planet ("Jupiter", "Sun", 71398, 779000000, 4332, 9.8, 6.09, 3.13, 0.343, jupiterColor, nullImage, satellites, nullVector));

    // Update satellites to Saturn's moons
    satellites = {"Dione", "Enceladus", "Iapetus", "Mimas", "Rhea", "Tethys", "Titan"};
    // Update rings to saturnrings
    rings = {"SaturnRings"};
    planetMap.emplace("Saturn", new planet ("Saturn", "Sun", 60270, 1424000000, 10585, 10.7, 5.51, 26.73, 0.342, saturnColor, nullImage, satellites, rings));

    // Update satellites to Uranus' moons
    satellites = {"Ariel", "Miranda", "Oberon", "Titania", "Umbriel"};
    planetMap.emplace("Uranus", new planet ("Uranus", "Sun", 25550, 2867000000, 30682, 15.5, 6.48, 97.77, 0.3, uranusColor, nullImage, satellites, nullVector));

    // Update satellites to Neptune's largest moon
    satellites = {"Triton"};
    planetMap.emplace("Neptune", new planet ("Neptune", "Sun", 24750, 4492000000, 60195, 15.8, 6.43, 28.32, 0.290, neptuneColor, nullImage, satellites, nullVector));
}

/* setMoons()
 *
 * This function creates a new planet object
 * for each moon which will be drawn and
 * places them into the moonMap vector.
 *
 */
void setMoons() {

    // Earths moon
    moonMap.emplace("Luna", new planet("Luna", "Earth", 1738, 384400, 27.322, 27.322, 5.145, 6.687, 0.136, moonColor, nullImage, nullVector, nullVector));

    moonMap.emplace("Phobos", new planet("Phobos", "Mars", 280, 9270, 0.319, 7.656, 1.093, 0, 0.071, moonColor, nullImage, nullVector, nullVector));

    moonMap.emplace("Callisto", new planet("Callisto", "Jupiter", 2400, 1883000, 16.689, 400.536, 0.205, 0, 0.22, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Europa", new planet("Europa", "Jupiter", 1563, 670900, 3.551, 85, 0.471, 0.1, 0.67, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Ganymede", new planet("Ganymede", "Jupiter", 2638, 1070000, 7.155, 171.72, 0.204, 0.33, 0.43, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Io", new planet("Io", "Jupiter", 1814.5, 421600, 1.769, 42.456, 0.05, 0, 0.63, moonColor, nullImage, nullVector, nullVector));

    moonMap.emplace("Dione", new planet("Dione", "Saturn", 561.4, 377400, 2.737, 65.68596, 0.019, 0, 0.998, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Enceladus", new planet("Enceladus", "Saturn", 249, 238020, 1.37, 32.885232, 0.019, 0, 0.99, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Iapetus", new planet("Iapetus", "Saturn", 718, 3561300, 79.3215, 1903.716, 15.47, 0, 0.225, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Mimas", new planet("Mimas", "Saturn", 199, 185520, 0.942, 22.608, 1.574, 0, 0.962, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Rhea", new planet("Rhea", "Saturn", 764, 527040, 4.518, 108.437088, 0.345, 0, 0.949, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Tethys", new planet("Tethys", "Saturn", 530, 294660, 1.888, 45.307248, 1.12, 0, 0.8, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Titan", new planet("Titan", "Saturn", 2575, 1221850, 15.945, 382.68, 0.34854, 0, 0.22, moonColor, nullImage, nullVector, nullVector));

    moonMap.emplace("Ariel", new planet("Ariel", "Uranus", 580, 191240, 2.52, 60.48, 0.260, 0, 0.23, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Miranda", new planet("Miranda", "Uranus", 236, 129780, 1.414, 33.936, 4.232, 0, 0.32, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Oberon", new planet("Oberon", "Uranus", 763, 582600, 13.463, 323.112, 0.058, 0, 0.14, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Titania", new planet("Titania", "Uranus", 789, 435840, 8.706, 208.944, 0.340 , 0, 0.17, moonColor, nullImage, nullVector, nullVector));
    moonMap.emplace("Umbriel", new planet("Umbriel", "Uranus", 595, 265970, 4.144, 99.456, 0.128, 0, 0.10, moonColor, nullImage, nullVector, nullVector));

    moonMap.emplace("Triton", new planet("Triton", "Neptune", 1352.5, 354800, 5.877, 141.048, 156.885, 0, 0.76, moonColor, nullImage, nullVector, nullVector));
}

/* setRings()
 *
 * This function creates a new ring object
 * for each set of rings (or in a certin case,
 * astraoid belt) which will be drawn and
 * places them into the ringMap vector.
 *
 */
void setRings() {
    float ringColor[] = {1, 1, 1};
    ringMap.emplace("SaturnRings", new ring ("SaturnRings", "Saturn", 67270, 140270, 0, 10.2, 0.342, ringColor, nullImage));
    float beltColor[] = {0.3, 0.3, 0.3};
    ringMap.emplace("AsteroidBelt", new ring ("AsteroidBelt", "Sun", 1100000, 1600000, 0, 0, 0.342, beltColor, nullImage));
}

/* storeTexture()
 *
 * This is an overloaded function that takes a
 * planet object and image data array. The iageis
 * used to create a mipmap which is then bound to
 * a name stored in the global variable texNames. The
 * associated index is stored in the planet's object.
 *
 * Parameters:
 *            int  i      - The index into the texNames variable
 *            int  numR   - The number of rows in the image
 *            int  numC   - The number of columns in the image
 *  unsigned char *image  - The data array for the image
 *         planet *body   - The object associated with the texture map
 *
 * Returns:
 *          int - The incrementation of the parameter i
 */
int storeTexture( int i, int numR, int numC, unsigned char *image, planet *body) {
    // Build the mipmap
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, numC, numR, GL_RGB, GL_UNSIGNED_BYTE, image);
    // Bind the texture to a name
    glBindTexture(GL_TEXTURE_2D, texNames[i]);
    // Store the index of the bound texture name
    body->setImage(i);
    return i + 1;
}
/* storeTexture()
 *
 * This is an overloaded function that takes a ring
 * object and image data array. The iageis used to
 * create a mipmap which is then bound to a name
 *  stored in the global variable texNames. The
 * associated index is stored in the ring's object.
 *
 * Parameters:
 *            int  i      - The index into the texNames variable
 *            int  numR   - The number of rows in the image
 *            int  numC   - The number of columns in the image
 *  unsigned char *image  - The data array for the image
 *           ring *body   - The object associated with the texture map
 *
 * Returns:
 *          int - The incrementation of the parameter i
 */
int storeTexture(int i, int numR, int numC, unsigned char *image, ring *body) {
    // Build the mipmap
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, numC, numR, GL_RGB, GL_UNSIGNED_BYTE, image);
    // Bind the texture to a name
    glBindTexture(GL_TEXTURE_2D, texNames[i]);
    // Store the index of the bound texture name
    body->setImage(i);
    return i + 1;

}
/* setRings()
 *
 * This function reads in a bmp file for each
 * body to be drawn and uses it to create a
 * mipmap. The mipmaps are then bound to a
 * name stored in texNames, with the associated
 * index being stored in the body's object.
 *
 */
void setTexImage () {
    // Variables to store image data in
    int numR, numC;
    unsigned char *image;
    // Generate texture names for each body
    glGenTextures(32, texNames);
    int i = 0;
    // For each planet, read out the associated file with the same name
    // e.g. "Earth" -> "earth.bmp"
    for (auto& p : planetMap) {
        if (LoadBmpFile(p.second->getName(), numR, numC, image)) {
            i = storeTexture(i, numR, numC, image, p.second);
        } else {
            p.second->setImage(-1);
        }
    }
    // For each moon, read out the associated file with the same name
    for (auto& m : moonMap) {
        if (LoadBmpFile(m.second->getName(), numR, numC, image)) {
            i = storeTexture(i, numR, numC, image, m.second);
        } else {
            m.second->setImage(-1);
        }
    }

    // For each ring system, read out the associated file with the same name
    for (auto& r : ringMap) {
        if (LoadBmpFile(r.second->getName(), numR, numC, image)) {
            i = storeTexture(i, numR, numC, image, r.second);
        } else {
            r.second->setImage(-1);
        }
    }
}
