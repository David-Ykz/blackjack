#ifndef DECK_H
#define DECK_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>

class Deck {
private:
    std::vector<int> cards;
    void generateDeck();

public:
    Deck(int numDecks);
    int dealCard();
    void undealCard(int card);
    void viewDeck();
};

#endif
