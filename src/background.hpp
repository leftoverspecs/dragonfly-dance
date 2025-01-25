#pragma once

#include "opengl/spriterenderer.hpp"

#include <opengl/spritemap.hpp>

class Background {
public:
    Background(GLfloat width, GLfloat height);

    void draw();
private:
    engine::opengl::SpriteMap map;
    engine::opengl::SpriteRenderer renderer;
    GLfloat width;
    GLfloat height;
};
