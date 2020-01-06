#include "Lights.h"

namespace Medusa
{		
	Light::Light(const std::shared_ptr<EntityData> data, const float& intensity, const Circe::Vec3& color):RenderingEntity(data), color(color)
	{
		setColor(color);
		setIntensity(intensity);		
	}
			
	void Light::setIntensity(const float& intensity)
	{
		Circe::normalize(color);
		color *= intensity;
		RenderingEntity::setUniform<Circe::Vec3>("lightColor", color);
	}
			
	void Light::setColor(const Circe::Vec3& newColor)
	{
		float intensity = Circe::length(color);
		color = newColor;
		Circe::normalize(color);
		color *= intensity;
		RenderingEntity::setUniform<Circe::Vec3>("lightColor", color);	
	}
	
	
	

	DirectionalLight::DirectionalLight(const std::shared_ptr<EntityData> data, const float& intensity, const Circe::Vec3& color, const Circe::Vec3& direction):Light(data, intensity, color)
	{
		setDirection(direction);
	}
	
	void DirectionalLight::setDirection(const Circe::Vec3& direction)
	{
		RenderingEntity::setUniform<Circe::Vec3>("lightDirection", direction);
	}
	
	
	
	AmbientLight::AmbientLight(const std::shared_ptr<EntityData> data, const float& intensity, const Circe::Vec3& color):Light(data, intensity, color)
	{}
	
	DirectionalLightPass::DirectionalLightPass():RenderingPass("DirectionalLight", std::make_shared<GLPassSettings>(true, false, false))
	{}
	
	DirectionalLight DirectionalLightPass::addEntity(const float& intensity, const Circe::Vec3& color, const Circe::Vec3& direction)
	{
		Mesh screenMesh = RenderingPass::getAssets()->getMesh("plane.obj", Medusa::TRIANGLE_RENDERING);
		return RenderingPass::createEntity<DirectionalLight>(screenMesh, intensity, color, direction);
	}
	
	AmbientLightPass::AmbientLightPass():RenderingPass("AmbientLight", std::make_shared<GLPassSettings>(true, false, false))
	{}

	AmbientLight AmbientLightPass::addEntity(const float& intensity, const Circe::Vec3& color)
	{
		Mesh screenMesh = RenderingPass::getAssets()->getMesh("plane.obj", Medusa::TRIANGLE_RENDERING);
		return RenderingPass::createEntity<AmbientLight>(screenMesh, intensity, color);
	}
}
