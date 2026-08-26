#include "graphics/HandViewUI.h"
#include "graphics/AssetManager.h"
#include "player/Player.h"
#include "fighter/Hero.h"
#include "card/Hand.h"
#include "card/Card.h"

#include <algorithm>
#include <unordered_map>

HandViewUI::HandViewUI(AssetManager *assets)
    : assets(assets)
{
}

std::string HandViewUI::getCardTextureKeyForHandView(const Card *card, const std::string &heroName)
{
    if (card == nullptr)
        return "";

    const std::string &name = card->getName();

    if (name == "Feint")
    {
        return (heroName == "DRACULA") ? "FeintDracula" : "FeintSherlock";
    }

    static const std::unordered_map<std::string, std::string> nameToKey = {
        {"Feeding Frenzy", "FeedingFrenzy"}, {"Beastform", "BeastForm"}, {"Ambush", "Ambush"}, {"Thirst For Sustenance", "ThirstForSustenance"}, {"Look Into My Eyes", "LookIntoMyEyes"}, {"Dash", "Dash"}, {"Exploit", "Exploit"}, {"Mistform", "MistForm"}, {"Baptism Of Blood", "BaptismOfBlood"}, {"Prey Upon", "PreyUpon"}, {"Ravening Seduction", "RaveningSeduction"}, {"The Game Is Afoot", "TheGameIsAfoot"}, {"Service Revolver", "ServiceRevolver"}, {"Counter Punch", "CounterPunch"}, {"Deduce Strategy", "DeduceStrategy"}, {"Education Never Ends", "EducationNeverEnds"}, {"Fixed Point", "FixedPoint"}, {"Study Methods", "StudyMethods"}, {"Administer Aid", "AdministerAid"}, {"Eliminate The Impossible", "EliminateTheImpossible"}, {"Master Of Disguise", "MasterOfDisguise"}, {"Coded Notes", "CodedNotes"}, {"Dreaming Of Revenge", "DreamingOfRevange"}, {"Emerge From Mist", "EmergeFromMist"}, {"Impossible To See", "ImpossibleToSee"}, {"Into Thin Air", "IntoThinAir"}, {"Lurking", "Lurking"}, {"Reign Of Terror", "ReignOfTerror"}, {"Rolling Fog", "RollingFog"}, {"Slip Away", "SlipAway"}, {"Step Lightly", "StepLightly"}};

    auto it = nameToKey.find(name);
    return (it == nameToKey.end()) ? "" : it->second;
}

void HandViewUI::open(Player *player)
{
    this->player = player;
    cards.clear();

    if (player != nullptr)
    {
        for (Card *card : player->getHand().getCards())
        {
            if (card != nullptr)
                cards.push_back(card);
        }
    }

    layout();
    open_ = true;
}

void HandViewUI::layout()
{
    cardBoxes.clear();

    const int maxPerRow = 4;
    const float boxWidth = 200.0f;
    const float boxHeight = 290.0f;
    const float gapX = 20.0f;
    const float gapY = 20.0f;
    const float startY = 110.0f;

    int total = static_cast<int>(cards.size());
    int rowCount = (total + maxPerRow - 1) / maxPerRow;
    int index = 0;

    for (int row = 0; row < rowCount; row++)
    {
        int cardsInRow = std::min(maxPerRow, total - index);

        float rowWidth = cardsInRow * boxWidth + (cardsInRow - 1) * gapX;
        float rowStartX = (GetScreenWidth() - rowWidth) / 2.0f;
        float rowY = startY + row * (boxHeight + gapY);

        for (int col = 0; col < cardsInRow; col++)
        {
            Rectangle box{rowStartX + col * (boxWidth + gapX), rowY, boxWidth, boxHeight};
            cardBoxes.push_back(box);
            index++;
        }
    }

    const float backWidth = 160.0f;
    const float backHeight = 50.0f;
    float lastRowBottom = startY + rowCount * (boxHeight + gapY);

    backButton = Rectangle{
        (GetScreenWidth() - backWidth) / 2.0f,
        lastRowBottom + 20.0f,
        backWidth,
        backHeight};
}

void HandViewUI::update()
{
    if (!open_)
        return;

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return;

    if (CheckCollisionPointRec(GetMousePosition(), backButton))
    {
        open_ = false;
    }
}

void HandViewUI::close()
{
    open_ = false;
}

bool HandViewUI::isOpen() const
{
    return open_;
}

void HandViewUI::draw()
{
    if (!open_ || assets == nullptr)
        return;

    Font font = assets->getGameFont();
    Vector2 mouse = GetMousePosition();

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 190});

    std::string playerLabel =
        (player != nullptr && player->getHero() != nullptr)
            ? player->getHero()->getName() + "'S HAND"
            : "HAND";

    Vector2 titleSize = MeasureTextEx(font, playerLabel.c_str(), 32.0f, 2.0f);

    DrawTextEx(
        font, playerLabel.c_str(),
        Vector2{(GetScreenWidth() - titleSize.x) / 2.0f, 50.0f},
        32.0f, 2.0f, WHITE);

    std::string heroName =
        (player != nullptr && player->getHero() != nullptr) ? player->getHero()->getName() : "";

    if (cards.empty())
    {
        const char *emptyText = "HAND IS EMPTY";
        Vector2 emptySize = MeasureTextEx(font, emptyText, 26.0f, 1.5f);

        DrawTextEx(
            font, emptyText,
            Vector2{(GetScreenWidth() - emptySize.x) / 2.0f, 200.0f},
            26.0f, 1.5f, WHITE);
    }

    for (size_t i = 0; i < cards.size() && i < cardBoxes.size(); i++)
    {
        Card *card = cards[i];
        Rectangle box = cardBoxes[i];

        DrawRectangleRounded(box, 0.08f, 20, Color{35, 35, 35, 235});
        DrawRectangleRoundedLines(box, 0.08f, 20, Color{150, 150, 150, 255});

        std::string textureKey = getCardTextureKeyForHandView(card, heroName);

        if (!textureKey.empty())
        {
            Texture2D texture = assets->getCard(textureKey);

            if (texture.id != 0)
            {
                const float padding = 12.0f;

                Rectangle source{0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
                Rectangle destination{box.x + padding, box.y + padding, box.width - 2.0f * padding, box.height - 55.0f};

                DrawTexturePro(texture, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            }
        }

        std::string name = card->getName();
        Vector2 nameSize = MeasureTextEx(font, name.c_str(), 17.0f, 1.0f);

        DrawTextEx(
            font, name.c_str(),
            Vector2{box.x + (box.width - nameSize.x) / 2.0f, box.y + box.height - 32.0f},
            17.0f, 1.0f, WHITE);
    }

    bool hovered = CheckCollisionPointRec(mouse, backButton);
    Color backColor = hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235};

    DrawRectangleRounded(backButton, 1.0f, 20, backColor);
    DrawRectangleRoundedLines(backButton, 1.0f, 20, hovered ? WHITE : Color{150, 150, 150, 255});

    const char *backText = "BACK";
    Vector2 backTextSize = MeasureTextEx(font, backText, 24.0f, 1.5f);

    DrawTextEx(
        font, backText,
        Vector2{backButton.x + (backButton.width - backTextSize.x) / 2.0f, backButton.y + (backButton.height - backTextSize.y) / 2.0f},
        24.0f, 1.5f, WHITE);
}