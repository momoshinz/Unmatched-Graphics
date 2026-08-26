#include "graphics/SaveManager.h"
#include "game/Game.h"
#include <filesystem>
#include <stdexcept>

using namespace std;
namespace fs = std::filesystem;

SaveManager::SaveManager(const string &directory)
    : saveDirectory(directory)
{
    if (!fs::exists(saveDirectory))
    {
        fs::create_directories(saveDirectory);
    }
}

vector<string> SaveManager::getSaveFiles() const
{
    vector<string> saveFiles;

    if (!fs::exists(saveDirectory))
        return saveFiles;

    for (const auto &entry : fs::directory_iterator(saveDirectory))
    {
        if (!entry.is_regular_file())
            continue;

        if (entry.path().extension() == ".json")
        {
            saveFiles.push_back(entry.path().filename().string());
        }
    }
    return saveFiles;
}

bool SaveManager::saveExists(const string &saveName) const
{
    return fs::exists(getSavePath(saveName));
}

string SaveManager::createNewSaveName() const
{
    int number = 1;

    while (true)
    {
        string saveName = "save" + to_string(number) + ".json";
        if (!saveExists(saveName))
        {
            return saveName;
        }
        number++;
    }
}

string SaveManager::getSavePath(const string &saveName) const
{
    return (fs::path(saveDirectory) / saveName).string();
}

void SaveManager::saveGame(Game *game, const string &saveName)
{
    if (game == nullptr)
    {
        throw runtime_error("Game pointer is null.");
    }

    string path = getSavePath(saveName);

    game->saveGame(path);
}

void SaveManager::loadGame(Game *game, const string &saveName)
{
    if (game == nullptr)
    {
        throw runtime_error("Game pointer is null.");
    }

    if (!saveExists(saveName))
    {
        throw runtime_error("Save file does not exist: " + saveName);
    }

    string path = getSavePath(saveName);
    game->loadGame(path);
}

void SaveManager::deleteSave(const string &saveName)
{
    string path = getSavePath(saveName);
    if (!saveExists(saveName))
    {
        throw runtime_error("Save file does not exist: " + saveName);
    }
    fs::remove(path);
}