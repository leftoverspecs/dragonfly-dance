#include "background.hpp"
#include "bubbles.hpp"
#include "dragonflies.hpp"
#include "player.hpp"
#include "music.hpp"


#include <SDL2/SDL_main.h>

#include <audio/audio.hpp>

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
          controller1(0),
          controller2(1),
          bubbles(WIDTH, HEIGHT),
          background(WIDTH, HEIGHT),
          player1(controller1, bubbles, true, 0, 0, WIDTH, HEIGHT),
          player2(controller2, bubbles, false, WIDTH - 10, 10, WIDTH, HEIGHT),
          dragonflies(bubbles, WIDTH, HEIGHT),
          sprites(floor_stone_png, sizeof(floor_stone_png), 1, 1),
          renderer(sprites, WIDTH, HEIGHT) {
        player1.set_other(player2);
        player2.set_other(player1);
    }


private:
    float time{0.0f};
    engine::sdl::OpenGlWindow window;
    Music music;
    engine::sdl::Controller controller1;
    engine::sdl::Controller controller2;
    Bubbles bubbles;
    Background background;
    Player player1;
    Player player2;
    Dragonflies dragonflies;
    engine::opengl::SpriteMap sprites;
    engine::opengl::SpriteRenderer renderer;

    void on_startup() override {
        music.play();
    }

    void on_loop(float delta_time) override {
        time += delta_time;
        bubbles.update(delta_time);
        player1.update(delta_time);
        player2.update(delta_time);
        dragonflies.update(delta_time);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if 0
        /*renderer.clear();
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
#endif
        background.draw();
        player1.draw();
        player2.draw();
        dragonflies.draw();
        bubbles.draw(time);
    }
};


int main(int argc, char *argv[]) {
    engine::sdl::initialize init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO);
    engine::audio::Audio audio(44100, MIX_DEFAULT_FORMAT, 2, 128);
    SpriteTestScene scene;
    scene.run();
    return 0;
}
