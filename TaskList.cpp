#include "TaskList.h"
#include <iostream>
#include <sstream>
using namespace std;

void TaskList::addTask(const Task &task)
{
    if (task.getDueDate().empty())
    {
        tasks.push_back(Task::Builder(task.getDescription())
                            .setCompleted(task.isCompleted())
                            .setDueDate("")
                            .build());
    }
    else
    {
        tasks.push_back(task);
    }
    saveState();
}


void TaskList::markTaskCompleted(int taskNumber)
{
    int pendingTaskCount = 0;
    for (Task &task : tasks)
    {
        if (!task.isCompleted())
        {
            pendingTaskCount++;
            if (pendingTaskCount == taskNumber)
            {
                std::string originalDueDate = task.getDueDate();

                task = Task::Builder(task.getDescription())
                           .setCompleted(true)
                           .setDueDate(originalDueDate)
                           .build();
                saveState();
                return;
            }
        }
    }
}

void TaskList::deleteTask(int taskNumber)
{
    if (taskNumber >= 1 && taskNumber <= tasks.size())
    {
        tasks.erase(tasks.begin() + taskNumber - 1);
        cout << "Task deleted successfully." << endl;
        saveState();
    }
    else
    {
        cout << "Invalid task number. No task deleted." << endl;
    }
}

void TaskList::viewTasks(const string &filter) const
{
    cout << endl;
    for (const Task &task : tasks)
    {
        if (filter == "Show all" || (filter == "Show completed" && task.isCompleted()) || (filter == "Show pending" && !task.isCompleted()))
        {
            cout << task.getDescription() << " - " << (task.isCompleted() ? "Completed" : "Pending");
            if (!task.getDueDate().empty())
            {
                cout << ", Due: " << task.getDueDate();
            }
            else
            {
                cout << ", Due: N/A";
            }
            cout << endl;
        }
    }
    cout << endl;
}

const vector<Task> &TaskList::getTasks() const
{
    return tasks;
}

void TaskList::undo()
{
    if (caretakerIndex > 0)
    {
        caretakerIndex--;
        const std::vector<std::string> &state = caretaker.getMemento(caretakerIndex).getState();

        if (state.size() == tasks.size())
        {
            int lastCompletedIndex = -1;
            for (size_t i = 0; i < tasks.size(); i++)
            {
                if (tasks[i].isCompleted())
                {
                    lastCompletedIndex = static_cast<int>(i);
                }
            }

            for (size_t i = 0; i < tasks.size(); i++)
            {
                Task::Builder builder(state[i]);
                Task task = builder.build();
                if (task.isCompleted())
                {
                    tasks[i] = Task::Builder(tasks[i].getDescription())
                                   .setDueDate(tasks[i].getDueDate())
                                   .setCompleted(task.isCompleted())
                                   .build();
                }
                else
                {
                    tasks[i] = Task::Builder(tasks[i].getDescription())
                                   .setDueDate("")
                                   .setCompleted(task.isCompleted())
                                   .build();
                }
            }

            if (lastCompletedIndex >= 0)
            {
                tasks[lastCompletedIndex] = Task::Builder(tasks[lastCompletedIndex].getDescription())
                                                .setDueDate(tasks[lastCompletedIndex].getDueDate())
                                                .setCompleted(false)
                                                .build();
            }

            cout << "Undo operation performed." << endl;
        }
        else
        {
            cout << "Undo operation failed. State data mismatch." << endl;
        }
    }
    else
    {
        cout << "Nothing to undo." << endl;
    }
}

void TaskList::redo()
{
    if (caretakerIndex < caretaker.getMementoCount() - 1)
    {
        caretakerIndex++;
        const vector<string> &state = caretaker.getMemento(caretakerIndex).getState();

        if (state.size() == tasks.size())
        {
            int lastPendingIndex = -1;
            for (size_t i = 0; i < tasks.size(); i++)
            {
                if (!tasks[i].isCompleted())
                {
                    lastPendingIndex = static_cast<int>(i);
                }
            }

            for (size_t i = 0; i < tasks.size(); i++)
            {
                Task::Builder builder(state[i]);
                Task task = builder.build();

                tasks[i] = Task::Builder(tasks[i].getDescription())
                               .setDueDate(tasks[i].getDueDate())
                               .setCompleted(task.isCompleted())
                               .build();
            }

            if (lastPendingIndex >= 0)
            {
                tasks[lastPendingIndex] = Task::Builder(tasks[lastPendingIndex].getDescription())
                                              .setDueDate(tasks[lastPendingIndex].getDueDate())
                                              .setCompleted(true)
                                              .build();
            }

            cout << "Redo operation performed." << endl;
        }
        else
        {
            cout << "Redo operation failed. State data mismatch." << endl;
        }
    }
    else
    {
        cout << "Nothing to redo." << endl;
    }
}

void TaskList::saveState()
{
    if (caretakerIndex == caretakerSize - 1)
    {
        caretaker.getMemento(0);
    }

    vector<string> taskDescriptions;
    for (const Task &task : tasks)
    {
        taskDescriptions.push_back(task.getDescription());
    }

    Memento memento(taskDescriptions);
    caretaker.addMemento(memento);

    if (caretakerIndex < caretakerSize - 1)
    {
        caretakerIndex++;
    }
}

void markPendingTaskCompleted(TaskList &taskList)
{
    while (true)
    {
        int taskNumber = 1;
        cout << "Pending Tasks:\n";
        for (const Task &task : taskList.getTasks())
        {
            if (!task.isCompleted())
            {
                cout << taskNumber << ". " << task.getDescription();
                if (!task.getDueDate().empty())
                {
                    cout << ", Due: " << task.getDueDate();
                }
                cout << endl;
                taskNumber++;
            }
        }

        int selectedTask;
        while (true)
        {
            cout << "Enter the number of the task to mark as completed (0 to cancel): ";
            string input;
            getline(cin, input);

            stringstream ss(input);
            if (ss >> selectedTask)
            {
                if (selectedTask == 0)
                {
                    cout << "Operation canceled." << endl;
                    return;
                }
                else if (selectedTask >= 1 && selectedTask <= taskNumber - 1)
                {
                    taskList.markTaskCompleted(selectedTask);
                    cout << "Task marked as completed." << endl;

                    const Task &completedTask = taskList.getTasks()[selectedTask - 1];
                    if (!completedTask.getDueDate().empty())
                    {
                        cout << "Due Date: " << completedTask.getDueDate() << endl;
                    }

                    cout << endl;
                    return; 
                }
            }

            cout << "Invalid input. Please enter a valid task number or 0 to cancel.\n";
        }
    }
}

void deleteTaskByNumber(TaskList &taskList)
{
    while (true)
    {
        int taskNumber = 1;
        cout << "All Tasks:\n";
        for (const Task &task : taskList.getTasks())
        {
            cout << taskNumber << ". " << task.getDescription();
            if (!task.getDueDate().empty())
            {
                cout << ", Due: " << task.getDueDate();
            }
            cout << endl;
            taskNumber++;
        }

        int selectedTask;
        while (true)
        {
            cout << "Enter the number of the task to delete (0 to cancel): ";
            string input;
            getline(cin, input);

            stringstream ss(input);
            if (ss >> selectedTask)
            {
                if (selectedTask == 0)
                {
                    cout << "Operation canceled." << endl;
                    return; 
                }
                else if (selectedTask >= 1 && selectedTask <= taskNumber - 1)
                {
                    taskList.deleteTask(selectedTask);
                    cout << endl;
                    return;
                }
            }

            cout << "Invalid input. Please enter a valid task number or 0 to cancel.\n";
        }
    }
}
