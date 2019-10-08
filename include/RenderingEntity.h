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
			RenderingEntity(const shared_ptr<Mesh>& mesh, const shared_ptr<ITransform>& transform);
			
			RenderingEntity();
			
			~RenderingEntity();
			
			void render(Shader& renderingPass, Camera& camera);
			
			void draw(const int& culling);
			
			bool update(const Mat<4>& projectionMatrix, const Mat<4>& viewMatrix);
			
			template<class T>
			void setUniform(const std::string& name, const T& value)
			{	
				material->setUniform(name, value);
			}
			
			void setTexture(const TextureType& type, const Texture& texture);
			
			std::shared_ptr<Material> getMaterial() const;

		private:
			shared_ptr<Mesh> m_mesh;
			weak_ptr<ITransform> m_transform;
			shared_ptr<Material> material;
			static int allid;
			int id;			
	};
	
	class EntityLoader// : public ResourceLoader<RenderingEntity>
	{
		public:
			void load(const std::string& folderLocation, const std::string& fileName);//, RenderingEntity& entity);
			
			void unload(RenderingEntity& entity);
		private:
			void loadComponent(const std::string& type, const std::string& value);
	};
}