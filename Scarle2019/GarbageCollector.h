#pragma once
#include "Deletable.h"
#include <thread>

class GarbageCollector
{
public:
	template<class T>
	void DeletePointer(T type)
	{
		Deletable* deletable = static_cast<Deletable*>(type);
		std::thread thread(&GarbageCollector::DelayDelete, this, deletable);
		thread.detach();
	}

private:
	void DelayDelete(Deletable* type)
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
		delete type;
	}

};