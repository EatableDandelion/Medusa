#include "Lights.h"

namespace Medusa
{
	
	
	Light::Light(const float& intensity, const Circe::Vec3& color):color(color)
	{
		setIntensity(intensity);
	}
			
	void Light::setIntensity(const float& intensity)
	{
		Circe::normalize(color);
		color *= intensity;
	}
			
	Circe::Vec3 Light::getColor() const
	{
		return color;
	}

	DirectionalLight::DirectionalLight(const float& intensity, const Circe::Vec3& color, const Circe::Vec3& direction):Light(intensity, color), direction(direction)
	{}
		
	Circe::Vec3 DirectionalLight::getDirection() const
	{
		return direction;
	}
	
	void DirectionalLight::update(RenderingEntity& screenEntity)
	{
		screenEntity.setUniform<Circe::Vec3>("lightDirection", direction);
		screenEntity.setUniform<Circe::Vec3>("lightColor", color);
	}
			
	
	AmbientLight::AmbientLight(const float& intensity, const Circe::Vec3& color):Light(intensity, color)
	{}
	
	void AmbientLight::update(RenderingEntity& screenEntity)
	{
		screenEntity.setUniform<Circe::Vec3>("lightColor", color);
	}
}
