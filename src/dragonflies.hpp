#pragma once

#include <opengl/spritemap.hpp>
#include <opengl/spriterenderer.hpp>

class Bubbles;

class Dragonflies {
public:
    Dragonflies(Bubbles &bubbles, GLfloat width, GLfloat height);

    void update(float delta_time);
    void draw();

    void add_dragonfly(glm::vec2 position, bool looks_right);
private:
    class Dragonfly {
    public:
        Dragonfly(glm::vec2 position, bool looks_right);

        void fly_home();
        void update(float delta_time);
        void draw(engine::opengl::SpriteRenderer &renderer) const;
        glm::vec2 get_position() const { return position; }
        bool is_flying_home() const { return flies_home; }
    private:
        glm::vec2 position;
        glm::vec2 velocity;
        GLfloat animation_time{};
        bool looks_right;
        bool flies_home{false};
    };
    Bubbles *bubbles;
    engine::opengl::SpriteMap sprites;
    engine::opengl::SpriteRenderer renderer;
    std::vector<Dragonfly> dragonflies;
    GLfloat width;
    GLfloat height;
    GLfloat spawn_time;
};
