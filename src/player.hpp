#pragma once

#include <glm/vec2.hpp>

#include <opengl/box.hpp>
#include <sdl/controller.hpp>
#include <opengl/spritemap.hpp>
#include <opengl/spriterenderer.hpp>

class Player {
public:
    Player(engine::sdl::Controller &controller,
           bool female, float x, float y, GLfloat width, GLfloat height);

    glm::vec2 get_position() const { return position; }
    void set_other(const Player &other);

    void update(float msec);
    void draw();

private:
    bool female;
    float time;
    engine::opengl::SpriteMap sprites;
    engine::opengl::SpriteRenderer renderer;
    engine::sdl::Controller *controller;
    int sprite_index_i;
    int sprite_index_j;
    bool face_left;
    glm::vec2 position;
    engine::opengl::Box body;
    engine::opengl::Box sword;
    glm::vec2 velocity;
    float last_time_standing;
    float slash_time;
    float dead_time;
    float screen_width;
    float hit_cooldown;
    const Player *other;
};
