#include "RenderingEntity.h"
#include <algorithm>

namespace Medusa
{	
	
	/*SpriteComponent::SpriteComponent()
	{
		
	}
	
	SpriteComponent::~SpriteComponent()
	{
		
	}
	
	void SpriteComponent::update(World& world, EntityHandle& entity)
	{
		
	}*/
	
	EntityData::EntityData(const Mesh& mesh): Entity(), m_mesh(mesh), visible(true)
	{
		material = std::make_shared<Material>();
	}

	EntityData::EntityData(const EntityData& other):Entity(other), m_mesh(other.m_mesh), material(other.material), visible(other.visible)
	{}
	
	void EntityData::updateModel()
	{
		setUniform("Model", Entity::getTransformMatrix());
	}
	
	void EntityData::updateMVP(const Circe::Mat<4>& viewProjection)
	{
		setUniform("MVP", viewProjection*Entity::getTransformMatrix());		
	}
	
	void EntityData::draw(const int& culling)
	{
		if(visible)
		{
			m_mesh.draw(culling);
		}
	}
	
	void EntityData::setTexture(const TextureType& type, const Texture& texture)
	{
		material->setTexture(type, texture);
	} 
	
	std::shared_ptr<Material> EntityData::getMaterial() const
	{ 
		return material;
	}
	
	void EntityData::setVisibility(const bool& visibility)
	{
		visible = visibility;
	}
	 
	
	RenderingHandler::RenderingHandler(const std::shared_ptr<EntityData> entityData):entity(entityData)
	{}
	
	RenderingHandler::RenderingHandler(const RenderingHandler& other):entity(other.entity)
	{}
			
	void RenderingHandler::draw(const int& culling)
	{
		entity->draw(culling);
	}
	
	void RenderingHandler::updateModel()
	{
		entity->updateModel();
	}
	
	void RenderingHandler::updateMVP(const Circe::Mat<4>& viewProjection)
	{
		entity->updateMVP(viewProjection);
	}
	
	void RenderingHandler::setTexture(const TextureType& type, const Texture& texture)
	{
		entity->setTexture(type, texture);
	}
	
	std::shared_ptr<Material> RenderingHandler::getMaterial() const
	{
		return entity->getMaterial();
	}
	
	void RenderingHandler::setVisibility(const bool& visibility)
	{
		entity->setVisibility(visibility);
	}
	
	int RenderingHandler::getId() const
	{
		return entity->getId();
	}
	
	void RenderingHandler::setTransform(const Transform3& transform)
	{
		entity->setTransform(transform);
	}
	
	Direction3 RenderingHandler::getSize() const
	{
		return entity->getSize();
	}
	
	void RenderingHandler::setSize(const Vec3& size)
	{
		entity->setSize(size);
	}
	
	Position3 RenderingHandler::getPosition() const
	{
		return entity->getPosition();
	}
	
	void RenderingHandler::setPosition(const Vec3& position)
	{
		entity->setPosition(position);
	}
	
	void RenderingHandler::setRotation(const Vec3& leftAxis, const Vec3& fwdAxis)
	{
		entity->setRotation(leftAxis, fwdAxis);
	}
	
	void RenderingHandler::attachTo(const RenderingHandler& parentEntity)
	{
		entity->attachTo(parentEntity.entity);
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
	
	void EntityLoader::unload(RenderingHandler& entity)
	{}
}
