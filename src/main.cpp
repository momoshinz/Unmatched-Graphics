#include "raylib.h"

int main()
{
    // -----------------------------
    // Window
    // -----------------------------
    const int screenWidth = 1000;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "Unmatched");
    SetTargetFPS(60);

    // -----------------------------
    // Load custom font
    // -----------------------------
    Font gameFont = LoadFont("assets/fonts/MyFont.ttf");

    // -----------------------------
    // Main menu state
    // -----------------------------
    int selectedButton = 0;

    const char *buttons[] = {
        "START GAME",
        "LOAD GAME",
        "EXIT"
    };

    const int buttonCount = 3;

    // -----------------------------
    // Main loop
    // -----------------------------
    while (!WindowShouldClose())
    {
        // =====================================
        // INPUT
        // =====================================

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

        // Mouse selection
        Vector2 mousePosition = GetMousePosition();

        for (int i = 0; i < buttonCount; i++)
        {
            Rectangle buttonRect = {
                350,
                300 + i * 80,
                300,
                55
            };

            if (CheckCollisionPointRec(mousePosition, buttonRect))
            {
                selectedButton = i;

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
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

        // Enter key
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

        // =====================================
        // DRAW
        // =====================================

        BeginDrawing();

        ClearBackground(Color{20, 20, 30, 255});

        // -------------------------------------
        // Game title
        // -------------------------------------

        const char *title = "UNMATCHED";

        float titleFontSize = 70;

        Vector2 titleSize =
            MeasureTextEx(
                gameFont,
                title,
                titleFontSize,
                3
            );

        DrawTextEx(
            gameFont,
            title,
            {
                (screenWidth - titleSize.x) / 2,
                100
            },
            titleFontSize,
            3,
            WHITE
        );

        // -------------------------------------
        // Buttons
        // -------------------------------------

        for (int i = 0; i < buttonCount; i++)
        {
            Rectangle buttonRect = {
                350,
                300 + i * 80,
                300,
                55
            };

            bool selected = (i == selectedButton);

            // Button background
            if (selected)
            {
                DrawRectangleRounded(
                    buttonRect,
                    0.5f,
                    20,
                    Color{80, 130, 220, 255}
                );
            }
            else
            {
                DrawRectangleRounded(
                    buttonRect,
                    0.5f,
                    20,
                    Color{45, 45, 60, 255}
                );
            }

            // Button text
            float fontSize = 25;

            Vector2 textSize =
                MeasureTextEx(
                    gameFont,
                    buttons[i],
                    fontSize,
                    2
                );

            float textX =
                buttonRect.x +
                (buttonRect.width - textSize.x) / 2;

            float textY =
                buttonRect.y +
                (buttonRect.height - textSize.y) / 2;

            DrawTextEx(
                gameFont,
                buttons[i],
                {textX, textY},
                fontSize,
                2,
                WHITE
            );
        }

        EndDrawing();
    }

    // -----------------------------
    // Cleanup
    // -----------------------------

    UnloadFont(gameFont);
    CloseWindow();

    return 0;
}