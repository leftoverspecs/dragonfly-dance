#include "player.hpp"

#include <assets/player.png.h>
#include <assets/player2.png.h>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace {

glm::vec2 GRAVITY(0.0f, -0.0025f);

const float JUMP_STRENGTH = 1.1f;
const float JUMP_COOLDOWN = 200.0f;

const float HORIZONTAL_SPEED = 0.15f;
const float HORIZONTAL_FRICTION = 1.2f;
const float HORIZONTAL_THRESHOLD = 0.01f;

const float MAX_VERTICAL_SPEED = 10.0f;
const float MAX_HORIZONTAL_SPEED = 10.0f;

const float GROUND = 10.0f;
const float GROUND_MARGIN = GROUND + 10.0f;
const float LEFT_BORDER = 25.0f;
const float RIGHT_BORDER = 45.0f;

}

Player::Player(engine::sdl::Controller &controller,
               Bubbles &bubbles,
               engine::opengl::SpriteRenderer &player,
               bool female, float x, float y, GLfloat screen_width, GLfloat screen_height)
  : female(female),
    time(0.0f),
    player(&player),
    bubbles(&bubbles),
    controller(&controller),
    sprite_index_i(0),
    sprite_index_j(0),
    face_left(!female),
    position(x, y),
    //body(0.0f, 0.0f, 80.0f, 85.0f, screen_width, screen_height),
    //sword(0.0f, 0.0f, 60.0f, 60.0f, screen_width, screen_height),
    velocity(0.0f, 0.0f),
    last_time_standing(0.0f),
    slash_time(0.0f),
    dead_time(0.0f),
    screen_width(screen_width),
    hit_cooldown(0.0f)
{ }

void Player::update(float msec) {
    time += msec;

    velocity += GRAVITY * msec;
    int intensity = controller->is_button_a_pressed();
    if (intensity > 0 && last_time_standing < JUMP_COOLDOWN) {
        velocity.y = JUMP_STRENGTH;
    }
    last_time_standing += msec;
    intensity = controller->is_button_left_pressed();
    if (intensity > 0) {
        velocity.x -= HORIZONTAL_SPEED; // HORIZONTAL_ACCELERATION * msec;
        face_left = true;
    }
    intensity = controller->is_button_right_pressed();
    if (intensity > 0) {
        velocity.x += HORIZONTAL_SPEED; // HORIZONTAL_ACCELERATION * msec;
        face_left = false;
    }
    intensity = controller->get_right_trigger();
    if (intensity > 0) {
        slash_time += msec;
    } else {
        slash_time = 0;
        bubbled = 0;
    }

    velocity.x /= HORIZONTAL_FRICTION;
    if (std::abs(velocity.x) < HORIZONTAL_THRESHOLD) {
        velocity.x = 0.0f;
    }
    velocity.x = glm::clamp(velocity.x, -MAX_HORIZONTAL_SPEED, MAX_HORIZONTAL_SPEED);
    velocity.y = glm::clamp(velocity.y, -MAX_VERTICAL_SPEED, MAX_VERTICAL_SPEED);
    position += velocity * msec;
    if (position.y < GROUND) {
        position.y = GROUND;
        velocity.y = 0.0f;
        last_time_standing = 0.0f;
    }
    if (female) {
        //std::cout << position.x << ", " << position.y << '\n';
    }
    if (position.x < LEFT_BORDER) {
        position.x = LEFT_BORDER;
        velocity.x = 0.0f;
    } else if (position.x > screen_width - RIGHT_BORDER) {
        position.x = screen_width - RIGHT_BORDER;
        velocity.x = 0.0f;
    }

    if (slash_time > 0.0f) {
        // Player drew its sword
        sprite_index_j = 2;
        sprite_index_i = std::min(static_cast<int>(std::floor(slash_time / 100.0f)) + 1, 8);
        if (bubbled < 30) {
            bubbles->add_bubble(position + 25.0f * glm::vec2(static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX) + glm::vec2(0.0f, 75.0f));
            bubbled++;
        }
    } else if (position.y > GROUND_MARGIN) {
        // Player is in air
        if (female) {
            sprite_index_j = 3;
        } else {
            sprite_index_j = 5;
        }
        if (velocity.y > 0.0f) {
            // Player ascends
            if (female) {
                sprite_index_i = 1;
            } else {
                sprite_index_i = 2;
            }
        } else if (velocity.y < 0.0f) {
            // Player descends
            if (female) {
                sprite_index_i = 3;
            } else {
                sprite_index_i = 4;
            }
        } else {
            // Player at peak
            if (female) {
                sprite_index_i = 2;
            } else {
                sprite_index_i = 3;
            }
        }
    } else if (position.y < GROUND_MARGIN) {
        // Player is considered on the ground
        if (std::abs(velocity.x) > 0.0f) {
            // Player walks
            sprite_index_i = static_cast<int>(std::floor(time / 100.0f)) % 8;
            sprite_index_j = 1;
            if (std::abs(velocity.x) > 0.1f) {
                // Player runs
            }
        } else {
            // Player is idle
            if (female) {
                sprite_index_i = static_cast<int>(std::floor(time / 250.0f)) % 6;
            } else {
                sprite_index_i = static_cast<int>(std::floor(time / 250.0f)) % 13;
            }
            sprite_index_j = 0;
        }
    }

    if (hit_cooldown > 0.0f) {
        hit_cooldown -= msec;
    } else {
        hit_cooldown = 0.0f;
    }
    if (hit_cooldown == 0.0f && false/*&& <hit condition>*/) {
        // Player is hit!
        controller->rumble(0xffff * 3 / 4, 0xffff * 3 / 4, 100);
        hit_cooldown = 1000.0f;
        velocity.x = face_left ? 1.0f : -1.0f;
    }
}

void Player::draw() {
    player->clear();
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(128.0f, 128.0f, 1.0f));
    model = glm::scale(model, glm::vec3(face_left ? -1.0f : 1.0f, 1.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-14.0f / 32.0f, 0.0f, 0.0f));
    float alpha = 1.0f;
    if (hit_cooldown > 0.0f) {
        alpha = static_cast<int>(std::floor(hit_cooldown / 5.0f)) % 2;
    }
    player->queue(model, glm::vec4(1.0f, 1.0f, 1.0f, alpha), sprite_index_i, sprite_index_j);
    player->draw();
    //body.draw(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    //sword.draw(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}
