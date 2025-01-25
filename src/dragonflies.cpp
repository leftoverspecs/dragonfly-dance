#include "dragonflies.hpp"

#include "bubbles.hpp"

#include <assets/dragonfly.png.h>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

Dragonflies::Dragonflies(Bubbles &bubbles, GLfloat width, GLfloat height)
    : bubbles(&bubbles),
      sprites(dragonfly_png, sizeof(dragonfly_png), 7, 4),
      renderer(sprites, width, height),
      width(width),
      height(height),
      spawn_time(1000.0f * static_cast<GLfloat>(rand()) / RAND_MAX)
{
}

void Dragonflies::update(float delta_time) {
    for (auto &dragonfly: dragonflies) {
        dragonfly.update(delta_time);
        if (!dragonfly.is_flying_home() && bubbles->check_bubbles(dragonfly.get_position())) {
            dragonfly.fly_home();
        }
    }
    std::erase_if(dragonflies, [this](const auto &dragonfly) {
       const auto position = dragonfly.get_position();
       return position.x < -20.0f || position.x > width + 20.0f || position.y < -20.0f || position.y > height + 20.0f;
    });
    spawn_time -= delta_time;
    if (spawn_time < 0.0f) {
        spawn_time = 3000.0f * static_cast<GLfloat>(rand()) / RAND_MAX;
        bool looks_right = static_cast<GLfloat>(rand()) / RAND_MAX >= 0.5f;
        add_dragonfly(glm::vec2(looks_right ? 0 : width, height * static_cast<GLfloat>(rand()) / RAND_MAX), looks_right);
    }
}

void Dragonflies::draw() {
    renderer.clear();
    for (const auto &dragonfly: dragonflies) {
        dragonfly.draw(renderer);
    }
    renderer.draw();
}

void Dragonflies::add_dragonfly(glm::vec2 position, bool looks_right) {
    dragonflies.push_back(Dragonfly(position, looks_right));
}

Dragonflies::Dragonfly::Dragonfly(glm::vec2 position, bool looks_right)
    : position(position),
      velocity(0.1 * (looks_right ? 1.0f : -1.0f) * static_cast<GLfloat>(rand()) / RAND_MAX,
               0.005 * (2.0f * static_cast<GLfloat>(rand()) / RAND_MAX - 1.0f)),
      looks_right(looks_right) {
}

void Dragonflies::Dragonfly::fly_home() {
    flies_home = true;
    looks_right = !looks_right;
    velocity = glm::vec2(looks_right ? 0.5f : -0.5f,
             static_cast<GLfloat>(rand()) / RAND_MAX);
}

void Dragonflies::Dragonfly::update(float delta_time) {
    animation_time += 0.02 * delta_time;
    if (animation_time > 3) {
        animation_time = 0;
    }
    position += velocity * delta_time;
}

void Dragonflies::Dragonfly::draw(engine::opengl::SpriteRenderer &renderer) const {
    glm::mat4 model(1.0f);
    model = translate(model, glm::vec3(position, 0.0f));
    model = scale(model, glm::vec3(64.0f, 64.0f, 1.0f));
    model = scale(model, glm::vec3(looks_right ? 1.0f : -1.0f, 1.0f, 1.0f));
    model = translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
    renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), static_cast<int>(animation_time), 0);
}
