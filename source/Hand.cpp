#include <algorithm>
#include <map>
#include <QSet>
#include "Hand.h"
#include "Deck.h"

Hand::Hand() {}

// create a new 5 card hand from the deck
void Hand::dealHand(Deck& deck) {
    // clear current hand
    cards.clear();

    // deal 5 new cards
    for (int i = 0; i < 5; i++) {
        cards.push_back(deck.dealCard());
    }
}

// sorts cards by value from lowest to highest
void Hand::sortValue() {
    // sort cards by value (ascending)
    std::sort(cards.begin(), cards.end(),
         [](const Card& a, const Card& b) {return a.getValue() < b.getValue(); });
}

void Hand::sortGroup() {
    // calculate value frequencies
    auto valueCount = getValueCounts();

    // sort by frequency, then by value
    std::sort(cards.begin(), cards.end(),
        [&valueCount](const Card& a, const Card& b) {
            if (valueCount.at(a.getValue()) != valueCount.at(b.getValue())) {
                return valueCount.at(a.getValue()) > valueCount.at(b.getValue());
            }
            return a.getValue() < b.getValue();
        });
}

// returns a four-character representing the hand type
QString Hand::getBest() const {
    // check for royal flush or straight flush
    if (isStraight() && isFlush()) {
        QVector<Card> sortedHand = cards;
        std::sort(sortedHand.begin(), sortedHand.end(),
                  [](const Card& a, const Card& b) { return a.getValue() < b.getValue(); });

    // check if it's royal flush (10-A)
    if (sortedHand[0].getValue() == 10 &&
        sortedHand[1].getValue() == 11 &&
        sortedHand[2].getValue() == 12 &&
        sortedHand[3].getValue() == 13 &&
        sortedHand[4].getValue() == 14) {
        return "ryfl";
    }

    // otherwise it's straight flush
    return "stfl";
    }

    // count value frequencies
    auto valueCount = getValueCounts();

    // check for four of a kind
    for (const auto& pair : valueCount) {
        if (pair.second == 4) {
            return "four";
        }
    }

    // check whether has three of a kind and a pair
    bool hasThree = false;
    bool hasPair = false;
    for (const auto& pair : valueCount) {
        if (pair.second == 3) {
            hasThree = true;
        }
        if (pair.second == 2) {
            hasPair = true;
        }
    }

    // check for full house ( 3 same value and a pair)
    if (hasThree && hasPair) {
        return "full";
    }

    // check for flush
    if (isFlush()) {
        return "flsh";
    }

    // check for straight
    if (isStraight()) {
        return "strt";
    }

    // check for three of a kind
    if (hasThree) {
        return "trio";
    }

    // count pairs
    int pairCount = 0;
    for (const auto& pair : valueCount) {
        if (pair.second == 2) {
            pairCount++;
        }
    }

    // check for two pairs
    if (pairCount == 2) {
        return "twop";
    }

    // check for one pair
    if (pairCount == 1) {
        return "pair";
    }

    // high card
    return "high";
}

// sets the hand with unique card number
void Hand::setHand(const QVector<int>& cardValues) {
    // clear the current hand
    cards.clear();

    // process each card value
    for (int cardNumber: cardValues) {
        int suit = cardNumber / 100; // first digit is suit
        int val = cardNumber % 100;  // last two digits are value

        // convert suit number to name
        QString suitName;
        if (suit == 1) {
            suitName = "Clubs";
        } else if (suit == 2) {
            suitName = "Diamonds";
        } else if (suit == 3) {
            suitName = "Hearts";
        } else if (suit == 4) {
            suitName = "Spades";
        }

        // convert ace to default 14
        int cardValue = val;
        if (val == 1) {
            cardValue = 14;
        }

        // create and add the card to the hand
        cards.push_back(Card(cardValue, suitName, cardNumber));
    }
}

// swaps selected card with one from the deck
void Hand::swapCard(const QVector<int>& cardIndices, Deck& deck) {

    // track already processed indices
    QSet<int> processedIndices;

    // track remaining swapped times
    int swapped = 0;

    for (int index: cardIndices) {
        int i = index - 1;

        // check if index is valid and not already processed
        if (i >= 0 && i < cards.size() &&
            !processedIndices.contains(i) && swapped < 3) {

            // store the original card
            Card oldCard = cards[i];

            // replace with a new card
            cards[i] = deck.dealCard();

            // insert the old card into the deck
            deck.insertCard(oldCard);

            // mark as processed
            processedIndices.insert(i);
            swapped++;
        }
    }
}

// checks if the hand is straight
bool Hand::isStraight(bool* isA5Straight) const {

    // copy cards and sort by value
    QVector<Card> sortedHand = cards;
    std::sort(sortedHand.begin(), sortedHand.end(),
            [](const Card& a, const Card& b) { return a.getValue() < b.getValue(); });

    // check for special straight (A-5)
    if (sortedHand[0].getValue() == 2 &&
        sortedHand[1].getValue() == 3 &&
        sortedHand[2].getValue() == 4 &&
        sortedHand[3].getValue() == 5 &&
        sortedHand[4].getValue() == 14) {
        return true;
    }

    // check for normal straight
    for (int i = 1; i < 5; i++) {
        // check consecutive value
        if (sortedHand[i].getValue() != sortedHand[i-1].getValue()+1) {
            return false;
        }
    }

    return true;
}

// checks if the hand is flush
bool Hand::isFlush() const {

    // get the suit of the first card
    QString suit = cards[0].getSuit();

    // compare all cards with the first suit
    for (int i = 1; i < 5; i++) {
        // if different suit is found, not flush
        if (cards[i].getSuit() != suit) {
            return false;
        }
    }

    return true;
}

// get the highest card value in the hand
int Hand::getHighCard() const {
    // find the highest card value
    int highValue = 0;
    for (const Card& card : cards) {
        if (card.getValue() > highValue) {
            highValue = card.getValue();
        }
    }
    return highValue;
}

// get each value frequency
std::map<int, int> Hand::getValueCounts() const {
    // count frequency of each card value
    std::map<int, int> counts;
    for (const Card& card : cards) {
        counts[card.getValue()]++;
    }
    return counts;
}

// returns cards in the hand
QVector<Card> Hand::getCards() const {
    return cards;
}

// returns the hand's rank (1-10, where 1 is best)
int Hand::getHandRank() const {
    QString handType = getBest();

    // give each hand type rank
    if (handType == "ryfl") return 1; // Royal Flush
    if (handType == "stfl") return 2; // Straight Flush
    if (handType == "four") return 3; // Four of a Kind
    if (handType == "full") return 4; // Full House
    if (handType == "flsh") return 5; // Flush
    if (handType == "strt") return 6; // Straight
    if (handType == "trio") return 7; // Three of a Kind
    if (handType == "twop") return 8; // Two Pair
    if (handType == "pair") return 9; // One Pair

    // high card is lowest rank
    return 10;
}

int Hand::getBestSwapStrategy(QVector<int>& indices) {
    indices.clear();

    // get current hand rank
    int rank = getHandRank();

    // get value counts
    auto valueCount = getValueCounts();

    // royal flush, straight flush (good enough)
    // full house, flush, straight (low rate to get better type, even worse)
    // don't choose to swap
    if (rank <= 2 || (rank >= 4 && rank <= 6)) {
        return 0;
    }
    // four of a kind, two pair changes the single card
    else if (rank == 3 || rank == 8) {
        // find the single card and change
        for (int i = 0; i < cards.size(); i++) {
            if (valueCount[cards[i].getValue()] == 1) {
                indices.push_back(i + 1);
                return 1;
            }
        }
    }
    // three of a kind, one pair changes different cards
    else if (rank == 7 || rank == 9) {
        // change single cards
        for (int i = 0; i < cards.size(); i++) {
            if (valueCount[cards[i].getValue()] == 1) {
                indices.push_back(i + 1);
            }
        }
        return indices.size();
    }
    // high card
    else {
        // sort cards
        QVector<Card> tempCards = cards;
        std::sort(tempCards.begin(), tempCards.end(),
                  [](const Card& a, const Card& b) { return a.getValue() > b.getValue(); });

        // change lowest three cards
        QSet<int> highCardValues;
        highCardValues.insert(tempCards[0].getValue());
        highCardValues.insert(tempCards[1].getValue());

        for (int i = 0; i < cards.size(); i++) {
            if (!highCardValues.contains(cards[i].getValue())) {
                indices.push_back(i + 1);
            }
        }
        return indices.size();
    }

    return 0;
}

// checks if the hand contains at least one pair
bool Hand::containsPair() const {
    // count number of pairs
    auto valueCount = getValueCounts();
    for (const auto& pair : valueCount) {
        if (pair.second >= 2) {
            return true;
        }
    }
    return false;
}

// checks if the hand contains two pair
bool Hand::containsTwoPair() const {
    // count number of pairs
    auto valueCount = getValueCounts();
    int pairCount = 0;
    for (const auto& pair : valueCount) {
        if (pair.second >= 2) {
            pairCount++;
        }
    }
    return pairCount >= 2;
}

// checks if the hand contains three of a kind
bool Hand::containsThreeOfAKind() const {
    auto valueCount = getValueCounts();
    for (const auto& pair : valueCount) {
        if (pair.second >= 3) {
            return true;
        }
    }
    return false;
}

// checks if the hand contains four of a kind
bool Hand::containsFourOfAKind() const {
    auto valueCount = getValueCounts();
    for (const auto& pair : valueCount) {
        if (pair.second >= 4) {
            return true;
        }
    }
    return false;
}

// checks if the hand contains a full house
bool Hand::containsFullHouse() const {
    // find three of a kind and a pair
    auto valueCount = getValueCounts();

    bool hasThree = false;
    bool hasPair = false;

    for (const auto& pair : valueCount) {
        if (pair.second == 3) {
            hasThree = true;
        }
        if (pair.second == 2) {
            hasPair = true;
        }
    }

    return hasThree && hasPair;
}

bool Hand::hasA5Straight() const {
    bool isA5 = false;
    isStraight(&isA5);
    return isA5;
}
