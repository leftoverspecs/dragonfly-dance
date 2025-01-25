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
    void add_or_inflate_bubble(glm::vec2 position);
    bool check_and_pop_bubble(glm::vec2 position);

private:
    engine::opengl::SpriteMap map;
    engine::opengl::SpriteRenderer renderer;

    class Bubble {
    public:
        Bubble(glm::vec2 position, GLfloat radius);

        glm::vec2 get_position() const { return position; }
        GLfloat get_radius() const { return radius; }
        bool absorbs(const Bubble &bubble) const;

        void inflate(float update);
        void update(float delta_time);

    private:
        GLfloat lifetime{};
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
