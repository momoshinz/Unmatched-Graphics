#include "player/Player.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include "fighter/DrWatson.h"
#include "fighter/Sisters.h"
#include "card/Card.h"
#include "fighter/Fog.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>

using namespace std;

Player::Player(int age)
    : age(age), hero(nullptr)
{
    if (age < 0)
    {
        throw invalid_argument("\n[!] ERROR : Age cannot be NEGATIVE!\n");
    }
}

Player::~Player()
{
    delete hero;

    for (Sidekick *sidekick : sidekicks)
    {
        delete sidekick;
    }

    sidekicks.clear();

    for (Fog *fog : fogs)
    {
        delete fog;
    }
    fogs.clear();
}

int Player::getAge() const
{
    return age;
}

void Player::setAge(int age)
{
    if (age <= 0)
    {
        throw invalid_argument("\n[!] ERROR : INVALID age!\n");
    }

    this->age = age;
}

bool Player::isYounger(const Player &other) const
{
    return age < other.age;
}

void Player::setHero(Hero *hero)
{
    if (hero == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Hero cannot be NULL!\n");
    }

    if (this->hero != nullptr)
    {
        throw logic_error("\n[!] ERROR : Hero has already been SELECTED!\n");
    }

    this->hero = hero;
    hero->setOwner(this);
}

Hero *Player::getHero() const
{
    return hero;
}

bool Player::hasHero() const
{
    return hero != nullptr;
}

void Player::addSideKick(Sidekick *sidekick)
{
    if (sidekick == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Sidekick cannot be NULL!\n");
    }

    sidekick->setOwner(this);

    sidekicks.push_back(sidekick);
}

void Player::addFog(Fog *fog)
{
    if (fog == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Fog cannot be NULL!\n");
    }

    fogs.push_back(fog);
}

const vector<Sidekick *> &Player::getSideKicks() const
{
    return sidekicks;
}

const vector<Fog *> &Player::getFogs() const
{
    return fogs;
}

vector<Sisters *> Player::getSisters() const
{
    vector<Sisters *> sisters;
    for (Sidekick *sidekick : sidekicks)
    {
        Sisters *sister = dynamic_cast<Sisters *>(sidekick);
        if (sister != nullptr)
        {
            sisters.push_back(sister);
        }
    }
    return sisters;
}

Sidekick *Player::removeSideKick(int index)
{
    if (index < 0 || index >= sidekicks.size())
    {
        throw out_of_range("\n[!] ERROR : No sidekick FOUND!\n");
    }

    Sidekick *removed = sidekicks[index];

    sidekicks.erase(sidekicks.begin() + index);

    return removed;
}

bool Player::drawCardToHand()
{
    Card *card = deck.drawCard();

    if (card == nullptr)
    {
        cout << "\n[!] ERROR : Deck empty. All your fighters take 2 damage!\n";
        if (hero != nullptr && hero->isAlive())
        {
            hero->takeDamage(2);
        }
        for (Sidekick *sidekick : sidekicks)
        {
            if (sidekick != nullptr && sidekick->isAlive())
            {
                sidekick->takeDamage(2);
            }
        }
        return false;
    }

    hand.addCard(card);
    return true;
}

int Player::drawCards(int count)
{
    int drawn = 0;

    for (int i = 0; i < count; i++)
    {
        if (drawCardToHand())
            drawn++;
        else
            break;
    }

    return drawn;
}

Watson *Player::getWatson() const
{
    for (Sidekick *sidekick : sidekicks)
    {
        Watson *watson = dynamic_cast<Watson *>(sidekick);

        if (watson != nullptr)
        {
            return watson;
        }
    }

    return nullptr;
}

Deck &Player::getDeck()
{
    return deck;
}

Hand &Player::getHand()
{
    return hand;
}

const Hand &Player::getHand() const
{
    return hand;
}

DiscardPile &Player::getDiscardPile()
{
    return discardPile;
}

bool Player::hasPlayableCard(const Fighter &fighter, CardType requiredType) const
{
    for (Card *card : hand.getCards())
    {
        if (card->canBePlayedBy(fighter, requiredType))
            return true;
    }

    return false;
}