#pragma once
#include <vector>
#include <string>
#include <memory>

#include "ComponentConstructor.h"

class World;

class Prefab
{
public:
	Prefab();
	Prefab(const std::string& name);

	void AddConstructor(ComponentConstructor* constructor);
	std::vector<ComponentConstructorInfo> Construct(World& world, eid_t parent, void* userinfo)const;

	void Finish(World& world, eid_t entity)const;

	void AddChild(const std::shared_ptr<Prefab>& prefab);
	std::vector<std::shared_ptr<Prefab>> GetChildPrefabs() const;

	void SetName(const std::string& name);
	std::string GetName() const;
private:
	std::vector<std::shared_ptr<ComponentConstructor>> constructors;
	std::vector<std::shared_ptr<Prefab>> childPrefabs;
	std::string name;
};
