#include <iostream>
#include <stack>
#include <chrono>
#include "deck.h"

int total(std::vector<int>& hand) {
    int sum = 0;
    bool containsAce = false;
    for (int& card : hand) {
        if (card == 1) {
            containsAce = true;
        }
        sum += card;
    }
    return sum < 12 ? sum + 10 : sum;
}



// Variance:
// 100 trials - 10%
// 1000 trials - 2%
double calculateDealerWinProbability(Deck& deck, std::vector<int>& playerCards, std::vector<int>& dealerCards) {
    const int NUM_TRIALS = 10000;
    const int playerTotal = total(playerCards);
    int numWins = 0;
    std::stack<int> dealtCards;
    for (int i = 0; i < NUM_TRIALS; i++) {
        int dealerHiddenCard = deck.dealCard();
        dealerCards.push_back(dealerHiddenCard);
        int dealerTotal = total(dealerCards);
        while (dealerTotal < 17) {
            int card = deck.dealCard();
            dealtCards.push(card);
            dealerCards.push_back(card);
            dealerTotal = total(dealerCards);
        }
        if (dealerTotal < 22 && dealerTotal > playerTotal) {
            numWins++;
        }
        while(!dealtCards.empty()) {
            // std::cout << " " << dealerCards.size() << std::endl;
            dealerCards.pop_back();

            deck.undealCard(dealtCards.top());
            dealtCards.pop();
        }
        deck.undealCard(dealerHiddenCard);
        dealerCards.pop_back();
    }
    return numWins/NUM_TRIALS;
}

double calculatePlayerBustProbability(Deck& deck, std::vector<int>& playerCards) {
    int numBusts = 0;
    for (int card = 10; card > 0; card--) {
        playerCards.push_back(card);
        int playerTotal = total(playerCards);
        playerCards.pop_back();
        if (playerTotal > 21) {
            numBusts += deck.getCardFrequency(card);
        } else {
            return numBusts/deck.size();
        }
    }
    return numBusts/deck.size();
}

bool playerDecision(Deck& deck, std::vector<int>& playerCards, std::vector<int>& dealerCards) {
    if (total(playerCards) <= 10) {
        return 1;
    }
    if (total(playerCards) >= 20) {
        return 0;
    }

    double dealerWinProbability = calculateDealerWinProbability(deck, playerCards, dealerCards);
    double playerBustProbability = calculatePlayerBustProbability(deck, playerCards);

//    std::cout << "Dealer win: " << dealerWinProbability << " Player Bust: " << playerBustProbability << std::endl;

    // Return true for hit and false for stand
    return playerBustProbability < dealerWinProbability;
}

std::vector<std::vector<int>> dealCards(Deck& deck, int numPlayers) {
    std::vector<std::vector<int>> hands = {};
    std::vector<int> dealerCards = {};
    dealerCards.push_back(deck.dealCard());
    hands.push_back(dealerCards);
    for (int playerNum = 0; playerNum < numPlayers; playerNum++) {
        std::vector<int> playerCards = {};
        for (int i = 0; i < 2; i++) {
            int card = deck.dealCard();
            playerCards.push_back(card);
        }
        hands.push_back(playerCards);
    }
    return hands;
}

void runMatch(int numDecks, int numPlayers) {
    const bool SHOW_DEBUG_INFO = false;
    Deck deck = Deck(numDecks);
    int numRounds = 0;
    int* roundsWon = new int[numPlayers];
    int* roundsTied = new int[numPlayers];
    std::fill(roundsWon, roundsWon + numPlayers, 0);
    std::fill(roundsTied, roundsTied + numPlayers, 0);

    auto start = std::chrono::high_resolution_clock::now();
    while (!deck.endOfDeck()) {
        numRounds++;
        if (SHOW_DEBUG_INFO) {            
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "========== New round ==========" << std::endl;
        }

        std::vector<std::vector<int>> hands = dealCards(deck, numPlayers);
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
                    hands[i].push_back(card);
                }
                if (total(hands[i]) > 21) {
                    hit = false;
                }
            }
        }
        hands[0].push_back(deck.dealCard());
        while (total(hands[0]) < 17) {
            int card = deck.dealCard();
            if (SHOW_DEBUG_INFO) {            
                std::cout << "Dealt card to dealer: " << card << std::endl;
            }
            hands[0].push_back(card);
        }

        for (int i = 1; i < hands.size(); i++) {
            if (total(hands[i]) < 22) {
                if (total(hands[0]) > 21 || total(hands[i]) > total(hands[0])) {
                    roundsWon[i - 1]++;
                } else if (total(hands[i]) == total(hands[0])) {
                    roundsTied[i - 1]++;
                }
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "========== Match Summary ==========" << std::endl;

    for (int i = 0; i < numPlayers; i++) {
        std::cout << "Player " << i + 1;
        std::cout << " has won " << roundsWon[i] << "/" << numRounds << " rounds"; 
        std::cout << " and tied " << roundsTied[i] << "/" << numRounds << " rounds" << std::endl;
    }
    std::cout << "Time taken: " << duration.count()/1000.0 << " seconds" << std::endl;

    delete roundsWon;
    delete roundsTied;
}

int main() {
    // Treat ace as 1 when calculating bust probability and 11 when calculating dealer win probability
    for (int i = 0; i < 10; i++) {
        runMatch(4, 2);
    }
    return 0;
}