#include "game.hpp"

#include "background.hpp"
#include "boxyfont.hpp"
#include "bubbles.hpp"
#include "dragonflies.hpp"
#include "intro.hpp"
#include "music.hpp"
#include "player.hpp"
#include "score.hpp"
#include "timer.hpp"

#include <opengl/spriterenderer.hpp>

#include <sdl/openglwindow.hpp>
#include <sdl/scene.hpp>


Game::Game(GLfloat width,
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
           int players)
    : Scene(screen, height, window),
      controller1(&controller1),
      controller2(players == 2 ? &controller2 : nullptr),
      background(&background),
      font(&font),
      music(&music),
      players(players),
      score(font),
      bubbles(width, height, score),
      player1(controller1, bubbles, player1, true, 0, 0, width, height),

      timer(font),
      dragonflies(bubbles, timer, width, height) {
    if (players == 2) {
        this->player2.emplace(controller2, bubbles, player2, false, width - 10, 10, width, height);
    }
}

void Game::on_faded_in() {
    music->fade_in_chase();
}

void Game::on_loop(float delta_time) {
    bubbles.update(delta_time);
    player1.update(delta_time);
    if (player2) {
        player2->update(delta_time);
    }
    timer.update(delta_time);
    dragonflies.update(delta_time);
    if (!is_fading_out() && timer.time_is_up()) {
        engine::audio::Music::fade_out(500.0f);
        fade_out(1000.0f);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    font->clear();
    timer.draw();
    score.draw();
    background->draw();
    player1.draw();
    if (player2) {
        player2->draw();
    }
    dragonflies.draw();
    bubbles.draw();
    font->draw();
}
