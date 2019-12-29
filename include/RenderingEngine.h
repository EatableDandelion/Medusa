#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include "RenderingEntity.h"
#include "Resource.h"
#include "Assets.h"
#include "RenderingPass.h"
#include <Circe/MathObj.h>
#include "Input.h"
#include "Lights.h"

namespace Medusa
{	
	class RenderingEngine
	{
		public:
			RenderingEngine(const int& windowWidth, const int& windowHeight);
			
			~RenderingEngine();
			
			void init(const std::shared_ptr<IRenderingPass> geoPass, const std::shared_ptr<IRenderingPass> ppPass, const std::shared_ptr<Assets> assets);
			
			void draw(const int& width, const int& height);
			
			Camera& getCamera();

		private:
			Camera camera;
			FrameBuffer framebuffer;
			std::shared_ptr<IRenderingPass> geometryPass;
			std::shared_ptr<IRenderingPass> postProcessPass;
	};
}
