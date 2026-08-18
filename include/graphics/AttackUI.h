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
    Game *game = nullptr;
    bool open = false;

    Fighter *selectedAttacker = nullptr;
    Fighter *selectedTarget = nullptr;

    std::vector<Fighter *> selectableFighters;
    std::vector<Rectangle> fighterBoxes;

    enum class AttackPhase
    {
        SelectAttacker,
        SelectAttackCard,
        AskDefenseCard,
        SelectTarget,
        SelectDefenseCard
    };

    AttackPhase phase = AttackPhase::SelectAttacker;

    std::vector<Card *> selectableAttackCards;
    std::vector<Rectangle> attackCardBoxes;

    Card *selectedAttackCard = nullptr;

    Player *attackPlayer = nullptr;
    int selectedAttackCardIndex = -1;
    bool confirmedAttackCard = false;

    std::vector<Fighter *> selectableTargets;
    std::vector<Rectangle> targetBoxes;

    Card *selectedDefenseCard = nullptr;

    std::vector<Card *> selectableDefenseCards;
    std::vector<Rectangle> defenseCardBoxes;

    bool defenseChoiceMade = false;
    bool wantsDefenseCard = false;
    Rectangle playButton;

    bool showError = false;
    std::string errorMessage;

    bool attackConfirmed = false;
    bool errorIsDefenseless = false;

public:
    AttackUI(AssetManager *assets);
    void openAttack(Player *player, const std::vector<Fighter *> &fighters, Game *game);
    void update();
    void draw();
    bool isOpen() const;

    Fighter *getSelectedAttacker() const;
    Fighter *getSelectedTarget() const;
    Card *getSelectedAttackCard() const;
    static Texture2D getCardTexture(AssetManager *assets, Card *card);
    Card *getSelectedDefenseCard() const;
    bool isAttackCardConfirmed() const;

    bool isAttackConfirmed() const;
    void resetAttackConfirmed();
};

#endif