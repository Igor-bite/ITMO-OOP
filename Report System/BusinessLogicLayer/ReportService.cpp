#pragma once
#include "Infrastructure/IReportService.cpp"

using namespace std;

class ReportService : public IReportService {
public:
    ReportService(FileTaskRepository *taskRepo, FileWorkerRepository *workerRepo, FileReportRepository *reportRepo)
            : IReportService(taskRepo, workerRepo, reportRepo) {}

    void addToHistory(Command* command, int id) override{
        history[id] = command;
    }

    void AddLeaderFor(Worker worker, Worker forWhoom, Worker leader) override {
        addLeader* command = new addLeader(worker, taskRepo, workerRepo, forWhoom, leader);
        command->execute();
        int id = nextCommandId++;
        commandsID.push_back(id);
        addToHistory(command, id);
    }

    int AddTask(Task task, Worker worker, Worker forWhoom) override {
        addTask* command = new addTask(task, worker, forWhoom, taskRepo, workerRepo);
        command->execute();
        int id = nextCommandId++;
        addToHistory(command, id);
        return task.getId();
    }

    int MakeTask(Task task, Worker worker) override {
        makeTask* command = new makeTask(task, worker, taskRepo, workerRepo);
        command->execute();
        int id = nextCommandId++;
        addToHistory(command, id);
        return task.getId();
    }

    void DeleteTask(Task task, Worker worker) override {
        deleteTask* command = new deleteTask(task, worker, taskRepo, workerRepo);
        command->execute();
        int id = nextCommandId++;
        addToHistory(command, id);
    }

    void markTaskActive(Task task, Worker worker) override {
        markActive* command = new markActive(task, worker, taskRepo, workerRepo);
        command->execute();
        int id = nextCommandId++;
        addToHistory(command, id);
    }

    void markTaskResolved(Task task, Worker worker) override {
        markResolved* command = new markResolved(task, worker, taskRepo, workerRepo);
        command->execute();
        int id = nextCommandId++;
        addToHistory(command, id);
    }

    Task findTask(int id) override {
        TaskData taskData = taskRepo->Get(id);
        Task task = Task(taskData.getTitle(), taskData.getDescription(), taskData.getState());
        return task;
    }

    vector<Task> findTasksOf(Worker worker) override {
        vector<Task> result = vector<Task>();
        vector<TaskData> tasksData = taskRepo->GetAll();
        WorkerData _worker = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(_worker)) {
            for (TaskData task : tasksData) {
                if (task.getWorkerId() == _worker.getId()) {
                    result.push_back(Task(task.getTitle(), task.getDescription(), task.getState()));
                }
            }
        }
        return result;
    }

    void changeTaskExecutor(Task task, Worker worker) override {
        changeExecutor* command = new changeExecutor(task, worker, taskRepo, workerRepo);
        command->execute();
        int id = nextCommandId++;
        addToHistory(command, id);
    }

    void makeReport(Worker worker) {
        WorkerData _worker = WorkerData(worker.getName(), worker.getLeaderID());
        workerRepo->Find(_worker);

        ReportData rep = ReportData(_worker.getId());
        reportRepo->Create(rep);
    }

    Report finishReport(Worker worker) {
        vector<Task> tasks = findTasksOf(worker);
        vector<int> resolvedTasks = vector<int>();
        for (int i = 0; i < tasks.size(); ++i) {
            if(tasks[i].getState() == 0) {
                resolvedTasks.push_back(tasks[i].getId());
            }
        }
        WorkerData _worker = WorkerData(worker.getName(), worker.getLeaderID());
        workerRepo->Find(_worker);


        ReportData report = ReportData(_worker.getId());
        report.setResolvedTasksId(resolvedTasks);
        reportRepo->Update(report);
        Report result = Report(_worker.getId());
        result.setResolvedTasksId(resolvedTasks);
        return result;
    }

};