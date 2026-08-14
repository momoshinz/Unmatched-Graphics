#ifndef ATTACKUI_H
#define ATTACKUI_H

#include <raylib.h>
#include <vector>

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

    public:
        AttackUI(AssetManager *assets);

        void openAttack(const std::vector<Fighter *> &fighters);

        void update();

        void draw();

        bool isOpen() const;

        Fighter *getSelectedAttacker() const;

        Fighter *getSelectedTarget() const;
};

#endif