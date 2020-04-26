#include "Powerup.h"
#include "Entities.h"

void Powerup::update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities)
{
	Entity::update(deltaTime, entities);
	
	timer -= deltaTime;

	if (timer <= 0)
	{
		remove();
		Entities::getInstance()->decrementNumActive();
	}
}