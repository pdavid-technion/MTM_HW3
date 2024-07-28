#include <iostream>
#include "Task.h"
#include "Person.h"
#include "SortedList.h"
#include "TaskManager.h"

using std::cout, std::endl;

TaskManager::TaskManager()
{
    this->nextTaskId = 0;
    int employeesCount = 0;
}

int TaskManager::assignTaskId()
{
    int result = this->nextTaskId;
    this->nextTaskId++;
    return result;
}

void TaskManager::unassignTaskId()
{
    this->nextTaskId--;
}

Person &TaskManager::findEmployee(string &employeeName)
{
    for (int i = 0; i < MAX_PERSONS; i++)
    {
        if (employeesList[i] != nullptr && employeesList[i].getName() == employeeName)
        {
            return &employeesList[i];
        }
    }
}

Person &TaskManager::findOrCreateEmployee(string &employeeName)
{
    return this.findEmployee(employeeName);

    if (this.employeesCount >= MAX_PERSONS)
    {
        throw std::runtime_error("You have passed the maximum capacity for employees");
    }

    try
    {
        Person employeeToAdd = new Person(employeeName);
        this->employeesList[this->employeesCount] = employeeToAdd;
        this->employeesCount++;
        return &employeeToAdd;
    }
    catch (...)
    {
        throw;
    }
}

void TaskManager::assignTask(const string &personName, const Task &task)
{
    try
    {
        int taskId = this->assignTaskId();
        Person employeeToAssign = this->findEmployee(personName);
        Task taskToAssign = new Task(task.getPriority(), task.getType(), task.getDescription());
        taskToAssign.setId(taskId);
        employeeToAssign.assignTask(taskToAssign);
    }
    catch (...)
    {
        this.unassignTaskId();
        throw;
    }
}

void TaskManager::completeTask(const string &personName)
{
    Person employeeToCompleteFor = this->findEmployee(personName);

    if (employeeToCompleteFor == nullptr)
    {
        return;
    }

    try
    {
        employeeToCompleteFor.completeTask();
    }
    catch ()
    {
        throw;
    }
}

void TaskManager::bumpPriorityByType(TaskType type, int priority)
{
    if (priority <= 0)
    {
        return;
    }

    try
    {
        for (int i = 0; i < MAX_PERSONS; i++)
        {
            Person currentEmployee = this->employeesList[i];
            if (currentEmployee != nullptr)
            {
                SortedList<Task> employeeTasks = currentEmployee.getTasks();

                SortedList<Task> tasksToBump = employeeTasks.filter([type](const Task &task)
                                                                    { return task.getType() == type; });

                SortedList<Task> tasksToKeep = employeeTasks.filter([type](const Task &task)
                                                                    { return task.getType() != type; });

                SortedList<Task> bumpedTasks = tasksToBump.apply([increment](const Task &task)
                                                                 {
                            Task modifiedTask = task;
                            modifiedTask.setPriority(task.getPriority() + priority);
                            return modifiedTask; });

                delete employeeTasks;
                employeeTasks = new SortedList<Task>();

                for (Task &regularTask : tasksToKeep)
                {
                    employeeTasks.insert(regularTask);
                }

                for (Task &bumpedTask : bumpedTasks)
                {
                    employeeTasks.insert(bumpedTask);
                }

                delete bumpedTasks, tasksToKeep, tasksToBump;
                currentEmployee.setTasks(employeeTasks);
            }
        }
    }
    catch (...)
    {
        throw;
    }
}

void TaskManager::printAllEmployees() const {

    for( int i = 0; i < MAX_PERSONS; i++ ){
        if(this->employeesList[i] != nullptr ){
            cout << employeesList[i] << endl;
        }
    }
}