#pragma once
#include "ComponentConstructor.h"

template<class ComponentClass>
class DefaultComponentConstructor :public ComponentConstructor
{
public:
	DefaultComponentConstructor(const typename ComponentClass::Data& data);
	virtual ComponentConstructorInfo Construct(World& world, eid_t parent, void* userinfo);
protected:
	typename ComponentClass::Data data;
};

template<class ComponentClass>
DefaultComponentConstructor<ComponentClass>::DefaultComponentConstructor(const typename ComponentClass::Data& data)
	:data(data)
{ }

template<class ComponentClass>
ComponentConstructorInfo DefaultComponentConstructor<ComponentClass>::Construct(World& world, eid_t parent, void* userinfo)
{
	ComponentClass* component = new ComponentClass();
	component->data = this->data;

	return ComponentConstructorInfo(component, typeid(ComponentClass).hash_code());
}
