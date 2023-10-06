#include "Task.h"
#include <iostream>
#include <bits/stdc++.h>
// Define the Task class methods
using namespace std;

Task::Task(const string &desc, bool comp, const string &date)
    : description(desc), completed(comp), dueDate(date) {}

string Task::getDescription() const
{
    return description;
}

bool Task::isCompleted() const
{
    return completed;
}

string Task::getDueDate() const
{
    return dueDate;
}

// Define the Task::Builder class methods

Task::Builder::Builder(const string &desc)
    : description(desc), completed(false), dueDate("") {}

Task::Builder &Task::Builder::setCompleted(bool comp)
{
    completed = comp;
    return *this;
}

Task::Builder &Task::Builder::setDueDate(const string &date)
{
    if (isValidDate(date))
    {
        dueDate = date;
    }
    else
    {
        cout << "Invalid date format or past date. Please enter a future date in DD/MM/YYYY format." << endl;
    }
    return *this;
}

Task Task::Builder::build()
{
    return Task(description, completed, dueDate);
}

bool Task::Builder::isValidDate(const string &date) const
{
    regex datePattern("^\\d{2}/\\d{2}/\\d{4}$");
    if (!regex_match(date, datePattern))
    {
        return false;
    }

    tm tm = {};
    istringstream ss(date);
    ss >> get_time(&tm, "%d/%m/%Y");
    if (ss.fail())
    {
        return false;
    }

    time_t now = time(nullptr);
    time_t dueTime = mktime(&tm);
    
    if (dueTime <= now)
    {
        return false;
    }

    return true;
}
