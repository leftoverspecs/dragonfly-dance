#pragma once

#include <sdl/scene.hpp>

#include "bubbles.hpp"
#include "dragonflies.hpp"
#include "player.hpp"
#include "score.hpp"
#include "timer.hpp"
#include "supply.hpp"

namespace engine::sdl {
class Controller;
}

namespace engine::opengl {
class SpriteMap;
class SpriteRenderer;
class Font;
}// namespace engine::opengl

class Background;
class Music;

class Game : public engine::sdl::Scene {
public:
    Game(GLfloat width,
          GLfloat height,
          engine::opengl::Screen& screen,
          engine::sdl::OpenGlWindow &window,
          engine::sdl::Controller &controller1,
          engine::sdl::Controller &controller2,
          Background &background,
          engine::opengl::SpriteRenderer &player1,
          engine::opengl::SpriteRenderer &player2,
          engine::opengl::Font &font,
          Music &music,
          int players,
          int last_highscore);

    int get_score() const { return score.get_score(); }
private:
    engine::sdl::Controller *controller1;
    engine::sdl::Controller *controller2;
    engine::opengl::BoxRenderer box;
    Background *background;
    engine::opengl::Font *font;
    Music *music;
    int players{1};

    Score score;
    Bubbles bubbles;
    Supply supply1;
    std::optional<Supply> supply2;
    Player player1;
    std::optional<Player> player2;
    Timer timer;
    Dragonflies dragonflies;

    void on_faded_in() override;
    void on_loop(float delta_time) override;
};
