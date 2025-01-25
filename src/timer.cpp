#include "timer.hpp"

#include <format>
#include <opengl/font.hpp>

#include <glm/gtx/transform.hpp>

Timer::Timer(engine::opengl::Font &font)
    : font(&font),
      initial_time(1 * 60 * 1000 + 42 * 1000),
      remaining_time(initial_time) {
}

void Timer::update(float delta_time) {
    remaining_time -= delta_time;
    if (remaining_time <= 0) {
        remaining_time = 0.0f;
    }
}

void Timer::draw() const {
    int minutes = static_cast<int>(remaining_time / (60 * 1000));
    int seconds = static_cast<int>((remaining_time - 60 * 1000 * minutes) / 1000);
    std::string time = std::format("{}:{:02d}", minutes, seconds);
    glm::mat4 model(1.0f);
    model = translate(model, glm::vec3(20, 540, 0));
    model = scale(model, 2.0f * glm::vec3(15.0, 20.0, 1.0f));
    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    if (remaining_time <= 11 * 1000) {
        const float value = 0.25f * std::sin(2 * M_PI * remaining_time / 1000.0f) + 0.75f;
        color = glm::vec4(value, 0.0f, 0.0f, value);
    }
    font->write(model, color, time.c_str());
}

bool Timer::time_is_up() const {
    return remaining_time <= 0.0f;
}
