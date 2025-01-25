cmake_minimum_required(VERSION 3.30)
project(ggj2025 CXX)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_net REQUIRED CONFIG)

add_subdirectory(engine)

add_executable(game WIN32 src/game.cpp
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
)
target_link_libraries(game PRIVATE engine SDL2::SDL2main)
target_assets(game PRIVATE
    assets/floor-stone.png
    assets/bubbles.png
    assets/background.png
    assets/player.png
    assets/player2.png
    assets/dragonfly.png
)
target_include_directories(game PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
