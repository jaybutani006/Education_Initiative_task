#include "Memento.h"

Memento::Memento(const std::vector<std::string> &taskList) : state(taskList) {}

const std::vector<std::string> &Memento::getState() const
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
    return Memento(std::vector<std::string>());
}

int Caretaker::getMementoCount() const
{
    return mementoCount;
}
