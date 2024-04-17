#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <vector>
#include <memory>
#include <iostream>
#include <GLFW/glfw3.h>

#include "shader/shaderManager.h"
#include "font/fontRenderer.h"
#include "shapes/rect.h"
#include "shapes/triangle.h"
#include "shapes/shape.h"
#include "shapes/arrow.h"

using std::vector, std::unique_ptr, std::make_unique, glm::ortho, glm::mat4, glm::vec3, glm::vec4;

/**
 * @brief The Engine class.
 * @details The Engine class is responsible for initializing the GLFW window, loading shaders, and rendering the game state.
 */
class Engine {
private:
    color red = color{.99, 0, .450, 1};
    color blue = color{0, .300, .604, 1};
    color yellow = color{.904, .910, 0, 1};
    color green = color{0,1,0,1};
    color white = color{1,1,1,1};
    int totalScore = 0;
    float speed = -1.5;
    /// @brief The actual GLFW window.
    GLFWwindow* window{};

    /// @brief The width and height of the window.
    const unsigned int width = 800, height = 600; // Window dimensions

    /// @brief Keyboard state (True if pressed, false if not pressed).
    /// @details Index this array with GLFW_KEY_{key} to get the state of a key.
    bool keys[1024];

    /// @brief Responsible for loading and storing all the shaders used in the project.
    /// @details Initialized in initShaders()
    unique_ptr<ShaderManager> shaderManager;

    /// @brief Responsible for rendering text on the screen.
    /// @details Initialized in initShaders()
    unique_ptr<FontRenderer> fontRenderer;

    // Shapes used in engine
    unique_ptr<Shape> divCenter;
    unique_ptr<Shape> divLeft;
    unique_ptr<Shape> divRight;

    vector<unique_ptr<Arrow>> arrows;
    unique_ptr<Arrow> arrow;
    unique_ptr<Rect> blinkS;

    vector<int> arrowNums;

    unique_ptr<Arrow> arrowMarkerQ1;
    unique_ptr<Arrow> arrowBaseClickQ1;

    unique_ptr<Arrow> arrowMarkerQ2;
    unique_ptr<Arrow> arrowBaseClickQ2;

    unique_ptr<Arrow> arrowMarkerQ3;
    unique_ptr<Arrow> arrowBaseClickQ3;

    unique_ptr<Arrow> arrowMarkerQ4;
    unique_ptr<Arrow> arrowBaseClickQ4;



    // Shaders
    Shader shapeShader;
    Shader textShader;

    double MouseX, MouseY;
    bool mousePressedLastFrame = false;
    bool arrowClickedLastFrame = false;

    /// @note Call glCheckError() after every OpenGL call to check for errors.
    GLenum glCheckError_(const char *file, int line);
    /// @brief Macro for glCheckError_ function. Used for debugging.
#define glCheckError() glCheckError_(__FILE__, __LINE__)

public:
    /// @brief Constructor for the Engine class.
    /// @details Initializes window and shaders.
    Engine();

    /// @brief Destructor for the Engine class.
    ~Engine();

    /// @brief Initializes the GLFW window.
    /// @return 0 if successful, -1 otherwise.
    unsigned int initWindow(bool debug = false);

    /// @brief Loads shaders from files and stores them in the shaderManager.
    /// @details Renderers are initialized here.
    void initShaders();

    /// @brief Initializes the shapes to be rendered.
    void initShapes();

    /// @brief Pushes back a new colored arrow to the arrows vector.
    void spawnArrow();
    void addPoint(string key);

    /// @brief Processes input from the user.
    /// @details (e.g. keyboard input, mouse input, etc.)
    void processInput();

    /// @brief Updates the game state.
    /// @details (e.g. collision detection, delta time, etc.)
    void update();

    /// @brief Renders the game state.
    /// @details Displays/renders objects on the screen.
    void render();

    /* deltaTime variables */
    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame (used to calculate deltaTime)

    // -----------------------------------
    // Getters
    // -----------------------------------

    /// @brief Returns true if the window should close.
    /// @details (Wrapper for glfwWindowShouldClose()).
    /// @return true if the window should close
    /// @return false if the window should not close
    bool shouldClose();

    /// Projection matrix used for 2D rendering (orthographic projection).
    /// We don't have to change this matrix since the screen size never changes.
    /// OpenGL uses the projection matrix to map the 3D scene to a 2D viewport.
    /// The projection matrix transforms coordinates in the camera space into normalized device coordinates (view space to clip space).
    /// @note The projection matrix is used in the vertex shader.
    // 4th quadrant
    mat4 PROJECTION = ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
    // 1st quadrant
//        mat4 PROJECTION = ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));

};

#endif //GRAPHICS_ENGINE_H