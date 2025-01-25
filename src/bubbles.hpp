#pragma once

#include "opengl/buffer.hpp"
#include "opengl/framebuffer.hpp"
#include "opengl/spritemap.hpp"
#include "opengl/spriterenderer.hpp"
#include <vector>

class Bubbles {
public:
    Bubbles(GLsizei width, GLsizei height);
    void draw(float time);

    void add_bubble(glm::vec2 position, GLfloat radius);
private:
    engine::opengl::SpriteMap map;
    engine::opengl::SpriteRenderer renderer;

    struct Bubble {
        glm::vec2 position;
        GLfloat radius;
    };
    GLsizei screen_width;
    GLsizei screen_height;
    std::vector<Bubble> bubbles;
};
