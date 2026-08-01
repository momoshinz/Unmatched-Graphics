#ifndef CARD_H
#define CARD_H
#include <vector>
#include <string>
#include "utils/Types.h"
#include "effect/Effect.h"

using namespace std;

class Fighter;
class Game;

class Card
{
protected:
    string name;
    CardType cardType;
    OwnerType ownerType;
    Timing timing;
    int boost;
    int deckCount;
    int combatValue;
    Effect *effect;
    bool effectsCanceled = false;

public:
    Card(const string &name,
         CardType cardType,
         OwnerType ownerType,
         Timing timing,
         int boost,
         int deckCount,
         int combatValue,
         Effect *effect);

    Card(const Card &other);
    virtual ~Card();

    string getName() const;
    CardType getCardType() const;
    OwnerType getOwnerType() const;
    int getAmount() const;
    int getCombatValue() const;
    int getBoost() const;
    Timing getTiming() const;
    bool hasTiming() const;
    Effect *getEffect() const;

    bool isAttack() const;
    bool isDefense() const;
    bool isVersatile() const;
    bool isScheme() const;
    bool isPlayableBy(const Fighter &fighter) const;

    virtual void play(Fighter &fighter, Fighter &target, Game &game) const;
    bool canBePlayedBy(const Fighter &fighter, CardType requiredType) const;
    bool hasEffect() const;

    void cancelEffects();
    bool isEffectsCanceled() const;
    void resetEffectsCancel();

    void display() const;
    void printWrapped(const string &text, int width) const;

    static Card createFeedingFrenzy();
    static Card createBeastform();
    static Card createAmbush();
    static Card createThirstForSustenance();
    static Card createLookIntoMyEyes();
    static Card createDash();
    static Card createExploit();
    static Card createFeintDracula();
    static Card createMistForm();
    static Card createBaptismOfBlood();
    static Card createPreyUpon();
    static Card createRaveningSeduction();

    static Card createTheGameIsAfoot();
    static Card createServiceRevolver();
    static Card createCounterPunch();
    static Card createDeduceStrategy();
    static Card createEducationNeverEnds();
    static Card createFixedPoint();
    static Card createStudyMethods();
    static Card createFeintSherlock();
    static Card createAdministerAid();
    static Card createEliminateTheImpossible();
    static Card createMasterOfDisguise();
};
#endif