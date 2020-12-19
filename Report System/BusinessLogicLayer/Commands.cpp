#include "DataTransferObjects/Task.cpp"
#include "DataTransferObjects/Worker.cpp"
using namespace std;

class Command {
protected:
    Worker worker;
    FileTaskRepository *taskRepo;
    FileWorkerRepository *workerRepo;

public:
    Command(const Worker &workerData, FileTaskRepository *taskRepo,
            FileWorkerRepository *workerRepo) : worker(workerData), taskRepo(taskRepo),
                                                workerRepo(workerRepo) {}

    virtual void execute() = 0;

    virtual void undo() = 0;

    const Worker &getWorker() const {
        return worker;
    }
};

class addLeader : public Command {
private:
    Worker forWhoom;
    Worker newLeader;
    int prevLeaderID;
public:
    addLeader(const Worker &workerData, FileTaskRepository *taskRepo,
              FileWorkerRepository *workerRepo, const Worker &forWhoom, const Worker &newLeader) : Command(workerData, taskRepo, workerRepo), forWhoom(forWhoom),
                                                                 newLeader(newLeader) {}

    void execute() override {
        WorkerData _worker = WorkerData(worker.getName(), worker.getLeaderID());
        WorkerData _forWhoom = WorkerData(forWhoom.getName(), forWhoom.getLeaderID());
        WorkerData _leader = WorkerData(newLeader.getName(), newLeader.getLeaderID());

        if (workerRepo->Find(_worker) && workerRepo->Find(_leader) && workerRepo->Find(_forWhoom)) {
            prevLeaderID = _forWhoom.getLeaderId();
            _forWhoom.setLeaderId(_leader.getId());
            workerRepo->Update(_forWhoom);
        }
    }

    void undo() override {
        WorkerData _worker = WorkerData(worker.getName(), worker.getLeaderID());
        WorkerData _forWhoom = WorkerData(forWhoom.getName(), forWhoom.getLeaderID());
        WorkerData _leader = WorkerData(newLeader.getName(), newLeader.getLeaderID());
        if (workerRepo->Find(_worker) && workerRepo->Find(_leader) && workerRepo->Find(_forWhoom)) {
            _forWhoom.setLeaderId(prevLeaderID);
            workerRepo->Update(_forWhoom);
        }
    }
};

class addTask : public Command {
private:
    Task task;
    Worker forWhoom;
public:
    addTask(const Task &taskData, const Worker &workerData, const Worker &forWhoomData, FileTaskRepository *taskRepo,
             FileWorkerRepository *workerRepo) : Command(workerData, taskRepo, workerRepo), task(taskData), forWhoom(forWhoomData) {}

    void execute() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        WorkerData _forWhoom = WorkerData(forWhoom.getName(), forWhoom.getLeaderID());

        if (workerRepo->Find(workerData) && workerRepo->Find(_forWhoom)) {
            taskData.setWorkerId(_forWhoom.getId());
            taskRepo->Create(taskData);
        }
    }

    void undo() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());

        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            taskRepo->Delete(taskData);
        }
    }
};

class makeTask : public Command {
private:
    Task task;
public:
    makeTask(const Task &taskData, const Worker &workerData, FileTaskRepository *taskRepo,
             FileWorkerRepository *workerRepo) : Command(workerData, taskRepo, workerRepo), task(taskData) {}

    void execute() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData)) {
            taskData.setWorkerId(workerData.getId());
            taskRepo->Create(taskData);
        }
    }

    void undo() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            taskRepo->Delete(taskData);
        }
    }
};

class deleteTask : public Command {
private:
    Task task;
public:
    deleteTask(const Task &taskData, const Worker &workerData, FileTaskRepository *taskRepo,
               FileWorkerRepository *workerRepo) : Command(workerData, taskRepo, workerRepo), task(taskData) {}

    void execute() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            taskRepo->Delete(taskData);
        }
    }

    void undo() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            taskData.setWorkerId(workerData.getId());
            taskRepo->Create(taskData);
        }
    }
};

class markOpened : public Command {
private:
    Task task;
    int prevState;
public:
    markOpened(const Task &taskData, const Worker &workerData, FileTaskRepository *taskRepo,
               FileWorkerRepository *workerRepo) : Command(workerData, taskRepo, workerRepo), task(taskData) {}

    void execute() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            prevState = taskData.getState();
            taskData.setOpened();
            taskRepo->Update(taskData);
        }
    }

    void undo() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            taskData.setState(prevState);
            taskRepo->Update(taskData);
        }
    }
};

class markActive : public Command {
private:
    Task task;
    int prevState;
public:
    markActive(const Task &taskData, const Worker &workerData, FileTaskRepository *taskRepo,
               FileWorkerRepository *workerRepo) : Command(workerData, taskRepo, workerRepo), task(taskData) {}

    void execute() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            prevState = taskData.getState();
            taskData.setActive();
            taskRepo->Update(taskData);
        }
    }

    void undo() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            taskData.setState(prevState);
            taskRepo->Update(taskData);
        }
    }
};

class markResolved : public Command {
private:
    Task task;
    int prevState;
public:
    markResolved(const Task &taskData, const Worker &workerData, FileTaskRepository *taskRepo,
                 FileWorkerRepository *workerRepo) : Command(workerData, taskRepo, workerRepo), task(taskData) {}

    void execute() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            prevState = taskData.getState();
            taskData.setResolved();
            taskRepo->Update(taskData);
        }
    }

    void undo() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            taskData.setState(prevState);
            taskRepo->Update(taskData);
        }
    }
};

class changeExecutor : public Command {
private:
    int prevExecutorId;
    Task task;
public:
    changeExecutor(const Task &taskData, const Worker &workerData, FileTaskRepository *taskRepo,
                   FileWorkerRepository *workerRepo) : Command(workerData, taskRepo, workerRepo), task(taskData) {}

    void execute() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            prevExecutorId = taskData.getWorkerId();
            taskData.setWorkerId(workerData.getId());
            taskRepo->Update(taskData);
        }
    }

    void undo() override {
        TaskData taskData = TaskData(task.getTitle(), task.getDescription(), task.getState());
        WorkerData workerData = WorkerData(worker.getName(), worker.getLeaderID());
        if (workerRepo->Find(workerData) && taskRepo->Find(taskData)) {
            taskData.setWorkerId(prevExecutorId);
            taskRepo->Update(taskData);
        }
    }
};
