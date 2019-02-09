#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include "RenderingEntity.h"
#include "Window.h"
#include "Resource.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderingPass.h"
#include <Circe/MathObj.h>
#include "Input.h"


namespace Medusa
{
	class RenderingEngine
	{
		public:
			RenderingEngine(const int& windowWidth, const int& windowHeight, const VertexSpecs& specs = {Attribute("position", 0, 3), Attribute("normal", 1, 3), Attribute("textCoords", 2, 2)});
			
			~RenderingEngine();
			
			void draw();
		
			void addEntity(const std::string& meshName, const std::string& textureName, const std::shared_ptr<ITransform>& transform, const int& faceOrientation);
			
			bool shouldCloseWindow() const;
			
			Mouse& getMouse();
			
			Keyboard& getKeyboard();

		private:
			vector<shared_ptr<RenderingEntity>> entities;
			Window<GLFWWindow> m_window;
			ResourceManager<Shader, ShaderData, ShaderLoader> shaderResources;
			ResourceManager<Mesh, MeshData, MeshLoader> meshResources;
			TextureManager textureResources;
			Camera camera;
			RenderingPass pass;
			FrameBuffer framebuffer;
			std::shared_ptr<Shader> screenShader;
			std::shared_ptr<Mesh> screenMesh;
			
			void initScreen();
			
			void renderScreen();
	};
}
