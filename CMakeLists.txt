cmake_minimum_required(VERSION 3.13)
project(rcc)

file(GLOB_RECURSE SRC "src/*.cpp")

add_executable(rcc ${SRC})

if (MSVC)
    target_compile_options(rcc PRIVATE /std:c++17)
endif()