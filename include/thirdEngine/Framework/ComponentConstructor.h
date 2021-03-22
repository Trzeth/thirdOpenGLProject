#pragma once
#include <cstdint>
#include <typeinfo>

class World;
class Component;

typedef uint32_t eid_t;
typedef uint32_t cid_t;

struct ComponentConstructorInfo {
	ComponentConstructorInfo() :ComponentConstructorInfo(nullptr, 0) { }
	ComponentConstructorInfo(Component* component, size_t typeidHash)
		:component(component), typeidHash(typeidHash) { }

	Component* component;
	size_t typeidHash;
};

class ComponentConstructor
{
public:
	ComponentConstructor() { }
	virtual ~ComponentConstructor() = default;
	virtual ComponentConstructorInfo Construct(World& world, eid_t parent, void* userinfo) const = 0;
	virtual void Finish(World& world, eid_t entity) { }
};
