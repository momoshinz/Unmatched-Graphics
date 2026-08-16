#ifndef ATTACKUI_H
#define ATTACKUI_H
#include "card/Card.h"
#include "card/Deck.h"
#include <raylib.h>
#include <vector>
#include "player/Player.h"
class Fighter;
class AssetManager;

class AttackUI
{
    private:
        AssetManager *assets;

        bool open = false;

        Fighter *selectedAttacker = nullptr;
        Fighter *selectedTarget = nullptr;

        std::vector<Fighter *> selectableFighters;
        std::vector<Rectangle> fighterBoxes;
        
        enum class AttackPhase
        {
            SelectAttacker,
            SelectAttackCard
        };

        AttackPhase phase = AttackPhase::SelectAttacker;

        std::vector<Card *> selectableAttackCards;
        std::vector<Rectangle> attackCardBoxes;

        Card *selectedAttackCard = nullptr;

        Player *attackPlayer = nullptr;
        int selectedAttackCardIndex = -1;
        bool confirmedAttackCard = false;

    public:
        AttackUI(AssetManager *assets);

        void openAttack(Player *player,const std::vector<Fighter *> &fighters);

        void update();

        void draw();

        bool isOpen() const;

        Fighter *getSelectedAttacker() const;

        Fighter *getSelectedTarget() const;

        Card *getSelectedAttackCard() const;

        static Texture2D getCardTexture(AssetManager *assets, Card *card);
};

#endif