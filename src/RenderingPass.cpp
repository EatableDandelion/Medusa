#include "RenderingPass.h"

#include <iostream>

namespace Medusa{
	
	GeometryPass::GeometryPass():RenderingPass("GeometryPass")
	{}
	
	void GeometryPass::bind()
	{
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
	
	void GeometryPass::updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera)
	{
		entity->updateModel();
		entity->updateMVP(camera.getProjectionMatrix()*camera.getViewMatrix());
	}
	
	void GeometryPass::unbind()
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(false);
	}
	
	void GeometryPass::addEntity(const Mesh& mesh, const Texture& texture, const std::shared_ptr<Transform<3>>& transform)
	{
		std::shared_ptr<RenderingEntity> entity = RenderingPass::createEntity(mesh, transform);
		entity->setTexture(TextureType::DIFFUSE0, texture);
	}

	DebugPass::DebugPass():RenderingPass("DebugDisplay")
	{}
	
	void DebugPass::bind()
	{
		glDepthMask(false);
	}
			
	void DebugPass::updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera)
	{
		entity->setUniform<Circe::Vec3>("objectColor", Circe::Vec3(0.5f, 1.0f, 1.0f));
		geometryPass.updateEntity(entity, camera);
	}
	
	void DebugPass::unbind()
	{
		glDepthMask(true);
	}
	
	void DebugPass::addEntity(const Mesh& mesh, const std::shared_ptr<Transform<3>>& transform)
	{
		RenderingPass::createEntity(mesh, transform);
	}
}
