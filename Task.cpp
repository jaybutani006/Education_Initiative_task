#include "Task.h"
#include <iostream>
#include <bits/stdc++.h>
// Define the Task class methods

Task::Task(const std::string &desc, bool comp, const std::string &date)
    : description(desc), completed(comp), dueDate(date) {}

std::string Task::getDescription() const
{
    return description;
}

bool Task::isCompleted() const
{
    return completed;
}

std::string Task::getDueDate() const
{
    return dueDate;
}

// Define the Task::Builder class methods

Task::Builder::Builder(const std::string &desc)
    : description(desc), completed(false), dueDate("") {}

Task::Builder &Task::Builder::setCompleted(bool comp)
{
    completed = comp;
    return *this;
}

Task::Builder &Task::Builder::setDueDate(const std::string &date)
{
    if (isValidDate(date))
    {
        dueDate = date;
    }
    else
    {
        std::cout << "Invalid date format or past date. Please enter a future date in DD/MM/YYYY format." << std::endl;
    }
    return *this;
}

Task Task::Builder::build()
{
    return Task(description, completed, dueDate);
}

bool Task::Builder::isValidDate(const std::string &date) const
{
    std::regex datePattern("^\\d{2}/\\d{2}/\\d{4}$");
    if (!std::regex_match(date, datePattern))
    {
        return false;
    }

    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%d/%m/%Y");
    if (ss.fail())
    {
        return false;
    }

    std::time_t now = std::time(nullptr);
    std::time_t dueTime = std::mktime(&tm);
    if (dueTime <= now)
    {
        return false;
    }

    return true;
}
