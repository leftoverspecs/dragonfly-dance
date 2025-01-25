#include "background.hpp"
#include "boxyfont.hpp"
#include "bubbles.hpp"
#include "dragonflies.hpp"
#include "music.hpp"
#include "player.hpp"
#include "score.hpp"
#include "timer.hpp"


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
          font(WIDTH, HEIGHT),
          controller1(0),
          controller2(1),
          score(font),
          bubbles(WIDTH, HEIGHT, score),
          background(WIDTH, HEIGHT),
          player1(controller1, bubbles, true, 0, 0, WIDTH, HEIGHT),
          player2(controller2, bubbles, false, WIDTH - 10, 10, WIDTH, HEIGHT),
          timer(font),
          dragonflies(bubbles, timer, WIDTH, HEIGHT),
          sprites(floor_stone_png, sizeof(floor_stone_png), 1, 1),
          renderer(sprites, WIDTH, HEIGHT) {
        player1.set_other(player2);
        player2.set_other(player1);
    }


private:
    float time{0.0f};
    engine::sdl::OpenGlWindow window;
    BoxyFont font;
    Music music;
    engine::sdl::Controller controller1;
    engine::sdl::Controller controller2;
    Score score;
    Bubbles bubbles;
    Background background;
    Player player1;
    Player player2;
    Timer timer;
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
        timer.update(delta_time);
        dragonflies.update(delta_time);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        font.clear();
        timer.draw();
        score.draw();
        background.draw();
        player1.draw();
        player2.draw();
        dragonflies.draw();
        bubbles.draw(time);
        font.draw();
    }
};


int main(int argc, char *argv[]) {
    engine::sdl::initialize init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO);
    engine::audio::Audio audio(44100, MIX_DEFAULT_FORMAT, 2, 128);
    SpriteTestScene scene;
    scene.run();
    return 0;
}
