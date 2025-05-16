#ifndef GAME_H
#define GAME_H

#include "Player.h"

class Game
{
private:
    Player human;            // human player
    Player computer;         // computer player
    Player draw;             // special player for draw
    Deck gameDeck;           // deck of cards
    int currentRoundNumber;  // 1-5

    // swap state
    bool canSwapCards;              // whether swapping is allowed
    bool playerHasSwapped;          // whether player has swapped cards
    bool computerHasSwapped;        // whether computer has swapped cards
    int totalPlayerSwapsRemaining;  // player's remaining swaps this round
    int playerSwapCount;            // the number of swapped cards
    int computerSwapCount;          // the number of computer swapped

    // helper method to convert cards to numbers
    QVector<int> cardsToNumbers(const QVector<Card>& cards) const;

    // compare cards when same hand type
    const Player& compareHighCards(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const;
    const Player& comparePair(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const;
    const Player& compareTwoPair(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const;
    const Player& compareThreeOfAKind(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const;
    const Player& compareFullHouse(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const;
    const Player& compareFourOfAKind(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const;
    const Player& compareStraight(const QVector<Card>& playerCards, const QVector<Card>& computerCards,
                                       const Hand& playerHand, const Hand& computerHand) const;

public:
    Game();

    void startGame();                    // starts the game
    bool dealNextRound();                // deals next round of cards, returns false if not enough cards
    Player const& winnerOfRound() const; // returns current round winner
    Player const& overallWinner() const; // returns overall winner of the game
    int currentRound() const;            // returns the current round number
    void resetGame();                    // resets the game state

    Player& getHumanPlayer();
    Player& getComputerPlayer();
    Deck& getDeck();
    bool isGameOver() const;
    bool canSwap() const; // check whether it can swap now
    void setPlayerHasSwapped(bool value);
    void setComputerHasSwapped(bool value);
    bool hasPlayerSwapped() const; // check whether the player has swapped
    bool hasComputerSwapped() const; // check whether the computer has swapped
    void performComputerSwap(); // swap cards for computer
    void setPlayerSwapCount(int count);
    int getPlayerSwapCount() const;
    int getPlayerSwapRemaining() const;
    int getComputerSwapCount() const; // get the number of cards computer swapped
    void decrementPlayerSwapsRemaining(int count = 1);
    void resetPlayerSwapsForNewRound(); // reset swap chances for a new round
};

#endif // GAME_H
