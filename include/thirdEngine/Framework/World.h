#pragma once
#include "Component.h"
#include "ComponentBitmask.h"

#include "Prefab.h"

#include <map>
#include <string>
#include <unordered_map>
#include <memory>
#include <assert.h>

typedef uint32_t eid_t;
typedef uint32_t cid_t;

class World
{
public:
	const static eid_t NullEntity;

	World() : nextComponentId(0), nextEntityId(0) { };

	void CleanupEntities();

	void Clear();

	eid_t ConstructPrefab(const Prefab& prefab, eid_t parent = World::NullEntity, void* userinfo = nullptr);

	eid_t GetNewEntity(const std::string& name = "");

	void RemoveEntity(eid_t entity);

	bool EntityHasComponents(eid_t entity, const ComponentBitmask& bitmask)const;

	/*!
	 \brief Orders entities depending on the expected components.
	 This is used when you have two known entities, but don't know which order they are in (e.g. collision detection).
	 \param e1 Upon calling, one of the entities to check. On return, this is the entity that has components in b1.
	 \param e2 Upon calling, one of the entities to check. On return, this is the entity that has components in b2.
	 \param b1 The components of the first entity.
	 \param b2 The components of the second entity.
	 \return True if either e1 or e2 had the components in b1 and b2, false otherwise.
	 */
	bool OrderEntities(eid_t& e1, eid_t& e2, const ComponentBitmask& b1, const ComponentBitmask& b2)const;

	template <class T>
	T* AddComponent(eid_t entity);

	//???
	template<class T>
	void RemoveComponent(eid_t entity);

	template<class T>
	T* GetComponent(eid_t entity, bool insert = false);

	template<class T>
	cid_t GetComponentId();

	template<class T>
	std::vector<eid_t> GetEntitiesWithComponent();

	struct Entity {
		Entity(const std::string& name, ComponentBitmask components)
			:name(name), components(components), maskedForDeletion(false) { }
		bool maskedForDeletion;
		std::string name;
		ComponentBitmask components;
	};

	class eid_iterator {
	public:
		eid_iterator();
		eid_iterator(std::map<eid_t, Entity>::iterator entityIterBegin,
			std::map<eid_t, Entity>::iterator entityIterEnd,
			ComponentBitmask match);
		eid_t value();
		void reset();
		void next();
		bool atEnd();
	private:
		std::map<eid_t, Entity>::iterator entityIterBegin;
		std::map<eid_t, Entity>::iterator entityIter;
		std::map<eid_t, Entity>::iterator entityIterEnd;
		ComponentBitmask match;
	};

	eid_iterator GetEidIterator(ComponentBitmask match);

private:
	//https://stackoverflow.com/questions/53168836/does-visual-studio-2017-need-an-explicit-move-constructor-declaration/53171318
	//typedef std::unordered_map<eid_t, std::unique_ptr<Component>> ComponentPool;
	class ComponentPool :public std::unordered_map<eid_t, std::unique_ptr<Component>> {
	public:
		ComponentPool() = default;
		ComponentPool(ComponentPool&& o) = default;
	};

	template<class T>
	cid_t registerComponent();

	cid_t getComponentId(size_t typeidHash);
	cid_t registerComponent(size_t typeidHash);

	std::unordered_map<size_t, cid_t> componentIdMap;

	// cid => eid => component
	std::vector<ComponentPool> entityComponentMaps;
	std::map<eid_t, Entity> entities;

	cid_t nextComponentId;
	eid_t nextEntityId;

	ComponentBitmask getEntityBitmask(eid_t eid)const;
};

template<class T>
T* World::AddComponent(eid_t entity)
{
	return this->GetComponent<T>(entity, true);
}

template<class T>
void World::RemoveComponent(eid_t entity)
{
	cid_t cid = GetComponentId<T>();
	ComponentPool& componentPool = this->entityComponentMaps[cid];

	auto iter = componentPool.find(entity);
	if (iter == componentPool.end()) {
		componentPool.erase(iter);

		auto entityIter = entities.find(entity);
		assert(entityIter != entities.end());

		entityIter->second.components.SetBit(cid, false);
	}
}

template<class T>
T* World::GetComponent(eid_t entity, bool insert)
{
	cid_t cid = this->GetComponentId<T>();
	ComponentPool& componentPool = this->entityComponentMaps[cid];

	auto iter = componentPool.find(entity);
	if (iter == componentPool.end()) {
		if (!insert)return nullptr;
		else
		{
			std::unique_ptr<Component> newComponent(static_cast<Component*>(new T()));
			auto emplaceIter = componentPool.emplace(entity, std::move(newComponent));
			iter = emplaceIter.first;

			auto entityIter = entities.find(entity);
			assert(entityIter != entities.end());

			entityIter->second.components.SetBit(cid, true);
		}
	}

	return static_cast<T*>(iter->second.get());
}

template<class T>
inline cid_t World::GetComponentId()
{
	size_t hash = typeid(T).hash_code();
	return getComponentId(hash);
}

template<class T>
std::vector<eid_t> World::GetEntitiesWithComponent()
{
	cid_t cid = GetComponentId<T>();
	ComponentPool& componentPool = this->entityComponentMaps[cid];
	std::vector<eid_t> entities;

	for (auto iter = componentPool.begin(); iter != componentPool.end(); iter++) {
		entities.push_back(iter->first);
	}

	return entities;
}

template<class T>
cid_t World::registerComponent()
{
	size_t hash = typeid(T).hash_code();
	return this->registerComponent(hash);
}