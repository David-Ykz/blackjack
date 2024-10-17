#include "deck.h"

void Deck::generateDeck() {
    for (int i = 1; i <= 13; i++) {
        for (int j = 0; j < 4; j++) {
            cards.push_back(std::min(i, 10));
        }
    }
}

Deck::Deck(int numDecks) {
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < numDecks; i++) {
        generateDeck();
    }
}

int Deck::dealCard() {
    int randomIndex = rand() % cards.size();
    int card = cards[randomIndex];
    cards.erase(cards.begin() + randomIndex);
    return card;
}

void Deck::undealCard(int card) {
    cards.push_back(card);
}

void Deck::viewDeck() {
    std::cout << cards.size() << " -- ";
    for (int card : cards) {
        std::cout << card << " ";
    }
    std::cout << std::endl;
}
