#include "World.h"

#include <iostream>

namespace Medusa{
	int World::frameNumber=0;
	
	//MedusaInterface World::s_renderer = MedusaInterface();
	
	World::World()
	{
		EntityHandle entity = newEntity();
	}

	World::~World()
	{
		entities.clear();
		//delete s_renderer;
	}
	
	void World::update()
	{		
		while(removedEntities.empty())
		{
			entities.erase(removedEntities.top());
			removedEntities.pop();
		}
		
		World::frameNumber++;
	}
	
	EntityHandle World::newEntity()
	{
		entities.insert(std::pair<int, EntityHandle>(0, EntityHandle(std::make_shared<Entity>())));
		return entities[0];
	}
	
	void World::removeEntity(const int id)
	{
		removedEntities.push(id);
	}
	
	EntityHandle World::getEntity(const int id)
	{
		return entities[id];
	}
	
/*	MedusaInterface World::getRenderer()
	{
		return World::s_renderer;
	}*/
	
	/*void World::setRenderer(std::shared_ptr<MedusaInterface> renderer)
	{
		World::s_renderer = renderer;
	}*/
	
}