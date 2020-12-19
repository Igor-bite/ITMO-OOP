#include <iostream>
#include <vector>
#include "Transport.cpp"
#include <utility>
using namespace std;

class MyException : public exception {
public:
    const char * what() const throw()
    {
        return "Something went wrong";
    }
};

template<class T>
class Race {
public:
    Race(Coordinate finish) {
        players = vector<T*>();
        this->finish = finish;
    }

    void registrate(T* transp) {
        int uid = players.size() + 1;
        transp->setID(uid);
        players.push_back(transp);
    }
    void printAllPlayersIds() {
        for(int i = 0; i < players.size(); ++i) {
            cout << "The id is " << players[i]->getID() << endl;
        }
    }

    T* whoWins() {
        double minTime = INTMAX_MAX;
        T* winner;
        for (T* player : players) {
            double time = player->MoveTo(finish);
            if (time < minTime) {
                minTime = time;
                winner = player;
            }
        }

        if (winner == nullptr) {
            throw MyException();
        } else {
            return winner;
        }
    }

    bool isThereAnyPlayers() {
        if (players.empty()) {
            return false;
        } else {
            return true;
        }
    }

private:
    Coordinate finish;
    vector<T*> players;
};