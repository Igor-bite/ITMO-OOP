#include "../../Date.cpp"
using namespace std;

class ReportViewModel {
private:
    Date makeDate;
    vector<int> resolvedTasksID;
    vector<int> changes; // храним id команд, совершённых в нужную дату

public:
    ReportViewModel() {
        makeDate = Date();
        resolvedTasksID = vector<int>();
        changes = vector<int>();
    }

    ReportViewModel(const Date &makeDate, const vector<int> &resolvedTasksId, const vector<int> &changes) : makeDate(
            makeDate), resolvedTasksID(resolvedTasksId), changes(changes) {}

    const Date &getMakeDate() const {
        return makeDate;
    }

    const vector<int> &getResolvedTasksId() const {
        return resolvedTasksID;
    }

    const vector<int> &getChanges() const {
        return changes;
    }
};