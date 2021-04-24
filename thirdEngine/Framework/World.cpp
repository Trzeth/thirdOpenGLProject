#include "World.h"

const eid_t World::NullEntity = UINT32_MAX;

void World::CleanupEntities()
{
	auto iter = entities.begin();
	while (iter != entities.end()) {
		if (iter->second.maskedForDeletion) {
			eid_t eid = iter->first;

			for (unsigned int i = 0; i != entityComponentMaps.size(); i++) {
				ComponentPool& componentPool = entityComponentMaps[i];
				auto componentIter = componentPool.find(eid);
				if (componentIter != componentPool.end()) {
					componentPool.erase(componentIter);
				}
			}

			iter = entities.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void World::Clear()
{
	entities.clear();

	for (unsigned int i = 0; i != this->entityComponentMaps.size(); i++) {
		this->entityComponentMaps[i].clear();
	}

	nextEntityId = 0;
}

eid_t World::ConstructPrefab(const Prefab& prefab, eid_t parent, void* userinfo)
{
	eid_t entity = this->GetNewEntity(prefab.GetName());
	auto entityIter = entities.find(entity);

	std::vector<ComponentConstructorInfo> infos = prefab.Construct(*this, parent, userinfo);
	for (unsigned int i = 0; i != infos.size(); i++) {
		ComponentConstructorInfo& info = infos[i];
		cid_t cid = getComponentId(info.typeidHash);
		ComponentPool& componentPool = this->entityComponentMaps[cid];
		componentPool.emplace(entity, std::unique_ptr<Component>(info.component));
		entityIter->second.components.SetBit(cid, true);
	}

	prefab.Finish(*this, entity);
	std::vector<std::shared_ptr<Prefab>> children = prefab.GetChildPrefabs();
	for (unsigned i = 0; i != children.size(); i++) {
		this->ConstructPrefab(*children[i], entity, userinfo);
	}

	return entity;
}

eid_t World::GetNewEntity(const std::string& name)
{
	eid_t id = nextEntityId++;
	std::string actualName = name;
	if (name.length() == 0) {
		actualName = "Entity " + id;
	}

	entities.emplace(id, World::Entity(actualName, ComponentBitmask()));
	return id;
}

void World::RemoveEntity(eid_t entity)
{
	auto entityIter = entities.find(entity);
	if (entityIter == entities.end()) {
		return;
	}
	entityIter->second.maskedForDeletion = true;
}

bool World::EntityHasComponents(eid_t entity, const ComponentBitmask& bitmask) const
{
	ComponentBitmask eb = this->getEntityBitmask(entity);
	return eb.HasComponents(bitmask);
}

bool World::OrderEntities(eid_t& e1, eid_t& e2, const ComponentBitmask& b1, const ComponentBitmask& b2) const
{
	ComponentBitmask eb1 = this->getEntityBitmask(e1);
	ComponentBitmask eb2 = this->getEntityBitmask(e2);

	if (eb1.HasComponents(b1) && eb2.HasComponents(b2)) {
		return true;
	}
	else if (eb1.HasComponents(b2) && eb2.HasComponents(b1)) {
		eid_t tmp = e1;
		e1 = e2;
		e2 = tmp;
		return true;
	}
	return false;
}

World::eid_iterator World::GetEidIterator(ComponentBitmask match)
{
	return eid_iterator(entities.begin(), entities.end(), match);
}

cid_t World::getComponentId(size_t typeidHash)
{
	auto iter = componentIdMap.find(typeidHash);
	cid_t id;
	if (iter == componentIdMap.end()) {
		id = this->registerComponent(typeidHash);
	}
	else {
		id = iter->second;
	}
	return id;
}

cid_t World::registerComponent(size_t typeidHash)
{
	cid_t id = nextComponentId++;
	componentIdMap.emplace(typeidHash, id);

	entityComponentMaps.emplace_back(std::move(ComponentPool()));
	assert(componentIdMap.size() == entityComponentMaps.size());
	return id;
}

ComponentBitmask World::getEntityBitmask(eid_t eid) const
{
	auto iter = entities.find(eid);
	if (iter != entities.end()) {
		return iter->second.components;
	}
	return ComponentBitmask();
}

World::eid_iterator::eid_iterator()
{
}

World::eid_iterator::eid_iterator(std::map<eid_t, Entity>::iterator entityIterBegin, std::map<eid_t, Entity>::iterator entityIterEnd, ComponentBitmask match)
{
	this->entityIter = entityIterBegin;
	this->entityIterEnd = entityIterEnd;
	this->match = match;

	while (this->entityIter != this->entityIterEnd && !this->entityIter->second.components.HasComponents(match))
	{
		this->entityIter++;
	}

	this->entityIterBegin = this->entityIter;
}

eid_t World::eid_iterator::value()
{
	return entityIter->first;
}

void World::eid_iterator::reset()
{
	this->entityIter = this->entityIterBegin;
}

void World::eid_iterator::next()
{
	do {
		entityIter++;
	} while (entityIter != entityIterEnd && !entityIter->second.components.HasComponents(match));
}

bool World::eid_iterator::atEnd()
{
	return entityIter == entityIterEnd;
}