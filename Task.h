#ifndef TASK_H
#define TASK_H

#include <string>
using namespace std;

class Task
{
public:
    class Builder;

    string getDescription() const;

    bool isCompleted() const;

    string getDueDate() const;

private:
    string description;
    bool completed;
    string dueDate;

    Task(const string &desc, bool comp, const string &date);
};


class Task::Builder
{
public:
    Builder(const string &desc);

    Builder &setCompleted(bool comp);

    Builder &setDueDate(const string &date);

    Task build();

    bool isValidDate(const string &date) const;

private:
    string description;
    bool completed;
    string dueDate;
};

#endif // TASK_H
