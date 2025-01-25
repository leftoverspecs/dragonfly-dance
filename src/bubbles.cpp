#include "bubbles.hpp"

#include "background.hpp"
#include "bubbles_sprite.hpp"
#include "score.hpp"

#include <glm/gtx/transform.hpp>
#include <map>

Bubbles::Bubbles(GLsizei width, GLsizei height, Score &score)
    : score(&score),
      map(bubbles_sprite, bubbles_sprite_size, 24, 16),
      renderer(map, width, height),
      screen_width(width), screen_height(height) {
}

void Bubbles::update(float delta_time) {
    for (auto &bubble: bubbles) {
        bubble.update(delta_time);
    }
    bool found = false;
    do {
        found = false;
        for (auto first = bubbles.begin(); first != bubbles.end(); ++first) {
            for (auto second = next(first); second != bubbles.end(); ++second) {
                if (first->absorbs(*second)) {
                    first->inflate(0.1f * second->get_radius());
                    score->update(first->get_destionation_radius() / 10.0f);
                    bubbles.erase(second);
                    found = true;
                    break;
                }
                if (second->absorbs(*first)) {
                    second->inflate(0.1f * first->get_radius());
                    score->update(second->get_destionation_radius() / 10.0f);
                    bubbles.erase(first);
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
    } while (found);
}

void Bubbles::draw(float time) {
    renderer.clear();
    for (const auto &bubble: bubbles) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(bubble.get_position(), 0.0f));
        model = scale(model, glm::vec3(2.0f * bubble.get_radius(), 2.0f * bubble.get_radius(), 1.0));
        model = translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
        renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 16, 8, 4, 4);
    }
    renderer.draw();
}

void Bubbles::add_or_inflate_bubble(glm::vec2 position) {
    for (auto &bubble: bubbles) {
        if (distance(position, bubble.get_position()) < bubble.get_radius()) {
            bubble.inflate(10.0f);
            return;
        }
    }
    add_bubble(position);
}

void Bubbles::add_bubble(glm::vec2 position) {
    bubbles.push_back(Bubble{position, 10.0f});
}

bool Bubbles::check_and_pop_bubble(glm::vec2 position) {
    for (auto it = bubbles.begin(); it != bubbles.end(); ++it) {
        if (distance(position, it->get_position()) < it->get_radius()) {
            bubbles.erase(it);
            return true;
        }
    }
    return false;
}

Bubbles::Bubble::Bubble(glm::vec2 position, GLfloat radius)
    : position(position),
      radius(radius),
      destination_radius(radius),
      start(position),
      mid(start + 20.0f * glm::vec2(2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f, 2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f)),
      end(mid + 20.0f * glm::vec2(2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f, 2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f)),
      velocity(0.0005f) {}

void Bubbles::Bubble::update(float delta_time) {
    lifetime += delta_time;
    time += velocity * delta_time;
    if (destination_radius > radius) {
        radius += delta_time * 0.05;
    } else {
        destination_radius = radius;
    }
    if (time < 1.0f) {
        position = (1 - time) * (1 - time) * start + 2 * time * (1 - time) * mid + time * time * end;
    } else {
        velocity = 0.0005f;
        start = position;
        mid = 2.0f * end - mid;
        end = mid + 20.0f * glm::vec2(2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f, 2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f);
        time = 0.0f;
    }
}

bool Bubbles::Bubble::absorbs(const Bubble &bubble) const {
    return bubble.lifetime >= 1000.0f && radius >= bubble.radius && distance(position, bubble.position) < radius;
}

void Bubbles::Bubble::inflate(float update) {
    destination_radius += update; // 10.0f;
}
