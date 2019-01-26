#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <iostream>

//https://github.com/redxdev/ECS

namespace Manure{
	
	using namespace std;
	
	struct Component
	{
		virtual ~Component() = default;
		virtual void print()=0;
	};
	
	struct Component1:public Component
	{
		virtual void print();		
	};
	
	struct Component2:public Component
	{
		virtual void print();
	};
	
	class Entity
	{
		public:
			template<class T>
			void addComponent()
			{
				components[getId<T>()] = make_shared<T>();
			}
			
			template<class T>
			type_index getId()
			{
				return type_index(typeid(T));
			}
			
			template<class T>
			shared_ptr<T> getComponent()
			{
				shared_ptr<Component> res = components[getId<T>()];
				return dynamic_pointer_cast<T>(res);
			}
			
			template<typename T>
			bool hasComponents()
			{
				return components.find(getId<T>())!=components.end();
			}
			
			template<typename T1, typename T2, typename... Ts>
			bool hasComponents()
			{
				return hasComponents<T1>() && hasComponents<T2, Ts...>();
			}
			
		private:
			unordered_map<type_index, shared_ptr<Component>> components;
	};
	
	class World
	{
		public:
			World();
			
			template<typename... T>
			void onEach(typename common_type<function<void(shared_ptr<T>...)>>::type func)
			{
				if(entity.hasComponents<T...>())
				{					
					func(entity.getComponent<T>()...);
				}
			}

		private:
			Entity entity;
	};
	
	class System
	{
		public:
			template<typename... T>
			void update(World& world)
			{
				world.onEach<T...>([](shared_ptr<T>... t){cout << "Yes..." << endl;});
			}
	};
}