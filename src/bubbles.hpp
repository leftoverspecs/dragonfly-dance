#pragma once

#include "opengl/buffer.hpp"
#include "opengl/framebuffer.hpp"
#include "opengl/spritemap.hpp"
#include "opengl/spriterenderer.hpp"
#include <vector>

class Bubbles {
public:
    Bubbles(GLsizei width, GLsizei height);

    void update(float delta_time);
    void draw(float time);

    void add_bubble(glm::vec2 position);

private:
    engine::opengl::SpriteMap map;
    engine::opengl::SpriteRenderer renderer;

    class Bubble {
    public:
        Bubble(glm::vec2 position, GLfloat radius)
            : position(position),
              radius(radius),
              destination_radius(radius),
              start(position),
              mid(start + 50.0f * glm::vec2(2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f, 2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f)),
              end(mid + 50.0f * glm::vec2(2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f, 2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f)),
              velocity(0.0001f) {}

        glm::vec2 get_position() const { return position; }
        GLfloat get_radius() const { return radius; }

        void inflate();
        void update(float delta_time);

    private:
        glm::vec2 position;
        GLfloat radius;
        GLfloat destination_radius;

        glm::vec2 start;
        glm::vec2 mid;
        glm::vec2 end;
        GLfloat time{};
        GLfloat velocity;
    };
    GLsizei screen_width;
    GLsizei screen_height;
    std::vector<Bubble> bubbles;
};
