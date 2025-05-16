#include "Deck.h"
#include <algorithm>
#include <random>

// constructor to create and initialize a deck
Deck::Deck() {
    createDeck(); // initialize the deck with 52 cards
}

// creates a standard pack of 52 cards
void Deck::createDeck() {
    // remove any existing cards
    cards.clear();

    // Suits in alpahbetical order: 1=club, 2=diamonds, 3=heart, 4=spade
    QVector<QString> suits = {"Clubs", "Diamonds", "Hearts", "Spades"};

    // create 13 cards for each suit (1-4)
    for (int s = 1; s <= 4; s++) {
        // for each card value
        for (int v = 1; v <= 13; v++) {
            int cardValue = v;

            // convert card value for ace with default 14
            if (v == 1) {
                cardValue = 14;
            }

            // calculate card number
            int cardNumber = s * 100 + v;

            // create card and add to deck
            cards.push_back(Card(cardValue, suits[s-1], cardNumber));
        }
    }
}

// shuffles the deck
void Deck::shuffle() {
    // use current time as seed for random generator
    unsigned seed = static_cast<unsigned int>(time(nullptr));
    // use default random engine with seed
    std::default_random_engine generator(seed);
    std::shuffle(cards.begin(), cards.end(), generator);
}

// deals the top card from the deck
Card Deck::dealCard() {
    // if deck is empty, create and shuffle a new deck
    if (cards.isEmpty()) {
        reset();
    }

    // take the first card in the deck
    Card card = cards.first();
    cards.removeFirst(); // remove the card from the deck
    return card;
}

// returns the number of remaining cards in the deck
size_t Deck::cardsRemaining() const{
    return static_cast<size_t>(cards.size());
};

// resets the deck to 52 shuffled cards
void Deck::reset() {
    createDeck();
    shuffle();
}

// inserts a card at a random position in the deck
void Deck::insertCard(const Card& card) {
    // create random position
    unsigned seed = static_cast<unsigned int>(time(nullptr));
    std::default_random_engine generator(seed);
    int position = generator() % (cards.size() + 1);

    // Insert the card at the random position
    cards.insert(cards.begin() + position, card);
}
