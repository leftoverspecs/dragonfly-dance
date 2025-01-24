#include "bubbles.hpp"


#include <SDL2/SDL_main.h>

#include <opengl/destination.hpp>
#include <opengl/spritemap.hpp>
#include <opengl/spriterenderer.hpp>

#include <sdl/initialize.hpp>
#include <sdl/openglwindow.hpp>
#include <sdl/scene.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <assets/floor-stone.png.h>

#ifdef _WIN32
extern "C" {

_declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
}
#endif

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

class SpriteTestScene : public engine::sdl::Scene {
public:
    SpriteTestScene()
        : Scene(HEIGHT, window), window("Floor", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WIDTH,
                                        HEIGHT,
                                        SDL_WINDOW_SHOWN),
          bubbles(WIDTH, HEIGHT),
          sprites(floor_stone_png, sizeof(floor_stone_png), 1, 1),
          renderer(sprites, WIDTH, HEIGHT) {}


private:
    engine::sdl::OpenGlWindow window;
    Bubbles bubbles;
    engine::opengl::SpriteMap sprites;
    engine::opengl::SpriteRenderer renderer;

    void on_loop(float delta_time) override {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            auto target = bubbles.bind_as_target();
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderer.clear();
            for (int x = 0; x < 5; ++x) {
                for (int y = 0; y < 5; ++y) {
                    glm::mat4 model(1.0f);
                    model = translate(model, glm::vec3(std::sin(SDL_GetTicks64() / 1000.0) * 50.0f, 0, 0));
                    model = scale(model, 2.0f * glm::vec3(sprites.get_sprite_width(), sprites.get_sprite_height(), 1.0f));
                    //model = translate(model, glm::vec3(static_cast<float>(x) / 2.0f + static_cast<float>(y) / 2.0, 5 - static_cast<float>(y) / 2.0 + static_cast<float>(x) / 2.0f, 0.0f));
                    model = translate(model, glm::vec3(
                        2.0f + static_cast<float>(x) / 2.0f - static_cast<float>(y) / 2.0,
                        5.0f - static_cast<float>(x) / 2.0f - static_cast<float>(y) / 2.0 /*+ 0.5f * std::sinf((x/2.0f + y) / M_PI_2 + SDL_GetTicks64() / 500.0f)*/, 0.0f));
                    //model = rotate(model, glm::radians(360.0f * static_cast<float>(SDL_GetTicks64()) / 1000.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    //model = translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
                    //model = glm::scale(model, glm::vec3(width - 20.0f, height - 20.0f, 1.0f));
                    renderer.queue(model, glm::vec4(1.0, 1.0f, 1.0f, 1.0f), 0, 0);

                }
            }
            renderer.draw();
        }
        bubbles.draw();
    }
};


int main(int argc, char *argv[]) {
    engine::sdl::initialize init(SDL_INIT_VIDEO);
    SpriteTestScene scene;
    scene.run();
    return 0;
}
