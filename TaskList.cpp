#include "TaskList.h"
#include <iostream>
#include <sstream>

void TaskList::addTask(const Task &task)
{
    tasks.push_back(task);
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
                task = Task::Builder(task.getDescription()).setCompleted(true).setDueDate(task.getDueDate()).build();
                saveState();
                return;
            }
        }
    }
    // saveState();
}

void TaskList::deleteTask(int taskNumber)
{
    if (taskNumber >= 1 && taskNumber <= tasks.size())
    {
        tasks.erase(tasks.begin() + taskNumber - 1);
        std::cout << "Task deleted successfully." << std::endl;
        saveState();
    }
    else
    {
        std::cout << "Invalid task number. No task deleted." << std::endl;
    }
}

void TaskList::viewTasks(const std::string &filter) const
{
    std::cout << std::endl;
    for (const Task &task : tasks)
    {
        if (filter == "Show all" || (filter == "Show completed" && task.isCompleted()) || (filter == "Show pending" && !task.isCompleted()))
        {
            std::cout << task.getDescription() << " - " << (task.isCompleted() ? "Completed" : "Pending");
            if (!task.getDueDate().empty())
            {
                std::cout << ", Due: " << task.getDueDate();
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

const std::vector<Task> &TaskList::getTasks() const
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

                tasks[i] = Task::Builder(tasks[i].getDescription())
                               .setDueDate(tasks[i].getDueDate())
                               .setCompleted(task.isCompleted())
                               .build();
            }

            if (lastCompletedIndex >= 0)
            {
                tasks[lastCompletedIndex] = Task::Builder(tasks[lastCompletedIndex].getDescription())
                                                .setDueDate(tasks[lastCompletedIndex].getDueDate())
                                                .setCompleted(false)
                                                .build();
            }

            std::cout << "Undo operation performed." << std::endl;
        }
        else
        {
            std::cout << "Undo operation failed. State data mismatch." << std::endl;
        }
    }
    else
    {
        std::cout << "Nothing to undo." << std::endl;
    }
}

void TaskList::redo()
{
    if (caretakerIndex < caretaker.getMementoCount() - 1)
    {
        caretakerIndex++;
        const std::vector<std::string> &state = caretaker.getMemento(caretakerIndex).getState();

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

            std::cout << "Redo operation performed." << std::endl;
        }
        else
        {
            std::cout << "Redo operation failed. State data mismatch." << std::endl;
        }
    }
    else
    {
        std::cout << "Nothing to redo." << std::endl;
    }
}

void TaskList::saveState()
{
    if (caretakerIndex == caretakerSize - 1)
    {
        caretaker.getMemento(0);
    }

    std::vector<std::string> taskDescriptions;
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
        std::cout << "Pending Tasks:\n";
        for (const Task &task : taskList.getTasks())
        {
            if (!task.isCompleted())
            {
                std::cout << taskNumber << ". " << task.getDescription();
                if (!task.getDueDate().empty())
                {
                    std::cout << ", Due: " << task.getDueDate();
                }
                std::cout << std::endl;
                taskNumber++;
            }
        }

        int selectedTask;
        while (true)
        {
            std::cout << "Enter the number of the task to mark as completed (0 to cancel): ";
            std::string input;
            std::getline(std::cin, input);

            std::stringstream ss(input);
            if (ss >> selectedTask)
            {
                if (selectedTask == 0)
                {
                    std::cout << "Operation canceled." << std::endl;
                    return;
                }
                else if (selectedTask >= 1 && selectedTask <= taskNumber - 1)
                {
                    taskList.markTaskCompleted(selectedTask);
                    std::cout << "Task marked as completed." << std::endl;

                    const Task &completedTask = taskList.getTasks()[selectedTask - 1];
                    if (!completedTask.getDueDate().empty())
                    {
                        std::cout << "Due Date: " << completedTask.getDueDate() << std::endl;
                    }

                    std::cout << std::endl;
                    return; 
                }
            }

            std::cout << "Invalid input. Please enter a valid task number or 0 to cancel.\n";
        }
    }
}

void deleteTaskByNumber(TaskList &taskList)
{
    while (true)
    {
        int taskNumber = 1;
        std::cout << "All Tasks:\n";
        for (const Task &task : taskList.getTasks())
        {
            std::cout << taskNumber << ". " << task.getDescription();
            if (!task.getDueDate().empty())
            {
                std::cout << ", Due: " << task.getDueDate();
            }
            std::cout << std::endl;
            taskNumber++;
        }

        int selectedTask;
        while (true)
        {
            std::cout << "Enter the number of the task to delete (0 to cancel): ";
            std::string input;
            std::getline(std::cin, input);

            std::stringstream ss(input);
            if (ss >> selectedTask)
            {
                if (selectedTask == 0)
                {
                    std::cout << "Operation canceled." << std::endl;
                    return; 
                }
                else if (selectedTask >= 1 && selectedTask <= taskNumber - 1)
                {
                    taskList.deleteTask(selectedTask);
                    std::cout << std::endl;
                    return;
                }
            }

            std::cout << "Invalid input. Please enter a valid task number or 0 to cancel.\n";
        }
    }
}
