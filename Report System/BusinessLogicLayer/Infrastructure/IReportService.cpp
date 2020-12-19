#pragma once
#include "../DataTransferObjects/Report.cpp"
#include "../DataTransferObjects/Task.cpp"
#include "../DataTransferObjects/Worker.cpp"
#include "../../DataAccessLayer/Repositories.cpp"
#include "../Commands.cpp"
#include <map>

class IReportService {
protected:
    FileTaskRepository *taskRepo;
    FileWorkerRepository *workerRepo;
    FileReportRepository *reportRepo;

    map<int, Command*> history; // map<id, command>
    vector<int> commandsID;
    int nextCommandId;
public:
    IReportService(FileTaskRepository *taskRepo, FileWorkerRepository *workerRepo, FileReportRepository *reportRepo)
    : taskRepo(taskRepo), workerRepo(workerRepo), reportRepo(reportRepo) {
        history = map<int, Command*>();
        commandsID = vector<int>();
        nextCommandId = 1;
    }

    virtual void addToHistory(Command* command, int id) = 0;

    virtual void AddLeaderFor(Worker worker, Worker forWhoom, Worker leader) = 0;

    virtual int AddTask(Task task, Worker worker, Worker forWhoom) = 0;

    virtual int MakeTask(Task task, Worker worker) = 0;

    virtual void DeleteTask(Task task, Worker worker) = 0;

    virtual void markTaskActive(Task task, Worker worker) = 0;

    virtual void markTaskResolved(Task task, Worker worker) = 0;

    virtual Task findTask(int id) = 0;

    virtual vector<Task> findTasksOf(Worker worker) = 0;

//    virtual vector<Task> findTasksChangedBy(Worker worker) = 0;

    virtual void changeTaskExecutor(Task task, Worker worker) = 0;
};