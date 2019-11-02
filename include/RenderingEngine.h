#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include "RenderingEntity.h"
#include "Window.h"
#include "Resource.h"
#include "Assets.h"
#include "RenderingPass.h"
#include <Circe/MathObj.h>
#include "Input.h"
#include "Lights.h"
#include "HUD.h"


namespace Medusa
{	
	class RenderingEngine
	{
		public:
			RenderingEngine(const int& windowWidth, const int& windowHeight, const VertexSpecs& specs = {Attribute("position", 0, 3), Attribute("normal", 1, 3), Attribute("textCoords", 2, 2)});
			
			~RenderingEngine();
			
			void draw();
		
			void addWorldEntity(const std::string& meshName, const std::string& textureName, const std::shared_ptr<Transform<3>>& transform);
			
			void addDebugEntity(const std::string& meshName, const std::shared_ptr<Transform<3>>& transform);
			
			void addHUDEntity(const std::shared_ptr<Transform<3>> transform, const std::string& texture);
			
			bool shouldCloseWindow() const;
			
			Mouse& getMouse();
			
			Keyboard& getKeyboard();
			
			Camera& getCamera();

		private:
			std::shared_ptr<Assets> assets;
			Window<GLFWWindow> m_window;
			Camera camera;
			FrameBuffer framebuffer;
			GeometryPass geometryPass;
			DebugPass debugPass;
			AmbientLightPass ambientLights;
			DirectionalLightPass directionalLights;	
			HUDPass huds;
	};
}
