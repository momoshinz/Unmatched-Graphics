#include "graphics/GameScreen.h"

GameScreen::GameScreen(AssetManager *assets)
    : assets(assets)
{
}

// =========================================
// UPDATE
// =========================================

int GameScreen::update()
{
    if (assets == nullptr)
    {
        return 0;
    }

    // =========================================
    // Top buttons
    // =========================================

    const float buttonWidth = 145.0f;
    const float buttonHeight = 48.0f;
    const float buttonGap = 15.0f;

    const float totalWidth =
        3.0f * buttonWidth +
        2.0f * buttonGap;

    const float startX =
        (GetScreenWidth() - totalWidth) / 2.0f;

    const float buttonY = 12.0f;

    Rectangle exitButton{
        startX,
        buttonY,
        buttonWidth,
        buttonHeight
    };

    Rectangle saveButton{
        startX + buttonWidth + buttonGap,
        buttonY,
        buttonWidth,
        buttonHeight
    };

    Rectangle guideButton{
        startX +
            2.0f * (buttonWidth + buttonGap),
        buttonY,
        buttonWidth,
        buttonHeight
    };

    Vector2 mousePosition =
        GetMousePosition();

    // =========================================
    // Mouse click
    // =========================================

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        // -----------------------------------------
        // EXIT
        // -----------------------------------------

        if (CheckCollisionPointRec(
                mousePosition,
                exitButton))
        {
            return 1;
        }

        // -----------------------------------------
        // SAVE GAME
        // فعلاً بدون منطق
        // -----------------------------------------

        if (CheckCollisionPointRec(
                mousePosition,
                saveButton))
        {
            // Later:
            // Save game logic
        }

        // -----------------------------------------
        // GUIDE
        // فعلاً بدون منطق
        // -----------------------------------------

        if (CheckCollisionPointRec(
                mousePosition,
                guideButton))
        {
            // Later:
            // Guide logic
        }
    }

    return 0;
}

// =========================================
// DRAW
// =========================================

void GameScreen::draw()
{
    if (assets == nullptr)
    {
        return;
    }

    // =========================================
    // 1. Background
    // =========================================

    Texture2D background =
        assets->getMainPanelBackground();

    if (background.id != 0)
    {
        DrawTexturePro(
            background,

            Rectangle{
                0.0f,
                0.0f,
                static_cast<float>(background.width),
                static_cast<float>(background.height)
            },

            Rectangle{
                0.0f,
                0.0f,
                static_cast<float>(GetScreenWidth()),
                static_cast<float>(GetScreenHeight())
            },

            Vector2{0.0f, 0.0f},

            0.0f,

            WHITE
        );
    }
    else
    {
        ClearBackground(
            Color{
                45,
                30,
                20,
                255
            }
        );
    }

    // =========================================
    // 2. Map
    // =========================================

    drawMap();

    // =========================================
    // 3. Player panels
    // =========================================

    drawPlayerPanels();

    // =========================================
    // 4. Top buttons
    // =========================================

    drawTopButtons();
}

// =========================================
// DRAW MAP
// =========================================

void GameScreen::drawMap()
{
    Texture2D map =
        assets->getGameMap();

    if (map.id == 0)
    {
        return;
    }

    // =========================================
    // Map area
    // =========================================

    const float panelWidth = 263.0f;

    // فاصله بین پنل و نقشه
    const float mapPadding = 15.0f;

    // فضای بالای صفحه برای دکمه‌ها
    const float topSpace = 75.0f;

    const float mapAreaX =
        panelWidth + mapPadding;

    const float mapAreaWidth =
        GetScreenWidth() -
        2.0f * (panelWidth + mapPadding);

    const float mapAreaY =
        topSpace;

    const float mapAreaHeight =
        GetScreenHeight() -
        topSpace;

    // =========================================
    // Calculate scale
    // =========================================

    float scaleX =
        mapAreaWidth /
        static_cast<float>(map.width);

    float scaleY =
        mapAreaHeight /
        static_cast<float>(map.height);

    float scale =
        (scaleX < scaleY)
            ? scaleX
            : scaleY;

    // کمی بزرگ‌تر از حالت قبلی
    scale *= 0.99f;

    float mapWidth =
        map.width * scale;

    float mapHeight =
        map.height * scale;

    // =========================================
    // Center map horizontally
    // =========================================

    float mapX =
        mapAreaX +
        (mapAreaWidth - mapWidth) / 2.0f;

    // =========================================
    // Position map
    // =========================================

    float mapY =
        mapAreaY +
        (mapAreaHeight - mapHeight) / 2.0f;

    // کمی بالاتر
    mapY -= 80.0f;

    // =========================================
    // Draw map
    // =========================================

    DrawTexturePro(
        map,

        Rectangle{
            0.0f,
            0.0f,
            static_cast<float>(map.width),
            static_cast<float>(map.height)
        },

        Rectangle{
            mapX,
            mapY,
            mapWidth,
            mapHeight
        },

        Vector2{
            0.0f,
            0.0f
        },

        0.0f,

        WHITE
    );
}

// =========================================
// DRAW PLAYER PANELS
// =========================================

void GameScreen::drawPlayerPanels()
{
    // عرض پنل‌ها
    const float panelWidth = 263.0f;

    // فاصله از لبه پنجره
    const float margin = 12.0f;

    // ارتفاع پنل
    const float panelHeight =
        GetScreenHeight() -
        2.0f * margin;

    // =========================================
    // Panel positions
    // =========================================

    const float leftPanelX =
        margin;

    const float rightPanelX =
        GetScreenWidth() -
        panelWidth -
        margin;

    // =========================================
    // Panel color
    // =========================================

    Color panelColor{
        0,
        0,
        0,
        180
    };

    // =========================================
    // Left panel
    // =========================================

    Rectangle leftPanel{
        leftPanelX,
        margin,
        panelWidth,
        panelHeight
    };

    DrawRectangleRounded(
        leftPanel,
        0.12f,
        30,
        panelColor
    );

    // =========================================
    // Right panel
    // =========================================

    Rectangle rightPanel{
        rightPanelX,
        margin,
        panelWidth,
        panelHeight
    };

    DrawRectangleRounded(
        rightPanel,
        0.12f,
        30,
        panelColor
    );
}

// =========================================
// DRAW TOP BUTTONS
// =========================================

void GameScreen::drawTopButtons()
{
    Font font =
        assets->getGameFont();

    // =========================================
    // Button dimensions
    // =========================================

    const float buttonWidth = 145.0f;
    const float buttonHeight = 48.0f;

    const float buttonGap = 15.0f;

    const float totalWidth =
        3.0f * buttonWidth +
        2.0f * buttonGap;

    const float startX =
        (GetScreenWidth() - totalWidth) / 2.0f;

    const float buttonY = 12.0f;

    // =========================================
    // Button rectangles
    // =========================================

    Rectangle exitButton{
        startX,
        buttonY,
        buttonWidth,
        buttonHeight
    };

    Rectangle saveButton{
        startX +
            buttonWidth +
            buttonGap,

        buttonY,

        buttonWidth,
        buttonHeight
    };

    Rectangle guideButton{
        startX +
            2.0f *
                (buttonWidth + buttonGap),

        buttonY,

        buttonWidth,
        buttonHeight
    };

    // =========================================
    // Mouse
    // =========================================

    Vector2 mousePosition =
        GetMousePosition();

    // =========================================
    // Colors
    // =========================================

    Color normalColor{
        25,
        25,
        25,
        190
    };

    Color hoverColor{
        65,
        65,
        65,
        210
    };

    // =========================================
    // EXIT
    // =========================================

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
        exitColor
    );

    // =========================================
    // SAVE GAME
    // =========================================

    Color saveColor =
        CheckCollisionPointRec(
            mousePosition,
            saveButton)
            ? hoverColor
            : normalColor;

    DrawRectangleRounded(
        saveButton,
        1.0f,
        32,
        saveColor
    );

    // =========================================
    // GUIDE
    // =========================================

    Color guideColor =
        CheckCollisionPointRec(
            mousePosition,
            guideButton)
            ? hoverColor
            : normalColor;

    DrawRectangleRounded(
        guideButton,
        1.0,
        32,
        guideColor
    );

    // =========================================
    // Text
    // =========================================

    const float fontSize = 28.0f;
    const float spacing = 2.0f;

    const char *exitText =
        "EXIT";

    const char *saveText =
        "SAVE GAME";

    const char *guideText =
        "GUIDE";

    // =========================================
    // Measure text
    // =========================================

    Vector2 exitTextSize =
        MeasureTextEx(
            font,
            exitText,
            fontSize,
            spacing
        );

    Vector2 saveTextSize =
        MeasureTextEx(
            font,
            saveText,
            fontSize,
            spacing
        );

    Vector2 guideTextSize =
        MeasureTextEx(
            font,
            guideText,
            fontSize,
            spacing
        );

    // =========================================
    // EXIT text
    // =========================================

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
                    2.0f
        },

        fontSize,
        spacing,
        WHITE
    );

    // =========================================
    // SAVE GAME text
    // =========================================

    DrawTextEx(
        font,
        saveText,

        Vector2{
            saveButton.x +
                (saveButton.width -
                 saveTextSize.x) /
                    2.0f,

            saveButton.y +
                (saveButton.height -
                 saveTextSize.y) /
                    2.0f
        },

        fontSize,
        spacing,
        WHITE
    );

    // =========================================
    // GUIDE text
    // =========================================

    DrawTextEx(
        font,
        guideText,

        Vector2{
            guideButton.x +
                (guideButton.width -
                 guideTextSize.x) /
                    2.0f,

            guideButton.y +
                (guideButton.height -
                 guideTextSize.y) /
                    2.0f
        },

        fontSize,
        spacing,
        WHITE
    );
}