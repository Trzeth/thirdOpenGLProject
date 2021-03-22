#include "Prefab.h"

Prefab::Prefab()
{
}

Prefab::Prefab(const std::string& name)
{
	this->name = name;
}

void Prefab::AddConstructor(ComponentConstructor* constructor)
{
	constructors.emplace_back(std::shared_ptr<ComponentConstructor>(constructor));
}

std::vector<ComponentConstructorInfo> Prefab::Construct(World& world, eid_t parent, void* userinfo) const
{
	std::vector<ComponentConstructorInfo> infos;
	for (unsigned i = 0; i != constructors.size(); i++) {
		infos.emplace_back(constructors[i]->Construct(world, parent, userinfo));
	}

	return infos;
}

void Prefab::Finish(World& world, eid_t entity) const
{
	for (unsigned i = 0; i != constructors.size(); i++) {
		constructors[i]->Finish(world, entity);
	}
}

void Prefab::AddChild(const std::shared_ptr<Prefab>& prefab)
{
	childPrefabs.emplace_back(prefab);
}

std::vector<std::shared_ptr<Prefab>> Prefab::GetChildPrefabs() const
{
	return childPrefabs;
}

void Prefab::SetName(const std::string& name)
{
	this->name = name;
}

std::string Prefab::GetName() const
{
	return name;
}