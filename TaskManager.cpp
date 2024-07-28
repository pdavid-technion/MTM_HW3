#include <iostream>
#include "Task.h"
#include "Person.h"
#include "SortedList.h"
#include "TaskManager.h"

using std::cout, std::endl;

TaskManager::TaskManager() {
    this->nextTaskId = 0;
    this->employeesCount = 0;

    for (int i = 0; i < MAX_PERSONS; i++) {
        Person newPerson;
        this->employeesList[i] = newPerson;
    }
}

int TaskManager::assignTaskId() {
    int result = this->nextTaskId;
    this->nextTaskId++;
    return result;
}

void TaskManager::unassignTaskId() {
     if (nextTaskId > 0 ) {
        --nextTaskId;
    }
}

Person *TaskManager::findEmployee(const string &employeeName) {
    for (int i = 0; i < this->employeesCount; i++) {
        if ( employeesList[i].getName() == employeeName) {
            return &employeesList[i];
        }
    }

    return nullptr;
}

Person *TaskManager::findOrCreateEmployee(const string &employeeName) {
    Person *employeeToFind = findEmployee(employeeName);

    if (employeeToFind != nullptr) {
        return employeeToFind;
    }

    if (this->employeesCount >= MAX_PERSONS) {
        throw std::runtime_error("You have passed the maximum capacity for employees");
    }

    try {
        employeesList[employeesCount] = Person(employeeName);
        return &employeesList[employeesCount++];
    }
    catch (...) {
        throw;
    }
}

void TaskManager::assignTask(const string &personName, const Task &task) {
    try {
        int taskId = assignTaskId();
        Person* employeeToAssign = findOrCreateEmployee(personName);
        Task taskToAssign(task.getPriority(), task.getType(), task.getDescription());
        taskToAssign.setId(taskId);
        employeeToAssign->assignTask(taskToAssign);
    }
    catch (...) {
        this->unassignTaskId();
        throw;
    }
}

void TaskManager::completeTask(const string &personName) {
    
    Person* employeeToCompleteFor = findEmployee(personName);
    
    if (employeeToCompleteFor == nullptr) {
        return;
    }

    try {
        employeeToCompleteFor->completeTask();
    }
    catch (...) {
        throw;
    }
}

bool filterByTaskType(const Task &task, TaskType type) {
    return task.getType() == type;
}

void TaskManager::bumpPriorityByType(TaskType type, int priority) {
    if (priority <= 0) {
        return;
    }

    try {
        for (int i = 0; i < this->employeesCount; i++) {
            
            Person& currentEmployee = this->employeesList[i];
            SortedList<Task> employeeTasks = currentEmployee.getTasks();
            SortedList<Task> bumpedTasks = employeeTasks.apply([type, priority](const Task &task) {
                if (task.getType() == type) {
                    Task newTask(task.getPriority() + priority, task.getType(),
                                task.getDescription());
                    newTask.setId(task.getId());
                    return newTask;
                }
                
                return task;
            });

            currentEmployee.setTasks(bumpedTasks);
    
        }
    }
    catch (...) {
        throw;
    }
}

void TaskManager::printAllEmployees() const {

    for (int i = 0; i < this->employeesCount; i++) {
        cout << this->employeesList[i] << endl;
    }
}

void TaskManager::printAllTasks() const {

    SortedList<Task> allTasks;

    for (int i = 0; i < this->employeesCount; i++) {
        for (const Task &currentTask : this->employeesList[i].getTasks()){
                allTasks.insert(currentTask);
            }
        
    }

    for (const Task &taskToPrint : allTasks) {
        cout << taskToPrint << endl;
    }

}

void TaskManager::printTasksByType(TaskType type) const {
    SortedList<Task> allTasks;

    for (int i = 0; i < this->employeesCount; i++) {
        SortedList<Task> filteredTasks = this->employeesList[i].getTasks()
        .filter([type](const Task &task) { 
            return filterByTaskType(task, type); 
        });
        
        for (const Task &currentTask : filteredTasks) {
                allTasks.insert(currentTask);
        }
    }

    for (const Task &taskToPrint : allTasks) {
        cout << taskToPrint << endl;
    }
}