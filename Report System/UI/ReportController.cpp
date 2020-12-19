#include "ViewModels/TaskViewModel.cpp"
#include "ViewModels/WorkerViewModel.cpp"
#include "ViewModels/ReportViewModel.cpp"

using namespace std;

class ReportController {
private:
    ReportService reportService;
public:
    ReportController(ReportService service) : reportService(service) {}

    void addLeaderFor(const WorkerViewModel &worker, const WorkerViewModel &forWhoom, const WorkerViewModel &leader) {
        Worker _worker = Worker(worker.getName(), worker.getLeaderId());
        Worker _forWhoom = Worker(forWhoom.getName(), forWhoom.getLeaderId());
        Worker _leader = Worker(leader.getName(), leader.getLeaderId());

        reportService.AddLeaderFor(_worker, _forWhoom, _leader);
    }

    int makeNewTask(TaskViewModel &task, WorkerViewModel &worker) {
        Task _task = Task(task.getTitle(), task.getDescription(), task.getState());
        Worker _worker = Worker(worker.getName(), worker.getLeaderId());

        return reportService.MakeTask(_task, _worker);
    }

    int addTaskFor(TaskViewModel &task, WorkerViewModel &worker, WorkerViewModel &forWhoom) {
        Task _task = Task(task.getTitle(), task.getDescription(), task.getState());
        Worker _worker = Worker(worker.getName(), worker.getLeaderId());
        Worker _forWhoom = Worker(forWhoom.getName(), forWhoom.getLeaderId());

        return reportService.AddTask(_task, _worker, _forWhoom);
    }

    void markTaskActive(TaskViewModel &task, WorkerViewModel &worker) {
        Task _task = Task(task.getTitle(), task.getDescription(), task.getState());
        Worker _worker = Worker(worker.getName(), worker.getLeaderId());

        reportService.markTaskActive(_task, _worker);
    }

    void markTaskResolved(TaskViewModel &task, WorkerViewModel &worker) {
        Task _task = Task(task.getTitle(), task.getDescription(), task.getState());
        Worker _worker = Worker(worker.getName(), worker.getLeaderId());

        reportService.markTaskResolved(_task, _worker);
    }

    TaskViewModel findTask(int id) {
        Task task = reportService.findTask(id);
        string state;
        switch (task.getState()) {
            case 2:
                state = "open";
                break;
            case 1:
                state = "active";
                break;
            case 0:
                state = "resolved";
                break;
            default:
                throw TaskStateException();
                break;
        }
        return TaskViewModel(task.getTitle(), task.getDescription(), state);
    }

    vector<TaskViewModel> findTasksOf(WorkerViewModel &worker) {
        Worker _worker = Worker(worker.getName(), worker.getLeaderId());
        vector<Task> tasks = reportService.findTasksOf(_worker);
        vector<TaskViewModel> tasksViews = vector<TaskViewModel>();
        for (Task t : tasks) {
            string state;
            switch (t.getState()) {
                case 2:
                    state = "open";
                    break;
                case 1:
                    state = "active";
                    break;
                case 0:
                    state = "resolved";
                    break;
                default:
                    throw TaskStateException();
                    break;
            }
            tasksViews.push_back(TaskViewModel(t.getTitle(), t.getDescription(), state));
        }
        return tasksViews;
    }

    void changeTaskExecutor(TaskViewModel &task, WorkerViewModel &worker) {
        Task _task = Task(task.getTitle(), task.getDescription(), task.getState());
        Worker _worker = Worker(worker.getName(), worker.getLeaderId());

        reportService.changeTaskExecutor(_task, _worker);
    }

    void makeReport(WorkerViewModel worker) {
        Worker _worker = Worker(worker.getName(), worker.getLeaderId());

        reportService.makeReport(_worker);
    }

    ReportViewModel finishReport(WorkerViewModel worker) {
        Worker _worker = Worker(worker.getName(), worker.getLeaderId());
        Report rep = reportService.finishReport(_worker);

        ReportViewModel repView = ReportViewModel(rep.getMakeDate(), rep.getResolvedTasksId(), rep.getChanges());
        return repView;
    }
};