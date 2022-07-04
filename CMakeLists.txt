cmake_minimum_required(VERSION 3.22)
project(Snake)

set(CMAKE_CXX_STANDARD 20)

add_executable(Snake main.cpp)
target_link_libraries(Snake SDL2 SDL2_ttf fftw)