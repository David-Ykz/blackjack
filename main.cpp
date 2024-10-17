#include <iostream>
#include <stack>
#include "deck.h"

double calculateDealerWinProbability(Deck deck, int playerCards, int dealerCards) {
    const double NUM_TRIALS = 10000;
    int numWins = 0;
    std::stack<int> dealtCards;
    for (int i = 0; i < NUM_TRIALS; i++) {
        while (dealerCards < 17) {
            int card = deck.dealCard();
            dealtCards.push(card);
            dealerCards += card;
        }
        if (dealerCards < 22 && dealerCards > playerCards) {
            numWins++;
        }
        while(!dealtCards.empty()) {
            dealerCards -= dealtCards.top();
            deck.undealCard(dealtCards.top());
            dealtCards.pop();
        }
    }
    return numWins/NUM_TRIALS;
}

double calculatePlayerBustProbability(Deck deck, int playerCards) {
    const double NUM_TRIALS = 10000;
    int numBusts = 0;
    for (int i = 0; i < NUM_TRIALS; i++) {
        int card = deck.dealCard();
        numBusts += playerCards + card > 21 ? 1 : 0;
        deck.undealCard(card);
    }
    return numBusts/NUM_TRIALS;
}

bool playerDecision(Deck deck, int playerCards, int dealerCards) {
    double dealerWinProbability = calculateDealerWinProbability(deck, playerCards, dealerCards);
    double playerBustProbability = calculatePlayerBustProbability(deck, playerCards);

    std::cout << "Dealer win: " << dealerWinProbability << " Player Bust: " << playerBustProbability << std::endl;

    // Return 1 for hit and 0 for stand
    return playerBustProbability < dealerWinProbability;
}

int main() {
    Deck deck = Deck(1);

    int playerCards = 0;
    int dealerCards = 0;
    std::cout << "player cards: ";
    for (int i = 0; i < 2; i++) {
        int card = deck.dealCard();
        std::cout << card << " ";
        playerCards += card;
    }
    std::cout << std::endl;
    std::cout << "dealer cards: ";
    for (int i = 0; i < 2; i++) {
        int card = deck.dealCard();
        std::cout << card << " ";
        dealerCards += card;
    }
    std::cout << std::endl;

    std::cout << "Decision: " << (playerDecision(deck, playerCards, dealerCards) ? "hit" : "stand") << std::endl;


    // if total < 12, call hit; else, call stand and hit
    // stand -- call dealer func ...
    // hit -- generate random card
    // if bust, return -1
    // else, call hit and stand


    // std::cout << "Hello World" << std::endl;


    return 0;
}