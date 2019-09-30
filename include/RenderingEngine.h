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
		
			void addEntity(const std::string& meshName, const std::string& textureName, const std::shared_ptr<ITransform>& transform);
			
			bool shouldCloseWindow() const;
			
			Mouse& getMouse();
			
			Keyboard& getKeyboard();
			
			Camera& getCamera();

		private:
			vector<shared_ptr<RenderingEntity>> entities;
			Window<GLFWWindow> m_window;
			ResourceManager<Mesh, MeshData, OBJLoader> meshResources;
			ResourceManager<Texture, TextureData, ImageTextureLoader> textureResources;
			ResourceManager<Shader, ShaderData, ShaderLoader> shaderResources;
			Camera camera;
			RenderingPass pass;
			FrameBuffer framebuffer;
			Shader screenShader;
			std::shared_ptr<Mesh> screenMesh;
			std::shared_ptr<Material> screenMaterial;
			
			void initScreen();
			
			void renderScreen();
	};
}
