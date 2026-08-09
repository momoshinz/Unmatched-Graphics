cmake_minimum_required(VERSION 3.10)

project(Unmatched-Graphics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
)

FetchContent_MakeAvailable(raylib)

# Main source
add_executable(Unmatched-Graphics
    src/main.cpp
)

target_link_libraries(Unmatched-Graphics
    PRIVATE raylib
)