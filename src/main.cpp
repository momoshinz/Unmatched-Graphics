#include "raylib.h"

int main()
{
    InitWindow(1000, 700, "Unmatched");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(DARKBLUE);

        // دکمه
        Rectangle button = {
            350, 300,   // x, y
            300, 80     // width, height
        };

        DrawRectangleRounded(
            button,
            1.0f,       // میزان گردی
            32,         // تعداد segment برای صاف‌تر شدن
            SKYBLUE     // رنگ
        );

        DrawText(
            "START GAME",
            410, 325,
            30,
            WHITE
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}