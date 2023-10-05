#ifndef TASK_H
#define TASK_H

#include <string>

class Task
{
public:
    class Builder;

    std::string getDescription() const;

    bool isCompleted() const;

    std::string getDueDate() const;

private:
    std::string description;
    bool completed;
    std::string dueDate;

    Task(const std::string &desc, bool comp, const std::string &date);
};

class Task::Builder
{
public:
    Builder(const std::string &desc);

    Builder &setCompleted(bool comp);

    Builder &setDueDate(const std::string &date);

    Task build();

    bool isValidDate(const std::string &date) const;

private:
    std::string description;
    bool completed;
    std::string dueDate;
};

#endif // TASK_H
