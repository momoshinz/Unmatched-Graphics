#ifndef PLAYER_H
#define PLAYER_H
using namespace std;
#include <vector>
#include <string>
#include "card/Deck.h"
#include "card/Hand.h"
#include "card/DiscardPile.h"
#include "fighter/Sisters.h"
#include "fighter/DrWatson.h"

class Hero;
class Sidekick;
class Watson;
class Player
{
private:
    int age;
    Hero *hero;
    vector<Sidekick *> sidekicks;
    Deck deck;
    Hand hand;
    DiscardPile discardPile;

public:
    Player(int age);
    ~Player();

    int getAge() const;
    void setAge(int age);
    bool isYounger(const Player &other) const;

    void setHero(Hero *hero);
    Hero *getHero() const;
    bool hasHero() const;

    void addSideKick(Sidekick *sidekick);
    const vector<Sidekick *> &getSideKicks() const;
    vector<Sisters *> getSisters() const;
    Sidekick *removeSideKick(int index);
    bool drawCardToHand();
    int drawCards(int count);
    Watson *getWatson() const;

    Deck &getDeck();
    Hand &getHand();
    const Hand &getHand() const;
    DiscardPile &getDiscardPile();

    bool hasPlayableCard(const Fighter &fighter, CardType requiredType) const;
};
#endif