#include "engine.h"
#include "random"

enum state {start, play, over};
state screen;

// Colors

int lastIntFrame;

Engine::Engine() : keys() {
    this->initWindow();
    this->initShaders();
    this->initShapes();

}

Engine::~Engine() {}

unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(width, height, "engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    // Configure text shader and renderer
    textShader = shaderManager->loadShader("../res/shaders/text.vert", "../res/shaders/text.frag", nullptr, "text");
    fontRenderer = make_unique<FontRenderer>(shaderManager->getShader("text"), "../res/fonts/MxPlus_IBM_BIOS.ttf", 24);

    // Set uniforms
    textShader.setVector2f("vertex", vec4(100, 100, .5, .5));
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);
}


void Engine::initShapes() {
    //inits three dividing lines in the window for the 4 quadrants.
    divCenter = make_unique<Rect>(shapeShader, vec2{width/2,height/2}, vec2{5, height}, color{0, 0, 1, 1}); // blue divider line in the center of the screen
    divLeft = make_unique<Rect>(shapeShader, vec2{width/4,height/2}, vec2{5, height}, color{0, 0, 1, 1}); // blue divider line on the left of the screen
    divRight = make_unique<Rect>(shapeShader, vec2{(width * 3)/4,height/2}, vec2{5, height}, color{0, 0, 1, 1}); // blue divider line on the right of the screen

    // makes a white arrow with a invisible arrow below it. this click arrow will turn a color when an input is pressed.
    // these two arrows are made for each quadrant.
    arrowMarkerQ1 = make_unique<Arrow>(shapeShader, vec2{(width * 1)/8,height/6}, vec2{37.5 , 31.25}, color{1, 1, 1, 1}, 1);
    arrowBaseClickQ1 = make_unique<Arrow>(shapeShader, vec2{(width * 1)/8,height/6}, vec2{50 , 47.25}, color{0, 0, 0, 0.1}, 1);

    arrowMarkerQ2 = make_unique<Arrow>(shapeShader, vec2{(width * 3)/8,height/6}, vec2{37.5 , 31.25}, color{1, 1, 1, 1}, 2);
    arrowBaseClickQ2 = make_unique<Arrow>(shapeShader, vec2{(width * 3)/8,height/6}, vec2{50 , 47.25}, color{0, 0, 0, 0.1}, 2);

    arrowMarkerQ3 = make_unique<Arrow>(shapeShader, vec2{(width * 5)/8,(height/6) - 25}, vec2{37.5 , 31.25}, color{1, 1, 1, 1}, 3);
    arrowBaseClickQ3 = make_unique<Arrow>(shapeShader, vec2{(width * 5)/8,(height/6) - 43}, vec2{50 , 47.25}, color{0, 0, 0, 0.1}, 3);

    arrowMarkerQ4 = make_unique<Arrow>(shapeShader, vec2{(width * 7)/8,height/6}, vec2{37.5 , 31.25}, color{1, 1, 1, 1}, 4);
    arrowBaseClickQ4 = make_unique<Arrow>(shapeShader, vec2{(width * 7)/8 - 6,height/6}, vec2{50 , 47.25}, color{0, 0, 0, 0.1}, 4);



}

void Engine::processInput() {
    glfwPollEvents();

    // Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            keys[key] = true;
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
            keys[key] = false;
    }

    // Close window if escape key is pressed
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

    // Mouse position saved to check for collisions
    glfwGetCursorPos(window, &MouseX, &MouseY);

    // DONE: If we're in the start screen and the user presses s, change screen to play
    if (screen == start) {
        if (keys[GLFW_KEY_S]) {
            screen = play;
        }
    }

    if (screen == play) {
        arrowClickedLastFrame = false;
        // for left (q1) turn click arrow on
        if(keys[GLFW_KEY_LEFT]) {
            // trigger function to check for a scored point
            addPoint("left");
            arrowClickedLastFrame = true;
            arrowBaseClickQ1->setColor(blue);
        } else {
            arrowBaseClickQ1->setColor(vec4{0, 0, 0, 0.1});
        }
        // for down (q2) turn click arrow on
        if(keys[GLFW_KEY_DOWN]){
            // trigger function to check for a scored point
            addPoint("down");
            arrowClickedLastFrame = true;
            arrowBaseClickQ2->setColor(green);
        } else {
            arrowBaseClickQ2->setColor(vec4{0, 0, 0, 0.1});
        }
        //for up (q3) turn click arrow on
        if(keys[GLFW_KEY_UP]){
            // trigger function to check for a scored point
            addPoint("up");
            arrowClickedLastFrame = true;
            arrowBaseClickQ3->setColor(yellow);
        } else {
            arrowBaseClickQ3->setColor(vec4{0, 0, 0, 0.1});
        }
        //for right (q4) turn click arrow on
        if(keys[GLFW_KEY_RIGHT]){
            // trigger function to check for a scored point
            addPoint("right");
            arrowClickedLastFrame = true;
            arrowBaseClickQ4->setColor(red);
        } else {
            arrowBaseClickQ4->setColor(vec4{0, 0, 0, 0.1});
        }
        // if arrow is not clicked, set all dividing lines to there original color.
        if(!arrowClickedLastFrame){
            color clr = {0,0,1,0.5};
            divCenter->setColor(clr);
            divLeft->setColor(clr);
            divRight->setColor(clr);
        }
    }

    // Mouse position is inverted because the origin of the window is in the top left corner
    MouseY = height - MouseY; // Invert y-axis of mouse position
    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    // Save mousePressed for next frame
    mousePressedLastFrame = mousePressed;

}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    int c = currentFrame;

    if(screen == play){

        // spawn an arrow every second or if the speed is fast enough spawn multiple arrows every second
        int c = currentFrame;
        if(currentFrame < (c + 0.25) && currentFrame > (c + 0.249) && speed < -2.5){
            spawnArrow();
        }
        if(currentFrame < (c + 0.40) && currentFrame > (c + 0.39) && speed < -3.5){
            spawnArrow();
        }
        if(currentFrame < (c + 0.90) && currentFrame > (c + 0.89) && totalScore > 150){
            spawnArrow();
        }
        if(c > lastIntFrame){
            spawnArrow();
        }

        // move arrow down the screen when spawned, and check if it is scored
        for(int i = 0; i < arrows.size(); i++){
            arrows[i]->moveY(speed);
            // if arrow is scored, increase score counter by 5.
            if(arrows[i]->getScored()){
                totalScore+= 5;
                if(speed > -4){
                    speed-= 0.08;
                }

                if(totalScore > 500 && speed < -8){
                    speed-= 0.01;
                }
                arrows.erase(arrows.begin() + i);
            }
                // if arrows are not scored and past the screen, it subtracts from the score.
            else if(arrows[i]->getPosY() < 0){
                if(!arrows[i]->getScored()){
                    screen = over;
                }
                arrows.erase(arrows.begin() + i);
            }
        }
        // if the total score gets to 100 user wins!
        lastIntFrame = c;
    }
}

void Engine::render() {
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f); // Set background color to a dark blue.
    glClear(GL_COLOR_BUFFER_BIT);

    // Set shader to use for all shapes
    shapeShader.use();

    // Render differently depending on screen
    int i = 0;
    switch (screen) {
        // render  start screen
        case start: {
            string start = "Press s to start";
            string rules1 = "Press arrow keys to score points. If an";
            string rules2 = "arrow is missed, its game over! as your";
            string rules3 = "score increases so does the pace of the";
            string rules4 = "game, so stay focused!";

            // (12 * message.length()) is the offset to center text.
            // 12 pixels is the width of each character scaled by 1.

            this->fontRenderer->renderText(start, width/2 - (12 * start.length()), height/2, 1, vec3{0, 1, 0});
            this->fontRenderer->renderText(rules1, width/2 - (14 * start.length()), height/2 - 40, 0.50, vec3{1, 1, 1});
            this->fontRenderer->renderText(rules2, width/2 - (14 * start.length()), height/2 - 60, 0.50, vec3{1, 1, 1});
            this->fontRenderer->renderText(rules3, width/2 - (14 * start.length()), height/2 - 80, 0.50, vec3{1, 1, 1});
            this->fontRenderer->renderText(rules4, width/2 - (14 * start.length()), height/2 - 100, 0.50, vec3{1, 1, 1});
            break;
        }
            // render play screen
        case play: {

            // renders divders
            divCenter->setUniforms();
            divCenter->draw();
            divLeft->setUniforms();
            divLeft->draw();
            divRight->setUniforms();
            divRight->draw();


            // renders click arrows first so they appear under marker arrows.
            arrowBaseClickQ1->setUniforms();
            arrowBaseClickQ1->draw();

            arrowBaseClickQ2->setUniforms();
            arrowBaseClickQ2->draw();

            arrowBaseClickQ3->setUniforms();
            arrowBaseClickQ3->draw();

            arrowBaseClickQ4->setUniforms();
            arrowBaseClickQ4->draw();

            //render white arrows that are used to check for score.
            arrowMarkerQ1->setUniforms();
            arrowMarkerQ1->draw();
            arrowMarkerQ2->setUniforms();
            arrowMarkerQ2->draw();
            arrowMarkerQ3->setUniforms();
            arrowMarkerQ3->draw();
            arrowMarkerQ4->setUniforms();
            arrowMarkerQ4->draw();


            // goes through arrows vector to render all spawned arrows.
            for(int i = 0; i < arrows.size(); i++){
                arrows[i]->setUniforms();
                arrows[i]->draw();
            }

            // render current score
            string scoreCounter = "Current score:  " + std::to_string(totalScore);
            if(totalScore >= 500){
                this->fontRenderer->renderText(scoreCounter, width/2 - (12 * scoreCounter.length()), (height/12) - 21, 1, red.vec);
            } else if( totalScore >= 250){
                this->fontRenderer->renderText(scoreCounter, width/2 - (12 * scoreCounter.length()), (height/12) - 21, 1, green.vec);
            } else if( totalScore >= 100){
                this->fontRenderer->renderText(scoreCounter, width/2 - (12 * scoreCounter.length()), (height/12) - 21, 1, yellow.vec);
            } else{
                this->fontRenderer->renderText(scoreCounter, width/2 - (12 * scoreCounter.length()), (height/12) - 21, 1, white.vec);
            }

            break;
        }
        case over: {
            // render game over screen.
            if(totalScore > 0){
                string message = "GAME OVER! your score was: " + std::to_string(totalScore);
                this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, 1, vec3{0, 1, 0});
            }
            else {
                string message = "GAME OVER! you scored no points!";
                this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, 1, vec3{1, 0, 0});
            }
            break;
        }
    }

    glfwSwapBuffers(window);
}
// adds point by checking position of arrow markers and current arrow in arrows vector
// changes divider color when a point is scored.
// PARAM: 'key' is used to determine which quadrant to check for a scored arrow.
void Engine::addPoint(string key) {
    if(key == "left") {
        // check every arrow in arrows.
        for (int i = 0; i < arrows.size(); i++) {
            // if arrow is the first quad and is not scored.
            if(arrows[i]->getRight() < divLeft->getPosX() && !(arrows[i]->getScored())) {
                //only triggers if it is in the last 3rd of the screen
                if(arrows[i]->getTop() < height/3) {
                    // if an arrow is within 20 pixels above or below marker set scored to true.
                    if(arrows[i]->getTop() < (arrowMarkerQ1->getTop() + 20) &&
                       arrows[i]->getBottom() > (arrowMarkerQ1->getBottom() - 20)) {
                        //add emphasis on success click by changing div color
                        divLeft->setColor(blue);
                        arrows[i]->setScored(true);
                    }
                }
            }
        }

    }
    if(key == "right"){
        // check every arrow in arrows.
        for (int i = 0; i < arrows.size(); i++) {
            //in fourth quartile (right arrow) and has not been scored
            if( arrows[i]->getRight() > divRight->getPosX() && !(arrows[i]->getScored())) {
                //only triggers if it is in the last 3rd of the screen
                if(arrows[i]->getTop() < height/3) {

                    if(arrows[i]->getTop() < (arrowMarkerQ4->getTop() + 20) &&
                       arrows[i]->getBottom() > (arrowMarkerQ4->getBottom() - 20)) {
                        //add emphasis on success click by changing div color
                        divRight->setColor(red);
                        arrows[i]->setScored(true);
                    }
                }
            }
        }
    }
    if(key == "up"){
        for (int i = 0; i < arrows.size(); i++) {
            //in third quartile (up arrow) and has not been scored
            if(arrows[i]->getRight() > divCenter->getPosX() && arrows[i]->getRight() < divRight->getPosX() &&
               !(arrows[i]->getScored())) {
                //only triggers if it is in the last 3rd of the screen
                if(arrows[i]->getTop() < height/3) {
                    // if an arrow is within 20 pixels above or below, marker set scored to true.
                    if(arrows[i]->getTop() < (arrowMarkerQ3->getTop() + 20) &&
                       arrows[i]->getBottom() > (arrowMarkerQ3->getBottom() - 20)) {
                        //add emphasis on success click by changing div color
                        divCenter->setColor(yellow);
                        arrows[i]->setScored(true);
                    }
                }
            }
        }

    }
    if(key == "down"){
        for (int i = 0; i < arrows.size(); i++) {
            //in second quartile (down arrow) and has not been scored
            if(arrows[i]->getRight() > divLeft->getPosX() && arrows[i]->getRight() < divCenter->getPosX() &&
               !(arrows[i]->getScored())) {
                //only triggers if it is in the last 3rd of the screen
                if(arrows[i]->getTop() < height/3) {
                    // if an arrow is within 20 pixels above or below marker set scored to true.
                    if(arrows[i]->getTop() < (arrowMarkerQ2->getTop() + 20) &&
                       arrows[i]->getBottom() > (arrowMarkerQ2->getBottom() - 20)) {
                        //add emphasis on success click by changing div color
                        divCenter->setColor(green);
                        arrows[i]->setScored(true);
                    }
                }
            }
        }
    }
}
void Engine::spawnArrow() {
    int quartile;
    long arrowPicked = 30;
    vec2 pos;
    color color;
    vec2 size = {30, 25};
    // adds an arrow into vector at random quadrant
    quartile = 1 + (rand() % 4);


    if(quartile == 1) {
        // make an arrow with that quadrants vector
        pos = {(width * 1) / 8, height};
        arrows.push_back(make_unique<Arrow>(shapeShader, pos, size, blue, 1));
    }
    if(quartile == 2) {
        pos = {(width * 3)/8, height};
        arrows.push_back(make_unique<Arrow>(shapeShader, pos, size, green, 2));
    }
    if(quartile == 3) {
        pos = {(width * 5)/8,height};
        arrows.push_back(make_unique<Arrow>(shapeShader, pos, size, yellow, 3));
    }
    if(quartile == 4) {
        pos = {(width * 7)/8,height};
        arrows.push_back(make_unique<Arrow>(shapeShader, pos, size, red, 4));
    }
}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}

GLenum Engine::glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        cout << error << " | " << file << " (" << line << ")" << endl;
    }
    return errorCode;
}