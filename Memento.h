#pragma once
#include <vector>
#include <string>

class Memento
{
public:
    Memento(const std::vector<std::string> &taskList);
    const std::vector<std::string> &getState() const;

private:
    std::vector<std::string> state;
};

class Caretaker
{
public:
    void addMemento(const Memento &memento);
    Memento getMemento(int index) const;
    int getMementoCount() const;

private:
    std::vector<Memento> mementos;
    int mementoCount = 0;
};
