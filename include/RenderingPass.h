#pragma once

#include "Shader.h"
#include "RenderingEntity.h"
#include <vector>
#include <memory>

namespace Medusa
{	
	class RenderingPass
	{
		public:
			RenderingPass();
			
			RenderingPass(const Shader& shader);
			
			~RenderingPass();
			
			void setShader(const Shader& shader);
			
			void bind();
					
			virtual void render(RenderingEntity& entity);
			
			void unbind();
			
		protected:
			virtual void setUp();
			virtual void takeDown();

		private:
			Shader m_shader;
	};
	
	class GeometryPass : public RenderingPass
	{
		protected:
			virtual void setUp();
			virtual void takeDown();
	};
	

}
