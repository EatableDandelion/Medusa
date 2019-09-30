#include "Camera.h"

namespace Medusa
{
	Camera::Camera(float width, float height, float nearField, float farField):width(width), height(height),nearField(nearField), farField(farField), m_transform(std::make_shared<Circe::Transform<3>>())
	{
	}
	
	Circe::Mat<4> Camera::getProjectionMatrix() const
	{
		return Circe::Mat44::orthoProjection(width, height, nearField, farField);
	}
	
	Circe::Mat<4> Camera::getViewMatrix() const
	{
		return m_transform->getTransformMatrix();
	}
		
	void Camera::update(const int windowWidth, const int windowHeight)
	{
		float ratio = windowWidth / (float) windowHeight;
		height=width/ratio;
	}

	std::shared_ptr<Circe::Transform<3>> Camera::getTransform()
	{
		return m_transform;
	}
	
	void Camera::translate(const float& x, const float& y, const float& z)
	{
		m_transform->translate(Circe::Vec3(x, y, z));
	}
			
	void Camera::rotate(const float& xRot, const float& yRot, const float& zRot)
	{
		m_transform->rotate(xRot, yRot, zRot);
	}
}
