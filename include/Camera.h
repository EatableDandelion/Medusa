#pragma once

#include "memory"
#include <Circe/Circe.h>

namespace Medusa
{
	class Camera
	{
		public:
			Camera(float width, float height, float nearField=-0.01f, float farField=1000.0f);
			
			Circe::Mat<4> getProjectionMatrix() const;
			
			Circe::Mat<4> getViewMatrix() const;
			
			void update(const int windowWidth, const int windowHeight);
			
			void translate(const Circe::REF_FRAME& frame, const float& x, const float& y, const float& z);
			
			void rotate(const float& xRot, const float& yRot, const float& zRot);
			
			//std::shared_ptr<Circe::Transform<3>> getTransform();

		private:
			float nearField, farField, width, height;
			std::shared_ptr<Circe::Transform<3>> m_transform;
	};
}