#include "System.h"

void System::Update(float dt)
{
	entityIterator = world.GetEidIterator(requiredComponents);
	while (!entityIterator.atEnd()) {
		updateEntity(dt, entityIterator.value());
		entityIterator.next();
	}
}