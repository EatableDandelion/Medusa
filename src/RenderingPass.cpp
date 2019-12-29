#include "RenderingPass.h"

#include <iostream>

namespace Medusa
{

	IPassSettings::IPassSettings(const bool& blending, const bool& depthRead, const bool& depthWrite):blending(blending), depthRead(depthRead), depthWrite(depthWrite)
	{}
	
	void IPassSettings::setBlending(const bool& activate)
	{
		blending = activate;
	}
		
	void IPassSettings::setDepthRead(const bool& activate)
	{
		depthRead = activate;
	}
			
	void IPassSettings::setDepthWrite(const bool& activate)
	{
		depthWrite = activate;
	}
	
	void IPassSettings::start()
	{
		if(blending)
		{
			startBlending();
		}
		if(depthRead)
		{
			startDepthRead();
		}
		if(depthWrite)
		{
			startDepthWrite();
		}
	}
			
	void IPassSettings::end()
	{
		if(depthRead)
		{
			stopDepthRead();
		}
		if(depthWrite)
		{
			stopDepthWrite();
		}
		if(blending)
		{
			stopBlending();
		}		
	}
	
	GLPassSettings::GLPassSettings(const bool& blending, const bool& depthRead, const bool& depthWrite):IPassSettings(blending, depthRead, depthWrite)
	{}
	
	void GLPassSettings::startBlending()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_DST_ALPHA);
	}
	
	void GLPassSettings::stopBlending()
	{
		glDisable(GL_BLEND);
	}
	
	void GLPassSettings::startDepthRead()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
	
	void GLPassSettings::stopDepthRead()
	{
		glDisable(GL_DEPTH_TEST);
	}
	
	void GLPassSettings::startDepthWrite()
	{
		glDepthMask(true);
	}
	
	void GLPassSettings::stopDepthWrite()
	{
		glDepthMask(false);
	}
	
	void GLPassSettings::setLineThickness(const float& thickness)
	{
		glLineWidth(thickness);
	}
	
	
	void IRenderingPass::renderAll(const Camera& camera)
	{
		if(active)
		{
			render(camera);
		}
		
		if(std::shared_ptr<IRenderingPass> next = m_next.lock())
		{
			next->renderAll(camera);
		}
	}
	
	void IRenderingPass::init(const std::shared_ptr<Assets>& assets)
	{
		initAssets(assets);
		if(std::shared_ptr<IRenderingPass> next = m_next.lock())
		{
			next->init(assets);
		}
	}
	
	void IRenderingPass::addNext(const std::shared_ptr<IRenderingPass> nextPass)
	{
		if(std::shared_ptr<IRenderingPass> next = m_next.lock())
		{
			nextPass->addNext(next);
			m_next = nextPass;
		}
		else
		{
			m_next = nextPass;
		}
	}
	
	std::shared_ptr<RenderingEntity> IRenderingPass::addEntity(const std::string& meshName, const std::string& textureName, const std::shared_ptr<Transform<3>>& transform)
	{
		return NULL;
	}
	
	std::shared_ptr<RenderingEntity> IRenderingPass::addEntity(const std::string& meshName, const std::shared_ptr<Transform<3>>& transform)
	{
		return NULL;
	}
	
	void IRenderingPass::setActive(const bool& isActive)
	{
		active = isActive;
	}
	
	
	
	
	GeometryPass::GeometryPass():RenderingPass("GeometryPass", false, true, true)
	{}
	
	void GeometryPass::updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera)
	{
		entity->updateModel();
		entity->updateMVP(camera.getProjectionMatrix()*camera.getViewMatrix());
	}
	
	std::shared_ptr<RenderingEntity> GeometryPass::addEntity(const std::string& mesh, const std::string& texture, const std::shared_ptr<Transform<3>>& transform)
	{
		std::shared_ptr<RenderingEntity> entity = RenderingPass::createEntity(RenderingPass::getAssets()->getMesh(mesh, Medusa::TRIANGLE_RENDERING), transform);
		entity->setTexture(TextureType::DIFFUSE0, RenderingPass::getAssets()->getTexture(texture));
		return entity;
	}




	DebugPass::DebugPass():RenderingPass("DebugDisplay", false, true, false)
	{
		
	}
			
	void DebugPass::updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera)
	{
		entity->setUniform<Circe::Vec3>("objectColor", Circe::Vec3(0.5f, 1.0f, 1.0f));
		geometryPass.updateEntity(entity, camera);
	}
	
	std::shared_ptr<RenderingEntity> DebugPass::addEntity(const std::string& mesh, const std::shared_ptr<Transform<3>>& transform)
	{
		return RenderingPass::createEntity(RenderingPass::getAssets()->getMesh(mesh, Medusa::WIRE_RENDERING), transform);
	}
	
	void DebugPass::setLineThickness(const float& thickness)
	{
		settings.setLineThickness(thickness);
	}
}
