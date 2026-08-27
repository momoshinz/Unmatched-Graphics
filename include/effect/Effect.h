#pragma once
#include <string>
#include <vector>

class Fighter;
class Game;
class Card;
class Space;

using namespace std;

struct EffectChoice
{
<<<<<<< HEAD
    Space *selectedSpace = nullptr;
    Space *secondSpace = nullptr;
    Fighter *selectedFighter = nullptr;
    int selectedCardIndex = -1;
    vector<int> selectedCardIndices;
    int selectedOrder = 0;
    int selectedFogId = -1;
    int selectedOptionIndex = -1;
=======
    Space *selectedSpace = nullptr;    
    Space *secondSpace = nullptr;       
    Fighter *selectedFighter = nullptr; 
    int selectedCardIndex = -1;         
    vector<int> selectedCardIndices;    
    int selectedOrder = 0;              
    int selectedFogId = -1;            
    int selectedOptionIndex = -1;     
>>>>>>> a9073d1d4f411fb1f12484d5667471d831251d21
};

enum class EffectInputKind
{
    None,
    ChooseAdjacentEmptySpace,
    ChooseReachableSpace,
    ChooseOpponentCardToBurn,
    ChooseEnemyFighter,
<<<<<<< HEAD
    ChooseTwoCardsAndOrder,
    ChooseFighterMoveThenFogMove,
    ChooseLurkingOption,
    ChooseFogSourceAndDestination,
    ChooseFogAndDestination,
    ChooseEnemyAndFogDestination,
=======
    ChooseTwoCardsAndOrder,        
    ChooseFighterMoveThenFogMove,  
    ChooseLurkingOption,           
    ChooseFogSourceAndDestination, 
    ChooseFogAndDestination,       
    ChooseEnemyAndFogDestination,  
>>>>>>> a9073d1d4f411fb1f12484d5667471d831251d21
    ChooseDefeatedSisterAndZoneSpace,
    ChooseCardsToDiscard,
    ChooseAnyEmptySpace,
    ChooseFighterAndReachableSpace,
    ChooseTargetAdjacentEmptySpace,
    ShowOpponentHand   
};

class Effect
{
public:
    Effect();
    virtual ~Effect() = default;

    virtual void apply(Game &game,
                       Fighter &fighter,
                       Fighter &target,
                       const Card &self,
                       Card *opponentCard,
                       bool didUserWin,
                       const EffectChoice &choice) = 0;

    virtual EffectInputKind getInputKind() const { return EffectInputKind::None; }
    virtual bool shouldRequestInput(Game &game, Fighter &user, Fighter &target, bool didUserWin) const
    {
        return true;
    }
<<<<<<< HEAD
    virtual int getMoveRange() const { return 0; }
    virtual int getFogMoveRange() const { return 0; }
=======
    virtual int getMoveRange() const { return 0; }   
    virtual int getFogMoveRange() const { return 0; } 
>>>>>>> a9073d1d4f411fb1f12484d5667471d831251d21

    virtual string getDescription() const = 0;
    virtual Effect *clone() const = 0;
};