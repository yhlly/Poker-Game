#include "Game.h"
#include <map>

// default constructor to initialize the game
Game::Game()
    : human("Player"), computer("Computer"), draw("Draw"), currentRoundNumber(0),
    canSwapCards(false), playerHasSwapped(false), computerHasSwapped(false),
    totalPlayerSwapsRemaining(3), playerSwapCount(0), computerSwapCount(0)  {}

// starts a new game by reseting game state
void Game::startGame() {
    gameDeck.reset();
    human.resetScore();
    computer.resetScore();
    currentRoundNumber = 0;
    playerHasSwapped = false;
    computerHasSwapped = false;
    canSwapCards = false;
    totalPlayerSwapsRemaining = 3;
    playerSwapCount = 0;
}

// deals cards for the next round
bool Game::dealNextRound() {
    // check if enough cards remaining
    if (gameDeck.cardsRemaining() < 10) {
        return false;
    }

    // deal cards to both player
    human.getHand().dealHand(gameDeck);
    computer.getHand().dealHand(gameDeck);

    // first round begins
    currentRoundNumber++;

    // reset swap state for a new round
    playerHasSwapped = false;
    computerHasSwapped = false;
    playerSwapCount = 0;
    totalPlayerSwapsRemaining = 3;

    // only swap in round 1-4
    canSwapCards = (currentRoundNumber >= 1 && currentRoundNumber <= 4);

    return true;
}

// determines and returns the winner of current round
Player const& Game::winnerOfRound() const {
    // get each player's hand type
    QString playerHandType = human.getHand().getBest();
    QString computerHandType = computer.getHand().getBest();

    std::map<QString, int> handRanks {
        {"ryfl", 1}, {"stfl", 2}, {"four", 3}, {"full", 4},
        {"flsh", 5}, {"strt", 6}, {"trio", 7}, {"twop", 8},
        {"pair", 9}, {"high", 10}
    };

    // get the rank of each player's hand
    int playerRank  = handRanks[playerHandType];
    int computerRank = handRanks[computerHandType];

    // lower rank means winner
    if (playerRank < computerRank) {
        return human;
    } else if (playerRank > computerRank) {
        return computer;
    }

    // get cards
    QVector<Card> playerCards = human.getHand().getCards();
    QVector<Card> computerCards = computer.getHand().getCards();

    // hand type are same, compare each cards
    if (playerHandType == "pair") {
        return comparePair(playerCards, computerCards);
    }
    else if (playerHandType == "twop") {
        return compareTwoPair(playerCards, computerCards);
    }
    else if (playerHandType == "trio") {
        return compareThreeOfAKind(playerCards, computerCards);
    }
    else if (playerHandType == "strt" || playerHandType == "stfl") {
        return compareStraight(playerCards, computerCards, human.getHand(), computer.getHand());
    }
    else if (playerHandType == "full") {
        return compareFullHouse(playerCards, computerCards);
    }
    else if (playerHandType == "four") {
        return compareFourOfAKind(playerCards, computerCards);
    }
    // flush card values can be all different like high
    else if (playerHandType == "high" || playerHandType == "flsh") {
        return compareHighCards(playerCards, computerCards);
    }

    return draw;
}

// determines and returns the overall winner after finishing game
Player const& Game::overallWinner() const {
    int playerScore = human.getScore();
    int computerScore = computer.getScore();

    if (playerScore > computerScore) {
        return human;
    } else if (playerScore < computerScore) {
        return computer;
    } else {
        return draw;
    }
}

// returns the current round number
int Game::currentRound() const {
    return currentRoundNumber;
}

// resets the game to initial state
void Game::resetGame() {
    startGame();
}

// computer swaps cards
void Game::performComputerSwap() {
    // if the either has swapped or swap count less than 1, return
    if (!playerHasSwapped || computerHasSwapped || playerSwapCount == 0) {
        return;
    }

    // get computer hand information
    Hand& computerHand = computer.getHand();
    QVector<Card> computerCards = computerHand.getCards();
    QString handType = computerHand.getBest();

    QVector<int> cardsToSwap;

    // swap cards based on player's swapped number and hand type
    if (handType == "ryfl" || handType == "stfl" ||
        handType == "full" || handType == "flsh" || handType == "strt") {
        return;
    }
    else if (handType == "four") {
        // computer swap the single card
        Hand tempHand;
        tempHand.setHand(cardsToNumbers(computerCards));
        tempHand.sortGroup();
        QVector<Card> sortedCards = tempHand.getCards();

        int index = -1;
        int value = sortedCards[0].getValue();

        for (int i = 0; i < sortedCards.size(); i++) {
            if (sortedCards[i].getValue() != value) {
                for (int j = 0; j < computerCards.size(); j++) {
                    if (computerCards[j].getNumber() == sortedCards[i].getNumber()) {
                        index = j;
                        break;
                    }
                }
                break;
            }
        }

        if (index != -1) {
            cardsToSwap.append(index + 1);
        }

    }
    // computer swaps cards
    else if (handType == "trio") {
        Hand tempHand;
        tempHand.setHand(cardsToNumbers(computerCards));
        tempHand.sortGroup();
        QVector<Card> sortedCards = tempHand.getCards();

        int trioValue = sortedCards[0].getValue();

        // if player swap one card, replace the lower one of the two different cards
        if (playerSwapCount == 1) {
            int minSingleValue = INT_MAX;
            int minSingleIndex = -1;

            for (int i = 0; i < computerCards.size(); i++) {
                if (computerCards[i].getValue() != trioValue &&
                    computerCards[i].getValue() < minSingleValue) {
                    minSingleValue = computerCards[i].getValue();
                    minSingleIndex = i;
                }
            }

            if (minSingleIndex != -1) {
                cardsToSwap.append(minSingleIndex + 1);
            }
        }

        // if player swap two or three cards, replace the different two cards
        else if (playerSwapCount == 2 || playerSwapCount == 3) {
            for (int i = 0; i < computerCards.size(); i++) {
                if (computerCards[i].getValue() != trioValue) {
                    cardsToSwap.append(i + 1);
                }
            }
        }
    }
    // computer swap single card
    else if (handType == "twop") {

        Hand tempHand;
        tempHand.setHand(cardsToNumbers(computerCards));
        tempHand.sortGroup();
        QVector<Card> sortedCards = tempHand.getCards();

        // find the single card
        int singleIndex = -1;
        int firstPairValue = sortedCards[0].getValue();
        int secondPairValue = sortedCards[2].getValue();

        for (int i = 0; i < computerCards.size(); i++) {
            if (computerCards[i].getValue() != firstPairValue &&
                computerCards[i].getValue() != secondPairValue) {
                singleIndex = i;
                break;
            }
        }

        if (singleIndex != -1) {
            cardsToSwap.append(singleIndex + 1);
        }

    }
    // computer swaps different cards from the lowest
    else if (handType == "pair") {

        Hand tempHand;
        tempHand.setHand(cardsToNumbers(computerCards));
        tempHand.sortGroup();
        QVector<Card> sortedCards = tempHand.getCards();

        int pairValue = sortedCards[0].getValue();

        // swaps same number as player
        // choose cards are not pairs
        QVector<int> nonPairIndices;
        QVector<int> nonPairValues;

        for (int i = 0; i < computerCards.size(); i++) {
            if (computerCards[i].getValue() != pairValue) {
                nonPairIndices.append(i);
                nonPairValues.append(computerCards[i].getValue());
            }
        }

        // firstly swap lower one
        for (int i = 0; i < nonPairIndices.size() - 1; i++) {
            for (int j = i + 1; j < nonPairIndices.size(); j++) {
                if (nonPairValues[i] > nonPairValues[j]) {
                    std::swap(nonPairIndices[i], nonPairIndices[j]);
                    std::swap(nonPairValues[i], nonPairValues[j]);
                }
            }
        }

        // check player's swap cards number
        for (int i = 0; i < qMin(playerSwapCount, nonPairIndices.size()); i++) {
            cardsToSwap.append(nonPairIndices[i] + 1);
        }
    }
    // computer swaps from the lowest one
    else if (handType == "high") {
        QVector<int> indices;
        QVector<int> values;

        for (int i = 0; i < computerCards.size(); i++) {
            indices.append(i);
            values.append(computerCards[i].getValue());
        }
        for (int i = 0; i < indices.size() - 1; i++) {
            for (int j = i + 1; j < indices.size(); j++) {
                if (values[i] > values[j]) {
                    std::swap(indices[i], indices[j]);
                    std::swap(values[i], values[j]);
                }
            }
        }

        // swap from the lowest card
        for (int i = 0; i < qMin(playerSwapCount, indices.size()); i++) {
            cardsToSwap.append(indices[i] + 1);
        }
    }

    // if there's card to swap recorder, set computerHasSwapped to true
    if (!cardsToSwap.isEmpty()) {
        computerHand.swapCard(cardsToSwap, gameDeck);
        computerHasSwapped = true;
        computerSwapCount = cardsToSwap.size(); // track how many cards were swapped
    } else {
        computerSwapCount = 0;
    }

}

Player& Game::getHumanPlayer() {
    return human;
}

Player& Game::getComputerPlayer() {
    return computer;
}

Deck& Game::getDeck() {
    return gameDeck;
}

// game only has five round and should end when less than 10 cards
bool Game::isGameOver() const {
    return currentRoundNumber >= 5 || gameDeck.cardsRemaining() < 10;
}

bool Game::canSwap() const {
    return canSwapCards && totalPlayerSwapsRemaining > 0;
}

void Game::setPlayerHasSwapped(bool value) {
    playerHasSwapped = value;
}

void Game::setComputerHasSwapped(bool value) {
    computerHasSwapped = value;
}

bool Game::hasPlayerSwapped() const {
    return playerHasSwapped;
}

bool Game::hasComputerSwapped() const {
    return computerHasSwapped;
}

void Game::setPlayerSwapCount(int count) {
    playerSwapCount = count;
}

int Game::getPlayerSwapCount() const {
    return playerSwapCount;
}

int Game::getPlayerSwapRemaining() const {
    return totalPlayerSwapsRemaining;
}

void Game::decrementPlayerSwapsRemaining(int count) {
    if (totalPlayerSwapsRemaining >= count) {
        totalPlayerSwapsRemaining -= count;
    } else {
        totalPlayerSwapsRemaining = 0;
    }
}

// helper method to convert Cards to numbers for setHand
QVector<int> Game::cardsToNumbers(const QVector<Card>& cards) const {
    QVector<int> numbers;
    for (const Card& card : cards) {
        numbers.append(card.getNumber());
    }
    return numbers;
}

// compare cards in high type (which can also be used to compare flush)
const Player& Game::compareHighCards(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const {
    // sort both hands for comparing
    Hand playerHand, computerHand;
    playerHand.setHand(cardsToNumbers(playerCards));
    computerHand.setHand(cardsToNumbers(computerCards));
    playerHand.sortValue();
    computerHand.sortValue();

    // get sorted cards
    QVector<Card> sortedPlayerCards = playerHand.getCards();
    QVector<Card> sortedComputerCards = computerHand.getCards();

    // Compare each card from the highest one
    for (int i = sortedPlayerCards.size() - 1; i >= 0; i--) {
        if (sortedPlayerCards[i].getValue() > sortedComputerCards[i].getValue()) {
            return human;
        } else if (sortedComputerCards[i].getValue() > sortedPlayerCards[i].getValue()) {
            return computer;
        }
    }

    // same hand, it's a draw
    return draw;
}

// compare cards with same pair type
const Player& Game::comparePair(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const {
    // sort both hands frequencies for comparing
    Hand playerHand, computerHand;
    playerHand.setHand(cardsToNumbers(playerCards));
    computerHand.setHand(cardsToNumbers(computerCards));
    playerHand.sortGroup();
    computerHand.sortGroup();

    // get sorted cards
    QVector<Card> sortedPlayerCards = playerHand.getCards();
    QVector<Card> sortedComputerCards = computerHand.getCards();

    // compare pair values (first card is part of the pair)
    if (sortedPlayerCards[0].getValue() > sortedComputerCards[0].getValue()) {
        return human;
    } else if (sortedComputerCards[0].getValue() > sortedPlayerCards[0].getValue()) {
        return computer;
    }

    // if pair are same, compare other cards from the highest one
    for (int i = 2; i < sortedPlayerCards.size(); i++) {
        if (sortedPlayerCards[i].getValue() > sortedComputerCards[i].getValue()) {
            return human;
        } else if (sortedComputerCards[i].getValue() > sortedPlayerCards[i].getValue()) {
            return computer;
        }
    }

    return draw;
}

// compare cards for two pair type
const Player& Game::compareTwoPair(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const {
    // sort both hands frequencies for comparing
    Hand playerHand, computerHand;
    playerHand.setHand(cardsToNumbers(playerCards));
    computerHand.setHand(cardsToNumbers(computerCards));
    playerHand.sortGroup();
    computerHand.sortGroup();

    QVector<Card> sortedPlayerCards = playerHand.getCards();
    QVector<Card> sortedComputerCards = computerHand.getCards();

    // compare first pair (indices 0-1)
    if (sortedPlayerCards[0].getValue() > sortedComputerCards[0].getValue()) {
        return human;
    } else if (sortedComputerCards[0].getValue() > sortedPlayerCards[0].getValue()) {
        return computer;
    }

    // compare second pair (indices 2-3)
    if (sortedPlayerCards[2].getValue() > sortedComputerCards[2].getValue()) {
        return human;
    } else if (sortedComputerCards[2].getValue() > sortedPlayerCards[2].getValue()) {
        return computer;
    }

    // compare single card
    if (sortedPlayerCards[4].getValue() > sortedComputerCards[4].getValue()) {
        return human;
    } else if (sortedComputerCards[4].getValue() > sortedPlayerCards[4].getValue()) {
        return computer;
    }

    return draw;
}

// compare cards of three of a kind
const Player& Game::compareThreeOfAKind(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const {
    // sort both hands frequencies for comparing
    Hand playerHand, computerHand;
    playerHand.setHand(cardsToNumbers(playerCards));
    computerHand.setHand(cardsToNumbers(computerCards));
    playerHand.sortGroup();
    computerHand.sortGroup();

    QVector<Card> sortedPlayerCards = playerHand.getCards();
    QVector<Card> sortedComputerCards = computerHand.getCards();

    // compare three of a kind values
    if (sortedPlayerCards[0].getValue() > sortedComputerCards[0].getValue()) {
        return human;
    } else if (sortedComputerCards[0].getValue() > sortedPlayerCards[0].getValue()) {
        return computer;
    }

    // if three card are same, compare the rest from the highest one
    for (int i = 3; i < sortedPlayerCards.size(); i++) {
        if (sortedPlayerCards[i].getValue() > sortedComputerCards[i].getValue()) {
            return human;
        } else if (sortedComputerCards[i].getValue() > sortedPlayerCards[i].getValue()) {
            return computer;
        }
    }

    return draw;
}

// compare straight or straight flush
const Player& Game::compareStraight(const QVector<Card>& playerCards, const QVector<Card>& computerCards,
                                         const Hand& playerHand, const Hand& computerHand) const {

    // sort cards
    Hand playerHandCopy, computerHandCopy;
    playerHandCopy.setHand(cardsToNumbers(playerCards));
    computerHandCopy.setHand(cardsToNumbers(computerCards));
    playerHandCopy.sortValue();
    computerHandCopy.sortValue();

    QVector<Card> sortedPlayerCards = playerHandCopy.getCards();
    QVector<Card> sortedComputerCards = computerHandCopy.getCards();

    // A-5 straight flush
    bool playerA5 = playerHand.hasA5Straight();
    bool computerA5 = computerHand.hasA5Straight();

    // A-5 straight is lower than all other straight
    if (playerA5 && !computerA5) {
        return computer;
    } else if (!playerA5 && computerA5) {
        return human;
    } else if (playerA5 && computerA5) {
        return draw; // both A-5, it's a draw
    }

    // get the last card which is the highest
    int playerHigh = sortedPlayerCards.last().getValue();
    int computerHigh = sortedComputerCards.last().getValue();

    // compare highest cards
    if (playerHigh > computerHigh) {
        return human;
    } else if (computerHigh > playerHigh) {
        return computer;
    }

    return draw;
}

// compare full house
const Player& Game::compareFullHouse(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const {
    // sort cards
    Hand playerHand, computerHand;
    playerHand.setHand(cardsToNumbers(playerCards));
    computerHand.setHand(cardsToNumbers(computerCards));
    playerHand.sortGroup();
    computerHand.sortGroup();
    QVector<Card> sortedPlayerCards = playerHand.getCards();
    QVector<Card> sortedComputerCards = computerHand.getCards();

    // compare three of a kind values
    if (sortedPlayerCards[0].getValue() > sortedComputerCards[0].getValue()) {
        return human;
    } else if (sortedComputerCards[0].getValue() > sortedPlayerCards[0].getValue()) {
        return computer;
    }

    // compare pair values
    if (sortedPlayerCards[3].getValue() > sortedComputerCards[3].getValue()) {
        return human;
    } else if (sortedComputerCards[3].getValue() > sortedPlayerCards[3].getValue()) {
        return computer;
    }

    return draw;
}

// compare four of a kind
const Player& Game::compareFourOfAKind(const QVector<Card>& playerCards, const QVector<Card>& computerCards) const {
    // sort cards
    Hand playerHand, computerHand;
    playerHand.setHand(cardsToNumbers(playerCards));
    computerHand.setHand(cardsToNumbers(computerCards));
    playerHand.sortGroup();
    computerHand.sortGroup();
    QVector<Card> sortedPlayerCards = playerHand.getCards();
    QVector<Card> sortedComputerCards = computerHand.getCards();

    // compare four of a kind values
    if (sortedPlayerCards[0].getValue() > sortedComputerCards[0].getValue()) {
        return human;
    } else if (sortedComputerCards[0].getValue() > sortedPlayerCards[0].getValue()) {
        return computer;
    }

    // compare single card
    if (sortedPlayerCards[4].getValue() > sortedComputerCards[4].getValue()) {
        return human;
    } else if (sortedComputerCards[4].getValue() > sortedPlayerCards[4].getValue()) {
        return computer;
    }

    return draw;
}

int Game::getComputerSwapCount() const {
    return computerSwapCount;
}
