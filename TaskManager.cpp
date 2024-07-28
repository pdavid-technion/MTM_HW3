#include "Task.h"
#include "Person.h"
#include "SortedList.h"
#include "TaskManager.h"

TaskManager::TaskManager()
{
    this->nextTaskId = 0;
    this->employeesList = new SortedList<Person>();
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
    for (Person &currentEmployee : *this->employeesList)
    {
        if (currentEmployee.getName() == employeeName)
        {
            return &currentEmployee;
        }
    }
}

Person &TaskManager::findOrCreateEmployee(string &employeeName)
{
    return this.findEmployee(employeeName);

    if (this->nextTaskId >= MAX_PERSONS)
    {
        throw std::runtime_error("You have passed the maximum capacity for employees");
    }

    try
    {
        Person employeeToAdd = new Person(employeeName);
        this->employeesList.insert(employeeToAdd);
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