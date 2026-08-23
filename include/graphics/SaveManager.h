#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "raylib.h"
#include <string>
#include <vector>

class Game;

class SaveManager
{
    private:
        std::string saveDirectory;

    public:
        SaveManager(const std::string &directory = "saves");

        std::vector<std::string> getSaveFiles() const;

        bool saveExists(const std::string &saveName) const;

        std::string createNewSaveName() const;

        void saveGame(Game *game, const std::string &saveName);

        void loadGame(Game *game, const std::string &saveName);

        void deleteSave(const std::string &saveName);

        std::string getSavePath(const std::string &saveName) const;
};

#endif