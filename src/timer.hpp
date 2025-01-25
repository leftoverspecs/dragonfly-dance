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
private:
    engine::opengl::Font *font;
    float remaining_time;
};
