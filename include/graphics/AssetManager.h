#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

class AssetManager
{
    private:
        Texture2D mainMenuBackground;
        Texture2D loadingBackground;
        Texture2D actionIcons;
        Texture2D rangedIcon;
        Texture2D meleeIcon;
        Texture2D MainPanelBackground;
        Font gameFont;
        Font titleFont;
        Font loading;
        Font guideFont;
        Texture2D fogTexture{};
        Music gameMusic;

        bool loaded;

        std::unordered_map<std::string, Texture2D> characterTextures;
        std::unordered_map<std::string, Texture2D> cardTextures;
        Texture2D gameMap;

        bool isTextureValid(const Texture2D &texture) const;

    public:
        AssetManager();
        ~AssetManager();

        bool load();
        void unload();

        Texture2D getMainMenuBackground() const;
        Texture2D getLoadingBackground() const;
        Font getGameFont() const;
        Font getTitleFont() const;
        Font getLoadingFont() const;
        Font getGuideFont() const;
        Texture2D getCharacter(const std::string &name) const;
        Texture2D getCard(const std::string &name) const;
        Texture2D getActionIcons() const;
        Texture2D getGameMap();
        Texture2D getMainPanelBackground() const;
        Texture2D getFogTexture() const;
        Music getGameMusic() const;

        bool isLoaded() const;
};