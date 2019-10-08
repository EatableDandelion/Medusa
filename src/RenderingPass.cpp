#include "RenderingPass.h"

#include <iostream>

namespace Medusa{
	
	RenderingPass::RenderingPass()
	{
		CIRCE_INFO("Initializing rendering pass.");
	}
	
	RenderingPass::RenderingPass(const Shader& shader):m_shader(shader)
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
		setUp();
		m_shader->bind();	
	}
	
	void RenderingPass::render(RenderingEntity& entity)
	{
		m_shader->update(entity.getMaterial());
		entity.draw(1);
	}
	
	void RenderingPass::unbind()
	{
		takeDown();
	}
	
	void RenderingPass::setUp()
	{}
	
	void RenderingPass::takeDown()
	{}
	
	void GeometryPass::setUp()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
	
	void GeometryPass::takeDown()
	{
		glDisable(GL_DEPTH_TEST);
	}
}
