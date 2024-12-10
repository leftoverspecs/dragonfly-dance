cmake_minimum_required(VERSION 3.30)
project(ggj2025 CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_net REQUIRED CONFIG)

add_subdirectory(engine)
