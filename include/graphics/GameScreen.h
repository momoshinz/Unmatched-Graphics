#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include <raylib.h>
#include "graphics/AssetManager.h"
#include "game/Game.h"
#include "graphics/PlayerPanel.h"

class GameScreen
{
    private:
        AssetManager *assets;
        Game *game;

        bool guideOpen = false;

        void drawMap();
        void drawPlayerPanels();
        void drawTopButtons();
        void drawGuidePopup();
        void drawSpaces();
        void drawPlacedFighters();

        Vector2 mapImageToScreen(Vector2 imagePosition) const;
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


    public:
        GameScreen(
            AssetManager *assets,
            Game *game);
            
            
        int update();
        void draw();

        void calculateMapTransform(
            float &mapX,
            float &mapY,
            float &scale,
            float &mapWidth,
            float &mapHeight) const;

        int getClickedSpaceId();
        void drawFogs();
        void drawActionButtons();
        Fighter* getClickedFighter();
};

#endif