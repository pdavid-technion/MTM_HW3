#include "Person.h"
#include "SortedList.h"
#include "TaskManager.h"


TaskManager::TaskManager(){
    this->nextTaskId = 0;
    this->employeesList = new SortedList<Person>();
}