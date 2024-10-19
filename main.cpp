#include <iostream>
#include <stack>
#include "deck.h"

double calculateDealerWinProbability(Deck deck, int playerCards, int dealerCards) {
    const double NUM_TRIALS = 100000;
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
    const double NUM_TRIALS = 100000;
    int numBusts = 0;
    for (int i = 0; i < NUM_TRIALS; i++) {
        int card = deck.dealCard();
        numBusts += playerCards + card > 21 ? 1 : 0;
        deck.undealCard(card);
    }
    return numBusts/NUM_TRIALS;
}

bool playerDecision(Deck deck, int playerCards, int dealerCards) {
    if (playerCards + 11 < 22) {
        return 1;
    }
    double dealerWinProbability = calculateDealerWinProbability(deck, playerCards, dealerCards);
    double playerBustProbability = calculatePlayerBustProbability(deck, playerCards);

//    std::cout << "Dealer win: " << dealerWinProbability << " Player Bust: " << playerBustProbability << std::endl;

    // Return true for hit and false for stand
    return playerBustProbability < dealerWinProbability;
}

std::vector<int> dealCards(Deck deck, int numPlayers) {
    std::vector<int> hands = {};
    int dealerCards = 0;
    for (int i = 0; i < 2; i++) {
        int card = deck.dealCard();
        dealerCards += card;
    }
    hands.push_back(dealerCards);
    for (int playerNum = 0; playerNum < numPlayers; playerNum++) {
        int playerCards = 0;
        for (int i = 0; i < 2; i++) {
            int card = deck.dealCard();
            playerCards += card;
        }
        hands.push_back(playerCards);
    }
    return hands;
}

int main() {
    const bool SHOW_DEBUG_INFO = false;
    Deck deck = Deck(4);
    int numPlayers = 2;
    int numRounds = 0;
    int* roundsWon = new int[numPlayers];
    int* roundsTied = new int[numPlayers];
    std::fill(roundsWon, roundsWon + numPlayers, 0);
    std::fill(roundsTied, roundsTied + numPlayers, 0);

    while (!deck.endOfDeck()) {
        numRounds++;
        if (SHOW_DEBUG_INFO) {            
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "========== New round ==========" << std::endl;
        }

        std::vector<int> hands = dealCards(deck, numPlayers);
        for (int i = 1; i < hands.size(); i++) {
            if (SHOW_DEBUG_INFO) {            
                std::cout << "----- Player " << i << " -----" << std::endl;
            }
            bool hit = true;
            while (hit) {
                hit = playerDecision(deck, hands[i], hands[0]);
                if (SHOW_DEBUG_INFO) {            
                    std::cout << "Decision: " << (hit ? "hit" : "stand") << std::endl;
                }
                if (hit) {
                    int card = deck.dealCard();
                    if (SHOW_DEBUG_INFO) {            
                        std::cout << "Dealt card to player: " << card << std::endl;
                    }
                    hands[i] += deck.dealCard();
                }
                if (hands[i] > 21) {
                    hit = false;
                }
            }
        }
        while (hands[0] < 17) {
            int card = deck.dealCard();
            if (SHOW_DEBUG_INFO) {            
                std::cout << "Dealt card to dealer: " << card << std::endl;
            }
            hands[0] += card;
        }

        for (int i = 1; i < hands.size(); i++) {
            if (hands[i] < 22) {
                if (hands[0] > 21 || hands[i] > hands[0]) {
                    roundsWon[i - 1]++;
                } else if (hands[i] == hands[0]) {
                    roundsTied[i - 1]++;
                }
            }
        }
        if (SHOW_DEBUG_INFO) {            
            std::cout << "have won" << std::endl;
        }
    }
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "========== Match Summary ==========" << std::endl;

    for (int i = 0; i < numPlayers; i++) {
        std::cout << "Player " << i + 1;
        std::cout << " has won " << roundsWon[i] << "/" << numRounds << " rounds"; 
        std::cout << " and tied " << roundsTied[i] << "/" << numRounds << " rounds" << std::endl;
    }

    delete roundsWon;
    delete roundsTied;

    // if total < 12, call hit; else, call stand and hit
    // stand -- call dealer func ...
    // hit -- generate random card
    // if bust, return -1
    // else, call hit and stand


    // std::cout << "Hello World" << std::endl;


    return 0;
}