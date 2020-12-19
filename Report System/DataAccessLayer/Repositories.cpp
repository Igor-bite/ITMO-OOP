#pragma once
#include "Infrastructure/IRepository.cpp"
#include "Entities/TaskData.cpp"
#include "Entities/WorkerData.cpp"
#include "Entities/ReportData.cpp"

using namespace std;

class FileTaskRepository : public IRepository<TaskData> {
public:
    FileTaskRepository(string _path) : IRepository<TaskData>(_path) {}

    /*
     * id.txt:
     * title
     * description
     * state
     */
    TaskData Get(int id) override {
        ifstream in(path + to_string(id) + ".txt");

        string title, description;
        string stateStr;
        int state;

        getline(in, title);
        getline(in, description);
        getline(in, stateStr);
        state = stoi(stateStr);

        return TaskData(title, description, state);
    }

    vector<TaskData> GetAll() {
        vector<TaskData> result = vector<TaskData>();
        for (int i = 0; i < availableIDs.size(); ++i) {
            result.push_back(Get(availableIDs[i]));
        }
        return result;
    }

    void Create(TaskData &item) override {
        item.setId(nextID++);
        ofstream out(path + item.getFilename());

        out << item.getTitle() << endl;
        out << item.getDescription() << endl;
        out << item.getState() << endl;

        out.close();
    }

    void Update(const TaskData &item) override {
        ofstream out(path + item.getFilename());

        out << item.getTitle() << endl;
        out << item.getDescription() << endl;
        out << item.getState() << endl;

        out.close();
    }

    bool Find(TaskData &item) override {
        for(int id : availableIDs) {
            TaskData cur = Get(id);
            if (cur == item) {
                item.setId(id);
                return true;
            }
        }
        return false;
    }
};

class FileWorkerRepository : public IRepository<WorkerData> {
public:
    FileWorkerRepository(string _path) : IRepository<WorkerData>(_path) {}
    /*
     * id.txt:
     * name
     * leaderID
     * numberOfJuniors
     * j
     * u
     * n
     * i
     * o
     * r
     * s
     * I
     * d
     */

    WorkerData Get(int id) override {
        ifstream in(path + to_string(id) + ".txt");

        string name;
        string leaderIDStr;
        int leaderID;
        string junNumStr;
        int juniorsNumber;
        vector<int> juniorsID = vector<int>();

        getline(in, name);
        getline(in, leaderIDStr);
        if (leaderIDStr == "-") {
            leaderID = 0;
        } else {
            leaderID = stoi(leaderIDStr);
        }

        getline(in, junNumStr);
        juniorsNumber = stoi(junNumStr);
        for(int i = 0; i < juniorsNumber; ++i) {
            int id;
            in >> id;
            juniorsID.push_back(id);
        }
        in.close();
        WorkerData worker = WorkerData(name, leaderID, juniorsID);
        worker.setId(id);
        return worker;
    }

    void Create(WorkerData &item) override {
        item.setId(nextID++);
        ofstream out(path + item.getFilename());

        out << item.getName() << endl;
        out << item.getLeaderId() << endl;

        vector<int> getJuniorsId = item.getJuniorsId();
        int junNum = getJuniorsId.size();
        out << junNum << endl;
        for(int i = 0; i < junNum; ++i) {
            out << getJuniorsId[i] << endl;
        }

        out.close();
    }

    void Update(const WorkerData &item) override {
        ofstream out(path + item.getFilename());

        out << item.getName() << endl;
        out << item.getLeaderId() << endl;
        int juniorsNumber = item.getJuniorsId().size();
        out << item.getJuniorsId().size() << endl;
        for (int i = 0; i < juniorsNumber; ++i) {

        }

        out.close();
    }

    bool Find(WorkerData &item) override {
        for(int id : availableIDs) {
            WorkerData cur = Get(id);
            if (cur == item) {
                item = cur;
                return true;
            }
        }
        return false;
    }
};

class FileReportRepository : public IRepository<ReportData> {
public:
    FileReportRepository(string _path) : IRepository<ReportData>(_path) {}

    /*
     * id.txt:
     * sec
     * min
     * hour
     * day
     * month
     * year
     * workerID
     * numberOfResolvedTasks
     * resolvedTasks(id)...
     * numberOfChanges
     * changes(id)...
     */

    ReportData Get(int id) override {
        ifstream in(path + to_string(id) + ".txt");

        Date makeDate;
        string dayStr, monthStr, yearStr, hourStr, minStr, secStr;
        int day, month, year, hour, min, sec;
        string workerIdStr;
        int workerId;

        string numberOfResolvedTasksStr;
        int numberOfResolvedTasks;
        vector<int> resolvedTasksID = vector<int>();
        string numberOfChangesStr;
        int numberOfChanges;
        vector<int> changes = vector<int>();

        getline(in, secStr);
        sec = stoi(secStr);
        getline(in, minStr);
        min = stoi(minStr);
        getline(in, hourStr);
        hour = stoi(hourStr);
        getline(in, dayStr);
        day = stoi(dayStr);
        getline(in, monthStr);
        month = stoi(monthStr);
        getline(in, yearStr);
        year = stoi(yearStr);
        makeDate = Date(day, month, year, hour, min, sec);

        getline(in, workerIdStr);
        workerId = stoi(workerIdStr);

        getline(in, numberOfResolvedTasksStr);
        numberOfResolvedTasks = stoi(numberOfResolvedTasksStr);
        for(int i = 0; i < numberOfResolvedTasks; ++i) {
            int id;
            in >> id;
            resolvedTasksID.push_back(id);
        }

        getline(in, numberOfChangesStr);
        numberOfChanges = stoi(numberOfChangesStr);
        for(int i = 0; i < numberOfChanges; ++i) {
            int id;
            in >> id;
            changes.push_back(id);
        }

        ReportData worker = ReportData(makeDate, workerId, resolvedTasksID, changes);
        worker.setId(id);
        return worker;
    }

    void Create(ReportData &item) override {
        item.setId(nextID++);
        ofstream out(path + item.getFilename());

        out << item.getMakeDate().getTime().getSec() << endl;
        out << item.getMakeDate().getTime().getMin() << endl;
        out << item.getMakeDate().getTime().getHour() << endl;

        out << item.getMakeDate().getDay()<< endl;
        out << item.getMakeDate().getMonth() << endl;
        out << item.getMakeDate().getYear() << endl;

        out << item.getWorkerId() << endl;

        vector<int> ResolvedTasks = item.getResolvedTasksId();
        int resTasksNum = ResolvedTasks.size();
        out << resTasksNum << endl;
        for(int i = 0; i < resTasksNum; ++i) {
            out << ResolvedTasks[i] << endl;
        }

        vector<int> changes = item.getChanges();
        int changesNum = changes.size();
        out << changesNum << endl;
        for(int i = 0; i < changesNum; ++i) {
            out << changes[i] << endl;
        }

        out.close();
    }

    void Update(const ReportData &item) override {
        ofstream out(path + item.getFilename());

        out << item.getMakeDate().getTime().getSec() << endl;
        out << item.getMakeDate().getTime().getMin() << endl;
        out << item.getMakeDate().getTime().getHour() << endl;

        out << item.getMakeDate().getDay()<< endl;
        out << item.getMakeDate().getMonth() << endl;
        out << item.getMakeDate().getYear() << endl;

        out << item.getWorkerId() << endl;

        vector<int> ResolvedTasks = item.getResolvedTasksId();
        int resTasksNum = ResolvedTasks.size();
        out << resTasksNum << endl;
        for(int i = 0; i < resTasksNum; ++i) {
            out << ResolvedTasks[i] << endl;
        }

        vector<int> changes = item.getChanges();
        int changesNum = changes.size();
        out << changesNum << endl;
        for(int i = 0; i < changesNum; ++i) {
            out << changes[i] << endl;
        }

        out.close();
    }

    bool Find(ReportData &item) override {
        for(int id : availableIDs) {
            ReportData cur = Get(id);
            if (cur == item) {
                item = cur;
                return true;
            }
        }
        return false;
    }

    ReportData Find(WorkerData &worker) {
        for(int id : availableIDs) {
            ReportData cur = Get(id);
            if (cur.getWorkerId() == worker.getId()) {
                return cur;
            }
        }
        throw NoReportException();
    }
};