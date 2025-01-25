#include "background.hpp"

#include <assets/background.png.h>
#include <glm/gtx/transform.hpp>

Background::Background(GLfloat width, GLfloat height)
    : map(background_png, sizeof(background_png), 1, 1),
      renderer(map, width, height),
      width(width), height(height) {}

void Background::draw() {
    renderer.clear();
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = scale(model, glm::vec3(width, height, 1.0));
    renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
    renderer.draw();
}
