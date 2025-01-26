#pragma once

#include <opengl/boxrenderer.hpp>

class Supply {
public:
    Supply(engine::opengl::BoxRenderer &box, GLfloat width, int player, int num_players);

    void update(GLfloat delta_time);
    void enqueue() const;
    bool empty() const { return current <= 0.0f; }
    void drain();

    void try_fill(glm::vec2 position);
private:
    engine::opengl::BoxRenderer *box;
    GLfloat time{};
    glm::vec4 color;
    GLfloat width;
    int player;
    int num_players;
    GLfloat max;
    GLfloat current;
};
