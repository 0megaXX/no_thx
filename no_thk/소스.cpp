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
    int score = 0;
    vector<int> cards;

    Player(string n) : name(n), coins(11) {}

    bool takeOrPassCard(int card, int& coinPool) {
        if (coins == 0) {
           
            cout << name << "는(은) 코인이 없어 카드 " << card << "를 받습니다." << endl;
          
            cards.push_back(card);
            sort(cards.begin(), cards.end());
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
            sort(cards.begin(), cards.end());
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


void Score(Player& p)
{
    

    if (p.cards.size() != 0)
    {
        int sub_score = p.cards[0];
        int last = p.cards[0];
        for (int a = 1; a < p.cards.size(); a++)
        {
            if (p.cards[a] == last + 1)
            {
                last = p.cards[a];
                continue;
            }
            else
            {
                sub_score += p.cards[a];
                last = p.cards[a];
            }
        }
        p.score = -(sub_score);

    }
}

void printPlayerStatus(const vector<Player>& players) {
    for (const auto& player : players) {
        cout << player.name << " - 코인: " << player.coins << "\n 카드: ";
        if (!player.cards.empty()) {

            for (const auto& card : player.cards) {
                cout << card << " ";
            }

        }
        else 
            cout << "없음";
        
        Score(const_cast<Player&>(player));
        cout << "\n 현재 점수 : " << player.score<<"\n\n";
       
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

            if (cardTaken) 
            {
                startingPlayerIndex = currentPlayerIndex;
                cout << players[currentPlayerIndex].name << "가 카드 " << deck[i] << "를 받고 코인 풀에서 " << coinPool << " 코인을 받습니다." << endl;
                coinPool = 0;
            }

            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
        }

        cout << "현재 코인 풀: " << coinPool << endl << endl;
        system("pause");
    }

    cout << "\n게임이 끝났습니다. 최종 결과는 다음과 같습니다:\n";

    // 각 플레이어의 최종 점수 계산
    for (auto& player : players) {
        player.score += player.coins; // 최종 점수는 점수 + 코인
    }

    // 점수를 기준으로 플레이어 정렬 (내림차순)
    sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
        return a.score > b.score;
        });

    // 최종 점수와 등수 출력
    int rank = 1;
    for (const auto& player : players) {
        cout << rank << "등: " << player.name << " - 점수: " << player.score << '\n';
        rank++;
    }

    cout << endl;


    return 0;
}