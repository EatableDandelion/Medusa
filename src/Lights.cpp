#include "Lights.h"

namespace Medusa
{		
	Light::Light(const Mesh& mesh, const float& intensity, const Circe::Vec3& color):RenderingEntity(mesh), color(color)
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
	
	
	

	DirectionalLight::DirectionalLight(const Mesh& mesh, const float& intensity, const Circe::Vec3& color, const Circe::Vec3& direction):Light(mesh, intensity, color)
	{
		setDirection(direction);
	}
	
	void DirectionalLight::setDirection(const Circe::Vec3& direction)
	{
		RenderingEntity::setUniform<Circe::Vec3>("lightDirection", direction);
	}
	
	
	
	AmbientLight::AmbientLight(const Mesh& mesh, const float& intensity, const Circe::Vec3& color):Light(mesh, intensity, color)
	{}
	/*
	PointLight::PointLight(const Mesh& mesh, const float& intensity, const Circe::Vec3& color):Light(mesh, intensity, color)
	{}*/
	
	DirectionalLightPass::DirectionalLightPass():RenderingPass("DirectionalLight", true, false, false)
	{}
	
	/*void DirectionalLightPass::bind()
	{
		glDepthMask(false);
		glDisable(GL_DEPTH_TEST);	
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}*/
	
	void DirectionalLightPass::updateEntity(std::shared_ptr<DirectionalLight>& entity, const Camera& camera)
	{}
	
	/*void DirectionalLightPass::unbind()
	{
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);	
	}*/
	
	void DirectionalLightPass::addEntity(const float& intensity, const Circe::Vec3& color, const Circe::Vec3& direction)
	{
		Mesh screenMesh = RenderingPass::getAssets()->getMesh("plane.obj", Medusa::TRIANGLE_RENDERING);
		RenderingPass::createEntity(screenMesh, intensity, color, direction);
	}
	
	AmbientLightPass::AmbientLightPass():RenderingPass("AmbientLight", true, false, false)
	{}
	
	/*void AmbientLightPass::bind()
	{
		glDepthMask(false);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}*/

	void AmbientLightPass::updateEntity(std::shared_ptr<AmbientLight>& entity, const Camera& camera)
	{}
	
	/*void AmbientLightPass::unbind()
	{
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}*/

	void AmbientLightPass::addEntity(const float& intensity, const Circe::Vec3& color)
	{
		Mesh screenMesh = RenderingPass::getAssets()->getMesh("plane.obj", Medusa::TRIANGLE_RENDERING);
		RenderingPass::createEntity(screenMesh, intensity, color);
	}
}
