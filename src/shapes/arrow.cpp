#include "arrow.h"
#include "../util/color.h"

Arrow::Arrow(Shader & shader, vec2 pos, vec2 size, struct color color, int quartile)
        : Shape(shader, pos, size, color) {
    scored = false;
    if (quartile == 1)
        initVectorsQ1();
    if (quartile == 2)
        initVectorsQ2();
    if (quartile == 3)
        initVectorsQ3();
    if (quartile == 4)
        initVectorsQ4();
    initVAO();
    initVBO();
    initEBO();
}

Arrow::Arrow(Arrow const& other) : Shape(other) {
    initVectorsQ4();
    initVAO();
    initVBO();
    initEBO();
}

Arrow::~Arrow() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Arrow::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void Arrow::initVectorsQ1() { //Left
    this->vertices.insert(vertices.end(), {
            0.0f, 0.5f,  // Top left
            1.0f, 0.5f,   // Top right
            0.0f, -0.5f, // Bottom left
            1.0f, -0.5f,  // Bottom right
            0.0f, 1.0f,   // top of the arrow
            -1.00f, 0.0f,   // tip of the arrow.
            0.0f, -1.0f,  // bottom of the arrow
    });
    this->indices.insert(indices.end(), {
            0, 1, 2, // First triangle
            1, 2, 3,  // Second triangle
            4, 5, 6 // third triangle (the added triangle of the arrow)
    });
}

void Arrow::initVectorsQ2() { //Down
    this->vertices.insert(vertices.end(), {
            0.5f, 0.0f,  // Top left
            0.5f, 1.0f,   // Top right
            -0.5f, 0.0f, // Bottom left
            -0.5f, 1.0f,  // Bottom right
            -1.0f, 0.0f,   // left side of the arrow
            0.0f, -1.0f,   // bottom tip of the arrow.
            1.0f, 0.0f,  // right side of the arrow
    });
    this->indices.insert(indices.end(), {
            0, 1, 2, // First triangle
            1, 2, 3,  // Second triangle
            4, 5, 6 // third triangle (the added triangle of the arrow)
    });
}

void Arrow::initVectorsQ3() { //Right
    this->vertices.insert(vertices.end(), {
            0.5f, 0.0f,  // Top left
            0.5f, 1.0f,   // Top right
            -0.5f, 0.0f, // Bottom left
            -0.5f, 1.0f,  // Bottom right
            -1.0f, 1.0f,   // left side of the arrow
            0.0f, 2.0f,   // top tip of the arrow.
            1.0f, 1.0f,  // right side of the arrow
    });
    this->indices.insert(indices.end(), {
            0, 1, 2, // First triangle
            1, 2, 3,  // Second triangle
            4, 5, 6 // third triangle (the added triangle of the arrow)
    });
}

void Arrow::initVectorsQ4() { //Up
    this->vertices.insert(vertices.end(), {
            -0.5f, 0.5f,  // Top left
            0.5f, 0.5f,   // Top right
            -0.5f, -0.5f, // Bottom left
            0.5f, -0.5f,  // Bottom right
            0.5f, 1.0f,   // top of the arrow
            1.50f, 0.0f,   // tip of the arrow.
            0.5f, -1.0f,  // bottom of the arrow
    });
    this->indices.insert(indices.end(), {
            0, 1, 2, // First triangle
            1, 2, 3,  // Second triangle
            4, 5, 6 // third triangle (the added triangle of the arrow)
    });
}

float Arrow::getTop() const         { return pos.y + (size.y /2); }
float Arrow::getLeft() const        { return pos.x - (size.x / 2); }
float Arrow::getRight() const       { return pos.x + (size.x /2); }
float Arrow::getTip() const         { return pos.x + (+ (size.x /2) + 0.25f);}
float Arrow::getBottom() const      { return pos.y - (size.y /2);}
bool Arrow::getScored() const { return scored;}
void Arrow::setScored(bool b) { scored = b;}