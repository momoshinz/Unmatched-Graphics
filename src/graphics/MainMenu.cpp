#include "graphics/MainMenu.h"
#include <cctype>
#include <fstream>
using namespace std;

MainMenu::MainMenu(AssetManager *assets)
    : assets(assets),
      state(State::MAIN_MENU),
      player1Name(""),
      player1Age(""),
      player2Name(""),
      player2Age(""),
      enteringName(true),
      enteringAge(false),
      selectedSave(-1)
{
}

bool MainMenu::isPlayer1Complete() const
{
    return !player1Name.empty() &&
           !player1Age.empty();
}

bool MainMenu::isPlayer2Complete() const
{
    return !player2Name.empty() &&
           !player2Age.empty();
}

void loadSaveFiles(std::vector<std::string> &saveFiles)
{
    saveFiles.clear();

    int slot = 1;

    while (true)
    {
        std::string filename = "save" + std::to_string(slot) + ".json";

        std::ifstream file(filename);

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
        DrawTexturePro(
            background,
            Rectangle{
                0,
                0,
                static_cast<float>(background.width),
                static_cast<float>(background.height)},
            Rectangle{
                0,
                0,
                static_cast<float>(GetScreenWidth()),
                static_cast<float>(GetScreenHeight())},
            Vector2{0, 0},
            0.0f,
            WHITE);
    }

    const char *title = "UNMATCHED";

    const float titleSize = 60.0f;
    const float titleSpacing = 2.0f;

    Vector2 titleTextSize = MeasureTextEx(titleFont, title, titleSize, titleSpacing);

    const float titleX = (GetScreenWidth() - titleTextSize.x) / 2.0f;

    const float titleY = 100.0f;

    DrawTextEx(titleFont, title, Vector2{titleX, titleY}, titleSize, titleSpacing, WHITE);

    if (state == State::MAIN_MENU)
    {
        const float buttonWidth = 300.0f;
        const float buttonHeight = 75.0f;

        const float buttonX =
            (GetScreenWidth() - buttonWidth) / 2.0f;

        const float firstButtonY = 390.0f;
        const float buttonGap = 25.0f;

        Rectangle newGameButton{buttonX, firstButtonY, buttonWidth, buttonHeight};

        Rectangle loadGameButton{buttonX, firstButtonY + buttonHeight + buttonGap,
                                 buttonWidth,
                                 buttonHeight};

        Rectangle exitButton{buttonX, firstButtonY + 2.0f * (buttonHeight + buttonGap),
                             buttonWidth,
                             buttonHeight};

        Vector2 mousePosition = GetMousePosition();

        Color normalColor{30, 30, 30, 220};

        Color hoverColor{
            60,
            60,
            60,
            235};

        // -----------------------------
        // New Game
        // -----------------------------

        Color newGameColor =
            CheckCollisionPointRec(
                mousePosition,
                newGameButton)
                ? hoverColor
                : normalColor;

        DrawRectangleRounded(
            newGameButton,
            1.0f,
            32,
            newGameColor);

        // -----------------------------
        // Load Game
        // -----------------------------

        Color loadGameColor =
            CheckCollisionPointRec(
                mousePosition,
                loadGameButton)
                ? hoverColor
                : normalColor;

        DrawRectangleRounded(
            loadGameButton,
            1.0f,
            32,
            loadGameColor);

        // -----------------------------
        // Exit
        // -----------------------------

        Color exitColor =
            CheckCollisionPointRec(
                mousePosition,
                exitButton)
                ? hoverColor
                : normalColor;

        DrawRectangleRounded(
            exitButton,
            1.0f,
            32,
            exitColor);

        // -----------------------------
        // Button Text
        // -----------------------------

        const float buttonFontSize = 35.0f;
        const float buttonSpacing = 2.0f;

        const char *newGameText = "NEW GAME";
        const char *loadGameText = "LOAD GAME";
        const char *exitText = "EXIT";

        Vector2 newGameTextSize =
            MeasureTextEx(
                font,
                newGameText,
                buttonFontSize,
                buttonSpacing);

        Vector2 loadGameTextSize =
            MeasureTextEx(
                font,
                loadGameText,
                buttonFontSize,
                buttonSpacing);

        Vector2 exitTextSize =
            MeasureTextEx(
                font,
                exitText,
                buttonFontSize,
                buttonSpacing);

        DrawTextEx(
            font,
            newGameText,

            Vector2{
                newGameButton.x +
                    (newGameButton.width -
                     newGameTextSize.x) /
                        2.0f,

                newGameButton.y +
                    (newGameButton.height -
                     newGameTextSize.y) /
                        2.0f},

            buttonFontSize,
            buttonSpacing,
            WHITE);

        DrawTextEx(
            font,
            loadGameText,

            Vector2{
                loadGameButton.x +
                    (loadGameButton.width -
                     loadGameTextSize.x) /
                        2.0f,

                loadGameButton.y +
                    (loadGameButton.height -
                     loadGameTextSize.y) /
                        2.0f},

            buttonFontSize,
            buttonSpacing,
            WHITE);

        DrawTextEx(
            font,
            exitText,

            Vector2{
                exitButton.x +
                    (exitButton.width -
                     exitTextSize.x) /
                        2.0f,

                exitButton.y +
                    (exitButton.height -
                     exitTextSize.y) /
                        2.0f},

            buttonFontSize,
            buttonSpacing,
            WHITE);
    }

    // =====================================
    // PLAYER 1 INPUT
    // =====================================

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

    // =====================================
    // PLAYER 2 INPUT
    // =====================================

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

    // =====================================
    // LOAD GAME
    // =====================================

    if (state == State::LOAD_GAME)
    {
        const char *loadTitle = "LOAD GAME";

        Vector2 loadTitleSize =
            MeasureTextEx(
                titleFont,
                loadTitle,
                50.0f,
                2.0f);

        DrawTextEx(
            titleFont,
            loadTitle,

            Vector2{
                (GetScreenWidth() -
                loadTitleSize.x) /
                    2.0f,

                80.0f},

            50.0f,
            2.0f,
            WHITE);

        // -----------------------------
        // Save files
        // -----------------------------

        float startY = 200.0f;

        const float buttonWidth = 400.0f;
        const float buttonHeight = 60.0f;
        const float buttonGap = 20.0f;

        const float buttonX =
            (GetScreenWidth() - buttonWidth) / 2.0f;

        Vector2 mousePosition =
            GetMousePosition();

        for (int i = 0; i < saveFiles.size(); i++)
        {
            Rectangle saveButton{
                buttonX,
                startY + i * (buttonHeight + buttonGap),
                buttonWidth,
                buttonHeight};

            Color normalColor{
                30,
                30,
                30,
                220};

            Color hoverColor{
                60,
                60,
                60,
                235};

            Color buttonColor =
                CheckCollisionPointRec(
                    mousePosition,
                    saveButton)
                    ? hoverColor
                    : normalColor;

            DrawRectangleRounded(
                saveButton,
                1.0f,
                32,
                buttonColor);

            std::string saveText =
                "SAVE " +
                std::to_string(i + 1);

            Vector2 saveTextSize =
                MeasureTextEx(
                    font,
                    saveText.c_str(),
                    25.0f,
                    2.0f);

            DrawTextEx(
                font,
                saveText.c_str(),

                Vector2{
                    saveButton.x +
                        (saveButton.width -
                        saveTextSize.x) /
                            2.0f,

                    saveButton.y +
                        (saveButton.height -
                        saveTextSize.y) /
                            2.0f},

                25.0f,
                2.0f,
                WHITE);
        }

        // -----------------------------
        // No saves
        // -----------------------------

        if (saveFiles.empty())
        {
            const char *noSaveText =
                "No saved games found.";

            Vector2 textSize =
                MeasureTextEx(
                    font,
                    noSaveText,
                    25.0f,
                    2.0f);

            DrawTextEx(
                font,
                noSaveText,

                Vector2{
                    (GetScreenWidth() -
                    textSize.x) /
                        2.0f,

                    300.0f},

                25.0f,
                2.0f,
                WHITE);
        }

        // -----------------------------
        // Back
        // -----------------------------

        Rectangle backButton{
            40.0f,
            GetScreenHeight() - 80.0f,
            160.0f,
            50.0f};

        Color backColor =
            CheckCollisionPointRec(
                mousePosition,
                backButton)
                ? Color{60, 60, 60, 235}
                : Color{30, 30, 30, 220};

        DrawRectangleRounded(
            backButton,
            1.0f,
            32,
            backColor);

        const char *backText = "BACK";

        Vector2 backTextSize =
            MeasureTextEx(
                font,
                backText,
                22.0f,
                2.0f);

        DrawTextEx(
            font,
            backText,

            Vector2{
                backButton.x +
                    (backButton.width -
                    backTextSize.x) /
                        2.0f,

                backButton.y +
                    (backButton.height -
                    backTextSize.y) /
                        2.0f},

            22.0f,
            2.0f,
            WHITE);
    }
    // =====================================
    // READY
    // =====================================

    if (state == State::READY)
    {
        // -----------------------------
        // Player 1 info
        // -----------------------------

        const float infoX =
            GetScreenWidth() - 420.0f;

        const float player1Y = 250.0f;
        const float player2Y = 390.0f;

        DrawTextEx(
            font,
            "PLAYER 1",

            Vector2{
                infoX,
                player1Y},

            30.0f,
            2.0f,
            WHITE);

        std::string player1Info =
            "Name: " +
            player1Name +
            "\nAge: " +
            player1Age;

        DrawTextEx(
            font,
            player1Info.c_str(),

            Vector2{
                infoX,
                player1Y + 45.0f},

            25.0f,
            2.0f,
            WHITE);

        // -----------------------------
        // Player 2 info
        // -----------------------------

        DrawTextEx(
            font,
            "PLAYER 2",

            Vector2{
                infoX,
                player2Y},

            30.0f,
            2.0f,
            WHITE);

        std::string player2Info =
            "Name: " +
            player2Name +
            "\nAge: " +
            player2Age;

        DrawTextEx(
            font,
            player2Info.c_str(),

            Vector2{
                infoX,
                player2Y + 45.0f},

            25.0f,
            2.0f,
            WHITE);

        // -----------------------------
        // START BUTTON
        // -----------------------------

        Rectangle startButton{
            500.0f,
            650.0f,
            300.0f,
            75.0f};

        DrawRectangleRounded(
            startButton,
            1.0f,
            32,
            Color{30, 30, 30, 220});

        const char *startText = "START";

        Vector2 startTextSize =
            MeasureTextEx(
                font,
                startText,
                35.0f,
                2.0f);

        DrawTextEx(
            font,
            startText,

            Vector2{
                startButton.x +
                    (startButton.width -
                     startTextSize.x) /
                        2.0f,

                startButton.y +
                    (startButton.height -
                     startTextSize.y) /
                        2.0f},

            35.0f,
            2.0f,
            WHITE);
    }
}

// =========================================
// PLAYER INPUT DRAW
// =========================================

void MainMenu::drawPlayerInput(
    Font font,
    const char *playerTitle,
    const std::string &name,
    const std::string &age,
    bool enteringName,
    bool enteringAge,
    const char *buttonText)
{
    const float boxWidth = 450.0f;
    const float boxHeight = 60.0f;

    const float boxX =
        (GetScreenWidth() - boxWidth) / 2.0f;

    const float nameY = 330.0f;
    const float ageY = 470.0f;

    const float buttonWidth = 300.0f;
    const float buttonHeight = 70.0f;

    const float buttonX =
        (GetScreenWidth() - buttonWidth) / 2.0f;

    const float buttonY = 600.0f;

    // =====================================
    // Player title
    // =====================================
    const float firstButtonY = 370.0f;
    const float buttonGap = 20.0f;
    Rectangle newGameButton{
        buttonX,
        firstButtonY,
        buttonWidth,
        buttonHeight};

    Vector2 playerTitleSize =
        MeasureTextEx(
            font,
            playerTitle,
            35.0f,
            2.0f);

    DrawTextEx(
        font,
        playerTitle,

        Vector2{
            (GetScreenWidth() -
             playerTitleSize.x) /
                2.0f,

            220.0f},

        35.0f,
        2.0f,
        WHITE);

    // =====================================
    // NAME LABEL
    // =====================================

    DrawTextEx(
        font,
        "PLAYER NAME:",

        Vector2{
            boxX,
            nameY - 40.0f},

        25.0f,
        2.0f,
        WHITE);

    // =====================================
    // NAME BOX
    // =====================================

    Rectangle nameBox{
        boxX,
        nameY,
        boxWidth,
        boxHeight};

    Color nameColor =
        enteringName
            ? Color{60, 60, 60, 240}
            : Color{30, 30, 30, 220};

    DrawRectangleRounded(
        nameBox,
        0.35f,
        32,
        nameColor);

    DrawTextEx(
        font,
        name.c_str(),

        Vector2{
            nameBox.x + 20.0f,
            nameBox.y + 15.0f},

        25.0f,
        2.0f,
        WHITE);

    // =====================================
    // AGE LABEL
    // =====================================

    DrawTextEx(
        font,
        "PLAYER AGE:",

        Vector2{
            boxX,
            ageY - 40.0f},

        25.0f,
        2.0f,
        WHITE);

    // =====================================
    // AGE BOX
    // =====================================

    Rectangle ageBox{
        boxX,
        ageY,
        boxWidth,
        boxHeight};

    Color ageColor =
        enteringAge
            ? Color{60, 60, 60, 240}
            : Color{30, 30, 30, 220};

    DrawRectangleRounded(
        ageBox,
        0.35f,
        32,
        ageColor);

    DrawTextEx(
        font,
        age.c_str(),

        Vector2{
            ageBox.x + 20.0f,
            ageBox.y + 15.0f},

        25.0f,
        2.0f,
        WHITE);

    bool complete =
        !name.empty() &&
        !age.empty();

    Vector2 mousePosition =
        GetMousePosition();

    Rectangle actionButton{
        buttonX,
        buttonY,
        buttonWidth,
        buttonHeight};

    Color normalButtonColor{
        30,
        30,
        30,
        230};

    Color hoverButtonColor{
        60,
        60,
        60,
        235};

    Color disabledButtonColor{
        30,
        30,
        30,
        100};

    Color buttonColor;

    if (!complete)
    {
        buttonColor = disabledButtonColor;
    }
    else if (CheckCollisionPointRec(
                 mousePosition,
                 actionButton))
    {
        buttonColor = hoverButtonColor;
    }
    else
    {
        buttonColor = normalButtonColor;
    }

    DrawRectangleRounded(
        actionButton,
        1.0f,
        32,
        buttonColor);

    Vector2 buttonTextSize =
        MeasureTextEx(
            font,
            buttonText,
            35.0f,
            2.0f);

    DrawTextEx(
        font,
        buttonText,

        Vector2{
            buttonX +
                (buttonWidth -
                 buttonTextSize.x) /
                    2.0f,

            buttonY +
                (buttonHeight -
                 buttonTextSize.y) /
                    2.0f},

        35.0f,
        2.0f,

        complete
            ? WHITE
            : Color{150, 150, 150, 150});


    // =====================================
    // ENTER MESSAGE
    // =====================================

    const char *enterMessage = "Please Enter to continue";

    Vector2 enterMessageSize =
        MeasureTextEx(
            font,
            enterMessage,
            18.0f,
            1.0f);

    DrawTextEx(
        font,
        enterMessage,

        Vector2{
            GetScreenWidth() - enterMessageSize.x - 20.0f,
            GetScreenHeight() - enterMessageSize.y - 15.0f},

        18.0f,
        1.0f,

        WHITE);
}

// =========================================
// HANDLE INPUT
// =========================================

int MainMenu::handleInput()
{
    if (assets == nullptr)
    {
        return 0;
    }

    const float buttonWidth = 300.0f;
    const float buttonHeight = 60.0f;

    const float buttonX =
        (GetScreenWidth() - buttonWidth) / 2.0f;

    const float firstButtonY = 500.0f;
    const float buttonGap = 20.0f;

    // =========================================
    // Button areas
    // =========================================

    Rectangle newGameButton{
        buttonX,
        firstButtonY,
        buttonWidth,
        buttonHeight};

    Rectangle loadGameButton{
        buttonX,
        firstButtonY + buttonHeight + buttonGap,
        buttonWidth,
        buttonHeight};

    Rectangle exitButton{
        buttonX,
        firstButtonY + 2.0f * (buttonHeight + buttonGap),
        buttonWidth,
        buttonHeight};

    // =========================================
    // Mouse
    // =========================================

    Vector2 mousePosition =
        GetMousePosition();

    // =====================================
    // MAIN MENU
    // =====================================

    if (state == State::MAIN_MENU)
    {
        const float buttonWidth = 300.0f;
        const float buttonHeight = 75.0f;

        const float buttonX =
            (GetScreenWidth() - buttonWidth) / 2.0f;

        const float firstButtonY = 390.0f;
        const float buttonGap = 25.0f;

        Rectangle newGameButton{
            buttonX,
            firstButtonY,
            buttonWidth,
            buttonHeight};

        Rectangle loadGameButton{
            buttonX,
            firstButtonY +
                buttonHeight +
                buttonGap,
            buttonWidth,
            buttonHeight};

        Rectangle exitButton{
            buttonX,
            firstButtonY +
                2.0f *
                    (buttonHeight + buttonGap),
            buttonWidth,
            buttonHeight};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // -----------------------------
            // NEW GAME
            // -----------------------------

            if (CheckCollisionPointRec(
                    mousePosition,
                    newGameButton))
            {
                // پاک کردن اطلاعات قبلی
                // در صورت شروع دوباره
                player1Name.clear();
                player1Age.clear();

                player2Name.clear();
                player2Age.clear();

                enteringName = true;
                enteringAge = false;

                // رفتن به صفحه Player 1
                state = State::PLAYER_1_INPUT;

                return 1;
            }

            // -----------------------------
            // LOAD GAME
            // فعلاً کاری نمی‌کنیم
            // -----------------------------

            if (CheckCollisionPointRec(mousePosition, loadGameButton))
            {
                loadSaveFiles(saveFiles);

                selectedSave = -1;

                state = State::LOAD_GAME;

                return 2;
            }

            // -----------------------------
            // EXIT
            // -----------------------------

            if (CheckCollisionPointRec(
                    mousePosition,
                    exitButton))
            {
                return 3;
            }
        }
    }

    // =====================================
    // PLAYER 1
    // =====================================

    if (state == State::PLAYER_1_INPUT)
    {
        const float buttonWidth = 300.0f;
        const float buttonHeight = 70.0f;

        const float buttonX =
            (GetScreenWidth() - buttonWidth) / 2.0f;

        const float buttonY = 600.0f;

        Rectangle nextButton{
            buttonX,
            buttonY,
            buttonWidth,
            buttonHeight};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(
                    mousePosition,
                    nextButton))
            {
                // فقط اگر هر دو اطلاعات وارد شده باشند
                if (isPlayer1Complete())
                {
                    enteringName = true;
                    enteringAge = false;

                    state = State::PLAYER_2_INPUT;
                }
            }
        }
    }

    // =====================================
    // PLAYER 2
    // =====================================

    if (state == State::PLAYER_2_INPUT)
    {
        const float buttonWidth = 300.0f;
        const float buttonHeight = 70.0f;

        const float buttonX =
            (GetScreenWidth() - buttonWidth) / 2.0f;

        const float buttonY = 600.0f;

        Rectangle finishButton{
            buttonX,
            buttonY,
            buttonWidth,
            buttonHeight};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(
                    mousePosition,
                    finishButton))
            {
                // فقط اگر اطلاعات کامل باشد
                if (isPlayer2Complete())
                {
                    state = State::READY;

                    enteringName = false;
                    enteringAge = false;
                }
            }
        }
    }
    // =====================================
    // LOAD GAME
    // =====================================

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
            // -----------------------------
            // Save buttons
            // -----------------------------

            for (int i = 0; i < saveFiles.size(); i++)
            {
                Rectangle saveButton{
                    buttonX,
                    startY + i * (buttonHeight + buttonGap),
                    buttonWidth,
                    buttonHeight};

                if (CheckCollisionPointRec(
                        mousePosition,
                        saveButton))
                {
                    selectedSave = i;

                    return 2;
                }
            }

            // -----------------------------
            // Back
            // -----------------------------

            Rectangle backButton{
                40.0f,
                GetScreenHeight() - 80.0f,
                160.0f,
                50.0f};

            if (CheckCollisionPointRec(
                    mousePosition,
                    backButton))
            {
                state = State::MAIN_MENU;
                selectedSave = -1;
            }
        }

        return 0;
    }

    // =====================================
    // READY
    // =====================================

    if (state == State::READY)
    {
        // فعلاً START هیچ کاری نمی‌کند.
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