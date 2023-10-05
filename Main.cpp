#include <iostream>
#include "Task.h"
#include "TaskList.h"
#include "Memento.h"
#include <sstream>
int main()
{
    TaskList taskList;

    while (true)
    {
        std::cout << "Options:\n";
        std::cout << "1. Add Task\n";
        std::cout << "2. Mark Completed\n";
        std::cout << "3. Delete Task\n";
        std::cout << "4. View Tasks\n";
        std::cout << "5. Undo\n";
        std::cout << "6. Redo\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";

        std::string choiceStr;
        std::getline(std::cin, choiceStr);

        int choice;
        std::stringstream ss(choiceStr);
        if (!(ss >> choice) || choice < 1 || choice > 7)
        {
            std::cout << "Invalid choice. Please enter a number between 1 and 7.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            std::string description, dueDate;
            std::cout << "Enter task description: ";
            std::getline(std::cin, description);
            bool isValid;
            do
            {
                isValid = true;
                if (!isValid)
                {
                    std::cout << "Invalid date format or past date. Please enter a future date in DD/MM/YYYY format." << std::endl;
                }
                std::cout << "Enter due date in DD/MM/YYYY format (optional): ";
                std::getline(std::cin, dueDate);
                Task::Builder taskBuilder(description);
                if (!dueDate.empty())
                {
                    taskBuilder.setDueDate(dueDate);
                }
                isValid = taskBuilder.isValidDate(dueDate);
            } while (!isValid);
            Task task = Task::Builder(description).setDueDate(dueDate).build();
            taskList.addTask(task);
            std::cout << "Task added successfully." << std::endl;
            std::cout << std::endl;
            break;
        }
        case 2:
        {
            markPendingTaskCompleted(taskList);
            break;
        }
        case 3:
        {
            deleteTaskByNumber(taskList);
            break;
        }
        case 4:
        {
            std::string filter;
            int filterChoice;

            while (true)
            {
                std::cout << "Options:\n";
                std::cout << "1. Show all\n";
                std::cout << "2. Show completed\n";
                std::cout << "3. Show pending\n";
                std::cout << "Enter your choice: ";

                std::string input;
                std::getline(std::cin, input);

                std::stringstream ss(input);
                if (ss >> filterChoice && filterChoice >= 1 && filterChoice <= 3)
                {
                    break;
                }
                else
                {
                    std::cout << "Invalid choice. Please enter a valid option (1, 2, or 3).\n";
                }
            }

            switch (filterChoice)
            {
            case 1:
                filter = "Show all";
                break;
            case 2:
                filter = "Show completed";
                break;
            case 3:
                filter = "Show pending";
                break;
            }

            taskList.viewTasks(filter);
            break;
        }
        case 5:
        {
            taskList.undo();
            break;
        }
        case 6:
        {
            taskList.redo();
            break;
        }
        case 7:
            return 0;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
