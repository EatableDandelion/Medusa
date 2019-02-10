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

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		m_shader->bind();	
	}
	
	bool RenderingPass::render(const Camera& camera, RenderingEntity& entity)
	{		
		if(entity.update(camera.getProjectionMatrix())){
			m_shader->update(entity.getMaterial());
			
			//Draw mesh
			entity.draw(1);
			return true;
		}else{
			return false;
		}
	}
	
	void RenderingPass::unbind()
	{
		glDisable(GL_DEPTH_TEST);
	}
}
