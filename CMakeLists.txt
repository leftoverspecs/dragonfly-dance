cmake_minimum_required(VERSION 3.30)
project(dragonflydance CXX)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)

add_subdirectory(engine)

add_executable(dragonflydance WIN32 src/game.cpp
    src/boxyfont.cpp
    src/boxyfont.hpp
    src/bubbles.cpp
    src/bubbles.hpp
    src/bubbles_sprite.cpp
    src/bubbles_sprite.hpp
    src/background.cpp
    src/background.hpp
    src/player.cpp
    src/player.hpp
    src/dragonflies.cpp
    src/dragonflies.hpp
    src/music.cpp
    src/music.hpp
    src/score.cpp
    src/score.hpp
    src/timer.cpp
    src/timer.hpp
    src/intro.cpp
    src/intro.hpp
    src/main.cpp
    src/game.hpp
    src/supply.cpp
    src/supply.hpp
    #src/audiofx.cpp
    #src/audiofx.hpp
)
target_link_libraries(dragonflydance PRIVATE engine SDL2::SDL2main SDL2_mixer::SDL2_mixer)
target_assets(dragonflydance PRIVATE
    assets/bubbles.png
    assets/background.png
    assets/player.png
    assets/player2.png
    assets/dragonfly.png
    assets/dragonfly.ogg
    assets/intro.ogg
    assets/boxyfont.png
    assets/logo.png
)
target_include_directories(dragonflydance PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
