#include "RenderingPass.h"

#include <iostream>

namespace Medusa{
	
	RenderingPass::RenderingPass()
	{
		CIRCE_INFO("Initializing rendering pass.");
	}
	
	RenderingPass::~RenderingPass()
	{
		CIRCE_INFO("Terminating rendering pass.");
	}
	
	void RenderingPass::setShader(const Shader& shader)
	{
		m_shader=shader;
	}
	
	void RenderingPass::bind()
	{
		//Attach shader
		m_shader.bind();		
	}
	
	void RenderingPass::render(const Camera& camera, RenderingEntity& entity)
	{		
		
		Mat<4> m = entity.getModelMatrix();
		Mat<4> p = camera.getProjectionMatrix();
		Mat<4> mvp = p*m;
		entity.setUniform("MVP", mvp);

		m_shader.update(entity.getMaterial());		
		
		//Draw mesh
		entity.draw();
		
	}
	
	void RenderingPass::unbind()
	{
		
	}
}
