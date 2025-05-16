#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , hasCalculatedRoundWinner(false)
    , pendingScoreUpdate(false)
{
    ui->setupUi(this);
    setWindowTitle("Poker Card Game");
    setMinimumSize(1200, 800);

    setupUI();

    // connect signals and slots
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(nextRoundButton, &QPushButton::clicked, this, &MainWindow::onNextRoundClicked);
    connect(swapButton, &QPushButton::clicked, this, &MainWindow::onSwapClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// initialize the UI elements and layouts
void MainWindow::setupUI() {

    // create central widget
    QWidget* centralWidget = new QWidget(this);

    // general poker game color
    centralWidget->setStyleSheet("background-color: darkGreen;");

    // create main layout
    QGridLayout* mainLayout = new QGridLayout(centralWidget);
    mainLayout->setSpacing(10);

    // game title at top
    QFont titleFont("Arial", 16, QFont::Bold);
    QLabel* titleLabel = new QLabel("Poker Card Game", this);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(titleLabel, 0, 0, 1, 10);

    // game status
    gameStatusLabel = new QLabel("Welcome! Press START to begin a new game.", this);
    gameStatusLabel->setAlignment(Qt::AlignCenter);
    gameStatusLabel->setFont(QFont("Arial", 10));
    gameStatusLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(gameStatusLabel, 1, 0, 1, 10);

    // game result information
    resultLabel = new QLabel("", this);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setFont(QFont("Arial", 11, QFont::Bold));
    resultLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(resultLabel, 2, 0, 1, 10);

    // notification about game
    notificationLabel = new QLabel("", this);
    notificationLabel->setAlignment(Qt::AlignCenter);
    notificationLabel->setFont(QFont("Arial", 10, QFont::Bold));
    notificationLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(notificationLabel, 3, 0, 1, 10);

    // game buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(15);

    startButton = new QPushButton("START", this);
    startButton->setMinimumHeight(35);
    startButton->setFixedWidth(120);
    startButton->setFont(QFont("Arial", 11, QFont::Bold));
    startButton->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 5px;");
    buttonLayout->addWidget(startButton);

    swapButton = new QPushButton("SWAP CARDS", this);
    swapButton->setEnabled(false);
    swapButton->setMinimumHeight(35);
    swapButton->setFixedWidth(120);
    swapButton->setFont(QFont("Arial", 11, QFont::Bold));
    swapButton->setStyleSheet("background-color: #FF9800; color: white; border-radius: 5px;");
    buttonLayout->addWidget(swapButton);

    nextRoundButton = new QPushButton("NEXT ROUND", this);
    nextRoundButton->setEnabled(false);
    nextRoundButton->setMinimumHeight(35);
    nextRoundButton->setFixedWidth(120);
    nextRoundButton->setFont(QFont("Arial", 11, QFont::Bold));
    nextRoundButton->setStyleSheet("background-color: #2196F3; color: white; border-radius: 5px;");
    buttonLayout->addWidget(nextRoundButton);

    // add button layout to main layout
    mainLayout->addLayout(buttonLayout, 4, 0, 1, 10);

    // game states information
    QHBoxLayout* statsLayout = new QHBoxLayout();
    statsLayout->setAlignment(Qt::AlignCenter);
    statsLayout->setSpacing(10);

    QFont scoreFont("Arial", 11);
    scoreFont.setBold(true);

    playerScoreLabel = new QLabel("Player: 0", this);
    playerScoreLabel->setFont(scoreFont);
    playerScoreLabel->setStyleSheet("color: white;");
    playerScoreLabel->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(playerScoreLabel);

    computerScoreLabel = new QLabel("Computer: 0", this);
    computerScoreLabel->setFont(scoreFont);
    computerScoreLabel->setStyleSheet("color: white;");
    computerScoreLabel->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(computerScoreLabel);

    roundLabel = new QLabel("Round: 0/5", this);
    roundLabel->setFont(scoreFont);
    roundLabel->setStyleSheet("color: white;");
    roundLabel->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(roundLabel);

    deckStatusLabel = new QLabel("Deck: 52 cards", this);
    deckStatusLabel->setFont(scoreFont);
    deckStatusLabel->setStyleSheet("color: white;");
    deckStatusLabel->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(deckStatusLabel);

    swapsRemainingLabel = new QLabel("Swaps: 3", this);
    swapsRemainingLabel->setFont(scoreFont);
    swapsRemainingLabel->setStyleSheet("color: white;");
    swapsRemainingLabel->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(swapsRemainingLabel);

    // add states layout to main layout
    mainLayout->addLayout(statsLayout, 5, 0, 1, 10);

    // computer hand
    QLabel* computerLabel = new QLabel("Computer's Hand", this);
    computerLabel->setFont(QFont("Arial", 12, QFont::Bold));
    computerLabel->setAlignment(Qt::AlignCenter);
    computerLabel->setStyleSheet("color: yellow;");
    mainLayout->addWidget(computerLabel, 6, 0, 1, 10);

    // computer hand type
    computerHandTypeLabel = new QLabel("Hand Type: ", this);
    computerHandTypeLabel->setAlignment(Qt::AlignCenter);
    computerHandTypeLabel->setFont(QFont("Arial", 11, QFont::Bold));
    computerHandTypeLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(computerHandTypeLabel, 7, 0, 1, 10);

    // computer swap notification
    computerSwapLabel = new QLabel("", this);
    computerSwapLabel->setAlignment(Qt::AlignCenter);
    QFont computerSwapFont = QFont("Arial", 10);
    computerSwapFont.setItalic(true);
    computerSwapLabel->setFont(computerSwapFont);
    computerSwapLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(computerSwapLabel, 8, 0, 1, 10);

    // computer cards
    QHBoxLayout* computerCardLayout = new QHBoxLayout();
    computerCardLayout->setAlignment(Qt::AlignCenter);
    computerCardLayout->setSpacing(15);

    for (int i = 0; i < 5; i++) {
        QVBoxLayout* cardContainer = new QVBoxLayout();
        cardContainer->setAlignment(Qt::AlignCenter);

        // card image
        QLabel* cardImageLabel = new QLabel(this);
        cardImageLabel->setMinimumSize(100, 150);
        cardImageLabel->setFixedSize(100, 150);
        cardImageLabel->setScaledContents(true);
        cardImageLabel->setAlignment(Qt::AlignCenter);
        cardImageLabel->setStyleSheet("background: transparent; border: none;");
        loadBackCardImage(cardImageLabel);
        cardContainer->addWidget(cardImageLabel);

        // card name
        QLabel* cardNameLabel = new QLabel("", this);
        cardNameLabel->setMinimumWidth(100);
        cardNameLabel->setAlignment(Qt::AlignCenter);
        cardNameLabel->setFont(QFont("Arial", 9));
        cardNameLabel->setStyleSheet("color: white; background: transparent;");
        cardContainer->addWidget(cardNameLabel);

        computerCardLayout->addLayout(cardContainer);
        computerCardImageLabels.append(cardImageLabel);
        computerCardNameLabels.append(cardNameLabel);
    }

    mainLayout->addLayout(computerCardLayout, 9, 0, 1, 10);

    // player section header
    QLabel* playerLabel = new QLabel("Player's Hand", this);
    playerLabel->setFont(QFont("Arial", 12, QFont::Bold));
    playerLabel->setAlignment(Qt::AlignCenter);
    playerLabel->setStyleSheet("color: yellow;");
    mainLayout->addWidget(playerLabel, 10, 0, 1, 10);

    // player hand type
    playerHandTypeLabel = new QLabel("Hand Type: ", this);
    playerHandTypeLabel->setAlignment(Qt::AlignCenter);
    playerHandTypeLabel->setFont(QFont("Arial", 11, QFont::Bold));
    playerHandTypeLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(playerHandTypeLabel, 11, 0, 1, 10);

    // swap info
    swapInfoLabel = new QLabel("Click on cards to select them for swapping", this);
    swapInfoLabel->setAlignment(Qt::AlignCenter);
    swapInfoLabel->setFont(QFont("Arial", 10, QFont::Bold));
    swapInfoLabel->setStyleSheet("color: white;");
    swapInfoLabel->setVisible(false);
    mainLayout->addWidget(swapInfoLabel, 12, 0, 1, 10);

    // player cards
    QHBoxLayout* playerCardLayout = new QHBoxLayout();
    playerCardLayout->setAlignment(Qt::AlignCenter);
    playerCardLayout->setSpacing(15);

    for (int i = 0; i < 5; i++) {
        QVBoxLayout* cardContainer = new QVBoxLayout();
        cardContainer->setAlignment(Qt::AlignCenter);

        // card image
        QLabel* cardImageLabel = new QLabel(this);
        cardImageLabel->setMinimumSize(100, 150);
        cardImageLabel->setFixedSize(100, 150);
        cardImageLabel->setScaledContents(true);
        cardImageLabel->setAlignment(Qt::AlignCenter);
        cardImageLabel->setStyleSheet("background: transparent; border: none;");
        cardImageLabel->setCursor(Qt::PointingHandCursor);
        cardImageLabel->setProperty("cardIndex", i);
        cardImageLabel->installEventFilter(this);
        loadBackCardImage(cardImageLabel);
        cardContainer->addWidget(cardImageLabel);

        // card name
        QLabel* cardNameLabel = new QLabel("", this);
        cardNameLabel->setMinimumWidth(100);
        cardNameLabel->setAlignment(Qt::AlignCenter);
        cardNameLabel->setFont(QFont("Arial", 9));
        cardNameLabel->setStyleSheet("color: white; background: transparent;");
        cardContainer->addWidget(cardNameLabel);

        playerCardLayout->addLayout(cardContainer);
        playerCardImageLabels.append(cardImageLabel);
        playerCardNameLabels.append(cardNameLabel);
        swappedCards.insert(i, false);
    }

    mainLayout->addLayout(playerCardLayout, 13, 0, 1, 10);

    setCentralWidget(centralWidget);
}

void MainWindow::showNotification(const QString& message) {
    notificationLabel->setText(message);

    QTimer::singleShot(3000, this, [this]() {
        notificationLabel->clear();
    });
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        for (int i = 0; i < playerCardImageLabels.size(); i++) {
            if (watched == playerCardImageLabels[i]) {
                if (game.currentRound() >= 1 && game.currentRound() <= 4 && game.getPlayerSwapRemaining() <= 0) {
                    showNotification("You have used all your swaps for this round.");
                }
                else if (game.canSwap()) {
                    toggleCardSwap(i);
                }
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::onSwapClicked() {
    if (game.getPlayerSwapRemaining() <= 0) {
        showNotification("You have used all your swaps for this round.");
        return;
    }
    performSwap();
}

int MainWindow::countSelectedCards() {
    int count = 0;
    for (auto it = swappedCards.begin(); it != swappedCards.end(); ++it) {
        if (it.value()) count++;
    }
    return count;
}

void MainWindow::updateSwapButtonState() {
    int selectedCount = countSelectedCards();
    swapButton->setEnabled(selectedCount > 0 && game.canSwap());
}

void MainWindow::toggleCardSwap(int cardIndex) {
    if (swappedCards[cardIndex]) {
        swappedCards[cardIndex] = false;
        playerCardImageLabels[cardIndex]->setStyleSheet("background: transparent; border: none;");
    }
    else if (countSelectedCards() < game.getPlayerSwapRemaining()) {
        swappedCards[cardIndex] = true;
        playerCardImageLabels[cardIndex]->setStyleSheet("background: transparent; border: 3px solid yellow;");
    }
    else {
        showNotification(QString("You can only swap up to %1 this round. Unselect a card first.").arg(game.getPlayerSwapRemaining()));
    }

    updateSwapButtonState();
}

void MainWindow::performSwap() {
    int swapCount = 0;
    QVector<int> indicesToSwap;

    for (int i = 0; i < swappedCards.size(); i++) {
        if (swappedCards[i]) {
            swapCount++;
            indicesToSwap.append(i + 1);
        }
    }

    if (swapCount > 0) {
        // perform the swap
        game.getHumanPlayer().getHand().swapCard(indicesToSwap, game.getDeck());

        // update game state
        game.setPlayerHasSwapped(true);
        game.setPlayerSwapCount(swapCount);
        game.decrementPlayerSwapsRemaining(swapCount);

        // reset swap indicators
        for (int i = 0; i < swappedCards.size(); i++) {
            swappedCards[i] = false;
            playerCardImageLabels[i]->setStyleSheet("background: transparent; border: none;");
        }

        // update UI
        updateCardDisplay();
        updateHandTypes();
        updateDeckStatus();
        updateSwapsRemainingDisplay();

        // computer swap cards
        game.performComputerSwap();

        // update UI if computer swapped
        if (game.hasComputerSwapped()) {
            updateCardDisplay();
            updateHandTypes();
            updateDeckStatus();

            computerSwapLabel->setText(QString("Computer has swapped %1 cards to improve its hand.").arg(game.getComputerSwapCount()));
            game.setComputerHasSwapped(false);
        } else {
            computerSwapLabel->setText("Computer chose not to swap any cards.");
        }

        game.setPlayerHasSwapped(false);

        // update round winner
        hasCalculatedRoundWinner = false;
        determineRoundWinner();
        hasCalculatedRoundWinner = true;
        pendingScoreUpdate = true;

        gameStatusLabel->setText(QString("You swapped %1 cards. %2 swaps remaining this round.")
                                     .arg(swapCount)
                                     .arg(game.getPlayerSwapRemaining()));

        updateSwapButtonState();
    }
}

void MainWindow::onStartClicked() {
    // reset the game state
    game.startGame();

    hasCalculatedRoundWinner = false;
    pendingScoreUpdate = false;

    // reset swap tracking
    for (int i = 0; i < swappedCards.size(); i++) {
        swappedCards[i] = false;
    }

    // deal cards
    game.dealNextRound();

    // update UI
    updateUI();

    // update button states
    startButton->setEnabled(false);
    nextRoundButton->setEnabled(true);
    swapButton->setEnabled(false);

    updateSwapControls();

    gameStatusLabel->setText("Game started! Round 1 dealt.");
    resultLabel->clear();
    computerSwapLabel->clear();

    // calculate round winner
    determineRoundWinner();
    hasCalculatedRoundWinner = true;
    pendingScoreUpdate = true;
}

void MainWindow::onNextRoundClicked() {
    // apply pending score update if needed
    if (pendingScoreUpdate) {
        applyPendingScoreUpdate();
        pendingScoreUpdate = false;
    }

    // check if game is over
    if (game.isGameOver()) {
        showGameOverMessage();
        return;
    }

    // reset for new round
    hasCalculatedRoundWinner = false;
    pendingScoreUpdate = false;

    // reset swap tracking
    for (int i = 0; i < swappedCards.size(); i++) {
        swappedCards[i] = false;
    }

    // deal cards
    if (game.dealNextRound()) {
        resultLabel->clear();
        computerSwapLabel->clear();

        updateUI();
        updateSwapControls();

        gameStatusLabel->setText(QString("Round %1 dealt.").arg(game.currentRound()));

        // update next round button text
        if (game.currentRound() >= 5 || game.getDeck().cardsRemaining() < 10) {
            nextRoundButton->setText("FINISH GAME");
        }

        // calculate round winner
        determineRoundWinner();
        hasCalculatedRoundWinner = true;
        pendingScoreUpdate = true;
    } else {
        showNotification("Not enough cards remain to deal a new round.");
        showGameOverMessage();
    }
}

void MainWindow::applyPendingScoreUpdate() {
    // get round winner
    Player const& winner = game.winnerOfRound();

    // update scores
    if (winner.getName() == "Player") {
        game.getHumanPlayer().incrementScore();
    } else if (winner.getName() == "Computer") {
        game.getComputerPlayer().incrementScore();
    }

    // update score display
    playerScoreLabel->setText(QString("Player: %1").arg(game.getHumanPlayer().getScore()));
    computerScoreLabel->setText(QString("Computer: %1").arg(game.getComputerPlayer().getScore()));
}

void MainWindow::updateUI() {
    // update round info
    roundLabel->setText(QString("Round: %1/5").arg(game.currentRound()));

    // update scores
    playerScoreLabel->setText(QString("Player: %1").arg(game.getHumanPlayer().getScore()));
    computerScoreLabel->setText(QString("Computer: %1").arg(game.getComputerPlayer().getScore()));

    // update cards, hand types, deck status, and swaps remaining
    updateCardDisplay();
    updateHandTypes();
    updateDeckStatus();
    updateSwapsRemainingDisplay();
}

void MainWindow::updateCardDisplay() {
    // get player cards
    QVector<Card> playerCards = game.getHumanPlayer().getHand().getCards();
    for (int i = 0; i < playerCards.size(); i++) {
        loadCardImage(playerCardImageLabels[i], playerCards[i]);
        playerCardNameLabels[i]->setText(playerCards[i].getName());
    }

    // get computer cards
    QVector<Card> computerCards = game.getComputerPlayer().getHand().getCards();
    for (int i = 0; i < computerCards.size(); i++) {
        loadCardImage(computerCardImageLabels[i], computerCards[i]);
        computerCardNameLabels[i]->setText(computerCards[i].getName());
    }
}

void MainWindow::updateHandTypes() {
    // get hand types
    QString playerHandType = game.getHumanPlayer().getHand().getBest();
    QString computerHandType = game.getComputerPlayer().getHand().getBest();

    // update labels
    playerHandTypeLabel->setText("Hand Type: " + handTypeToString(playerHandType));
    computerHandTypeLabel->setText("Hand Type: " + handTypeToString(computerHandType));
}

QString MainWindow::handTypeToString(const QString& handType) {
    if (handType == "ryfl") return "Royal Flush";
    if (handType == "stfl") return "Straight Flush";
    if (handType == "four") return "Four of a Kind";
    if (handType == "full") return "Full House";
    if (handType == "flsh") return "Flush";
    if (handType == "strt") return "Straight";
    if (handType == "trio") return "Three of a Kind";
    if (handType == "twop") return "Two Pair";
    if (handType == "pair") return "One Pair";
    if (handType == "high") return "High Card";
    return "Unknown";
}

void MainWindow::determineRoundWinner() {
    // determine the winner
    Player const& winner = game.winnerOfRound();

    // convert hand types to readable form
    QString playerHandType = handTypeToString(game.getHumanPlayer().getHand().getBest());
    QString computerHandType = handTypeToString(game.getComputerPlayer().getHand().getBest());

    // update result label based on winner
    if (winner.getName() == "Player") {
        resultLabel->setText(QString("Player wins with %1! (Computer has %2)").arg(playerHandType).arg(computerHandType));
        resultLabel->setStyleSheet("color: #AAFFAA; font-weight: bold; font-size: 14pt;");
    } else if (winner.getName() == "Computer") {
        resultLabel->setText(QString("Computer wins with %1! (Player has %2)").arg(computerHandType).arg(playerHandType));
        resultLabel->setStyleSheet("color: #FFAAAA; font-weight: bold; font-size: 14pt;");
    } else {
        // a draw
        resultLabel->setText(QString("Round is a draw! Both players have %1.").arg(playerHandType));
        resultLabel->setStyleSheet("color: #AAAAFF; font-weight: bold; font-size: 14pt;");
    }

    hasCalculatedRoundWinner = true;
}

void MainWindow::clearCardDisplay() {
    // reset all card displays
    for (int i = 0; i < playerCardImageLabels.size(); i++) {
        loadBackCardImage(playerCardImageLabels[i]);
        playerCardNameLabels[i]->clear();
    }

    for (int i = 0; i < computerCardImageLabels.size(); i++) {
        loadBackCardImage(computerCardImageLabels[i]);
        computerCardNameLabels[i]->clear();
    }

    // reset result labels
    playerHandTypeLabel->setText("Hand Type: ");
    computerHandTypeLabel->setText("Hand Type: ");
    resultLabel->clear();
    roundLabel->setText("Round: 0/5");
    playerScoreLabel->setText("Player: 0");
    computerScoreLabel->setText("Computer: 0");
    computerSwapLabel->clear();
    swapsRemainingLabel->setText("Swaps: 3");
}

void MainWindow::updateSwapControls() {
    // check if swapping is allowed
    bool canSwapNow = game.canSwap();
    swapInfoLabel->setVisible(canSwapNow);

    // reset all swap indicators
    for (int i = 0; i < swappedCards.size(); i++) {
        swappedCards[i] = false;
        playerCardImageLabels[i]->setStyleSheet("background: transparent; border: none;");
    }

    // update swap info with remaining swaps
    if (canSwapNow) {
        swapInfoLabel->setText(QString("Click on cards to select them for swapping")
                                   .arg(game.getPlayerSwapRemaining()));
    }

    updateSwapButtonState();
}

void MainWindow::loadCardImage(QLabel* cardLabel, const Card& card) {
    // get the card image
    QPixmap cardPixmap(card.getImagePath());

    if (cardPixmap.isNull()) {
        cardLabel->setText(card.getName());
        cardLabel->setStyleSheet("background-color: white; border: 1px solid black; border-radius: 5px; padding: 5px;");
    } else {
        cardLabel->setPixmap(cardPixmap);
    }
}

void MainWindow::loadBackCardImage(QLabel* cardLabel) {
    // use a white, transparent background
    cardLabel->setPixmap(QPixmap());
    cardLabel->setText("");
    cardLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0.2); border: 1px solid white; border-radius: 5px;");
}

void MainWindow::showGameOverMessage() {
    // apply pending score update
    if (pendingScoreUpdate) {
        applyPendingScoreUpdate();
        pendingScoreUpdate = false;
    }

    // determine the winner
    QString resultMessage;
    Player const& winner = game.overallWinner();

    if (winner.getName() == "Player") {
        resultMessage = QString("You win the game %1-%2!").arg(game.getHumanPlayer().getScore()).arg(game.getComputerPlayer().getScore());
    } else if (winner.getName() == "Computer") {
        resultMessage = QString("Computer wins the game %1-%2!").arg(game.getComputerPlayer().getScore()).arg(game.getHumanPlayer().getScore());
    } else {
        resultMessage = QString("The game is a draw %1-%1!").arg(game.getHumanPlayer().getScore());
    }

    // update game status
    gameStatusLabel->setText("GAME OVER: " + resultMessage);
    gameStatusLabel->setStyleSheet("color: white; font-weight: bold;"); // Gold color

    // update UI state
    startButton->setEnabled(true);
    nextRoundButton->setEnabled(false);
    swapButton->setEnabled(false);
    nextRoundButton->setText("NEXT ROUND");
    clearCardDisplay();
    swapInfoLabel->setVisible(false);
}

void MainWindow::updateDeckStatus() {
    // get remaining cards
    deckStatusLabel->setText(QString("Deck: %1 cards").arg(game.getDeck().cardsRemaining()));
}

void MainWindow::updateSwapsRemainingDisplay() {
    // ensure round 5 to have 0 swaps
    if (game.currentRound() == 5) {
        swapsRemainingLabel->setText("Swaps: 0");
    } else {
        swapsRemainingLabel->setText(QString("Swaps: %1").arg(game.getPlayerSwapRemaining()));
    }
}
