#include "intro.hpp"

#include "background.hpp"
#include "music.hpp"
#include "sdl/controller.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <opengl/font.hpp>

Intro::Intro(GLfloat width,
             GLfloat height,
             engine::sdl::OpenGlWindow &window,
             engine::sdl::Controller &controller1,
             engine::sdl::Controller &controller2,
             Background &background,
             engine::opengl::SpriteRenderer &player1,
             engine::opengl::SpriteRenderer &player2,
             engine::opengl::Font &font,
             Music &music)
    : Scene(height, window),
      controller1(&controller1),
      controller2(&controller2),
      background(&background),
      player1(&player1),
      player2(&player2),
      font(&font),
      music(&music),
      box(width, height) {
      //box(100, 100, width - 200, height - 200, 100, 100, width - 200, height - 200, width, height) {
}

void Intro::on_startup() {
    music->intro();
}

void Intro::on_loop(float delta_time) {
    animation_index += 0.005 * delta_time;
    if (animation_index > 4.0f) {
        animation_index = 0;
    }
    if (controller1->is_button_up_pressed() || controller2->is_button_up_pressed()) {
        players = 1;
    } else if (controller1->is_button_down_pressed() || controller2->is_button_down_pressed()) {
        players = 2;
    } else if (controller1->is_button_a_pressed() || controller2->is_button_a_pressed()) {
        exit();
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    font->clear();
    player1->clear();
    player2->clear();
    box.clear();

    const glm::vec4 highlight(2.0f, 2.0f, 2.0f, 1.0f);
    const glm::vec4 normal(0.5f, 0.5f, 0.5f, 1.0f);
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(150.0f, 275.0f, 0.0f));
        model = glm::scale(model, glm::vec3(96.0f, 96.0f, 1.0f));
        player1->queue(model, players == 1 ? highlight : normal, animation_index, 0);
    }
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(125.0f, 175.0f, 0.0f));
        model = glm::scale(model, glm::vec3(96.0f, 96.0f, 1.0f));
        player1->queue(model, players == 2 ? highlight : normal, animation_index, 0);
    }
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(150.0f, 175.0f, 0.0f));
        model = glm::scale(model, glm::vec3(96.0f, 96.0f, 1.0f));
        player2->queue(model, players == 2 ? highlight : normal, animation_index, 0);
    }
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(250.0f, 275.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 40.0f, 1.0f));
        font->write(model, players == 1 ? highlight : normal, "Single Player");
    }
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(250.0f, 175.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 40.0f, 1.0f));
        font->write(model, players == 2 ? highlight : normal, "Two Players");
    }
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(140.0f, 400.0f, 0.0f));
        model = glm::scale(model, glm::vec3(45.0f, 64.0f, 1.0f));
        font->write(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Dragonfly Chase!");
    }
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(100.0f, 100.0f, 0.0f));
        model = glm::scale(model, glm::vec3(600.0f, 400.0f, 1.0f));
        box.queue_frame(model, glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    }
    background->draw();
    box.draw();
    player1->draw();
    player2->draw();
    font->draw();
}
