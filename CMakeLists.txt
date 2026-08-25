cmake_minimum_required(VERSION 3.10)

project(Unmatched-Graphics)

# --------------------------------------------------
# Dependencies
# --------------------------------------------------

include(FetchContent)

# nlohmann/json
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

set(SUPPORT_FILEFORMAT_JPG ON CACHE BOOL "Support JPG textures" FORCE)

FetchContent_MakeAvailable(raylib)


# --------------------------------------------------
# C++ Standard
# --------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# --------------------------------------------------
# Include Directory
# --------------------------------------------------

include_directories(include)


# --------------------------------------------------
# Source Files
# --------------------------------------------------

set(SOURCES

    src/main.cpp
    src/board/Board.cpp
    src/board/Space.cpp
    src/card/Card.cpp
    src/card/Deck.cpp
    src/card/DiscardPile.cpp
    src/card/Hand.cpp

    src/fighter/Fighter.cpp
    src/fighter/Hero.cpp
    src/fighter/Sidekick.cpp
    src/fighter/Dracula.cpp
    src/fighter/Sherlock.cpp
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

    src/game/Game.cpp
    src/game/CombatSystem.cpp
    src/game/TurnManager.cpp

    src/player/Player.cpp

    src/graphics/AssetManager.cpp
    src/graphics/LoadingScreen.cpp
    src/graphics/MainMenu.cpp
    src/graphics/GameScreen.cpp
    src/graphics/MapCoordinates.cpp
    src/graphics/PlayerPanel.cpp
    src/graphics/AttackUI.cpp
    src/graphics/SchemeUI.cpp
    src/graphics/ManeuverUI.cpp
    src/graphics/EffectUI.cpp
    src/graphics/SaveManager.cpp
    src/graphics/Transition.cpp
    src/graphics/DraculaAbilityUI.cpp
    
    src/ui/TerminalUI.cpp
)


# --------------------------------------------------
# Executable
# --------------------------------------------------

add_executable(Unmatched-Graphics ${SOURCES})


# --------------------------------------------------
# Libraries
# --------------------------------------------------

target_link_libraries(
    Unmatched-Graphics
    PRIVATE
    nlohmann_json::nlohmann_json
    raylib
)

add_custom_command(TARGET Unmatched-Graphics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Unmatched_Assets"
    "$<TARGET_FILE_DIR:Unmatched-Graphics>/Unmatched_Assets"
)
