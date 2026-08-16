#include "graphics/SchemeUI.h"
#include "graphics/AssetManager.h"
#include "card/Hand.h"
#include "card/Card.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>

SchemeUI::SchemeUI(AssetManager *assets)
    : assets(assets)
{
}

// ============================================================
// اسم نمایشی کارت -> کلید تکسچر در AssetManager
// (فقط کارت‌های Scheme، چون این پنجره فقط اونا رو نشون می‌ده)
// ============================================================

std::string SchemeUI::getCardTextureKey(const Card *card)
{
    static const std::unordered_map<std::string, std::string> nameToKey = {
        {"Mistform", "MistForm"},
        {"Baptism Of Blood", "BaptismOfBlood"},
        {"Prey Upon", "PreyUpon"},
        {"Ravening Seduction", "RaveningSeduction"},
        {"Administer Aid", "AdministerAid"},
        {"Eliminate The Impossible", "EliminateTheImpossible"},
        {"Master Of Disguise", "MasterOfDisguise"},
        {"Reign Of Terror", "ReignOfTerror"},
        {"Rolling Fog", "RollingFog"},
        {"Step Lightly", "StepLightly"}};

    if (card == nullptr)
    {
        return "";
    }

    auto it = nameToKey.find(card->getName());

    if (it == nameToKey.end())
    {
        return "";
    }

    return it->second;
}

// ============================================================
// OPEN SCHEME
// ============================================================

void SchemeUI::openScheme(const Hand &hand)
{
    selectableCards.clear();
    cardBoxes.clear();

    selectedIndex = -1;
    confirmed = false;
    emptyMessage = false;

    for (Card *card : hand.getCards())
    {
        if (card == nullptr)
        {
            continue;
        }

        if (!card->isScheme())
        {
            continue;
        }

        selectableCards.push_back(card);

        if (selectableCards.size() >= 7)
        {
            break;
        }
    }

    if (selectableCards.empty())
    {
        open = true;
        emptyMessage = true;
        const float backWidth = 180.0f;
        const float backHeight = 55.0f;

        backButton = Rectangle{
            (GetScreenWidth() - backWidth) / 2.0f,
            (GetScreenHeight() / 2.0f) + 60.0f,
            backWidth,
            backHeight};

        std::cout << "[!] No Scheme card in hand." << std::endl;
        return;
    }

    // ========================================================
    // چیدمان Grid: حداکثر ۳ کارت در هر ردیف
    // ========================================================

    const int maxPerRow = 3;

    const float boxWidth = 220.0f;
    const float boxHeight = 320.0f;
    const float gapX = 25.0f;
    const float gapY = 25.0f;

    const float startY = 130.0f;

    int totalCards = static_cast<int>(selectableCards.size());
    int rowCount = (totalCards + maxPerRow - 1) / maxPerRow;

    int cardIndex = 0;

    for (int row = 0; row < rowCount; row++)
    {
        int cardsInRow = std::min(maxPerRow, totalCards - cardIndex);

        float rowWidth =
            cardsInRow * boxWidth +
            (cardsInRow - 1) * gapX;

        float rowStartX =
            (GetScreenWidth() - rowWidth) / 2.0f;

        float rowY =
            startY + row * (boxHeight + gapY);

        for (int col = 0; col < cardsInRow; col++)
        {
            Rectangle box{
                rowStartX + col * (boxWidth + gapX),
                rowY,
                boxWidth,
                boxHeight};

            cardBoxes.push_back(box);
            cardIndex++;
        }
    }

    // ========================================================
    // دکمه‌ی Play (زیر آخرین ردیف)
    // ========================================================

    const float playWidth = 220.0f;
    const float playHeight = 60.0f;

    float lastRowBottom =
        startY + rowCount * (boxHeight + gapY);

    playButton = Rectangle{
        (GetScreenWidth() - playWidth) / 2.0f,
        lastRowBottom + 15.0f,
        playWidth,
        playHeight};

    open = true;

    std::cout << "Scheme UI opened." << std::endl;
}

// ============================================================
// UPDATE
// ============================================================

void SchemeUI::update()
{
    if (!open)
    {
        return;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return;
    }

    Vector2 mouse = GetMousePosition();

    if (emptyMessage)
    {
        if (CheckCollisionPointRec(mouse, backButton))
        {
            open = false;
            emptyMessage = false;
        }
        return;
    }

    // انتخاب / تعویض کارت
    for (size_t i = 0; i < cardBoxes.size(); i++)
    {
        if (CheckCollisionPointRec(mouse, cardBoxes[i]))
        {
            selectedIndex = static_cast<int>(i);

            std::cout << "Scheme card selected: "
                      << selectableCards[i]->getName()
                      << std::endl;

            return;
        }
    }

    // دکمه‌ی Play (فقط وقتی کارتی انتخاب شده)
    if (selectedIndex != -1 &&
        CheckCollisionPointRec(mouse, playButton))
    {
        confirmed = true;
        open = false;
    }
}

// ============================================================
// DRAW
// ============================================================

void SchemeUI::draw()
{
    if (!open || assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    // Overlay تیره
    DrawRectangle(
        0, 0,
        GetScreenWidth(), GetScreenHeight(),
        Color{0, 0, 0, 190});

    if (emptyMessage)
    {
        const char *message = "HERO HAS NO SCHEME CARD IN HAND!";
        const float messageSize = 36.0f;

        Vector2 messageTextSize =
            MeasureTextEx(font, message, messageSize, 2.0f);

        DrawTextEx(
            font, message,
            Vector2{
                (GetScreenWidth() - messageTextSize.x) / 2.0f,
                (GetScreenHeight() - messageTextSize.y) / 2.0f - 40.0f},
            messageSize, 2.0f, WHITE);

        // دکمه‌ی Back کپسولی
        Vector2 mouse = GetMousePosition();
        bool hovered = CheckCollisionPointRec(mouse, backButton);

        Color backColor =
            hovered
                ? Color{75, 75, 75, 245}
                : Color{35, 35, 35, 235};

        DrawRectangleRounded(backButton, 1.0f, 20, backColor);

        DrawRectangleRoundedLines(
            backButton, 1.0f, 20,
            hovered ? WHITE : Color{150, 150, 150, 255});

        const char *backText = "BACK";
        const float backFontSize = 24.0f;

        Vector2 backTextSize =
            MeasureTextEx(font, backText, backFontSize, 1.5f);

        DrawTextEx(
            font, backText,
            Vector2{
                backButton.x + (backButton.width - backTextSize.x) / 2.0f,
                backButton.y + (backButton.height - backTextSize.y) / 2.0f},
            backFontSize, 1.5f, WHITE);

        return;
    }

    // Title
    const char *title = "CHOOSE YOUR SCHEME CARD";
    const float titleSize = 38.0f;

    Vector2 titleMeasure =
        MeasureTextEx(font, title, titleSize, 2.0f);

    DrawTextEx(
        font, title,
        Vector2{
            (GetScreenWidth() - titleMeasure.x) / 2.0f,
            70.0f},
        titleSize, 2.0f, WHITE);

    Vector2 mouse = GetMousePosition();

    // کارت‌ها
    for (size_t i = 0; i < selectableCards.size(); i++)
    {
        Card *card = selectableCards[i];
        Rectangle box = cardBoxes[i];

        bool hovered = CheckCollisionPointRec(mouse, box);
        bool selected = (static_cast<int>(i) == selectedIndex);

        Color boxColor;
        if (selected)
            boxColor = Color{120, 85, 40, 245};
        else if (hovered)
            boxColor = Color{75, 75, 75, 245};
        else
            boxColor = Color{35, 35, 35, 235};

        DrawRectangleRounded(box, 0.08f, 20, boxColor);

        DrawRectangleRoundedLines(
            box, 0.08f, 20,
            (hovered || selected) ? WHITE : Color{150, 150, 150, 255});

        // تصویر کارت
        // تصویر کارت
        std::string textureKey = getCardTextureKey(card);

        if (!textureKey.empty())
        {
            Texture2D texture = assets->getCard(textureKey);

            if (texture.id != 0)
            {
                const float padding = 14.0f;

                Rectangle source{
                    0.0f, 0.0f,
                    static_cast<float>(texture.width),
                    static_cast<float>(texture.height)};

                Rectangle destination{
                    box.x + padding,
                    box.y + padding,
                    box.width - 2.0f * padding,
                    box.height - 65.0f}; // بزرگ‌تر، جا برای اسم زیرش

                DrawTexturePro(
                    texture, source, destination,
                    Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            }
        }

        // اسم کارت زیر عکس (فونت بزرگ‌تر)
        std::string name = card->getName();

        const float nameFontSize = 20.0f;

        Vector2 nameSize =
            MeasureTextEx(font, name.c_str(), nameFontSize, 1.0f);

        DrawTextEx(
            font, name.c_str(),
            Vector2{
                box.x + (box.width - nameSize.x) / 2.0f,
                box.y + box.height - 40.0f},
            nameFontSize, 1.0f, WHITE);
    }

    // دکمه‌ی Play
    bool playEnabled = (selectedIndex != -1);
    bool playHovered = CheckCollisionPointRec(mouse, playButton);

    Color playColor;
    if (!playEnabled)
        playColor = Color{30, 30, 30, 120};
    else if (playHovered)
        playColor = Color{75, 75, 75, 245};
    else
        playColor = Color{35, 35, 35, 235};

    DrawRectangleRounded(playButton, 1.0f, 20, playColor);

    const char *playText = "PLAY";
    const float playFontSize = 26.0f;

    Vector2 playTextSize =
        MeasureTextEx(font, playText, playFontSize, 1.5f);

    DrawTextEx(
        font, playText,
        Vector2{
            playButton.x + (playButton.width - playTextSize.x) / 2.0f,
            playButton.y + (playButton.height - playTextSize.y) / 2.0f},
        playFontSize, 1.5f,
        playEnabled ? WHITE : Color{150, 150, 150, 150});
}

// ============================================================
// GETTERS
// ============================================================

bool SchemeUI::isOpen() const
{
    return open;
}

bool SchemeUI::isConfirmed() const
{
    return confirmed;
}

void SchemeUI::resetConfirmed()
{
    confirmed = false;
    selectedIndex = -1;
}

Card *SchemeUI::getSelectedCard() const
{
    if (selectedIndex < 0 ||
        selectedIndex >= static_cast<int>(selectableCards.size()))
    {
        return nullptr;
    }

    return selectableCards[selectedIndex];
}