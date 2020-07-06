#include "World.h"

#include <iostream>

namespace Medusa{
	
	void TestComponent1::update(World& world, EntityHandle& entity)
	{
		std::cout << "Cmponent 1" << std::endl;	
	}
	
	TestComponent2::TestComponent2(std::string name):name(name)
	{}
	
	void TestComponent2::update(World& world, EntityHandle& entity)
	{
		std::cout << name << std::endl;		
	}
	
	int Entity::allid=0;
	
	Entity::Entity():id(allid++)
	{
		CIRCE_INFO("Initializing new entity "+std::to_string(id));
	}

	Entity::Entity(const Entity& other):m_transform(other.m_transform), id(other.id)
	{}
	
	Entity::~Entity()
	{
		CIRCE_INFO("Terminating entity "+std::to_string(id));
		components.clear();
	}
	
	int Entity::getId() const
	{
		return id;
	}
	
	void Entity::setTransform(const Transform3& transform)
	{
		m_transform = transform;
	}
	
	Direction3 Entity::getSize() const
	{
		return m_transform.getFrameScale();
	}
	
	void Entity::setSize(const Vec3& size)
	{
		m_transform.setFrameScale(size(0),size(1),size(2));
	}
	
	Position3 Entity::getPosition() const
	{
		return m_transform.getFramePosition();
	}
	
	void Entity::setPosition(const Vec3& position)
	{
		m_transform.setFramePosition(position(0),position(1),position(2));
	}
	
	void Entity::setRotation(const Vec3& leftAxis, const Vec3& fwdAxis)
	{
		m_transform.setFrameRotation(leftAxis, fwdAxis);
	}
	
	void Entity::attachTo(const std::shared_ptr<Entity>& parentEntity)
	{
		parent = parentEntity;
	}
	
	Mat<4> Entity::getTransformMatrix() const
	{
		if(std::shared_ptr<Entity> papa = parent.lock())
			return m_transform.getTransformMatrix()*papa->m_transform.getTransformMatrix();
		return m_transform.getTransformMatrix();
	}
	
	void Entity::update(World& world, EntityHandle& entity)
	{
		for(auto& component : components)
		{
			component.second->update(world, entity);
		}
	}
	
	EntityHandle::EntityHandle(const std::shared_ptr<Entity> entity):entity(entity)
	{}
	
	EntityHandle::EntityHandle():entity(std::make_shared<Entity>())
	{}
	
	float EntityHandle::getSize() const
	{
		return entity->getSize()(0);
	}
			
	void EntityHandle::setSize(const float& size)
	{
		entity->setSize(Vec3(size, size, 1.0f));
	}
	
	Position2 EntityHandle::getPosition() const
	{
		return Position2(REF_FRAME::GLOBAL, entity->getPosition()(0), entity->getPosition()(1));
	}
	
	void EntityHandle::setPosition(const Vec2& position)
	{
		entity->setPosition(Vec3(position(0), position(1), 0.0f));
	}
	
	void EntityHandle::lookAt(const Vec2& position)
	{
		setRotation(position-getPosition().getValue());
	}
	
	void EntityHandle::setRotation(const Vec2& forward)
	{
		Vec3 fwd(forward(0), forward(1), 0.0f);
		Vec3 left(-forward(1), forward(0), 0.0f);
		entity->setRotation(left, fwd);
	}
	
	void EntityHandle::update(World& world)
	{
		entity->update(world, *this);
	}
	
	int World::frameNumber=0;
	
	World::World()
	{
		EntityHandle entity = newEntity();
		entity.addComponent<TestComponent2>("This is a test component.");
	}

	World::~World()
	{
		entities.clear();
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
	
}