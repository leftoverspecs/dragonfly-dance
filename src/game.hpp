#pragma once

#include <sdl/scene.hpp>

#include "bubbles.hpp"
#include "dragonflies.hpp"
#include "player.hpp"
#include "score.hpp"
#include "timer.hpp"

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
          engine::sdl::OpenGlWindow &window,
          engine::sdl::Controller &controller1,
          engine::sdl::Controller &controller2,
          Background &background,
          engine::opengl::SpriteRenderer &player1,
          engine::opengl::SpriteRenderer &player2,
          engine::opengl::Font &font,
          Music &music,
          int players);

private:
    engine::sdl::Controller *controller1;
    engine::sdl::Controller *controller2;
    Background *background;
    engine::opengl::Font *font;
    Music *music;
    int players{1};

    Score score;
    Bubbles bubbles;
    Player player1;
    std::optional<Player> player2;
    Timer timer;
    Dragonflies dragonflies;

    void on_startup() override;
    void on_loop(float delta_time) override;
};
