#include <vector>
#include "Race.cpp"
using namespace std;

class RaceEngine {
public:
    RaceEngine() = default;

    void addGeneralRace(Race<Transport> r) {
        genRace.push_back(r);
    }

    void addAirRace(Race<AirTransport> r) {
        airRace.push_back(r);
    }

    void addGroundRace(Race<GroundTransport> r) {
        grRace.push_back(r);
    }

    void getInfo() {
        for (int i = 0; i < genRace.size(); ++i) {
            genRace[i].printAllPlayersIds();
        }
        for (int i = 0; i < airRace.size(); ++i) {
            airRace[i].printAllPlayersIds();
        }
        for (int i = 0; i < grRace.size(); ++i) {
            grRace[i].printAllPlayersIds();
        }
    }

    void allWinners() {
        cout << "In general races: " << endl;
        for (int i = 0; i < genRace.size(); ++i) {
            if (genRace[i].isThereAnyPlayers()) {
                cout << i + 1 << ") ";
                try {
                    cout << "Winner has id " << genRace[i].whoWins()->getID() << endl;
                } catch(MyException e) {
                    cout << e.what() << endl;
                }
            } else {
                cout << i + 1 << ") There are no players" << endl;
            }
        }
        cout << "In Air races: " << endl;
        for (int i = 0; i < airRace.size(); ++i) {
            if (airRace[i].isThereAnyPlayers()) {
                cout << i + 1 << ") ";
                try {
                    cout << "Winner has id " << airRace[i].whoWins()->getID() << endl;
                } catch(MyException e) {
                    cout << e.what() << endl;
                }
            } else {
                cout << i + 1 << ") There are no players" << endl;
            }
        }
        cout << "In Ground races: " << endl;
        for (int i = 0; i < grRace.size(); ++i) {
            if (grRace[i].isThereAnyPlayers()) {
                cout << i + 1 << ") ";
                try {
                    cout << "Winner has id " << grRace[i].whoWins()->getID() << endl;
                } catch(MyException e) {
                    cout << e.what() << endl;
                }
            } else {
                cout << i + 1 << ") There are no players" << endl;
            }
        }
    }

 private:
    vector<Race<Transport>> genRace = vector<Race<Transport>>();
    vector<Race<AirTransport>> airRace = vector<Race<AirTransport>>();
    vector<Race<GroundTransport>> grRace = vector<Race<GroundTransport>>();
};