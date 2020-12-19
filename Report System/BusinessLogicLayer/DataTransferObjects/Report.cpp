#pragma once
#include "../../Date.cpp"
#include <vector>
using namespace std;

class Report {
protected:
    Date makeDate;
    int workerId;
    vector<int> resolvedTasksID;
    vector<int> changes; // храним id команд, совершённых в нужную дату

public:
    Report(int workerId) : workerId(workerId) {
        makeDate = Date();
        resolvedTasksID = vector<int>();
        changes = vector<int>();
    }

    Report(const Date &makeDate, int workerId, const vector<int> &resolvedTasksId, const vector<int> &changes)
            : makeDate(makeDate), workerId(workerId), resolvedTasksID(resolvedTasksId), changes(changes) {}

    Date getMakeDate() const {
        return makeDate;
    }

    void setMakeDate(const Date &_makeDate) {
        makeDate = _makeDate;
    }

    int getWorkerId() const {
        return workerId;
    }

    void setResolvedTasksId(const vector<int> &resolvedTasksId) {
        resolvedTasksID = resolvedTasksId;
    }

    void setWorkerId(int _workerId) {
        workerId = _workerId;
    }

    const vector<int> &getResolvedTasksId() const {
        return resolvedTasksID;
    }

    const vector<int> &getChanges() const {
        return changes;
    }
};