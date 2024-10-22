#include <iostream>
#include <stack>
#include <chrono>
#include "deck.h"

int print = 0;
int hundredCounter = 0;
int thousandCounter = 0;

int total(std::vector<int>& hand) {
    int sum = 0;
    bool containsAce = false;
    for (int& card : hand) {
        if (card == 1) {
            containsAce = true;
        }
        sum += card;
    }
    return (containsAce && sum < 12) ? sum + 10 : sum;
}

double calculatePlayerBustProbability(Deck& deck, std::vector<int>& playerCards) {
    double numBusts = 0.0;
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


double calculateDealerWinProbability(Deck& deck, std::vector<int>& playerCards, std::vector<int>& dealerCards) {
    std::cout << total(playerCards) << " - " << dealerCards[0] << " " << total(dealerCards) << std::endl;
    const double NUM_TRIALS = 5000;
    const int playerTotal = total(playerCards);
    const double playerBust = calculatePlayerBustProbability(deck, playerCards);
    double numWins = 0;
    std::stack<int> dealtCards;
    for (int i = 0; i < NUM_TRIALS; i++) {
        if (i == 99 && print < 30) {
            std::cout << playerBust << " 100 - " << numWins/i << std::endl;
            print++;
        }
        if (i == 999 && print < 30) {
            std::cout << playerBust << " 1000 - " << numWins/i << std::endl;
            print++;
        }
        if (i == 9999 && print < 30) {
            std::cout << playerBust << " 10000 - " << numWins/i << std::endl;
            print++;
        }
        if (i == 999 && fabs(playerBust - numWins/i) > 0.05) {
            hundredCounter++;
            return numWins/i;
        }
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



std::vector<Deck> testDeckInitialization(int numDecks, int numTests) {
    std::vector<Deck> decks = {};
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numTests; i++) {
        decks.push_back(Deck(numDecks));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken to initialize decks: " << duration.count()/(1000.0 * decks.size()) << " seconds" << std::endl;
    return decks;
}

void testDealhands(std::vector<Deck> decks, int numPlayers) {
    auto start = std::chrono::high_resolution_clock::now();
    for (Deck deck : decks) {
        std::vector<std::vector<int>> hands = dealCards(deck, numPlayers);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken to deal hands: " << duration.count()/(1000.0 * decks.size()) << " seconds" << std::endl;
}

void testTotal(int numTests) {
    Deck deck = Deck(4);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numTests; i++) {
        std::vector<int> cards = {};
        cards.push_back(deck.dealCard());
        cards.push_back(deck.dealCard());
        total(cards);
        deck.undealCard(cards[0]);
        deck.undealCard(cards[1]);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken to calculate total: " << duration.count()/(1000.0 * numTests) << " seconds" << std::endl;
}

void testCalculateDealerWinProbability(int numTests) {
    std::vector<Deck> decks = {};
    std::vector<std::vector<std::vector<int>>> allHands = {};
    for (int i = 0; i < numTests; i++) {
        decks.push_back(Deck(4));
        allHands.push_back(dealCards(decks.back(), 1));
    }
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1; i++) {
        calculateDealerWinProbability(decks[i], allHands[i][1], allHands[i][0]);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken to calculate dealer wins: " << duration.count()/(1000.0 * 1) << " seconds" << std::endl;
}

void testCalculatePlayerBustProbability(int numTests) {
    auto start = std::chrono::high_resolution_clock::now();
    Deck deck = Deck(4);
    for (int i = 0; i < numTests; i++) {
        std::vector<int> cards;
        cards.push_back(deck.dealCard());
        cards.push_back(deck.dealCard());
//        std::cout << total(cards) << " - " << calculatePlayerBustProbability(deck, cards) << std::endl;
        deck.undealCard(cards[0]);
        deck.undealCard(cards[1]);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken to calculate player busts: " << duration.count()/(1000.0 * numTests) << " seconds" << std::endl;
}

int main() {
    // std::vector<Deck> decks = testDeckInitialization(4, 10000);
    // testDealhands(decks, 1);
    // testTotal(10000);
    testCalculateDealerWinProbability(1000);
    testCalculatePlayerBustProbability(1000);

    std::cout << "hundred " << hundredCounter << std::endl;
    std::cout << "thousand " << thousandCounter << std::endl;



    return 0;
}