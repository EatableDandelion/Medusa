#include "Assets.h"

namespace Medusa
{
	Assets::Assets(const std::string& meshFolder, const std::string& textureFolder, const std::string& shaderFolder, const VertexSpecs& specs):shaderResources(shaderFolder), meshResources(meshFolder), textureResources(textureFolder), specs(specs)
	{}
	
	Assets::~Assets()
	{
		textureResources.unloadAll();
		shaderResources.unloadAll();
		meshResources.unloadAll();
	}
	
	void Assets::loadMesh(const std::string& name, const int& faceShading)
	{
		meshResources.load(name, faceShading);
	}
			
	void Assets::loadTexture(const std::string& name)
	{
		textureResources.load(name);
	}
	
	void Assets::loadShader(const std::string& name)
	{
		shaderResources.load(name, specs);
	}
	
	Mesh Assets::getMesh(const std::string& name, const MeshType& meshType) const
	{
		return meshResources.getResource(name, meshType);
	}
	
	Texture Assets::getTexture(const std::string& name) const
	{
		return textureResources.getResource(name);
	}
	
	Shader Assets::getShader(const std::string& name) const
	{
		return shaderResources.getResource(name);
	}
}