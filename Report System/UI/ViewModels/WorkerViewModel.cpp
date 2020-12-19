using namespace std;

class WorkerViewModel {
private:
    string name;
    int leaderID;
public:
    WorkerViewModel(string _name, int _leaderId) : name(_name), leaderID(_leaderId) {}

    WorkerViewModel(const string &name) : name(name) {
        leaderID = 0;
    }

    const string &getName() const {
        return name;
    }

    int getLeaderId() const {
        return leaderID;
    }
};