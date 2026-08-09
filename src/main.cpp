#include "raylib.h"

int main()
{
    // =====================================
    // Window
    // =====================================

    const int screenWidth = 1000;
    const int screenHeight = 700;

    InitWindow(
        screenWidth,
        screenHeight,
        "Unmatched"
    );

    SetTargetFPS(60);


    // =====================================
    // Font
    // =====================================

    Font gameFont =
        LoadFont("assets/fonts/MyFont.ttf");


    // =====================================
    // Main Menu
    // =====================================

    int selectedButton = 0;

    const char *buttons[] =
    {
        "START GAME",
        "LOAD GAME",
        "EXIT"
    };

    const int buttonCount = 3;


    // =====================================
    // Button Settings
    // =====================================

    const float buttonWidth = 320.0f;
    const float buttonHeight = 60.0f;

    const float buttonSpacing = 25.0f;

    const float buttonFontSize = 25.0f;
    const float buttonTextSpacing = 2.0f;


    // =====================================
    // Title Settings
    // =====================================

    const char *title = "UNMATCHED";

    const float titleFontSize = 70.0f;
    const float titleSpacing = 3.0f;


    // =====================================
    // Main Loop
    // =====================================

    while (!WindowShouldClose())
    {

        // =================================
        // INPUT
        // =================================

        // Keyboard
        if (IsKeyPressed(KEY_UP))
        {
            selectedButton--;

            if (selectedButton < 0)
                selectedButton = buttonCount - 1;
        }

        if (IsKeyPressed(KEY_DOWN))
        {
            selectedButton++;

            if (selectedButton >= buttonCount)
                selectedButton = 0;
        }


        // =================================
        // Calculate Button Positions
        // =================================

        const float totalButtonsHeight =
            buttonCount * buttonHeight +
            (buttonCount - 1) * buttonSpacing;


        // کل گروه دکمه‌ها در مرکز صفحه
        const float firstButtonY =
            (screenHeight - totalButtonsHeight) / 2.0f + 70.0f;


        // =================================
        // Mouse
        // =================================

        Vector2 mousePosition =
            GetMousePosition();

        for (int i = 0; i < buttonCount; i++)
        {
            Rectangle buttonRect =
            {
                (screenWidth - buttonWidth) / 2.0f,

                firstButtonY +
                i * (buttonHeight + buttonSpacing),

                buttonWidth,
                buttonHeight
            };


            // Mouse hover
            if (CheckCollisionPointRec(
                    mousePosition,
                    buttonRect))
            {
                selectedButton = i;


                // Mouse click
                if (IsMouseButtonPressed(
                        MOUSE_LEFT_BUTTON))
                {
                    if (i == 0)
                    {
                        // START GAME
                    }

                    else if (i == 1)
                    {
                        // LOAD GAME
                    }

                    else if (i == 2)
                    {
                        CloseWindow();
                        return 0;
                    }
                }
            }
        }


        // =================================
        // ENTER
        // =================================

        if (IsKeyPressed(KEY_ENTER))
        {
            if (selectedButton == 0)
            {
                // START GAME
            }

            else if (selectedButton == 1)
            {
                // LOAD GAME
            }

            else if (selectedButton == 2)
            {
                CloseWindow();
                return 0;
            }
        }


        // =================================
        // DRAW
        // =================================

        BeginDrawing();

        ClearBackground(
            Color{20, 20, 30, 255}
        );


        // =================================
        // TITLE
        // =================================

        Vector2 titleSize =
            MeasureTextEx(
                gameFont,
                title,
                titleFontSize,
                titleSpacing
            );


        // عنوان دقیقاً وسط افقی
        const float titleX =
            (screenWidth - titleSize.x) / 2.0f;


        // عنوان
        const float titleY = 100.0f;


        DrawTextEx(
            gameFont,
            title,
            {
                titleX,
                titleY
            },
            titleFontSize,
            titleSpacing,
            WHITE
        );


        // =================================
        // BUTTONS
        // =================================

        for (int i = 0; i < buttonCount; i++)
        {
            // ---------------------------------
            // Button Rectangle
            // ---------------------------------

            Rectangle buttonRect =
            {
                (screenWidth - buttonWidth) / 2.0f,

                firstButtonY +
                i * (buttonHeight + buttonSpacing),

                buttonWidth,
                buttonHeight
            };


            // ---------------------------------
            // Selected
            // ---------------------------------

            const bool selected =
                (i == selectedButton);


            // ---------------------------------
            // Button Color
            // ---------------------------------

            Color buttonColor;

            if (selected)
            {
                buttonColor =
                    Color{80, 130, 220, 255};
            }

            else
            {
                buttonColor =
                    Color{45, 45, 60, 255};
            }


            // ---------------------------------
            // Capsule
            // ---------------------------------

            DrawRectangleRounded(
                buttonRect,

                1.0f,       // کاملاً گرد

                32,         // smoothness

                buttonColor
            );


            // ---------------------------------
            // Text Size
            // ---------------------------------

            Vector2 textSize =
                MeasureTextEx(
                    gameFont,
                    buttons[i],
                    buttonFontSize,
                    buttonTextSpacing
                );


            // ---------------------------------
            // Center Text
            // ---------------------------------

            const float textX =
                buttonRect.x +
                (buttonRect.width -
                 textSize.x) / 2.0f;


            const float textY =
                buttonRect.y +
                (buttonRect.height -
                 textSize.y) / 2.0f;


            // ---------------------------------
            // Draw Text
            // ---------------------------------

            DrawTextEx(
                gameFont,
                buttons[i],

                {
                    textX,
                    textY
                },

                buttonFontSize,
                buttonTextSpacing,
                WHITE
            );
        }


        EndDrawing();
    }


    // =====================================
    // Cleanup
    // =====================================

    UnloadFont(gameFont);

    CloseWindow();

    return 0;
}