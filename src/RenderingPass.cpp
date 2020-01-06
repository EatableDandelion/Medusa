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
	
	
	
	
	
	
	
	
	RenderingPass::RenderingPass(const std::string& shaderName, const std::shared_ptr<IPassSettings> settings):shaderName(shaderName), settings(settings)
	{
		CIRCE_INFO("Initializing rendering pass.");
	}
	
	RenderingPass::RenderingPass(const Shader& shader, const std::shared_ptr<IPassSettings> settings):m_shader(shader), settings(settings)
	{
		CIRCE_INFO("Initializing rendering pass.");
	}
	
	RenderingPass::~RenderingPass()
	{
		CIRCE_INFO("Terminating rendering pass.");
	}
	
	void RenderingPass::updateEntity(std::shared_ptr<EntityData>& entity, const Camera& camera)
	{}
	
	void RenderingPass::renderAll(const Camera& camera)
	{
		if(active)
		{
			render(camera);
		}
		
		if(std::shared_ptr<RenderingPass> next = m_next.lock())
		{
			next->renderAll(camera);
		}
	}
	
	void RenderingPass::render(const std::shared_ptr<EntityData>& entity)
	{
		m_shader->update(entity->getMaterial());
		entity->draw(1);
	}
	
	void RenderingPass::render(const Camera& camera)
	{
		bind();
		
		for(std::shared_ptr<EntityData> entity : entities)
		{
			updateEntity(entity, camera);
			render(entity);
		}

		unbind();
	}
	
	void RenderingPass::setShader(const Shader& shader)
	{
		m_shader=shader;
	}
	
	void RenderingPass::init(const std::shared_ptr<Assets>& assets)
	{
		initAssets(assets);
		if(std::shared_ptr<RenderingPass> next = m_next.lock())
		{
			next->init(assets);
		}
	}
	
	void RenderingPass::initAssets(const std::shared_ptr<Assets>& assets)
	{
		m_shader = assets->getShader(shaderName);
		m_assets = assets;	
	}
	
	std::shared_ptr<Assets> RenderingPass::getAssets()
	{
		return m_assets;
	}
	
	void RenderingPass::addNext(const std::shared_ptr<RenderingPass> nextPass)
	{
		if(std::shared_ptr<RenderingPass> next = m_next.lock())
		{
			nextPass->addNext(next);
			m_next = nextPass;
		}
		else
		{
			m_next = nextPass;
		}
	}
	
	void RenderingPass::setActive(const bool& isActive)
	{
		active = isActive;
	}
	
	void RenderingPass::bind()
	{
		m_shader->bind();
		settings->start();
	}		
	
	void RenderingPass::unbind()
	{
		settings->end();
	}
			
			
	
	
	GeometryPass::GeometryPass():RenderingPass("GeometryPass", std::make_shared<GLPassSettings>(false, true, true))
	{}
	
	void GeometryPass::updateEntity(std::shared_ptr<EntityData>& entity, const Camera& camera)
	{
		entity->updateModel();
		entity->updateMVP(camera.getProjectionMatrix()*camera.getViewMatrix());
	}
	
	RenderingEntity GeometryPass::addEntity(const std::string& mesh, const std::string& texture)
	{
		RenderingEntity entity = RenderingPass::createEntity<RenderingEntity>(RenderingPass::getAssets()->getMesh(mesh, Medusa::TRIANGLE_RENDERING));
		entity.setTexture(TextureType::DIFFUSE0, RenderingPass::getAssets()->getTexture(texture));
		return entity;
	}




	DebugPass::DebugPass():RenderingPass("DebugDisplay", std::make_shared<GLPassSettings>(false, true, false))
	{
		
	}
			
	void DebugPass::updateEntity(std::shared_ptr<EntityData>& entity, const Camera& camera)
	{
		entity->setUniform<Circe::Vec3>("objectColor", Circe::Vec3(0.5f, 1.0f, 1.0f));
		geometryPass.updateEntity(entity, camera);
	}
	
	RenderingEntity DebugPass::addEntity(const std::string& mesh)
	{
		return RenderingPass::createEntity<RenderingEntity>(RenderingPass::getAssets()->getMesh(mesh, Medusa::WIRE_RENDERING));
	}
	
	void DebugPass::setLineThickness(const float& thickness)
	{
		settings->setLineThickness(thickness);
	}
}
