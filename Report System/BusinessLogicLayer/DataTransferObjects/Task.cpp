#pragma once
using namespace std;

class Task {
private:
    string title;
    string description;
    int state;
    int ID;
public:
    Task(const string &title, const string &description, int state) : title(title), description(description),
                                                                      state(state) {}

    const string &getTitle() const {
        return title;
    }

    const string &getDescription() const {
        return description;
    }

    const int &getState() const {
        return state;
    }

    int getId() const {
        return ID;
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

