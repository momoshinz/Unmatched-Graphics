#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>

class AssetManager
{
    private:
        Texture2D mainMenuBackground;
        Texture2D loadingBackground;
        Texture2D board;

        Font gameFont;

        bool loaded;

        Texture2D dracula;
        Texture2D sisters;
        Texture2D sister1;
        Texture2D sister2;
        Texture2D sister3;
        Texture2D draculaArt;
        Texture2D draculaArtTran;
        Texture2D draculaHealth;
        Texture2D draculaBackCard;
        Texture2D draculaHeroCard;

        Texture2D sherlock;
        Texture2D sherlockArt;
        Texture2D sherlockArtTran;
        Texture2D sherlockHealth;
        Texture2D sherlockBackCard;
        Texture2D sherlockHeroCard;
        Texture2D watson;
        Texture2D watsonHealth;

        Texture2D invisibleMan;
        Texture2D invisibleManArt;
        Texture2D fog;

        std::unordered_map<std::string, Texture2D> cardTextures;
        std::unordered_map<std::string, Texture2D> characterTextures;

    public:
        AssetManager();
        ~AssetManager();

        bool load();

        void unload();

        Texture2D getMainMenuBackground() const;
        Texture2D getLoadingBackground() const;
        Texture2D getBoard() const;

        Font getGameFont() const;

        Texture2D getCharacter(const std::string &name) const;
        Texture2D getCard(const std::string &name) const;
        bool isLoaded() const;
};