cmake_minimum_required(VERSION 3.16)

project(FlopFish VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-O3)

add_library(flopfish_engine
    src/move.cpp
    src/move_generation.cpp
    src/move_application.cpp
    src/position.cpp
    src/utils.cpp
    src/zobrist_keys.cpp
    src/flopfish_v1.cpp
    src/flopfish_v2.cpp
    src/flopfish_v3.cpp
)

target_include_directories(flopfish_engine PUBLIC include)

add_executable(flopfish src/game.cpp)
target_link_libraries(flopfish PRIVATE flopfish_engine)

add_executable(perft tests/perft.cpp)
target_link_libraries(perft PRIVATE flopfish_engine)
