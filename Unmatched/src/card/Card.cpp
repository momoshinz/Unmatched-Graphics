#include "card/Card.h"
#include "fighter/Fighter.h"
#include "game/Game.h"
#include "player/Player.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include "effect/DraculaCards/Ambush.h"
#include "effect/DraculaCards/BaptismOfBlood.h"
#include "effect/DraculaCards/Beastform.h"
#include "effect/DraculaCards/Dash.h"
#include "effect/DraculaCards/Exploit.h"
#include "effect/DraculaCards/FeedingFrenzy.h"
#include "effect/DraculaCards/FeintDracula.h"
#include "effect/DraculaCards/LookIntoMyEyes.h"
#include "effect/DraculaCards/MistForm.h"
#include "effect/DraculaCards/PreyUpon.h"
#include "effect/DraculaCards/RaveningSeduction.h"
#include "effect/DraculaCards/ThirstForSustenance.h"
#include "effect/SherlockCards/AdministerAid.h"
#include "effect/SherlockCards/CounterPunch.h"
#include "effect/SherlockCards/DeduceStrategy.h"
#include "effect/SherlockCards/EducationNeverEnds.h"
#include "effect/SherlockCards/EliminateTheImpossible.h"
#include "effect/SherlockCards/FeintSherlock.h"
#include "effect/SherlockCards/FixedPoint.h"
#include "effect/SherlockCards/MasterOfDisguise.h"
#include "effect/SherlockCards/StudyMethods.h"
#include "effect/SherlockCards/TheGameIsAfoot.h"
using namespace std;

Card::Card(const string &name, CardType cardType, OwnerType ownerType, Timing timing,
           int boost, int deckCount, int combatValue, Effect *effect)
    : name(name), cardType(cardType), ownerType(ownerType), timing(timing),
      boost(boost), deckCount(deckCount), combatValue(combatValue), effect(effect)
{
}

Card::Card(const Card &other)
    : name(other.name),
      cardType(other.cardType),
      ownerType(other.ownerType),
      timing(other.timing),
      boost(other.boost),
      deckCount(other.deckCount),
      combatValue(other.combatValue),
      effect(other.effect ? other.effect->clone() : nullptr)
{
}

Card::~Card()
{
    delete effect;
}

string Card::getName() const
{
    return name;
}

CardType Card::getCardType() const
{
    return cardType;
}

OwnerType Card::getOwnerType() const
{
    return ownerType;
}

int Card::getAmount() const
{
    return deckCount;
}

int Card::getCombatValue() const
{
    return combatValue;
}

int Card::getBoost() const
{
    return boost;
}

Timing Card::getTiming() const
{
    return timing;
}

bool Card::hasEffect() const
{
    return effect != nullptr;
}

bool Card::hasTiming() const
{
    if (timing == Timing::None)
    {
        return false;
    }
    return true;
}

Effect *Card::getEffect() const
{
    return effect;
}

bool Card::isAttack() const
{
    return cardType == CardType::Attack;
}

bool Card::isDefense() const
{
    return cardType == CardType::Defense;
}

bool Card::isVersatile() const
{
    return cardType == CardType::Versatile;
}

bool Card::isScheme() const
{
    return cardType == CardType::Scheme;
}

bool Card::isPlayableBy(const Fighter &fighter) const
{
    if (ownerType == OwnerType::Any)
    {
        return true;
    }
    if (ownerType == OwnerType::Hero && fighter.isHero())
    {
        return true;
    }
    if (ownerType == OwnerType::Sidekick && !fighter.isHero())
    {
        return true;
    }
    return false;
}

void Card::play(Fighter &fighter, Fighter &target, Game &game) const
{
    if (!isPlayableBy(fighter))
    {
        throw runtime_error("\n[!] ERROR : This card cannot be played by " + fighter.getName());
    }
    cout << "\n[.] " << fighter.getName() << " plays " << name << "!" << endl;
    display();
}

void Card::cancelEffects()
{
    effectsCanceled = true;
}

bool Card::isEffectsCanceled() const
{
    return effectsCanceled;
}

void Card::resetEffectsCancel()
{
    effectsCanceled = false;
}

void Card::printWrapped(const string &text, int width) const
{
    size_t start = 0;

    while (start < text.size())
    {
        while (start < text.size() && text[start] == '\n')
            start++;

        if (start >= text.size())
            break;

        size_t end = start + width;

        if (end >= text.size())
        {
            cout << "| "
                 << left << setw(width)
                 << text.substr(start)
                 << "|\n";
            break;
        }

        size_t newline = text.find('\n', start);
        if (newline != string::npos && newline < end)
        {
            cout << "| "
                 << left << setw(width)
                 << text.substr(start, newline - start)
                 << "|\n";

            start = newline + 1;
            continue;
        }

        size_t space = text.rfind(' ', end);

        if (space == string::npos || space < start)
            space = end;

        cout << "| "
             << left << setw(width)
             << text.substr(start, space - start)
             << "|\n";

        start = space;

        while (start < text.size() && text[start] == ' ')
            start++;
    }
}

bool Card::canBePlayedBy(const Fighter &fighter, CardType requiredType) const
{
    if (!(isVersatile() || getCardType() == requiredType))
        return false;

    return isPlayableBy(fighter);
}

void Card::display() const
{
    const int WIDTH = 40;

    auto line = [WIDTH](char c = '-')
    {
        cout << '+' << string(WIDTH, c) << "+\n";
    };

    auto row = [WIDTH](const string &text)
    {
        cout << "| "
             << left << setw(WIDTH - 2)
             << text
             << "|\n";
    };

    string typeText;
    switch (cardType)
    {
    case CardType::Attack:
        typeText = "Attack";
        break;

    case CardType::Defense:
        typeText = "Defense";
        break;

    case CardType::Versatile:
        typeText = "Versatile";
        break;

    case CardType::Scheme:
        typeText = "Scheme";
        break;
    }

    string timingText;
    switch (timing)
    {
    case Timing::None:
        timingText = "None";
        break;

    case Timing::Immediately:
        timingText = "Immediately";
        break;

    case Timing::DuringCombat:
        timingText = "During Combat";
        break;

    case Timing::AfterCombat:
        timingText = "After Combat";
        break;
    }

    string ownerText;
    switch (ownerType)
    {
    case OwnerType::Hero:
        ownerText = "Hero";
        break;

    case OwnerType::Sidekick:
        ownerText = "Sidekick";
        break;

    case OwnerType::Any:
        ownerText = "Any";
        break;
    }

    line();

    row(name);

    line();

    row("Type   : " + typeText);
    row("Amount : " + to_string(deckCount));
    row("Value  : " + to_string(combatValue));
    row("Boost  : " + to_string(boost));
    row("Timing : " + timingText);
    row("Owner  : " + ownerText);

    line();

    row("Effect :");

    string effectText = hasEffect()
                            ? effect->getDescription()
                            : "None";

    printWrapped(effectText, WIDTH - 2);

    line();
}

Card Card::createFeedingFrenzy()
{
    return Card("Feeding Frenzy", CardType::Attack, OwnerType::Hero,
                Timing::DuringCombat, 3, 3, 2, new FeedingFrenzy());
}

Card Card::createBeastform()
{
    return Card("Beastform", CardType::Attack, OwnerType::Hero,
                Timing::DuringCombat, 4, 2, 6, new BeastForm());
}

Card Card::createAmbush()
{
    return Card("Ambush", CardType::Attack, OwnerType::Any,
                Timing::DuringCombat, 3, 2, 2, new Ambush());
}

Card Card::createThirstForSustenance()
{
    return Card("Thirst For Sustenance", CardType::Attack, OwnerType::Sidekick,
                Timing::AfterCombat, 3, 3, 3, new ThirstForSustenance());
}

Card Card::createLookIntoMyEyes()
{
    return Card("Look Into My Eyes", CardType::Defense, OwnerType::Hero,
                Timing::DuringCombat, 2, 3, 1, new LookIntoMyEyes());
}

Card Card::createDash()
{
    return Card("Dash", CardType::Versatile, OwnerType::Any,
                Timing::AfterCombat, 1, 3, 3, new Dash());
}

Card Card::createExploit()
{
    return Card("Exploit", CardType::Versatile, OwnerType::Any,
                Timing::AfterCombat, 1, 3, 4, new Exploit());
}

Card Card::createFeintDracula()
{
    return Card("Feint", CardType::Versatile, OwnerType::Any,
                Timing::Immediately, 2, 3, 2, new FeintDracula());
}

Card Card::createMistForm()
{
    return Card("Mistform", CardType::Scheme, OwnerType::Hero,
                Timing::None, 2, 2, 0, new MistForm());
}

Card Card::createBaptismOfBlood()
{
    return Card("Baptism Of Blood", CardType::Scheme, OwnerType::Hero,
                Timing::None, 2, 2, 0, new BaptismOfBlood());
}

Card Card::createPreyUpon()
{
    return Card("Prey Upon", CardType::Scheme, OwnerType::Hero,
                Timing::None, 4, 2, 0, new PreyUpon());
}

Card Card::createRaveningSeduction()
{
    return Card("Ravening Seduction", CardType::Scheme, OwnerType::Sidekick,
                Timing::None, 2, 3, 0, new RaveningSeduction());
}

Card Card::createTheGameIsAfoot()
{
    return Card("The Game Is Afoot", CardType::Attack, OwnerType::Hero,
                Timing::AfterCombat, 2, 2, 5, new TheGameIsAfoot());
}

Card Card::createServiceRevolver()
{
    return Card("Service Revolver", CardType::Attack, OwnerType::Sidekick,
                Timing::None, 3, 2, 5, nullptr);
}

Card Card::createCounterPunch()
{
    return Card("Counter Punch", CardType::Versatile, OwnerType::Hero,
                Timing::AfterCombat, 1, 3, 3, new CounterPunch());
}

Card Card::createDeduceStrategy()
{
    return Card("Deduce Strategy", CardType::Versatile, OwnerType::Hero,
                Timing::DuringCombat, 1, 3, 3, new DeduceStrategy);
}

Card Card::createEducationNeverEnds()
{
    return Card("Education Never Ends", CardType::Versatile, OwnerType::Any,
                Timing::AfterCombat, 1, 2, 3, new EducationNeverEnds());
}

Card Card::createFixedPoint()
{
    return Card("Fixed Point", CardType::Versatile, OwnerType::Sidekick,
                Timing::AfterCombat, 1, 2, 3, new FixedPoint());
}

Card Card::createStudyMethods()
{
    return Card("Study Methods", CardType::Versatile, OwnerType::Any,
                Timing::AfterCombat, 2, 2, 3, new StudyMethods);
}

Card Card::createFeintSherlock()
{
    return Card("Feint", CardType::Versatile, OwnerType::Any,
                Timing::Immediately, 1, 3, 2, new FeintSherlock());
}

Card Card::createAdministerAid()
{
    return Card("Administer Aid", CardType::Scheme, OwnerType::Sidekick,
                Timing::None, 2, 2, 0, new AdministerAid());
}

Card Card::createEliminateTheImpossible()
{
    return Card("Eliminate The Impossible", CardType::Scheme, OwnerType::Hero,
                Timing::None, 2, 2, 0, new EliminateTheImpossible());
}

Card Card::createMasterOfDisguise()
{
    return Card("Master Of Disguise", CardType::Scheme, OwnerType::Hero,
                Timing::None, 2, 2, 0, new MasterOfDisguise());
}