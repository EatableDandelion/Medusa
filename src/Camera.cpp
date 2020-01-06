#include "Camera.h"

namespace Medusa
{
	Camera::Camera(float width, float height, float nearField, float farField):width(width), height(height),nearField(nearField), farField(farField)
	{
		//position = Circe::Vec<3>(0,0,0);
		m_transform.setFrameRotation(Circe::Vec3(1,0,0), Circe::Vec3(0,1,0));
	}
	
	Circe::Mat<4> Camera::getProjectionMatrix() const
	{
		//return Circe::Mat44::orthoProjection(width, height, nearField, farField);
		return Circe::Mat44::perspectiveProjection(70, width/height, 0.01f, 1000.0f);
	}
	
	Circe::Mat<4> Camera::getViewMatrix() const
	{
		Circe::Mat<4> rotationMatrix = Circe::Mat44::rotationMatrix(m_transform.getFrameRotation().getConjugate());
		Circe::Mat<4> positionMatrix = Circe::Mat44::positionMatrix(m_transform.getFramePosition()*(-1.0f));

		return rotationMatrix*positionMatrix;
	}
		
	void Camera::update(const int windowWidth, const int windowHeight)
	{
		float ratio = windowWidth / (float) windowHeight;
		height=width/ratio;
	}
	
	void Camera::translate(const float& x, const float& y, const float& z)
	{
		m_transform.translate(Circe::Direction3(Circe::REF_FRAME::LOCAL, x, y, z));
	}
			
	void Camera::rotate(const float& xRot, const float& yRot, const float& zRot)
	{
		m_transform.rotate(Circe::Direction3(Circe::REF_FRAME::GLOBAL, xRot, yRot, zRot));
	}
}
