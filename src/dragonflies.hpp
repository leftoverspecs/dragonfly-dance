#pragma once

#include <opengl/spritemap.hpp>
#include <opengl/spriterenderer.hpp>

class Bubbles;
class Timer;

class Dragonflies {
public:
    Dragonflies(Bubbles &bubbles, Timer &timer, GLfloat width, GLfloat height, int num_players);

    void update(float delta_time);
    void draw();

    void add_dragonfly();
    size_t count() const { return dragonflies.size(); }

private:
    class Dragonfly {
    public:
        Dragonfly(GLfloat width, GLfloat height, glm::vec2 start, glm::vec2 target);

        void fly_home(bool inverse);
        void update(float delta_time);
        void draw(engine::opengl::SpriteRenderer &renderer) const;
        glm::vec2 get_position() const { return position; }
        bool is_flying_home() const { return flies_home; }
    private:
        GLfloat width;
        GLfloat height;

        glm::vec2 position;
        bool flies_home{false};

        GLfloat time{};
        GLfloat flight_time;

        glm::vec2 start;
        glm::vec2 mid;
        glm::vec2 end;
        bool looks_right{false};

        GLfloat animation_time{};
    };
    Bubbles *bubbles;
    Timer *timer;
    engine::opengl::SpriteMap sprites;
    engine::opengl::SpriteRenderer renderer;
    std::vector<Dragonfly> dragonflies;
    GLfloat width;
    GLfloat height;
    int num_players;
    GLfloat spawn_time;
};
