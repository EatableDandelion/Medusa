#pragma once
#include <Circe/MathObj.h>

namespace Medusa
{
	class Camera
	{
		public:
			Camera(float width, float height, float nearField=-0.01f, float farField=1000.0f);
			
			Circe::Mat<4> getProjectionMatrix() const;
			
			void update(const int windowWidth, const int windowHeight);

		private:
			float nearField, farField, width, height;
	};
}