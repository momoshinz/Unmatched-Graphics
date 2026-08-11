#include "graphics/AssetManager.h"
#include <iostream>

AssetManager::AssetManager()
    : mainMenuBackground{},
      loadingBackground{},
      board{},
      gameFont{},
      loaded(false)
{
}

bool AssetManager::isTextureValid(const Texture2D &texture) const
{
    return texture.id != 0;
}

bool AssetManager::load()
{
    std::cout << "Loading main menu...\n";
    mainMenuBackground =
        LoadTexture("Unmatched_Assets/main_menu.png");

    std::cout << "mainMenu ID: "
              << mainMenuBackground.id << "\n";
/*
    std::cout << "Loading loading screen...\n";
    loadingBackground =
        LoadTexture("Unmatched_Assets/loading.jpg");

    std::cout << "loading ID: "
              << loadingBackground.id << "\n";

    std::cout << "Loading board...\n";
    board =
        LoadTexture("Unmatched_Assets/board.jpg");

    std::cout << "board ID: "
              << board.id << "\n";

    if (mainMenuBackground.id == 0)
    {
        std::cout << "[ERROR] mainMenuBackground FAILED!\n";
        return false;
    }*/
/*
    if (loadingBackground.id == 0)
    {
        std::cout << "[ERROR] loadingBackground FAILED!\n";
        return false;
    }

    if (board.id == 0)
    {
        std::cout << "[ERROR] board FAILED!\n";
        return false;
    }*/

    loaded = true;

    std::cout << "ALL BASIC ASSETS LOADED SUCCESSFULLY!\n";

    return true;
}

/*
bool AssetManager::load()
{
    std::cout << "Loading main menu...\n";

    mainMenuBackground =
        LoadTexture("Unmatched_Assets/main_menu.png");

    std::cout << "Loading loading screen...\n";

    loadingBackground =
        LoadTexture("Unmatched_Assets/loading.png");

    std::cout << "Loading board...\n";

    board =
        LoadTexture("Unmatched_Assets/board.png");

    std::cout << "Loading font...\n";

    gameFont =
        LoadFont("Unmatched_Assets/fonts/game_font.ttf");

    std::cout << "Basic assets loaded.\n";

    // فعلاً بقیه‌ی character و card ها را کامنت کن

    if (mainMenuBackground.id == 0 ||
        loadingBackground.id == 0 ||
        board.id == 0 ||
        gameFont.texture.id == 0)
    {
        std::cout << "One of the basic assets FAILED!\n";
        return false;
    }

    loaded = true;

    std::cout << "AssetManager::load() SUCCESS!\n";

    return true;


    mainMenuBackground = LoadTexture("Unmatched_Assets/main_menu.png");
    loadingBackground = LoadTexture("Unmatched_Assets/loading.png");
    board = LoadTexture("Unmatched_Assets/board.png");
    gameFont = LoadFont("Unmatched_Assets/fonts/game_font.ttf");

    characterTextures["dracula"] = LoadTexture("Unmatched_Assets/dracula/dracula.png");
    characterTextures["dracula_art"] = LoadTexture("Unmatched_Assets/dracula/DracArt.png");
    characterTextures["dracula_art_transparent"] = LoadTexture("Unmatched_Assets/dracula/DracArtTran.png");
    characterTextures["dracula_health"] = LoadTexture("Unmatched_Assets/dracula/draculaHealth.png");
    characterTextures["dracula_backcard"] = LoadTexture("Unmatched_Assets/dracula/draculaBackCard.png");
    characterTextures["sisters"] = LoadTexture("Unmatched_Assets/dracula/3sisters.png");
    characterTextures["sherlock"] = LoadTexture("Unmatched_Assets/sherlock/sherlockTran.png");
    characterTextures["sherlock_art"] = LoadTexture("Unmatched_Assets/sherlock/holmsArt.png");
    characterTextures["sherlock_transparent"] = LoadTexture("Unmatched_Assets/sherlock/holmsArtTransparent.png");
    characterTextures["sherlock_health"] = LoadTexture("Unmatched_Assets/sherlock/sherlockHealth.png");
    characterTextures["sherlock_card"] = LoadTexture("Unmatched_Assets/sherlock/sherlockHerocard.png");
    characterTextures["watson"] = LoadTexture("Unmatched_Assets/sherlock/drwatson.png");
    characterTextures["watson_health"] = LoadTexture("Unmatched_Assets/sherlock/watsonHealth.png");
    characterTextures["sherlock_backcard"] = LoadTexture("Unmatched_Assets/sherlock/holmsBackCard.png");
    characterTextures["invisible_man"] = LoadTexture("Unmatched_Assets/invisibleMan/invArt.png");
    characterTextures["invisible_man_transparent"] = LoadTexture("Unmatched_Assets/invisibleMan/tranInv.png");
    characterTextures["fog"] = LoadTexture("Unmatched_Assets/invisibleMan/fog.png");

    cardTextures["BeastForm"] = LoadTexture("Unmatched_Assets/cards/dracula/Beastform.png");
    cardTextures["Ambush"] = LoadTexture("Unmatched_Assets/cards/dracula/ambush.png");
    cardTextures["BaptismOfBlood"] = LoadTexture("Unmatched_Assets/cards/dracula/baptism-of-blood.png");
    cardTextures["Dash"] = LoadTexture("Unmatched_Assets/cards/dracula/dash.png");
    cardTextures["Exploit"] = LoadTexture("Unmatched_Assets/cards/dracula/exploit.png");
    cardTextures["FeedingFrenzy"] = LoadTexture("Unmatched_Assets/cards/dracula/feeding-frenzy.png");
    cardTextures["FeintDracula"] = LoadTexture("Unmatched_Assets/cards/dracula/feint (1).png");
    cardTextures["LookIntoMyEyes"] = LoadTexture("Unmatched_Assets/cards/dracula/look-into-my-eyes.png");
    cardTextures["MistForm"] = LoadTexture("Unmatched_Assets/cards/dracula/mistform.png");
    cardTextures["PreyUpon"] = LoadTexture("Unmatched_Assets/cards/dracula/prey-upon.png");
    cardTextures["RaveningSeduction"] = LoadTexture("Unmatched_Assets/cards/dracula/ravening-seduction.png");
    cardTextures["ThirstForSustenance"] = LoadTexture("Unmatched_Assets/cards/dracula/thirst-for-sustenance.png");
    cardTextures["AdministerAid"] = LoadTexture("Unmatched_Assets/cards/sherlock/administer-aid.png");
    cardTextures["CounterPunch"] = LoadTexture("Unmatched_Assets/cards/sherlock/counterpunch.png");
    cardTextures["DeduceStrategy"] = LoadTexture("Unmatched_Assets/cards/sherlock/deduce-strategy.png");
    cardTextures["EducationNeverEnds"] = LoadTexture("Unmatched_Assets/cards/sherlock/education-never-ends.png");
    cardTextures["EliminateTheImpossible"] = LoadTexture("Unmatched_Assets/cards/sherlock/eliminate-the-impossible.png");
    cardTextures["FeintSherlock"] = LoadTexture("Unmatched_Assets/cards/sherlock/feint (2).png");
    cardTextures["FixedPoint"] = LoadTexture("Unmatched_Assets/cards/sherlock/fixed-point-in-a-changing-aga.png");
    cardTextures["MasterOfDisguise"] = LoadTexture("Unmatched_Assets/cards/sherlock/master-of-disguise.png");
    cardTextures["StudyMethods"] = LoadTexture("Unmatched_Assets/cards/sherlock/study-methods.png");
    cardTextures["TheGameIsAfoot"] = LoadTexture("Unmatched_Assets/cards/sherlock/the-game-is-afoot.png");
    cardTextures["ServiceRevolver"] = LoadTexture("Unmatched_Assets/cards/sherlock/service-revolver.png");
    cardTextures["CodedNotes"] = LoadTexture("Unmatched_Assets/cards/invisibleMan/coded-notes.png");
    cardTextures["DreamingOfRevange"] = LoadTexture("Unmatched_Assets/cards/invisibleMan/dreaming-of-revange.png");
    cardTextures["EmergeFromMist"] = LoadTexture("Unmatched_Assets/cards/invisibleMan/emerge-from-mist.png");
    cardTextures["ImpossibleToSee"] = LoadTexture("Unmatched_Assets/cards/invisibleMan/impossible-to-see.png");
    cardTextures["IntoThinAir"] = LoadTexture("Unmatched_Assets/cards/invisibleMan/into-thin-air.png");
    cardTextures["Lurking"] = LoadTexture("Unmatched_Assets/cards/invisibleMan/lurking.png");
    cardTextures["ReignOfTerror"] = LoadTexture("Unmatched_Assets/cards/invisibleMan/reign-of-terror.png");
    cardTextures["RollingFog"] = LoadTexture("Unmatched_Assets/cards/invisibleMan/rolling-fog.png");
    cardTextures["SlipAway"] = LoadTexture("Unmatched_Assets/cards/invisibleMan/slip-away.png");
    cardTextures["StepLightly"] = LoadTexture("Unmatched_Assets/cards/invisibleMan/step-lightly.png");

    if (!isTextureValid(mainMenuBackground) || !isTextureValid(loadingBackground) || !isTextureValid(board) ||
        gameFont.texture.id == 0)
    {
        unload();
        return false;
    }

    loaded = true;
    return true;
}*/

void AssetManager::unload()
{
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

    if (board.id != 0)
    {
        UnloadTexture(board);
        board = {};
    }

    if (gameFont.texture.id != 0)
    {
        UnloadFont(gameFont);
        gameFont = {};
    }

    for (auto &[name, texture] : characterTextures)
    {
        if (texture.id != 0)
        {
            UnloadTexture(texture);
        }
    }
    characterTextures.clear();

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

Texture2D AssetManager::getBoard() const
{
    return board;
}

Font AssetManager::getGameFont() const
{
    return gameFont;
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

bool AssetManager::isLoaded() const
{
    return loaded;
}