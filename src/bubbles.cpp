#include "bubbles.hpp"

#include "bubbles_sprite.hpp"

#include <glm/gtx/transform.hpp>


namespace {

GLfloat screen_vertex_data[] = {
    /*   X      Y     S     T    */
    -2.0f, -2.0f, -0.5f, -0.5f,
    -2.0f, 2.0f, -0.5f, 1.5f,
    2.0f, 2.0f, 1.5f, 1.5f,

    -2.0f, -2.0f, -0.5f, -0.5f,
    2.0f, 2.0f, 1.5f, 1.5f,
    2.0f, -2.0f, 1.5f, -0.5f};

constexpr int ATTRIBUTE_POSITION = 0;
constexpr int ATTRIBUTE_TEXTURE_COORD = 1;

}

Bubbles::Bubbles(GLsizei width, GLsizei height)
    : map(bubbles_sprite, bubbles_sprite_size, 24, 16),
      renderer(map, width, height),
      screen_width(width), screen_height(height) {
    for (int i = 0; i < 10; ++i) {
        add_bubble(glm::vec2(width * static_cast<GLfloat>(rand()) / RAND_MAX, height * static_cast<GLfloat>(rand()) / RAND_MAX),
            0.5 * height * static_cast<GLfloat>(rand()) / RAND_MAX);
    }
    /*bubbles.push_back(BubbleEntry{0.5f});
    bubbles.push_back(BubbleEntry{0.5f});
    bubbles.push_back(BubbleEntry{0.07f});

    bubbles.push_back(BubbleEntry{0.75f});
    bubbles.push_back(BubbleEntry{0.75f});
    bubbles.push_back(BubbleEntry{0.08f});

    bubbles.push_back(BubbleEntry{0.25f});
    bubbles.push_back(BubbleEntry{0.25f});
    bubbles.push_back(BubbleEntry{0.02f});*/
}

void Bubbles::draw(float time) {
    renderer.clear();
    for (const auto &bubble : bubbles) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(bubble.position, 0.0f));
        model = scale(model, glm::vec3(bubble.radius, bubble.radius, 1.0));
        model = translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
        renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 16, 8, 4, 4);
    }
    renderer.draw();
}

void Bubbles::add_bubble(glm::vec2 position, GLfloat radius) {
    bubbles.push_back(Bubble{position, radius});
}
