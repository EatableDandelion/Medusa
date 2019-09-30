#include "RenderingEntity.h"
#include <algorithm>

namespace Medusa
{	
	int RenderingEntity::allid=0;
	
	RenderingEntity::RenderingEntity(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<ITransform>& transform):m_mesh(mesh), m_transform(weak_ptr<ITransform>(transform)), material(make_shared<Material>()), id(allid++)
	{
		CIRCE_INFO("Initializing entity "+std::to_string(id));
	}
	
	RenderingEntity::~RenderingEntity()
	{
		CIRCE_INFO("Terminating entity "+std::to_string(id));
	}
	
	void RenderingEntity::render(Shader& renderingPass, Camera& camera)
	{
		
	}
	
	void RenderingEntity::draw(const int& culling)
	{
		m_mesh->draw(culling);
	}
	
	bool RenderingEntity::update(const Mat<4>& projectionMatrix, const Mat<4>& viewMatrix)
	{		
		if(auto transform = m_transform.lock()){
			Mat<4> m = transform->getTransformMatrix();
			Mat<4> mv = viewMatrix*m;
			setUniform("MVP", projectionMatrix*mv);
			setUniform("MV", mv);
			
			return true;
		}else{
			return false;
		}
	}
	
	void RenderingEntity::setTexture(const Texture& texture)
	{
		material->setTexture(texture);
	}
	
	std::shared_ptr<Material> RenderingEntity::getMaterial() const
	{
		return material;
	}


	void EntityLoader::load(const std::string& folderLocation, const std::string& fileName)//, RenderingEntity& entity)
	{
		ifstream file;
		file.open(folderLocation+fileName);
		std::string line;
		while(std::getline(file, line))
		{
			std::istringstream iss(line);
			std::vector<std::string> tokens((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
			std::string varValue = tokens[1];
			varValue.erase(remove(varValue.begin(), varValue.end(), '\"'), varValue.end());
			loadComponent(tokens[0], varValue);
		}
		file.close();
	}
	
	void EntityLoader::loadComponent(const std::string& type, const std::string& value)
	{
		std::cout << value << std::endl;
		if(type.find("MESH") != std::string::npos)
		{
			
		}
		else if(type.find("TEXTURE") != std::string::npos)
		{
			int slot = 0;
			if(type.find("DIFFUSE") != std::string::npos)
			{
				slot = 0;
			}
			else if(type.find("NORMAL") != std::string::npos)
			{
				slot = 1;
			}
			
		}
		else
		{
			CIRCE_ERROR(type+" not recognized.");
		}
	}
	
	void EntityLoader::unload(RenderingEntity& entity)
	{
		
	}
}
