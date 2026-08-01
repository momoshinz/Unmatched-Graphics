cmake_minimum_required(VERSION 3.10)

# Project name
project(SimpleCppProject)

include(FetchContent)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG v6.1.9
)

FetchContent_MakeAvailable(ftxui)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the include directory for header files
include_directories(include)

# Define source files
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
    src/game/CombatSystem.cpp
    src/game/Game.cpp
    src/game/TurnManager.cpp
    src/player/Player.cpp
    src/ui/TerminalUI.cpp
    src/ui/MainMenu.cpp
)

# Build the executable
add_executable(Unmatched ${SOURCES})

target_link_libraries(Unmatched
    PRIVATE
        ftxui::screen
        ftxui::dom
        ftxui::component
)