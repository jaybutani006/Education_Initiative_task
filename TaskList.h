#pragma once
#include <vector>
#include <string>
#include "Task.h"
#include "Memento.h"

class TaskList
{
public:
    void addTask(const Task &task);
    void markTaskCompleted(int taskNumber);
    void deleteTask(int taskNumber);
    void viewTasks(const std::string &filter) const;
    const std::vector<Task> &getTasks() const;
    void undo();
    void redo();

private:
    std::vector<Task> tasks;
    Caretaker caretaker;
    int caretakerIndex = -1;
    const int caretakerSize = 10;

    void saveState();
};

void markPendingTaskCompleted(TaskList &taskList);
void deleteTaskByNumber(TaskList &taskList);
