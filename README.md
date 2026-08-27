# 🕯️ Unmatched Game

Welcome to Unmatched Game, a graphical adaptation of the famous Unmatched board game.
This project recreates the strategic one-versus-one experience of the original game in a 
fully interactive graphical environment using C++ and Raylib.
Players take control of legendary heroes such as Sherlock Holmes, Count Dracula, and The Invisible Man, 
each equipped with unique abilities, sidekicks, and customized decks of cards.
Every match requires tactical movement, careful card management, strategic use of abilities, 
and intelligent combat decisions to defeat the opposing hero.
This project was developed as an Object-Oriented Programming project and demonstrates modular software architecture, 
game design principles, graphical user interface development, and clean C++ programming practices.

## 🎲 Heroes

Currently implemented fighters:

* 🕵️ Sherlock Holmes

  * **Special Ability:** Sherlock Holmes and Dr. Watson's abilities can never be disabled.

* 🧛 Count Dracula

  * **Special Ability:** At the start of your turn, you may deal 1 damage to a fighter adjacent to Dracula (even your sisters). If you do, draw a card.

* 🌫️ The Invisible Man

  * **Special Ability:** The Invisible Man can use Fog to move between spaces and conceal his position, making him difficult
  * to target and allowing for strategic movement across the battlefield.

## 📂 Project Structure
```
Unmatched/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── .gitmodules
├── external/
│   └── ftxui/
├── include/
│   ├── board/
│   │   ├── Board.h
│   │   └── Space.h
│   ├── card/
│   │   ├── Card.h
│   │   ├── Deck.h
│   │   ├── DiscardPile.h
│   │   └── Hand.h
│   ├── effect/
│   │   ├── Effect.h
│   │   ├── DraculaCards/
│   │   └── SherlockCards/
│   ├── fighter/
│   │   ├── Fighter.h
│   │   ├── Hero.h
│   │   ├── Sidekick.h
│   │   ├── Dracula.h
│   │   ├── Sherlock.h
│   │   ├── Sisters.h
│   │   └── DrWatson.h
│   ├── game/
│   │   ├── Game.h
│   │   ├── CombatSystem.h
│   │   └── TurnManager.h
│   ├── player/
│   │   └── Player.h
│   ├── ui/
│   │   ├── MainMenu.h
│   │   ├── TerminalUI.h
│   │   └── MapTemplate.h
│   └── utils/
└── src/
    ├── main.cpp
    ├── board/
    │   ├── Board.cpp
    │   └── Space.cpp
    ├── card/
    │   ├── Card.cpp
    │   ├── Deck.cpp
    │   ├── DiscardPile.cpp
    │   └── Hand.cpp
    ├── effect/
    │   ├── Effect.cpp
    │   ├── DraculaCards/
    │   └── SherlockCards/
    ├── fighter/
    │   ├── Fighter.cpp
    │   ├── Hero.cpp
    │   ├── Sidekick.cpp
    │   ├── Dracula.cpp
    │   ├── Sherlock.cpp
    │   ├── Sisters.cpp
    │   └── DrWatson.cpp
    ├── game/
    │   ├── Game.cpp
    │   ├── CombatSystem.cpp
    │   └── TurnManager.cpp
    │── graphics/
    │   ├── AssetManager.cpp
    │   ├── AttackUI.cpp
    │   ├── DraculaAbilityUI.cpp
    │   ├── EffectUI.cpp
    │   ├── GameScreen.cpp
    │   ├── HandViewUI.cpp
    │   ├── LoadingScreen.cpp
    │   ├── MainMenu.cpp
    │   ├── ManeuverUI.cpp
    │   ├── MapCoordinates.cpp
    │   ├── PlayerPanel.cpp
    │   ├── SaveManager.cpp
    │   ├── SchemeUI.cpp
    │   ├── Transition.cpp
    │   └── WinnerUI.cpp     
    ├── player/
    │   └── Player.cpp
    └── ui/
    │   └── TerminalUI.cpp
    │── main.cpp
```

## 🛠 Technologies Used

* **C++17** — Main programming language
* **CMake** — Build system and dependency management
* **Raylib 6.0** — Graphics, rendering, window management, input handling, and audio
* **nlohmann/json 3.12.0** — JSON serialization for saving and loading game data
* **Object-Oriented Programming (OOP)**
* **Git / GitHub** — Version control and project collaboration


## How to run

### Prerequisites
* **C++17 Compiler**
* **CMake 3.20+**
* **Git**

### Clone the Repository
```bash
git clone https://github.com/momoshinz/Unmatched-Graphics.git
cd Unmatched
```

### 📦 Download Game Assets
The game requires the `Unmatched_Assets` folder to run properly.

Download the assets from the following link:

```text
<https://drive.google.com/file/d/1rFYiy6056YsT2NlpfybQbjv8QEl7wZyQ/view?usp=drivesdk>
```

After downloading, extract the assets and make sure the folder 
is placed in the **root directory of the project**:

```text
Unmatched/
├── CMakeLists.txt
├── include/
├── src/
├── Unmatched_Assets/
└── ...
```

The `Unmatched_Assets` folder must be located next to `CMakeLists.txt`.

> **Important:** Do not rename the `Unmatched_Assets` folder, as the application expects this exact directory name.

### 🔨 Build the Project

#### Linux

```bash
cmake -S . -B build
cmake --build build
```

Run the application:

```bash
./build/Unmatched-Graphics
```

#### Windows

Using MinGW:

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

Run the application:

```bash
build\Unmatched-Graphics.exe
```

### 📁 Assets After Build

CMake automatically copies the `Unmatched_Assets` directory next to the executable after building.

The resulting structure will look similar to:

```text
build/
├── Unmatched-Graphics.exe
└── Unmatched_Assets/
    ├── ...
```

This allows the game to load its required textures,
maps, fonts, audio, and other resources at runtime.

## 🎮 Gameplay
Each player controls one Hero and their Sidekicks.

During every turn:
1. Move fighters across the map.
2. Play Attack, Defense, Versatile or Scheme cards.
3. Trigger special abilities.
4. Defeat the opponent's Hero to win the match.

## 👨‍💻 Team Members
- [Mohadese Nejatbakhsh](https://github.com/momoshinz)
- [Dina Sharifypanah](https://github.com/Dinanooo)

## Repository Link
https://github.com/momoshinz/Unmatched-Graphics.git
