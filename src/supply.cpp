#include "supply.hpp"

#include <glm/gtc/matrix_transform.hpp>

Supply::Supply(engine::opengl::BoxRenderer &box, GLfloat width, int player, int num_players)
    : box(&box),
      color(1.0f, 1.0f, 1.0f, 0.5f),
      width(width),
      player(player),
      num_players(num_players),
      max(50.0),
      current(max) {
}

void Supply::update(GLfloat delta_time) {
    time += delta_time;
    GLfloat ratio = current / max;
    if (ratio < 0.2f) {
        GLfloat value = 0.5f * std::cos(2 * M_PI * time / 1000.0f) + 0.5f;
        color = glm::vec4(1.0f, value, value, 0.5f);
    } else {
        color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
    }
}

void Supply::enqueue() const {
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(player == 1 ? 10.0f : width - 40.0f, 10.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 150.0f, 1.0f));
        box->queue_frame(model, color);
    }
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(player == 1 ? 15.0f : width - 35.0f, 15.0f, 0.0f));
        GLfloat ratio = current / max;
        model = glm::scale(model, glm::vec3(20.0f, 140.0f * ratio, 1.0f));
        box->queue_frame(model, glm::vec4(0.75f * (1.0 - ratio), 0.75 * ratio, 0.0f, 1.0f));
    }
}
void Supply::drain() {
    current -= 0.2f;
    if (current < 0.0f) {
        current = 0.0f;
    }
    if (current / max < 0.2f) {
        time = 0.0f;
    }
}
void Supply::try_fill(glm::vec2 position) {
    if (current < max) {
        if (player == 1 && position.x < 40.0f && position.y < 160.0f) {
            current += 0.5f / num_players;
        } else if (player == 2 && position.x > width - 60.0f && position.y < 160.0f) {
            current += 0.5f / num_players;
        }
    } else {
        current = max;
    }
}
