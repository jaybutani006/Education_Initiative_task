#pragma once
#include <vector>
#include <string>
#include "Task.h"
#include "Memento.h"
using namespace std;

class TaskList
{
public:
    void addTask(const Task &task);
    void markTaskCompleted(int taskNumber);
    void deleteTask(int taskNumber);
    void viewTasks(const string &filter) const;
    const vector<Task> &getTasks() const;
    void undo();
    void redo();

private:
    vector<Task> tasks;
    Caretaker caretaker;
    int caretakerIndex = -1;
    const int caretakerSize = 10;

    void saveState();
    
};

void markPendingTaskCompleted(TaskList &taskList);
void deleteTaskByNumber(TaskList &taskList);
