#include "Player.h"

// default constructor to create a player
Player::Player() : name("Player"), score(0) {}

// constructor to create a player with specified name
Player::Player(const QString& playerName) :
    name(playerName), score(0) {}

// resets player's score to zero
void Player::resetScore() {
    score = 0;
};

// increases player's score by one
void Player::incrementScore() {
    score++;
};

// returns player's name
QString Player::getName() const {
    return name;
};

// returns player's current score
int Player::getScore() const {
    return score;
};

// returns a reference to player's hand (modifiable)
Hand& Player::getHand() {
    return playerHand;
}

// returns a const reference to player's hand (non-modifiable)
const Hand& Player::getHand() const {
    return playerHand;
};

