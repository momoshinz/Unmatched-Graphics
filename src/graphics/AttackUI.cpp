#include "graphics/AttackUI.h"

#include "graphics/AssetManager.h"
#include "fighter/Fighter.h"
#include "player/Player.h"
#include <iostream>
#include <string>

AttackUI::AttackUI(AssetManager *assets)
    : assets(assets)
{
}

// ============================================================
// OPEN ATTACK
// ============================================================

void AttackUI::openAttack(Player *player, const std::vector<Fighter *> &fighters)
{
    selectableFighters.clear();
    fighterBoxes.clear();

    selectableAttackCards.clear();
    attackCardBoxes.clear();

    selectedAttacker = nullptr;
    selectedTarget = nullptr;
    selectedAttackCard = nullptr;
    attackPlayer = player;

    int selectedAttackCardIndex = -1;
    bool confirmedAttackCard = false;

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

    const float totalWidth = selectableFighters.size() * boxWidth +
    (selectableFighters.size() - 1) * gapX;

    const float startX =
        (GetScreenWidth() - totalWidth) / 2.0f;

    const float startY = 170.0f;

    for (size_t i = 0;
        i < selectableFighters.size();
        i++)
    {
        Rectangle box{
            startX + i * (boxWidth + gapX),
            startY,
            boxWidth,
            boxHeight
        };

        fighterBoxes.push_back(box);
    }
    open = true;

    std::cout << "Attack UI opened."
              << std::endl;
}

// ============================================================
// GET CHARACTER TEXTURE
// ============================================================

static Texture2D getFighterTexture(
    AssetManager *assets,
    Fighter *fighter)
{
    if (assets == nullptr ||
        fighter == nullptr)
    {
        return {};
    }

    std::string name =
        fighter->getName();

    // -----------------------------
    // Dracula
    // -----------------------------

    if (name == "DRACULA")
    {
        return assets->getCharacter("dracula");
    }

    // -----------------------------
    // Dracula Sisters
    // -----------------------------

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

    // -----------------------------
    // Sherlock
    // -----------------------------

    if (name == "SHERLOCK HOLMES")
    {
        return assets->getCharacter("sherlock_art");
    }

    // -----------------------------
    // Doctor Watson
    // -----------------------------

    if (name == "* Dr. Watson")
    {
        return assets->getCharacter("drwatson");
    }
    
    // -----------------------------
    // Invisible Man
    // -----------------------------

    if (name == "INVISIBLE MAN")
    {
        return assets->getCharacter("invisible_man");
    }

    return {};
}

// ============================================================
// UPDATE
// ============================================================

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

    // ========================================================
    // SELECT ATTACKER
    // ========================================================

    if (phase == AttackPhase::SelectAttacker)
    {
        for (size_t i = 0;
             i < fighterBoxes.size();
             i++)
        {
            if (CheckCollisionPointRec(
                    mouse,
                    fighterBoxes[i]))
            {
                selectedAttacker =
                    selectableFighters[i];

                std::cout
                    << "Attacker selected : "
                    << selectedAttacker->getName()
                    << std::endl;

                // --------------------------------------------
                // پیدا کردن کارت‌های قابل استفاده
                // --------------------------------------------

                selectableAttackCards.clear();
                attackCardBoxes.clear();

                // این Player باید از openAttack ذخیره شده باشد
                // پس player را به عنوان member در AttackUI نگه می‌داریم.

                for (Card *card : attackPlayer->getHand().getCards())
                {
                    if (card == nullptr)
                        continue;

                    bool isAttackCard =
                        card->isAttack();

                    bool isVersatileCard =
                        card->isVersatile();

                    if (!isAttackCard &&
                        !isVersatileCard)
                    {
                        continue;
                    }

                    if (!card->canBePlayedBy(
                            *selectedAttacker,
                            CardType::Attack))
                    {
                        continue;
                    }

                    selectableAttackCards.push_back(card);
                }

                // --------------------------------------------
                // اگر کارت مناسبی نداریم
                // --------------------------------------------

                if (selectableAttackCards.empty())
                {
                    std::cout
                        << "[!] No playable attack cards."
                        << std::endl;

                    return;
                }

                // --------------------------------------------
                // محاسبه کارت‌ها
                // --------------------------------------------

                const float cardWidth = 260.0f;
                const float cardHeight = 330.0f;
                const float cardGap = 25.0f;

                const float totalWidth =
                    selectableAttackCards.size() * cardWidth +
                    (selectableAttackCards.size() - 1) * cardGap;

                const float startX =
                    (GetScreenWidth() - totalWidth) / 2.0f;

                const float startY = 160.0f;

                for (size_t j = 0;
                     j < selectableAttackCards.size();
                     j++)
                {
                    Rectangle box{
                        startX +
                            j * (cardWidth + cardGap),

                        startY,

                        cardWidth,
                        cardHeight
                    };

                    attackCardBoxes.push_back(box);
                }

                phase = AttackPhase::SelectAttackCard;

                return;
            }
        }
    }

    // ========================================================
    // SELECT ATTACK CARD
    // ========================================================

    if (phase == AttackPhase::SelectAttackCard)
    {
        for (size_t i = 0;
             i < attackCardBoxes.size();
             i++)
        {
            if (CheckCollisionPointRec(
                    mouse,
                    attackCardBoxes[i]))
            {
                selectedAttackCard =
                    selectableAttackCards[i];

                std::cout
                    << "Attack card selected : "
                    << selectedAttackCard->getName()
                    << std::endl;

                open = false;

                return;
            }
        }
    }
}

// ============================================================
// DRAW
// ============================================================

void AttackUI::draw()
{
    if (!open || assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    // ========================================================
    // Dark overlay
    // ========================================================

    DrawRectangle(
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight(),
        Color{0, 0, 0, 190});

    // ========================================================
    // SELECT ATTACK CARD
    // ========================================================

    if (phase == AttackPhase::SelectAttackCard)
    {
        const char *title =
            "CHOOSE ATTACK CARD";

        const float titleSize = 38.0f;

        Vector2 titleMeasure =
            MeasureTextEx(
                font,
                title,
                titleSize,
                2.0f);

        DrawTextEx(
            font,
            title,

            Vector2{
                (GetScreenWidth() -
                 titleMeasure.x) / 2.0f,
                60.0f},

            titleSize,
            2.0f,
            WHITE);

        Vector2 mouse = GetMousePosition();

        for (size_t i = 0; i < selectableAttackCards.size(); i++)
        {
            Card *card = selectableAttackCards[i];

            Rectangle box = attackCardBoxes[i];

            bool hovered =
                CheckCollisionPointRec(mouse, box);

            bool selected =
                (static_cast<int>(i) == selectedAttackCardIndex);

            // ------------------------------------------------
            // Card box
            // ------------------------------------------------

            Color boxColor;

            if (selected)
                boxColor = Color{120, 85, 40, 245};
            else if (hovered)
                boxColor = Color{75, 75, 75, 245};
            else
                boxColor = Color{35, 35, 35, 235};

            DrawRectangleRounded(
                box,
                0.08f,
                20,
                boxColor);
            // ------------------------------------------------
            // Border
            // ------------------------------------------------

            DrawRectangleRoundedLines(
            box,
            0.08f,
            20,
            (hovered || selected)
                ? WHITE
                : Color{150, 150, 150, 255});


            Texture2D texture = getCardTexture(assets, card);

            if (texture.id != 0)
            {
                const float padding = 14.0f;

                Rectangle source{
                    0.0f,
                    0.0f,
                    static_cast<float>(texture.width),
                    static_cast<float>(texture.height)
                };

                Rectangle destination{
                    box.x + padding,
                    box.y + padding,
                    box.width - 2.0f * padding,
                    box.height - 65.0f
                };

                DrawTexturePro(
                    texture,
                    source,
                    destination,
                    Vector2{0.0f, 0.0f},
                    0.0f,
                    WHITE
                );
            }

            // ------------------------------------------------
            // Card name
            // ------------------------------------------------

            std::string name = card->getName();

            const float nameFontSize = 20.0f;

            Vector2 nameSize =
                MeasureTextEx(
                    font,
                    name.c_str(),
                    nameFontSize,
                    1.0f);

            DrawTextEx(
                font,
                name.c_str(),

                Vector2{
                    box.x +
                        (box.width - nameSize.x) / 2.0f,

                    box.y +
                        box.height -
                        40.0f
                },

                nameFontSize,
                1.0f,
                WHITE);
        }
        return;
    }

    // ========================================================
    // SELECT ATTACKER
    // ========================================================

    const char *title =
        "CHOOSE YOUR ATTACKER";

    const float titleSize =
        38.0f;

    Vector2 titleMeasure =
        MeasureTextEx(
            font,
            title,
            titleSize,
            2.0f);

    DrawTextEx(
        font,
        title,

        Vector2{
            (GetScreenWidth() -
             titleMeasure.x) / 2.0f,

            70.0f},

        titleSize,
        2.0f,
        WHITE);

    // ========================================================
    // Fighter boxes
    // ========================================================

    Vector2 mouse =
        GetMousePosition();

    for (size_t i = 0;
         i < selectableFighters.size();
         i++)
    {
        Fighter *fighter =
            selectableFighters[i];

        if (fighter == nullptr)
        {
            continue;
        }

        Rectangle box =
            fighterBoxes[i];

        bool hovered =
            CheckCollisionPointRec(
                mouse,
                box);

        // ----------------------------------------------------
        // Box
        // ----------------------------------------------------

        Color boxColor =
            hovered
                ? Color{75, 75, 75, 245}
                : Color{35, 35, 35, 235};

        DrawRectangleRounded(
            box,
            0.08f,
            20,
            boxColor);

        // ----------------------------------------------------
        // Border
        // ----------------------------------------------------

        DrawRectangleRoundedLines(
            box,
            0.08f,
            20,
            hovered
                ? WHITE
                : Color{150, 150, 150, 255});

        // ----------------------------------------------------
        // Fighter image
        // ----------------------------------------------------

        Texture2D texture =
            getFighterTexture(
                assets,
                fighter);

        if (texture.id != 0)
        {
            const float imagePadding =
                15.0f;

            Rectangle source{
                0.0f,
                0.0f,
                static_cast<float>(
                    texture.width),
                static_cast<float>(
                    texture.height)};

            Rectangle destination{
                box.x + imagePadding,
                box.y + imagePadding,

                box.width -
                    2.0f * imagePadding,

                220.0f};

            DrawTexturePro(
                texture,
                source,
                destination,
                Vector2{0.0f, 0.0f},
                0.0f,
                WHITE);
        }

        // ----------------------------------------------------
        // Fighter name
        // ----------------------------------------------------

        std::string name =
            fighter->getName();

        Vector2 nameSize =
            MeasureTextEx(
                font,
                name.c_str(),
                22.0f,
                1.0f);

        DrawTextEx(
            font,
            name.c_str(),

            Vector2{
                box.x +
                    (box.width -
                     nameSize.x) / 2.0f,

                box.y + 245.0f},

            22.0f,
            1.0f,
            WHITE);

        // ----------------------------------------------------
        // Health
        // ----------------------------------------------------

        std::string healthText =
            "HP: " +
            std::to_string(
                fighter->getHealth());

        Vector2 healthSize =
            MeasureTextEx(
                font,
                healthText.c_str(),
                18.0f,
                1.0f);

        DrawTextEx(
            font,
            healthText.c_str(),

            Vector2{
                box.x +
                    (box.width -
                     healthSize.x) / 2.0f,

                box.y + 270.0f},

            18.0f,
            1.0f,
            WHITE);
    }
}
// ============================================================
// GETTERS
// ============================================================

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

    std::string name = card->getName();

    // Dracula
    if (name == "Beast Form")
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

    if (name == "Thirst for Sustenance")
        return assets->getCard("ThirstForSustenance");

    // Sherlock
    if (name == "Administer Aid")
        return assets->getCard("AdministerAid");

    if (name == "Counter Punch")
        return assets->getCard("CounterPunch");

    if (name == "Deduce Strategy")
        return assets->getCard("DeduceStrategy");

    if (name == "Education Never Ends")
        return assets->getCard("EducationNeverEnds");

    if (name == "Eliminate the Impossible")
        return assets->getCard("EliminateTheImpossible");

    if (name == "Fixed Point")
        return assets->getCard("FixedPoint");

    if (name == "Master of Disguise")
        return assets->getCard("MasterOfDisguise");

    if (name == "Study Methods")
        return assets->getCard("StudyMethods");

    if (name == "The Game Is Afoot")
        return assets->getCard("TheGameIsAfoot");

    if (name == "Service Revolver")
        return assets->getCard("ServiceRevolver");

    // Invisible Man
    if (name == "Coded Notes")
        return assets->getCard("CodedNotes");

    if (name == "Dreaming of Revenge")
        return assets->getCard("DreamingOfRevange");

    if (name == "Emerge from Mist")
        return assets->getCard("EmergeFromMist");

    if (name == "Impossible to See")
        return assets->getCard("ImpossibleToSee");

    if (name == "Into Thin Air")
        return assets->getCard("IntoThinAir");

    if (name == "Lurking")
        return assets->getCard("Lurking");

    if (name == "Reign of Terror")
        return assets->getCard("ReignOfTerror");

    if (name == "Rolling Fog")
        return assets->getCard("RollingFog");

    if (name == "Slip Away")
        return assets->getCard("SlipAway");

    if (name == "Step Lightly")
        return assets->getCard("StepLightly");

    return {};
}