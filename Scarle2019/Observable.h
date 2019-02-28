#pragma once

#include "Observer.h"
#include <vector>

/*
	Interface for the observable/subject.
*/

class Observable
{
public:
	Observable();
	~Observable();
	virtual void AddObserver(Observer* _observer) = 0;
	virtual void RemoveObserver(Observer* _observer) = 0;
	virtual void NotifyObservers() = 0;
};

