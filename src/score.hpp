#pragma once
#include <GL/glew.h>
#include <glm/vec4.hpp>

namespace engine::opengl {
class Font;
}

class Score {
public:
    Score(engine::opengl::Font &font, int last_highscore);

    void update_score(int points);
    void update(GLfloat delta_time);
    void draw() const;

    int get_score() const { return points; }
private:
    engine::opengl::Font *font;
    GLfloat time{};
    glm::vec4 color;
    int last_highscore;
    int points{};
};
