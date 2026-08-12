#include "graphics/GameScreen.h"

GameScreen::GameScreen(AssetManager *assets)
    : assets(assets)
{
}

void GameScreen::update()
{
    // فعلاً چیزی نداریم
}

void GameScreen::draw()
{
    if (assets == nullptr)
    {
        return;
    }

    // -----------------------------
    // Background
    // -----------------------------

    ClearBackground(Color{45, 45, 45, 255});

    // -----------------------------
    // Map
    // -----------------------------

    drawMap();

    // -----------------------------
    // Player panels
    // -----------------------------

    drawPlayerPanels();
}

void GameScreen::drawMap()
{
    Texture2D map = assets->getGameMap();

    if (map.id == 0)
    {
        return;
    }

    const float sidePanelWidth = 230.0f;
    const float padding = 20.0f;

    const float mapAreaX =
        sidePanelWidth + padding;

    const float mapAreaWidth =
        GetScreenWidth() -
        2.0f * (sidePanelWidth + padding);

    const float mapAreaY = 20.0f;

    const float mapAreaHeight =
        GetScreenHeight() - 40.0f;

    float scaleX =
        mapAreaWidth / map.width;

    float scaleY =
        mapAreaHeight / map.height;

    float scale =
        (scaleX < scaleY) ? scaleX : scaleY;

    float mapWidth =
        map.width * scale;

    float mapHeight =
        map.height * scale;

    float mapX =
        mapAreaX +
        (mapAreaWidth - mapWidth) / 2.0f;

    float mapY =
        mapAreaY +
        (mapAreaHeight - mapHeight) / 2.0f;

    Rectangle source{
        0,
        0,
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
        Vector2{0, 0},
        0.0f,
        WHITE);
}

void GameScreen::drawPlayerPanels()
{
    const float panelWidth = 230.0f;
    const float panelY = 20.0f;
    const float panelHeight =
        GetScreenHeight() - 40.0f;

    const float leftPanelX = 20.0f;

    const float rightPanelX =
        GetScreenWidth() -
        panelWidth -
        20.0f;

    Color panelColor{
        30,
        30,
        30,
        230};

    // -----------------------------
    // Left panel
    // -----------------------------

    Rectangle leftPanel{
        leftPanelX,
        panelY,
        panelWidth,
        panelHeight};

    DrawRectangleRounded(
        leftPanel,
        0.08f,
        20,
        panelColor);

    // -----------------------------
    // Right panel
    // -----------------------------

    Rectangle rightPanel{
        rightPanelX,
        panelY,
        panelWidth,
        panelHeight};

    DrawRectangleRounded(
        rightPanel,
        0.08f,
        20,
        panelColor);
}