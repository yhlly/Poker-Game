#ifndef CARD_H
#define CARD_H

#include <QString>

class Card
{
private:
    int value;       // 1-14 (A can be 1 or 14)
    QString suit;    // "Clubs", "Diamonds", "Hearts" or "Spades"
    int cardNumber;  // suit (1-4) followed by value (01-13)

public:
    Card();
    Card(int v, QString s, int num);

    int getValue() const;     // returns the value
    QString getSuit() const;  // returns the suit name
    QString getName() const;  // returns the card name: value of suit
    int getNumber() const;    // returns the card number

    QString getImagePath() const; // returns the path of image
};

#endif // CARD_H
