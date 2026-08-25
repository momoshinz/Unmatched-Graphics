#pragma once
#include <raylib.h>
#include "graphics/AssetManager.h"
#include "game/Game.h"
#include "graphics/PlayerPanel.h"
#include "graphics/AttackUI.h"
#include "graphics/SchemeUI.h"
#include "graphics/ManeuverUI.h"
#include "graphics/EffectUI.h"
#include "graphics/SaveManager.h"
#include "graphics/DraculaAbilityUI.h"

class GameScreen
{
private:
    AssetManager *assets;
    Game *game;
    AttackUI attackUI;
    SchemeUI schemeUI;
    ManeuverUI maneuverUI;
    EffectUI effectUI;
    Card *pendingSchemeCard = nullptr;
    DraculaAbilityUI draculaAbilityUI;

    bool guideOpen = false;

    enum class ActionChoice
    {
        NONE,
        ATTACK,
        MANEUVER,
        SCHEME
    };
    ActionChoice selectedAction;
    Rectangle attackButton;
    Rectangle maneuverButton;
    Rectangle schemeButton;

    enum class AttackPhase
    {
        NONE,
        CHOOSE_ATTACKER,
        CHOOSE_TARGET,
        CHOOSE_ATTACK_CARD,
        CHOOSE_DEFENSE,
        RESOLVE
    };

    AttackPhase attackPhase = AttackPhase::NONE;

    Fighter *attackFighter = nullptr;
    Fighter *attackTarget = nullptr;
    Card *selectedAttackCard = nullptr;

    PlayerPanel rightPlayerPanel;
    PlayerPanel leftPlayerPanel;

    bool combatInProgress = false;
    bool combatEffectRequested = false;
    bool combatShowLookButton = false;
    Rectangle lookButton{};
    void drawLookButton(); // for study methods

    bool combatResultPopupOpen = false;
    Rectangle resultRevealButton{};
    Rectangle resultBackButton{};

    bool deckEmptyPopupOpen = false;
    Rectangle deckEmptyOkButton{};

    // the process of checking hand limit
    bool handLimitPopupOpen = false;
    Player *handLimitPlayer = nullptr;
    std::vector<Card *> handLimitCards;
    std::vector<Rectangle> handLimitCardBoxes;
    int handLimitSelectedIndex = -1;
    Rectangle handLimitBurnButton{};

    Fighter *pendingEffectFighter = nullptr;

    SaveManager saveManager;
    bool saveMenuOpen = false;
    std::vector<std::string> saveFiles;
    std::vector<Rectangle> saveButtons;
    Rectangle newSaveButton;
    Rectangle saveMenuBackButton;

    bool feintBlockedPopupOpen = false;
    std::string feintBlockedMessage;
    Rectangle feintBlockedOkButton{};

    float feintBlockedPopupTimer = 0.0f;
    const float feintBlockedPopupDuration = 8.0f;

public:
    GameScreen(AssetManager *assets, Game *game);

    int update();
    void draw();

    Vector2 mapImageToScreen(Vector2 imagePosition) const;
    void calculateMapTransform(
        float &mapX,
        float &mapY,
        float &scale,
        float &mapWidth,
        float &mapHeight) const;

    void drawMap();
    void drawPlayerPanels();
    void drawTopButtons();
    void drawGuidePopup();
    void drawSpaces();
    void drawPlacedFighters();
    
    int getClickedSpaceId();
    void drawFogs();
    void drawActionButtons();
    Fighter *getClickedFighter();
    void drawTurnIndicator();
    void checkAndEndTurnIfNeeded();
    void drawManeuverMovableSpaces();
    void finalizeSchemeCard(Card *playedCard);
    void drawEffectSelectableSpaces();
    void drawCombatEffectText();
    void drawResultRevealButton();
    void drawCombatResultPopup();

    void drawDeckEmptyPopup();
    void checkHandLimit();
    void openHandLimitPopup(Player *player);
    void drawHandLimitPopup();

    void drawSaveMenu();
    void updateSaveMenu();
    void refreshSaveFiles();

    void drawFeintBlockedPopup();

    void resetPlayerPanelAnimations();
};