//
// Created by Quinn Doyle on 4/4/24.
//

#ifndef GRAPHICS_ARROW_H
#define GRAPHICS_ARROW_H

#include "shape.h"
#include "rect.h"
#include "triangle.h"
#include "../shader/shader.h"
#include <iostream>
using glm::vec2, glm::vec3;

class Arrow : public Shape {
private:
    bool scored;
    /// @brief Initializes the vertices and indices of the arrow in quartile 1 pointing left
    void initVectorsQ1();
    /// @brief Initializes the vertices and indices of the arrow in quartile 2 pointing down
    void initVectorsQ2();
    /// @brief Initializes the vertices and indices of the arrow in quartile 3 pointing up
    void initVectorsQ3();
    /// @brief Initializes the vertices and indices of the arrow in quartile 4 pointing right
    void initVectorsQ4();
public:
    /// @brief Construct a new Arrow object
    /// @details This constructor will call the InitRenderData function.
    /// @param shader The shader to use
    /// @param pos The position of the arrow
    /// @param size The size of the arrow
    /// @param color The color of the arrow
    /// @param quartile The quartile the arrow will appear in aswell as direction
    Arrow(Shader & shader, vec2 pos, vec2 size, struct color color, int quartile);

    Arrow(Arrow const& other);

    /// @brief Destroy the Arrow object and delete it's VAO and VBO
    ~Arrow();

    /// @brief Binds the VAO and calls the virtual draw function
    void draw() const override;

    float getTop() const override;
    float getLeft() const override;
// DONE: Implement getRight, getTop, and getBottom
    float getRight() const override;
    float getTip() const;
    float getBottom() const override;
    bool getScored() const;
    void setScored(bool b);

};
#endif //GRAPHICS_ARROW_H
