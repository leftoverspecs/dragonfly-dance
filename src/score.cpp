#include "score.hpp"

#include <format>
#include <opengl/font.hpp>

#include <glm/gtx/transform.hpp>

Score::Score(engine::opengl::Font &font) : font(&font) {
}

void Score::update(int point) {
    this->points += point;
}

void Score::draw() const {
    std::string s = std::format("{:06d}", points);
    glm::mat4 model(1.0f);
    model = translate(model, glm::vec3(600, 540, 0));
    model = scale(model, 2.0f * glm::vec3(15.0, 20.0, 1.0f));
    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    font->write(model, color, s.c_str());
}
