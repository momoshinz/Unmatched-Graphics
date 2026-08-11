#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

class AssetManager
{
    private:
        Texture2D mainMenuBackground;
        Texture2D loadingBackground;
        Texture2D board;
        Texture2D actionIcons;
        Texture2D rangedIcon;
        Texture2D meleeIcon;
        Font gameFont;

        bool loaded;

        std::unordered_map<std::string, Texture2D> characterTextures;
        std::unordered_map<std::string, Texture2D> cardTextures;

        bool isTextureValid(const Texture2D &texture) const;

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
        Texture2D getActionIcons() const;

        bool isLoaded() const;
};