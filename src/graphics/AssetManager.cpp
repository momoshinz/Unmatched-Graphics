#include "graphics/AssetManager.h"

AssetManager::AssetManager()
    : mainMenuBackground{},
      loadingBackground{},
      board{},
      gameFont{},
      loaded(false)
{
}

bool AssetManager::load()
{
    mainMenuBackground = LoadTexture("Unmatched_Assets/main_menu.png");

    loadingBackground = LoadTexture("Unmatched_Assets/loading.png");

    board = LoadTexture("Unmatched_Assets/board.jpeg");

    gameFont = LoadFont("Unmatched_Assets/fonts/game_font.ttf");

    characterTextures["dracula"] = LoadTexture("Unmatched_Assets/dracula/dracula.png");

    characterTextures["draculaHeroCard"] = LoadTexture("Unmatched_Assets/dracula/dracHero.svg");

    characterTextures["dracula_art"] = LoadTexture("Unmatched_Assets/dracula/DracArt.png");

    characterTextures["dracula_art_transparent"] = LoadTexture("Unmatched_Assets/dracula/DracArtTran.png");

    characterTextures["dracula_health"] = LoadTexture("Unmatched_Assets/dracula/draculaHealth.png");

    characterTextures["dracula_backcard"] = LoadTexture("Unmatched_Assets/dracula/draculaBackCard.png");

    characterTextures["sisters"] = LoadTexture("Unmatched_Assets/dracula/3sisters.png");

    characterTextures["sister1"] = LoadTexture("Unmatched_Assets/dracula/sis1.svg");

    characterTextures["sister2"] = LoadTexture("Unmatched_Assets/dracula/sis2.svg");

    characterTextures["sister3"] = LoadTexture("Unmatched_Assets/dracula/sis3.svg");



    characterTextures["sherlock"] = LoadTexture("Unmatched_Assets/sherlock/sherlockTran.png");

    characterTextures["sherlock_art"] = LoadTexture("Unmatched_Assets/sherlock/holmsArt.png");

    characterTextures["sherlock_transparent"] = LoadTexture("Unmatched_Assets/sherlock/holmsArtTransparent.png");

    characterTextures["sherlock_health"] = LoadTexture("Unmatched_Assets/sherlock/sherlockHealth.png");

    characterTextures["sherlock_card"] = LoadTexture("Unmatched_Assets/sherlock/sherlockHerocard.png");

    characterTextures["watson"] = LoadTexture("Unmatched_Assets/sherlock/drwatson.png");

    characterTextures["watson_health"] = LoadTexture("Unmatched_Assets/sherlock/watsonHealth.png");

    characterTextures["sherlock_backcard"] = LoadTexture("Unmatched_Assets/sherlock/holmsBackCard.png");



    characterTextures["invisibleMan"] = LoadTexture("Unmatched_Assets/invisibleMan/invArt.png");

    characterTextures["invisible_man_transparent"] = LoadTexture("Unmatched_Assets/invisibleMan/tranInv.png");

    characterTextures["fog"] = LoadTexture("Unmatched_Assets/invisibleMan/fog.png");


    cardTextures["BeastForm"] = LoadTexture("Unmatched_Assets/cards/dracula/BeastForm.png");

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


    cardTextures["AdministerAid"] = LoadTexture("Unmatched_Assets/cards/dracula/administer-aid.png");

    cardTextures["CounterPunch"] = LoadTexture("Unmatched_Assets/cards/dracula/counterpunch.png");

    cardTextures["DeduceStrategy"] = LoadTexture("Unmatched_Assets/cards/dracula/deduce-strategy.png");

    cardTextures["EducationNeverEnds"] = LoadTexture("Unmatched_Assets/cards/dracula/education-never-ends.png");

    cardTextures["EliminateTheImpossible"] = LoadTexture("Unmatched_Assets/cards/dracula/eliminate-the-impossible.png");

    cardTextures["FeintSherlock"] = LoadTexture("Unmatched_Assets/cards/dracula/feint (2).png");

    cardTextures["FixedPoint"] = LoadTexture("Unmatched_Assets/cards/dracula/fixed-point-in-a-changing-aga.png");

    cardTextures["MasterOfDisguise"] = LoadTexture("Unmatched_Assets/cards/dracula/master-of-disguise.png");

    cardTextures["StudyMethods"] = LoadTexture("Unmatched_Assets/cards/dracula/study-methods.png");

    cardTextures["TheGameIsAfoot"] = LoadTexture("Unmatched_Assets/cards/dracula/the-game-is-afoot.png");


    cardTextures["CodedNotes"] = LoadTexture("Unmatched_Assets/cards/dracula/coded-notes.png");

    cardTextures["DreamingOfRevange"] = LoadTexture("Unmatched_Assets/cards/dracula/dreaming-of-revange.png");

    cardTextures["EmergeFromMist"] = LoadTexture("Unmatched_Assets/cards/dracula/emerge-from-mist.png");

    cardTextures["ImpossibleToSee"] = LoadTexture("Unmatched_Assets/cards/dracula/impossible-to-see.png");

    cardTextures["IntoThinAir"] = LoadTexture("Unmatched_Assets/cards/dracula/into-thin-air.png");

    cardTextures["Lurking"] = LoadTexture("Unmatched_Assets/cards/dracula/lurking.png");

    cardTextures["ReignOfTerror"] = LoadTexture("Unmatched_Assets/cards/dracula/reign-of-terror.png");

    cardTextures["RollingFog"] = LoadTexture("Unmatched_Assets/cards/dracula/rolling-fog.png");

    cardTextures["SlipAway"] = LoadTexture("Unmatched_Assets/cards/dracula/slip-away.png");

    cardTextures["StepLightly"] = LoadTexture("Unmatched_Assets/cards/dracula/step-lightly.png");


    if (mainMenuBackground.id == 0 || loadingBackground.id == 0 || board.id == 0 || gameFont.texture.id == 0)
    {
        unload();
        return false;
    }

    loaded = true;
    return true;
}

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

