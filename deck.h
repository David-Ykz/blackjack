#ifndef DECK_H
#define DECK_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>
#include <chrono>

class Deck {
private:
    std::vector<int> cards;
    int cardFrequencies[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    void generateDeck();
    const int SHUFFLE_THRESHOLD = 20;

public:
    Deck(int numDecks);
    int dealCard();
    int dealCard(int card);
    void undealCard(int card);
    void viewDeck();
    void viewCardFrequencies();
    int getCardFrequency(int card);
    int size();
    bool endOfDeck();

    int total(std::vector<int>& hand);
    bool softHand(std::vector<int>& hand);
    double bustProbability(std::vector<int>& hand);
    double dealerWinProbability(std::vector<int>& dealerHand, std::vector<int>& playerHand);
    std::vector<std::vector<int>> dealHands(int numHands);
    int compareHands(std::vector<int>& dealerHand, std::vector<int>& playerHand);
};

#endif
