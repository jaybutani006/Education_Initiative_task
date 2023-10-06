#include <iostream>
#include "Task.h"
#include "TaskList.h"
#include "Memento.h"
#include <sstream>
using namespace std;

int main()
{
    TaskList taskList;

    while (true)
    {
        cout << "Options:\n";
        cout << "1. Add Task\n";
        cout << "2. Mark Completed\n";
        cout << "3. Delete Task\n";
        cout << "4. View Tasks\n";
        cout << "5. Undo\n";
        cout << "6. Redo\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        string choiceStr;
        getline(cin, choiceStr);

        int choice;
        stringstream ss(choiceStr);
        if (!(ss >> choice) || choice < 1 || choice > 7)
        {
            cout << "Invalid choice. Please enter a number between 1 and 7.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            string description, dueDate;
            cout << "Enter task description: ";
            getline(cin, description);
            bool isDateValid = true;

            do
            {
                isDateValid = true;
                cout << "Enter due date in DD/MM/YYYY format (optional, press Enter to skip): ";
                getline(cin, dueDate);

                Task::Builder taskBuilder(description);

                if (!dueDate.empty() && !taskBuilder.isValidDate(dueDate))
                {
                    cout << "Invalid date format or past date. Please enter a future date in DD/MM/YYYY format." << endl;
                    isDateValid = false;
                }
            } while (!isDateValid);
            if (!dueDate.empty())
            {
                Task task = Task::Builder(description).setDueDate(dueDate).build();
                taskList.addTask(task);
                cout << "Task added successfully." << endl;
            }
            else
            {
                Task task = Task::Builder(description).build();
                taskList.addTask(task);
                cout << "Task added without a due date." << endl;
            }

            cout << endl;
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
            string filter;
            int filterChoice;

            while (true)
            {
                cout << "Options:\n";
                cout << "1. Show all\n";
                cout << "2. Show completed\n";
                cout << "3. Show pending\n";
                cout << "Enter your choice: ";

                string input;
                getline(cin, input);

                stringstream ss(input);
                if (ss >> filterChoice && filterChoice >= 1 && filterChoice <= 3)
                {
                    break;
                }
                else
                {
                    cout << "Invalid choice. Please enter a valid option (1, 2, or 3).\n";
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
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
