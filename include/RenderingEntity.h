#pragma once
#include <memory>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include <Circe/Circe.h>
#include <unordered_map>
#include <vector>
#include <iterator>
#include "World.h"

namespace Medusa
{
	using namespace std;
	using namespace Circe;
	
	/*class SpriteComponent : public Component
	{
		public:
			SpriteComponent(const std::string& name);
			~SpriteComponent();
			virtual void update(World& world, EntityHandle& entity);
		
		private:
			
	};*/
	
	class EntityData : public Entity
	{
		public:
			EntityData(const Mesh& mesh);
			
			EntityData(const EntityData& other);
			
			void draw(const int& culling);
			
			template<class T>
			void setUniform(const std::string& name, const T& value)
			{
				material->setUniform(name, value);
			}
			
			void updateModel();
			
			void updateMVP(const Circe::Mat<4>& viewProjection);
			
			void setTexture(const TextureType& type, const Texture& texture);
			
			std::shared_ptr<Material> getMaterial() const;

			void setVisibility(const bool& visibility);

		private:
			Mesh m_mesh;
			shared_ptr<Material> material;			
			bool visible;			
			
	};
	
	//Handler/Proxy class for EntityData
	class RenderingHandler
	{
		public:
			RenderingHandler(const std::shared_ptr<EntityData> entityData);
			
			RenderingHandler(const RenderingHandler& other);
			
			virtual void draw(const int& culling);
			
			template<class T>
			void setUniform(const std::string& name, const T& value)
			{
				entity->setUniform<T>(name, value);
			}
			
			void updateModel();
			
			void updateMVP(const Circe::Mat<4>& viewProjection);
			
			void setTexture(const TextureType& type, const Texture& texture);
			
			std::shared_ptr<Material> getMaterial() const;

			//void attachTo(const std::shared_ptr<Transform3>& parentTransform);
			
			void setVisibility(const bool& visibility);
			
			int getId() const;
			
			void setTransform(const Transform3& transform);
			
			Direction3 getSize() const;
			
			void setSize(const Vec3& size);
			
			Position3 getPosition() const;
			
			void setPosition(const Vec3& position);
			
			void setRotation(const Vec3& leftAxis, const Vec3& fwdAxis);
			
			void attachTo(const RenderingHandler& parentEntity);
			
		private:
			std::shared_ptr<EntityData> entity;
	};
	
	class EntityLoader
	{
		public:
			void load(const std::string& folderLocation, const std::string& fileName);//, RenderingEntity& entity);
			
			void unload(RenderingHandler& entity);
		private:
			void loadComponent(const std::string& type, const std::string& value);
	};
	
	template<typename T> 
	class EntityCollection;
	
	template<typename T>
	class EntityCollection
	{
		typedef typename std::unordered_map<int, std::shared_ptr<T>>::iterator iterator;
		public:
			int add(std::shared_ptr<T>& entity)
			{
				int id = entity->getId();
				entities.insert(std::pair<int, std::shared_ptr<T>>(id, entity));
				return id;
			}
			
			void remove(const int& id)
			{
				entities.erase(id);
			}
			
			std::shared_ptr<T> get(const int& id)
			{
				return entities.at(id);
			}
			
			iterator begin()
			{
				return entities.begin();
			}
			
			iterator end()
			{
				return entities.end();
			}
			
			std::shared_ptr<T> operator()(const int& id)
			{
				return get(id);
			}
		
		private:
			std::unordered_map<int, std::shared_ptr<T>> entities;
			
	};
}