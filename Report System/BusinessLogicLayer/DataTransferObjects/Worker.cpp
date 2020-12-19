#pragma once
#include <vector>
using namespace std;

class Worker {
private:
    string name;
    int leaderID;
    vector<int> juniorsID;
public:
    Worker(string _name, int _leaderID, vector<int> _juniorsID, vector<int> _tasksID) : name(_name), leaderID(_leaderID),
                                                                                    juniorsID(_juniorsID) {}

    Worker(const string &name, int leaderId) : name(name), leaderID(leaderId) {}

    string getName() const {
        return name;
    }

    int getLeaderID() const {
        return leaderID;
    };

//    bool isYours(int id) {
//
//    }

    bool hasJuniors() {
        if (juniorsID.empty()) {
            return false;
        } else {
            return true;
        }
    }

    int getLeaderId() const {
        return leaderID;
    }

    const vector<int> &getJuniorsId() const {
        return juniorsID;
    }

};