#include "round_manager.h"

bool RoundManager::playerDecision() {
    bool isSoftHand = deck->softHand(playerCards);
    int playerTotal = deck->total(playerCards);
    int dealerCard = deck->total(dealerCards);
    
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

int RoundManager::countCards() {
    int count = 0;
    for (int card = 1; card < 11; card++) {
        if (card == 1 || card == 10) {
            count += deck->getCardFrequency(card);
        } else if (card < 7) {
            count -= deck->getCardFrequency(card);
        }
    }
    return count;
}

double RoundManager::roundWinProbability() {
    const int NUM_SIMULATIONS = 100000;
    int roundsWon = 0;
    int roundsLost = 0;

    for (int simulationNum = 0; simulationNum < NUM_SIMULATIONS; simulationNum++) {
        dealerCards.push_back(deck->dealCard());
        playerCards.push_back(deck->dealCard());
        playerCards.push_back(deck->dealCard());

        bool hit = true;
        while (hit) {
            hit = playerDecision();
            if (hit) {
                playerCards.push_back(deck->dealCard());
            }
            if (deck->total(playerCards) > 21) {
                hit = false;
            }
        }
        dealerCards.push_back(deck->dealCard());
        while (deck->total(dealerCards) < 17) {
            dealerCards.push_back(deck->dealCard());
        }
        int roundOutcome = deck->compareHands(dealerCards, playerCards);
        if (roundOutcome > 0) {
            roundsWon++;
        } else if (roundOutcome < 0) {
            roundsLost++;
        }
        for (int card : dealerCards) {
            deck->undealCard(card);
        }
        for (int card : playerCards) {
            deck->undealCard(card);
        }
    }
    return (double)(roundsWon - roundsLost)/NUM_SIMULATIONS;
}

RoundManager::RoundManager() {
    deck = new Deck(4);
}

int RoundManager::initializeRound(std::vector<int> dealerHand, std::vector<int> playerHand) {
    dealerCards.clear();
    playerCards.clear();
    double winProbability = roundWinProbability();
    for (int card : dealerHand) {
        dealerCards.push_back(deck->dealCard(card));
    }
    for (int card : playerHand) {
        playerCards.push_back(deck->dealCard(card));
    }
    deck->viewDeck();
    return (int)(20 * winProbability);
}

bool RoundManager::dealCard(int card, bool playerTurn) {
    deck->dealCard(card);
    if (playerTurn) {
        playerCards.push_back(card);
    } else {
        dealerCards.push_back(card);
    }
    return playerDecision();
}

