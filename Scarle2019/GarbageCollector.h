#pragma once
#include <thread>

class GarbageCollector
{
public:
	template<class T>
	void DeletePointer(T type)
	{
		std::thread thread(&GarbageCollector::DelayDelete, this, type);
		thread.detach();
	}

private:
	template<class T>
	void DelayDelete(T type)
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
		delete type;
	}

	void DelayDelete(Item* item)
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
		delete item;
	}
};