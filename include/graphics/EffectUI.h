#pragma once
#include "raylib.h"
#include "effect/Effect.h"
#include <vector>
#include <string>

class AssetManager;
class Game;
class Fighter;
class Card;
class Space;
class Player;

class EffectUI
{
public:
    explicit EffectUI(AssetManager *assets);

    // فرآیند رو با کارت/فایتر/بازی مشخص شروع می‌کنه.
    // اگه effect->getInputKind() == None باشه، isReady() فوراً true می‌شه.
    void open(Game *game, Effect *effect, Fighter *fighter, Fighter *target);

    void update();
    void draw();

    bool isOpen() const;
    bool isReady() const; // یعنی انتخاب کاربر کامل شده و آماده‌ی apply()
    const EffectChoice &getChoice() const;

    void reset();

private:
    AssetManager *assets;
    Game *game = nullptr;
    Effect *effect = nullptr;
    Fighter *fighter = nullptr;
    Fighter *target = nullptr;

    EffectInputKind inputKind = EffectInputKind::None;
    EffectChoice choice;

    bool open_ = false;
    bool ready = false;

    // ------------------------------------------------------------
    // برای ChooseAdjacentEmptySpace / ChooseReachableSpace
    // (دایره‌های قابل‌کلیک روی نقشه - GameScreen این‌ها رو می‌خونه و می‌کشه)
    // ------------------------------------------------------------
    std::vector<Space *> candidateSpaces;

    // ------------------------------------------------------------
    // برای ChooseOpponentCardToBurn (پنجره‌ی کارت‌ها، شبیه SchemeUI)
    // ------------------------------------------------------------
    std::vector<Card *> candidateCards;
    std::vector<Rectangle> cardBoxes;
    int selectedCardIndex = -1;
    Rectangle confirmButton{};

    // ------------------------------------------------------------
    // برای ChooseEnemyFighter (کادرهای اسم، شبیه ManeuverUI::SELECT_FIGHTER)
    // ------------------------------------------------------------
    std::vector<Fighter *> candidateFighters;
    std::vector<Rectangle> fighterBoxes;

    void setupChooseAdjacentEmptySpace();
    void setupChooseReachableSpace();
    void setupChooseOpponentCardToBurn();
    void setupChooseEnemyFighter();

    void layoutCardWindow();
    void layoutFighterWindow();

    static std::string getCardTextureKey(const Card *card, const std::string &heroName);

public:
    // GameScreen برای رسم دایره‌های روی نقشه به این لیست نیاز داره
    const std::vector<Space *> &getCandidateSpaces() const;
    bool isChoosingSpace() const;
    void selectSpace(Space *space);
};