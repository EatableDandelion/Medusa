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
			RenderingEntity(const shared_ptr<Mesh>& mesh, ITransform* transform);
			
			~RenderingEntity();
			
			void render(Shader& renderingPass, Camera& camera);
			
			void draw();
			
			Mat<4> getModelMatrix();
			
			template<class T>
			void setUniform(const std::string& name, const T& value)
			{	
				material->setUniform(name, value);
			}
			
			std::shared_ptr<Material> getMaterial() const;

		private:
			shared_ptr<Mesh> m_mesh;
			shared_ptr<ITransform> m_transform;
			shared_ptr<Material> material;
			static int allid;
			int id;
			
	};
}