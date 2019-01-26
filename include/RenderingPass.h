#pragma once

#include "Shader.h"
#include "Camera.h"
#include "RenderingEntity.h"
#include <vector>
#include <memory>

namespace Medusa
{	
	class RenderingPass
	{
		public:
			RenderingPass();
			
			~RenderingPass();
			
			void setShader(const Shader& shader);
			
			void bind();
			
			void render(const Camera& camera, RenderingEntity& entity);
			
			void unbind();

		private:
			Shader m_shader;
	};
	

}
