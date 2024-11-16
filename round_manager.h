#ifndef ROUND_MANAGER_H
#define ROUND_MANAGER_H

#include "deck.cpp"

class RoundManager {
private:
    std::vector<int> playerCards;
    std::vector<int> dealerCards;
    Deck* deck;

    std::vector<std::vector<int>> hard_totals = {
        // Dealer upcards:  2,  3,  4,  5,  6,  7,  8,  9,  10,  A
        {1,  1,  0,  0,  0,  1,  1,  1,  1,  1}, // 12
        {0,  0,  0,  0,  0,  1,  1,  1,  1,  1}, // 13
        {0,  0,  0,  0,  0,  1,  1,  1,  1,  1}, // 14
        {0,  0,  0,  0,  0,  1,  1,  1,  1,  1}, // 15
        {0,  0,  0,  0,  0,  1,  1,  1,  1,  1}, // 16
    };

    std::vector<std::vector<int>> soft_totals = {
        // Dealer upcards:  2,  3,  4,  5,  6,  7,  8,  9,  10,  A
        {1,  1,  1,  1,  1,  0,  1,  1,  1,  1}, // A,7 (18)
        {0,  0,  0,  0,  1,  0,  0,  0,  0,  0}, // A,8 (19)
        {0,  0,  0,  0,  0,  0,  0,  0,  0,  0}  // A,9 (20)
    };
    int countCards();
    double roundWinProbability();
    bool playerDecision();

public:
    RoundManager();
    int initializeRound(std::vector<int> dealerHand, std::vector<int> playerHand);
    bool dealCard(int card, bool playerTurn);

};
// Initialize setup - takes 2 list of integers and initializes dealer and player hands and deals the appropriate cards, then returns a betting amount
// Make decision - takes 2 list of integers similar to "initialize setup" and returns 0 for stand and 1 for hit

#endif
