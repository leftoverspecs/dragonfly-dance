#include "bubbles.hpp"

#include "background.hpp"
#include "bubbles_sprite.hpp"

#include <glm/gtx/transform.hpp>


namespace {

GLfloat screen_vertex_data[] = {
    /*   X      Y     S     T    */
    -2.0f, -2.0f, -0.5f, -0.5f,
    -2.0f, 2.0f, -0.5f, 1.5f,
    2.0f, 2.0f, 1.5f, 1.5f,

    -2.0f, -2.0f, -0.5f, -0.5f,
    2.0f, 2.0f, 1.5f, 1.5f,
    2.0f, -2.0f, 1.5f, -0.5f};

constexpr int ATTRIBUTE_POSITION = 0;
constexpr int ATTRIBUTE_TEXTURE_COORD = 1;

}

Bubbles::Bubbles(GLsizei width, GLsizei height)
    : map(bubbles_sprite, bubbles_sprite_size, 24, 16),
      renderer(map, width, height),
      screen_width(width), screen_height(height) {
}

void Bubbles::update(float delta_time) {
    for (auto &bubble : bubbles) {
        bubble.update(delta_time);
    }
}

void Bubbles::draw(float time) {
    renderer.clear();
    for (const auto &bubble : bubbles) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(bubble.get_position(), 0.0f));
        model = scale(model, glm::vec3(bubble.get_radius(), bubble.get_radius(), 1.0));
        model = translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
        renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 16, 8, 4, 4);
    }
    renderer.draw();
}

void Bubbles::add_bubble(glm::vec2 position) {
    for (auto &bubble : bubbles) {
        if (distance(position, bubble.get_position()) < bubble.get_radius()) {
            bubble.inflate();
            return;
        }
    }
    bubbles.push_back(Bubble{position, 20.0f});
}

void Bubbles::Bubble::update(float delta_time) {
    time += velocity * delta_time;
    if (destination_radius > radius) {
        radius += delta_time * 0.05;
    } else {
        destination_radius = radius;
    }
    if (time < 1.0f) {
        position = (1 - time) * (1 - time) * start + 2 * time * (1 - time) * mid + time * time * end;
    } else {
        start = position;
        mid = 2.0f * end - mid;
        end = mid + 50.0f * glm::vec2(2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f, 2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f);
        time = 0.0f;
    }
}

void Bubbles::Bubble::inflate() {
    destination_radius = destination_radius + 10.0f;
}
