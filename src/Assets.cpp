#include "Assets.h"

namespace Medusa
{
	Assets::Assets(const VertexSpecs& specs):specs(specs)
	{}
	
	Assets::~Assets()
	{
		textureResources.unloadAll();
		shaderResources.unloadAll();
		meshResources.unloadAll();
	}
	
	void Assets::loadDirectory(const char* directory)
	{
		struct dirent *entry;
		DIR *dir = opendir(directory);
		while((entry = readdir(dir)) != NULL)
		{
			std::string fileName = entry->d_name;
			std::size_t dotPos = fileName.find(".");
			
			if(dotPos == std::string::npos) 
			{//No extension, assume it's a directory
				char subDir[32];
				strcpy(subDir, directory);
				strcat(subDir, fileName.c_str());
				strcat(subDir, "/");
				loadDirectory(subDir);
			}
			else 
			{//else it's a file
				std::string extension = fileName.substr(dotPos+1, fileName.length());			
				if(extension == "obj")
				{
					loadMesh(directory, fileName);
				}
				else if(extension == "png" || extension == "jpg" || extension == "jpeg")
				{
					loadTexture(directory, fileName);
				}
				else if(extension == "vs")
				{
					loadShader(directory, fileName.substr(0, dotPos));
				}
			}
		
		}
		closedir(dir);
	}
	
	void Assets::loadMesh(const std::string& folder, const std::string& name)
	{
		meshResources.load(folder, name);
	}
			
	void Assets::loadTexture(const std::string& folder, const std::string& name)
	{
		textureResources.load(folder, name);
	}
	
	void Assets::loadShader(const std::string& folder, const std::string& name)
	{
		shaderResources.load(folder, name, specs);
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