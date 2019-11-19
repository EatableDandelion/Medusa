#pragma once
#include <memory>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include <Circe/Circe.h>
#include <vector>
#include <iterator>

namespace Medusa
{
	using namespace std;
	using namespace Circe;
	
	class RenderingEntity
	{
		public:
			RenderingEntity(const Mesh& mesh, const shared_ptr<Transform<3>>& transform);
			
			RenderingEntity(const Mesh& mesh);
			
			~RenderingEntity();

			virtual void draw(const int& culling);
			
			template<class T>
			void setUniform(const std::string& name, const T& value)
			{
				material->setUniform(name, value);
			}
			
			void updateModel();
			
			void updateMVP(const Circe::Mat<4>& viewProjection);
			
			void setTexture(const TextureType& type, const Texture& texture);
			
			std::shared_ptr<Material> getMaterial() const;

			void attachTo(const std::shared_ptr<Transform<3>>& parentTransform);
			
			void setVisibility(const bool& visibility);
			
			int getId() const;
			
			void setTransform(const std::shared_ptr<Transform<3>>& transform);
			
		private:
			Mesh m_mesh;
			weak_ptr<Transform<3>> m_transform;
			shared_ptr<Material> material;
			static int allid;
			int id;
			bool visible;
			
			Mat<4> getTransformMatrix() const;	
	};
	
	class EntityLoader
	{
		public:
			void load(const std::string& folderLocation, const std::string& fileName);//, RenderingEntity& entity);
			
			void unload(RenderingEntity& entity);
		private:
			void loadComponent(const std::string& type, const std::string& value);
	};
	
	template<typename T> 
	class RenderingCollection;
	
	template<typename T>
	class RenderingIterator
	{
		public:
			RenderingIterator(RenderingCollection<T>& collection, const int& startIndex):collection(collection), index(startIndex)
			{}
			
			RenderingIterator<T> operator++()
			{
				index++;
				return *this;
			}
			
			bool operator!=(const RenderingIterator& other) const
			{
				return index != other.index;
			}
			
			std::shared_ptr<T>& operator*()
			{
				return collection.entities[index];
			}
			
			int getIndex() const
			{
				return index;
			}
			
		private:
			int index;
			RenderingCollection<T>& collection;
	};
	
	
	template<typename T>
	class RenderingCollection
	{
		public:
			int add(std::shared_ptr<T>& entity)
			{
				entities.push_back(std::move(entity));
				return entities.size() - 1;
			}
			
			std::shared_ptr<T> get(const int& id)
			{
				return entities[id];
			}
			
			RenderingIterator<T> begin()
			{
				return RenderingIterator<T>(*this, 0);
			}
			
			RenderingIterator<T> end()
			{
				return RenderingIterator<T>(*this, entities.size());
			}	
			
			std::shared_ptr<T> get(const RenderingIterator<T>& iterator)
			{
				return entities[iterator.getIndex()];
			}
		
		private:
			std::vector<std::shared_ptr<T>> entities;
			friend class RenderingIterator<T>;
	};
}