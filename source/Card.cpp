#include "Card.h"

// default constructor to initialize empty card
Card::Card() : value(0), suit(""), cardNumber(0) {}

// constructor to initialize card with value, suit and card number
Card::Card(int v, QString s, int num)
    : value(v), suit(s), cardNumber(num) {};

// returns card's value
int Card::getValue() const {
    return value;
};

// returns card's suit
QString Card::getSuit() const {
    return suit;
};

// returns card's name
QString Card::getName() const {

    QString cardName;

    // convert numeric value to its value name
    if (value == 1) {
        cardName = "Low Ace";
    } else if (value == 2) {
        cardName = "Two";
    } else if (value == 3) {
        cardName = "Three";
    } else if (value == 4) {
        cardName = "Four";
    } else if (value == 5) {
        cardName = "Five";
    } else if (value == 6) {
        cardName = "Six";
    } else if (value == 7) {
        cardName = "Seven";
    } else if (value == 8) {
        cardName = "Eight";
    } else if (value == 9) {
        cardName = "Nine";
    } else if (value == 10) {
        cardName = "Ten";
    } else if (value == 11) {
        cardName = "Jack";
    } else if (value == 12) {
        cardName = "Queen";
    } else if (value == 13) {
        cardName = "King";
    } else if (value == 14) {
        cardName = "High Ace";
    } else {
        cardName = QString(); // Invalid card value
    }

    // combine value with suit
    cardName += " of " + suit;

    return cardName;
}

// returns card's number representation
int Card::getNumber() const {
    return cardNumber;
};

QString Card::getImagePath() const {
    QString imageName;
    QString valueName;

    // convert card value to name used in the image name
    if (value == 1 || value == 14) {
        valueName = "ace";
    } else if (value == 11) {
        valueName = "jack";
    } else if (value == 12) {
        valueName = "queen";
    } else if (value == 13) {
        valueName = "king";
    } else {
        // number cards use numeric value
        valueName = QString::number(value);
    }

    // convert suit to lowercase for image name
    QString suitName = suit.toLower();

    // find the path of image
    imageName = QString(":/cards/cards/%1_of_%2.png").arg(valueName).arg(suitName);

    return imageName;
}
