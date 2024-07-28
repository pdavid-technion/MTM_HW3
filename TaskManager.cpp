#include <iostream>
#include "Task.h"
#include "Person.h"
#include "SortedList.h"
#include "TaskManager.h"

using std::cout, std::endl;

TaskManager::TaskManager()
{
    this->nextTaskId = 0;
    this->employeesCount = 0;
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

Person* TaskManager::findEmployee(const string &employeeName)
{
    for (int i = 0; i < MAX_PERSONS; i++)
    {
        if (employeesList[i] != nullptr && employeesList[i]->getName() == employeeName)
        {
            return employeesList[i];
        }
    }

    return nullptr;
}

Person& TaskManager::findOrCreateEmployee(const string &employeeName)
{
    Person* employeeToFind = this->findEmployee(employeeName);
    
    if( employeeToFind != nullptr ){
        return *employeeToFind;
    }

    if (this->employeesCount >= MAX_PERSONS)
    {
        throw std::runtime_error("You have passed the maximum capacity for employees");
    }

    try
    {
        Person *employeeToAdd = new Person(employeeName);
        this->employeesList[this->employeesCount] = employeeToAdd;
        this->employeesCount++;
        return *employeeToAdd;
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
        Person employeeToAssign = this->findOrCreateEmployee(personName);
        Task taskToAssign(task.getPriority(), task.getType(), task.getDescription());
        taskToAssign.setId(taskId);
        employeeToAssign.assignTask(taskToAssign);

  
    }
    catch (...)
    {
        this->unassignTaskId();
        throw;
    }
}

void TaskManager::completeTask(const string &personName)
{
    Person* employeeToCompleteFor = this->findEmployee(personName);

    if (employeeToCompleteFor == nullptr)
    {
        return;
    }

    try
    {
        employeeToCompleteFor->completeTask();
    }
    catch (...)
    {
        throw;
    }
}
bool filterByTaskType(const Task &task, TaskType type)
{
    return task.getType() == type;
}
bool filterByNotTaskType(const Task &task, TaskType type)
{
    return task.getType() != type;
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
            Person *currentEmployee = this->employeesList[i];
            if (currentEmployee != nullptr)
            {
                SortedList<Task> employeeTasks = currentEmployee->getTasks();

                SortedList<Task> tasksToBump = employeeTasks.filter(
                    [type](const Task &task)
                    { return filterByTaskType(task, type); });

                SortedList<Task> tasksToKeep = employeeTasks.filter(
                    [type](const Task &task)
                    { return filterByNotTaskType(task, type); });

                SortedList<Task> 
                    bumpedTasks = tasksToBump.apply([priority](const Task &task)
                                                    {
                            Task modifiedTask(task.getPriority() + priority, task.getType(), task.getDescription());
                            modifiedTask.setId(task.getId());
                            return modifiedTask; });

                // delete employeeTasks;
                employeeTasks = tasksToKeep;

                for (const Task &bumpedTask : bumpedTasks)
                {
                    employeeTasks.insert(bumpedTask);
                }

                currentEmployee->setTasks(employeeTasks);

                // delete tasksToBump;
                // delete bumpedTasks;
                // delete tasksToKeep;
            }
        }
    }
    catch (...)
    {
        throw;
    }
}

void TaskManager::printAllEmployees() const
{

    for (int i = 0; i < MAX_PERSONS; i++)
    {
        if (this->employeesList[i] != nullptr)
        {
            cout << *this->employeesList[i] << endl;
        }
    }
}

void TaskManager::printAllTasks() const
{

    SortedList<Task> allTasks;

    for (int i = 0; i < MAX_PERSONS; i++)
    {
        if (this->employeesList[i] != nullptr)
        {
            for (const Task &currentTask : this->employeesList[i]->getTasks())
            {
                allTasks.insert(currentTask);
            }
        }
    }

    for (const Task &taskToPrint : allTasks)
    {
        cout << taskToPrint << endl;
    }

    //delete allTasks;
}

void TaskManager::printTasksByType(TaskType type) const
{
    SortedList<Task> allTasks;

    for (int i = 0; i < MAX_PERSONS; i++)
    {
        if (this->employeesList[i] != nullptr)
        {
            SortedList<Task> filteredTasks = this->employeesList[i]->getTasks().filter([type](const Task &task)
                    { return filterByTaskType(task, type); });

            for (const Task &currentTask : filteredTasks)
            {
                allTasks.insert(currentTask);
            }

            //delete filteredTasks;
        }
    }

    for (const Task &taskToPrint : allTasks)
    {
        cout << taskToPrint << endl;
    }

    //delete allTasks;
}