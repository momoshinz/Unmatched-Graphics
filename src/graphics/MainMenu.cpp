#include "graphics/MainMenu.h"
#include <cctype>
#include <fstream>
#include "fighter/Hero.h"
#include "player/Player.h"
#include "board/Space.h"
#include "graphics/MapCoordinates.h"
#include "graphics/GameScreen.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

MainMenu::MainMenu(AssetManager *assets, Game *game)
    : assets(assets),
      game(game),
      state(State::MAIN_MENU),
      saveManager("saves"),
      player1Name(""),
      player1Age(""),
      player2Name(""),
      player2Age(""),
      enteringName(true),
      enteringAge(false),
      selectedSave(-1),
      currentHeroPlayer(HeroSelectionPlayer::PLAYER_1),
      player1Hero(""),
      player2Hero(""),
      player1HeroSelected(false),
      player2HeroSelected(false),
      placement(Placement::YOUNGER_HERO),
      selectedStartSpace(-1),
      placementPlayer(1),
      placementHeroPlaced(false),
      placementStartSpace(-1),
      placementSidekickIndex(0),
      currentFogIndex(0),
      fogForYounger(false)
{
}

bool MainMenu::isPlayer1Complete() const
{
    return !player1Name.empty() && !player1Age.empty();
}

bool MainMenu::isPlayer2Complete() const
{
    return !player2Name.empty() && !player2Age.empty();
}

void loadSaveFiles(std::vector<std::string> &saveFiles)
{
    saveFiles.clear();

    int slot = 1;

    while (true)
    {
        string filename = "save" + std::to_string(slot) + ".json";

        string path = "saves/" + filename;

        ifstream file(path);

        if (!file)
            break;

        saveFiles.push_back(filename);

        slot++;
    }
}

void MainMenu::update()
{
    if (assets == nullptr)
    {
        return;
    }

    if (state == State::PLAYER_1_INPUT)
    {
        int key = GetCharPressed();

        while (key > 0)
        {
            if (enteringName)
            {
                if (key >= 32 && key <= 125)
                {
                    player1Name += static_cast<char>(key);
                }
            }

            if (enteringAge)
            {
                if (key >= '0' && key <= '9')
                {
                    player1Age += static_cast<char>(key);
                }
            }

            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (enteringName && !player1Name.empty())
            {
                player1Name.pop_back();
            }

            if (enteringAge && !player1Age.empty())
            {
                player1Age.pop_back();
            }
        }

        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ENTER))
        {
            if (enteringName && !player1Name.empty())
            {
                enteringName = false;
                enteringAge = true;
            }
        }
    }

    if (state == State::PLAYER_2_INPUT)
    {
        int key = GetCharPressed();

        while (key > 0)
        {
            if (enteringName)
            {
                if (key >= 32 && key <= 125)
                {
                    player2Name += static_cast<char>(key);
                }
            }

            if (enteringAge)
            {
                if (key >= '0' && key <= '9')
                {
                    player2Age += static_cast<char>(key);
                }
            }

            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (enteringName && !player2Name.empty())
            {
                player2Name.pop_back();
            }

            if (enteringAge && !player2Age.empty())
            {
                player2Age.pop_back();
            }
        }

        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ENTER))
        {
            if (enteringName && !player2Name.empty())
            {
                enteringName = false;
                enteringAge = true;
            }
        }
    }
}

void MainMenu::draw()
{
    if (assets == nullptr)
    {
        return;
    }

    Texture2D background = assets->getMainMenuBackground();
    Font font = assets->getGameFont();
    Font titleFont = assets->getTitleFont();

    if (background.id != 0)
    {
        DrawTexturePro(background,
            Rectangle{0, 0,
                      static_cast<float>(background.width),
                      static_cast<float>(background.height)},
            Rectangle{0, 0,
                      static_cast<float>(GetScreenWidth()),
                      static_cast<float>(GetScreenHeight())},
                      Vector2{0, 0},
                      0.0f,
                      WHITE);
    }

    if (state != State::LOAD_GAME)
    {
        const char *title = "UNMATCHED";
        const float titleSize = 58.0f;
        const float titleSpacing = 10.0f;

        Vector2 titleTextSize = MeasureTextEx(titleFont, title, titleSize, titleSpacing);

        const float titleX = (GetScreenWidth() - titleTextSize.x) / 2.0f;

        const float titleY = 100.0f;

        DrawTextEx(titleFont, title, Vector2{titleX, titleY}, titleSize, titleSpacing, WHITE);
    }

    if (state == State::MAIN_MENU)
    {
        const float buttonWidth = 300.0f;
        const float buttonHeight = 75.0f;

        const float buttonX = (GetScreenWidth() - buttonWidth) / 2.0f;

        const float firstButtonY = 390.0f;
        const float buttonGap = 25.0f;

        Rectangle newGameButton{buttonX, firstButtonY, buttonWidth, buttonHeight};

        Rectangle loadGameButton{buttonX, firstButtonY + buttonHeight + buttonGap,
                                 buttonWidth, buttonHeight};

        Rectangle exitButton{buttonX, firstButtonY + 2.0f * (buttonHeight + buttonGap),
                             buttonWidth, buttonHeight};

        Vector2 mousePosition = GetMousePosition();

        Color normalColor{30, 30, 30, 220};

        Color hoverColor{60, 60, 60, 235};

        Color newGameColor = CheckCollisionPointRec(mousePosition, newGameButton) ? hoverColor : normalColor;

        DrawRectangleRounded(newGameButton, 1.0f, 32, newGameColor);

        Color loadGameColor = CheckCollisionPointRec(mousePosition, loadGameButton) ? hoverColor : normalColor;

        DrawRectangleRounded(loadGameButton, 1.0f, 32, loadGameColor);

        Color exitColor = CheckCollisionPointRec(mousePosition, exitButton) ? hoverColor : normalColor;

        DrawRectangleRounded(exitButton, 1.0f, 32, exitColor);

        const float buttonFontSize = 35.0f;
        const float buttonSpacing = 2.0f;
        const char *newGameText = "NEW GAME";
        const char *loadGameText = "LOAD GAME";
        const char *exitText = "EXIT";

        Vector2 newGameTextSize = MeasureTextEx(font, newGameText, buttonFontSize, buttonSpacing);

        Vector2 loadGameTextSize = MeasureTextEx(font, loadGameText, buttonFontSize, buttonSpacing);

        Vector2 exitTextSize = MeasureTextEx(font, exitText, buttonFontSize, buttonSpacing);

        DrawTextEx(font, newGameText,
                   Vector2{newGameButton.x + (newGameButton.width - newGameTextSize.x) / 2.0f,
                           newGameButton.y + (newGameButton.height - newGameTextSize.y) / 2.0f},
                           buttonFontSize, buttonSpacing, WHITE);

        DrawTextEx(font, loadGameText,
                   Vector2{loadGameButton.x + (loadGameButton.width - loadGameTextSize.x) / 2.0f,
                           loadGameButton.y + (loadGameButton.height - loadGameTextSize.y) / 2.0f},
                           buttonFontSize, buttonSpacing, WHITE);

        DrawTextEx(font, exitText,
                   Vector2{exitButton.x + (exitButton.width - exitTextSize.x) / 2.0f,
                           exitButton.y + (exitButton.height - exitTextSize.y) / 2.0f},
                           buttonFontSize, buttonSpacing, WHITE);
    }

    if (state == State::PLAYER_1_INPUT)
    {
        drawPlayerInput(
            font,
            "PLAYER 1",
            player1Name,
            player1Age,
            enteringName,
            enteringAge,
            "NEXT");
    }

    if (state == State::PLAYER_2_INPUT)
    {
        drawPlayerInput(
            font,
            "PLAYER 2",
            player2Name,
            player2Age,
            enteringName,
            enteringAge,
            "FINISH");
    }

    if (state == State::LOAD_GAME)
    {
        const char *loadTitle = "SAVED GAMES";

        Vector2 loadTitleSize = MeasureTextEx(titleFont, loadTitle, 50.0f, 2.0f);

        DrawTextEx(titleFont, loadTitle,
                   Vector2{(GetScreenWidth() - loadTitleSize.x) / 2.0f, 80.0f}, 50.0f, 2.0f, WHITE);

        float startY = 200.0f;
        const float buttonWidth = 400.0f;
        const float buttonHeight = 60.0f;
        const float buttonGap = 20.0f;

        const float buttonX = (GetScreenWidth() - buttonWidth) / 2.0f;

        Vector2 mousePosition = GetMousePosition();

        for (int i = 0; i < saveFiles.size(); i++)
        {
            Rectangle saveButton{buttonX, startY + i * (buttonHeight + buttonGap), buttonWidth, buttonHeight};

            Color normalColor{30, 30, 30, 220};

            Color hoverColor{60, 60, 60, 235};

            Color buttonColor = CheckCollisionPointRec(mousePosition, saveButton) ? hoverColor : normalColor;

            DrawRectangleRounded(saveButton, 1.0f, 32, buttonColor);

            std::string saveText = "SAVE " + std::to_string(i + 1);

            Vector2 saveTextSize = MeasureTextEx(font, saveText.c_str(), 25.0f, 2.0f);

            DrawTextEx(font, saveText.c_str(),
                        Vector2{saveButton.x + (saveButton.width - saveTextSize.x) / 2.0f,
                        saveButton.y + (saveButton.height - saveTextSize.y) / 2.0f}, 25.0f, 2.0f, WHITE);
        }

        if (saveFiles.empty())
        {
            const char *noSaveText = "No Saved Games Found!";

            Vector2 textSize =MeasureTextEx(font, noSaveText, 38.0f, 2.0f);

            DrawTextEx(font, noSaveText,

                Vector2{(GetScreenWidth() - textSize.x) / 2.0f, 500.0f}, 38.0f, 2.0f, WHITE);
        }

        Rectangle backButton{40.0f, GetScreenHeight() - 90.0f, 190.0f, 60.0f};

        Color backColor = CheckCollisionPointRec(mousePosition, backButton) ? Color{60, 60, 60, 235} : Color{30, 30, 30, 220};

        DrawRectangleRounded(backButton, 1.0f, 32, backColor);

        const char *backText = "BACK";

        Vector2 backTextSize = MeasureTextEx(font, backText, 30.0f, 2.0f);

        DrawTextEx(font, backText,
            Vector2{backButton.x + (backButton.width - backTextSize.x) / 2.0f,
                    backButton.y + (backButton.height - backTextSize.y) / 2.0f}, 30.f, 2.0f, WHITE);
    }

    if (state == State::READY)
    {
        const float infoX = GetScreenWidth() - 420.0f;

        const float player1Y = 180.0f;
        const float player2Y = 430.0f;

        const float playerTitleSize = 40.0f;
        const float playerInfoSize = 32.0f;
        const float textSpacing = 2.0f;


        DrawTextEx(font, "PLAYER 1", Vector2{infoX, player1Y}, playerTitleSize, textSpacing, WHITE);

        string player1Info = "Name: " + player1Name + "\nAge: " + player1Age;

        DrawTextEx(font, player1Info.c_str(),
            Vector2{infoX, player1Y + 55.0f}, playerInfoSize, textSpacing, WHITE);

        DrawTextEx(font, "PLAYER 2",
            Vector2{infoX, player2Y}, playerTitleSize, textSpacing, WHITE);

        string player2Info = "Name: " + player2Name + "\nAge: " + player2Age;

        DrawTextEx(font, player2Info.c_str(),
            Vector2{infoX, player2Y + 55.0f}, playerInfoSize, textSpacing, WHITE);

        const float startButtonWidth = 300.0f;
        const float startButtonHeight = 75.0f;

        const float startButtonX = (GetScreenWidth() - startButtonWidth) / 2.0f;

        const float startButtonY = (GetScreenHeight() - startButtonHeight) / 2.0f;

        Rectangle startButton{startButtonX, startButtonY, startButtonWidth, startButtonHeight};

        Vector2 mousePosition = GetMousePosition();

        Color normalStartColor{30, 30, 30, 220};

        Color hoverStartColor{60, 60, 60, 235};

        Color startButtonColor = CheckCollisionPointRec(mousePosition, startButton) ? hoverStartColor : normalStartColor;

        DrawRectangleRounded(startButton, 1.0f, 32, startButtonColor);

        const char *startText = "START ?";
        const float startFontSize = 35.0f;

        Vector2 startTextSize = MeasureTextEx(font, startText, startFontSize, textSpacing);

        DrawTextEx(font, startText,
            Vector2{startButton.x + (startButton.width - startTextSize.x) / 2.0f,
                    startButton.y + (startButton.height - startTextSize.y) / 2.0f}, startFontSize, textSpacing, WHITE);
    }

    if (state == State::HERO_SELECTION)
    {
        drawHeroSelection(font);
    }
    if (state == State::PLACEMENT)
    {
        drawPlacement(assets->getGameFont());
    }
}

void MainMenu::drawPlayerInput(Font font, const char *playerTitle, const std::string &name, const std::string &age,
                               bool enteringName, bool enteringAge, const char *buttonText)
{
    const float boxWidth = 450.0f;
    const float boxHeight = 60.0f;

    const float boxX = (GetScreenWidth() - boxWidth) / 2.0f;

    const float nameY = 330.0f;
    const float ageY = 470.0f;

    const float buttonWidth = 300.0f;
    const float buttonHeight = 70.0f;

    const float buttonX = (GetScreenWidth() - buttonWidth) / 2.0f;

    const float buttonY = 600.0f;

    const float firstButtonY = 370.0f;
    const float buttonGap = 20.0f;
    Rectangle newGameButton{buttonX, firstButtonY, buttonWidth, buttonHeight};

    Vector2 playerTitleSize = MeasureTextEx(font, playerTitle, 35.0f, 2.0f);

    DrawTextEx(font, playerTitle,
        Vector2{(GetScreenWidth() - playerTitleSize.x) / 2.0f, 220.0f}, 35.0f, 2.0f, WHITE);

    DrawTextEx(font, "PLAYER NAME:",
        Vector2{ boxX, nameY - 40.0f}, 25.0f, 2.0f, WHITE);

    Rectangle nameBox{boxX, nameY, boxWidth, boxHeight};

    Color nameColor = enteringName ? Color{60, 60, 60, 240} : Color{30, 30, 30, 220};

    DrawRectangleRounded(nameBox, 0.35f, 32, nameColor);

    DrawTextEx(font, name.c_str(),
        Vector2{nameBox.x + 20.0f, nameBox.y + 15.0f}, 25.0f, 2.0f, WHITE);


    DrawTextEx(font, "PLAYER AGE:",
        Vector2{boxX, ageY - 40.0f}, 25.0f, 2.0f, WHITE);

    Rectangle ageBox{boxX, ageY, boxWidth, boxHeight};

    Color ageColor = enteringAge ? Color{60, 60, 60, 240} : Color{30, 30, 30, 220};

    DrawRectangleRounded(ageBox, 0.35f, 32, ageColor);

    DrawTextEx(font, age.c_str(),
        Vector2{ageBox.x + 20.0f, ageBox.y + 15.0f}, 25.0f, 2.0f, WHITE);

    bool complete = !name.empty() && !age.empty();

    Vector2 mousePosition = GetMousePosition();

    Rectangle actionButton{buttonX, buttonY, buttonWidth, buttonHeight};

    Color normalButtonColor{30, 30, 30, 230};

    Color hoverButtonColor{60, 60, 60, 235};

    Color disabledButtonColor{30, 30, 30, 100};

    Color buttonColor;

    if (!complete)
    {
        buttonColor = disabledButtonColor;
    }
    else if (CheckCollisionPointRec(mousePosition, actionButton))
    {
        buttonColor = hoverButtonColor;
    }
    else
    {
        buttonColor = normalButtonColor;
    }

    DrawRectangleRounded(actionButton, 1.0f, 32, buttonColor);

    Vector2 buttonTextSize = MeasureTextEx(font, buttonText, 35.0f, 2.0f);

    DrawTextEx(font, buttonText,
        Vector2{buttonX + (buttonWidth - buttonTextSize.x) / 2.0f,
        buttonY + (buttonHeight - buttonTextSize.y) / 2.0f}, 35.0f, 2.0f,
        complete ? WHITE : Color{150, 150, 150, 150});

    const char *enterMessage = "> press Enter To Confirm Your Information.";

    Vector2 enterMessageSize = MeasureTextEx(font, enterMessage, 25.0f, 1.0f);

    DrawTextEx(font, enterMessage,
        Vector2{GetScreenWidth() - enterMessageSize.x - 20.0f, GetScreenHeight() - 
        enterMessageSize.y - 15.0f}, 25.0f, 1.0f, WHITE);
}

int MainMenu::handleInput()
{
    if (assets == nullptr)
    {
        return 0;
    }

    if (state == State::PLACEMENT)
    {
        updatePlacement();

        if (placement == Placement::FINISHED)
        {
            std::cout << "=== PLACEMENT FINISHED ===" << std::endl;

            state = State::MAIN_MENU;

            return 4;
        }

        return 0;
    }

    const float buttonWidth = 300.0f;
    const float buttonHeight = 60.0f;

    const float buttonX = (GetScreenWidth() - buttonWidth) / 2.0f;

    const float firstButtonY = 500.0f;
    const float buttonGap = 20.0f;

    Rectangle newGameButton{buttonX, firstButtonY, buttonWidth, buttonHeight};

    Rectangle loadGameButton{buttonX, firstButtonY + buttonHeight + buttonGap, buttonWidth, buttonHeight};

    Rectangle exitButton{buttonX, firstButtonY + 2.0f * (buttonHeight + buttonGap), buttonWidth, buttonHeight};

    Vector2 mousePosition = GetMousePosition();

    if (state == State::MAIN_MENU)
    {
        const float buttonWidth = 300.0f;
        const float buttonHeight = 75.0f;

        const float buttonX = (GetScreenWidth() - buttonWidth) / 2.0f;

        const float firstButtonY = 390.0f;
        const float buttonGap = 25.0f;

        Rectangle newGameButton{buttonX, firstButtonY, buttonWidth, buttonHeight};

        Rectangle loadGameButton{buttonX, firstButtonY + buttonHeight + buttonGap, buttonWidth, buttonHeight};

        Rectangle exitButton{buttonX, firstButtonY + 2.0f * (buttonHeight + buttonGap), buttonWidth, buttonHeight};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePosition, newGameButton))
            {
                player1Name.clear();
                player1Age.clear();

                player2Name.clear();
                player2Age.clear();

                enteringName = true;
                enteringAge = false;

                state = State::PLAYER_1_INPUT;

                return 1;
            }

            if (CheckCollisionPointRec(mousePosition, loadGameButton))
            {
                loadSaveFiles(saveFiles);

                selectedSave = -1;

                state = State::LOAD_GAME;

                return 2;
            }

            if (CheckCollisionPointRec(
                    mousePosition,
                    exitButton))
            {
                return 3;
            }
        }
    }


    if (state == State::PLAYER_1_INPUT)
    {
        const float buttonWidth = 300.0f;
        const float buttonHeight = 70.0f;

        const float buttonX = (GetScreenWidth() - buttonWidth) / 2.0f;

        const float buttonY = 600.0f;

        Rectangle nextButton{buttonX, buttonY, buttonWidth, buttonHeight};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePosition, nextButton))
            {
                if (isPlayer1Complete())
                {
                    enteringName = true;
                    enteringAge = false;

                    state = State::PLAYER_2_INPUT;
                }
            }
        }
    }

    if (state == State::PLAYER_2_INPUT)
    {
        const float buttonWidth = 300.0f;
        const float buttonHeight = 70.0f;

        const float buttonX = (GetScreenWidth() - buttonWidth) / 2.0f;

        const float buttonY = 600.0f;

        Rectangle finishButton{buttonX, buttonY, buttonWidth, buttonHeight};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePosition, finishButton))
            {
                if (isPlayer2Complete())
                {
                    state = State::READY;

                    enteringName = false;
                    enteringAge = false;

                    return 0;
                }
            }
        }
    }

    if (state == State::LOAD_GAME)
    {
        const float buttonWidth = 400.0f;
        const float buttonHeight = 60.0f;
        const float buttonGap = 20.0f;

        const float buttonX =
            (GetScreenWidth() - buttonWidth) / 2.0f;

        const float startY = 200.0f;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {

            for (int i = 0; i < saveFiles.size(); i++)
            {
                Rectangle saveButton{buttonX, startY + i * (buttonHeight + buttonGap), buttonWidth, buttonHeight};

                if (CheckCollisionPointRec(mousePosition, saveButton))
                {
                    selectedSave = i;

                    SaveManager saveManager;

                    try
                    {
                        saveManager.loadGame(game, saveFiles[i]);

                        return 5;
                    }
                    catch (const std::exception &e)
                    {
                        cout << "Load failed: " << e.what() << endl;
                        return 0;
                    }
                }
            }

            Rectangle backButton{40.0f, GetScreenHeight() - 80.0f, 160.0f, 50.0f};

            if (CheckCollisionPointRec(mousePosition, backButton))
            {
                state = State::MAIN_MENU;
                selectedSave = -1;
            }
        }

        return 0;
    }

    if (state == State::READY)
    {
        const float startButtonWidth = 300.0f;
        const float startButtonHeight = 75.0f;

        const float startButtonX = (GetScreenWidth() - startButtonWidth) / 2.0f;

        const float startButtonY = (GetScreenHeight() - startButtonHeight) / 2.0f;

        Rectangle startButton{startButtonX, startButtonY, startButtonWidth, startButtonHeight};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePosition, startButton))
            {
                startHeroSelection();
                return 0;
            }
        }
    }

    if (state == State::HERO_SELECTION)
    {
        const float boxWidth = 250.0f;
        const float boxHeight = 300.0f;
        const float gap = 35.0f;

        const float totalWidth = 3.0f * boxWidth + 2.0f * gap;

        const float startX = (GetScreenWidth() - totalWidth) / 2.0f;

        const float boxY = 300.0f;

        Rectangle draculaBox{startX, boxY, boxWidth, boxHeight};

        Rectangle sherlockBox{startX + boxWidth + gap, boxY, boxWidth, boxHeight};

        Rectangle invisibleBox{ startX + 2.0f * (boxWidth + gap), boxY, boxWidth, boxHeight};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePosition, draculaBox))
            {
                selectHero("dracula");
            }

            if (CheckCollisionPointRec(mousePosition, sherlockBox))
            {
                selectHero("sherlock");
            }

            if (CheckCollisionPointRec(mousePosition, invisibleBox))
            {
                selectHero("invisible_man");
            }
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            if (CheckCollisionPointRec(mousePosition, draculaBox))
            {
                selectHero("dracula");
            }

            if (CheckCollisionPointRec(mousePosition, sherlockBox))
            {
                selectHero("sherlock");
            }

            if (CheckCollisionPointRec(mousePosition, invisibleBox))
            {
                selectHero("invisible_man");
            }
        }

        Rectangle finishButton{(GetScreenWidth() - 300.0f) / 2.0f, 700.0f, 300.0f, 70.0f};

        if (bothHeroesSelected())
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(mousePosition, finishButton))
                {
                    game->startGame();
                    startPlacement();
                    if (state == State::PLACEMENT && placement == Placement::FINISHED)
                    {
                        return 4;
                    }
                }
            }
        }
    }

    return 0;
}

std::string MainMenu::getSelectedSave() const
{
    if (selectedSave < 0 ||
        selectedSave >= saveFiles.size())
    {
        return "";
    }

    return saveFiles[selectedSave];
}

void MainMenu::startHeroSelection()
{
    if (game == nullptr)
    {
        return;
    }

    int age1 = std::stoi(player1Age);
    int age2 = std::stoi(player2Age);

    game->initialize(age1, age2);

    Player *younger = game->getYoungerPlayer();

    if (younger == nullptr)
    {
        return;
    }

    const vector<Player *> &players = game->getPlayers();

    if (players.size() < 2)
    {
        return;
    }

    if (younger == players[0])
    {
        currentHeroPlayer = HeroSelectionPlayer::PLAYER_1;
    }
    else
    {
        currentHeroPlayer = HeroSelectionPlayer::PLAYER_2;
    }

    player1Hero.clear();
    player2Hero.clear();

    player1HeroSelected = false;
    player2HeroSelected = false;

    state = State::HERO_SELECTION;
}

void MainMenu::drawHeroSelection(Font font)
{
    string playerName;

    if (currentHeroPlayer == HeroSelectionPlayer::PLAYER_1)
    {
        playerName = player1Name;
    }
    else
    {
        playerName = player2Name;
    }

    if (!bothHeroesSelected())
    {
        string instruction = playerName + ", CHOOSE YOUR HERO";

        Vector2 instructionSize = MeasureTextEx(font, instruction.c_str(), 35.0f, 2.0f);

        DrawTextEx(font, instruction.c_str(),
            Vector2{(GetScreenWidth() - instructionSize.x) / 2.0f, 210.0f}, 35.0f, 2.0f, WHITE);
    }

    int age1 = std::stoi(player1Age);
    int age2 = std::stoi(player2Age);

    if (age1 == age2)
    {
        const char *line1 = "Both players are the same age!";

        const char *line2 = "Hero selection order was chosen randomly.";

        const float messageFontSize = 30.0f;
        const float messageSpacing = 1.0f;
        const float rightPadding = 35.0f;

        Vector2 line1Size = MeasureTextEx(font, line1, messageFontSize, messageSpacing);

        Vector2 line2Size = MeasureTextEx(font, line2, messageFontSize, messageSpacing);

        DrawTextEx(font, line1, Vector2{ GetScreenWidth() - line1Size.x - rightPadding, 80.0f},
                    messageFontSize, messageSpacing, WHITE);

        DrawTextEx(font, line2,
            Vector2{GetScreenWidth() - line2Size.x - rightPadding, 108.0f}, messageFontSize,
            messageSpacing, WHITE);
    }

    Texture2D dracula = assets->getCharacter("dracula");

    Texture2D sherlock = assets->getCharacter("sherlock");

    Texture2D invisibleMan = assets->getCharacter("invisible_man");

    const float boxWidth = 320.0f;
    const float boxHeight = 370.0f;
    const float gap = 35.0f;

    const float totalWidth = 3.0f * boxWidth + 2.0f * gap;

    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;

    const float boxY = 300.0f;

    Rectangle draculaBox{startX, boxY, boxWidth, boxHeight};

    Rectangle sherlockBox{startX + boxWidth + gap, boxY, boxWidth, boxHeight};

    Rectangle invisibleBox{startX + 2.0f * (boxWidth + gap), boxY, boxWidth, boxHeight};

    Vector2 mousePosition = GetMousePosition();

    Color normalColor{30, 30, 30, 220};

    Color hoverColor{70, 70, 70, 240};

    Color selectedColor{100, 70, 30, 255};

    Color draculaColor = normalColor;

    if (player1Hero == "dracula" || player2Hero == "dracula")
    {
        draculaColor = selectedColor;
    }
    else if (CheckCollisionPointRec(mousePosition, draculaBox))
    {
        draculaColor = hoverColor;
    }

    Color sherlockColor = normalColor;

    if (player1Hero == "sherlock" || player2Hero == "sherlock")
    {
        sherlockColor = selectedColor;
    }
    else if (CheckCollisionPointRec(mousePosition, sherlockBox))
    {
        sherlockColor = hoverColor;
    }

    Color invisibleColor = normalColor;

    if (player1Hero == "invisible_man" || player2Hero == "invisible_man")
    {
        invisibleColor = selectedColor;
    }
    else if (CheckCollisionPointRec(mousePosition, invisibleBox))
    {
        invisibleColor = hoverColor;
    }

    DrawRectangleRounded(draculaBox,0.15f, 32, draculaColor);

    DrawRectangleRounded(sherlockBox, 0.15f, 32, sherlockColor);

    DrawRectangleRounded(invisibleBox, 0.15f, 32, invisibleColor);

    drawHeroTexture(dracula, draculaBox);

    drawHeroTexture(sherlock, sherlockBox);

    drawHeroTexture(invisibleMan, invisibleBox);

    drawCenteredText(font, "DRACULA", draculaBox.x, draculaBox.y + draculaBox.height - 45.0f,
                     draculaBox.width, 25.0f);

    drawCenteredText(font, "SHERLOCK HOLMES", sherlockBox.x, sherlockBox.y + sherlockBox.height - 45.0f,
                     sherlockBox.width, 25.0f);

    drawCenteredText(font, "INVISIBLE MAN", invisibleBox.x, invisibleBox.y + invisibleBox.height - 45.0f,
                     invisibleBox.width, 25.0f);

    Rectangle finishButton{(GetScreenWidth() - 300.0f) / 2.0f, 700.0f, 300.0f, 70.0f};

    bool bothSelected = player1HeroSelected && player2HeroSelected;

    Color finishColor = bothSelected ? Color{30, 30, 30, 230} : Color{30, 30, 30, 100};

    if (bothSelected && CheckCollisionPointRec(mousePosition, finishButton))
    {
        finishColor = Color{60, 60, 60, 235};
    }

    DrawRectangleRounded(finishButton, 1.0f, 32, finishColor);

    drawCenteredText(font, "FINISH", finishButton.x, finishButton.y + 17.0f, finishButton.width, 35.0f);
}

void MainMenu::drawHeroTexture(Texture2D texture, Rectangle box)
{
    if (texture.id == 0)
    {
        return;
    }

    const float padding = 15.0f;

    Rectangle source{0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};

    Rectangle destination{box.x + padding, box.y + padding, box.width - 2.0f * padding, box.height - 70.0f};

    DrawTexturePro(texture, source, destination, Vector2{0, 0}, 0.0f, WHITE);
}

void MainMenu::drawCenteredText(Font font, const char *text, float x, float y, float width, float fontSize)
{
    Vector2 textSize = MeasureTextEx(font, text, fontSize, 2.0f);

    DrawTextEx(font, text, Vector2{x + (width - textSize.x) / 2.0f, y}, fontSize, 2.0f, WHITE);
}

void MainMenu::selectHero(const std::string &hero)
{
    int playerIndex;

    if (currentHeroPlayer == HeroSelectionPlayer::PLAYER_1)
    {
        playerIndex = 1;
    }
    else
    {
        playerIndex = 2;
    }

    if (game == nullptr)
    {
        return;
    }

    bool assigned = game->assignHero(playerIndex, hero);

    if (!assigned)
    {
        return;
    }

    if (playerIndex == 1)
    {
        player1Hero = hero;
        player1HeroSelected = true;
    }
    else
    {
        player2Hero = hero;
        player2HeroSelected = true;
    }

    if (!bothHeroesSelected())
    {
        if (currentHeroPlayer == HeroSelectionPlayer::PLAYER_1)
        {
            currentHeroPlayer = HeroSelectionPlayer::PLAYER_2;
        }
        else
        {
            currentHeroPlayer = HeroSelectionPlayer::PLAYER_1;
        }
    }
}

bool MainMenu::bothHeroesSelected() const
{
    return player1HeroSelected && player2HeroSelected;
}

void MainMenu::startPlacement()
{
    selectedStartSpace = -1;
    placementStartSpace = -1;

    placementHeroPlaced = false;
    placementSidekickIndex = 0;
    currentFogIndex = 0;

    if (game == nullptr)
    {
        return;
    }

    Player *younger = game->getYoungerPlayer();

    if (younger == nullptr)
    {
        return;
    }

    const vector<Player *> &players = game->getPlayers();

    if (players.size() < 2)
    {
        return;
    }

    if (younger == players[0])
    {
        placementPlayer = 1;
    }
    else if (younger == players[1])
    {
        placementPlayer = 2;
    }
    else
    {
        return;
    }

    placement = Placement::YOUNGER_HERO;

    state = State::PLACEMENT;
}

bool MainMenu::placeHeroOnSpace(int spaceId)
{
    if (game == nullptr)
    {
        return false;
    }

    Board &board = game->getBoard();

    Space *space = board.getSpace(spaceId);

    if (space == nullptr)
    {
        return false;
    }

    if (space->isOccupied())
    {
        return false;
    }

    const std::vector<Player *> &players = game->getPlayers();

    if (placementPlayer < 1 || placementPlayer > players.size())
    {
        return false;
    }

    Player *player = players[placementPlayer - 1];

    if (player == nullptr)
    {
        return false;
    }

    Hero *hero = player->getHero();

    if (hero == nullptr)
    {
        return false;
    }

    board.moveFighter(hero, space);

    placementStartSpace = spaceId;
    selectedStartSpace = spaceId;
    placementHeroPlaced = true;
    placementSidekickIndex = 0;

    if (!player->getSideKicks().empty())
    {
        if (placement == Placement::YOUNGER_HERO)
        {
            placement = Placement::YOUNGER_SIDEKICKS;
        }
        else if (placement == Placement::OLDER_HERO)
        {
            placement = Placement::OLDER_SIDEKICKS;
        }

        return true;
    }

    if (placement == Placement::YOUNGER_HERO)
    {
        if (player != nullptr && !player->getFogs().empty())
        {
            placement = Placement::FOG;
            fogForYounger = true;

            currentFogIndex = 0;

            placementHeroPlaced = false;
            placementStartSpace = -1;
            selectedStartSpace = -1;
            placementSidekickIndex = 0;

            return true;
        }

        if (placementPlayer == 1)
        {
            placementPlayer = 2;
        }
        else
        {
            placementPlayer = 1;
        }

        placement = Placement::OLDER_HERO;

        placementHeroPlaced = false;
        placementStartSpace = -1;
        selectedStartSpace = -1;
        placementSidekickIndex = 0;

        return true;
    }

    if (placement == Placement::OLDER_HERO)
    {
        if (player != nullptr && !player->getFogs().empty())
        {
            placement = Placement::FOG;
            fogForYounger = false;

            currentFogIndex = 0;

            placementHeroPlaced = false;
            placementStartSpace = -1;
            selectedStartSpace = -1;
            placementSidekickIndex = 0;

            return true;
        }

        placement = Placement::FINISHED;

        placementHeroPlaced = false;
        placementStartSpace = -1;
        selectedStartSpace = -1;
        placementSidekickIndex = 0;
        currentFogIndex = 0;

        return true;
    }
    return false;
}

bool MainMenu::isValidSidekickPlacement(Space *space) const
{
    if (game == nullptr || space == nullptr)
    {
        return false;
    }

    if (space->isOccupied())
    {
        return false;
    }

    if (placementStartSpace == -1)
    {
        return false;
    }

    const Board &gameBoard = game->getBoard();

    if (!gameBoard.sameZone(placementStartSpace, space->getId()))
    {
        return false;
    }

    return true;
}

bool MainMenu::placeSidekickOnSpace(int spaceId)
{
    if (game == nullptr)
    {
        return false;
    }

    Board &gameBoard = game->getBoard();

    Space *space = gameBoard.getSpace(spaceId);

    if (space == nullptr)
    {
        return false;
    }

    if (!isValidSidekickPlacement(space))
    {
        return false;
    }

    const vector<Player *> &players = game->getPlayers();

    if (players.size() < 2)
    {
        return false;
    }

    Player *player = nullptr;

    if (placementPlayer == 1)
    {
        player = players[0];
    }
    else
    {
        player = players[1];
    }

    if (player == nullptr)
    {
        return false;
    }

    std::vector<Sidekick *> sidekicks = player->getSideKicks();

    if (placementSidekickIndex < 0 || placementSidekickIndex >= static_cast<int>(sidekicks.size()))
    {
        return false;
    }

    Sidekick *sidekick = sidekicks[placementSidekickIndex];

    if (sidekick == nullptr)
    {
        return false;
    }

    gameBoard.moveFighter(sidekick, space);

    placementSidekickIndex++;

    if (placementSidekickIndex >= static_cast<int>(sidekicks.size()))
    {
        finishPlacement();
    }

    return true;
}

void MainMenu::finishPlacement()
{
    if (placement == Placement::YOUNGER_SIDEKICKS)
    {
        const vector<Player *> &players = game->getPlayers();

        Player *currentPlayer =
            (placementPlayer >= 1 && placementPlayer <= static_cast<int>(players.size()))
                ? players[placementPlayer - 1]
                : nullptr;

        if (currentPlayer != nullptr && !currentPlayer->getFogs().empty())
        {
            placement = Placement::FOG;
            fogForYounger = true;

            currentFogIndex = 0;

            placementHeroPlaced = false;
            placementStartSpace = -1;
            selectedStartSpace = -1;
            placementSidekickIndex = 0;

            return;
        }

        if (placementPlayer == 1)
        {
            placementPlayer = 2;
        }
        else
        {
            placementPlayer = 1;
        }

        placement = Placement::OLDER_HERO;

        placementHeroPlaced = false;
        placementStartSpace = -1;
        selectedStartSpace = -1;
        placementSidekickIndex = 0;

        return;
    }

    if (placement == Placement::OLDER_SIDEKICKS)
    {
        const vector<Player *> &players = game->getPlayers();

        Player *currentPlayer =
            (placementPlayer >= 1 && placementPlayer <= static_cast<int>(players.size()))
                ? players[placementPlayer - 1]
                : nullptr;

        if (currentPlayer != nullptr && !currentPlayer->getFogs().empty())
        {
            placement = Placement::FOG;
            fogForYounger = false;

            currentFogIndex = 0;

            placementHeroPlaced = false;
            placementStartSpace = -1;
            selectedStartSpace = -1;
            placementSidekickIndex = 0;

            return;
        }

        placement = Placement::FINISHED;

        placementHeroPlaced = false;
        placementStartSpace = -1;
        selectedStartSpace = -1;
        placementSidekickIndex = 0;
        currentFogIndex = 0;

        return;
    }

    if (placement == Placement::FOG)
    {
        if (fogForYounger)
        {
            fogForYounger = false;

            if (placementPlayer == 1)
            {
                placementPlayer = 2;
            }
            else
            {
                placementPlayer = 1;
            }

            placement = Placement::OLDER_HERO;
        }
        else
        {
            placement = Placement::FINISHED;
        }

        placementHeroPlaced = false;
        placementStartSpace = -1;
        selectedStartSpace = -1;
        placementSidekickIndex = 0;
        currentFogIndex = 0;

        return;
    }
}

void MainMenu::drawPlacement(Font font)
{
    Texture2D map = assets->getGameMap();

    if (map.id == 0)
        return;

    float scale = getMapScale();
    Vector2 mapPosition = getMapPosition();

    Rectangle source{0.0f, 0.0f, static_cast<float>(map.width), static_cast<float>(map.height)};

    Rectangle destination{mapPosition.x, mapPosition.y, map.width * scale, map.height * scale};

    DrawTexturePro(map, source, destination, Vector2{0, 0}, 0.0f, WHITE);

    Vector2 mouse = GetMousePosition();

    Color hoverColor = Color{255, 255, 255, 90};   
    Color selectedColor = Color{140, 200, 255, 80}; 
    Color heroColor = Color{200, 160, 255, 80};     
    Color sidekickColor = Color{0, 0, 255, 150};    

    int hoveredSpace = -1;

    for (int i = 0; i < 32; i++)
    {
        Vector2 center = mapImageToScreen(SPACE_GRAPHICS[i].center);
        float radius = SPACE_GRAPHICS[i].radius * scale;

        if (CheckCollisionPointCircle(mouse, center, radius))
        {
            hoveredSpace = i + 1;
        }

        if (placement == Placement::YOUNGER_SIDEKICKS || placement == Placement::OLDER_SIDEKICKS)
        {
            Space *space = game->getBoard().getSpace(i + 1);
            if (space != nullptr && isValidSidekickPlacement(space))
            {
                DrawCircleV(center, radius, Color{255, 220, 120, 120});
            }
        }

        if (placement == Placement::YOUNGER_HERO || placement == Placement::OLDER_HERO)
        {
            if (i + 1 == 7 || i + 1 == 22)
            {
                DrawCircleV(center, radius, Color{200, 160, 250, 180});
            }
        }

        if (placement == Placement::FOG)
        {
            Space *space = game->getBoard().getSpace(i + 1);

            if (space != nullptr && !space->isOccupied())
            {
                DrawCircleV(center, radius, Color{140, 200, 255, 120});
            }
        }

        const vector<Player *> &players = game->getPlayers();

        for (Player *player : players)
        {
            if (player == nullptr)
            {
                continue;
            }

            const vector<Fog *> &fogs = player->getFogs();

            for (Fog *fog : fogs)
            {
                if (fog == nullptr || fog->getPosition() == nullptr)
                {
                    continue;
                }

                int fogSpaceId = fog->getPosition()->getId();

                if (fogSpaceId < 1 || fogSpaceId > 32)
                {
                    continue;
                }

                Vector2 fogCenter = mapImageToScreen(SPACE_GRAPHICS[fogSpaceId - 1].center);

                float fogRadius = SPACE_GRAPHICS[fogSpaceId - 1].radius * getMapScale();

                DrawCircleV(fogCenter, fogRadius, Color{180, 220, 255, 120});
            }
        }

        if (i + 1 == placementStartSpace)
        {
            DrawCircleV(center, radius, heroColor);
        }

        if (i + 1 == hoveredSpace)
        {
            DrawCircleV(center, radius, hoverColor);
            DrawCircleLinesV(center, radius, Color{255, 255, 255, 200});
        }
    }

    const char *title = "";

    if (placement == Placement::YOUNGER_HERO)
    {
        title = "YOUNGER PLAYER - PLACE YOUR HERO";
    }

    else if (placement == Placement::YOUNGER_SIDEKICKS)
    {
        title = "YOUNGER PLAYER - PLACE YOUR SIDEKICKS";
    }
    else if (placement == Placement::OLDER_HERO)
    {
        title = "OLDER PLAYER - PLACE YOUR HERO";
    }
    else if (placement == Placement::OLDER_SIDEKICKS)
    {
        title = "OLDER PLAYER - PLACE YOUR SIDEKICKS";
    }
    else if (placement == Placement::FOG)
    {
        title = "INVISIBLE MAN - PLACE YOUR FOG";
    }
    else
    {
        return;
    }

    float fontSize = 30.0f;

    Vector2 titleSize = MeasureTextEx(font, title, fontSize, 2.0f);

    DrawTextEx(font, title,
        Vector2{(GetScreenWidth() - titleSize.x) / 2.0f, 20.0f}, fontSize, 2.0f, WHITE);

    const char *instruction = "";

    if (placement == Placement::YOUNGER_HERO || placement == Placement::OLDER_HERO)
    {
        instruction = "Choose Space 7 or Space 22 for your Hero";
    }
    else if (placement == Placement::FOG)
    {
        instruction = "Choose a valid Space for your Fog";
    }
    else
    {
        instruction = "Choose a valid Space for your Sidekick";
    }

    float instructionSize = 22.0f;

    Vector2 instructionTextSize = MeasureTextEx(font, instruction, instructionSize, 1.5f);

    DrawTextEx(font, instruction,
        Vector2{(GetScreenWidth() - instructionTextSize.x) / 2.0f, 55.0f}, instructionSize, 1.5f, WHITE);
}

float MainMenu::getMapScale() const
{
    Texture2D map = assets->getGameMap();

    if (map.id == 0)
    {
        return 1.0f;
    }

    const float panelWidth = 263.0f;
    const float mapPadding = 15.0f;
    const float topSpace = 75.0f;

    const float mapAreaWidth = GetScreenWidth() - 2.0f * (panelWidth + mapPadding);

    const float mapAreaHeight = GetScreenHeight() - topSpace;

    float scaleX = mapAreaWidth / static_cast<float>(map.width);

    float scaleY = mapAreaHeight / static_cast<float>(map.height);

    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    scale *= 0.99f;

    return scale;
}

Vector2 MainMenu::getMapPosition() const
{
    Texture2D map = assets->getGameMap();

    if (map.id == 0)
    {
        return Vector2{0.0f, 0.0f};
    }

    const float panelWidth = 263.0f;
    const float mapPadding = 15.0f;
    const float topSpace = 75.0f;

    const float mapAreaX = panelWidth + mapPadding;

    const float mapAreaWidth = GetScreenWidth() - 2.0f * (panelWidth + mapPadding);

    const float mapAreaY = topSpace;

    const float mapAreaHeight = GetScreenHeight() - topSpace;

    float scaleX = mapAreaWidth / static_cast<float>(map.width);

    float scaleY = mapAreaHeight / static_cast<float>(map.height);

    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    scale *= 0.99f;

    float mapWidth = map.width * scale;

    float mapHeight = map.height * scale;

    float mapX = mapAreaX + (mapAreaWidth - mapWidth) / 2.0f;

    float mapY = mapAreaY + (mapAreaHeight - mapHeight) / 2.0f;

    mapY -= 80.0f;

    return Vector2{mapX, mapY};
}

Vector2 MainMenu::mapImageToScreen(Vector2 imagePosition) const
{
    float scale = getMapScale();

    Vector2 mapPosition = getMapPosition();

    return Vector2{ mapPosition.x + imagePosition.x * scale,

        mapPosition.y + imagePosition.y * scale};
}

void MainMenu::updatePlacement()
{
    if (placement == Placement::FINISHED)
    {
        return;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return;
    }

    Vector2 mouse = GetMousePosition();

    if (placement == Placement::YOUNGER_HERO || placement == Placement::OLDER_HERO)
    {
        Vector2 space7 = mapImageToScreen(SPACE_GRAPHICS[6].center);

        float radius7 = SPACE_GRAPHICS[6].radius * getMapScale();

        if (CheckCollisionPointCircle(mouse, space7, radius7))
        {
            if (placeHeroOnSpace(7))
            {
                cout << "Hero placed on Space 7" << std::endl;
            }

            return;
        }

        Vector2 space22 = mapImageToScreen(SPACE_GRAPHICS[21].center);

        float radius22 = SPACE_GRAPHICS[21].radius * getMapScale();

        if (CheckCollisionPointCircle(mouse, space22, radius22))
        {
            if (placeHeroOnSpace(22))
            {
                cout << "Hero placed on Space 22" << std::endl;
            }

            return;
        }

        return;
    }

    if (placement == Placement::YOUNGER_SIDEKICKS || placement == Placement::OLDER_SIDEKICKS)
    {
        for (int i = 0; i < 32; i++)
        {
            Vector2 center = mapImageToScreen(SPACE_GRAPHICS[i].center);

            float radius = SPACE_GRAPHICS[i].radius * getMapScale();

            if (CheckCollisionPointCircle(mouse, center, radius))
            {
                if (placeSidekickOnSpace(i + 1))
                {
                    cout << "Sidekick placed on Space " << i + 1 << endl;
                }

                return;
            }
        }
    }

    if (placement == Placement::FOG)
    {
        for (int i = 0; i < 32; i++)
        {
            Vector2 center = mapImageToScreen(SPACE_GRAPHICS[i].center);

            float radius = SPACE_GRAPHICS[i].radius * getMapScale();

            if (CheckCollisionPointCircle(mouse, center, radius))
            {
                if (placeFogOnSpace(i + 1))
                {
                    cout << "Fog placed on Space " << i + 1 << endl;
                }

                return;
            }
        }
    }
}

bool MainMenu::placeFogOnSpace(int spaceId)
{
    if (game == nullptr)
    {
        return false;
    }

    if (placement != Placement::FOG)
    {
        return false;
    }

    Board &board = game->getBoard();

    Space *space = board.getSpace(spaceId);

    if (space == nullptr)
    {
        return false;
    }

    Fog *fog = getCurrentFog();

    if (fog == nullptr)
    {
        return false;
    }

    const vector<Player *> &players = game->getPlayers();

    if (placementPlayer < 1 || placementPlayer > static_cast<int>(players.size()))
    {
        return false;
    }

    Player *player = players[placementPlayer - 1];

    if (player == nullptr)
    {
        return false;
    }

    const vector<Fog *> &fogs = player->getFogs();

    for (Fog *existingFog : fogs)
    {
        if (existingFog != nullptr && existingFog->getPosition() == space)
        {
            return false;
        }
    }

    fog->setPosition(space);
    space->setFogToken(true);
    currentFogIndex++;

    if (currentFogIndex >= static_cast<int>(fogs.size()))
    {
        currentFogIndex = 0;

        placementHeroPlaced = false;
        placementStartSpace = -1;
        selectedStartSpace = -1;
        placementSidekickIndex = 0;

        if (fogForYounger)
        {
            fogForYounger = false;

            if (placementPlayer == 1)
            {
                placementPlayer = 2;
            }
            else
            {
                placementPlayer = 1;
            }

            placement = Placement::OLDER_HERO;
        }
        else
        {
            placement = Placement::FINISHED;
        }
    }

    return true;
}

Fog *MainMenu::getCurrentFog()
{
    if (game == nullptr)
    {
        return nullptr;
    }

    const vector<Player *> &players = game->getPlayers();

    if (placementPlayer < 1 || placementPlayer > static_cast<int>(players.size()))
    {
        return nullptr;
    }

    Player *player = players[placementPlayer - 1];

    if (player == nullptr)
    {
        return nullptr;
    }

    const vector<Fog *> &fogs = player->getFogs();

    if (currentFogIndex < 0 || currentFogIndex >= static_cast<int>(fogs.size()))
    {
        return nullptr;
    }

    return fogs[currentFogIndex];
}