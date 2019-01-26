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


namespace Medusa
{//TODO: debug renderer with service locator
	class RenderingEngine
	{
		public:
			RenderingEngine(const VertexSpecs& specs = {Attribute("position", 0, 3), Attribute("normal", 1, 3), Attribute("textCoords", 2, 2)});
			
			~RenderingEngine();
			
			void draw();
		
			void addEntity(const std::string& meshName, const std::string& textureName, ITransform* transform);
			
			bool shouldCloseWindow() const;
			
			//TEMP
			//void initScreen();
			
			//TEMP
			//void renderScreen();

		private:
			vector<shared_ptr<RenderingEntity>> entities;
			Window<GLFWWindow> m_window;
			ResourceManager<Shader, ShaderData, ShaderLoader> shaderResources;
			ResourceManager<Mesh, MeshData, MeshLoader> meshResources;
			//ResourceManager<Texture, TextureData, TextureLoader> textureResources;
			TextureManager textureResources;
			Camera camera;
			RenderingPass pass;
			/*std::shared_ptr<FrameBuffer> framebuffer;
			std::shared_ptr<Shader> screenShader;
			std::shared_ptr<Mesh> screenMesh;*/
	};
}
