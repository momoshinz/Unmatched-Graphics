#include "raylib.h"

int main()
{
    InitWindow(800, 450, "Unmatched");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(
            "UNMATCHED",
            300,
            200,
            40,
            DARKBLUE
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}