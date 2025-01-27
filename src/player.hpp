#pragma once

#include "bubbles.hpp"
#include "supply.hpp"


#include <glm/vec2.hpp>

#include <sdl/controller.hpp>
#include <opengl/spriterenderer.hpp>

class Bubbles;
class Supply;

class Player {
public:
    Player(engine::sdl::Controller &controller,
           Bubbles &bubbles,
           Supply &supply,
           engine::opengl::SpriteRenderer &player,
           bool female, float x, float y, GLfloat width, GLfloat height);

    glm::vec2 get_position() const { return position; }

    void update(float msec);
    void draw();

private:
    bool female;
    float time;
    engine::opengl::SpriteRenderer *player;
    Bubbles *bubbles;
    Supply *supply;
    engine::sdl::Controller *controller;
    int sprite_index_i;
    int sprite_index_j;
    bool face_left;
    glm::vec2 position;
    glm::vec2 velocity;
    float last_time_standing;
    float slash_time;
    float dead_time;
    float screen_width;
    float hit_cooldown;
};
