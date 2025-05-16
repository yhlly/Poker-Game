#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include <QDebug>
#include <QTimer>
#include "Game.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Game game;
    bool hasCalculatedRoundWinner;  // prevent duplicate calculation
    bool pendingScoreUpdate;        // track pending score updates
    QMap<int, bool> swappedCards;   // selected cards for swapping

    // UI elements
    QPushButton* startButton;
    QPushButton* nextRoundButton;
    QPushButton* swapButton;
    QLabel* playerScoreLabel;
    QLabel* computerScoreLabel;
    QLabel* roundLabel;
    QLabel* playerHandTypeLabel;
    QLabel* computerHandTypeLabel;
    QLabel* resultLabel;
    QLabel* swapInfoLabel;
    QLabel* gameStatusLabel;
    QLabel* deckStatusLabel;
    QLabel* computerSwapLabel;
    QLabel* swapsRemainingLabel;
    QLabel* notificationLabel;

    // card labels for player and computer
    QVector<QLabel*> playerCardImageLabels;
    QVector<QLabel*> playerCardNameLabels;
    QVector<QLabel*> computerCardImageLabels;
    QVector<QLabel*> computerCardNameLabels;

    // methods for UI
    void setupUI();
    void updateUI();
    void updateCardDisplay();
    void updateHandTypes();
    void determineRoundWinner();
    void clearCardDisplay();
    void updateSwapControls();
    void loadCardImage(QLabel* cardLabel, const Card& card);
    void loadBackCardImage(QLabel* cardLabel);
    void showGameOverMessage();
    void updateDeckStatus();
    void updateSwapsRemainingDisplay();
    QString handTypeToString(const QString& handType);
    void toggleCardSwap(int cardIndex);
    void applyPendingScoreUpdate();
    void performSwap();
    int countSelectedCards();
    void updateSwapButtonState();
    void showNotification(const QString& message);
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onStartClicked();
    void onNextRoundClicked();
    void onSwapClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
