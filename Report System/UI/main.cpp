#include <iostream>
#include "../BusinessLogicLayer/ReportService.cpp"
#include "ViewModels/TaskViewModel.cpp"
#include "ReportController.cpp"
using namespace std;

int main() {
    string tasksFile = "/Users/igorklyuzev/Desktop/Tasks/";
    string workersFile = "/Users/igorklyuzev/Desktop/Workers/";
    string reportsFile = "/Users/igorklyuzev/Desktop/Reports/";

    FileTaskRepository *taskRepo = new FileTaskRepository(tasksFile);
    FileWorkerRepository *workerRepo = new FileWorkerRepository(workersFile);
    FileReportRepository *reportRepo = new FileReportRepository(reportsFile);
    ReportService reportService = ReportService(taskRepo, workerRepo, reportRepo);
    ReportController controller = ReportController(reportService);

    WorkerViewModel teamLead = WorkerViewModel("TeamLead");
    WorkerViewModel leader1 = WorkerViewModel("Leader1");
    WorkerViewModel leader2 = WorkerViewModel("Leader2");
    WorkerViewModel worker1 = WorkerViewModel("Worker1");
    WorkerViewModel worker2 = WorkerViewModel("Worker2");
    WorkerViewModel worker3 = WorkerViewModel("Worker3");

    controller.addLeaderFor(teamLead, worker1, leader1);
    controller.addLeaderFor(teamLead, worker2, leader1);
    controller.addLeaderFor(teamLead, worker3, leader2);
    controller.addLeaderFor(teamLead, leader1, teamLead);
    controller.addLeaderFor(teamLead, leader2, teamLead);

    TaskViewModel task1 = TaskViewModel("do the work", "very important", "opened");

    controller.makeNewTask(task1, worker1);

    vector<TaskViewModel> tasks = controller.findTasksOf(worker1);
    controller.makeReport(worker1);
    ReportViewModel report = controller.finishReport(worker1);

    return 0;
}
