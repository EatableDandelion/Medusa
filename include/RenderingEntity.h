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
			
		private:
			Mesh m_mesh;
			weak_ptr<Transform<3>> m_transform;
			shared_ptr<Material> material;
			static int allid;
			int id;		
			
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
}