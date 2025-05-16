#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"

class Player
{
private:
    QString name;    // player's name
    int score;       // player's score
    Hand playerHand; // player's current hand of cards

public:
    Player();

    Player(const QString& playerName);

    void resetScore();        // resets the score to zero
    void incrementScore();    // increases the score by one
    QString getName() const;  // returns the name
    int getScore() const;     // returns the current score

    // get player's hand
    Hand& getHand();
    const Hand& getHand() const;
};

#endif // PLAYER_H
