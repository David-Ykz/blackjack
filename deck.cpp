#include "deck.h"

void Deck::generateDeck() {
    for (int i = 1; i <= 13; i++) {
        for (int j = 0; j < 4; j++) {
            cards.push_back(std::min(i, 10));
        }
        cardFrequencies[std::min(i, 10)] += 4;
    }
}

Deck::Deck(int numDecks) {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    srand(static_cast<unsigned int>(duration.count()));
    for (int i = 0; i < numDecks; i++) {
        generateDeck();
    }
}

int Deck::dealCard() {
    int randomIndex = rand() % cards.size();
    int card = cards[randomIndex];
    int lastCard = cards[cards.size() - 1];
    cards[randomIndex] = lastCard;
    cards.pop_back();

    cardFrequencies[card]--;

    return card;
}

void Deck::undealCard(int card) {
    cards.push_back(card);
    cardFrequencies[card]++;
}

void Deck::viewDeck() {
    std::cout << cards.size() << " -- ";
    for (int card : cards) {
        std::cout << card << " ";
    }
    std::cout << std::endl;
}

void Deck::viewCardFrequencies() {
    for (int frequency : cardFrequencies) {
        std::cout << frequency << " ";
    }
    std::cout << std::endl;
}

int Deck::getCardFrequency(int card) {
    return cardFrequencies[card];
}

int Deck::size() {
    return cards.size();
}

bool Deck::endOfDeck() {
    return cards.size() < SHUFFLE_THRESHOLD;
}

