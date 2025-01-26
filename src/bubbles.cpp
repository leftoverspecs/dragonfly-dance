#include "bubbles.hpp"

#include "background.hpp"
#include "bubbles_sprite.hpp"
#include "score.hpp"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>
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
    std::erase_if(bubbles, [this](Bubble &bubble) {
        const auto position = bubble.get_position();
        GLfloat radius = bubble.get_radius();
        return position.x < -radius || position.x > screen_width + radius || position.y < -radius || position.y > screen_height + radius;
    });
    bool found = false;
    do {
        found = false;
        for (auto first = bubbles.begin(); first != bubbles.end(); ++first) {
            for (auto second = next(first); second != bubbles.end(); ++second) {
                if (first->absorbs(*second)) {
                    first->inflate(0.1f * second->get_radius());
                    //score->update_score(first->get_lifetime() / 1000.0f * first->get_destionation_radius() / 10.0f);
                    bubbles.erase(second);
                    found = true;
                    break;
                }
                if (second->absorbs(*first)) {
                    second->inflate(0.1f * first->get_radius());
                    //score->update_score(second->get_lifetime() / 1000.0f * second->get_destionation_radius() / 10.0f);
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

void Bubbles::draw() {
    renderer.clear();
    for (const auto &bubble: bubbles) {
        bubble.draw(renderer);
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
        if (distance(position, it->get_position()) < 1.2f * it->get_radius()) {
            score->update_score((it->get_radius() / 20.0f) * (it->get_lifetime() / 1000.0f));
            bubbles.erase(it);
            return true;
        }
    }
    return false;
}

void Bubbles::stir(glm::vec2 position, glm::vec2 velocity) {
    for (auto &bubble: bubbles) {
        bubble.stir(position, velocity);
    }
}

std::optional<glm::vec2> Bubbles::get_largest_bubble_position() const {
    GLfloat largest_size = 0.0f;
    std::optional<glm::vec2> result;
    for (const auto &bubble: bubbles) {
        if (bubble.get_destionation_radius() > largest_size) {
            result = bubble.get_position();
            largest_size = bubble.get_destionation_radius();
        }
    }
    return result;
}

Bubbles::Bubble::Bubble(glm::vec2 position, GLfloat radius)
    : position(position),
      velocity(glm::diskRand(glm::linearRand(0.0f, 0.0001f))),
      acceleration(0.0f, 0.0f),
      radius(radius),
      destination_radius(radius) {}

void Bubbles::Bubble::update(float delta_time) {
    lifetime += delta_time;
    acceleration /= 1.1f;
    if (length(acceleration) < 0.001f) {
        acceleration = glm::vec2(0.0f, 0.0f);
    }
    velocity += acceleration * delta_time;
    velocity /= 1.01f;
    position += (velocity + glm::vec2(0.0f, -0.01f)) * delta_time;
    if (destination_radius > radius) {
        radius += delta_time * 0.05;
    } else {
        destination_radius = radius;
    }
    velocity += glm::diskRand(glm::linearRand(0.0f, 0.0001f)) * delta_time;
    if (length(velocity) > 1.0f) {
        velocity /= length(velocity);
    }
}
void Bubbles::Bubble::draw(engine::opengl::SpriteRenderer &renderer) const {
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = scale(model, glm::vec3(2.0f * radius, 2.0f * radius, 1.0));
    model = translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
    renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 16, 8, 4, 4);
}

void Bubbles::Bubble::stir(glm::vec2 position, glm::vec2 velocity) {
    if (distance(this->position, position) < radius * 10.0) {
        this->acceleration += 0.001f * velocity / radius;
        this->velocity += 0.5f * velocity / (radius * radius);
    }
}

bool Bubbles::Bubble::absorbs(const Bubble &bubble) const {
    return bubble.lifetime >= 1000.0f && radius >= bubble.radius && distance(position, bubble.position) < 1.5f * radius;
}

void Bubbles::Bubble::inflate(float update) {
    destination_radius += update; // 10.0f;
}
