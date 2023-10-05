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
                // Save the state for undo/redo
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

        // Save the state for undo/redo
        saveState();
    }
    else
    {
        std::cout << "Invalid task number. No task deleted." << std::endl;
    }
    // saveState();
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

        // Check if the state size matches the current tasks size
        if (state.size() == tasks.size())
        {
            // Find the index of the last completed task
            int lastCompletedIndex = -1;
            for (size_t i = 0; i < tasks.size(); i++)
            {
                if (tasks[i].isCompleted())
                {
                    lastCompletedIndex = static_cast<int>(i);
                }
            }

            // Update the tasks with the state information while preserving completion status
            for (size_t i = 0; i < tasks.size(); i++)
            {
                Task::Builder builder(state[i]);
                Task task = builder.build();

                // Preserve the completion status of the task
                tasks[i] = Task::Builder(tasks[i].getDescription())
                               .setDueDate(tasks[i].getDueDate())
                               .setCompleted(task.isCompleted())
                               .build();
            }

            // Only change the last completed task to pending if it was previously completed
            if (lastCompletedIndex >= 0)
            {
                tasks[lastCompletedIndex] = Task::Builder(tasks[lastCompletedIndex].getDescription())
                                                .setDueDate(tasks[lastCompletedIndex].getDueDate())
                                                .setCompleted(false) // Change to pending
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

        // Check if the state size matches the current tasks size
        if (state.size() == tasks.size())
        {
            // Find the index of the last pending task
            int lastPendingIndex = -1;
            for (size_t i = 0; i < tasks.size(); i++)
            {
                if (!tasks[i].isCompleted())
                {
                    lastPendingIndex = static_cast<int>(i);
                }
            }

            // Update the tasks with the state information while preserving completion status
            for (size_t i = 0; i < tasks.size(); i++)
            {
                Task::Builder builder(state[i]);
                Task task = builder.build();

                // Preserve the completion status of the task
                tasks[i] = Task::Builder(tasks[i].getDescription())
                               .setDueDate(tasks[i].getDueDate())
                               .setCompleted(task.isCompleted())
                               .build();
            }

            // Only change the last undone (previously pending) task to completed if it was previously pending
            if (lastPendingIndex >= 0)
            {
                tasks[lastPendingIndex] = Task::Builder(tasks[lastPendingIndex].getDescription())
                                              .setDueDate(tasks[lastPendingIndex].getDueDate())
                                              .setCompleted(true) // Change to completed
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
        caretakerIndex--;
        caretaker.getMemento(0); // Remove the oldest state
    }

    std::vector<std::string> taskDescriptions;
    for (const Task &task : tasks)
    {
        taskDescriptions.push_back(task.getDescription());
    }

    Memento memento(taskDescriptions);
    caretaker.addMemento(memento);
    caretakerIndex++;
}

void markPendingTaskCompleted(TaskList &taskList)
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
        if (ss >> selectedTask && selectedTask >= 1 && selectedTask <= taskNumber - 1)
        {
            taskList.markTaskCompleted(selectedTask);
            std::cout << "Task marked as completed." << std::endl;

            // Display the due date of the completed task
            const Task &completedTask = taskList.getTasks()[selectedTask - 1];
            if (!completedTask.getDueDate().empty())
            {
                std::cout << "Due Date: " << completedTask.getDueDate() << std::endl;
            }

            std::cout << std::endl;
            break;
        }
        else if (selectedTask == 0)
        {
            std::cout << "Operation canceled." << std::endl;
            break;
        }
        else
        {
            std::cout << "Invalid task number. Please enter a valid task number or 0 to cancel.\n";
        }
    }
}

void deleteTaskByNumber(TaskList &taskList)
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
        if (ss >> selectedTask && selectedTask >= 1 && selectedTask <= taskNumber - 1)
        {
            taskList.deleteTask(selectedTask);
            std::cout << "Task deleted successfully." << std::endl;
            std::cout << std::endl;
            break;
        }
        else if (selectedTask == 0)
        {
            std::cout << "Operation canceled." << std::endl;
            break;
        }
        else
        {
            std::cout << "Invalid task number. Please enter a valid task number or 0 to cancel.\n";
        }
    }
}
