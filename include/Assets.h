#pragma once

#include "Resource.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

namespace Medusa
{
	class Assets
	{
		public:
			Assets(const std::string& meshFolder, const std::string& textureFolder, const std::string& shaderFolder, const VertexSpecs& specs);
		
			~Assets();
		
			void loadMesh(const std::string& name, const int& faceShading);
			
			void loadTexture(const std::string& name);
			
			void loadShader(const std::string& name);
			
			Mesh getMesh(const std::string& name, const MeshType& meshType) const;
			
			Texture getTexture(const std::string& name) const;
			
			Shader getShader(const std::string& name) const;
		
		private:
			ResourceManager<Mesh, MeshData, OBJLoader> meshResources;
			ResourceManager<Texture, TextureData, ImageTextureLoader> textureResources;
			ResourceManager<Shader, ShaderData, ShaderLoader> shaderResources;
			VertexSpecs specs;
	};
}