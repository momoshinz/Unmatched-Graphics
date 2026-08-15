#include "graphics/AssetManager.h"
#include <iostream>

using namespace std;

AssetManager::AssetManager()
    : mainMenuBackground{},
      loadingBackground{},
      gameMap{},
      MainPanelBackground{},
      gameFont{},
      titleFont{},
      loading{},
      loaded(false)
{
}

bool AssetManager::isTextureValid(const Texture2D &texture) const
{
    return texture.id != 0;
}

bool AssetManager::load()
{
    cout << "Loading assets...\n";

    // =========================================
    // Backgrounds
    // =========================================

    mainMenuBackground =
        LoadTexture("Unmatched_Assets/main_menu.png");

    loadingBackground =
        LoadTexture("Unmatched_Assets/loading.png");

    gameMap =
        LoadTexture("Unmatched_Assets/board.png");

    MainPanelBackground =
        LoadTexture("Unmatched_Assets/mainPanel.png");

    // =========================================
    // Fonts
    // =========================================

    gameFont =
        LoadFont("Unmatched_Assets/fonts/Sweet Magic.ttf");

    titleFont =
        LoadFont("Unmatched_Assets/fonts/title.ttf");

    loading =
        LoadFont("Unmatched_Assets/fonts/NexaRustSlab.ttf");

    guideFont = LoadFont("Unmatched_Assets/fonts/guide.TTF");

    // =========================================
    // Dracula
    // =========================================

    characterTextures["dracula"] =
        LoadTexture("Unmatched_Assets/dracula/DracArtTran.png");

    characterTextures["dracula_art"] =
        LoadTexture("Unmatched_Assets/dracula/DracArt.png");

    characterTextures["dracula_art_transparent"] =
        LoadTexture("Unmatched_Assets/dracula/DracArtTran.png");

    characterTextures["dracula_health"] =
        LoadTexture("Unmatched_Assets/dracula/draculaHealth.png");

    characterTextures["dracula_backcard"] =
        LoadTexture("Unmatched_Assets/dracula/draculaBackCard.png");

    characterTextures["dracula-herocard"] =
        LoadTexture("Unmatched_Assets/dracula/dracHero.png");

    characterTextures["sisters"] =
        LoadTexture("Unmatched_Assets/dracula/3sisters.png");

    characterTextures["sister1"] =
        LoadTexture("Unmatched_Assets/dracula/sis1.png");

    characterTextures["sister2"] =
        LoadTexture("Unmatched_Assets/dracula/sis2.png");

    characterTextures["sister3"] =
        LoadTexture("Unmatched_Assets/dracula/sis3.png");

    // =========================================
    // Sherlock
    // =========================================

    characterTextures["sherlock"] =
        LoadTexture("Unmatched_Assets/sherlock/holmsArtTransparent.png");

    characterTextures["sherlock_art"] =
        LoadTexture("Unmatched_Assets/sherlock/holmsArt.png");

    characterTextures["sherlock_transparent"] =
        LoadTexture("Unmatched_Assets/sherlock/holmsArtTransparent.png");

    characterTextures["sherlock_health"] =
        LoadTexture("Unmatched_Assets/sherlock/sherlockHealth.png");

    characterTextures["sherlock_backcard"] =
        LoadTexture("Unmatched_Assets/sherlock/holmsBackCard.png");

    characterTextures["sherlock_card"] =
        LoadTexture("Unmatched_Assets/sherlock/sherlockHerocard.png");

    characterTextures["watson"] =
        LoadTexture("Unmatched_Assets/sherlock/drwatson.png");

    characterTextures["drwatson"] =
        LoadTexture("Unmatched_Assets/sherlock/watson.png");

    characterTextures["watson_health"] =
        LoadTexture("Unmatched_Assets/sherlock/watsonHealth.png");

    // =========================================
    // Invisible Man
    // =========================================

    characterTextures["invisible_man"] =
        LoadTexture("Unmatched_Assets/invisibleMan/invisibleManArT.png");

    characterTextures["invisible_man_transparent"] =
        LoadTexture("Unmatched_Assets/invisibleMan/tranInv.png");

    characterTextures["foggy"] =
        LoadTexture("Unmatched_Assets/invisibleMan/Fogs.png");

    fogTexture = LoadTexture("Unmatched_Assets/invisibleMan/fog.png");

    // =========================================
    // Dracula cards
    // =========================================

    cardTextures["BeastForm"] =
        LoadTexture("Unmatched_Assets/cards/dracula/Beastform.png");

    cardTextures["Ambush"] =
        LoadTexture("Unmatched_Assets/cards/dracula/ambush.png");

    cardTextures["BaptismOfBlood"] =
        LoadTexture("Unmatched_Assets/cards/dracula/baptism-of-blood.png");

    cardTextures["Dash"] =
        LoadTexture("Unmatched_Assets/cards/dracula/dash.png");

    cardTextures["Exploit"] =
        LoadTexture("Unmatched_Assets/cards/dracula/exploit.png");

    cardTextures["FeedingFrenzy"] =
        LoadTexture("Unmatched_Assets/cards/dracula/feeding-frenzy.png");

    cardTextures["FeintDracula"] =
        LoadTexture("Unmatched_Assets/cards/dracula/feint (1).png");

    cardTextures["LookIntoMyEyes"] =
        LoadTexture("Unmatched_Assets/cards/dracula/look-into-my-eyes.png");

    cardTextures["MistForm"] =
        LoadTexture("Unmatched_Assets/cards/dracula/mistform.png");

    cardTextures["PreyUpon"] =
        LoadTexture("Unmatched_Assets/cards/dracula/prey-upon.png");

    cardTextures["RaveningSeduction"] =
        LoadTexture("Unmatched_Assets/cards/dracula/ravening-seduction.png");

    cardTextures["ThirstForSustenance"] =
        LoadTexture("Unmatched_Assets/cards/dracula/thirst-for-sustenance.png");

    // =========================================
    // Sherlock cards
    // =========================================
    cardTextures["AdministerAid"] =
        LoadTexture("Unmatched_Assets/cards/sherlock/administer-aid.png");

    cardTextures["CounterPunch"] =
        LoadTexture("Unmatched_Assets/cards/sherlock/counterpunch.png");

    cardTextures["DeduceStrategy"] =
        LoadTexture("Unmatched_Assets/cards/sherlock/deduce-strategy.png");

    cardTextures["EducationNeverEnds"] =
        LoadTexture("Unmatched_Assets/cards/sherlock/education-never-ends.png");

    cardTextures["EliminateTheImpossible"] =
        LoadTexture("Unmatched_Assets/cards/sherlock/eliminate-the-impossible.png");

    cardTextures["FeintSherlock"] =
        LoadTexture("Unmatched_Assets/cards/sherlock/feint (2).png");

    cardTextures["FixedPoint"] =
        LoadTexture("Unmatched_Assets/cards/sherlock/fixed-point-in-a-changing-aga.png");

    cardTextures["MasterOfDisguise"] =
        LoadTexture("Unmatched_Assets/cards/sherlock/master-of-disguise.png");

    cardTextures["StudyMethods"] =
        LoadTexture("Unmatched_Assets/cards/sherlock/study-methods.png");

    cardTextures["TheGameIsAfoot"] =
        LoadTexture("Unmatched_Assets/cards/sherlock/the-game-is-afoot.png");

    cardTextures["ServiceRevolver"] =
        LoadTexture("Unmatched_Assets/cards/sherlock/service-revolver.png");

    // =========================================
    // Invisible Man cards
    // =========================================

    cardTextures["CodedNotes"] =
        LoadTexture("Unmatched_Assets/cards/invisibleMan/coded-notes.png");

    cardTextures["DreamingOfRevange"] =
        LoadTexture("Unmatched_Assets/cards/invisibleMan/dreaming-of-revange.png");

    cardTextures["EmergeFromMist"] =
        LoadTexture("Unmatched_Assets/cards/invisibleMan/emerge-from-mist.png");

    cardTextures["ImpossibleToSee"] =
        LoadTexture("Unmatched_Assets/cards/invisibleMan/impossible-to-see.png");

    cardTextures["IntoThinAir"] =
        LoadTexture("Unmatched_Assets/cards/invisibleMan/into-thin-air.png");

    cardTextures["Lurking"] =
        LoadTexture("Unmatched_Assets/cards/invisibleMan/lurking.png");

    cardTextures["ReignOfTerror"] =
        LoadTexture("Unmatched_Assets/cards/invisibleMan/reign-of-terror.png");

    cardTextures["RollingFog"] =
        LoadTexture("Unmatched_Assets/cards/invisibleMan/rolling-fog.png");

    cardTextures["SlipAway"] =
        LoadTexture("Unmatched_Assets/cards/invisibleMan/slip-away.png");

    cardTextures["StepLightly"] =
        LoadTexture("Unmatched_Assets/cards/invisibleMan/step-lightly.png");

    // =========================================
    // Icons
    // =========================================

    actionIcons =
        LoadTexture("Unmatched_Assets/actions.png");

    rangedIcon =
        LoadTexture("Unmatched_Assets/ranged.png");

    meleeIcon =
        LoadTexture("Unmatched_Assets/melee.png");

    // =========================================
    // REQUIRED ASSETS
    // =========================================

    if (!isTextureValid(mainMenuBackground))
    {
        cout << "[ERROR] main_menu.png failed to load.\n";
        unload();
        return false;
    }

    if (!isTextureValid(loadingBackground))
    {
        cout << "[ERROR] loading.png failed to load.\n";
        unload();
        return false;
    }

    if (!isTextureValid(gameMap))
    {
        cout << "[ERROR] board.png failed to load.\n";
        unload();
        return false;
    }

    if (gameFont.texture.id == 0)
    {
        cout << "[ERROR] Sweet Magic.ttf failed to load.\n";
        unload();
        return false;
    }

    // =========================================
    // Success
    // =========================================

    loaded = true;

    cout << "All required assets loaded successfully.\n";

    return true;
}

void AssetManager::unload()
{
    // =========================================
    // Backgrounds
    // =========================================

    if (mainMenuBackground.id != 0)
    {
        UnloadTexture(mainMenuBackground);
        mainMenuBackground = {};
    }

    if (loadingBackground.id != 0)
    {
        UnloadTexture(loadingBackground);
        loadingBackground = {};
    }

    // =========================================
    // Game Map
    // =========================================

    if (gameMap.id != 0)
    {
        UnloadTexture(gameMap);
        gameMap = {};
    }

    if (MainPanelBackground.id != 0)
    {
        UnloadTexture(MainPanelBackground);
        MainPanelBackground = {};
    }

    if (gameFont.texture.id != 0)
    {
        UnloadFont(gameFont);
        gameFont = {};
    }

    if (titleFont.texture.id != 0)
    {
        UnloadFont(titleFont);
        titleFont = {};
    }

    if (loading.texture.id != 0)
    {
        UnloadFont(loading);
        loading = {};
    }

    if (fogTexture.id != 0)
    {
        UnloadTexture(fogTexture);
        fogTexture = {};
    }

    // =========================================
    // Icons
    // =========================================

    if (actionIcons.id != 0)
    {
        UnloadTexture(actionIcons);
        actionIcons = {};
    }

    if (rangedIcon.id != 0)
    {
        UnloadTexture(rangedIcon);
        rangedIcon = {};
    }

    if (meleeIcon.id != 0)
    {
        UnloadTexture(meleeIcon);
        meleeIcon = {};
    }

    // =========================================
    // Character textures
    // =========================================

    for (auto &[name, texture] : characterTextures)
    {
        if (texture.id != 0)
        {
            UnloadTexture(texture);
        }
    }

    characterTextures.clear();

    // =========================================
    // Card textures
    // =========================================

    for (auto &[name, texture] : cardTextures)
    {
        if (texture.id != 0)
        {
            UnloadTexture(texture);
        }
    }

    cardTextures.clear();

    loaded = false;
}

AssetManager::~AssetManager()
{
    unload();
}

Texture2D AssetManager::getMainMenuBackground() const
{
    return mainMenuBackground;
}

Texture2D AssetManager::getLoadingBackground() const
{
    return loadingBackground;
}

Texture2D AssetManager::getGameMap()
{
    return gameMap;
}

Font AssetManager::getGameFont() const
{
    return gameFont;
}

Font AssetManager::getTitleFont() const
{
    return titleFont;
}

Font AssetManager::getLoadingFont() const
{
    return loading;
}

Font AssetManager::getGuideFont() const
{
    return guideFont;
}

Texture2D AssetManager::getCharacter(const std::string &name) const
{
    auto it = characterTextures.find(name);

    if (it == characterTextures.end())
    {
        return {};
    }

    return it->second;
}

Texture2D AssetManager::getCard(const std::string &name) const
{
    auto it = cardTextures.find(name);

    if (it == cardTextures.end())
    {
        return {};
    }

    return it->second;
}

Texture2D AssetManager::getActionIcons() const
{
    return actionIcons;
}

bool AssetManager::isLoaded() const
{
    return loaded;
}

Texture2D AssetManager::getMainPanelBackground() const
{
    return MainPanelBackground;
}

Texture2D AssetManager::getFogTexture() const
{
    return fogTexture;
}