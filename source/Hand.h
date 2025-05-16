#ifndef HAND_H
#define HAND_H

#include "Deck.h"
#include <map>

class Hand
{
private:
    QVector<Card> cards; // five cards as a hand

public:
    Hand();

    void dealHand(Deck& deck); // deal a new hand
    void sortValue(); // sorts cards by value
    void sortGroup(); // sorts cards by frequency groups
    QString getBest() const; // gets the best hand type
    void setHand(const QVector<int>& cardValues); // sets a hand with cards numeric value
    void swapCard(const QVector<int>& cardIndices, Deck& deck); // swaps selected cards with deck

    bool isStraight(bool* isA5Straight = nullptr) const;  // checks if the hand forms a straight
    bool isFlush() const;     // checks if the hand is a flush
    int getHighCard() const;  // returns the highest card value in the hand
    std::map<int, int> getValueCounts() const; // get value counts for each card
    QVector<Card> getCards() const; // returns cards
    int getBestSwapStrategy(QVector<int>& indices); // computer swap strategies
    int getHandRank() const; // get hand rank
    bool hasA5Straight() const; // check if hand is A-5 straight

    // check if hand contains each hand type
    bool containsPair() const;
    bool containsTwoPair() const;
    bool containsThreeOfAKind() const;
    bool containsFourOfAKind() const;
    bool containsFullHouse() const;
};

#endif // HAND_H
