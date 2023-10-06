#include "Memento.h"
using namespace std;

Memento::Memento(const vector<string> &taskList) : state(taskList) {}

const vector<string> &Memento::getState() const
{
    return state;
}

void Caretaker::addMemento(const Memento &memento)
{
    mementos.push_back(memento);
    mementoCount++;
}

Memento Caretaker::getMemento(int index) const
{
    if (index >= 0 && index < mementos.size())
    {
        
        return mementos[index];
    }
    return Memento(vector<string>());
}

int Caretaker::getMementoCount() const
{
    return mementoCount;
}
