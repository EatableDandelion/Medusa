#include "HUD.h"

namespace Medusa
{
	
	HUDComponent::HUDComponent(const Mesh& mesh, const shared_ptr<Transform<3>>& transform):RenderingEntity(mesh, transform)	
	{
		RenderingEntity::setUniform<Circe::Vec2>("textOffset", Circe::Vec2(0.0f, 0.0f));
		RenderingEntity::setUniform<Circe::Vec2>("textSize", Circe::Vec2(1.0f, 1.0f));
	}
	
	HUDLabel::HUDLabel(const Mesh& mesh, const shared_ptr<Transform<3>>& transform, const std::string& label, const int& maxNbChar):HUDComponent(mesh, transform), nbChar(maxNbChar), label(label), transform(transform)
	{
	
	}

	void HUDLabel::setLabel(const std::string& newLabel)
	{
		label = newLabel;
	}
	
	void HUDLabel::draw(const int& culling)
	{
		int n = label.length();
		float dOffset = 1.0f/(float)n;
		int i = 0;
		transform->setFrameScale(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, 0.1f, 0.1f, 1.0f));
		float totalTranslation = 0.0f;
		while(i<n)
		{
			RenderingEntity::setUniform<Circe::Vec2>("textOffset", Circe::Vec2(0.0f, 0.0f));
			RenderingEntity::setUniform<Circe::Vec2>("textSize", Circe::Vec2(1.0f, 1.0f));
			transform->translate(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, dOffset, 0.0f, 0.0f));
			totalTranslation+=dOffset;
			RenderingEntity::updateModel();//need to change the shader everytime!
			RenderingEntity::draw(culling);
			i++;
		}
		transform->translate(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, -totalTranslation, 0.0f, 0.0f));
	}
	
	
	HUDPass::HUDPass():RenderingPass("HUD")
	{}
	
	void HUDPass::bind()
	{
		glDepthMask(false);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
	}
	
	void HUDPass::updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera)
	{
		entity->updateModel();
	}
	
	void HUDPass::unbind()
	{
		glDepthFunc(GL_LESS);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(true);	
	}
	
	void HUDPass::addEntity(const Texture& texture, const shared_ptr<Transform<3>>& transform)
	{
		Mesh screenMesh = RenderingPass::getAssets()->getMesh("plane.obj", Medusa::TRIANGLE_RENDERING);
		std::shared_ptr<HUDComponent> entity = RenderingPass::createEntity<HUDComponent>(screenMesh, transform, "test", 4);
		entity->setTexture(TextureType::DIFFUSE0, texture);		
	}
	
	void HUDPass::addLabel(const Texture& texture, const shared_ptr<Transform<3>>& transform)
	{
		Mesh screenMesh = RenderingPass::getAssets()->getMesh("plane.obj", Medusa::TRIANGLE_RENDERING);
		std::shared_ptr<HUDComponent> entity = RenderingPass::createEntity<HUDLabel>(screenMesh, transform, "test", 4);
		entity->setTexture(TextureType::DIFFUSE0, texture);	
	}
	
}
