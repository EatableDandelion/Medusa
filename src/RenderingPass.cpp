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
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//glFrontFace(GL_CW);
		//glCullFace(GL_BACK);
		m_shader.bind();	
	}
	
	bool RenderingPass::render(const Camera& camera, RenderingEntity& entity)
	{		
		
		/*Mat<4> m = entity.getModelMatrix();
		Mat<4> p = camera.getProjectionMatrix();
		Mat<4> mvp = p*m;
		entity.setUniform("MVP", mvp);*/
		if(entity.update(camera.getProjectionMatrix())){
			m_shader.update(entity.getMaterial());
			
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
