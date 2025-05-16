#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <QVector>

class Deck
{
private:
    QVector<Card> cards; // standard pack of 52 cards

public:
    Deck();

    void createDeck();              // generates a complete pack of 52 cards
    void shuffle();                 // randomly arranges the cards
    Card dealCard();                // returns the first card in the deck and continues until the entire deck has been dealt
    size_t cardsRemaining() const;  // returns the number of cards remaining in the deck
    void reset();                   // quickly restores deck to 52 shuffled cards.

    // inserts a card at a random position in the deck
    void insertCard(const Card& card);
};

#endif // DECK_H
