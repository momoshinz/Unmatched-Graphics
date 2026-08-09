#include "raylib.h"

struct Button
{
    Rectangle rectangle;
    const char *text;
};

int main()
{
    // ---------------------------------------
    // Window
    // ---------------------------------------

    const int screenWidth = 1000;
    const int screenHeight = 700;

    InitWindow(
        screenWidth,
        screenHeight,
        "Unmatched"
    );

    SetTargetFPS(60);


    // ---------------------------------------
    // Main Menu Buttons
    // ---------------------------------------

    Button startButton = {
        {350, 300, 300, 70},
        "START GAME"
    };

    Button loadButton = {
        {350, 390, 300, 70},
        "LOAD GAME"
    };

    Button exitButton = {
        {350, 480, 300, 70},
        "EXIT"
    };


    // ---------------------------------------
    // Main Loop
    // ---------------------------------------

    while (!WindowShouldClose())
    {
        // Mouse position
        Vector2 mousePosition = GetMousePosition();


        // ---------------------------------------
        // Button Hover
        // ---------------------------------------

        bool startHovered =
            CheckCollisionPointRec(
                mousePosition,
                startButton.rectangle
            );

        bool loadHovered =
            CheckCollisionPointRec(
                mousePosition,
                loadButton.rectangle
            );

        bool exitHovered =
            CheckCollisionPointRec(
                mousePosition,
                exitButton.rectangle
            );


        // ---------------------------------------
        // Button Click
        // ---------------------------------------

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (startHovered)
            {
                TraceLog(LOG_INFO, "START GAME clicked");
            }

            if (loadHovered)
            {
                TraceLog(LOG_INFO, "LOAD GAME clicked");
            }

            if (exitHovered)
            {
                break;
            }
        }


        // ---------------------------------------
        // Drawing
        // ---------------------------------------

        BeginDrawing();

        // Background
        ClearBackground(Color{20, 24, 35, 255});


        // ---------------------------------------
        // Game Title
        // ---------------------------------------

        const char *title = "UNMATCHED";

        int titleFontSize = 70;

        int titleWidth =
            MeasureText(title, titleFontSize);

        DrawText(
            title,
            (screenWidth - titleWidth) / 2,
            100,
            titleFontSize,
            GOLD
        );


        // ---------------------------------------
        // Subtitle
        // ---------------------------------------

        const char *subtitle =
            "BATTLE OF LEGENDS";

        int subtitleFontSize = 20;

        int subtitleWidth =
            MeasureText(
                subtitle,
                subtitleFontSize
            );

        DrawText(
            subtitle,
            (screenWidth - subtitleWidth) / 2,
            185,
            subtitleFontSize,
            LIGHTGRAY
        );


        // ---------------------------------------
        // Draw START GAME button
        // ---------------------------------------

        Color startColor =
            startHovered
                ? Color{90, 170, 255, 255}
                : Color{55, 120, 200, 255};

        DrawRectangleRounded(
            startButton.rectangle,
            1.0f,
            32,
            startColor
        );

        int startTextWidth =
            MeasureText(
                startButton.text,
                25
            );

        DrawText(
            startButton.text,
            startButton.rectangle.x +
                (startButton.rectangle.width - startTextWidth) / 2,
            startButton.rectangle.y + 22,
            25,
            WHITE
        );


        // ---------------------------------------
        // Draw LOAD GAMEbutton
        // ---------------------------------------

        Color loadColor =
            loadHovered
                ? Color{90, 170, 255, 255}
                : Color{55, 120, 200, 255};

        DrawRectangleRounded(
            loadButton.rectangle,
            1.0f,
            32,
            loadColor
        );

        int loadTextWidth =
            MeasureText(
                loadButton.text,
                25
            );

        DrawText(
            loadButton.text,
            loadButton.rectangle.x +
                (loadButton.rectangle.width - loadTextWidth) / 2,
            loadButton.rectangle.y + 22,
            25,
            WHITE
        );


        // ---------------------------------------
        // Draw EXIT button
        // ---------------------------------------

        Color exitColor =
            exitHovered
                ? Color{230, 90, 90, 255}
                : Color{170, 55, 65, 255};

        DrawRectangleRounded(
            exitButton.rectangle,
            1.0f,
            32,
            exitColor
        );

        int exitTextWidth =
            MeasureText(
                exitButton.text,
                25
            );

        DrawText(
            exitButton.text,
            exitButton.rectangle.x +
                (exitButton.rectangle.width - exitTextWidth) / 2,
            exitButton.rectangle.y + 22,
            25,
            WHITE
        );


        EndDrawing();
    }


    // ---------------------------------------
    // Close Window
    // ---------------------------------------

    CloseWindow();

    return 0;
}