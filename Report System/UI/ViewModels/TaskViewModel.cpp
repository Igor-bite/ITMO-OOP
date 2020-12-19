#pragma once
using namespace std;

class TaskViewModel {
private:
    string title;
    string description;
    string state;

public:
    TaskViewModel(string _title, string _description, string _state) : title(_title), description(_description), state(_state) {}

    const string &getTitle() const {
        return title;
    }

    const string &getDescription() const {
        return description;
    }

    const int getState() const {
        if (state == "open") {
            return 2;
        } else if (state == "active") {
            return 1;
        } else if (state == "resolved") {
            return 0;
        }
        return 2;
    }
};