#pragma once

#include <functional>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <iostream>
#include <Circe/Circe.h>

//https://github.com/redxdev/ECS

namespace Medusa
{
	using namespace std;
	using namespace Circe;
	
	class Entity;
	class EntityHandle;
	class World;
	
	class Component
	{
		public:
			virtual ~Component() = default;
			virtual void update(World& world, EntityHandle& entity) = 0;
	};
	
	class TestComponent1 : public Component
	{
		public:
			virtual void update(World& world, EntityHandle& entity);
	};
	
	class TestComponent2 : public Component
	{
		public:
			TestComponent2(std::string name);
			virtual void update(World& world, EntityHandle& entity);
			
		private:
			std::string name;
	};
	
	class Entity
	{
		public:
			Entity();
			
			Entity(const Entity& other);
			
			~Entity();
		
			template<class T, typename... Args>
			void addComponent(Args&&... args)
			{
				components[getComponentId<T>()] = std::make_shared<T>(std::forward<Args>(args)...);
			}
			
			template<class T>
			std::shared_ptr<T> getComponent()
			{
				std::shared_ptr<Component> res = components[getComponentId<T>()];
				return dynamic_pointer_cast<T>(res);
			}
			
			template<typename T>
			bool hasComponents()
			{
				return components.find(getComponentId<T>())!=components.end();
			}
			
			template<typename T1, typename T2, typename... Ts>
			bool hasComponents()
			{
				return hasComponents<T1>() && hasComponents<T2, Ts...>();
			}
			
			int getId() const;
			
			void setTransform(const Transform3& transform);
			
			Direction3 getSize() const;
			
			void setSize(const Vec3& size);
			
			Position3 getPosition() const;
			
			void setPosition(const Vec3& position);
			
			void setRotation(const Vec3& leftAxis, const Vec3& fwdAxis);
			
			void attachTo(const std::shared_ptr<Entity>& parentEntity);
		
			Mat<4> getTransformMatrix() const;
			
			void update(World& world, EntityHandle& entity);
			
		private:
			static int allid;			
			const int id;
			std::unordered_map<type_index, shared_ptr<Component>> components;			
			std::weak_ptr<Entity> parent;
			Circe::Transform3 m_transform;
			
			template<class T>
			type_index getComponentId()
			{
				return type_index(typeid(T));
			}
			
	};
	
	class EntityHandle
	{
		public:
			EntityHandle(const std::shared_ptr<Entity> entity);
			
			EntityHandle();
			
			template<class T, typename... Args>
			void addComponent(Args&&... args)
			{
				entity->addComponent<T>(args...);
			}
			
			template<class T>
			std::shared_ptr<T> getComponent()
			{
				return entity->getComponent<T>();
			}
			
			template<typename T>
			bool hasComponents()
			{
				return entity->hasComponents<T>();
			}
			
			template<typename T1, typename T2, typename... Ts>
			bool hasComponents()
			{
				return entity->hasComponents<T1,T2,Ts...>();
			}
			
			int getId() const;
			
			float getSize() const;
			
			void setSize(const float& size);
			
			Position2 getPosition() const;
			
			void setPosition(const Vec2& position);
			
			void lookAt(const Vec2& position);
			
			void setRotation(const Vec2& forward);
			
			void update(World& world);
			
		private:
			std::shared_ptr<Entity> entity;
	};
	
	class World
	{
		public:
			World();
			
			~World();
			
			void update();
			
			template<typename... T>
			void onEach(typename common_type<function<void(shared_ptr<T>...)>>::type func)
			{
				std::unordered_map<int, EntityHandle>::iterator it;
				for(it = entities.begin(); it != entities.end(); it++)
				{
					if(it->second.hasComponents<T...>())
					{					
						func(it->second.getComponent<T>()...);
					}
				}
			}
			
			template<typename T>
			std::vector<EntityHandle> getEntitiesInRange(const float radius, const Position2& position)
			{
				std::vector<EntityHandle> result;
				std::unordered_map<int, EntityHandle>::iterator it;
				for(it = entities.begin(); it != entities.end(); it++)
				{
					if(it->second.getPosition().distance2(position) < radius*radius)
					{
						if(it->second.hasComponents<T>())
						{					
							result.push_back(it->second);
						}
					}
				}
				return result;
			}
			
			EntityHandle newEntity();

			void removeEntity(const int id);
			
			EntityHandle getEntity(const int id);
			
			static int frameNumber;
			
		private:
			std::unordered_map<int, EntityHandle> entities;
			std::stack<int> removedEntities;
			//static MedusaInterface renderer;
	};
	
	class System
	{
		public:
			/*template<typename... Components>
			void update(World& world)
			{
				world.onEach<Components...>([](shared_ptr<Components>... t){t->print();});
			}*/
			
			/*template<typename Component>
			void update(World& world)
			{
				world.onEach<Component>([](shared_ptr<Component> component){component->print();});
			}*/
	};
}