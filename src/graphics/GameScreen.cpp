#include "graphics/GameScreen.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include "graphics/MapCoordinates.h"
#include "board/Space.h"
#include "fighter/Hero.h"

GameScreen::GameScreen(
    AssetManager *assets,
    Game *game)
    : assets(assets),
      game(game)
{
}
// ============================================================
// GRAPHICAL POSITION OF EACH SPACE
//
// Coordinates are based on the ORIGINAL 1536 x 1024 map image.
//
// IMPORTANT:
// These coordinates are NOT screen coordinates.
// They are coordinates inside the original map image.
//
// Index 0 -> Board Space 1
// Index 1 -> Board Space 2
// ...
// Index 31 -> Board Space 32
// ============================================================

int GameScreen::update()
{
    if (assets == nullptr)
    {
        return 0;
    }

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

    Rectangle exitButton{
        startX,
        buttonY,
        buttonWidth,
        buttonHeight};

    Rectangle saveButton{
        startX + buttonWidth + buttonGap,
        buttonY,
        buttonWidth,
        buttonHeight};

    Rectangle guideButton{
        startX +
            2.0f * (buttonWidth + buttonGap),
        buttonY,
        buttonWidth,
        buttonHeight};

    Vector2 mousePosition = GetMousePosition();

    // =========================================
    // GUIDE POPUP
    // =========================================

    if (guideOpen)
    {
        const float backWidth = 130.0f;
        const float backHeight = 45.0f;

        Rectangle backButton{
            (GetScreenWidth() - backWidth) / 2.0f,
            GetScreenHeight() - 90.0f,
            backWidth,
            backHeight};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(
                    mousePosition,
                    backButton))
            {
                guideOpen = false;
            }
        }

        // وقتی Guide باز است،
        // هیچ چیز دیگری نباید کلیک شود.
        return 0;
    }

    // =========================================
    // Mouse click
    // =========================================

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        // =========================================
        // EXIT
        // =========================================

        if (CheckCollisionPointRec(
                mousePosition,
                exitButton))
        {
            return 1;
        }

        // =========================================
        // SAVE GAME
        // =========================================

        if (CheckCollisionPointRec(
                mousePosition,
                saveButton))
        {
            // TODO:
            // Save game logic
            return 0;
        }

        // =========================================
        // GUIDE
        // =========================================

        if (CheckCollisionPointRec(
                mousePosition,
                guideButton))
        {
            guideOpen = true;
            return 0;
        }

        // =========================================
        // MAP SPACE CLICK
        // =========================================

        int clickedSpaceId =
            getClickedSpaceId();

        if (clickedSpaceId != -1)
        {
            std::cout
                << "Clicked Space: "
                << clickedSpaceId
                << std::endl;
        }
    }

    return 0;
}

void GameScreen::draw()
{
    if (assets == nullptr)
    {
        return;
    }

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
                static_cast<float>(background.height)},
            Rectangle{
                0.0f,
                0.0f,
                static_cast<float>(GetScreenWidth()),
                static_cast<float>(GetScreenHeight())},
            Vector2{0.0f, 0.0f},
            0.0f,
            WHITE);
    }
    else
    {
        ClearBackground(
            Color{
                45,
                30,
                20,
                255});
    }

    drawMap();
    drawPlayerPanels();
    drawTopButtons();

    if (guideOpen)
    {
        drawGuidePopup();
    }
}

// =========================================
// DRAW MAP
// =========================================

// ============================================================
// DRAW MAP
// ============================================================

void GameScreen::drawMap()
{
    Texture2D map =
        assets->getGameMap();

    if (map.id == 0)
    {
        return;
    }

    // =====================================
    // DRAW MAP
    // =====================================

    float mapX;
    float mapY;
    float scale;
    float mapWidth;
    float mapHeight;

    calculateMapTransform(
        mapX,
        mapY,
        scale,
        mapWidth,
        mapHeight);

    Rectangle source{
        0.0f,
        0.0f,
        static_cast<float>(map.width),
        static_cast<float>(map.height)};

    Rectangle destination{
        mapX,
        mapY,
        mapWidth,
        mapHeight};

    DrawTexturePro(
        map,
        source,
        destination,
        Vector2{0.0f, 0.0f},
        0.0f,
        WHITE);

    // =====================================
    // DRAW SPACE CIRCLES + NUMBERS
    // =====================================

    drawSpaces();

    // =====================================
    // DRAW HEROES + SIDEKICKS
    // =====================================

    drawPlacedFighters();
}

// =========================================
// DRAW PLAYER PANELS
// =========================================

void GameScreen::drawPlayerPanels()
{
    // عرض پنل‌ها
    const float panelWidth = 320.0f;

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
        180};

    // =========================================
    // Left panel
    // =========================================

    Rectangle leftPanel{
        leftPanelX,
        margin,
        panelWidth,
        panelHeight};

    DrawRectangleRounded(
        leftPanel,
        0.12f,
        30,
        panelColor);

    // =========================================
    // Right panel
    // =========================================

    Rectangle rightPanel{
        rightPanelX,
        margin,
        panelWidth,
        panelHeight};

    DrawRectangleRounded(
        rightPanel,
        0.12f,
        30,
        panelColor);
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
        buttonHeight};

    Rectangle saveButton{
        startX +
            buttonWidth +
            buttonGap,

        buttonY,

        buttonWidth,
        buttonHeight};

    Rectangle guideButton{
        startX +
            2.0f *
                (buttonWidth + buttonGap),

        buttonY,

        buttonWidth,
        buttonHeight};

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
        190};

    Color hoverColor{
        65,
        65,
        65,
        210};

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
        exitColor);

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
        saveColor);

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
        guideColor);

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
            spacing);

    Vector2 saveTextSize =
        MeasureTextEx(
            font,
            saveText,
            fontSize,
            spacing);

    Vector2 guideTextSize =
        MeasureTextEx(
            font,
            guideText,
            fontSize,
            spacing);

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
                    2.0f},

        fontSize,
        spacing,
        WHITE);

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
                    2.0f},

        fontSize,
        spacing,
        WHITE);

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
                    2.0f},

        fontSize,
        spacing,
        WHITE);
}

void GameScreen::drawGuidePopup()
{
    Font font = assets->getGameFont();

    // =========================================
    // Dark overlay
    // =========================================

    DrawRectangle(
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight(),
        Color{0, 0, 0, 120});

    // =========================================
    // Popup size
    // =========================================

    const float popupWidth = 900.0f;
    const float popupHeight = 720.0f;

    Rectangle popup{
        (GetScreenWidth() - popupWidth) / 2.0f,
        (GetScreenHeight() - popupHeight) / 2.0f,
        popupWidth,
        popupHeight};

    // =========================================
    // Popup background
    // =========================================

    DrawRectangleRounded(
        popup,
        0.04f,
        20,
        Color{25, 20, 18, 245});

    // =========================================
    // Popup border
    // =========================================

    DrawRectangleRoundedLines(
        popup,
        0.04f,
        20,
        Color{180, 160, 130, 255});

    // =========================================
    // Title
    // =========================================

    const char *title = "INSTRUCTIONS";

    const float titleSize = 42.0f;

    Vector2 titleSizeVec =
        MeasureTextEx(
            font,
            title,
            titleSize,
            2.0f);

    DrawTextEx(
        font,
        title,

        Vector2{
            popup.x +
                (popup.width - titleSizeVec.x) / 2.0f,

            popup.y + 25.0f},

        titleSize,
        2.0f,

        WHITE);

    // =========================================
    // Guide text
    // =========================================

    const float textSize = 22.0f;
    const float spacing = 1.5f;

    float x = popup.x + 45.0f;
    float y = popup.y + 100.0f;

    const float lineHeight = 29.0f;

    DrawTextEx(
        font,
        "Don't You Really Know How to Play :o ? Then Read Carefully ..",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight * 1.7f;

    // =========================================
    // ACTIONS
    // =========================================

    DrawTextEx(
        font,
        "[ ACTIONS ]",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "Attack   ~>  Attack an enemy fighter.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "Maneuver ~>  Move a fighter and draw a card.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "Scheme   ~>  Play a Scheme card.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    // =========================================
    // RULES
    // =========================================

    y += lineHeight * 1.4f;

    DrawTextEx(
        font,
        "[ RULES ]",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[o] Each turn you have 2 actions.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[o] Every Hero card carries a unique effect,",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "    and on top of that each Hero possesses a special ability of their own.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[o] Your hand must never exceed 7 cards.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font, "[o] Heroes enter the battlefield alongside their Sidekicks, NEVER ALONE.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "    allowing instant teleportation between them.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[o] Through tactical moves in each turn, your goal is clear:",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "    defeat the enemy Hero and claim VICTORY.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[o] The younger player chooses the fighter and steps onto the field first.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    // =========================================
    // Good Luck
    // =========================================

    y += lineHeight * 1.5f;

    DrawTextEx(
        font,
        "Good Luck, Fighter. You'll need it :]",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    // =========================================
    // BACK BUTTON
    // =========================================

    const float backWidth = 130.0f;
    const float backHeight = 45.0f;

    Rectangle backButton{
        (GetScreenWidth() - backWidth) / 2.0f,
        popup.y + popup.height - 65.0f,
        backWidth,
        backHeight};

    Vector2 mousePosition =
        GetMousePosition();

    bool hovered =
        CheckCollisionPointRec(
            mousePosition,
            backButton);

    Color backColor =
        hovered
            ? Color{75, 75, 75, 255}
            : Color{40, 40, 40, 255};

    DrawRectangleRounded(
        backButton,
        0.25f,
        20,
        backColor);

    const char *backText = "BACK";

    const float backTextSize = 24.0f;

    Vector2 backTextMeasure =
        MeasureTextEx(
            font,
            backText,
            backTextSize,
            1.5f);

    DrawTextEx(
        font,
        backText,

        Vector2{
            backButton.x +
                (backButton.width -
                 backTextMeasure.x) /
                    2.0f,

            backButton.y +
                (backButton.height -
                 backTextMeasure.y) /
                    2.0f},

        backTextSize,
        1.5f,
        WHITE);
}

// ============================================================
// CALCULATE MAP TRANSFORMATION
// ============================================================

void GameScreen::calculateMapTransform(
    float &mapX,
    float &mapY,
    float &scale,
    float &mapWidth,
    float &mapHeight) const
{
    Texture2D map =
        assets->getGameMap();

    if (map.id == 0)
    {
        mapX = 0.0f;
        mapY = 0.0f;
        scale = 1.0f;
        mapWidth = 0.0f;
        mapHeight = 0.0f;
        return;
    }

    // =========================================
    // Map layout settings
    // =========================================

    const float panelWidth = 300.0f;
    const float mapPadding = 15.0f;
    const float topSpace = 75.0f;

    // =========================================
    // Available map area
    // =========================================

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

    const float scaleX =
        mapAreaWidth /
        static_cast<float>(map.width);

    const float scaleY =
        mapAreaHeight /
        static_cast<float>(map.height);

    scale =
        std::min(scaleX, scaleY);

    // Slight margin around map
    scale *= 0.95f;

    // =========================================
    // Final map dimensions
    // =========================================

    mapWidth =
        static_cast<float>(map.width) *
        scale;

    mapHeight =
        static_cast<float>(map.height) *
        scale;

    // =========================================
    // Center map horizontally
    // =========================================

    mapX =
        mapAreaX +
        (mapAreaWidth - mapWidth) / 2.0f;

    // =========================================
    // Center map vertically
    // =========================================

    mapY =
        mapAreaY +
        (mapAreaHeight - mapHeight) / 2.0f;

    // =========================================
    // Move map slightly upward
    // =========================================

    mapY -= 100.0f;
}

int GameScreen::getClickedSpaceId()
{
    if (assets == nullptr)
    {
        return -1;
    }

    Texture2D map =
        assets->getGameMap();

    if (map.id == 0)
    {
        return -1;
    }

    // =========================================
    // Mouse position
    // =========================================

    Vector2 mouse =
        GetMousePosition();

    // =========================================
    // Map transformation
    // =========================================

    float mapX;
    float mapY;
    float scale;
    float mapWidth;
    float mapHeight;

    calculateMapTransform(
        mapX,
        mapY,
        scale,
        mapWidth,
        mapHeight);

    if (scale <= 0.0f)
    {
        return -1;
    }

    // =========================================
    // Screen -> Original map image
    // =========================================

    float imageX =
        (mouse.x - mapX) / scale;

    float imageY =
        (mouse.y - mapY) / scale;

    // =========================================
    // Outside map
    // =========================================

    if (imageX < 0.0f ||
        imageY < 0.0f ||
        imageX > static_cast<float>(map.width) ||
        imageY > static_cast<float>(map.height))
    {
        return -1;
    }

    // =========================================
    // Check all 32 spaces
    // =========================================

    for (int i = 0; i < 32; i++)
    {
        const SpaceGraphic &space =
            SPACE_GRAPHICS[i];

        float dx =
            imageX - space.center.x;

        float dy =
            imageY - space.center.y;

        float distanceSquared =
            dx * dx + dy * dy;

        float radiusSquared =
            space.radius * space.radius;

        if (distanceSquared <= radiusSquared)
        {
            return i + 1;
        }
    }

    return -1;
}

Vector2 GameScreen::mapImageToScreen(
    Vector2 imagePosition) const
{
    float mapX;
    float mapY;
    float scale;
    float mapWidth;
    float mapHeight;

    calculateMapTransform(
        mapX,
        mapY,
        scale,
        mapWidth,
        mapHeight);

    return Vector2{
        mapX +
            imagePosition.x * scale,

        mapY +
            imagePosition.y * scale};
}

void GameScreen::drawSpaces()
{
    if (assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    float mapX;
    float mapY;
    float scale;
    float mapWidth;
    float mapHeight;

    calculateMapTransform(
        mapX,
        mapY,
        scale,
        mapWidth,
        mapHeight);

    for (int i = 0; i < 32; i++)
    {
        Vector2 center =
            mapImageToScreen(
                SPACE_GRAPHICS[i].center);

        float radius =
            SPACE_GRAPHICS[i].radius * scale;

        DrawCircleV(
            center,
            radius,
            Color{255, 255, 255, 70});

        std::string number =
            std::to_string(i + 1);

        Vector2 textSize =
            MeasureTextEx(
                font,
                number.c_str(),
                14.0f,
                1.0f);

        DrawTextEx(
            font,
            number.c_str(),

            Vector2{
                center.x - textSize.x / 2.0f,
                center.y - textSize.y / 2.0f},

            14.0f,
            1.0f,
            WHITE);
    }
}

void GameScreen::drawPlacedFighters()
{
    if (game == nullptr ||
        assets == nullptr)
    {
        return;
    }

    Font font =
        assets->getGameFont();

    const std::vector<Player *> &players =
        game->getPlayers();

    for (Player *player : players)
    {
        if (player == nullptr)
        {
            continue;
        }

        // =====================================
        // HERO
        // =====================================

        Hero *hero =
            player->getHero();

        if (hero != nullptr)
        {
            Space *space =
                hero->getPosition();

            if (space != nullptr)
            {
                int spaceId =
                    space->getId();

                if (spaceId >= 1 &&
                    spaceId <= 32)
                {
                    Vector2 center =
                        mapImageToScreen(
                            SPACE_GRAPHICS[spaceId - 1].center);

                    DrawCircleV(
                        center,
                        22.0f,
                        Color{
                            180,
                            40,
                            40,
                            230});

                    std::string heroName =
                        hero->getName();

                    Vector2 textSize =
                        MeasureTextEx(
                            font,
                            heroName.c_str(),
                            16.0f,
                            1.0f);

                    DrawTextEx(
                        font,
                        heroName.c_str(),

                        Vector2{
                            center.x -
                                textSize.x / 2.0f,

                            center.y -
                                35.0f},

                        16.0f,
                        1.0f,
                        WHITE);
                }
            }
        }

        // =====================================
        // SIDEKICKS
        // =====================================

        std::vector<Sidekick *> sidekicks =
            player->getSideKicks();

        for (Sidekick *sidekick : sidekicks)
        {
            if (sidekick == nullptr)
            {
                continue;
            }

            Space *space =
                sidekick->getPosition();

            if (space == nullptr)
            {
                continue;
            }

            int spaceId =
                space->getId();

            if (spaceId < 1 ||
                spaceId > 32)
            {
                continue;
            }

            Vector2 center =
                mapImageToScreen(
                    SPACE_GRAPHICS[spaceId - 1].center);

            DrawCircleV(
                center,
                15.0f,
                Color{
                    60,
                    120,
                    200,
                    230});

            std::string sidekickName =
                sidekick->getName();

            Vector2 textSize =
                MeasureTextEx(
                    font,
                    sidekickName.c_str(),
                    12.0f,
                    1.0f);

            DrawTextEx(
                font,
                sidekickName.c_str(),

                Vector2{
                    center.x -
                        textSize.x / 2.0f,

                    center.y +
                        18.0f},

                12.0f,
                1.0f,
                WHITE);
        }
    }
}