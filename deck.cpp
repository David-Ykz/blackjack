#include "deck.h"
#include <stack>

void Deck::generateDeck() {
    for (int i = 1; i <= 13; i++) {
        for (int j = 0; j < 4; j++) {
            cards.push_back(std::min(i, 10));
        }
        cardFrequencies[std::min(i, 10) - 1] += 4;
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
    if (cards.size() == 0) {
        std::cout << "no cards left" << std::endl;
    }

    int randomIndex = rand() % cards.size();
    int card = cards[randomIndex];
    int lastCard = cards[cards.size() - 1];
    cards[randomIndex] = lastCard;
    cards.pop_back();

    cardFrequencies[card - 1]--;

    return card;
}

void Deck::undealCard(int card) {
    cards.push_back(card);
    cardFrequencies[card - 1]++;
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
    return cardFrequencies[card - 1];
}

int Deck::size() {
    return cards.size();
}

bool Deck::endOfDeck() {
    return cards.size() < SHUFFLE_THRESHOLD;
}

int Deck::total(std::vector<int>& hand) {
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

bool Deck::softHand(std::vector<int>& hand) {
    int sum = 0;
    bool containsAce = false;
    for (int& card : hand) {
        if (card == 1) {
            containsAce = true;
        }
        sum += card;
    }
    return containsAce && sum < 12;
}

double Deck::bustProbability(std::vector<int>& hand) {
    int numBusts = 0;
    for (int card = 10; card > 0; card--) {
        hand.push_back(card);
        int cardTotal = total(hand);
        hand.pop_back();
        if (cardTotal > 21) {
            numBusts += cardFrequencies[card - 1];
        } else {
            if ((double)numBusts/cards.size() > 1) {
                viewCardFrequencies();
                viewDeck();
            }
            return (double)numBusts/cards.size();
        }
    }
    return (double)numBusts/cards.size();
}

double Deck::dealerWinProbability(std::vector<int>& dealerCards, std::vector<int>& playerCards) {
    const double NUM_TRIALS = 5000;
    const int playerTotal = total(playerCards);
    const double playerBust = bustProbability(playerCards);
    int numWins = 0;
    std::stack<int> dealtCards;
    for (int i = 0; i < NUM_TRIALS; i++) {
        if (i == 500 && fabs(playerBust - (double)numWins/i) > 0.1) {
            return (double)numWins/i;
        }
        if (i == 1000 && fabs(playerBust - (double)numWins/i) > 0.04) {
            return (double)numWins/i;
        }
        int dealerHiddenCard = dealCard();
        dealerCards.push_back(dealerHiddenCard);
        int dealerTotal = total(dealerCards);
        while (dealerTotal < 17) {
            int card = dealCard();
            dealtCards.push(card);
            dealerCards.push_back(card);
            dealerTotal = total(dealerCards);
        }
        if (dealerTotal < 22 && dealerTotal > playerTotal) {
            numWins++;
        }
        while(!dealtCards.empty()) {
            dealerCards.pop_back();

            undealCard(dealtCards.top());
            dealtCards.pop();
        }
        undealCard(dealerHiddenCard);
        dealerCards.pop_back();
    }
    return (double)numWins/NUM_TRIALS;
}

std::vector<std::vector<int>> Deck::dealHands(int numHands) {
    std::vector<std::vector<int>> hands = {};
    std::vector<int> dealerCards = {};
    dealerCards.push_back(dealCard());
    hands.push_back(dealerCards);
    for (int playerNum = 0; playerNum < numHands - 1; playerNum++) {
        std::vector<int> playerCards = {};
        for (int i = 0; i < 2; i++) {
            int card = dealCard();
            playerCards.push_back(card);
        }
        hands.push_back(playerCards);
    }
    return hands;
}

// 1: player win, 0: player tie, -1: player loss
int Deck::compareHands(std::vector<int>& dealerHand, std::vector<int>& playerHand) {
    int playerTotal = total(playerHand);
    int dealerTotal = total(dealerHand);
    if (playerTotal > 21) {
        return -1;
    }
    if (dealerTotal > 21) {
        return 1;
    }
    if (playerTotal == dealerTotal) {
        return 0;
    }
    if (playerTotal == 21) {
        return 2;
    }
    return playerTotal > dealerTotal ? 1 : -1;
}