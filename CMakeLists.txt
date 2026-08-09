cmake_minimum_required(VERSION 3.10)

project(SimpleCppProject)

include(FetchContent)

# JSON
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(json)

# raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
)

FetchContent_MakeAvailable(raylib)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/board/Board.cpp
    src/board/Space.cpp
    src/card/Card.cpp
    src/card/Deck.cpp
    src/card/DiscardPile.cpp
    src/card/Hand.cpp

    src/fighter/Dracula.cpp
    src/fighter/Fighter.cpp
    src/fighter/Hero.cpp
    src/fighter/Sherlock.cpp
    src/fighter/Sidekick.cpp
    src/fighter/Sisters.cpp
    src/fighter/DrWatson.cpp
    src/fighter/InvisibleMan.cpp
    src/fighter/Fog.cpp

    src/effect/Effect.cpp
    # ... بقیه effect ها

    src/game/CombatSystem.cpp
    src/game/Game.cpp
    src/game/TurnManager.cpp

    src/player/Player.cpp

    src/ui/TerminalUI.cpp
    src/ui/MainMenu.cpp
)

add_executable(Unmatched-Graphics ${SOURCES})

target_link_libraries(Unmatched-Graphics
    PRIVATE
    nlohmann_json::nlohmann_json
    raylib
)