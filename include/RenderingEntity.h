#pragma once
#include <memory>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include <Circe/Circe.h>

namespace Medusa
{
	using namespace std;
	using namespace Circe;
	
	class RenderingEntity
	{
		public:
			RenderingEntity(const shared_ptr<Mesh>& mesh, const shared_ptr<ITransform>& transform);
			
			~RenderingEntity();
			
			void render(Shader& renderingPass, Camera& camera);
			
			void draw();
			
			bool update(const Mat<4>& projectionMatrix);
			
			template<class T>
			void setUniform(const std::string& name, const T& value)
			{	
				material->setUniform(name, value);
			}
			
			void setTexture(const Texture& texture);
			
			std::shared_ptr<Material> getMaterial() const;

		private:
			shared_ptr<Mesh> m_mesh;
			weak_ptr<ITransform> m_transform;
			shared_ptr<Material> material;
			static int allid;
			int id;
			
	};
}