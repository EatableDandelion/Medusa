#include "RenderingEntity.h"
#include <algorithm>

namespace Medusa
{	
	int RenderingEntity::allid=0;
	
	RenderingEntity::RenderingEntity(const Mesh& mesh, const std::shared_ptr<Transform<3>>& transform):m_mesh(mesh), m_transform(weak_ptr<Transform<3>>(transform)), material(make_shared<Material>()), id(allid++), visible(true)
	{
		CIRCE_INFO("Initializing entity "+std::to_string(id));
	}
	
	RenderingEntity::RenderingEntity(const Mesh& mesh): m_mesh(mesh), material(make_shared<Material>()), m_transform(std::make_shared<Circe::Transform<3>>()), id(allid++), visible(true)
	{
		CIRCE_INFO("Initializing entity "+std::to_string(id));
	}
	
	RenderingEntity::~RenderingEntity()
	{
		CIRCE_INFO("Terminating entity "+std::to_string(id));
	}
	
	void RenderingEntity::updateModel()
	{
		setUniform("Model", getTransformMatrix());
	}
	
	void RenderingEntity::updateMVP(const Circe::Mat<4>& viewProjection)
	{
		setUniform("MVP", viewProjection*getTransformMatrix());		
	}
	
	void RenderingEntity::draw(const int& culling)
	{
		if(visible)
		{
			m_mesh.draw(culling);
		}
	}
	
	void RenderingEntity::setTexture(const TextureType& type, const Texture& texture)
	{
		material->setTexture(type, texture);
	}
	
	std::shared_ptr<Material> RenderingEntity::getMaterial() const
	{
		return material;
	}
	
	void RenderingEntity::setVisibility(const bool& visibility)
	{
		visible = visibility;
	}
	
	Mat<4> RenderingEntity::getTransformMatrix() const
	{
		if(auto transform = m_transform.lock())
		{
			return transform->getTransformMatrix();
		}
		else
		{
			return Mat<4>();
		}
	}
	
	void RenderingEntity::attachTo(const std::shared_ptr<Transform<3>>& parentTransform)
	{
		if(auto transform = m_transform.lock())
		{
			transform->attachTo(parentTransform);
		}
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
