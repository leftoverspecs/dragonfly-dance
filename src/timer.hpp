#pragma once

namespace engine::opengl {
class Font;
}

class Timer {
public:
    Timer(engine::opengl::Font &font);

    void update(float delta_time);
    void draw() const;

    bool time_is_up() const;
    float get_available_time_percentage() const { return remaining_time / initial_time; }
private:
    engine::opengl::Font *font;
    float initial_time;
    float remaining_time;
};
