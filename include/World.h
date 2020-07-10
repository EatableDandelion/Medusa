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
#include "Medusa.h"

namespace Medusa
{
	using namespace std;
	using namespace Circe;

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
			
		//	static MedusaInterface getRenderer();
			/*{
				return World::s_renderer;
			}*/
			
			//static void setRenderer(std::shared_ptr<MedusaInterface> renderer);
			//static void setRenderer(std::shared_ptr<MedusaInterface> renderer);
			/*{
				World::s_renderer = renderer;
			}*/
			
		private:
			std::unordered_map<int, EntityHandle> entities;
			std::stack<int> removedEntities;
			//static MedusaInterface s_renderer;
	};
}