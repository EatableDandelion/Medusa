#pragma once

#include <dirent.h>
#include <sys/types.h>
#include "Resource.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

namespace Medusa
{
	class Assets
	{
		public:
			Assets(const VertexSpecs& specs = {Attribute("position", 0, 3), Attribute("normal", 1, 3), Attribute("textCoords", 2, 2)});
		
			~Assets();
			
			void loadDirectory(const char* name);
			
			void loadMesh(const std::string& folder, const std::string& name);
			
			void loadTexture(const std::string& folder, const std::string& name);
			
			void loadShader(const std::string& folder, const std::string& name);
			
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