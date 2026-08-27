#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

class AssetManager
{
    private:
        Texture2D mainMenuBackground;
        Texture2D loadingBackground;
        Texture2D MainPanelBackground;
        Font gameFont;
        Font titleFont;
        Font loading;
        Font guideFont;
        Texture2D fogTexture{};
        Music gameMusic;
        Texture2D winnerBackground;

        bool loaded;

        std::unordered_map<std::string, Texture2D> characterTextures;
        std::unordered_map<std::string, Texture2D> cardTextures;
        Texture2D gameMap;

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
        Texture2D getGameMap();
        Texture2D getMainPanelBackground() const;
        Texture2D getFogTexture() const;
        Music getGameMusic() const;
        Texture2D getWinnerBackground() const;

        bool isLoaded() const;
        bool isTextureValid(const Texture2D &texture) const;
};