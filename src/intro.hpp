#pragma once

#include <opengl/boxrenderer.hpp>

#include <sdl/scene.hpp>

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

class Intro : public engine::sdl::Scene {
public:
    Intro(GLfloat width,
          GLfloat height,
          engine::opengl::Screen& screen,
          engine::sdl::OpenGlWindow &window,
          engine::sdl::Controller &controller1,
          engine::sdl::Controller &controller2,
          Background &background,
          engine::opengl::SpriteRenderer &player1,
          engine::opengl::SpriteRenderer &player2,
          engine::opengl::Font &font,
          Music &music);

    int get_players() const { return players; }
private:
    engine::sdl::Controller *controller1;
    engine::sdl::Controller *controller2;
    Background *background;
    engine::opengl::SpriteRenderer *player1;
    engine::opengl::SpriteRenderer *player2;
    engine::opengl::Font *font;
    Music *music;
    engine::opengl::BoxRenderer box;
    GLfloat animation_index{};
    int players{1};

    void on_faded_in() override;
    void on_loop(float delta_time) override;
};
