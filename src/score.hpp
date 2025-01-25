#pragma once

namespace engine::opengl {
class Font;
}

class Score {
public:
    Score(engine::opengl::Font &font);

    void update(int points);
    void draw() const;

    bool time_is_up() const;
private:
    engine::opengl::Font *font;
    int points{};
};
