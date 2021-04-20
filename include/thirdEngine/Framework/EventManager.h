#pragma once
#include <unordered_map>
#include <functional>

#include "Event.h"
#include "World.h"

typedef uint32_t eventid_t;

class EventManager
{
public:
	EventManager(const World& world) :world(world), nextEventId(0) { }

	template<class T>
	void SendEvent(const T& event);

	void SendEvent(size_t hash);

	template<class T>
	uint32_t RegisterForEvent(std::function<void(const T&)> eventListener);
private:

	template<class T>
	eventid_t registerEventType();

	typedef std::function<void(const Event*)> EventCallbackInternal;
	typedef std::vector<EventCallbackInternal> EventCallbackList;

	std::unordered_map<size_t, eventid_t> eventTypeMap;
	std::vector<EventCallbackList> eventListeners;
	eventid_t nextEventId;
	const World& world;
};

template<class T>
inline void EventManager::SendEvent(const T& event)
{
	auto iter = this->eventTypeMap.find(typeid(T).hash_code());
	if (iter == this->eventTypeMap.end()) {
		return;
	}

	EventCallbackList& list = eventListeners[iter->second];
	for (EventCallbackInternal& callback : list) {
		callback(&event);
	}
}

inline void EventManager::SendEvent(size_t hash)
{
	auto iter = this->eventTypeMap.find(hash);
	if (iter == this->eventTypeMap.end()) {
		return;
	}

	EventCallbackList& list = eventListeners[iter->second];
	for (EventCallbackInternal& callback : list) {
		callback(new Event());
	}
}

template<class T>
inline uint32_t EventManager::RegisterForEvent(std::function<void(const T&)> eventListener)
{
	eventid_t eventid;
	auto iter = this->eventTypeMap.find(typeid(T).hash_code());

	if (iter == this->eventTypeMap.end()) {
		eventid = this->registerEventType<T>();
	}
	else
	{
		eventid = iter->second;
	}

	EventCallbackList& list = eventListeners[eventid];
	list.push_back([eventListener](const Event* event) {
		eventListener(*(static_cast<const T*>(event)));
		});

	return list.size() - 1;
}

template<class T>
inline eventid_t EventManager::registerEventType()
{
	size_t hash = typeid(T).hash_code();
	eventTypeMap.emplace(hash, nextEventId);
	eventListeners.push_back(EventCallbackList());

	return nextEventId++;
}
