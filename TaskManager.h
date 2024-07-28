#pragma once

#include "Task.h"
#include "Person.h"
#include "SortedList.h"

using mtm::SortedList;

/**
 * @brief Class managing tasks assigned to multiple persons.
 */
class TaskManager {
private:
    /**
     * @brief Maximum number of persons the TaskManager can handle.
     */
    static const int MAX_PERSONS = 10;

     /**
     * @brief List of employees the manager handles
     */
    SortedList<Person> employeesList;

    /**
     * @brief The next Id to be allocated to a task
     */
    int nextTaskId;

/**
     * @brief Assigns the current Id and advances the field keeping it
     */
    int assignTaskId();
    void unassignTaskId();

    Person& findOrCreateEmployee(string& employeeName);
    Person& findEmployee(string& employeeName);

public:
    /**
     * @brief Default constructor to create a TaskManager object.
     *
     * Note - you may add =default if needed.
     */
    TaskManager();

    /**
     * @brief Deleted copy constructor to prevent copying of TaskManager objects.
     */
    TaskManager(const TaskManager &other) = delete;

    /**
     * @brief Deleted copy assignment operator to prevent assignment of TaskManager objects.
     */
    TaskManager &operator=(const TaskManager &other) = delete;

    /**
     * @brief Assigns a task to a person.
     *
     * @param personName The name of the person to whom the task will be assigned.
     * @param task The task to be assigned.
     */
    void assignTask(const string &personName, const Task &task);

    /**
     * @brief Completes the highest priority task assigned to a person.
     *
     * @param personName The name of the person who will complete the task.
     */
    void completeTask(const string &personName);

    /**
     * @brief Bumps the priority of all tasks of a specific type.
     *
     * @param type The type of tasks whose priority will be bumped.
     * @param priority The amount by which the priority will be increased.
     */
    void bumpPriorityByType(TaskType type, int priority);

    /**
     * @brief Prints all employees and their tasks.
     */
    void printAllEmployees() const;

    /**
     * @brief Prints all tasks of a specific type.
     *
     * @param type The type of tasks to be printed.
     */
    void printTasksByType(TaskType type) const;

    /**
     * @brief Prints all tasks assigned to all employees.
     */
    void printAllTasks() const;
};
