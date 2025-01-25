#include "dragonflies.hpp"

#include "background.hpp"
#include "bubbles.hpp"
#include "timer.hpp"

#include <assets/dragonfly.png.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>
#include <ostream>

Dragonflies::Dragonflies(Bubbles &bubbles, Timer &timer, GLfloat width, GLfloat height)
    : bubbles(&bubbles),
      timer(&timer),
      sprites(dragonfly_png, sizeof(dragonfly_png), 7, 4),
      renderer(sprites, width, height),
      width(width),
      height(height),
      spawn_time(10000.0f * static_cast<GLfloat>(rand()) / RAND_MAX) {
    //add_dragonfly();
}

void Dragonflies::update(float delta_time) {
    for (auto &dragonfly: dragonflies) {
        dragonfly.update(delta_time);
        if (!dragonfly.is_flying_home() && bubbles->check_and_pop_bubble(dragonfly.get_position())) {
            dragonfly.fly_home(true);
        }
    }
    if (std::erase_if(dragonflies, [this](const auto &dragonfly) {
        const auto position = dragonfly.get_position();
        return position.x < -20.0f || position.x > width + 20.0f || position.y < -20.0f || position.y > height + 20.0f;
    }) > 0) {
        //add_dragonfly();
    }
    spawn_time -= delta_time;
    if (spawn_time < 0.0f || dragonflies.size() < 2) {
        //spawn_time = 3000.0f * static_cast<GLfloat>(rand()) / RAND_MAX;
        const GLfloat factor = (1 - exp(-2 * timer->get_available_time_percentage())) / (1 - exp(-2));
        spawn_time = (1000.0f * static_cast<GLfloat>(rand()) / RAND_MAX + 500.0f) * factor + 100.0f;
        std::cout << spawn_time << std::endl;
        add_dragonfly();
    }
}

void Dragonflies::draw() {
    renderer.clear();
    for (const auto &dragonfly: dragonflies) {
        dragonfly.draw(renderer);
    }
    renderer.draw();
}

void Dragonflies::add_dragonfly() {
    const bool left = static_cast<GLfloat>(rand()) / RAND_MAX <= 0.5f;
    const glm::vec2 start(left ? -20 : width + 20, height * static_cast<GLfloat>(rand()) / RAND_MAX);
    dragonflies.push_back(Dragonfly(width, height, start,
        bubbles->get_largest_bubble_position().value_or(glm::vec2(width / 2.0f,
            height * static_cast<GLfloat>(rand()) / RAND_MAX))));
}

Dragonflies::Dragonfly::Dragonfly(GLfloat width, GLfloat height, glm::vec2 start, glm::vec2 target)
    : width(width),
      height(height),
      position(start),
      flight_time(20.0f * distance(start, target) /** static_cast<GLfloat>(rand()) / RAND_MAX*/),
      start(position),
      mid(0.5f * (position + target) + glm::vec2(target.y - position.y, position.x - target.x) * static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX)),
      end(target) {
}

void Dragonflies::Dragonfly::fly_home(bool inverse) {
    flies_home = true;
    start = position;
    GLfloat end_x = (inverse ? !looks_right : looks_right) ? width + 50.0f : -50.0f;
    end = glm::vec2(end_x, height * static_cast<GLfloat>(rand()) / RAND_MAX);
    mid = 0.5f * (start + end) + 0.1f * glm::vec2(end.y - start.y, start.x - end.x) * static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
    flight_time = 5.0f * distance(start, end) /** static_cast<GLfloat>(rand()) / RAND_MAX*/;
    time = 0.0f;
}

void Dragonflies::Dragonfly::update(float delta_time) {
    animation_time += 0.02 * delta_time;
    if (animation_time > 3) {
        animation_time = 0;
    }
    time += delta_time;
    if (time <= flight_time) {
        const GLfloat t = time / flight_time;
        glm::vec2 new_position = (1 - t) * (1 - t) * start + 2 * t * (1 - t) * mid + t * t * end;
        looks_right = position.x < new_position.x;
        position = new_position;
    } else if (!flies_home) {
        position = end;
        fly_home(false);
    }
}

void Dragonflies::Dragonfly::draw(engine::opengl::SpriteRenderer &renderer) const {
    {
        glm::mat4 model(1.0f);
        model = translate(model, glm::vec3(position, 0.0f));
        model = scale(model, glm::vec3(64.0f, 64.0f, 1.0f));
        model = scale(model, glm::vec3(looks_right ? 1.0f : -1.0f, 1.0f, 1.0f));
        model = translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
        renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), static_cast<int>(animation_time), 0);
    }
    /*{
        glm::mat4 model(1.0f);
        model = translate(model, glm::vec3(start, 0.0f));
        model = scale(model, glm::vec3(64.0f, 64.0f, 1.0f));
        model = scale(model, glm::vec3(looks_right ? 1.0f : -1.0f, 1.0f, 1.0f));
        model = translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
        renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
    }
    {
        glm::mat4 model(1.0f);
        model = translate(model, glm::vec3(mid, 0.0f));
        model = scale(model, glm::vec3(64.0f, 64.0f, 1.0f));
        model = scale(model, glm::vec3(looks_right ? 1.0f : -1.0f, 1.0f, 1.0f));
        model = translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
        renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
    }
    {
        glm::mat4 model(1.0f);
        model = translate(model, glm::vec3(end, 0.0f));
        model = scale(model, glm::vec3(64.0f, 64.0f, 1.0f));
        model = scale(model, glm::vec3(looks_right ? 1.0f : -1.0f, 1.0f, 1.0f));
        model = translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
        renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
    }*/
}
