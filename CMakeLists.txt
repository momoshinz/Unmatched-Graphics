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
    src/effect/DraculaCards/Ambush.cpp
    src/effect/DraculaCards/BaptismOfBlood.cpp
    src/effect/DraculaCards/BeastForm.cpp
    src/effect/DraculaCards/Dash.cpp
    src/effect/DraculaCards/Exploit.cpp
    src/effect/DraculaCards/FeedingFrenzy.cpp
    src/effect/DraculaCards/FeintDracula.cpp
    src/effect/DraculaCards/LookIntoMyEyes.cpp
    src/effect/DraculaCards/MistForm.cpp
    src/effect/DraculaCards/PreyUpon.cpp
    src/effect/DraculaCards/RaveningSeduction.cpp
    src/effect/DraculaCards/ThirstForSustenance.cpp
    src/effect/SherlockCards/AdministerAid.cpp
    src/effect/SherlockCards/CounterPunch.cpp
    src/effect/SherlockCards/DeduceStrategy.cpp
    src/effect/SherlockCards/EducationNeverEnds.cpp
    src/effect/SherlockCards/EliminateTheImpossible.cpp
    src/effect/SherlockCards/FeintSherlock.cpp
    src/effect/SherlockCards/FixedPoint.cpp
    src/effect/SherlockCards/MasterOfDisguise.cpp
    src/effect/SherlockCards/StudyMethods.cpp
    src/effect/SherlockCards/TheGameIsAfoot.cpp
    src/effect/InvisibleManCards/CodedNotes.cpp
    src/effect/InvisibleManCards/DreamingOfRevenge.cpp
    src/effect/InvisibleManCards/EmergeFromMist.cpp
    src/effect/InvisibleManCards/ImpossibleToSee.cpp
    src/effect/InvisibleManCards/IntoThinAir.cpp
    src/effect/InvisibleManCards/Lurking.cpp
    src/effect/InvisibleManCards/ReignOfTerror.cpp
    src/effect/InvisibleManCards/RollingFog.cpp
    src/effect/InvisibleManCards/SlipAway.cpp
    src/effect/InvisibleManCards/StepLightly.cpp

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