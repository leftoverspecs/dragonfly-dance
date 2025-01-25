#include "background.hpp"
#include "boxyfont.hpp"
#include "bubbles.hpp"
#include "game.hpp"
#include "intro.hpp"
#include "music.hpp"
#include "player.hpp"


#include <audio/audio.hpp>

#include <opengl/destination.hpp>
#include <opengl/spritemap.hpp>
#include <opengl/spriterenderer.hpp>

#include <sdl/initialize.hpp>
#include <sdl/openglwindow.hpp>
#include <sdl/scene.hpp>

#include <glm/gtx/transform.hpp>

#include <assets/player.png.h>
#include <assets/player2.png.h>

#ifdef _WIN32
extern "C" {

    _declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
}
#endif

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

int main(int argc, char *argv[]) {
    engine::sdl::initialize init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO);
    engine::sdl::OpenGlWindow window("Dragonfly Chase!", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WIDTH,
                                        HEIGHT,
                                        SDL_WINDOW_SHOWN);
    engine::opengl::Screen screen(WIDTH, HEIGHT);
    engine::audio::Audio audio(44100, MIX_DEFAULT_FORMAT, 2, 128);
    Background background(WIDTH, HEIGHT);
    engine::sdl::Controller controller1(0);
    engine::sdl::Controller controller2(1);
    engine::opengl::SpriteMap player1map(player_png, sizeof(player_png), 9, 7);
    engine::opengl::SpriteMap player2map(player2_png, sizeof(player2_png), 13, 16);
    engine::opengl::SpriteRenderer player1(player1map, WIDTH, HEIGHT);
    engine::opengl::SpriteRenderer player2(player2map, WIDTH, HEIGHT);
    BoxyFont font(WIDTH, HEIGHT);
    Music music;
    Intro intro(WIDTH, HEIGHT, screen, window, controller1, controller2, background, player1, player2, font, music);
    while (true) {
        controller1.reset();
        controller2.reset();
        if (!intro.run()) {
            break;
        }
        controller1.reset();
        controller2.reset();
        Game game(WIDTH, HEIGHT, screen, window, controller1, controller2, background, player1, player2, font, music, intro.get_players());
        if (!game.run()) {
            break;
        }
    }
    return 0;
}
