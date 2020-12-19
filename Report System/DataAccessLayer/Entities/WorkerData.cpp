#pragma once
#include "../Infrastructure/IEntity.cpp"

using namespace std;

class WorkerData : public IEntity {
private:
    string name;
    int leaderID;
    vector<int> juniorsID;

public:
    WorkerData(string _name, int _leaderID, vector<int> _juniorsID) : name(_name), leaderID(_leaderID), juniorsID(_juniorsID) {}

    WorkerData(string _name, int _leaderID) : name(_name), leaderID(_leaderID) {
        juniorsID = vector<int>();
    }

    const string &getName() const {
        return name;
    }

    int getLeaderId() const {
        return leaderID;
    }

    void setLeaderId(int leaderId) {
        leaderID = leaderId;
    }

    const vector<int> &getJuniorsId() const {
        return juniorsID;
    }
};

bool operator==(WorkerData t1, WorkerData t2) {
    if (t1.getName() == t2.getName() && t1.getLeaderId() == t2.getLeaderId()) {
        return true;
    } else {
        return false;
    }
}

