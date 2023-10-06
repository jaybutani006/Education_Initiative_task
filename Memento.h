#pragma once
#include <vector>
#include <string>
using namespace std;

class Memento
{
public:
    Memento(const vector<string> &taskList);
    const vector<string> &getState() const;

private:
    vector<string> state;
};

class Caretaker
{
public:
    void addMemento(const Memento &memento);
    Memento getMemento(int index) const;
    int getMementoCount() const;


private:
    vector<Memento> mementos;
    int mementoCount = 0;
};
