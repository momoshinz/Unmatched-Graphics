#include "graphics/AttackUI.h"
#include "game/Game.h"
#include "board/Board.h"
#include "graphics/AssetManager.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "player/Player.h"
#include <iostream>
#include <string>

AttackUI::AttackUI(AssetManager *assets) : assets(assets)
{
}

void AttackUI::openAttack(Player *player, const std::vector<Fighter *> &fighters, Game *game)
{
    this->game = game;
    selectableFighters.clear();
    fighterBoxes.clear();

    selectableAttackCards.clear();
    attackCardBoxes.clear();

    selectableTargets.clear();
    targetBoxes.clear();

    selectedAttacker = nullptr;
    selectedTarget = nullptr;
    selectedAttackCard = nullptr;
    attackPlayer = player;

    selectedAttackCardIndex = -1;
    confirmedAttackCard = false;

    selectableDefenseCards.clear();
    defenseCardBoxes.clear();

    selectedDefenseCard = nullptr;

    defenseChoiceMade = false;
    wantsDefenseCard = false;

    attackConfirmed = false;
    errorIsDefenseless = false;

    phase = AttackPhase::SelectAttacker;

    if (player == nullptr)
    {
        open = false;
        return;
    }

    for (Fighter *fighter : fighters)
    {
        if (fighter == nullptr)
            continue;

        if (!fighter->isAlive())
            continue;

        selectableFighters.push_back(fighter);
    }

    if (selectableFighters.empty())
    {
        open = false;
        return;
    }

    const float boxWidth = 220.0f;
    const float boxHeight = 320.0f;
    const float gapX = 25.0f;
    const float gapY = 25.0f;

    const float totalWidth = selectableFighters.size() * boxWidth + (selectableFighters.size() - 1) * gapX;

    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;

    const float startY = 170.0f;

    for (size_t i = 0 ; i < selectableFighters.size(); i++)
    {
        Rectangle box{startX + i * (boxWidth + gapX), startY, boxWidth, boxHeight};
        fighterBoxes.push_back(box);
    }
    open = true;
    std::cout << "Attack UI opened." << std::endl;
}

static Texture2D getFighterTexture(AssetManager *assets, Fighter *fighter)
{
    if (assets == nullptr ||
        fighter == nullptr)
    {
        return {};
    }

    std::string name = fighter->getName();

    if (name == "DRACULA")
    {
        return assets->getCharacter("dracula");
    }

    if (name == "* Sister 1")
    {
        return assets->getCharacter("sister1");
    }

    if (name == "* Sister 2")
    {
        return assets->getCharacter("sister2");
    }

    if (name == "* Sister 3")
    {
        return assets->getCharacter("sister3");
    }

    if (name == "SHERLOCK HOLMES")
    {
        return assets->getCharacter("sherlock_transparent");
    }

    if (name == "* Dr. Watson")
    {
        return assets->getCharacter("drwatson");
    }

    if (name == "INVISIBLE MAN")
    {
        return assets->getCharacter("invisible_man");
    }

    return {};
}


void AttackUI::update()
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

    if (showError)
    {
        Rectangle closeButton{(GetScreenWidth() - 220.0f) / 2.0f, 350.0f, 220.0f, 60.0f};

        if (CheckCollisionPointRec(mouse, closeButton))
        {
            bool wasDefenseless = errorIsDefenseless;

            showError = false;
            errorMessage.clear();
            errorIsDefenseless = false; 

            open = false;

            if (wasDefenseless)
            {
                attackConfirmed = true;
            }

            return;
        }

        return;
    }

    if (phase == AttackPhase::SelectAttacker)
    {
        for (size_t i = 0; i < fighterBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, fighterBoxes[i]))
            {
                selectedAttacker = selectableFighters[i];

                std::cout << "Attacker selected : " << selectedAttacker->getName() << std::endl;

                selectableAttackCards.clear();
                attackCardBoxes.clear();

                for (Card *card : attackPlayer->getHand().getCards())
                {
                    if (card == nullptr)
                        continue;

                    bool isAttackCard = card->isAttack();

                    bool isVersatileCard = card->isVersatile();

                    if (!isAttackCard && !isVersatileCard)
                    {
                        continue;
                    }

                    if (!card->canBePlayedBy(*selectedAttacker, CardType::Attack))
                    {
                        continue;
                    }

                    selectableAttackCards.push_back(card);
                }

                if (selectableAttackCards.empty())
                {
                    showError = true;
                    errorMessage = "THIS FIGHTER HAS NO PLAYABLE ATTACK CARD!";
                    errorIsDefenseless = false; 
                    return;
                }

                const float cardWidth = 220.0f;
                const float cardHeight = 320.0f;
                const float cardGap = 25.0f;

                const float totalWidth = selectableAttackCards.size() * cardWidth + 
                    (selectableAttackCards.size() - 1) * cardGap;

                const float startX = (GetScreenWidth() - totalWidth) / 2.0f;

                const float startY = 160.0f;

                for (size_t j = 0; j < selectableAttackCards.size(); j++)
                {
                    Rectangle box{startX + j * (cardWidth + cardGap), startY, cardWidth, cardHeight};

                    attackCardBoxes.push_back(box);
                }

                const float playWidth = 220.0f;
                const float playHeight = 60.0f;

                playButton = Rectangle{(GetScreenWidth() - playWidth) / 2.0f, startY + cardHeight + 25.0f,
                            playWidth, playHeight};

                phase = AttackPhase::SelectAttackCard;
                return;
            }
        }
    }

    if (phase == AttackPhase::SelectAttackCard)
    {
        for (size_t i = 0; i < attackCardBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, attackCardBoxes[i]))
            {
                selectedAttackCardIndex = static_cast<int>(i);

                std::cout << "Attack card selected: " << selectableAttackCards[i]->getName() << std::endl;
                return;
            }
        }

        if (selectedAttackCardIndex != -1 && CheckCollisionPointRec(mouse, playButton))
        {
            selectedAttackCard = selectableAttackCards[selectedAttackCardIndex];

            confirmedAttackCard = true;

            std::cout << "Attack card confirmed: " << selectedAttackCard->getName() << std::endl;

            selectableTargets.clear();
            targetBoxes.clear();

            for (Player *other : game->getPlayers())
            {
                if (other == nullptr || other == attackPlayer) 
                    continue;

                Hero *enemyHero = other->getHero();
                if (enemyHero != nullptr && enemyHero->isAlive())
                {
                    bool inRange = selectedAttacker->isAdjacent(enemyHero, game->getBoard());
                    if (!inRange && selectedAttacker->isRanged())
                    {
                        inRange = selectedAttacker->isInSameZone(enemyHero, game->getBoard());
                    }

                    if (inRange)
                    {
                        selectableTargets.push_back(enemyHero);
                    }
                }

                for (Sidekick *enemySidekick : other->getSideKicks())
                {
                    if (enemySidekick == nullptr || !enemySidekick->isAlive())
                        continue;

                    bool inRange = selectedAttacker->isAdjacent(enemySidekick, game->getBoard());
                    if (!inRange && selectedAttacker->isRanged())
                    {
                        inRange = selectedAttacker->isInSameZone(enemySidekick, game->getBoard());
                    }

                    if (inRange)
                    {
                        selectableTargets.push_back(enemySidekick);
                    }
                }
            }

            if (selectableTargets.empty())
            {
                showError = true;
                errorMessage = "NO VALID TARGET FOR THIS ATTACKER!";
                errorIsDefenseless = false;
                return;
            }

            const float boxWidth = 220.0f;
            const float boxHeight = 320.0f;
            const float gapX = 25.0f;

            const float totalWidth = selectableTargets.size() * boxWidth + (selectableTargets.size() - 1) * gapX;

            const float startX = (GetScreenWidth() - totalWidth) / 2.0f;

            const float startY = 170.0f;

            for (size_t j = 0; j < selectableTargets.size(); j++)
            {
                Rectangle box{startX + j * (boxWidth + gapX), startY, boxWidth, boxHeight};

                targetBoxes.push_back(box);
            }

            phase = AttackPhase::SelectTarget;

            return;
        }
    }

    if (phase == AttackPhase::SelectTarget)
    {
        for (size_t i = 0; i < targetBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, targetBoxes[i]))
            {
                selectedTarget = selectableTargets[i];

                std::cout << "Target selected : " << selectedTarget->getName() << std::endl;

                phase = AttackPhase::AskDefenseCard;

                return;
            }
        }
    }

    if (phase == AttackPhase::AskDefenseCard)
    {
        Rectangle yesButton{GetScreenWidth() / 2.0f - 230.0f, 430.0f, 200.0f, 70.0f};

        Rectangle noButton{GetScreenWidth() / 2.0f + 30.0f, 430.0f, 200.0f, 70.0f};

        if (CheckCollisionPointRec(mouse, yesButton))
        {
            wantsDefenseCard = true;
            defenseChoiceMade = true;

            selectableDefenseCards.clear();
            defenseCardBoxes.clear();

            if (selectedTarget == nullptr)
            {
                std::cout << "[!] No defender selected." << std::endl;
                open = false;
                return;
            }

            Player *defenderPlayer = selectedTarget->getOwner();

            if (defenderPlayer == nullptr)
            {
                std::cout << "[!] Defender has no owner." << std::endl;
                open = false;
                return;
            }

            for (Card *card : defenderPlayer->getHand().getCards())
            {
                if (card == nullptr)
                    continue;

                bool isDefense = card->isDefense();

                bool isVersatile = card->isVersatile();

                if (!isDefense && !isVersatile)
                    continue;

                if (!card->canBePlayedBy(*selectedTarget, CardType::Defense))
                {
                    continue;
                }

                selectableDefenseCards.push_back(card);
            }

            if (selectableDefenseCards.empty())
            {
                showError = true;
                errorMessage = "DEFENDER HAS NO PLAYABLE DEFEND CARD!";
                errorIsDefenseless = true; 
                return;
            }

            const float cardWidth = 220.0f;
            const float cardHeight = 320.0f;
            const float cardGap = 25.0f;

            const float totalWidth = selectableDefenseCards.size() * cardWidth +
                (selectableDefenseCards.size() - 1) * cardGap;

            const float startX = (GetScreenWidth() - totalWidth) / 2.0f;

            const float startY = 170.0f;

            for (size_t i = 0; i < selectableDefenseCards.size(); i++)
            {
                Rectangle box{startX + i * (cardWidth + cardGap), startY, cardWidth, cardHeight};
                defenseCardBoxes.push_back(box);
            }
            phase = AttackPhase::SelectDefenseCard;
            return;
        }

        if (CheckCollisionPointRec(mouse, noButton))
        {
            wantsDefenseCard = false;
            defenseChoiceMade = true;

            selectedDefenseCard = nullptr;

            std::cout << "Defender chose NOT to play a defense card." << std::endl;
            open = false;
            attackConfirmed = true;
            return;
        }
    }

    if (phase == AttackPhase::SelectDefenseCard)
    {
        for (size_t i = 0; i < defenseCardBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, defenseCardBoxes[i]))
            {
                selectedDefenseCard = selectableDefenseCards[i];

                std::cout << "Defense card selected : " << selectedDefenseCard->getName() << std::endl;
                open = false;
                attackConfirmed = true;

                return;
            }
        }
    }
}

void AttackUI::draw()
{
    if (!open || assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    if (showError)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 220});
        const float messageSize = 32.0f;

        Vector2 messageSizeVec = MeasureTextEx(font, errorMessage.c_str(), messageSize, 2.0f);

        DrawTextEx(font, errorMessage.c_str(),

            Vector2{(GetScreenWidth() - messageSizeVec.x) / 2.0f, 250.0f}, messageSize, 2.0f, WHITE);

        Rectangle closeButton{(GetScreenWidth() - 220.0f) / 2.0f, 350.0f, 220.0f, 60.0f};

        Vector2 mouse = GetMousePosition();

        bool hovered = CheckCollisionPointRec(mouse,closeButton);

        DrawRectangleRounded(closeButton, 1.0f, 20, hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});

        DrawRectangleRoundedLines(closeButton, 1.0f, 20, hovered ? WHITE : Color{150, 150, 150, 255});

        const char *text = "CLOSE";

        Vector2 textSize = MeasureTextEx(font, text, 26.0f, 1.5f);

        DrawTextEx(font, text, Vector2{closeButton.x +(closeButton.width - textSize.x) / 2.0f,
                closeButton.y + (closeButton.height - textSize.y) / 2.0f}, 26.0f, 1.5f, WHITE);
        return;
    }

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 190});

    if (phase == AttackPhase::SelectAttackCard)
    {
        const char *title = "CHOOSE ATTACK CARD";

        const float titleSize = 38.0f;

        Vector2 titleMeasure = MeasureTextEx(font, title, titleSize, 2.0f);

        DrawTextEx(font, title, Vector2{(GetScreenWidth() - titleMeasure.x) / 2.0f, 60.0f}, titleSize, 2.0f, WHITE);

        Vector2 mouse = GetMousePosition();

        for (size_t i = 0; i < selectableAttackCards.size(); i++)
        {
            Card *card = selectableAttackCards[i];

            Rectangle box = attackCardBoxes[i];

            bool hovered = CheckCollisionPointRec(mouse, box);

            bool selected = (static_cast<int>(i) == selectedAttackCardIndex);

            Color boxColor;

            if (selected)
            {
                boxColor = Color{120, 85, 40, 245};
            }
            else if (hovered)
            {
                boxColor = Color{75, 75, 75, 245};
            }
            else
            {
                boxColor = Color{35, 35, 35, 235};
            }

            DrawRectangleRounded(box, 0.08f, 20, boxColor);

            DrawRectangleRoundedLines(box, 0.08f, 20, (hovered || selected) ? WHITE : Color{150, 150, 150, 255});

            Texture2D texture = getCardTexture(assets, card);

            if (texture.id != 0)
            {
                const float padding = 14.0f;

                Rectangle source{0.0f,  0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};

                Rectangle destination{box.x + padding, box.y + padding, box.width - 2.0f * padding,
                    box.height - 65.0f};

                DrawTexturePro(texture, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            }

            std::string name = card->getName();

            const float nameFontSize = 20.0f;

            Vector2 nameSize = MeasureTextEx(font, name.c_str(), nameFontSize, 1.0f);

            DrawTextEx(font, name.c_str(),

                Vector2{box.x + (box.width - nameSize.x) / 2.0f, box.y + box.height - 40.0f}, nameFontSize, 1.0f, WHITE);
        }

        bool playEnabled = (selectedAttackCardIndex != -1);

        bool playHovered = CheckCollisionPointRec(mouse, playButton);

        Color playColor;

        if (!playEnabled)
        {
            playColor = Color{30, 30, 30, 120};
        }
        else if (playHovered)
        {
            playColor = Color{75, 75, 75, 245};
        }
        else
        {
            playColor = Color{35, 35, 35, 235};
        }

        DrawRectangleRounded(playButton, 1.0f, 20, playColor);

        DrawRectangleRoundedLines(playButton, 1.0f, 20, playHovered ? WHITE : Color{150, 150, 150, 255});

        const char *playText = "PLAY";

        const float playFontSize = 26.0f;

        Vector2 playTextSize = MeasureTextEx(font, playText, playFontSize, 1.5f);

        DrawTextEx(font, playText, Vector2{playButton.x + (playButton.width - playTextSize.x) / 2.0f,
                playButton.y + (playButton.height - playTextSize.y) / 2.0f}, playFontSize, 1.5f,
                playEnabled ? WHITE : Color{150, 150, 150, 150});

        return;
    }

    if (phase == AttackPhase::SelectAttacker)
    {
        const char *title = "CHOOSE YOUR ATTACKER";

        const float titleSize = 38.0f;

        Vector2 titleMeasure = MeasureTextEx(font, title, titleSize, 2.0f);

        DrawTextEx(font, title, Vector2{(GetScreenWidth() - titleMeasure.x) / 2.0f, 70.0f}, titleSize, 2.0f, WHITE);
    }

    if (phase == AttackPhase::SelectAttacker)
    {
        Vector2 mouse = GetMousePosition();

        for (size_t i = 0; i < selectableFighters.size(); i++)
        {
            Fighter *fighter = selectableFighters[i];

            if (fighter == nullptr)
            {
                continue;
            }

            Rectangle box = fighterBoxes[i];

            bool hovered = CheckCollisionPointRec(mouse, box);

            Color boxColor = hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235};

            DrawRectangleRounded(box, 0.08f, 20, boxColor);

            DrawRectangleRoundedLines(box, 0.08f, 20, hovered ? WHITE  : Color{150, 150, 150, 255});

            Texture2D texture = getFighterTexture(assets, fighter);

            if (texture.id != 0)
            {
                const float imagePadding = 15.0f;

                Rectangle source{0.0f, 0.0f, static_cast<float>( texture.width), static_cast<float>( texture.height)};

                Rectangle destination{box.x + imagePadding, box.y + imagePadding, box.width - 2.0f * imagePadding, 220.0f};

                DrawTexturePro(texture, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            }

            std::string name = fighter->getName();

            Vector2 nameSize = MeasureTextEx(font, name.c_str(), 22.0f, 1.0f);

            DrawTextEx(font, name.c_str(), Vector2{box.x + (box.width - nameSize.x) / 2.0f, box.y + 245.0f}, 22.0f, 1.0f, WHITE);

            string healthText = "HP: " + std::to_string(fighter->getHealth());

            Vector2 healthSize = MeasureTextEx(font, healthText.c_str(), 18.0f, 1.0f);

            DrawTextEx(font, healthText.c_str(), Vector2{box.x + (box.width - healthSize.x) / 2.0f, box.y + 270.0f}, 18.0f, 1.0f, WHITE);
        }
    }

    if (phase == AttackPhase::SelectTarget)
    {
        const char *title = "CHOOSE YOUR TARGET";

        const float titleSize = 38.0f;

        Vector2 titleMeasure = MeasureTextEx(font, title, titleSize, 2.0f);

        DrawTextEx(font, title, Vector2{(GetScreenWidth() - titleMeasure.x) / 2.0f, 70.0f}, titleSize, 2.0f, WHITE);

        Vector2 mouse = GetMousePosition();

        for (size_t i = 0; i < selectableTargets.size(); i++)
        {
            Fighter *fighter = selectableTargets[i];

            Rectangle box = targetBoxes[i];

            bool hovered = CheckCollisionPointRec(mouse, box);

            Color boxColor = hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235};

            DrawRectangleRounded(box, 0.08f, 20, boxColor);

            DrawRectangleRoundedLines(box, 0.08f, 20, hovered ? WHITE : Color{150, 150, 150, 255});

            Texture2D texture = getFighterTexture(assets, fighter);

            if (texture.id != 0)
            {
                const float imagePadding = 15.0f;

                Rectangle source{0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};

                Rectangle destination{box.x + imagePadding, box.y + imagePadding, box.width - 2.0f * imagePadding, 220.0f};

                DrawTexturePro(texture, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            }

            string name = fighter->getName();

            Vector2 nameSize = MeasureTextEx(font, name.c_str(), 22.0f, 1.0f);

            DrawTextEx(font, name.c_str(), Vector2{box.x + (box.width - nameSize.x) / 2.0f, box.y + 245.0f}, 22.0f, 1.0f, WHITE);

            string healthText = "HP: " + std::to_string(fighter->getHealth());

            Vector2 healthSize = MeasureTextEx(font, healthText.c_str(), 18.0f, 1.0f);

            DrawTextEx(font, healthText.c_str(), Vector2{box.x + (box.width - healthSize.x) / 2.0f, box.y + 270.0f}, 18.0f, 1.0f, WHITE);
        }

        return;
    }

    if (phase == AttackPhase::AskDefenseCard)
    {
        const char *title = "DO YOU WANT TO PLAY A DEFENSE CARD?";

        const float titleSize = 32.0f;

        Vector2 titleMeasure = MeasureTextEx(font, title, titleSize, 2.0f);

        DrawTextEx(font, title, Vector2{(GetScreenWidth() - titleMeasure.x) / 2.0f, 180.0f}, titleSize, 2.0f, WHITE);

        Rectangle yesButton{GetScreenWidth() / 2.0f - 230.0f, 430.0f, 200.0f, 70.0f};

        Rectangle noButton{GetScreenWidth() / 2.0f + 30.0f, 430.0f, 200.0f, 70.0f};

        Vector2 mouse = GetMousePosition();

        bool yesHovered = CheckCollisionPointRec(mouse, yesButton);

        bool noHovered = CheckCollisionPointRec(mouse, noButton);

        DrawRectangleRounded(yesButton, 1.0f, 20, yesHovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});

        DrawRectangleRoundedLines(yesButton, 1.0f, 20, yesHovered ? WHITE : Color{150, 150, 150, 255});

        DrawRectangleRounded(noButton, 1.0f, 20, noHovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});

        DrawRectangleRoundedLines(noButton, 1.0, 20, noHovered ? WHITE : Color{150, 150, 150, 255});

        const char *yesText = "YES";

        Vector2 yesSize = MeasureTextEx(font, yesText, 26.0f, 1.0f);

        DrawTextEx(font, yesText, Vector2{yesButton.x +
                    (yesButton.width - yesSize.x) / 2.0f, yesButton.y +
                    (yesButton.height - yesSize.y) / 2.0f}, 26.0f, 1.0f, WHITE);

        const char *noText = "NO";

        Vector2 noSize = MeasureTextEx(font, noText, 26.0f, 1.0f);

        DrawTextEx(font, noText, Vector2{noButton.x +
                    (noButton.width - noSize.x) / 2.0f, noButton.y +
                    (noButton.height - noSize.y) / 2.0f}, 26.0f, 1.0f, WHITE);

        return;
    }

    if (phase == AttackPhase::SelectDefenseCard)
    {
        const char *title = "CHOOSE DEFENSE CARD";

        const float titleSize = 38.0f;

        Vector2 titleMeasure = MeasureTextEx(font, title, titleSize, 2.0f);

        DrawTextEx(font, title, Vector2{(GetScreenWidth() - titleMeasure.x) / 2.0f, 60.0f}, titleSize, 2.0f, WHITE);

        Vector2 mouse = GetMousePosition();

        for (size_t i = 0; i < selectableDefenseCards.size(); i++)
        {
            Card *card = selectableDefenseCards[i];

            Rectangle box = defenseCardBoxes[i];

            bool hovered = CheckCollisionPointRec(mouse, box);

            Color boxColor = hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235};

            DrawRectangleRounded( box, 0.08f, 20, boxColor);

            DrawRectangleRoundedLines(box, 0.08f, 20, hovered ? WHITE : Color{150, 150, 150, 255});

            Texture2D texture = getCardTexture(assets, card);

            if (texture.id != 0)
            {
                const float padding = 14.0f;

                Rectangle source{0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};

                Rectangle destination{box.x + padding,box.y + padding, box.width - 2.0f * padding, box.height - 65.0f};

                DrawTexturePro(texture, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            }

            string name = card->getName();

            const float nameFontSize = 20.0f;

            Vector2 nameSize = MeasureTextEx(font, name.c_str(), nameFontSize, 1.0f);

            DrawTextEx(font, name.c_str(), Vector2{box.x + (box.width - nameSize.x) / 2.0f, box.y + box.height - 40.0f},
                        nameFontSize, 1.0f,WHITE);
        }

        return;
    }
}


bool AttackUI::isOpen() const
{
    return open;
}

Fighter *AttackUI::getSelectedAttacker() const
{
    return selectedAttacker;
}

Fighter *AttackUI::getSelectedTarget() const
{
    return selectedTarget;
}

Card *AttackUI::getSelectedAttackCard() const
{
    return selectedAttackCard;
}

Texture2D AttackUI::getCardTexture(AssetManager *assets, Card *card)
{
    if (assets == nullptr || card == nullptr)
    {
        return {};
    }

    string name = card->getName();

    if (name == "Beastform")
        return assets->getCard("BeastForm");

    if (name == "Ambush")
        return assets->getCard("Ambush");

    if (name == "Baptism of Blood")
        return assets->getCard("BaptismOfBlood");

    if (name == "Dash")
        return assets->getCard("Dash");

    if (name == "Exploit")
        return assets->getCard("Exploit");

    if (name == "Feeding Frenzy")
        return assets->getCard("FeedingFrenzy");

    if (name == "Feint")
        return assets->getCard("FeintDracula");

    if (name == "Look Into My Eyes")
        return assets->getCard("LookIntoMyEyes");

    if (name == "Mist Form")
        return assets->getCard("MistForm");

    if (name == "Prey Upon")
        return assets->getCard("PreyUpon");

    if (name == "Ravening Seduction")
        return assets->getCard("RaveningSeduction");

    if (name == "Thirst For Sustenance")
        return assets->getCard("ThirstForSustenance");

    if (name == "Administer Aid")
        return assets->getCard("AdministerAid");

    if (name == "Counter Punch")
        return assets->getCard("CounterPunch");

    if (name == "Deduce Strategy")
        return assets->getCard("DeduceStrategy");

    if (name == "Education Never Ends")
        return assets->getCard("EducationNeverEnds");

    if (name == "Eliminate The Impossible")
        return assets->getCard("EliminateTheImpossible");

    if (name == "Fixed Point")
        return assets->getCard("FixedPoint");

    if (name == "Master Of Disguise")
        return assets->getCard("MasterOfDisguise");

    if (name == "Study Methods")
        return assets->getCard("StudyMethods");

    if (name == "The Game Is Afoot")
        return assets->getCard("TheGameIsAfoot");

    if (name == "Service Revolver")
        return assets->getCard("ServiceRevolver");

    if (name == "Coded Notes")
        return assets->getCard("CodedNotes");

    if (name == "Dreaming Of Revenge")
        return assets->getCard("DreamingOfRevange");

    if (name == "Emerge From Mist")
        return assets->getCard("EmergeFromMist");

    if (name == "Impossible To See")
        return assets->getCard("ImpossibleToSee");

    if (name == "Into Thin Air")
        return assets->getCard("IntoThinAir");

    if (name == "Lurking")
        return assets->getCard("Lurking");

    if (name == "Reign Of Terror")
        return assets->getCard("ReignOfTerror");

    if (name == "Rolling Fog")
        return assets->getCard("RollingFog");

    if (name == "Slip Away")
        return assets->getCard("SlipAway");

    if (name == "Step Lightly")
        return assets->getCard("StepLightly");

    return {};
}

Card *AttackUI::getSelectedDefenseCard() const
{
    return selectedDefenseCard;
}

bool AttackUI::isAttackConfirmed() const
{
    return attackConfirmed;
}

void AttackUI::resetAttackConfirmed()
{
    attackConfirmed = false;
}

void AttackUI::reset()
{
    open = false;

    game = nullptr;
    attackPlayer = nullptr;

    selectableFighters.clear();
    fighterBoxes.clear();

    selectableAttackCards.clear();
    attackCardBoxes.clear();

    selectableTargets.clear();
    targetBoxes.clear();

    selectableDefenseCards.clear();
    defenseCardBoxes.clear();

    selectedAttacker = nullptr;
    selectedTarget = nullptr;

    selectedAttackCard = nullptr;
    selectedDefenseCard = nullptr;

    selectedAttackCardIndex = -1;

    confirmedAttackCard = false;
    defenseChoiceMade = false;
    wantsDefenseCard = false;

    attackConfirmed = false;

    showError = false;
    errorMessage.clear();
    errorIsDefenseless = false;

    phase = AttackPhase::SelectAttacker;
}