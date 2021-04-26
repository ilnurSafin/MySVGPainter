#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "observer.h"

class Subject{
public:
    virtual ~Subject(){}
    void addObserver(Observer* obs)
    {
        observers.push_back(obs);
    }
    void notifyEveryone()
    {
        for(Observer* obs : observers)
        {
            obs->onSubjectChange(this);
        }
    }
private:
    std::vector<Observer*> observers;
};

#endif // SUBJECT_H
