#include "Camera.h"

namespace Medusa
{
	Camera::Camera(float width, float height, float nearField, float farField):width(width), height(height),nearField(nearField), farField(farField)	
	{
	}
	
	Circe::Mat<4> Camera::getProjectionMatrix() const
	{
		return Circe::Mat44::orthoProjection(width, height, nearField, farField);
	}
	
	void Camera::update(const int windowWidth, const int windowHeight)
	{
		float ratio = windowWidth / (float) windowHeight;
		height=width/ratio;
	}

}
