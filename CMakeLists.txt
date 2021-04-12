cmake_minimum_required(VERSION 3.12)
project(Console_Frame_Animation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Console_Frame_Animation main.cpp libs/console.cpp libs/console.h libs/animation.cpp libs/animation.h)