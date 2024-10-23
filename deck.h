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
    const int SHUFFLE_THRESHOLD = 10;

public:
    Deck(int numDecks);
    int dealCard();
    void undealCard(int card);
    void viewDeck();
    void viewCardFrequencies();
    int getCardFrequency(int card);
    int size();
    bool endOfDeck();
};

#endif
