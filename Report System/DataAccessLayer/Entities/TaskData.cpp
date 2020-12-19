#pragma once
#include "../Infrastructure/IEntity.cpp"
using namespace std;

class TaskData : public IEntity {
private:
    string title;
    string description;
    int state; // 0 - resolved, 1 - active, 2 - open
    int workerId;
    /*
     * Open - задача создана, но к её выполнению ещё не приступили.
     * Active - задача находится в процессе выполнения.
     * Resolved - задача выполнена.
     */
public:
    TaskData(string _title, string descr, int _state) : title(_title), description(descr), state(_state) {}

    const string &getTitle() const {
        return title;
    }

    int getState() const {
        return state;
    }

    const string &getDescription() const {
        return description;
    }

    string getFilename() const {
        return to_string(ID) + ".txt";
    }

    int getWorkerId() const {
        return workerId;
    }

    void setWorkerId(int workerId) {
        TaskData::workerId = workerId;
    }

    void setOpened() {
        state = 2;
    }

    void setActive() {
        state = 1;
    }

    void setResolved() {
        state = 0;
    }

    void setState(int _state) {
        state = _state;
    }
};

bool operator==(TaskData t1, TaskData t2) {
    if (t1.getTitle() == t2.getTitle() && t1.getDescription() == t2.getDescription()) {
        return true;
    } else {
        return false;
    }
}

