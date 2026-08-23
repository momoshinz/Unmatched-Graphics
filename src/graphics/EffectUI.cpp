#include "graphics/EffectUI.h"
#include "graphics/AssetManager.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include "fighter/Fog.h"
#include "board/Board.h"
#include "board/Space.h"
#include "card/Hand.h"
#include "card/Card.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>

// ============================================================
// اسم فایتر -> کلید تکسچر کاراکتر در AssetManager
// ============================================================

static Texture2D getFighterTextureForEffectUI(AssetManager *assets, Fighter *fighter)
{
    if (assets == nullptr || fighter == nullptr)
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
        return assets->getCharacter("sherlock_art");
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

EffectUI::EffectUI(AssetManager *assets)
    : assets(assets)
{
}

// ============================================================
// اسم کارت -> کلید تکسچر
// ============================================================

std::string EffectUI::getCardTextureKey(const Card *card, const std::string &heroName)
{
    if (card == nullptr)
    {
        return "";
    }

    const std::string &name = card->getName();

    if (name == "Feint")
    {
        return (heroName == "DRACULA") ? "FeintDracula" : "FeintSherlock";
    }

    static const std::unordered_map<std::string, std::string> nameToKey = {
        {"Feeding Frenzy", "FeedingFrenzy"},
        {"Beastform", "BeastForm"},
        {"Ambush", "Ambush"},
        {"Thirst For Sustenance", "ThirstForSustenance"},
        {"Look Into My Eyes", "LookIntoMyEyes"},
        {"Dash", "Dash"},
        {"Exploit", "Exploit"},
        {"Mistform", "MistForm"},
        {"Baptism Of Blood", "BaptismOfBlood"},
        {"Prey Upon", "PreyUpon"},
        {"Ravening Seduction", "RaveningSeduction"},
        {"The Game Is Afoot", "TheGameIsAfoot"},
        {"Service Revolver", "ServiceRevolver"},
        {"Counter Punch", "CounterPunch"},
        {"Deduce Strategy", "DeduceStrategy"},
        {"Education Never Ends", "EducationNeverEnds"},
        {"Fixed Point", "FixedPoint"},
        {"Study Methods", "StudyMethods"},
        {"Administer Aid", "AdministerAid"},
        {"Eliminate The Impossible", "EliminateTheImpossible"},
        {"Master Of Disguise", "MasterOfDisguise"},
        {"Coded Notes", "CodedNotes"},
        {"Dreaming Of Revenge", "DreamingOfRevange"},
        {"Emerge From Mist", "EmergeFromMist"},
        {"Impossible To See", "ImpossibleToSee"},
        {"Into Thin Air", "IntoThinAir"},
        {"Lurking", "Lurking"},
        {"Reign Of Terror", "ReignOfTerror"},
        {"Rolling Fog", "RollingFog"},
        {"Slip Away", "SlipAway"},
        {"Step Lightly", "StepLightly"}};

    auto it = nameToKey.find(name);

    if (it == nameToKey.end())
    {
        return "";
    }

    return it->second;
}

// ============================================================
// OPEN
// ============================================================

void EffectUI::open(Game *game, Effect *effect, Fighter *fighter, Fighter *target)
{
    this->game = game;
    this->effect = effect;
    this->fighter = fighter;
    this->target = target;
    this->actingPlayer = (fighter != nullptr) ? fighter->getOwner() : nullptr;

    choice = EffectChoice{};
    subPhase = 0;
    pendingFirstCardIndex = -1;

    candidateSpaces.clear();
    candidateCards.clear();
    cardBoxes.clear();
    selectedCardIndex = -1;

    candidateFighters.clear();
    fighterBoxes.clear();

    candidateFogs.clear();
    fogBoxes.clear();
    selectedFog = nullptr;
    fogSourceSpace = nullptr;

    ready = false;

    if (effect == nullptr)
    {
        open_ = false;
        ready = true;
        return;
    }

    inputKind = effect->getInputKind();

    switch (inputKind)
    {
    case EffectInputKind::None:
        open_ = false;
        ready = true;
        return;

    case EffectInputKind::ChooseAdjacentEmptySpace:
        setupChooseAdjacentEmptySpace();
        break;

    case EffectInputKind::ChooseReachableSpace:
        setupChooseReachableSpace();
        break;

    case EffectInputKind::ChooseAnyEmptySpace:
        setupChooseAnyEmptySpace();
        break;

    case EffectInputKind::ChooseOpponentCardToBurn:
        setupChooseOpponentCardToBurn();
        break;

    case EffectInputKind::ChooseEnemyFighter:
        setupChooseEnemyFighter();
        break;

    case EffectInputKind::ChooseTwoCardsAndOrder:
        setupChooseTwoCardsAndOrder();
        break;

    case EffectInputKind::ChooseFighterMoveThenFogMove:
        setupChooseFighterMoveThenFogMove();
        break;

    case EffectInputKind::ChooseLurkingOption:
        setupChooseLurkingOption();
        break;

    case EffectInputKind::ChooseFogSourceAndDestination:
        setupChooseFogSourceAndDestination();
        break;

    case EffectInputKind::ChooseFogAndDestination:
        setupChooseFogAndDestination();
        break;

    case EffectInputKind::ChooseEnemyAndFogDestination:
        setupChooseEnemyAndFogDestination();
        break;

    case EffectInputKind::ChooseDefeatedSisterAndZoneSpace:
        setupChooseDefeatedSisterAndZoneSpace();
        break;

    case EffectInputKind::ChooseCardsToDiscard:
        setupChooseCardsToDiscard();
        break;

    case EffectInputKind::ChooseFighterAndReachableSpace:
        setupChooseFighterAndReachableSpace();
        break;

    case EffectInputKind::ChooseTargetAdjacentEmptySpace:
        setupChooseTargetAdjacentEmptySpace();
        break;

    case EffectInputKind::ShowOpponentHand:
        setupShowOpponentHand();
        break;
    }
    open_ = true;
}

void EffectUI::finalizeReady()
{
    open_ = false;
    ready = true;
}

// ============================================================
// SETUP: کارت‌های ساده (از قبل داشتیم)
// ============================================================

void EffectUI::setupChooseAdjacentEmptySpace()
{
    candidateSpaces.clear();

    Fighter *referenceFighter = (target != nullptr) ? target : fighter;

    if (referenceFighter == nullptr)
        return;

    Space *center = referenceFighter->getPosition();

    if (center == nullptr)
        return;

    for (Space *neighbor : center->getNeighbors())
    {
        if (neighbor != nullptr && !neighbor->isOccupied())
        {
            candidateSpaces.push_back(neighbor);
        }
    }
}

void EffectUI::setupChooseReachableSpace()
{
    candidateSpaces.clear();

    if (fighter == nullptr || game == nullptr || effect == nullptr)
        return;

    candidateSpaces = game->getBoard().getAvailableMoves(fighter, effect->getMoveRange());
}

void EffectUI::setupChooseOpponentCardToBurn()
{
    candidateCards.clear();

    if (actingPlayer == nullptr || game == nullptr)
        return;

    Player *opponent = game->selectOpponent(*actingPlayer);
    if (opponent == nullptr)
        return;

    for (Card *card : opponent->getHand().getCards())
    {
        if (card != nullptr)
            candidateCards.push_back(card);
        if (candidateCards.size() >= 7)
            break;
    }

    if (!candidateCards.empty())
    {
        layoutCardWindow(candidateCards);
    }
}

void EffectUI::setupChooseEnemyFighter()
{
    candidateFighters.clear();

    if (actingPlayer == nullptr || game == nullptr)
        return;

    Player *opponent = game->selectOpponent(*actingPlayer);
    if (opponent == nullptr)
        return;

    Hero *enemyHero = opponent->getHero();
    if (enemyHero != nullptr && enemyHero->isAlive())
    {
        candidateFighters.push_back(enemyHero);
    }

    for (Sidekick *sidekick : opponent->getSideKicks())
    {
        if (sidekick != nullptr && sidekick->isAlive())
        {
            candidateFighters.push_back(sidekick);
        }
    }

    if (!candidateFighters.empty())
    {
        layoutFighterWindow(candidateFighters);
    }
}

// ============================================================
// SETUP: CodedNotes
// ============================================================

void EffectUI::setupChooseTwoCardsAndOrder()
{
    subPhase = 0;

    if (actingPlayer == nullptr)
        return;

    // کشیدن ۳ کارت همینجا (قبل از نمایش انتخاب)
    actingPlayer->drawCards(3);
    std::cout << "[+] Invisible Man drew 3 cards." << std::endl;

    candidateCards.clear();
    for (Card *card : actingPlayer->getHand().getCards())
    {
        candidateCards.push_back(card);
    }

    layoutCardWindow(candidateCards);
}

// ============================================================
// SETUP: IntoThinAir
// ============================================================

void EffectUI::setupChooseFighterMoveThenFogMove()
{
    // ------------------------------------------------
    // Into Thin Air
    //
    // subPhase 0:
    // انتخاب مستقیم مقصد Invisible Man روی نقشه
    //
    // subPhase 2:
    // انتخاب Fog از پنجره
    //
    // subPhase 3:
    // انتخاب مقصد Fog روی نقشه
    // ------------------------------------------------

    subPhase = 0;

    candidateSpaces.clear();
    candidateFogs.clear();

    selectedFog = nullptr;
    fogSourceSpace = nullptr;

    if (fighter == nullptr || game == nullptr || effect == nullptr)
        return;

    // ---------------------------------------------
    // مرحله اول:
    // خانه‌هایی که Invisible Man می‌تواند به آن‌ها برود
    // ---------------------------------------------

    candidateSpaces =
        game->getBoard().getAvailableMoves(
            fighter,
            effect->getMoveRange());

    std::cout << "[Into Thin Air] Invisible Man movement options: "
              << candidateSpaces.size() << std::endl;

    for (Space *space : candidateSpaces)
    {
        if (space != nullptr)
        {
            std::cout << "  -> Space "
                      << space->getId()
                      << std::endl;
        }
    }

    // اگر هیچ خانه‌ای برای Invisible Man وجود ندارد
    if (candidateSpaces.empty())
    {
        std::cout
            << "[!] Invisible Man has no valid destination."
            << std::endl;

        finalizeReady();
        return;
    }

    // ---------------------------------------------
    // مهم:
    // دیگر layoutYesNo نداریم.
    // مستقیماً وارد انتخاب روی نقشه می‌شویم.
    // ---------------------------------------------
}

// ============================================================
// SETUP: Lurking
// ============================================================

void EffectUI::setupChooseLurkingOption()
{
    subPhase = 0;
    layoutTwoOptions("MOVE TO A FOG TOKEN", "MOVE A FOG TOKEN");
}

// ============================================================
// SETUP: RollingFog
// ============================================================

void EffectUI::setupChooseFogSourceAndDestination()
{
    subPhase = 0;
    candidateFogs.clear();
    selectedFog = nullptr;
    fogSourceSpace = nullptr;

    if (actingPlayer == nullptr)
        return;

    for (Fog *fog : actingPlayer->getFogs())
    {
        if (fog != nullptr)
            candidateFogs.push_back(fog);
    }

    if (candidateFogs.empty())
    {
        std::cout << "[!] No Fog tokens available." << std::endl;
        return;
    }

    layoutFogWindow();
}

// ============================================================
// SETUP: SlipAway
// ============================================================

void EffectUI::setupChooseFogAndDestination()
{
    subPhase = 0;

    if (actingPlayer == nullptr)
        return;

    for (Fog *fog : actingPlayer->getFogs())
    {
        if (fog != nullptr)
            candidateFogs.push_back(fog);
    }

    if (!candidateFogs.empty())
    {
        layoutFogWindow();
    }
}

// ============================================================
// SETUP: StepLightly
// ============================================================

void EffectUI::setupChooseEnemyAndFogDestination()
{
    subPhase = 0;
    candidateFighters.clear();

    if (fighter == nullptr || game == nullptr || actingPlayer == nullptr)
        return;

    for (Space *space : game->getBoard().getAdjacentSpaces(fighter->getPosition()))
    {
        if (space == nullptr)
            continue;

        Fighter *enemy = space->getFighter();

        if (enemy != nullptr && enemy->getOwner() != actingPlayer && enemy->isAlive())
        {
            candidateFighters.push_back(enemy);
        }
    }

    if (!candidateFighters.empty())
    {
        layoutFighterWindow(candidateFighters);
    }
    else
    {
        // بدون دشمن مجاور، مستقیم برو به مرحله‌ی فاگ
        subPhase = 1;

        candidateFogs.clear();
        if (actingPlayer != nullptr)
        {
            for (Fog *fog : actingPlayer->getFogs())
            {
                if (fog != nullptr)
                    candidateFogs.push_back(fog);
            }
        }

        if (!candidateFogs.empty())
        {
            layoutFogWindow();
        }
        else
        {
            finalizeReady();
        }
    }
}

// ============================================================
// LAYOUT HELPERS
// ============================================================

void EffectUI::layoutCardWindow(const std::vector<Card *> &cards)
{
    cardBoxes.clear();
    selectedCardIndex = -1;

    const int maxPerRow = 4;
    const float boxWidth = 220.0f;
    const float boxHeight = 320.0f;
    const float gapX = 25.0f;
    const float gapY = 25.0f;
    const float startY = 130.0f;

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

    const float confirmWidth = 220.0f;
    const float confirmHeight = 55.0f;
    float lastRowBottom = startY + rowCount * (boxHeight + gapY);

    confirmButton = Rectangle{
        (GetScreenWidth() - confirmWidth) / 2.0f,
        lastRowBottom + 15.0f,
        confirmWidth, confirmHeight};
}

void EffectUI::layoutFighterWindow(const std::vector<Fighter *> &fighters)
{
    fighterBoxes.clear();

    const float boxWidth = 220.0f;
    const float boxHeight = 320.0f;
    const float gapX = 25.0f;

    int count = static_cast<int>(fighters.size());
    const float totalWidth = count * boxWidth + (count > 0 ? (count - 1) : 0) * gapX;
    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float startY = 170.0f;

    for (int i = 0; i < count; i++)
    {
        Rectangle box{startX + i * (boxWidth + gapX), startY, boxWidth, boxHeight};
        fighterBoxes.push_back(box);
    }
}

void EffectUI::layoutFogWindow()
{
    fogBoxes.clear();

    const float boxWidth = 220.0f;
    const float boxHeight = 90.0f;
    const float gap = 20.0f;

    int count = static_cast<int>(candidateFogs.size());
    const float totalWidth = count * boxWidth + (count > 0 ? (count - 1) : 0) * gap;
    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float startY = GetScreenHeight() / 2.0f - boxHeight / 2.0f;

    for (int i = 0; i < count; i++)
    {
        Rectangle box{startX + i * (boxWidth + gap), startY, boxWidth, boxHeight};
        fogBoxes.push_back(box);
    }
}

void EffectUI::layoutYesNo()
{
    const float buttonWidth = 160.0f;
    const float buttonHeight = 55.0f;
    const float gap = 30.0f;

    const float totalWidth = 2.0f * buttonWidth + gap;
    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float buttonY = GetScreenHeight() / 2.0f;

    yesButton = Rectangle{startX, buttonY, buttonWidth, buttonHeight};
    noButton = Rectangle{startX + buttonWidth + gap, buttonY, buttonWidth, buttonHeight};
}

void EffectUI::layoutTwoOptions(const std::string &textA, const std::string &textB)
{
    optionTextA = textA;
    optionTextB = textB;

    const float buttonWidth = 320.0f;
    const float buttonHeight = 65.0f;
    const float gap = 30.0f;

    const float totalWidth = 2.0f * buttonWidth + gap;
    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float buttonY = GetScreenHeight() / 2.0f;

    optionButtonA = Rectangle{startX, buttonY, buttonWidth, buttonHeight};
    optionButtonB = Rectangle{startX + buttonWidth + gap, buttonY, buttonWidth, buttonHeight};
}

void EffectUI::beginFogDestinationStage(int range, bool excludeSource)
{
    candidateSpaces.clear();

    if (game == nullptr || selectedFog == nullptr)
        return;

    candidateSpaces = game->getBoard().getAvailableFogMoves(selectedFog, range);

    if (excludeSource && fogSourceSpace != nullptr)
    {
        candidateSpaces.erase(
            std::remove(candidateSpaces.begin(), candidateSpaces.end(), fogSourceSpace),
            candidateSpaces.end());
    }
}

// ============================================================
// UPDATE
// ============================================================

void EffectUI::update()
{
    if (!open_ || ready)
        return;

    if (isChoosingSpace())
        return; // GameScreen کلیک روی نقشه رو پردازش می‌کنه

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return;

    Vector2 mouse = GetMousePosition();

    if (inputKind == EffectInputKind::ChooseDefeatedSisterAndZoneSpace)
    {
        // مرحله اول: انتخاب Sister
        if (subPhase == 0)
        {
            for (size_t i = 0; i < fighterBoxes.size(); i++)
            {
                if (CheckCollisionPointRec(mouse, fighterBoxes[i]))
                {
                    choice.selectedFighter = candidateFighters[i];

                    subPhase = 1;

                    candidateSpaces.clear();

                    // Zone دراکولا را پیدا کن
                    if (fighter != nullptr &&
                        fighter->getPosition() != nullptr)
                    {
                        const vector<ZoneType> &draculaZones =
                            fighter->getPosition()->getZones();

                        for (Space *space : game->getBoard().getSpaces())
                        {
                            if (space == nullptr)
                                continue;

                            if (space->isOccupied())
                                continue;

                            const vector<ZoneType> &zones =
                                space->getZones();

                            bool sameZone = false;

                            for (ZoneType zone : zones)
                            {
                                for (ZoneType draculaZone : draculaZones)
                                {
                                    if (zone == draculaZone)
                                    {
                                        sameZone = true;
                                        break;
                                    }
                                }

                                if (sameZone)
                                    break;
                            }

                            if (sameZone)
                            {
                                candidateSpaces.push_back(space);
                            }
                        }
                    }

                    if (candidateSpaces.empty())
                    {
                        finalizeReady();
                    }

                    return;
                }
            }

            return;
        }

        // مرحله دوم توسط selectSpace انجام می‌شود
        return;
    }

    // -------------------- کارت‌های ساده --------------------

    if (inputKind == EffectInputKind::ChooseOpponentCardToBurn)
    {
        for (size_t i = 0; i < cardBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, cardBoxes[i]))
            {
                selectedCardIndex = static_cast<int>(i);
                return;
            }
        }

        if (selectedCardIndex != -1 && CheckCollisionPointRec(mouse, confirmButton))
        {
            choice.selectedCardIndex = selectedCardIndex;
            finalizeReady();
        }
        return;
    }

    if (inputKind == EffectInputKind::ChooseFighterAndReachableSpace)
    {
        // مرحله اول: انتخاب Fighter
        if (subPhase == 0)
        {
            for (size_t i = 0; i < fighterBoxes.size(); i++)
            {
                if (CheckCollisionPointRec(mouse, fighterBoxes[i]))
                {
                    choice.selectedFighter = candidateFighters[i];

                    subPhase = 1;

                    candidateSpaces.clear();

                    if (game != nullptr &&
                        choice.selectedFighter != nullptr)
                    {
                        candidateSpaces =
                            game->getBoard().getAvailableMoves(
                                choice.selectedFighter,
                                2);
                    }

                    if (candidateSpaces.empty())
                    {
                        finalizeReady();
                    }

                    return;
                }
            }

            return;
        }

        return;
    }

    if (inputKind == EffectInputKind::ChooseEnemyFighter)
    {
        for (size_t i = 0; i < fighterBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, fighterBoxes[i]))
            {
                choice.selectedFighter = candidateFighters[i];
                finalizeReady();
                return;
            }
        }
        return;
    }

    if (inputKind == EffectInputKind::ShowOpponentHand)
    {
        if (CheckCollisionPointRec(mouse, confirmButton))
        {
            finalizeReady();
        }
        return;
    }

    // -------------------- Beastform --------------------

    if (inputKind == EffectInputKind::ChooseCardsToDiscard)
    {
        // -------------------------
        // انتخاب / لغو انتخاب کارت
        // -------------------------

        for (size_t i = 0; i < cardBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, cardBoxes[i]))
            {
                int index = static_cast<int>(i);

                auto it = std::find(
                    choice.selectedCardIndices.begin(),
                    choice.selectedCardIndices.end(),
                    index);

                if (it != choice.selectedCardIndices.end())
                {
                    // لغو انتخاب
                    choice.selectedCardIndices.erase(it);
                }
                else
                {
                    // انتخاب کارت
                    choice.selectedCardIndices.push_back(index);
                }

                return;
            }
        }

        // -------------------------
        // DONE
        // -------------------------

        if (CheckCollisionPointRec(mouse, confirmButton))
        {
            std::cout
                << "[+] BeastForm card selection finished."
                << std::endl;

            finalizeReady();
            return;
        }

        return;
    }

    // -------------------- CodedNotes --------------------

    if (inputKind == EffectInputKind::ChooseTwoCardsAndOrder)
    {
        if (subPhase == 0 || subPhase == 1)
        {
            for (size_t i = 0; i < cardBoxes.size(); i++)
            {
                if (CheckCollisionPointRec(mouse, cardBoxes[i]))
                {
                    if (subPhase == 0)
                    {
                        pendingFirstCardIndex = static_cast<int>(i);
                        choice.selectedCardIndices.clear();
                        choice.selectedCardIndices.push_back(pendingFirstCardIndex);
                        subPhase = 1;
                    }
                    else if (subPhase == 1 && static_cast<int>(i) != pendingFirstCardIndex)
                    {
                        choice.selectedCardIndices.push_back(static_cast<int>(i));

                        std::string firstName = candidateCards[pendingFirstCardIndex]->getName();
                        std::string secondName = candidateCards[i]->getName();

                        layoutTwoOptions(firstName + " ON TOP", secondName + " ON TOP");
                        subPhase = 2;
                    }
                    return;
                }
            }
            return;
        }

        if (subPhase == 2)
        {
            if (CheckCollisionPointRec(mouse, optionButtonA))
            {
                choice.selectedOrder = 1;
                finalizeReady();
            }
            else if (CheckCollisionPointRec(mouse, optionButtonB))
            {
                choice.selectedOrder = 2;
                finalizeReady();
            }
            return;
        }
        return;
    }

    // -------------------- IntoThinAir --------------------

    if (inputKind == EffectInputKind::ChooseFighterMoveThenFogMove)
    {
        // ------------------------------------------------
        // subPhase 0:
        // Invisible Man باید مستقیماً روی نقشه حرکت کند.
        // کلیک روی نقشه توسط GameScreen -> selectSpace()
        // مدیریت می‌شود.
        // ------------------------------------------------

        if (subPhase == 0)
        {
            return;
        }

        // ------------------------------------------------
        // subPhase 2:
        // انتخاب Fog از پنجره
        // ------------------------------------------------

        if (subPhase == 2)
        {
            for (size_t i = 0; i < fogBoxes.size(); i++)
            {
                if (CheckCollisionPointRec(mouse, fogBoxes[i]))
                {
                    if (i >= candidateFogs.size())
                        return;

                    selectedFog = candidateFogs[i];

                    choice.selectedFogId =
                        static_cast<int>(i);

                    subPhase = 3;

                    beginFogDestinationStage(
                        effect->getFogMoveRange(),
                        false);

                    if (candidateSpaces.empty())
                    {
                        std::cout
                            << "[!] No destination for Fog."
                            << std::endl;

                        finalizeReady();
                    }

                    return;
                }
            }

            return;
        }

        // ------------------------------------------------
        // subPhase 3:
        // انتخاب مقصد Fog روی نقشه
        // توسط selectSpace() انجام می‌شود.
        // ------------------------------------------------

        return;
    }

    // -------------------- Lurking --------------------

    if (inputKind == EffectInputKind::ChooseLurkingOption)
    {
        if (subPhase == 0)
        {
            if (CheckCollisionPointRec(mouse, optionButtonA))
            {
                choice.selectedOptionIndex = 1;
                subPhase = 1;

                candidateSpaces.clear();
                if (game != nullptr)
                {
                    for (Space *space : game->getBoard().getSpaces())
                    {
                        if (space != nullptr && space->hasFogToken())
                        {
                            candidateSpaces.push_back(space);
                        }
                    }
                }

                if (candidateSpaces.empty())
                {
                    std::cout << "[!] No Fog token on the board." << std::endl;
                    finalizeReady();
                }
            }
            else if (CheckCollisionPointRec(mouse, optionButtonB))
            {
                choice.selectedOptionIndex = 2;
                subPhase = 2;

                candidateFogs.clear();
                if (actingPlayer != nullptr)
                {
                    for (Fog *fog : actingPlayer->getFogs())
                        if (fog != nullptr)
                            candidateFogs.push_back(fog);
                }

                if (!candidateFogs.empty())
                    layoutFogWindow();
                else
                    finalizeReady();
            }
            return;
        }

        if (subPhase == 2)
        {
            for (size_t i = 0; i < fogBoxes.size(); i++)
            {
                if (CheckCollisionPointRec(mouse, fogBoxes[i]))
                {
                    selectedFog = candidateFogs[i];
                    choice.selectedFogId = static_cast<int>(i);
                    subPhase = 3;
                    beginFogDestinationStage(effect->getFogMoveRange(), false);

                    if (candidateSpaces.empty())
                    {
                        finalizeReady();
                    }
                    return;
                }
            }
            return;
        }
        return;
    }

    // -------------------- SlipAway --------------------

    if (inputKind == EffectInputKind::ChooseFogAndDestination)
    {
        if (subPhase == 0)
        {
            for (size_t i = 0; i < fogBoxes.size(); i++)
            {
                if (CheckCollisionPointRec(mouse, fogBoxes[i]))
                {
                    selectedFog = candidateFogs[i];
                    choice.selectedFogId = static_cast<int>(i);
                    subPhase = 1;

                    candidateSpaces.clear();
                    if (game != nullptr)
                    {
                        for (Space *space : game->getBoard().getSpaces())
                        {
                            if (space != nullptr && !space->isOccupied())
                            {
                                candidateSpaces.push_back(space);
                            }
                        }
                    }

                    if (candidateSpaces.empty())
                    {
                        finalizeReady();
                    }
                    return;
                }
            }
            return;
        }
        return;
    }

    if (inputKind == EffectInputKind::ChooseFogSourceAndDestination)
    {
        if (subPhase == 0)
        {
            for (size_t i = 0; i < fogBoxes.size(); i++)
            {
                if (CheckCollisionPointRec(mouse, fogBoxes[i]))
                {
                    selectedFog = candidateFogs[i];
                    choice.selectedFogId = static_cast<int>(i);
                    fogSourceSpace = selectedFog->getPosition();
                    choice.selectedSpace = fogSourceSpace;

                    subPhase = 1;
                    beginUnlimitedFogDestinationStage(fogSourceSpace);

                    if (candidateSpaces.empty())
                    {
                        std::cout << "[!] No destination for fog." << std::endl;
                        finalizeReady();
                    }
                    return;
                }
            }
            return;
        }
        return;
    }

    // -------------------- StepLightly --------------------

    if (inputKind == EffectInputKind::ChooseEnemyAndFogDestination)
    {
        if (subPhase == 0)
        {
            for (size_t i = 0; i < fighterBoxes.size(); i++)
            {
                if (CheckCollisionPointRec(mouse, fighterBoxes[i]))
                {
                    choice.selectedFighter = candidateFighters[i];
                    subPhase = 1;

                    candidateFogs.clear();
                    if (actingPlayer != nullptr)
                    {
                        for (Fog *fog : actingPlayer->getFogs())
                            if (fog != nullptr)
                                candidateFogs.push_back(fog);
                    }

                    if (!candidateFogs.empty())
                        layoutFogWindow();
                    else
                        finalizeReady();
                    return;
                }
            }
            return;
        }

        if (subPhase == 1)
        {
            for (size_t i = 0; i < fogBoxes.size(); i++)
            {
                if (CheckCollisionPointRec(mouse, fogBoxes[i]))
                {
                    selectedFog = candidateFogs[i];
                    choice.selectedFogId = static_cast<int>(i);
                    subPhase = 2;
                    beginFogDestinationStage(effect->getFogMoveRange(), false);

                    if (candidateSpaces.empty())
                    {
                        finalizeReady();
                    }
                    return;
                }
            }
            return;
        }
        return;
    }
}

// ============================================================
// SELECT SPACE (صدا زده می‌شه از GameScreen وقتی روی نقشه کلیک می‌شه)
// ============================================================

void EffectUI::selectSpace(Space *space)
{
    if (space == nullptr)
        return;

    bool valid = false;

    for (Space *candidate : candidateSpaces)
    {
        if (candidate == space)
        {
            valid = true;
            break;
        }
    }

    if (!valid)
        return;

    // ============================================================
    // انواع تک‌مرحله‌ای
    // ============================================================

    if (inputKind == EffectInputKind::ChooseAdjacentEmptySpace ||
        inputKind == EffectInputKind::ChooseReachableSpace ||
        inputKind == EffectInputKind::ChooseAnyEmptySpace ||
        inputKind == EffectInputKind::ChooseTargetAdjacentEmptySpace)
    {
        choice.selectedSpace = space;

        finalizeReady();
        return;
    }

    // ============================================================
    // ChooseFighterAndReachableSpace
    // ============================================================

    if (inputKind == EffectInputKind::ChooseFighterAndReachableSpace)
    {
        if (subPhase == 1)
        {
            choice.selectedSpace = space;

            finalizeReady();
            return;
        }

        return;
    }

    // ============================================================
    // Into Thin Air
    //
    // subPhase 0:
    //      Invisible Man -> انتخاب مقصد روی نقشه
    //
    // subPhase 2:
    //      انتخاب Fog از پنجره
    //
    // subPhase 3:
    //      Fog -> انتخاب مقصد روی نقشه
    // ============================================================

    if (inputKind == EffectInputKind::ChooseFighterMoveThenFogMove)
    {
        // --------------------------------------------------------
        // مرحله اول:
        // حرکت Invisible Man
        // --------------------------------------------------------

        if (subPhase == 0)
        {
            choice.selectedSpace = space;

            std::cout
                << "[+] Invisible Man destination selected: Space "
                << space->getId()
                << std::endl;

            // ----------------------------------------------------
            // حالا باید Fog انتخاب شود
            // ----------------------------------------------------

            subPhase = 2;

            candidateSpaces.clear();
            candidateFogs.clear();

            selectedFog = nullptr;
            fogSourceSpace = nullptr;

            if (actingPlayer != nullptr)
            {
                for (Fog *fog : actingPlayer->getFogs())
                {
                    if (fog != nullptr)
                    {
                        candidateFogs.push_back(fog);
                    }
                }
            }

            std::cout
                << "[Into Thin Air] Available Fog tokens: "
                << candidateFogs.size()
                << std::endl;

            // ----------------------------------------------------
            // اگر Fog داریم، پنجره انتخاب Fog باز می‌شود
            // ----------------------------------------------------

            if (!candidateFogs.empty())
            {
                layoutFogWindow();
            }
            else
            {
                // هیچ Fogی برای حرکت وجود ندارد
                finalizeReady();
            }

            return;
        }

        // --------------------------------------------------------
        // مرحله سوم:
        // انتخاب مقصد Fog
        // --------------------------------------------------------

        if (subPhase == 3)
        {
            choice.secondSpace = space;

            std::cout
                << "[+] Fog destination selected: Space "
                << space->getId()
                << std::endl;

            finalizeReady();
            return;
        }

        return;
    }

    // ============================================================
    // Lurking
    // ============================================================

    if (inputKind == EffectInputKind::ChooseLurkingOption)
    {
        if (subPhase == 1)
        {
            choice.selectedSpace = space;

            finalizeReady();
            return;
        }

        if (subPhase == 3)
        {
            choice.secondSpace = space;

            finalizeReady();
            return;
        }

        return;
    }

    // ============================================================
    // RollingFog
    // ============================================================

    if (inputKind == EffectInputKind::ChooseFogSourceAndDestination)
    {
        if (subPhase == 1)
        {
            choice.secondSpace = space;

            finalizeReady();
            return;
        }

        return;
    }

    // ============================================================
    // SlipAway
    // ============================================================

    if (inputKind == EffectInputKind::ChooseFogAndDestination)
    {
        if (subPhase == 1)
        {
            choice.selectedSpace = space;

            finalizeReady();
            return;
        }

        return;
    }

    // ============================================================
    // StepLightly
    // ============================================================

    if (inputKind == EffectInputKind::ChooseEnemyAndFogDestination)
    {
        if (subPhase == 2)
        {
            choice.secondSpace = space;

            finalizeReady();
            return;
        }

        return;
    }

    // ============================================================
    // Defeated Sister
    // ============================================================

    if (inputKind == EffectInputKind::ChooseDefeatedSisterAndZoneSpace)
    {
        if (subPhase == 1)
        {
            choice.selectedSpace = space;

            finalizeReady();
            return;
        }

        return;
    }
}

// ============================================================
// DRAW
// ============================================================

void EffectUI::draw()
{
    if (!open_ || assets == nullptr)
        return;

    Font font = assets->getGameFont();
    Vector2 mouse = GetMousePosition();

    // -------------------- انتخاب روی نقشه (بدون overlay تیره) --------------------

    if (isChoosingSpace())
    {
        const char *title;

        if (inputKind == EffectInputKind::ChooseFighterMoveThenFogMove)
        {
            if (subPhase == 0)
            {
                title = "CHOOSE A DESTINATION FOR INVISIBLE MAN";
            }
            else
            {
                title = "CHOOSE A DESTINATION FOR THE FOG";
            }
        }
        else
        {
            title = "CHOOSE A DESTINATION ON THE MAP";
        }

        const float titleSize = 28.0f;

        Vector2 titleTextSize =
            MeasureTextEx(
                font,
                title,
                titleSize,
                1.5f);

        DrawTextEx(
            font,
            title,
            Vector2{
                (GetScreenWidth() - titleTextSize.x) / 2.0f,
                20.0f},
            titleSize,
            1.5f,
            WHITE);

        return;
    }
    // -------------------- بقیه: overlay تیره --------------------

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 190});

    // ---- دو گزینه (Lurking subPhase 0 / CodedNotes subPhase 2) ----
    bool showTwoOptions =
        (inputKind == EffectInputKind::ChooseLurkingOption && subPhase == 0) ||
        (inputKind == EffectInputKind::ChooseTwoCardsAndOrder && subPhase == 2);

    if (showTwoOptions)
    {
        const char *title =
            (inputKind == EffectInputKind::ChooseLurkingOption)
                ? "CHOOSE ONE EFFECT"
                : "WHICH CARD ON TOP?";

        Vector2 titleSize = MeasureTextEx(font, title, 30.0f, 2.0f);
        DrawTextEx(font, title,
                   Vector2{(GetScreenWidth() - titleSize.x) / 2.0f, GetScreenHeight() / 2.0f - 100.0f},
                   30.0f, 2.0f, WHITE);

        bool hoveredA = CheckCollisionPointRec(mouse, optionButtonA);
        DrawRectangleRounded(optionButtonA, 0.3f, 20, hoveredA ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});
        Vector2 aSize = MeasureTextEx(font, optionTextA.c_str(), 20.0f, 1.5f);
        DrawTextEx(font, optionTextA.c_str(),
                   Vector2{optionButtonA.x + (optionButtonA.width - aSize.x) / 2.0f, optionButtonA.y + (optionButtonA.height - aSize.y) / 2.0f},
                   20.0f, 1.5f, WHITE);

        bool hoveredB = CheckCollisionPointRec(mouse, optionButtonB);
        DrawRectangleRounded(optionButtonB, 0.3f, 20, hoveredB ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});
        Vector2 bSize = MeasureTextEx(font, optionTextB.c_str(), 20.0f, 1.5f);
        DrawTextEx(font, optionTextB.c_str(),
                   Vector2{optionButtonB.x + (optionButtonB.width - bSize.x) / 2.0f, optionButtonB.y + (optionButtonB.height - bSize.y) / 2.0f},
                   20.0f, 1.5f, WHITE);
        return;
    }

    // ---- لیست فاگ‌ها ----
    bool showFogList =
        !fogBoxes.empty() &&
        ((inputKind == EffectInputKind::ChooseFighterMoveThenFogMove && subPhase == 2) ||
         (inputKind == EffectInputKind::ChooseLurkingOption && subPhase == 2) ||
         (inputKind == EffectInputKind::ChooseFogAndDestination && subPhase == 0) ||
         (inputKind == EffectInputKind::ChooseEnemyAndFogDestination && subPhase == 1) ||
         (inputKind == EffectInputKind::ChooseFogSourceAndDestination && subPhase == 0));

    if (showFogList)
    {
        const char *title = "CHOOSE A FOG TOKEN";
        Vector2 titleSize = MeasureTextEx(font, title, 32.0f, 2.0f);
        DrawTextEx(font, title,
                   Vector2{(GetScreenWidth() - titleSize.x) / 2.0f, 70.0f},
                   32.0f, 2.0f, WHITE);

        for (size_t i = 0; i < candidateFogs.size(); i++)
        {
            Rectangle box = fogBoxes[i];
            bool hovered = CheckCollisionPointRec(mouse, box);

            DrawRectangleRounded(box, 0.3f, 20, hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});
            DrawRectangleRoundedLines(box, 0.3f, 20, hovered ? WHITE : Color{150, 150, 150, 255});

            std::string text = "Fog " + std::to_string(candidateFogs[i]->getID());
            if (candidateFogs[i]->getPosition() != nullptr)
            {
                text += " { Home " + std::to_string(candidateFogs[i]->getPosition()->getId()) + " }";
            }

            Vector2 textSize = MeasureTextEx(font, text.c_str(), 24.0f, 1.0f);
            DrawTextEx(font, text.c_str(),
                       Vector2{box.x + (box.width - textSize.x) / 2.0f, box.y + (box.height - textSize.y) / 2.0f},
                       24.0f, 1.0f, WHITE);
        }
        return;
    }

    // ---- لیست فایترها (دشمن / دشمن مجاور / خواهر شکست‌خورده) ----
    bool showFighterList =
        !fighterBoxes.empty() &&
        (inputKind == EffectInputKind::ChooseEnemyFighter ||
         (inputKind == EffectInputKind::ChooseEnemyAndFogDestination && subPhase == 0) ||
         (inputKind == EffectInputKind::ChooseDefeatedSisterAndZoneSpace && subPhase == 0) ||
         inputKind == EffectInputKind::ChooseFighterAndReachableSpace);

    if (showFighterList)
    {
        const char *title;

        if (inputKind == EffectInputKind::ChooseDefeatedSisterAndZoneSpace)
        {
            title = "CHOOSE A DEFEATED SISTER";
        }
        else if (inputKind == EffectInputKind::ChooseFighterAndReachableSpace)
        {
            title = "CHOOSE A FIGHTER TO MOVE";
        }
        else
        {
            title = "CHOOSE A FIGHTER";
        }

        Vector2 titleSize =
            MeasureTextEx(font, title, 32.0f, 2.0f);

        DrawTextEx(
            font,
            title,
            Vector2{
                (GetScreenWidth() - titleSize.x) / 2.0f,
                70.0f},
            32.0f,
            2.0f,
            WHITE);

        for (size_t i = 0; i < candidateFighters.size(); i++)
        {
            Fighter *candidate = candidateFighters[i];

            if (candidate == nullptr || i >= fighterBoxes.size())
                continue;

            Rectangle box = fighterBoxes[i];

            bool hovered =
                CheckCollisionPointRec(mouse, box);

            Color boxColor =
                hovered
                    ? Color{75, 75, 75, 245}
                    : Color{35, 35, 35, 235};

            DrawRectangleRounded(
                box,
                0.15f,
                15,
                boxColor);

            (
                box,
                0.15f,
                15,
                hovered
                    ? WHITE
                    : Color{150, 150, 150, 255});

            if (inputKind != EffectInputKind::ChooseFighterAndReachableSpace)
            {
                Texture2D texture = getFighterTextureForEffectUI(assets, candidate);

                if (texture.id != 0)
                {
                    const float imagePadding = 15.0f;

                    Rectangle source{
                        0.0f, 0.0f,
                        static_cast<float>(texture.width),
                        static_cast<float>(texture.height)};

                    Rectangle destination{
                        box.x + imagePadding,
                        box.y + imagePadding,
                        box.width - 2.0f * imagePadding,
                        220.0f};

                    DrawTexturePro(
                        texture, source, destination,
                        Vector2{0.0f, 0.0f}, 0.0f, WHITE);
                }
            }
            std::string name = candidate->getName();
            // ====================================================
            // Ravening Seduction:
            // کادر 180x80 است، پس اسم باید وسط همان کادر باشد.
            // ====================================================

            if (inputKind == EffectInputKind::ChooseFighterAndReachableSpace)
            {
                Vector2 nameSize =
                    MeasureTextEx(
                        font,
                        name.c_str(),
                        20.0f,
                        1.0f);

                DrawTextEx(
                    font,
                    name.c_str(),
                    Vector2{
                        box.x + (box.width - nameSize.x) / 2.0f,
                        box.y + (box.height - nameSize.y) / 2.0f},
                    20.0f,
                    1.0f,
                    WHITE);
            }

            // ====================================================
            // بقیه Effectها:
            // همان ظاهر قبلی با اسم + HP
            // ====================================================
            else
            {
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
                        box.x + (box.width - nameSize.x) / 2.0f,
                        box.y + 245.0f},
                    22.0f,
                    1.0f,
                    WHITE);

                std::string hpText =
                    "HP: " + std::to_string(candidate->getHealth());

                Vector2 hpSize =
                    MeasureTextEx(
                        font,
                        hpText.c_str(),
                        18.0f,
                        1.0f);

                DrawTextEx(
                    font,
                    hpText.c_str(),
                    Vector2{
                        box.x + (box.width - hpSize.x) / 2.0f,
                        box.y + 270.0f},
                    18.0f,
                    1.0f,
                    WHITE);
            }
        }

        return;
    }

    // ---- لیست کارت‌ها (سوزوندن حریف / انتخاب برای CodedNotes / دور ریختن / نمایش دست حریف) ----
    bool showCardList =
        !cardBoxes.empty() &&
        (inputKind == EffectInputKind::ChooseOpponentCardToBurn ||
         inputKind == EffectInputKind::ChooseTwoCardsAndOrder ||
         inputKind == EffectInputKind::ChooseCardsToDiscard ||
         inputKind == EffectInputKind::ShowOpponentHand);

    if (showCardList)
    {
        const char *title;

        if (inputKind == EffectInputKind::ChooseOpponentCardToBurn)
        {
            title = "CHOOSE A CARD TO BURN";
        }
        else if (inputKind == EffectInputKind::ChooseTwoCardsAndOrder)
        {
            title = (subPhase == 0) ? "CHOOSE FIRST CARD" : "CHOOSE SECOND CARD";
        }
        else if (inputKind == EffectInputKind::ChooseCardsToDiscard)
        {
            title = "CHOOSE CARDS TO DISCARD";
        }
        else
        {
            title = "OPPONENT'S HAND";
        }

        Vector2 titleSize = MeasureTextEx(font, title, 32.0f, 2.0f);
        DrawTextEx(font, title,
                   Vector2{(GetScreenWidth() - titleSize.x) / 2.0f, 70.0f},
                   32.0f, 2.0f, WHITE);

        std::string heroName =
            (actingPlayer != nullptr && actingPlayer->getHero() != nullptr)
                ? actingPlayer->getHero()->getName()
                : "";

        for (size_t i = 0; i < candidateCards.size(); i++)
        {
            Card *card = candidateCards[i];
            Rectangle box = cardBoxes[i];

            bool hovered = CheckCollisionPointRec(mouse, box);
            bool selected =
                (inputKind == EffectInputKind::ChooseOpponentCardToBurn &&
                 static_cast<int>(i) == selectedCardIndex) ||

                (inputKind == EffectInputKind::ChooseTwoCardsAndOrder &&
                 static_cast<int>(i) == pendingFirstCardIndex) ||

                (inputKind == EffectInputKind::ChooseCardsToDiscard &&
                 std::find(
                     choice.selectedCardIndices.begin(),
                     choice.selectedCardIndices.end(),
                     static_cast<int>(i)) != choice.selectedCardIndices.end());

            Color boxColor;
            if (selected)
                boxColor = Color{120, 85, 40, 245};
            else if (hovered)
                boxColor = Color{75, 75, 75, 245};
            else
                boxColor = Color{35, 35, 35, 235};

            DrawRectangleRounded(box, 0.08f, 20, boxColor);
            DrawRectangleRoundedLines(box, 0.08f, 20, (hovered || selected) ? WHITE : Color{150, 150, 150, 255});

            std::string textureKey = getCardTextureKey(card, heroName);
            if (!textureKey.empty())
            {
                Texture2D texture = assets->getCard(textureKey);
                if (texture.id != 0)
                {
                    const float padding = 14.0f;
                    Rectangle source{0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
                    Rectangle destination{box.x + padding, box.y + padding, box.width - 2.0f * padding, box.height - 65.0f};
                    DrawTexturePro(texture, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
                }
            }

            std::string name = card->getName();
            Vector2 nameSize = MeasureTextEx(font, name.c_str(), 20.0f, 1.0f);
            DrawTextEx(font, name.c_str(),
                       Vector2{box.x + (box.width - nameSize.x) / 2.0f, box.y + box.height - 40.0f},
                       20.0f, 1.0f, WHITE);
        }

        // ---- دکمه‌ی پایین: BURN / CONFIRM / CLOSE (بسته به نوع) ----
        if (inputKind == EffectInputKind::ChooseOpponentCardToBurn ||
            inputKind == EffectInputKind::ChooseCardsToDiscard ||
            inputKind == EffectInputKind::ShowOpponentHand)
        {
            bool confirmEnabled =
                (inputKind == EffectInputKind::ChooseOpponentCardToBurn)
                    ? (selectedCardIndex != -1)
                    : true; // ChooseCardsToDiscard و ShowOpponentHand همیشه فعال

            bool confirmHovered = CheckCollisionPointRec(mouse, confirmButton);

            Color confirmColor;
            if (!confirmEnabled)
                confirmColor = Color{30, 30, 30, 120};
            else if (confirmHovered)
                confirmColor = Color{140, 40, 40, 245};
            else
                confirmColor = Color{100, 30, 30, 235};

            DrawRectangleRounded(confirmButton, 1.0f, 20, confirmColor);

            const char *confirmText;
            if (inputKind == EffectInputKind::ChooseCardsToDiscard)
                confirmText = "DONE";
            else if (inputKind == EffectInputKind::ShowOpponentHand)
                confirmText = "BACK";
            else
                confirmText = "BURN";

            Vector2 confirmTextSize = MeasureTextEx(font, confirmText, 26.0f, 1.5f);
            DrawTextEx(font, confirmText,
                       Vector2{confirmButton.x + (confirmButton.width - confirmTextSize.x) / 2.0f, confirmButton.y + (confirmButton.height - confirmTextSize.y) / 2.0f},
                       26.0f, 1.5f, confirmEnabled ? WHITE : Color{150, 150, 150, 150});
        }
        return;
    }
}
// ============================================================
// GETTERS
// ============================================================

bool EffectUI::isOpen() const { return open_; }
bool EffectUI::isReady() const { return ready; }
const EffectChoice &EffectUI::getChoice() const { return choice; }

void EffectUI::reset()
{
    open_ = false;
    ready = false;
    choice = EffectChoice{};
    subPhase = 0;
    pendingFirstCardIndex = -1;
    candidateSpaces.clear();
    candidateCards.clear();
    candidateFighters.clear();
    candidateFogs.clear();
    selectedFog = nullptr;
    fogSourceSpace = nullptr;
}

const std::vector<Space *> &EffectUI::getCandidateSpaces() const
{
    return candidateSpaces;
}

bool EffectUI::isChoosingSpace() const
{
    if (!open_)
        return false;

    switch (inputKind)
    {
    case EffectInputKind::ChooseAdjacentEmptySpace:
    case EffectInputKind::ChooseReachableSpace:
    case EffectInputKind::ChooseAnyEmptySpace:
    case EffectInputKind::ChooseTargetAdjacentEmptySpace:
        return true;

    case EffectInputKind::ChooseFighterMoveThenFogMove:
        // Into Thin Air:
        // subPhase 0 = انتخاب مقصد Invisible Man
        // subPhase 3 = انتخاب مقصد Fog
        return subPhase == 0 || subPhase == 3;

    case EffectInputKind::ChooseLurkingOption:
        return subPhase == 1 || subPhase == 3;

    case EffectInputKind::ChooseFogSourceAndDestination:
        return subPhase == 1;

    case EffectInputKind::ChooseFogAndDestination:
        return subPhase == 1;

    case EffectInputKind::ChooseEnemyAndFogDestination:
        return subPhase == 2;

    case EffectInputKind::ChooseDefeatedSisterAndZoneSpace:
        return subPhase == 1;

    case EffectInputKind::ChooseFighterAndReachableSpace:
        return subPhase == 1;

    default:
        return false;
    }
}

void EffectUI::setupChooseCardsToDiscard()
{
    subPhase = 0;

    candidateCards.clear();

    if (actingPlayer == nullptr)
        return;

    for (Card *card : actingPlayer->getHand().getCards())
    {
        if (card != nullptr)
            candidateCards.push_back(card);
    }

    // حتی اگر دست خالی باشد، UI باز می‌شود
    // و کاربر می‌تواند با CONFIRM بدون انتخاب کارت ادامه دهد.
    layoutCardWindow(candidateCards);

    choice.selectedCardIndices.clear();
}

void EffectUI::setupChooseDefeatedSisterAndZoneSpace()
{
    subPhase = 0;

    candidateFighters.clear();
    candidateSpaces.clear();

    if (actingPlayer == nullptr || game == nullptr)
        return;

    // پیدا کردن Sisterهای شکست‌خورده
    for (Sidekick *sidekick : actingPlayer->getSideKicks())
    {
        if (sidekick != nullptr &&
            sidekick->isSister() &&
            !sidekick->isAlive())
        {
            candidateFighters.push_back(sidekick);
        }
    }

    // اگر Sister شکست‌خورده نداریم
    if (candidateFighters.empty())
    {
        finalizeReady();
        return;
    }

    layoutFighterWindow(candidateFighters);
}

void EffectUI::setupChooseAnyEmptySpace()
{
    candidateSpaces.clear();

    if (game == nullptr)
        return;

    for (Space *space : game->getBoard().getSpaces())
    {
        if (space == nullptr)
            continue;

        if (space->isOccupied())
            continue;

        candidateSpaces.push_back(space);
    }
}

void EffectUI::setupChooseFighterAndReachableSpace()
{
    subPhase = 0;

    candidateFighters.clear();
    candidateSpaces.clear();

    if (game == nullptr)
        return;

    for (Space *space : game->getBoard().getSpaces())
    {
        if (space == nullptr)
            continue;

        Fighter *f = space->getFighter();

        if (f != nullptr && f->isAlive())
        {
            candidateFighters.push_back(f);
        }
    }

    if (candidateFighters.empty())
    {
        finalizeReady();
        return;
    }

    layoutRaveningFighters();
}

void EffectUI::setupChooseTargetAdjacentEmptySpace()
{
    candidateSpaces.clear();

    if (target == nullptr || game == nullptr)
        return;

    Space *targetSpace = target->getPosition();

    if (targetSpace == nullptr)
        return;

    for (Space *space : targetSpace->getNeighbors())
    {
        if (space == nullptr)
            continue;

        if (space->isOccupied())
            continue;

        candidateSpaces.push_back(space);
    }
}

void EffectUI::setupShowOpponentHand()
{
    candidateCards.clear();

    if (target == nullptr)
        return;

    Player *opponent = target->getOwner();
    if (opponent == nullptr)
        return;

    for (Card *card : opponent->getHand().getCards())
    {
        if (card != nullptr)
            candidateCards.push_back(card);
        if (candidateCards.size() >= 7)
            break;
    }

    layoutCardWindow(candidateCards);
}

void EffectUI::finishCardSelection()
{
    if (inputKind != EffectInputKind::ChooseCardsToDiscard)
        return;

    // انتخاب کارت‌ها همین الان داخل selectedCardIndices است
    finalizeReady();
}

void EffectUI::layoutRaveningFighters()
{
    fighterBoxes.clear();

    const int maxPerRow = 3;

    const float boxWidth = 180.0f;
    const float boxHeight = 80.0f;

    const float gapX = 25.0f;
    const float gapY = 20.0f;

    const float startY = 170.0f;

    int count = static_cast<int>(candidateFighters.size());

    int rowCount = (count + maxPerRow - 1) / maxPerRow;

    for (int row = 0; row < rowCount; row++)
    {
        int fightersInThisRow =
            std::min(maxPerRow, count - row * maxPerRow);

        float rowWidth =
            fightersInThisRow * boxWidth +
            (fightersInThisRow - 1) * gapX;

        float startX =
            (GetScreenWidth() - rowWidth) / 2.0f;

        for (int col = 0; col < fightersInThisRow; col++)
        {
            float x =
                startX + col * (boxWidth + gapX);

            float y =
                startY + row * (boxHeight + gapY);

            fighterBoxes.push_back(
                Rectangle{
                    x,
                    y,
                    boxWidth,
                    boxHeight});
        }
    }
}

void EffectUI::beginUnlimitedFogDestinationStage(Space *excludeSource)
{
    candidateSpaces.clear();

    if (game == nullptr)
        return;

    for (Space *space : game->getBoard().getSpaces())
    {
        if (space == nullptr)
            continue;
        if (space == excludeSource)
            continue;
        if (space->hasFogToken())
            continue;

        candidateSpaces.push_back(space);
    }
}