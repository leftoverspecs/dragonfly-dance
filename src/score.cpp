#include "score.hpp"

#include <format>
#include <opengl/font.hpp>

#include <glm/gtx/transform.hpp>
#include <SDL.h>

Score::Score(engine::opengl::Font &font, int last_highscore) : font(&font), color(1.0f, 1.0f, 1.0f, 1.0f), last_highscore(last_highscore) {
}

void Score::update_score(int point) {
    this->points += point;
}

void Score::update(GLfloat delta_time) {
    if (this->points > last_highscore) {
        time += delta_time;
        const GLfloat phase = 2 * M_PI * time / 1000.0f;
        this->color = glm::vec4(0.5f * std::sin(phase) + 0.5f,
            0.5f * std::sin(phase + M_PI / 2.0f) + 0.5f,
            0.5f * std::sin(phase + M_PI) + 0.5f, 1.0f);
    } else {
        this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void Score::draw() const {
    std::string s = std::format("{:06d}", points);
    glm::mat4 model(1.0f);
    model = translate(model, glm::vec3(600, 540, 0));
    model = scale(model, 2.0f * glm::vec3(15.0, 20.0, 1.0f));
    font->write(model, color, s.c_str());
}
