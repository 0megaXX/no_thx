#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <numeric>
#include <string>

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void setColor(string colorCode) {
    cout << "\033[" << colorCode << "m";
}

void resetColor() {
    cout << "\033[0m";
}

class Player {
public:
    string name;
    int coins;
    vector<int> cards;

    Player(string n) : name(n), coins(11) {}

    bool takeOrPassCard(int card, int& coinPool) {
        if (coins == 0) {
           
            cout << name << "는(은) 코인이 없어 카드 " << card << "를 받습니다." << endl;
          
            cards.push_back(card);
            coins += coinPool;
           // coinPool = 0;
            return true;
        }

        setColor("34");  // 파란색
        cout << name;
        resetColor();
        cout<< ", 카드 " << card << "를 받으시겠습니까? 코인 풀: " << coinPool << " (Y/N): ";
       
        char response;
        cin >> response;

        if (response == 'Y' || response == 'y') {
            cards.push_back(card);
            coins += coinPool;
          //  coinPool = 0;
            return true;
        }
        else {
            coins--;
            coinPool++;
            return false;
        }
    }
};

void printPlayerStatus(const vector<Player>& players) {
    for (const auto& player : players) {
        cout << player.name << " - 코인: " << player.coins << ", 카드: ";
        if (!player.cards.empty()) {
            for (const auto& card : player.cards) {
                cout << card << " ";
            }
        }
        else {
            cout << "없음";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    int numPlayers;
    cout << "사용자의 수를 입력하세요 : ";
    cin >> numPlayers;

    vector<int> deck(35);
    iota(deck.begin(), deck.end(), 1);
    mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    shuffle(deck.begin(), deck.end(), rng);

    int cardsToRemove = numPlayers + 2;
    deck.erase(deck.begin(), deck.begin() + cardsToRemove);

    vector<Player> players;
    for (int i = 0; i < numPlayers; ++i) {
        players.emplace_back("플레이어 " + to_string(i + 1));
    }

    int coinPool = 0;
    size_t startingPlayerIndex = rng() % numPlayers;

    for (size_t i = 0; i < deck.size(); ++i) {
        size_t currentPlayerIndex = startingPlayerIndex;
        bool cardTaken = false;
        while (!cardTaken) {
            clearScreen();
            printPlayerStatus(players);
            cardTaken = players[currentPlayerIndex].takeOrPassCard(deck[i], coinPool);

            if (cardTaken) {
                startingPlayerIndex = currentPlayerIndex;
                cout << players[currentPlayerIndex].name << "가 카드 " << deck[i] << "를 받고 코인 풀에서 " << coinPool << " 코인을 받습니다." << endl;
                coinPool = 0;
            }

            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
        }

        cout << "현재 코인 풀: " << coinPool << endl << endl;
        system("pause");
    }

    return 0;
}