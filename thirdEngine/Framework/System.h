#pragma once

#include <vector>
#include <unordered_set>
#include <typeinfo>

#include "World.h"

class System
{
public:
	System(World& world) :world(world) { };
	virtual ~System() = default;

	void Update(float dt);
protected:
	virtual void updateEntity(float dt, eid_t entity) = 0;

	template<class T>
	void require();

	World& world;

	ComponentBitmask requiredComponents;
private:
	World::eid_iterator entityIterator;
};

template<class T>
void System::require()
{
	cid_t cid = world.GetComponentId<T>();
	requiredComponents.SetBit(cid, true);
}
