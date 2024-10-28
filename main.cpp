#include <iostream>
#include <stack>
#include <chrono>
#include "deck.h"
#include <cstdlib>
#include <vector>
#include <fstream>

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


// bool playerDecision(Deck& deck, std::vector<int>& dealerCards, std::vector<int>& playerCards) {
//     if (deck.total(playerCards) <= 10) {
//         return 1;
//     }
//     if (deck.total(playerCards) >= 20) {
//         return 0;
//     }

//     double dealerWinProbability = deck.dealerWinProbability(dealerCards, playerCards);
//     double playerBustProbability = deck.bustProbability(playerCards);

// //    std::cout << dealerWinProbability << " -- " << playerBustProbability << std::endl;

//     // Return true for hit and false for stand
//     return playerBustProbability < dealerWinProbability;
// }

bool simpleHeuristicDecision(Deck& deck, std::vector<int>& playerCards) {
    return deck.total(playerCards) < 18;
}


bool playerDecision(Deck& deck, std::vector<int>& dealerCards, std::vector<int>& playerCards) {
    bool isSoftHand = deck.softHand(playerCards);
    int playerTotal = deck.total(playerCards);
    int dealerCard = deck.total(dealerCards);
    
    if (isSoftHand) {
        if (playerTotal < 18) {
            return true;
        } else if (playerTotal > 19) {
            return false;
        }
        return soft_totals[playerTotal - 18][dealerCard - 2];
    } else {
        if (playerTotal < 12) {
            return true;
        } else if (playerTotal > 16) {
            return false;
        }
        return hard_totals[playerTotal - 12][dealerCard - 2];
    }
}

int cardCounting(Deck& deck) {
    int count = 0;
    for (int card = 1; card < 11; card++) {
        if (card == 1 || card == 10) {
            count += deck.getCardFrequency(card);
        } else if (card < 7) {
            count -= deck.getCardFrequency(card);
        }
    }
    return count;
}

double roundWinProbability(Deck& deck, int numPlayers, int playerIndex) {
    const int NUM_SIMULATIONS = 4000;
    int roundsWon = 0;
    int roundsTied = 0;
    int roundsLost = 0;

    for (int simulationNum = 0; simulationNum < NUM_SIMULATIONS; simulationNum++) {
        std::vector<std::vector<int>> hands = deck.dealHands(numPlayers);
        for (int i = 1; i < hands.size(); i++) {
            bool hit = true;
            while (hit) {
                hit = playerDecision(deck, hands[0], hands[i]);
                if (hit) {
                    int card = deck.dealCard();
                    hands[i].push_back(card);
                }
                if (deck.total(hands[i]) > 21) {
                    hit = false;
                }
            }
        }
        hands[0].push_back(deck.dealCard());
        while (deck.total(hands[0]) < 17) {
            int card = deck.dealCard();
            hands[0].push_back(card);
        }
        int roundOutcome = deck.compareHands(hands[0], hands[playerIndex]);
        if (roundOutcome == 1) {
            roundsWon++;
        } else if (roundOutcome == 0) {
            roundsTied++;
        } else {
            roundsLost++;
        }

        for (std::vector<int> hand : hands) {
            for (int card : hand) {
                deck.undealCard(card);
            }
        }
    }
    return (double)(roundsWon - roundsLost)/NUM_SIMULATIONS;
}

double* runMatch(int numDecks, int numPlayers) {
    const bool SHOW_DEBUG_INFO = false;
    const double BET_SCALING = 0.05;
    const int MAX_BET = 500;
    const int playerIndex = 0;
    Deck deck = Deck(numDecks);
    int numRounds = 0;
    int* roundsWon = new int[numPlayers];
    int* roundsTied = new int[numPlayers];
    double* bets = new double[numPlayers];
    double* winnings = new double[numPlayers];
//    int* winnings = new int[numPlayers];
    std::fill(roundsWon, roundsWon + numPlayers, 0);
    std::fill(roundsTied, roundsTied + numPlayers, 0);
    std::fill(bets, bets + numPlayers, 100);
    std::fill(winnings, winnings + numPlayers, 0);

    auto start = std::chrono::high_resolution_clock::now();
    while (!deck.endOfDeck()) {
        numRounds++;
        if (SHOW_DEBUG_INFO) {            
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "========== New round ==========" << std::endl;
        }

        bets[!playerIndex] = 10 * cardCounting(deck);
        bets[playerIndex] = 20 * roundWinProbability(deck, numPlayers, playerIndex);
        // bets[playerIndex] = MAX_BET * roundWinProbability(deck, numPlayers, playerIndex);
        
        for (int i = 0; i < numPlayers; i++) {
            if (bets[i] < 100) {
                bets[i] = 100;
            } else if (bets[i] > MAX_BET) {
                bets[i] = MAX_BET;
            }
        }

        std::vector<std::vector<int>> hands = deck.dealHands(numPlayers + 1);
        for (int i = 1; i < hands.size(); i++) {
            bool hit = true;
            while (hit) {
                // if (playerIndex + 1 == i) {
                //     hit = playerDecision(deck, hands[0], hands[i]);
                // } else {
                //     hit = simpleHeuristicDecision(deck, hands[i]);
                // }
                hit = playerDecision(deck, hands[0], hands[i]);
                if (hit) {
                    hands[i].push_back(deck.dealCard());
                }
                if (deck.total(hands[i]) > 21) {
                    hit = false;
                }
            }
        }
        hands[0].push_back(deck.dealCard());
        while (deck.total(hands[0]) < 17) {
            hands[0].push_back(deck.dealCard());
        }

        for (int i = 1; i < hands.size(); i++) {
            int roundOutcome = deck.compareHands(hands[0], hands[i]);
            if (roundOutcome == 2) {
                roundsWon[i - 1]++;
                winnings[i - 1] += 1.5 * bets[i - 1];
            } else if (roundOutcome == 1) {
                roundsWon[i - 1]++;
                winnings[i - 1] += bets[i - 1];
            } else if (roundOutcome == 0) {
                roundsTied[i - 1]++;
            } else {
                winnings[i - 1] -= bets[i - 1];
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // std::cout << "========== Match Summary ==========" << std::endl;

    // for (int i = 0; i < numPlayers; i++) {
    //     std::cout << "Player " << i + 1;
    //     std::cout << " has won " << roundsWon[i] << "/" << numRounds << " rounds"; 
    //     std::cout << " and tied " << roundsTied[i] << "/" << numRounds << " rounds";
    //     std::cout << ", winning " << winnings[i] << std::endl;
    // }
    // std::cout << "Time taken: " << duration.count()/1000.0 << " seconds" << std::endl;

    delete roundsWon;
    delete roundsTied;
    delete bets;

    return winnings;
}

int main() {
    const int NUM_PLAYERS = 2;
    const int NUM_DECKS = 4;
    const int NUM_MATCHES = 10000;
    double* totalWinnings = new double[NUM_PLAYERS];
    std::fill(totalWinnings, totalWinnings + NUM_PLAYERS, 0);

    std::ofstream p1File("player1.csv");
    std::ofstream p2File("player2.csv");
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 1; i <= NUM_MATCHES; i++) {
//        std::cout << i << " ";
        double* winnings = runMatch(4, 2);
        for (int j = 0; j < NUM_PLAYERS; j++) {
            totalWinnings[j] += winnings[j];
        }
        p1File << winnings[0] << ",";
        p2File << winnings[1] << ",";

        delete winnings;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count()/1000.0 << " seconds" << std::endl;


    std::cout << "---" << std::endl;
    std::cout << totalWinnings[0]/NUM_MATCHES << " - " << totalWinnings[1]/NUM_MATCHES << std::endl;
    return 0;
}