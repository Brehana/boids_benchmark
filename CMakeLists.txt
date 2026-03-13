cmake_minimum_required(VERSION 3.10)
project(BoidsBenchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
find_package(SDL2 REQUIRED)

pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)

add_executable(boids
    src/main.cpp
    src/boids.cpp
    src/benchmark.cpp
    src/visualizer.cpp
)

target_include_directories(boids PRIVATE
    src
)

target_compile_options(boids PRIVATE -O2 -Wall -Wextra -Wpedantic)

target_link_libraries(boids PRIVATE
    SDL2::SDL2
    PkgConfig::SDL2_TTF
)