#include "Mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

/* Version
 *
 * Qt Creator 16.0.1
 * Based on Qt 6.9.0 (MSVC 2022, x86_64)
 */

/* Computer Swap Strategy
 *
 * 1. Royal Flush, Straight Flush, Full House, Flush, or Straight: never swap
 *    Reason: The first two are good enough, and the last three are difficult to change to make it a better hand type.
 *
 * 2. Four of a Kind, or Two Pair: Only swaps the single card if player swaps.
 *
 * 3. Three of a Kind:
 *    - If player swapped one card: Swaps the lower of the two different cards
 *    - If player swapped two or three cards: Swaps both two different cards
 *
 * 4. One Pair: Swaps the lowest cards that aren't part of the pair
 *
 * 5. High Card: Swaps the lowest cards firstly
 */
